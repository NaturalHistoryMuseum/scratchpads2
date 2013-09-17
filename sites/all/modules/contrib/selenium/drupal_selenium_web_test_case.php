<?php

/**
 * @file
 * Classes for interacting with Selenium.
 */

// Server details constant.
define('SELENIUM_SERVER_URL', 'http://' . variable_get('selenium_server_host', 'localhost:4444') . "/wd/hub");

/**
 * Test case for Selenium test.
 */
abstract class DrupalSeleniumWebTestCase extends DrupalWebTestCase {

  /**
   * Selenium Web Driver instance.
   *
   * @var SeleniumWebDriver
   */
  protected $driver;

  /**
   * Browser that runs test.
   */
  protected $browser;

  /**
   * Allowed driver types.
   */
  protected $allowed_browsers = array('firefox', 'chrome', 'opera', 'iexplorer');

  /**
   * Special switcher for testing on original site (ie NOT ON SIMPLETEST SANDBOX).
   * Tests are performed via standard Simpletest interface but not using its sandbox
   * - using the original site.
   * It is useful for some browsers, which can't override User-Agent string (it
   * means can't redirect to a sandbox).
   * To use it set this variable to TRUE on setUp() before parent::setUp().
   * Please note: use this possibility with caution since it can damage your
   * original site. Never run it on production sites and always make backup
   * before launching.
   */
  public $onOriginal = FALSE;

  /**
   * Database prefix for Simpletest sandbox.
   */
  protected $sandboxDatabasePrefix;

  /**
   * Database prefix for original site.
   */
  protected $originalDatabasePrefix;

  protected function setUp() {
    // Backward compatibility together with support of new way of passing modules parameter.
    // @see DrupalWebTestCase::setUp()
    $modules = func_get_args();
    if (isset($modules[0]) && is_array($modules[0])) {
      $modules = $modules[0];
    }
    parent::setUp($modules);
    // Determine which test to run.
    // By default we run Firefox.
    $browser = 'firefox';
    if (in_array($this->browser, $this->allowed_browsers)) {
      $browser = $this->browser;
    }
    $this->driver = $this->seleniumDriver($browser);
    // Determine whether we should run on Simpletest sandbox or original site.
    if ($this->onOriginal) {
      // Unset database prefix to run all tests on the original site.
      // It is a workaround to make it possible to run Selenium-tests via
      // browsers, that don't support user-agent setting.
      // @see http://drupal.org/node/1874076
      // 0) Preparations: initialize the helper variables.
      $this->sandboxDatabasePrefix = $this->databasePrefix;
      $this->originalDatabasePrefix = NULL;
      // 1) First create a backup of sandbox connection to make it possible to switch back later.
      Database::renameConnection('default', 'simpletest_sandbox');
      $connection_info = Database::getConnectionInfo('simpletest_sandbox');
      Database::addConnectionInfo('default', 'default', $connection_info['default']);
      // 2) Switch connection to original site.
      $this->switchConnectionTo('simpletest_original_default');
    }
  }

  /**
   * Run all tests in this class.
   *
   * Regardless of whether $methods are passed or not, only method names
   * starting with "test" are executed.
   *
   * @param $methods
   *   (optional) A list of method names in the test case class to run; e.g.,
   *   array('testFoo', 'testBar'). By default, all methods of the class are
   *   taken into account, but it can be useful to only run a few selected test
   *   methods during debugging.
   */
  public function run(array $methods = array()) {
    // Initialize verbose debugging.
    simpletest_verbose(NULL, variable_get('file_public_path', conf_path() . '/files'), get_class($this));

    // HTTP auth settings (<username>:<password>) for the simpletest browser
    // when sending requests to the test site.
    $this->httpauth_method = variable_get('simpletest_httpauth_method', CURLAUTH_BASIC);
    $username = variable_get('simpletest_httpauth_username', NULL);
    $password = variable_get('simpletest_httpauth_password', NULL);
    if ($username && $password) {
      $this->httpauth_credentials = $username . ':' . $password;
    }

    set_error_handler(array($this, 'errorHandler'));
    $class = get_class($this);
    // Iterate through all the methods in this class, unless a specific list of
    // methods to run was passed.
    $class_methods = get_class_methods($class);
    if ($methods) {
      $class_methods = array_intersect($class_methods, $methods);
    }
    foreach ($class_methods as $method) {
      // If the current method starts with "test", run it - it's a test.
      if (strtolower(substr($method, 0, 4)) == 'test') {
        // Get information about test case.
        $info = $this->getInfo();
        // If no browsers are set we use internal.
        if (!isset($info['browsers'])) {
           $info['browsers'] = array('internal');
        }

        // Insert a fail record. This will be deleted on completion to ensure
        // that testing completed.
        $method_info = new ReflectionMethod($class, $method);
        $caller = array(
          'file' => $method_info->getFileName(),
          'line' => $method_info->getStartLine(),
          'function' => $class . '->' . $method . '()',
        );

        // Run test in each browser.
        foreach ($info['browsers'] as $browser) {
          $this->browser = $browser;

          $completion_check_id = DrupalTestCase::insertAssert($this->testId, $class, FALSE, t('The test did not complete due to a fatal error.'), 'Completion check', $caller);
          try {
            $this->setUp();
            $this->$method();
            // Finish up.
          }
          catch (Exception $e) {
            $this->exceptionHandler($e);
          }
          $this->tearDown();
          // Remove the completion check record.
          DrupalTestCase::deleteAssert($completion_check_id);
        }
      }
    }
    // Clear out the error messages and restore error handler.
    drupal_get_messages();
    restore_error_handler();
  }

  /**
   * Init driver of specified type.
   *
   * @param string $browser
   *   Type of the driver.
   * @return object
   */
  protected function seleniumDriver($browser) {
    // We need to define user agent only for local tests, which manipulate database.
    // For remote tests it's not necessary.
    if (isset($this->databasePrefix)) {
      $test_id = $GLOBALS['drupal_test_info']['test_run_id'];
      if (preg_match('/simpletest\d+/', $test_id, $matches)) {
        $user_agent = drupal_generate_test_ua($matches[0]);
      }
      else {
        throw new Exception('Test is not ready to init connection to Webdriver (no database prefix)');
      }
    }
    else {
      $user_agent = '';
      $test_id = $this->testId;
    }
    switch ($browser) {
      case 'firefox':
        return new SeleniumFirefoxDriver($user_agent, $test_id);
      case 'chrome':
        return new SeleniumChromeDriver($user_agent, $test_id);
      case 'opera':
        return new SeleniumOperaDriver($user_agent, $test_id);
      case 'iexplorer':
        return new SeleniumIExplorerDriver($user_agent, $test_id);
    }
  }

  public function tearDown() {
    if ($this->onOriginal) {
      // 3) We must switch connection back to make simpletest to eliminate sandbox - not the original site.
      $this->switchConnectionTo('simpletest_sandbox');
    }
    parent::tearDown();
  }

  protected function switchConnectionTo($connection_name = 'simpletest_original_default') {
    if ($this->checkSameConnection($connection_name)) {
      return;
    }
    switch ($connection_name) {
      case 'simpletest_sandbox':
        $this->databasePrefix = $this->sandboxDatabasePrefix;
        break;
      case 'simpletest_original_default':
      default:
        $this->databasePrefix = $this->originalDatabasePrefix;
        break;
    }
    // Remove default.
    Database::removeConnection('default');
    // Add default as a copy of the connection, we need to switch to.
    $connection_info = Database::getConnectionInfo($connection_name);
    Database::addConnectionInfo('default', 'default', $connection_info['default']);
  }

  /**
   * Protection from switching to self. Helps to find logic errors.
   * @param string $connection_name
   * @return boolean
   */
  protected function checkSameConnection($connection_name = 'simpletest_original_default') {
    $same = FALSE;
    switch ($connection_name) {
      case 'simpletest_sandbox':
        $same = ($this->databasePrefix === $this->sandboxDatabasePrefix);
        break;
      case 'simpletest_original_default':
      default:
        $same = ($this->databasePrefix === $this->originalDatabasePrefix);
        break;
    }
    if ($same) {
      $this->fail("Already on $connection_name connection", 'Connection');
    }
    return $same;
  }

  /**
   * Open specific url.
   */
  protected function drupalGet($url) {
    $this->driver->openUrl($url);
    $this->verbose('GET request to: ' . $url .
                   '<hr />Ending URL: ' . $this->getUrl() .
                   '<hr />' . $this->drupalGetContent());
  }

  /**
   * Take a screenshot from current page.
   * Save it to verbose directory and add verbose message.
   */
  protected function verboseScreenshot() {
    // Take screenshot of current page.
    $screenshot = FALSE;
    try {
      $screenshot = $this->driver->getScreenshot();
    }
    catch (Exception $e) {
      $this->verbose(t('No support for screenshots in %driver', array('%driver' => get_class($this->driver))));
    }
    if ($screenshot) {
      // Prepare directory.
      $directory = $this->originalFileDirectory . '/simpletest/verbose/screenshots';
      $writable = file_prepare_directory($directory, FILE_CREATE_DIRECTORY);
      if ($writable) {
        $testname = $this->getTestName();
        // Trying to save screenshot to verbose directory.
        $file = file_unmanaged_save_data($screenshot, $this->originalFileDirectory . '/simpletest/verbose/screenshots/' . $testname . '.png', FILE_EXISTS_RENAME);

        // Adding verbose message with link to screenshot.
        $this->error(l(t('Screenshot created.'), $GLOBALS['base_url'] . '/' . $file, array('attributes' => array('target' => '_blank'))), 'User notice');
      }
    }
  }

  /**
   * Implements assertTextHelper.
   */
  protected function assertTextHelper($text, $message = '', $group, $not_exists) {
    $this->plainTextContent = filter_xss($this->driver->getBodyText(), array());

    // Remove all symbols of new line as we need raw text here.
    $this->plainTextContent = str_replace("\n", '', $this->plainTextContent);

    if (!$message) {
      $message = !$not_exists ? t('"@text" found', array('@text' => $text)) : t('"@text" not found', array('@text' => $text));
    }
    return $this->assert($not_exists == (strpos($this->plainTextContent, $text) === FALSE), $message, $group);
  }

  /**
   * Implements assertTitle.
   */
  protected function assertTitle($title, $message = '', $group = 'Other') {
    $actual = $this->driver->getPageTitle();
    if (!$message) {
      $message = t('Page title @actual is equal to @expected.', array(
        '@actual' => var_export($actual, TRUE),
        '@expected' => var_export($title, TRUE),
      ));
    }
    return $this->assertEqual($actual, $title, $message, $group);
  }

  /**
   * Asserts that a field exists with the given name or id.
   *
   * @param string $field
   *   Name or id of field to assert.
   * @param string $message
   *   Message to display.
   * @param string $group
   *   The group this message belongs to.
   * @return boolean
   *   TRUE on pass, FALSE on fail.
   */
  protected function assertField($field, $message = '', $group = 'Other') {
    try {
      $element = $this->driver->getElement("name=$field");
    } catch (Exception $e) {
      try {
        $element = $this->driver->getElement("id=$field");
      } catch (Exception $e) {
        $element = FALSE;
      }
    }
    return $this->assertTrue(!empty($element), $message ? $message : t('Field %locator found', array('%locator' => $field)), $group);
  }

  /**
    * Implements assertNoField.
    *
    * @param $field
    *   Name or id of field to assert.
    * @param $message
    *   Message to display.
    * @param $group
    *   The group this message belongs to.
    * @return
    *   TRUE on pass, FALSE on fail.
    */
   protected function assertNoField($field, $message = '', $group = 'Other') {
     try {
       $element = $this->driver->getElement("name=$field");
     }
     catch (Exception $e) {
       try {
         $element = $this->driver->getElement("id=$field");
       }
       catch (Exception $e) {
         $element = FALSE;
       }
    }
    return $this->assertTrue(empty($element), $message ? $message : t('Field %locator not found', array('%locator' => $field)), $group);
  }

  /**
   * Implements assertLink.
   */
  protected function assertLink($label, $index = 0, $message = '', $group = 'Other') {
    $links = $this->driver->waitForElements('link=' . $label);
    $message = ($message ?  $message : t('Link with label %label found.', array('%label' => $label)));
    return $this->assert(isset($links[$index]), $message, $group);
  }

  /**
   * Follows a link by name.
   *
   * Will click the first link found with this link text by default, or a
   * later one if an index is given. Match is case insensitive with
   * normalized space. The label is translated label. There is an assert
   * for successful click.
   *
   * @param $label
   *   Text between the anchor tags.
   * @param $index
   *   Link position counting from zero.
   * @return
   *   Page on success, or FALSE on failure.
   */
  protected function clickLink($label, $index = 0) {
    // Assert that link exists.
    if (!$this->assertLink($label, $index)) {
      return;
    }
    // Get link elements.
    $links = $this->driver->getAllElements('link=' . $label);
    $link_element = $links[$index];
    // Get current and target urls.
    $url_before = $this->getUrl();
    $url_target = $link_element->getAttributeValue('href');
    $this->assertTrue(isset($links[$index]), t('Clicked link %label (@url_target) from @url_before', array('%label' => $label, '@url_target' => $url_target, '@url_before' => $url_before)), t('Browser'));
    // Click on element;
    $link_element->click();
  }

  /**
   * Pass if a link with the specified label is found, and optional with the
   * specified index.
   *
   * @param $label
   *   Text between the anchor tags.
   * @param $index
   *   Link position counting from zero.
   * @param $message
   *   Message to display.
   * @param $group
   *   The group this message belongs to, defaults to 'Other'.
   * @return
   *   TRUE if the assertion succeeded, FALSE otherwise.
   */
  protected function assertNoLink($label, $index = 0, $message = '', $group = 'Other') {
    $links = $this->driver->waitForElements('link=' . $label);
    $message = ($message ?  $message : t('Link with label %label not found.', array('%label' => $label)));
    return $this->assert(!isset($links[$index]), $message, $group);
  }

  /**
   * Pass if a link containing a given href (part) is found.
   *
   * @param $href
   *   The full or partial value of the 'href' attribute of the anchor tag.
   * @param $index
   *   Link position counting from zero.
   * @param $message
   *   Message to display.
   * @param $group
   *   The group this message belongs to, defaults to 'Other'.
   *
   * @return
   *   TRUE if the assertion succeeded, FALSE otherwise.
   */
  protected function assertLinkByHref($href, $index = 0, $message = '', $group = 'Other') {
    $links = $this->driver->getAllElements("//a[contains(@href, '$href')]");
    $message = ($message ?  $message : t('Link containing href %href found.', array('%href' => $href)));
    return $this->assert(isset($links[$index]), $message, $group);
  }

  /**
   * Pass if a link containing a given href (part) is not found.
   *
   * @param $href
   *   The full or partial value of the 'href' attribute of the anchor tag.
   * @param $index
   *   Link position counting from zero.
   * @param $message
   *   Message to display.
   * @param $group
   *   The group this message belongs to, defaults to 'Other'.
   *
   * @return
   *   TRUE if the assertion succeeded, FALSE otherwise.
   */
  protected function assertNoLinkByHref($href, $index = 0, $message = '', $group = 'Other') {
    $links = $this->driver->getAllElements("//a[contains(@href, '$href')]");
    $message = ($message ?  $message : t('Link containing href %href not found.', array('%href' => $href)));
    return $this->assert(!isset($links[$index]), $message, $group);
  }

  /**
   * Implements assertOptionSelected.
   * Asserts that a select option in the current page is checked.
   *
   * @param string $locator
   * @param string $option
   *   Option to assert.
   * @param string $message
   *   Message to display.
   * @return boolean
   *   TRUE on pass, FALSE on fail.
   */
  protected function assertOptionSelected($locator, $option, $message = '') {
    $selected = FALSE;
    $element = $this->driver->getElement($locator);
    $is_select = $element && $element->getTagName() == 'select';
    if ($is_select) {
      $id = $element->getAttributeValue('id');
      $message = $message ? $message : t('Option @option for field @id is selected.', array('@option' => $option, '@id' => $id));
      $selected_options = $this->getSelectedItem($element);
      foreach ($selected_options as $selected_option) {
        if ($selected_option->getValue() == $option) {
          $selected = TRUE;
          break;
        }
      }
    }
    else {
      $message = t('There is no element with locator @locator or element is not select list.', array('@locator' => $locator));
    }
    return $this->assertTrue($is_select && $selected, $message, t('Browser'));
  }

  /**
   * Implements assertNoOptionSelected.
   * Asserts that a select option in the current page is not checked.
   *
   * @param string $locator
   * @param string $option
   *   Option to assert.
   * @param string $message
   *   Message to display.
   * @return boolean
   *   TRUE on pass, FALSE on fail.
   */
  protected function assertNoOptionSelected($locator, $option, $message = '') {
    $selected = FALSE;
    $element = $this->driver->getElement($locator);
    $is_select = $element && $element->getTagName() == 'select';
    if ($is_select) {
      $id = $element->getAttributeValue('id');
      $message = $message ? $message : t('Option @option for field @id is not selected.', array('@option' => $option, '@id' => $id));
      $selected_options = $this->getSelectedItem($element);
      foreach ($selected_options as $selected_option) {
        if ($selected_option->getValue() == $option) {
          $selected = TRUE;
          break;
        }
      }
    }
    else {
      $message = t('There is no element with locator @locator or element is not select list.', array('@locator' => $locator));
    }

    return $this->assertTrue($is_select && !$selected, $message, t('Browser'));
  }

  /**
   * Implements assertFieldChecked.
   * Asserts that a checkbox field in the current page is checked.
   *
   * @param string $locator
   * @param string $message
   *   Message to display.
   * @return boolean
   *   TRUE on pass, FALSE on fail.
   */
  protected function assertFieldChecked($locator, $message = '') {
    $element = $this->driver->getElement($locator);
    $is_checkbox = $element && ($element->getTagName() == 'checkbox' || $element->getAttributeValue('type') == 'checkbox');
    if ($is_checkbox) {
      $id = $element->getAttributeValue('id');
      $message = $message ? $message : t('Checkbox field @id is checked.', array('@id' => $id));
    }
    else {
      $message = t('There is no element with locator @locator or element is not checkbox.', array('@locator' => $locator));
    }

    return $this->assertTrue($is_checkbox && $element->isSelected() , $message, t('Browser'));
  }

  /**
   * Implements assertNoFieldChecked.
   * Asserts that a checkbox field in the current page is not checked.
   *
   * @param string $locator
   * @param string $message
   *   Message to display.
   * @return boolean
   *   TRUE on pass, FALSE on fail.
   */
  protected function assertNoFieldChecked($locator, $message = '') {
    $element = $this->driver->getElement($locator);
    $is_checkbox = $element && ($element->getTagName() == 'checkbox' || $element->getAttributeValue('type') == 'checkbox');
    if ($is_checkbox) {
      $id = $element->getAttributeValue('id');
      $message = $message ? $message : t('Checkbox field @id is not checked.', array('@id' => $id));
    }
    else {
      $message = t('There is no element with locator @locator or element is not checkbox.', array('@locator' => $locator));
    }

    return $this->assertTrue($is_checkbox && !$element->isSelected() , $message, t('Browser'));
  }

  /**
   * Asserts that each HTML ID is used for just a single element.
   *
   * @param $message
   *   Message to display.
   * @param $group
   *   The group this message belongs to.
   * @param $ids_to_skip
   *   An optional array of ids to skip when checking for duplicates. It is
   *   always a bug to have duplicate HTML IDs, so this parameter is to enable
   *   incremental fixing of core code. Whenever a test passes this parameter,
   *   it should add a "todo" comment above the call to this function explaining
   *   the legacy bug that the test wishes to ignore and including a link to an
   *   issue that is working to fix that legacy bug.
   * @return
   *   TRUE on pass, FALSE on fail.
   */
  protected function assertNoDuplicateIds($message = '', $group = 'Other', $ids_to_skip = array()) {
    try {
      $elements = $this->driver->getAllElements("//*[@id]");
      $status = TRUE;
      foreach ($elements as $element) {
        $id = (string) $element->getAttributeValue("id");
        if (isset($seen_ids[$id]) && !in_array($id, $ids_to_skip)) {
          $this->fail(t('The HTML ID %id is unique.', array('%id' => $id)), $group);
          $status = FALSE;
        }
        $seen_ids[$id] = TRUE;
      }
    }
    catch (Exception $e) {
      $status = FALSE;
    }
    return $this->assertTrue($status, $message ? $message : t('No Duplicate Ids'), $group);
  }

  /**
   * Asserts that a field exists in the current page with the given name and value.
   *
   * @param $name
   *   Name of field to assert.
   * @param $value
   *   Value of the field to assert.
   * @param $message
   *   Message to display.
   * @param $group
   *   The group this message belongs to.
   * @return
   *   TRUE on pass, FALSE on fail.
   */
  protected function assertFieldByName($name, $value = '', $message = '', $group = 'Other') {
    try {
      $element = $this->driver->getElement("name=$name");
      if ($value) {
        $element = $this->elementValue($element, $value);
      }
    }
    catch (Exception $e) {
      $element = FALSE;
    }
    return $this->assertTrue(!empty($element), $message ? $message : t('Field found by name'), $group);
  }

  /**
   * Asserts that a field not exists in the current page with the given name and value.
   *
   * @param $name
   *   Name of field to assert.
   * @param $value
   *   Value of the field to assert.
   * @param $message
   *   Message to display.
   * @param $group
   *   The group this message belongs to.
   * @return
   *   TRUE on pass, FALSE on fail.
   */
  protected function assertNoFieldByName($name, $value = '', $message = '', $group = 'Other') {
    try {
      $element = $this->driver->getElement("name=$name");
      if ($value) {
        $element = $this->elementValue($element, $value);
      }
    }
    catch (Exception $e) {
      $element = FALSE;
    }
    return $this->assertTrue(empty($element), $message ? $message : t('Field found by name'), $group);
  }

  /**
   * Asserts that a field exists in the current page with the given id and value.
   *
   * @param $id
   *   Id of field to assert.
   * @param $value
   *   Value of the field to assert.
   * @param $message
   *   Message to display.
   * @param $group
   *   The group this message belongs to.
   * @return
   *   TRUE on pass, FALSE on fail.
   */
  protected function assertFieldById($id, $value = '', $message = '', $group = 'Other') {
    try {
      $element = $this->driver->getElement("id=$id");
      if ($value) {
        $element = $this->elementValue($element, $value);
      }
    }
    catch (Exception $e) {
      $element = FALSE;
    }
    return $this->assertTrue(!empty($element), $message ? $message : t('Field found by id'), $group);
  }

  /**
   * Asserts that a field not exists in the current page with the given id and value.
   *
   * @param $id
   *   Id of field to assert.
   * @param $value
   *   Value of the field to assert.
   * @param $message
   *   Message to display.
   * @param $group
   *   The group this message belongs to.
   * @return
   *   TRUE on pass, FALSE on fail.
   */
  protected function assertNoFieldById($id, $value = '', $message = '', $group = 'Other') {
    try {
      $element = $this->driver->getElement("id=$id");
      if ($value) {
        $element = $this->elementValue($element, $value);
      }
    }
    catch (Exception $e) {
      $element = FALSE;
    }
    return $this->assertTrue(empty($element), $message ? $message : t('Field found by id'), $group);
  }

  /**
   * Check the value of the form element.
   *
   * @param SeleniumWebElement $element
   * @param string $value
   * @return
   *   TRUE on pass, FALSE on fail.
   */
  protected function elementValue($element, $value) {
    switch ($element->getTagName()) {
      case 'input':
        $element_value = $element->getValue();
        break;
      case 'textarea':
        $element_value = $element->getText();
        break;
      case 'select':
        $element_value = $element->getSelected()->getValue();
        $element_text = $element->getSelected()->getText();
        break;
    }
    return $value == $element_value || $value == $element_text;
  }

  /**
   * Asserts that a field exists in the current page by the given XPath.
   *
   * @param $xpath
   *   XPath used to find the field.
   * @param $value
   *   (optional) Value of the field to assert.
   * @param $message
   *   (optional) Message to display.
   * @param $group
   *   (optional) The group this message belongs to.
   *
   * @return
   *   TRUE on pass, FALSE on fail.
   */
  protected function assertFieldByXPath($xpath, $value = NULL, $message = '', $group = 'Other') {
      try {
      $element = $this->driver->getElement($xpath);
      if ($value) {
        $element = $this->elementValue($element, $value);
      }
    }
    catch (Exception $e) {
      $element = FALSE;
    }
    return $this->assertTrue(!empty($element), $message ? $message : t('Field found by Xpath'), $group);
  }

  /**
   * Execute a POST request on a Drupal page.
   * It will be done as usual POST request with SimpleBrowser.
   *
   * @param $path
   *   Location of the post form. Either a Drupal path or an absolute path or
   *   NULL to post to the current page. For multi-stage forms you can set the
   *   path to NULL and have it post to the last received page. Example:
   *
   *   @code
   *   // First step in form.
   *   $edit = array(...);
   *   $this->formSubmit('some_url', $edit, t('Save'));
   *
   *   // Second step in form.
   *   $edit = array(...);
   *   $this->formSubmit(NULL, $edit, t('Save'));
   *   @endcode
   * @param  $edit
   *   Field data in an associative array. Changes the current input fields
   *   (where possible) to the values indicated. A checkbox can be set to
   *   TRUE to be checked and FALSE to be unchecked. Note that when a form
   *   contains file upload fields, other fields cannot start with the '@'
   *   character.
   *
   *   Multiple select fields can be set using name[] and setting each of the
   *   possible values. Example:
   *   @code
   *   $edit = array();
   *   $edit['name[]'] = array('value1', 'value2');
   *   @endcode
   * @param $submit
   *   Value of the submit button whose click is to be emulated. For example,
   *   t('Save'). The processing of the request depends on this value. For
   *   example, a form may have one button with the value t('Save') and another
   *   button with the value t('Delete'), and execute different code depending.
   */
  protected function drupalPost($path, $edit, $submit, $disable_js = array()) {
    if ($this->getUrl() != $path && !is_null($path)) {
      $this->drupalGet($path);
    }
    // Disable javascripts that hide elements.
    $this->disableJs($disable_js);
    // Find form elements and set the values.
    foreach ($edit as $selector => $value){
      $element = $this->driver->getElement("name=$selector");
      // Type of input element. Can be textarea, select or input. If input,
      // we need to check 'type' property.
      $type = $element->getTagName();
      if (strtolower($type) == 'input') {
        $type = $element->getAttributeValue('type');
      }
      switch (strtolower($type)) {
        case 'text':
        case 'textarea':
        case 'password':
          // Clear element first then send text data.
          $element->clear();
          $element->sendKeys($value);
          break;
        case 'select':
          $element->selectValue($value);
          break;
        case 'radio':
          $elements = $this->driver->getAllElements("name=$selector");
          foreach ($elements as $element) {
            if ($element->getValue() == $value) {
              $element->click();
            }
          }
          break;
        case 'checkbox':
          $elements = $this->driver->getAllElements("name=$selector");
          if (!is_array($value)) {
            $value = array($value);
          }
          foreach ($elements as $element) {
            $element_value = $element->getValue();
            $element_selected = $element->isSelected();
            // Click on element if it should be selected but isn't or if element
            // shouldn't be selected but it is.
            if ((in_array($element_value, $value) && !$element_selected) ||
                (!in_array($element_value, $value) && $element_selected)) {
              $element->click();
            }
          }
          break;
      }
    }
    // Find button and submit the form.
    $elements = $this->driver->getAllElements("name=op");
    foreach ($elements as $element) {
      $val = $element->getValue();
      if ($val == $submit){
        $element->click();
        break;
      }
    }
    // Wait till message appear.
    // @todo It is a workaround to determine whether the page is loaded, since submit() and click() don't wait for page to load.
    // @see http://drupal.org/node/1893666
    // @see http://code.google.com/p/selenium/wiki/FrequentlyAskedQuestions#Q:_WebDriver_fails_to_find_elements_/_Does_not_block_on_page_loa
    $this->driver->waitForElements('css=#messages div.messages .placeholder');
    $this->verbose('POST request to: ' . $path .
                   '<hr />Ending URL: ' . $this->getUrl() .
                   '<hr />Fields: ' . highlight_string('<?php ' . var_export($edit, TRUE), TRUE) .
                   '<hr />' . $this->drupalGetContent());
  }

  /**
   * Injects javascript code to disable work of some of the drupal javascripts.
   *
   * For example vertical tabs hides some of the elements on the node form.
   * This leads to situation when Selenium can't access to hidden fields. So if
   * we use drupalPost method that should behave similar to native simpletest
   * method we are not able to submit the form properly.
   *
   * @param array $scripts
   */
  function disableJs($scripts = array()) {
    // Check if vertical tab is present - and disable JS, hiding additional fields.
    if ($this->driver->isElementPresent('css=vertical-tabs')) {
      $scripts += array(
        'vertical tabs' => TRUE,
      );
    }
    foreach ($scripts as $type => $execute) {
      if (!$execute) {
        continue;
      }
      $javascript = '';
      switch ($type) {
        case 'vertical tabs':
          // @todo jQuery doesn't work out of the box.
          // @see http://drupal.org/node/1893678
          //$javascript = 'jQuery(".vertical-tabs-pane").show();';
          $javascript = 'nodeList = document.querySelectorAll(".vertical-tabs-pane");
for (var i = 0; i < nodeList.length; ++i) {
  var item = nodeList[i];
  item.style.display = "block";
}';
          break;
      }
      // Inject javascript.
      if (!empty($javascript)) {
        $this->driver->executeJsSync($javascript);
      }
    }
  }

  /**
   * Get name of current test running.
   *
   * @return string
   */
  protected function getTestName() {
    $backtrace = debug_backtrace();
    foreach ($backtrace as $bt_item) {
      if (strtolower(substr($bt_item['function'], 0, 4)) == 'test') {
        return $bt_item['function'];
      }
    }
  }

  /**
   * Implements getSelectedItem.
   * Get the selected value from a select field.
   *
   * @param $element
   *   SimpleXMLElement select element.
   * @return
   *   The selected options array.
   */
  protected function getSelectedItem(SeleniumWebElement $element) {
    $result = array();
    foreach ($element->getOptions() as $option) {
      if ($option->isSelected()) {
        $result[] = $option;
      }
    }
    return $result;
  }

  /**
   * Pass if the browser's URL matches the given path.
   *
   * @param $path
   *   The expected system path.
   * @param $options
   *   (optional) Any additional options to pass for $path to url().
   * @param $message
   *   Message to display.
   * @param $group
   *   The group this message belongs to, defaults to 'Other'.
   *
   * @return
   *   TRUE on pass, FALSE on fail.
   */
  protected function assertUrl($path, array $options = array(), $message = '', $group = 'Other') {
    if (!$message) {
      $message = t('Current URL is @url.', array(
        '@url' => var_export(url($path, $options), TRUE),
      ));
    }
    $options['absolute'] = TRUE;
    return $this->assertEqual($this->getUrl(), url($path, $options), $message, $group);
  }

  /**
   * Pass if the page title is not the given string.
   *
   * @param $title
   *   The string the title should not be.
   * @param $message
   *   Message to display.
   * @param $group
   *   The group this message belongs to.
   * @return
   *   TRUE on pass, FALSE on fail.
   */
  protected function assertNoTitle($title, $message = '', $group = 'Other') {
    if (!$message) {
      $message = t('Page title @actual is not equal to @unexpected.', array(
        '@actual' => var_export($this->driver->getPageTitle(), TRUE),
        '@unexpected' => var_export($title, TRUE),
      ));
    }
    return $this->assertNotEqual($this->driver->getPageTitle(), $title, $message, $group);
  }

  /**
   * Gets the current raw HTML of requested page.
   *
   * @todo It doesn't work for Opera after click() or submit(): all verboses after click() or submit() are empty.
   * @see http://drupal.org/node/1893674
   */
  protected function drupalGetContent() {
    return $this->driver->getSource();
  }

  /**
   * Get the current url of the browser.
   *
   * @return
   *   The current url.
   */
  protected function getUrl() {
    return $this->driver->getUrl();
  }
}


/**
 * Class of the connection to Webdriver.
 *
 * Original implementation https://github.com/chibimagic/WebDriver-PHP
 */
class SeleniumWebdriver {
  protected $session_id;
  private static $status_codes = array(
    0 => array("Success", " The command executed successfully."),
    7 => array("NoSuchElement", " An element could not be located on the page using the given search parameters."),
    8 => array("NoSuchFrame", " A request to switch to a frame could not be satisfied because the frame could not be found."),
    9 => array("UnknownCommand", " The requested resource could not be found, or a request was received using an HTTP method that is not supported by the mapped resource."),
    10 => array("StaleElementReference", " An element command failed because the referenced element is no longer attached to the DOM."),
    11 => array("ElementNotVisible", " An element command could not be completed because the element is not visible on the page."),
    12 => array("InvalidElementState", " An element command could not be completed because the element is in an invalid state (e.g. attempting to click a disabled element)."),
    13 => array("UnknownError", " An unknown server-side error occurred while processing the command."),
    15 => array("ElementIsNotSelectable", " An attempt was made to select an element that cannot be selected."),
    17 => array("JavaScriptError", " An error occurred while executing user supplied JavaScript."),
    19 => array("XPathLookupError", " An error occurred while searching for an element by XPath."),
    23 => array("NoSuchWindow", " A request to switch to a different window could not be satisfied because the window could not be found."),
    24 => array("InvalidCookieDomain", " An illegal attempt was made to set a cookie under a different domain than the current page."),
    25 => array("UnableToSetCookie", " A request to set a cookie's value could not be satisfied."),
    28 => array("Timeout", " A command did not complete before its timeout expired."),
    303 => array("See other", "See other"),
  );

  /**
   * Execute call to server.
   */
  public function execute($http_type, $relative_url, $variables = array()) {
    if (!empty($variables)) {
      $variables = json_encode($variables);
    }
    $relative_url = str_replace(':sessionId', $this->session_id, $relative_url);
    $full_url = SELENIUM_SERVER_URL . $relative_url;
    // cUrl request.
    $curl = curl_init($full_url);
    curl_setopt($curl, CURLOPT_CUSTOMREQUEST, $http_type);
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($curl, CURLOPT_HEADER, TRUE);
    if (($http_type === "POST" || $http_type === "PUT") && !empty($variables)) {
      curl_setopt($curl, CURLOPT_POSTFIELDS, $variables);
    }
    if ($debug = variable_get('selenium_debug', FALSE)) {
      curl_setopt($curl, CURLOPT_VERBOSE, TRUE);
      curl_setopt($curl, CURLOPT_STDERR, $f = fopen(__DIR__.DIRECTORY_SEPARATOR.'verboseOut.txt', "a+"));
    }
    $full_response = curl_exec($curl);
    if ($debug) {
      fclose($f);
    }
    curl_close($curl);
    $response_parts = explode("\r\n\r\n", $full_response, 2);
    $response['header'] = $response_parts[0];
    if (!empty($response_parts[1])) {
      $response['body'] = $response_parts[1];
    }
    if (isset($response['body'])) {
      $this->check_response_status($response['body'], $variables);
    }
    return $response;
  }

  private function check_response_status($body, $variables) {
    $array = json_decode(trim($body), TRUE);
    if (!is_null($array)) {
      $response_status_code = $array["status"];
      if (!self::$status_codes[$response_status_code]) {
        throw new Exception("Unknown status code $response_status_code returned from server.\n$body");
      }
      if (!in_array($response_status_code, array(0, 303))) {
        $message = $response_status_code . " - " . self::$status_codes[$response_status_code][0] . " - " . self::$status_codes[$response_status_code][1] . "\n";
        $message .= "Arguments: " . print_r($variables, TRUE) . "\n";
        if (isset($array['value']['message'])) {
          $message .= "Message: " . $array['value']['message'] . "\n";
        } else {
          $message .= "Response: " . $body . "\n";
        }
        throw new Exception($message);
      }
    }
  }

  /**
   * Destroy session.
   */
  public function __destruct() {
    $this->execute("DELETE", "/session/:sessionId");
  }

  /**
   * Getters
   */

  /**
   * Get server status.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/status
   */
  public function getServerStatus() {
    $response = $this->execute("GET", "/status");
    return $this->GetJSONValue($response);
  }

  /**
   * Get current URL of the browser.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/url
   */
  public function getUrl() {
    $response = $this->execute("GET", "/session/:sessionId/url");
    return $this->GetJSONValue($response);
  }

  /**
   * Get current page title.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/title
   */
  public function getPageTitle() {
    $response = $this->execute("GET", "/session/:sessionId/title");
    return $this->GetJSONValue($response);
  }

  /**
   * Get current page source.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/source
   */
  public function getSource() {
    $response = $this->execute("GET", "/session/:sessionId/source");
    return $this->GetJSONValue($response);
  }

  /**
   * Get visible text of the body.
   */
  public function getBodyText() {
    $result = $this->getElement("tag name=body")->getText();
    return $result;
  }

  /**
   * Get a screenshot of the current page.
   *
   * See http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/screenshot
   */
  public function getScreenshot() {
    $response = $this->execute("GET", "/session/:sessionId/screenshot");
    $base64_encoded_png = $this->GetJSONValue($response);
    return base64_decode($base64_encoded_png);
  }

  /**
   * Get element.
   *
   * @param string $locator
   * @return SeleniumWebElement
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element
   *
   * @todo Refactor using getAllElements(), since it takes identical time to request the server and we need to wait until element is loaded anyway.
   * @see http://drupal.org/node/1893666
   */
  public function getElement($locator) {
    $variables = $this->ParseLocator($locator);
    $response = $this->execute("POST", "/session/:sessionId/element", $variables);
    $element_id = $this->GetJSONValue($response, "ELEMENT");
    $element = new SeleniumWebElement($this, $element_id, $locator);
    return $element;
  }

  /**
   * Wait for element.
   */
  public function waitForElements($locator) {
    $elements = array();
    $timeout = 10;
    // Wait for elements.
    while ($timeout > 0 && empty($elements)) {
      $elements = $this->getAllElements($locator);
      $this->sleep(1);
      $timeout--;
    }
    return $elements;
  }

  /**
   * Wait for visible elements.
   *
   * Check only $item element for visibility.
   *
   * @todo Refactor it since its logic is not transparent and results are not obvious.
   * @see http://drupal.org/node/1893666
   */
  public function waitForVisibleElements($locator, $item = 0) {
    $elements = $this->waitForElements($locator);
    if (!empty($elements) && isset($elements[$item])) {
      $element = $elements[$item];
      if ($element->isVisible()) {
        return $elements;
      }
    }
    return $elements;
  }

  /**
   * sleep() is interrupted by signals - so use a workaround.
   * @param int $seconds
   */
  public function sleep($seconds) {
    $remained = array($seconds, 0);
    while (is_array($remained)) {
      $remained = time_nanosleep(reset($remained), end($remained));
    }
  }

  /**
   * Get all elements.
   *
   * @param string $locator
   * @return array of SeleniumWebElement objects
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/elements
   *
   * @todo Move waitForElements() functionality here, since getAllElements needs to wait until the elements become available.
   * @see http://drupal.org/node/1893666
   */
  public function getAllElements($locator) {
    $variables = $this->ParseLocator($locator);
    $response = $this->execute("POST", "/session/:sessionId/elements", $variables);
    $element_ids = $this->GetJSONValue($response, "ELEMENT");
    $elements = array();
    foreach ($element_ids as $element_id) {
      $elements[] = new SeleniumWebElement($this, $element_id, $locator);
    }
    return $elements;
  }

  /**
   * Get element that currently has focus.
   *
   * @return SeleniumWebElement
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/active
   */
  public function getActiveElement() {
    $response = $this->execute("POST", "/session/:sessionId/element/active");
    $element_id = $this->GetJSONValue($response, "ELEMENT");
    return new SeleniumWebElement($this, $element_id, "active=true");
  }

  /**
   * Check if element presents on the page.
   *
   * @param string $locator
   * @return boolean
   */
  public function isElementPresent($locator) {
    /*
    try {
      $this->getElement($locator);
      $is_element_present = TRUE;
    }
    catch (Exception $e) {
      $is_element_present = FALSE;
    }
     *
     */
    // Avoid exceptions since they cause fails of sendKeys (it leaves an input field empty).
    $elements = $this->waitForElements($locator);
    $is_element_present = !empty($elements) ? TRUE : FALSE;
    return $is_element_present;
  }

  /**
   * Retrive current window handle.
   *
   * @return type
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/window_handle
   */
  public function getWindowHandle() {
    $response = $this->execute("GET", "/session/:sessionId/window_handle");
    return $this->GetJSONValue($response);
  }

  /**
   * Retrieve list of all window handles available to the session.
   *
   * @return type
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/window_handles
   */
  public function getAllWindowHandles() {
    $response = $this->execute("GET", "/session/:sessionId/window_handles");
    return $this->GetJSONValue($response);
  }

  // See http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/speed
  // Not supported as of Selenium 2.0b3
  public function get_input_speed() {
    $response = $this->execute("GET", "/session/:sessionId/speed");
    return $this->GetJSONValue($response);
  }

  /**
   * Get all cookies.
   *
   * @return type
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/cookie
   */
  public function get_all_cookies() {
    $response = $this->execute("GET", "/session/:sessionId/cookie");
    return $this->GetJSONValue($response);
  }

  /**
   * Get specific cookie.
   *
   * @param string $name
   *   Cookie name.
   * @param string $property
   *   What property to return.
   * @return type
   */
  public function get_cookie($name, $property = NULL) {
    $all_cookies = $this->getCookies();
    foreach ($all_cookies as $cookie) {
      if ($cookie['name'] == $name) {
        if (is_null($property)) {
          return $cookie;
        }
        return $cookie[$property];
      }
    }
  }

  /**
   * Setters.
   */

  /**
   * Set the amount of time, in milliseconds, that asynchronous scripts executed
   *
   * @param int $milliseconds
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/timeouts/async_script
   */
  public function setAsyncTimeout($milliseconds) {
    $variables = array("ms" => $milliseconds);
    $this->execute("POST", "/session/:sessionId/timeouts/async_script", $variables);
  }

  // @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/timeouts/implicit_wait
  public function setImplicitWait($milliseconds) {
    $variables = array("ms" => $milliseconds);
    $this->execute("POST", "/session/:sessionId/timeouts/implicit_wait", $variables);
  }

  /**
   * Navigate to URL.
   *
   * @param string $url
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/url
   */
  public function openUrl($url) {
    if (is_array($url)) {
      $path = $url[0];
      $options = $url[1];
      $options['absolute'] = TRUE;
      $full_url = url($path, $options);
    }
    else {
      $full_url = url($url, array('absolute' => TRUE));
    }
    $variables = array("url" => $full_url);
    $this->execute("POST", "/session/:sessionId/url", $variables);
  }

  /**
   * Navigate forward in browser's history.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/forward
   */
  public function historyForward() {
    $this->execute("POST", "/session/:sessionId/forward");
  }

  /**
   * Navigate back in browser's history.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/back
   */
  public function historyBack() {
    $this->execute("POST", "/session/:sessionId/back");
  }

  /**
   * Refresh the page.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/refresh
   */
  public function refresh() {
    $this->execute("POST", "/session/:sessionId/refresh");
  }

  /**
   * Change focus to another opened window.
   *
   * @param string $window_title
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/window
   */
  public function selectWindow($window_title) {
    $all_window_handles = $this->getAllWindowHandles();
    $all_titles = array();
    $current_title = "";
    foreach ($all_window_handles as $window_handle) {
      $variables = array("name" => $window_handle);
      $this->execute("POST", "/session/:sessionId/window", $variables);
      $current_title = $this->getTitle();
      $all_titles[] = $current_title;
      if ($current_title == $window_title) {
        break;
      }
    }
    if ($current_title != $window_title) {
      throw new Exception("Could not find window with title <$window_title>. Found " . count($all_titles) . " windows: " . implode("; ", $all_titles));
    }
  }

  /**
   * Close the current window.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/window
   */
  public function closeWindow() {
    $this->execute("DELETE", "/session/:sessionId/window");
  }

  // See http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/ime/deactivate
  // Not supported as of Selenium 2.0b3
  public function deactivate_ime() {
    $this->execute("POST", "/session/:sessionId/ime/deactivate");
  }

  // See http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/ime/activate
  // Not supported as of Selenium 2.0b3
  public function activate_ime() {
    $this->execute("POST", "/session/:sessionId/ime/activate");
  }

  /**
   * Change focus to another frame on the page.
   *
   * @param string $identifier
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/frame
   */
  public function selectFrame($identifier) {
    $variables = array("id" => $identifier);
    $this->execute("POST", "/session/:sessionId/frame", $variables);
  }

  /**
   * Set cookie.
   *
   * @param string $name
   * @param string $value
   * @param string $path
   * @param string $domain
   * @param boolean $secure
   * @param integer $expiry
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/cookie
   */
  public function setCookie($name, $value, $path = NULL, $domain = NULL, $secure = FALSE, $expiry = NULL) {
    $variables = array(
      'cookie' => array(
        'name' => $name,
        'value' => $value,
        'secure' => $secure, // The documentation says this is optional, but selenium server 2.0b1 throws a NullPointerException if it's not provided
      )
    );
    if (!is_null($path)) {
      $variables['cookie']['path'] = $path;
    }
    if (!is_null($domain)) {
      $variables['cookie']['domain'] = $domain;
    }
    if (!is_null($expiry)) {
      $variables['cookie']['expiry'] = $expiry;
    }
    $this->execute("POST", "/session/:sessionId/cookie", $variables);
  }

  /**
   * Delete all cookies.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/cookie
   */
  public function deleteAllCookies() {
    $this->execute("DELETE", "/session/:sessionId/cookie");
  }

  /**
   * Delete cookie.
   *
   * @param string $name
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/cookie/:name
   */
  public function delete_cookie($name) {
    $this->execute("DELETE", "/session/:sessionId/cookie/" . $name);
  }

  /**
   * Inject a snippet of JavaScript into the page for execution in the context
   * of the currently selected frame. The executed script is assumed to be
   * synchronous and the result of evaluating the script is returned to the client.
   *
   * The script argument defines the script to execute in the form of a function
   * body. The value returned by that function will be returned to the client.
   * The function will be invoked with the provided args array and the values
   * may be accessed via the arguments object in the order specified.
   *
   * @param string $javascript
   * @param array $arguments
   * @return type
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/execute
   */
  public function executeJsSync($javascript, $arguments = array()) {
    $variables = array(
      "script" => $javascript,
      "args" => $arguments,
    );
    $result = $this->execute("POST", "/session/:sessionId/execute", $variables);
    return $result;
  }

  /**
   * Inject a snippet of JavaScript into the page for execution in the context
   * of the currently selected frame. The executed script is assumed to be
   * asynchronous and must signal that is done by invoking the provided callback,
   * which is always provided as the final argument to the function. The value
   * to this callback will be returned to the client.
   * Asynchronous script commands may not span page loads. If an unload event
   * is fired while waiting for a script result, an error should be returned
   * to the client.
   * The script argument defines the script to execute in teh form of a function
   * body. The function will be invoked with the provided args array and the
   * values may be accessed via the arguments object in the order specified.
   * The final argument will always be a callback function that must be invoked
   * to signal that the script has finished.
   *
   * @param string $javascript
   * @param array $arguments
   * @return type
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/execute_async
   */
  public function executeJsAsync($javascript, $arguments = array()) {
    $variables = array(
      "script" => $javascript,
      "args" => $arguments,
    );
    return $this->execute("POST", "/session/:sessionId/execute_async", $variables);
  }

  // See http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/speed
  // Not supported as of Selenium 2.0b3
  public function set_input_speed($speed) {
    $variables = array("speed" => $speed);
    $this->execute("POST", "/session/:sessionId/speed", $variables);
  }

  /**
   * Send an event to the active element to depress or release a modifier key.
   *
   * @param type $modifier_code
   * @param type $is_down
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/modifier
   *
   * @todo It looks like we should remove it, since there is no such command in API. It seems that it is replaced with sendKeys().
   * @see http://drupal.org/node/1893670
   */
  private function sendModifier($modifier_code, $is_down) {
    $variables = array(
      'value' => $modifier_code,
      'isdown' => $is_down
    );
    $this->execute("POST", "/session/:sessionId/modifier", $variables);
  }

  /**
   * Send standard events to active element.
   *
   * @todo Refactor using sendKeys().
   * @see sendKeys()
   * @see http://drupal.org/node/1893670
   */
  public function eventCtrlDown()     { $this->sendModifier("U+E009", TRUE); }
  public function eventCtrlUp()       { $this->sendModifier("U+E009", FALSE); }
  public function eventShiftDown()    { $this->sendModifier("U+E008", TRUE); }
  public function eventShiftUp()      { $this->sendModifier("U+E008", FALSE); }
  public function eventAltDown()      { $this->sendModifier("U+E00A", TRUE); }
  public function eventAltUp()        { $this->sendModifier("U+E00A", FALSE); }
  public function eventCommandDown()  { $this->sendModifier("U+E03D", TRUE); }
  public function eventCommandUp()    { $this->sendModifier("U+E03D", FALSE); }

  /**
   * Move cursor from element.
   *
   * @param type $right
   * @param type $down
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/moveto
   */
  public function moveCursor($right, $down) {
    $variables = array(
      "xoffset" => $right,
      "yoffset" => $down
    );
    $this->execute("POST", "/session/:sessionId/moveto", $variables);
  }

  /**
   * Click mouse button.
   *
   * @param type $button
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/click
   */
  private function mouseClickButton($button) {
    $variables = array("button" => $button);
    $this->execute("POST", "/session/:sessionId/click", $variables);
  }

  /**
   * Click specific mouse button.
   */
  public function mouseClick()        { $this->mouseClickButton(0); }
  public function mouseClickMiddle()  { $this->mouseClickButton(1); }
  public function mouseClickRight()   { $this->mouseClickButton(2); }

  /**
   * Mouse left button click and hold.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/buttondown
   */
  public function mouseClickHold() {
    $this->execute("POST", "/session/:sessionId/buttondown");
  }

  /**
   * Relese mouse click hold.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/buttonup
   */
  public function mouseClickRelease() {
    $this->execute("POST", "/session/:sessionId/buttonup");
  }

  /**
   * Double click.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/doubleclick
   */
  public function mouseClickDouble() {
    $this->execute("POST", "/session/:sessionId/doubleclick");
  }

  /**
   * Helpers.
   */
  public static function ParseLocator($locator) {
    $se1_to_se2 = array(
      "identifier" => "id",
      "id" => "id",
      "name" => "name",
      "xpath" => "xpath",
      "link" => "link text",
      "css" => "css selector",
      // The dom selector in Se1 isn't in Se2
      // Se2 has 4 new selectors
      "partial link text",
      "tag name",
      "class",
      "class name"
    );

    $locator_parts = explode("=", $locator, 2);
    if (array_key_exists($locator_parts[0], $se1_to_se2) && $locator_parts[1]) { // Explicit Se1 selector
      $strategy = $se1_to_se2[$locator_parts[0]];
      $value = $locator_parts[1];
    }
    elseif (in_array($locator_parts[0], $se1_to_se2) && $locator_parts[1]) { // Explicit Se2 selector
      $strategy = $locator_parts[0];
      $value = $locator_parts[1];
    }
    elseif (substr($locator, 0, 2) === "//") { // Guess the selector based on Se1
        $strategy = "xpath";
        $value = $locator;
    }
    elseif (substr($locator, 0, 9) === "document." || substr($locator, 0, 4) === "dom=") {
      throw new Exception("DOM selectors aren't supported in WebDriver: $locator");
    }
    else { // Fall back to id
      $strategy = "id";
      $value = $locator;
    }
    return array("using" => $strategy, "value" => $value);
  }

  public static function GetJSONValue($curl_response, $attribute = NULL) {
    if (!isset($curl_response['body'])) {
      throw new Exception("Response had no body\n{$curl_response['header']}");
    }
    $array = json_decode(trim($curl_response['body']), TRUE);
    if ($array === NULL) {
      throw new Exception("Body could not be decoded as JSON\n{$curl_response['body']}");
    }
    if (!isset($array["value"])) {
      throw new Exception("JSON had no value\n" . print_r($array, TRUE));
    }
    if ($attribute === NULL) {
      $rv = $array["value"];
    } else {
      if (isset($array["value"][$attribute])) {
        $rv = $array["value"][$attribute];
      } else if (is_array($array["value"])) {
        $rv = array();
        foreach ($array["value"] as $a_value) {
          if (isset($a_value[$attribute])) {
            $rv[] = $a_value[$attribute];
          } else {
            throw new Exception("JSON value did not have attribute $attribute\n" . $array["value"]["message"]);
          }
        }
      } else {
        throw new Exception("JSON value did not have attribute $attribute\n" . $array["value"]["message"]);
      }
    }
    return $rv;
  }
}

/**
 * Selenium element.
 */
class SeleniumWebElement {

  /**
   * Selenium Web Driver instance.
   *
   * @var SeleniumWebDriver
   */
  private $driver;

  /**
   * ID of the session to route the command to.
   *
   * @var string
   */
  private $element_id;

  /**
   * Locator must return the first matching element located in the DOM.
   *
   * @var string
   */
  private $locator;

  /**
   * UTF-8 Keys.
   *
   * @var type
   */
  private static $keys = array(
    'NullKey' => "\uE000",
    'CancelKey' => "\uE001",
    'HelpKey' => "\uE002",
    'BackspaceKey' => "\uE003",
    'TabKey' => "\uE004",
    'ClearKey' => "\uE005",
    'ReturnKey' => "\uE006",
    'EnterKey' => "\uE007",
    'ShiftKey' => "\uE008",
    'ControlKey' => "\uE009",
    'AltKey' => "\uE00A",
    'PauseKey' => "\uE00B",
    'EscapeKey' => "\uE00C",
    'SpaceKey' => "\uE00D",
    'PageUpKey' => "\uE00E",
    'PageDownKey' => "\uE00F",
    'EndKey' => "\uE010",
    'HomeKey' => "\uE011",
    'LeftArrowKey' => "\uE012",
    'UpArrowKey' => "\uE013",
    'RightArrowKey' => "\uE014",
    'DownArrowKey' => "\uE015",
    'InsertKey' => "\uE016",
    'DeleteKey' => "\uE017",
    'SemicolonKey' => "\uE018",
    'EqualsKey' => "\uE019",
    'Numpad0Key' => "\uE01A",
    'Numpad1Key' => "\uE01B",
    'Numpad2Key' => "\uE01C",
    'Numpad3Key' => "\uE01D",
    'Numpad4Key' => "\uE01E",
    'Numpad5Key' => "\uE01F",
    'Numpad6Key' => "\uE020",
    'Numpad7Key' => "\uE021",
    'Numpad8Key' => "\uE022",
    'Numpad9Key' => "\uE023",
    'MultiplyKey' => "\uE024",
    'AddKey' => "\uE025",
    'SeparatorKey' => "\uE026",
    'SubtractKey' => "\uE027",
    'DecimalKey' => "\uE028",
    'DivideKey' => "\uE029",
    'F1Key' => "\uE031",
    'F2Key' => "\uE032",
    'F3Key' => "\uE033",
    'F4Key' => "\uE034",
    'F5Key' => "\uE035",
    'F6Key' => "\uE036",
    'F7Key' => "\uE037",
    'F8Key' => "\uE038",
    'F9Key' => "\uE039",
    'F10Key' => "\uE03A",
    'F11Key' => "\uE03B",
    'F12Key' => "\uE03C",
    'CommandKey' => "\uE03D",
    'MetaKey' => "\uE03D",
  );

  public function __construct($driver, $element_id, $locator) {
    $this->driver = $driver;
    $this->element_id = $element_id;
    $this->locator = $locator;
  }

  private function execute($http_type, $relative_url, $variables = array()) {
    $result = $this->driver->execute($http_type, "/session/:sessionId/element/" . $this->element_id . $relative_url, $variables);
    return $result;
  }

  /**
   * Getters
   */

  /**
   * Describe the identified element.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id
   */
  public function describe() {
    $response = $this->execute("GET", "");
    return $this->driver->GetJSONValue($response);
  }

  /**
   * Returns the visible text for the element.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/text
   */
  public function getText() {
    $response = $this->execute("GET", "/text");
    return $this->driver->GetJSONValue($response);
  }

  /**
   * Query for the value of an element, as determined by its value attribute.
   *
   * @return string | NULL
   *   The element's value, or NULL if it does not have a value attribute.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/value
   */
  public function getValue() {
    $response = $this->execute("GET", "/value");
    return $this->driver->GetJSONValue($response);
  }

  /**
   * Determine if an element is currently displayed.
   *
   * @return boolean
   *   Whether the element is displayed.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/displayed
   */
  public function isVisible() {
    $response = $this->execute("GET", "/displayed");
    return $this->driver->GetJSONValue($response);
  }

  /**
   * Determine if an element is currently enabled.
   *
   * @return boolean
   *   Whether the element is enabled.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/enabled
   */
  public function isEnabled() {
    $response = $this->execute("GET", "/enabled");
    return $this->driver->GetJSONValue($response);
  }

  /**
   * Determine if an OPTION element, or an INPUT element of type checkbox or radiobutton is currently selected.
   *
   * @return boolean
   *    Whether the element is selected.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/selected
   */
  public function isSelected() {
    $response = $this->execute("GET", "/selected");
    return $this->driver->GetJSONValue($response);
  }

  /**
   * Search for an element on the page, starting from the identified element.
   * The located element will be returned as a SeleniumWebElement JSON object.
   * Each locator must return the first matching element located in the DOM.
   *
   * @return SeleniumWebElement object
   *    A SeleniumWebElement JSON object for the located element.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/element
   */
  public function getNextElement($locator) {
    $variables = $this->driver->ParseLocator($locator);
    $response = $this->execute("POST", "/element", $variables);
    $next_element_id = $this->driver->GetJSONValue($response, "ELEMENT");
    return new SeleniumWebElement($this->driver, $next_element_id, $locator);
  }

  /**
   * Search for multiple elements on the page, starting from the identified element.
   * The located elements will be returned as a SeleniumWebElement JSON objects.
   * Elements should be returned in the order located in the DOM.
   *
   * @return array
   *    A list of SeleniumWebElement JSON objects for the located elements.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/elements
   */
  public function getAllNextElements($locator) {
    $variables = $this->driver->ParseLocator($locator);
    $response = $this->execute("POST", "/elements", $variables);
    $all_element_ids = $this->driver->GetJSONValue($response, "ELEMENT");
    $all_elements = array();
    foreach ($all_element_ids as $element_id) {
      $all_elements[] = new SeleniumWebElement($this->driver, $element_id, $locator);
    }
    return $all_elements;
  }

  /**
   * Query for an element's tag name.
   *
   * @return string
   *    The element's tag name, as a lowercase string.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/name
   */
  public function getTagName() {
    $response = $this->execute("GET", "/name");
    return $this->driver->GetJSONValue($response);
  }

  /**
   * Get the value of an element's attribute.
   *
   * @return string | NULL
   *    The value of the attribute, or NULL if it is not set on the element.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/attribute/:name
   */
  public function getAttributeValue($attribute_name) {
    $response = $this->execute("GET", "/attribute/" . $attribute_name);
    return $this->driver->GetJSONValue($response);
  }

  /**
   * Test if two element IDs refer to the same DOM element.
   *
   * @return boolean
   *   Whether the two IDs refer to the same element.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/equals/:other
   */
  public function isSameElementAs($other_element_id) {
    $response = $this->execute("GET", "/equals/" . $other_element_id);
    return $this->driver->GetJSONValue($response);
  }

  /**
   * Determine an element's location on the page.
   * The point (0, 0) refers to the upper-left corner of the page.
   * The element's coordinates are returned as an array with x and y properties.
   *
   * @return array(x:integer, y:integer)
   *   The X and Y coordinates for the element on the page.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/location
   */
  public function getLocation() {
    $response = $this->execute("GET", "/location");
    return $this->driver->GetJSONValue($response);
  }

  /**
   * Determine an element's size in pixels.
   * The size will be returned as an array with width and height properties.
   *
   * @return array(width:integer, height:integer)
   *   The width and height of the element, in pixels.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/size
   */
  public function getSize() {
    $response = $this->execute("GET", "/size");
    return $this->driver->GetJSONValue($response);
  }

  /**
   * Query the value of an element's computed CSS property.
   * The CSS property to query should be specified using the CSS property name,
   * not the JavaScript property name (e.g. background-color instead of backgroundColor).
   *
   * @return string
   *   The value of the specified CSS property.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/css/:propertyName
   */
  public function getCssValue($property_name) {
    $response = $this->execute("GET", "/css/" . $property_name);
    return $this->driver->GetJSONValue($response);
  }


  /**
   * Setters
   */

  /**
   * Click on an element.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/click
   */
  public function click() {
    $this->execute("POST", "/click");
  }

  /**
   * Submit a FORM element. The submit command may also be applied to any element
   * that is a descendant of a FORM element.
   * WARNING: This method is a bad idea, use click() instead, since it has better
   * emulation of user-browser interaction.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/submit
   */
  public function submit() {
    $this->execute("POST", "/submit");
  }

  /**
   * Clear a TEXTAREA or text INPUT element's value.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/clear
   */
  public function clear() {
    $this->execute("POST", "/clear");
  }

  /**
   * Move the mouse over an element.
   * Not supported as of Selenium 2.0b3
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/hover
   */
  public function hover() {
    $this->execute("POST", "/hover");
  }

  /**
   * Toggle whether an OPTION element, or an INPUT element of type checkbox or radiobutton is currently selected.
   *
   * @return boolean
   *   Whether the element is selected after toggling its state.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/toggle
   */
  public function toggle() {
    $response = $this->execute("POST", "/toggle");
    return $this->driver->GetJSONValue($response);
  }

  /**
   * Query for the value of an element, as determined by its value attribute.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/value
   */
  public function sendKeys($keys) {
    // @todo Replace this solution with switching to necessary window.
    // It seems that Opera looses active window if you leave the window in which
    // Selenium executes the test.
    // @see http://drupal.org/node/1893688
    while ($this->getValue() != $keys) {
      // First clear input.
      $this->clear();
      // Then try to send keys. "Try" - is for Opera, which is too capricious.
      // Sometimes it doesn't: for example, after catching an exception or
      // switching active window.
      // @see isElementPresent()
      $variables = array("value" => preg_split('//u', $keys, -1, PREG_SPLIT_NO_EMPTY));
      $this->execute("POST", "/value", $variables);
      $this->driver->sleep(1);
    }
  }

  /**
   * Get key from $keys.
   */
  public function getKey($key_name) {
    if (isset(self::$keys[$key_name])) {
      return json_decode('"' . self::$keys[$key_name] . '"');
    }
    else {
      throw new Exception("Can't type key $key_name");
    }
  }
  /**
   * Drag and drop an element.
   * The distance to drag an element should be specified relative to the upper-left corner of the page.
   *
   * @param integer
   *   The number of pixels to drag the element in the horizontal direction.
   *   A positive value indicates the element should be dragged to the right,
   *   while a negative value indicates that it should be dragged to the left.
   *
   * @param integer
   *   The number of pixels to drag the element in the vertical direction.
   *   A positive value indicates the element should be dragged down towards the bottom of the screen,
   *   while a negative value indicates that it should be dragged towards the top of the screen.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/element/:id/drag
   */
  public function dragAndDrop($pixels_right, $pixels_down) {
    $variables = array(
      "x" => $pixels_right,
      "y" => $pixels_down
    );
    $this->execute("POST", "/drag", $variables);
  }

  /**
   * Move the mouse by an offset of the specificed element,
   * the mouse will be moved to the center of the element.
   * If the element is not visible, it will be scrolled into view.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/moveto
   */
  public function moveCursorCenter() {
    $variables = array("element" => $this->element_id);
    $this->driver->execute("POST", "/session/:sessionId/moveto", $variables);
  }

  /**
   * Move the mouse by an offset of the specificed element.
   * If the element is not visible, it will be scrolled into view.
   *
   * @param integer
   *   X offset to move to, relative to the top-left corner of the element.
   * @param integer
   *   Y offset to move to, relative to the top-left corner of the element.
   *
   * @see http://code.google.com/p/selenium/wiki/JsonWireProtocol#/session/:sessionId/moveto
   */
  public function moveCursorRelative($right, $down) {
    $variables = array(
      "element" => $this->element_id,
      "xoffset" => $right,
      "yoffset" => $down,
    );
    $this->driver->execute("POST", "/session/:sessionId/moveto", $variables);
  }


  /*
   * Getters for <select> elements
   */

  /**
   * Search for selected option of <select> element on the page.
   * The located element will be returned as a SeleniumWebElement JSON object.
   *
   * @return SeleniumWebElement object
   *    A SeleniumWebElement JSON object for the located element.
   */
  public function getSelected() {
    // See http://code.google.com/p/selenium/issues/detail?id=1518
    try {
      return $this->getNextElement("css=option[selected]"); // Does not work in IE8
    }
    catch (Exception $e) {
      return $this->getNextElement("css=option[selected='selected']"); // Does not work in IE7
    }
  }

  /**
   * Search for options for <select> element on the page, starting from the identified element.
   * The located elements will be returned as a SeleniumWebElement JSON objects.
   * Elements should be returned in the order located in the DOM.
   *
   * @return array
   *    A list of SeleniumWebElement JSON objects for the located elements.
   */
  public function getOptions() {
    return $this->getAllNextElements("tag name=option");
  }

  /**
   * Setters for <select> elements
   */

  /**
   * Search for <select> element on the page, starting from the identified element,
   * which has option with specificed label.
   *
   * @param string
   *   Label of the option for select element
   */
  public function selectLabel($label) {
    $option_element = $this->getNextElement("xpath=.//option[text()='" . $label . "']");
    $option_element->click();
    // Workaround for AJAX behavior.
    $this->driver->sleep(5);
  }

  /**
   * Search for <select> element on the page, starting from the identified element,
   * which has option with specificed value.
   *
   * @param string
   *   Value of the option for select element
   */
  public function selectValue($value) {
    $option_element = $this->getNextElement("xpath=.//option[@value='" . $value . "']");
    $option_element->click();
    // Workaround for AJAX behavior.
    $this->driver->sleep(5);
  }

  /**
   * Search for <select> element on the page, starting from the identified element,
   * which has option with specificed attribute.
   *
   * @param string
   */
  public function selectIndex($index) {
    $option_element = $this->getNextElement("xpath=.//option[" . $index . "]");
    $option_element->click();
    // Workaround for AJAX behavior.
    $this->driver->sleep(5);
  }
}

/**
 * Class of the connection to Firefox.
 */
class SeleniumFirefoxDriver extends SeleniumWebDriver {

  // @todo Abstract this behavior in parent function.
  // @see http://drupal.org/node/1893672
  function __construct($user_agent, $test_id) {
    $temporary_path = file_directory_temp();
    file_prepare_directory($temporary_path);
    $zip_file_path = $temporary_path . '/' . $test_id . '_firefox_profile.zip';
    // Generate Firefox profile.
    $zip = new ZipArchive;
    $res = $zip->open($zip_file_path, ZipArchive::CREATE);
    if ($res === TRUE) {
      $zip->addFromString('prefs.js', 'user_pref("general.useragent.override", "' . $user_agent . '");');
      $zip->close();
    }
    else {
      throw new Exception('Cant create firefox profile ' . $zip_file_path);
    }
    // By specifications of the Webdriver we should encode firefox
    // profile zip archive with base64.
    $firefox_profile = base64_encode(file_get_contents($zip_file_path));
    // Start browser.
    $capabilities = array(
      'browserName' => 'firefox',
      'javascriptEnabled' => TRUE,
      'cssSelectorsEnabled' => TRUE,
      'platform' => 'ANY',
      'firefox_profile' => $firefox_profile,
    );
    $variables = array("desiredCapabilities" => $capabilities);
    $response = $this->execute("POST", "/session", $variables);
    // Parse out session id.
    preg_match("/\n[Ll]ocation:.*\/([^\n$]*)/", $response['header'], $matches);
    if (count($matches) > 0) {
      $this->session_id = trim($matches[1]);
    }
    else {
      $message = "Did not get a session id from " . SELENIUM_SERVER_URL . "\n";
      if (!empty($response['body'])) {
        $message .= $response['body'];
      }
      elseif (!empty($response['header'])) {
        $message .= $response['header'];
      }
      else {
        $message .= "No response from server.";
      }
      throw new Exception($message);
    }
  }
}

/**
 * Class of the connection to Chrome.
 */
class SeleniumChromeDriver extends SeleniumWebDriver {

  // @todo Abstract this behavior in parent function.
  // @see http://drupal.org/node/1893672
  function __construct($user_agent, $testCase) {
    $user_agent_string = '--user-agent=' . $user_agent;
    // Start browser.
    $capabilities = array(
      'browserName' => 'chrome',
      'javascriptEnabled' => TRUE,
      'cssSelectorsEnabled' => TRUE,
      'platform' => 'ANY',
      'chromeOptions' => array('args' => array($user_agent_string)),
    );
    $variables = array("desiredCapabilities" => $capabilities);
    $response = $this->execute("POST", "/session", $variables);
    // We add new line charachter to header as ChromeDriver doesn't have ending
    // new line charachter.
    $response['header'] .= "\n";
    // Parse out session id.
    preg_match("/\n[Ll]ocation:.*\/([^\n$]*)/", $response['header'], $matches);
    if (count($matches) > 0) {
      $this->session_id = trim($matches[1]);
    }
    else {
      $message = "Did not get a session id from " . SELENIUM_SERVER_URL . "\n";
      if (!empty($response['body'])) {
        $message .= $response['body'];
      }
      elseif (!empty($response['header'])) {
        $message .= $response['header'];
      }
      else {
        $message .= "No response from server.";
      }
      throw new Exception($message);
    }
  }
}

/**
 * Class of the connection to Opera.
 */
class SeleniumOperaDriver extends SeleniumWebDriver {

  // @todo Abstract this behavior in parent function.
  // @see http://drupal.org/node/1893672
  function __construct($user_agent, $testCase) {
    // Start browser.
    $capabilities = array(
      'browserName' => 'opera',
      'javascriptEnabled' => TRUE,
      'cssSelectorsEnabled' => TRUE,
      'platform' => 'ANY',
    );
    $variables = array("desiredCapabilities" => $capabilities);
    $response = $this->execute("POST", "/session", $variables);
    // Parse out session id.
    preg_match("/\n[Ll]ocation:.*\/([^\n$]*)/", $response['header'], $matches);
    if (count($matches) > 0) {
      $this->session_id = trim($matches[1]);
    }
    else {
      $message = "Did not get a session id from " . SELENIUM_SERVER_URL . "\n";
      if (!empty($response['body'])) {
        $message .= $response['body'];
      }
      elseif (!empty($response['header'])) {
        $message .= $response['header'];
      }
      else {
        $message .= "No response from server.";
      }
      throw new Exception($message);
    }
  }
}

/**
 * Class of the connection to Internet Explorer.
 */
class SeleniumIExplorerDriver extends SeleniumWebDriver {

  // @todo Abstract this behavior in parent function.
  // @see http://drupal.org/node/1893672
  function __construct($user_agent, $testCase) {
    // Start browser.
    $capabilities = array(
      'browserName' => 'internet explorer',
      'javascriptEnabled' => TRUE,
      'cssSelectorsEnabled' => TRUE,
      'platform' => 'ANY',
    );

    $variables = array("desiredCapabilities" => $capabilities);
    $response = $this->execute("POST", "/session", $variables);
    // Parse out session id.
    preg_match("/\n[Ll]ocation:.*\/([^\n$]*)/", $response['header'], $matches);
    if (count($matches) > 0) {
      $this->session_id = trim($matches[1]);
    }
    else {
      $message = "Did not get a session id from " . SELENIUM_SERVER_URL . "\n";
      if (!empty($response['body'])) {
        $message .= $response['body'];
      }
      elseif (!empty($response['header'])) {
        $message .= $response['header'];
      }
      else {
        $message .= "No response from server.";
      }
      throw new Exception($message);
    }
  }
}