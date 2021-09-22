<?php

/**
 * @file
 * Contains SimplenewsSource interface and implementations.
 */

/**
 * The source used to build a newsletter mail.
 *
 * @ingroup source
 */
interface SimplenewsSourceInterface {

  /**
   * Returns the mail headers.
   *
   * @param $headers
   *   The default mail headers.
   *
   * @return
   *   Mail headers as an array.
   */
  function getHeaders(array $headers);

  /**
   * Returns the mail subject.
   */
  function getSubject();

  /**
   * Returns the mail body.
   *
   * The body should either be plaintext or html, depending on the format.
   */
  function getBody();

  /**
   * Returns the plaintext body.
   */
  function getPlainBody();

  /**
   * Returns the mail footer.
   *
   * The footer should either be plaintext or html, depending on the format.
   */
  function getFooter();

  /**
   * Returns the plain footer.
   */
  function getPlainFooter();

  /**
   * Returns the mail format.
   *
   * @return
   *   The mail format as string, either 'plain' or 'html'.
   */
  function getFormat();

  /**
   * Returns the recipent of this newsletter mail.
   *
   * @return
   *   The recipient mail address(es) of this newsletter as a string.
   */
  function getRecipient();

  /**
   * The language that should be used for this newsletter mail.
   */
  function getLanguage();

  /**
   * Returns an array of attachments for this newsletter mail.
   *
   * @return
   *   An array of managed file objects with properties uri, filemime and so on.
   */
  function getAttachments();

  /**
   * Returns the token context to be used with token replacements.
   *
   * @return
   *   An array of objects as required by token_replace().
   */
  function getTokenContext();

  /**
   * Returns the mail key to be used for drupal_mail().
   *
   * @return
   *   The mail key, either test or node.
   */
  function getKey();

  /**
   * Returns the formatted from mail address.
   */
  function getFromFormatted();

  /**
   * Returns the plain mail address.
   */
  function getFromAddress();
}

/**
 * Source interface based on a node.
 *
 * This is the interface that needs to be implemented to be compatible with
 * the default simplenews spool implementation and therefore exposed in
 * hook_simplenews_source_cache_info().
 *
 * @ingroup source
 */
interface SimplenewsSourceNodeInterface extends SimplenewsSourceInterface {

  /**
   * Create a source based on a node and subscriber.
   */
  function __construct($node, $subscriber);

  /**
   * Returns the actually used node of this source.
   */
  function getNode();

  /**
   * Returns the subscriber object.
   */
  function getSubscriber();
}

/**
 * Interface for a simplenews source cache implementation.
 *
 * This is only compatible with the SimplenewsSourceNodeInterface interface.
 *
 * @ingroup source
 */
interface SimplenewsSourceCacheInterface {

  /**
   * Create a new instance, allows to initialize based on the used
   * source.
   */
  function __construct(SimplenewsSourceNodeInterface $source);

  /**
   * Return a cached element, if existing.
   *
   * Although group and key can be used to identify the requested cache, the
   * implementations are responsible to create a unique cache key themself using
   * the $source. For example based on the node id and the language.
   *
   * @param $group
   *   Group of the cache key, which allows cache implementations to decide what
   *   they want to cache. Currently used groups:
   *     - data: Raw data, e.g. attachments.
   *     - build: Built and themed content, before personalizations like tokens.
   *     - final: The final returned data. Caching this means that newsletter
   *       can not be personalized anymore.
   * @param $key
   *   Identifies the requested element, e.g. body, footer or attachments.
   */
  function get($group, $key);

  /**
   * Write an element to the cache.
   *
   * Although group and key can be used to identify the requested cache, the
   * implementations are responsible to create a unique cache key themself using
   * the $source. For example based on the node id and the language.
   *
   * @param $group
   *   Group of the cache key, which allows cache implementations to decide what
   *   they want to cache. Currently used groups:
   *     - data: Raw data, e.g. attachments.
   *     - build: Built and themed content, before personalizations like tokens.
   *     - final: The final returned data. Caching this means that newsletter
   *       can not be personalized anymore.
   * @param $key
   *   Identifies the requested element, e.g. body, footer or attachments.
   * @param $data
   *   The data to be saved in the cache.
   */
  function set($group, $key, $data);
}

/**
 * A Simplenews spool implementation is a factory for Simplenews sources.
 *
 * Their main functionility is to return a number of sources based on the passed
 * in array of mail spool rows. Additionally, it needs to return the processed
 * mail rows after a source was sent.
 *
 * @todo: Move spool functions into this interface.
 *
 * @ingroup spool
 */
interface SimplenewsSpoolInterface {

  /**
   * Initalizes the spool implementation.
   *
   * @param $spool_list
   *   An array of rows from the {simplenews_mail_spool} table.
   */
  function __construct($pool_list);

  /**
   * Returns a Simplenews source to be sent.
   *
   * A single source may represent any number of mail spool rows, e.g. by
   * addressing them as BCC.
   */
  function nextSource();

  /**
   * Returns the processed mail spool rows, keyed by the msid.
   *
   * Only rows that were processed while preparing the previously returned
   * source must be returned.
   *
   * @return
   *   An array of mail spool rows, keyed by the msid. Can optionally have set
   *   the following additional properties.
   *     - actual_nid: In case of content translation, the source node that was
   *       used for this mail.
   *     - error: FALSE if the prepration for this row failed. For example set
   *       when the corresponding node failed to load.
   *     - status: A simplenews spool status to indicate the status.
   */
  function getProcessed();
}

/**
 * Simplenews Spool implementation.
 *
 * @ingroup spool
 */
class SimplenewsSpool implements SimplenewsSpoolInterface {

  /**
   * Array with mail spool rows being processed.
   *
   * @var array
   */
  protected $spool_list;

  /**
   * Array of the processed mail spool rows.
   */
  protected $processed = array();

  /**
   * Implements SimplenewsSpoolInterface::_construct($spool_list);
   */
  public function __construct($spool_list) {
    $this->spool_list = $spool_list;
  }

  /**
   * Implements SimplenewsSpoolInterface::nextSource();
   */
  public function nextSource() {
    // Get the current mail spool row and update the internal pointer to the
    // next row.
    $return = each($this->spool_list);
    // If we're done, return false.
    if (!$return) {
      return FALSE;
    }
    $spool_data = $return['value'];

    // Store this spool row as processed.
    $this->processed[$spool_data->msid] = $spool_data;

    $node = node_load($spool_data->nid);
    if (!$node) {
      // If node the load failed, set the processed status done and proceed with
      // the next mail.
      $this->processed[$spool_data->msid]->result = array(
        'status' => SIMPLENEWS_SPOOL_DONE,
        'error' => TRUE
      );
      return $this->nextSource();
    }

    if ($spool_data->data) {
      $subscriber = $spool_data->data;
    }
    else {
      $subscriber = simplenews_subscriber_load_by_mail($spool_data->mail);
    }

    if (!$subscriber) {
      // If loading the subscriber failed, set the processed status done and
      // proceed with the next mail.
      $this->processed[$spool_data->msid]->result = array(
        'status' => SIMPLENEWS_SPOOL_DONE,
        'error' => TRUE
      );
      return $this->nextSource();
    }

    $source_class = $this->getSourceImplementation($spool_data);
    $source = new $source_class($node, $subscriber);

    // Set which node is actually used. In case of a translation set, this might
    // not be the same node.
    $this->processed[$spool_data->msid]->actual_nid = $source->getNode()->nid;
    return $source;
  }

  /**
   * Implements SimplenewsSpoolInterface::getProcessed();
   */
  function getProcessed() {
    $processed = $this->processed;
    $this->processed = array();
    return $processed;
  }

  /**
   * Return the Simplenews source implementation for the given mail spool row.
   */
  protected function getSourceImplementation($spool_data) {
    return variable_get('simplenews_source', 'SimplenewsSourceNode');
  }
}

/**
 * Simplenews source implementation based on nodes for a single subscriber.
 *
 * @ingroup source
 */
class SimplenewsSourceNode implements SimplenewsSourceNodeInterface {

  /**
   * The node object.
   */
  protected $node;

  /**
   * The cached build render array.
   */
  protected $build;

  /**
   * The newsletter category.
   */
  protected $category;

  /**
   * The subscriber and therefore recipient of this mail.
   */
  protected $subscriber;

  /**
   * The mail key used for drupal_mail().
   */
  protected $key = 'test';

  /**
   * The simplenews newsletter.
   */
  protected $newsletter;

  /**
   * Cache implementation used for this source.
   *
   * @var SimplenewsSourceCacheInterface
   */
  protected $cache;

  /**
   * Implements SimplenewsSourceInterface::_construct();
   */
  public function __construct($node, $subscriber) {
    $this->setSubscriber($subscriber);
    $this->setNode($node);
    $this->newsletter = simplenews_newsletter_load($node->nid);
    $this->category = simplenews_category_load($this->newsletter->tid);
    $this->initCache();
  }

  /**
   * Set the node of this source.
   *
   * If the node is part of a translation set, switch to the node for the
   * requested language, if existent.
   */
  public function setNode($node) {
    $langcode = $this->getLanguage();
    $nid = $node->nid;
    if (module_exists('translation')) {
      // If the node has translations and a translation is required
      // the equivalent of the node in the required language is used
      // or the base node (nid == tnid) is used.
      if ($tnid = $node->tnid) {
        if ($langcode != $node->language) {
          $translations = translation_node_get_translations($tnid);
          // A translation is available in the preferred language.
          if ($translation = $translations[$langcode]) {
            $nid = $translation->nid;
            $langcode = $translation->language;
          }
          else {
            // No translation found which matches the preferred language.
            foreach ($translations as $translation) {
              if ($translation->nid == $tnid) {
                $nid = $tnid;
                $langcode = $translation->language;
                break;
              }
            }
          }
        }
      }
    }
    // If a translation of the node is used, load this node.
    if ($nid != $node->nid) {
      $this->node = node_load($nid);
    }
    else {
      $this->node = $node;
    }
  }

  /**
   * Initialize the cache implementation.
   */
  protected function initCache() {
    $class = variable_get('simplenews_source_cache', 'SimplenewsSourceCacheBuild');
    $this->cache = new $class($this);
  }

  /**
   * Returns the corresponding category.
   */
  public function getCategory() {
    return $this->category;
  }

  /**
   * Set the active subscriber.
   */
  public function setSubscriber($subscriber) {
    $this->subscriber = $subscriber;
  }

  /**
   * Return the subscriber object.
   */
  public function getSubscriber() {
    return $this->subscriber;
  }

  /**
   * Implements SimplenewsSourceInterface::getHeaders().
   */
  public function getHeaders(array $headers) {

    // If receipt is requested, add headers.
    if ($this->category->receipt) {
      $headers['Disposition-Notification-To'] = $this->getFromAddress();
      $headers['X-Confirm-Reading-To'] = $this->getFromAddress();
    }

    // Add priority if set.
    switch ($this->category->priority) {
      case SIMPLENEWS_PRIORITY_HIGHEST:
        $headers['Priority'] = 'High';
        $headers['X-Priority'] = '1';
        $headers['X-MSMail-Priority'] = 'Highest';
        break;
      case SIMPLENEWS_PRIORITY_HIGH:
        $headers['Priority'] = 'urgent';
        $headers['X-Priority'] = '2';
        $headers['X-MSMail-Priority'] = 'High';
        break;
      case SIMPLENEWS_PRIORITY_NORMAL:
        $headers['Priority'] = 'normal';
        $headers['X-Priority'] = '3';
        $headers['X-MSMail-Priority'] = 'Normal';
        break;
      case SIMPLENEWS_PRIORITY_LOW:
        $headers['Priority'] = 'non-urgent';
        $headers['X-Priority'] = '4';
        $headers['X-MSMail-Priority'] = 'Low';
        break;
      case SIMPLENEWS_PRIORITY_LOWEST:
        $headers['Priority'] = 'non-urgent';
        $headers['X-Priority'] = '5';
        $headers['X-MSMail-Priority'] = 'Lowest';
        break;
    }

    // Add user specific header data.
    $headers['From'] = $this->getFromFormatted();
    $headers['List-Unsubscribe'] = '<' . token_replace('[simplenews-subscriber:unsubscribe-url]', $this->getTokenContext(), array('sanitize' => FALSE)) . '>';

    // Add general headers
    $headers['Precedence'] = 'bulk';
    return $headers;
  }

  /**
   * Implements SimplenewsSourceInterface::getTokenContext().
   */
  function getTokenContext() {
    return array(
      'category' => $this->getCategory(),
      'simplenews_subscriber' => $this->getSubscriber(),
      'node' => $this->getNode(),
    );
  }

  /**
   * Set the mail key.
   */
  function setKey($key) {
    $this->key = $key;
  }

  /**
   * Implements SimplenewsSourceInterface::getKey().
   */
  function getKey() {
    return $this->key;
  }

  /**
   * Implements SimplenewsSourceInterface::getFromFormatted().
   */
  function getFromFormatted() {
    // Windows based PHP systems don't accept formatted email addresses.
    if (drupal_substr(PHP_OS, 0, 3) == 'WIN') {
      return $this->getFromAddress();
    }

    return '"' . addslashes(mime_header_encode($this->getCategory()->from_name)) . '" <' . $this->getFromAddress() . '>';
  }

  /**
   * Implements SimplenewsSourceInterface::getFromAddress().
   */
  function getFromAddress() {
    return $this->getCategory()->from_address;
  }

  /**
   * Implements SimplenewsSourceInterface::getRecipient().
   */
  function getRecipient() {
    return $this->getSubscriber()->mail;
  }

  /**
   * Implements SimplenewsSourceInterface::getFormat().
   */
  function getFormat() {
    return $this->getCategory()->format;
  }

  /**
   * Implements SimplenewsSourceInterface::getLanguage().
   */
  function getLanguage() {
    return $this->getSubscriber()->language;
  }

  /**
   * Implements SimplenewsSourceSpoolInterface::getNode().
   */
  function getNode() {
    return $this->node;
  }

  /**
   * Implements SimplenewsSourceInterface::getSubject().
   */
  function getSubject() {
    // Build email subject and perform some sanitizing.
    $langcode = $this->getLanguage();
    $language_list = language_list();
    // Use the requested language if enabled.
    $language = isset($language_list[$langcode]) ? $language_list[$langcode] : NULL;
    $subject = token_replace($this->getCategory()->email_subject, $this->getTokenContext(), array('sanitize' => FALSE, 'language' => $language));

    // Line breaks are removed from the email subject to prevent injection of
    // malicious data into the email header.
    $subject = str_replace(array("\r", "\n"), '', $subject);
    return $subject;
  }

  /**
   * Set up the necessary language and user context.
   */
  protected function setContext() {

    // Switch to the user
    if ($this->uid = $this->getSubscriber()->uid) {
      simplenews_impersonate_user($this->uid);
    }

    // Change language if the requested language is enabled.
    $language = $this->getLanguage();
    $languages = language_list();
    if (isset($languages[$language])) {
      $this->original_language = $GLOBALS['language'];
      $GLOBALS['language'] = $languages[$language];
      $GLOBALS['language_url'] = $languages[$language];
      // Overwrites the current content language for i18n_select.
      if (module_exists('i18n_select')) {
        $GLOBALS['language_content'] = $languages[$language];
      }
    }
  }

  /**
   * Reset the context.
   */
  protected function resetContext() {

    // Switch back to the previous user.
    if ($this->uid) {
      simplenews_revert_user();
    }

    // Switch language back.
    if (!empty($this->original_language)) {
      $GLOBALS['language'] = $this->original_language;
      $GLOBALS['language_url'] = $this->original_language;
      if (module_exists('i18n_select')) {
        $GLOBALS['language_content'] = $this->original_language;
      }
    }
  }

  /**
   * Build the node object.
   *
   * The resulting build array is cached as it is used in multiple places.
   * @param $format
   *   (Optional) Override the default format. Defaults to getFormat().
   */
  protected function build($format = NULL) {
    if (empty($format)) {
      $format = $this->getFormat();
    }
    if (!empty($this->build[$format])) {
      return $this->build[$format];
    }

    // Build message body
    // Supported view modes: 'email_plain', 'email_html', 'email_textalt'
    $build = node_view($this->node, 'email_' . $format);
    unset($build['#theme']);

    foreach (field_info_instances('node', $this->node->type) as $field_name => $field) {
      if (isset($build[$field_name])) {
        $build[$field_name]['#theme'] = 'simplenews_field';
      }
    }

    $this->build[$format] = $build;
    return $this->build[$format];
  }

  /**
   * Build the themed newsletter body.
   *
   * @param $format
   *   (Optional) Override the default format. Defaults to getFormat().
   */
  protected function buildBody($format = NULL) {
    if (empty($format)) {
      $format = $this->getFormat();
    }
    if ($cache = $this->cache->get('build', 'body:' . $format)) {
      return $cache;
    }
    $body = theme('simplenews_newsletter_body', array('build' => $this->build($format), 'category' => $this->getCategory(), 'language' => $this->getLanguage(), 'simplenews_subscriber' => $this->getSubscriber()));
    $this->cache->set('build', 'body:' . $format, $body);
    return $body;
  }

  /**
   * Implements SimplenewsSourceInterface::getBody().
   */
  public function getBody() {
    return $this->getBodyWithFormat($this->getFormat());
  }

  /**
   * Implements SimplenewsSourceInterface::getBody().
   */
  public function getPlainBody() {
    return $this->getBodyWithFormat('plain');
  }

   /**
   * Get the body with the requested format.
   *
   * @param $format
   *   Either html or plain.
   *
   * @return
   *   The rendered mail body as a string.
   */
  protected function getBodyWithFormat($format) {
    // Switch to correct user and language context.
    $this->setContext();

    if ($cache = $this->cache->get('final', 'body:' . $format)) {
      return $cache;
    }

    $body = $this->buildBody($format);

    // Build message body, replace tokens.
    $body = token_replace($body, $this->getTokenContext(), array('sanitize' => FALSE));
    if ($format == 'plain') {
      // Convert HTML to text if requested to do so.
      $body = simplenews_html_to_text($body, $this->getCategory()->hyperlinks);
    }
    $this->cache->set('final', 'body:' . $format, $body);
    $this->resetContext();
    return $body;
  }

  /**
   * Builds the themed footer.
   *
   * @param $format
   *   (Optional) Set the format of this footer build, overrides the default
   *   format.
   */
  protected function buildFooter($format = NULL) {
    if (empty($format)) {
      $format = $this->getFormat();
    }

    if ($cache = $this->cache->get('build', 'footer:' . $format)) {
      return $cache;
    }

    // Build and buffer message footer
    $footer = theme('simplenews_newsletter_footer', array(
      'build' => $this->build($format),
      'category' => $this->getCategory(),
      'context' => $this->getTokenContext(),
      'key' => $this->getKey(),
      'language' => $this->getLanguage(),
      'format' => $format,
    ));
    $this->cache->set('build', 'footer:' . $format, $footer);
    return $footer;
  }

  /**
   * Implements SimplenewsSourceInterface::getFooter().
   */
  public function getFooter() {
    return $this->getFooterWithFormat($this->getFormat());
  }

  /**
   * Implements SimplenewsSourceInterface::getPlainFooter().
   */
  public function getPlainFooter() {
    return $this->getFooterWithFormat('plain');
  }

  /**
   * Get the footer in the specified format.
   *
   * @param $format
   *   Either html or plain.
   *
   * @return
   *   The footer for the requested format.
   */
  protected function getFooterWithFormat($format) {
    // Switch to correct user and language context.
    $this->setContext();
    if ($cache = $this->cache->get('final', 'footer:' . $format)) {
      return $cache;
    }
    $final_footer = token_replace($this->buildFooter($format), $this->getTokenContext(), array('sanitize' => FALSE));
    $this->cache->set('final', 'footer:' . $format, $final_footer);
    $this->resetContext();
    return $final_footer;
  }

  /**
   * Implements SimplenewsSourceInterface::getAttachments().
   */
  function getAttachments() {
    if ($cache = $this->cache->get('data', 'attachments')) {
      return $cache;
    }

    $attachments = array();
    $build = $this->build();
    $fids = array();
    foreach (field_info_instances('node', $this->node->type) as $field_name => $field_instance) {
      // @todo: Find a better way to support more field types.
      // Only add fields of type file which are enabled for the current view
      // mode as attachments.
      $field = field_info_field($field_name);
      if ($field['type'] == 'file' && isset($build[$field_name])) {

        if ($items = field_get_items('node', $this->node, $field_name)) {
          foreach ($items as $item) {
            $fids[] = $item['fid'];
          }
        }
      }
    }
    if (!empty($fids)) {
      $attachments = file_load_multiple($fids);
    }

    $this->cache->set('data', 'attachments', $attachments);
    return $attachments;
  }
}

/**
 * Abstract implementation of the source caching that does static caching.
 *
 * Subclasses need to implement the abstract function isCacheable() to decide
 * what should be cached.
 *
 * @ingroup source
 */
abstract class SimplenewsSourceCacheStatic implements SimplenewsSourceCacheInterface {

  /**
   * The simplenews source for which this cache is used.
   *
   * @var SimplenewsSourceNodeInterface
   */
  protected $source;

  /**
   * The cache identifier for the given source.
   */
  protected $cid;

  /**
   * The static cache.
   */
  protected static $cache = array();

  /**
   * Implements SimplenewsSourceNodeInterface::__construct().
   */
  public function __construct(SimplenewsSourceNodeInterface $source) {
    $this->source = $source;

    self::$cache = &drupal_static(__CLASS__, array());
  }

  /**
   * Returns the cache identifier for the current source.
   */
  protected function getCid() {
    if (empty($this->cid)) {
      $this->cid = $this->source->getNode()->nid . ':' . $this->source->getLanguage();
    }
    return $this->cid;
  }

  /**
   * Implements SimplenewsSourceNodeInterface::get().
   */
  public function get($group, $key) {
    if (!$this->isCacheable($group, $key)) {
      return;
    }

    if (isset(self::$cache[$this->getCid()][$group][$key])) {
      return self::$cache[$this->getCid()][$group][$key];
    }
  }

  /**
   * Implements SimplenewsSourceNodeInterface::set().
   */
  public function set($group, $key, $data) {
    if (!$this->isCacheable($group, $key)) {
      return;
    }

    self::$cache[$this->getCid()][$group][$key] = $data;
  }

  /**
   * Return if the requested element should be cached.
   *
   * @return
   *   TRUE if it should be cached, FALSE otherwise.
   */
  abstract function isCacheable($group, $key);
}

/**
 * Cache implementation that does not cache anything at all.
 *
 * @ingroup source
 */
class SimplenewsSourceCacheNone extends SimplenewsSourceCacheStatic {

  /**
   * Implements SimplenewsSourceCacheStatic::set().
   */
  public function isCacheable($group, $key) {
    return FALSE;
  }

}

/**
 * Source cache implementation that caches build and data element.
 *
 * @ingroup source
 */
class SimplenewsSourceCacheBuild extends SimplenewsSourceCacheStatic {

  /**
   * Implements SimplenewsSourceCacheStatic::set().
   */
  function isCacheable($group, $key) {

    // Only cache for anon users.
    if (user_is_logged_in()) {
      return FALSE;
    }

     // Only cache data and build information.
    return in_array($group, array('data', 'build'));
  }

}

/**
 * Example source implementation used for tests.
 *
 * @ingroup source
 */
class SimplenewsSourceTest implements SimplenewsSourceInterface {

  protected $format;

  public function __construct($format) {
    $this->format = $format;
  }

  public function getAttachments() {
    return array(
      array(
        'uri' => 'example://test.png',
        'filemime' => 'x-example',
        'filename' => 'test.png',
      ),
    );
  }

  public function getBody() {
    return $this->getFormat() == 'plain' ? $this->getPlainBody() : 'the body';
  }

  public function getFooter() {
    return $this->getFormat() == 'plain' ? $this->getPlainFooter() : 'the footer';
  }

  public function getPlainFooter() {
    return 'the plain footer';
  }

  public function getFormat() {
    return $this->format;
  }

  public function getFromAddress() {
    return 'test@example.org';
  }

  public function getFromFormatted() {
    return 'Test <test@example.org>';
  }

  public function getHeaders(array $headers) {
    $headers['X-Simplenews-Test'] = 'OK';
    return $headers;
  }

  public function getKey() {
    return 'node';
  }

  public function getLanguage() {
    return 'en';
  }

  public function getPlainBody() {
    return 'the plain body';
  }

  public function getRecipient() {
    return 'recipient@example.org';
  }

  public function getSubject() {
    return 'the subject';
  }

  public function getTokenContext() {
    return array();
  }
}
