<?php

/**
 * @file
 * Summon plugins and render the media browser.
 */

/**
 * Media browser page callback.
 */
function media_browser($selected = NULL) {
  $output = array();
  $output['#attached']['library'][] = array('media', 'media_browser_page');

  // Build out browser settings. Permissions- and security-related behaviors
  // should not rely on these parameters, since they come from the HTTP query.
  // @TODO make sure we treat parameters as user input.
  $params = drupal_get_query_parameters() + array(
    'types' => array(),
    'multiselect' => FALSE,
  );

  // Transform text 'true' and 'false' to actual booleans.
  foreach ($params as $k => $v) {
    if ($v === 'true') { $params[$k] = TRUE; }
    elseif ($v === 'false') { $params[$k] = FALSE; }
  }

  array_walk_recursive($params, '_media_recursive_check_plain');
  $params = media_set_browser_params($params);

  // If one or more files have been selected, the browser interaction is now
  // complete. Return empty page content to the dialog which now needs to close,
  // but populate Drupal.settings with information about the selected files.
  if (isset($params['fid'])) {
    $fids = is_array($params['fid']) ? $params['fid'] : array($params['fid']);
    if (!is_numeric($fids[0])) {
      throw new Exception('Error selecting media, fid param is not an fid or an array of fids');
    }
    $files = file_load_multiple($fids);
    foreach ($files as $file) {
      media_browser_build_media_item($file);
    }
    $setting = array('media' => array('selectedMedia' => array_values($files)));
    drupal_add_js($setting, 'setting');
    return $output;
  }

  ctools_include('plugins');
  $plugins = ctools_get_plugins('media', 'browser');

  // Allow parameters to provide a list of enabled or disabled media browser plugins.
  if (!empty($params['enabledPlugins'])) {
    $plugins = array_intersect_key($plugins, array_fill_keys($params['enabledPlugins'], 1));
  }
  elseif (!empty($params['disabledPlugins'])) {
    $plugins = array_diff_key($plugins, array_fill_keys($params['disabledPlugins'], 1));
  }

  // Render plugins.
  $plugin_output = array();
  foreach ($plugins as $key => &$plugin) {
    if ($plugin_class = ctools_plugin_get_class($plugin, 'handler')) {
      $access = TRUE;
      if (isset($plugin['access callback']) && ($callback = $plugin['access callback']) && function_exists($callback)) {
        $arguments = (isset($plugin['access arguments']) ? $plugin['access arguments'] : array());
        $access = call_user_func_array($callback, $arguments);
      }
      if ($access) {
        $plugin_obj = new $plugin_class($plugin, $params);
        $plugin_output[$key] = $plugin_obj->view() + array(
          '#title' => $plugin['title'],
          '#weight' => isset($plugin['weight']) ? $plugin['weight'] : 0,
        );
      }
    }

    // We need to get a submit and cancel button on each tab. If the plugin
    // is not returning a form element we need to add a submit button.
    // This is a fairly broad assumption.
    if (empty($plugin_output[$key]['#form']) && ! empty($plugin_output[$key]['#markup'])) {
      $plugin_output[$key]['#markup'] .= l(t('Submit'), '', array(
        'attributes' => array('class' => array('button', 'fake-submit', $key))
      ));
      $plugin_output[$key]['#markup'] .= l(t('Cancel'), '', array(
      'attributes' => array('class' => array('button', 'fake-cancel', $key))
      ));
    }

    // I'm not sure if it is ever the case that a plugin form will ever have
    // the correct cancel button so we add it here. Put it inline with the
    // current submit button. This is a fairly broad assumption.
    if (!empty($plugin_output[$key]['form']['actions']) && !isset($plugin_output[$key]['form']['actions']['cancel'])) {
      $plugin_output[$key]['form']['actions']['cancel'] = array(
        '#type' => 'link',
        '#title' => t('Cancel'),
        '#href' => '',
        '#attributes' => array('class' => array('button', 'fake-cancel', $key)),
        '#weight' => 100,
      );
    }
  }

  // Allow modules to change the tab names or whatever else they want to change
  // before we render.  Perhaps this should be an alter on the theming function
  // that we should write to be making the tabs.
  drupal_alter('media_browser_plugins', $plugin_output);

  $tabs = array(); // List of tabs to render.

  $settings = array('media' => array('browser' => array()));
  $browser_settings =& $settings['media']['browser'];

  //@todo: replace with Tabs module if it gets upgraded.
  foreach (element_children($plugin_output, TRUE) as $key) {
    $plugin =& $plugin_output[$key];

    //Add any JS settings
    $browser_settings[$key] = isset($plugin['#settings']) ? $plugin['#settings'] : array();

    // If this is a "ajax" style tab, add the href, otherwise an id.
    $href = isset($plugin['#callback']) ? $plugin['#callback'] : "#media-tab-$key";
    $tabs[] = "<a href='$href'><span>" . check_plain($plugins[$key]['title']) . "</span></a>";

    // Create a div for each tab's content.
    $plugin['#prefix'] = <<<EOS
    <div class="media-browser-tab" id="media-tab-$key">
EOS;
    $plugin['#suffix'] = <<<EOS
    </div>
    <!-- End #media-tab-$key -->
EOS;
  }

  drupal_add_js($settings, 'setting');

  $output['tabset'] = array(
    '#prefix' => '<div id="media-browser-tabset">',
    '#suffix' => '</div>',
  );
  $output['tabset']['list'] = array(
    '#markup' => theme('item_list', array('items' => $tabs)),
  );

  $output['tabset']['plugins'] = $plugin_output;
  return $output;
}

/**
 * Provides a singleton of the params passed to the media browser.
 *
 * This is useful in situations like form alters because callers can pass
 * id="wysiywg_form" or whatever they want, and a form alter could pick this up.
 * We may want to change the hook_media_browser_plugin_view() implementations to
 * use this function instead of being passed params for consistency.
 *
 * It also offers a chance for some meddler to meddle with them.
 *
 * @param array $params
 *   An array of parameters provided when a media_browser is launched.
 *
 * @see media_browser()
 */
function media_set_browser_params(array $params = NULL) {
  $stored_params = &drupal_static(__FUNCTION__, array());

  if (isset($params)) {
    $stored_params = $params;
    // Allow modules to alter the parameters.
    drupal_alter('media_browser_params', $stored_params);
  }

  return $stored_params;
}

/**
 * For sanity in grammar.
 *
 * @see media_set_browser_params()
 */
function media_get_browser_params() {
  return media_set_browser_params();
}

/**
 * AJAX Callback function to return a list of media files
 *
 * @TODO this should probably live with the 'library' media browser plugin.
 */
function media_browser_list() {
  $params = drupal_get_query_parameters();
  // How do we validate these?  I don't know.
  // I think PDO should protect them, but I'm not 100% certain.
  array_walk_recursive($params, '_media_recursive_check_plain');

  $types = isset($params['types']) ? $params['types'] : NULL;
  $url_include_patterns = isset($params['url_include_patterns']) ? $params['url_include_patterns'] : NULL;
  $url_exclude_patterns = isset($params['url_exclude_patterns']) ? $params['url_exclude_patterns'] : NULL;

  $start = isset($params['start']) ? $params['start'] : 0;
  $limit = isset($params['limit']) ? $params['limit'] : media_variable_get('browser_pager_limit');

  $query = db_select('file_managed', 'f');
  $query->fields('f', array('fid'));
  $query->range($start, $limit);
  $query->orderBy('f.timestamp', 'DESC');

  // Add conditions based on file type *or* allowed extensions.
  $condition = $query;
  if (!empty($types) && !empty($params['file_extensions'])) {
    $condition = db_or();
  }
  if (!empty($types)) {
    $condition->condition('f.type', $types, 'IN');
  }
  if (!empty($params['file_extensions'])) {
    $extensions = array_filter(explode(' ', $params['file_extensions']));
    foreach ($extensions as $extension) {
      $condition->condition('f.uri', '%' . db_like('.' . $extension), 'LIKE');
    }
  }
  if ($condition instanceof DatabaseCondition) {
    $query->condition($condition);
  }

  if ($url_include_patterns) {
    $query->condition('f.uri', '%' . db_like($url_include_patterns) . '%', 'LIKE');
    // Insert stream related restrictions here.
  }
  if ($url_exclude_patterns) {
    $query->condition('f.uri', '%' . db_like($url_exclude_patterns) . '%', 'NOT LIKE');
  }

  // @todo Implement granular editorial access: http://drupal.org/node/696970.
  //   In the meantime, protect information about private files from being
  //   discovered by unprivileged users. See also media_view_page().
  if (!user_access('administer files')) {
    $query->condition('f.uri', db_like('private://') . '%', 'NOT LIKE');
  }

  $query->condition('f.status', FILE_STATUS_PERMANENT);

  foreach (array_keys(media_get_hidden_stream_wrappers()) as $name) {
    $query->condition('f.uri', db_like($name . '://') . '%', 'NOT LIKE');
  }

  $fids = $query->execute()->fetchCol();
  $files = file_load_multiple($fids);
  foreach ($files as $file) {
    media_browser_build_media_item($file);
  }

  drupal_json_output(array('media' => array_values($files)));
  exit();
}

/**
 * Silly function to recursively run check_plain on an array.
 *
 * There is probably something in core I am not aware of that does this.
 *
 * @param $value
 * @param $key
 */
function _media_recursive_check_plain(&$value, $key) {
  $value = check_plain($value);
}

/**
 * Attaches media browser javascript to an element.
 *
 * @param $element
 *  The element array to attach to.
 */
function media_attach_browser_js(&$element) {
  $javascript = media_browser_js();
  foreach ($javascript as $key => $definitions) {
    foreach ($definitions as $definition) {
      $element['#attached'][$key][] = $definition;
    }
  }
}

/**
 * Helper function to define browser javascript.
 */
function media_browser_js() {
  $settings = array(
    'browserUrl' => url('media/browser',
      array('query' => array('render' => 'media-popup'))),
    'styleSelectorUrl' => url('media/-media_id-/format-form',
      array('query' => array('render' => 'media-popup'))),
  );

  $js = array(
    'library' => array(
      array('media', 'media_browser'),
    ),
    'js' => array(
      array(
       'data' => array('media' => $settings),
       'type' => 'setting',
      ),
    ),
  );
  return $js;
}

/**
 * Menu callback for testing the media browser
 */
function media_browser_testbed($form) {
  media_attach_browser_js($form);

  $form['test_element'] = array(
    '#type' => 'media',
    '#media_options' => array(
      'global' => array(
        'types' => array('video', 'audio'),
      ),
    )
  );

  $launcher = '<a href="#" id="launcher"> Launch Media Browser</a>';

  $form['options'] = array(
    '#type' => 'textarea',
    '#title' => 'Options (JSON)',
    '#rows' => 10,
  );

  $form['launcher'] = array(
    '#markup' => $launcher,
  );

  $form['result'] = array(
    '#type' => 'textarea',
    '#title' => 'Result',
  );


  $js = <<<EOF
    Drupal.behaviors.mediaTest = {
    attach: function(context, settings) {
      var delim = "---";
      var recentOptions = [];
      var recentOptionsCookie = jQuery.cookie("recentOptions");
      if (recentOptionsCookie) {
        recentOptions = recentOptionsCookie.split("---");
      }

      var recentSelectBox = jQuery('<select id="recent_options" style="width:100%"></select>').change(function() { jQuery('#edit-options').val(jQuery(this).val())});

      jQuery('.form-item-options').append('<label for="recent_options">Recent</a>');
      jQuery('.form-item-options').append(recentSelectBox);
      jQuery('.form-item-options').append(jQuery('<a href="#">Reset</a>').click(function() {alert('reset'); jQuery.cookie("recentOptions", null); window.location.reload(); }));

      jQuery.each(recentOptions, function (idx, val) {
        recentSelectBox.append(jQuery('<option></option>').val(val).html(val));
      });


      jQuery('#launcher').click(function () {
        jQuery('#edit-result').val('');
        var options = {};
        var optionsTxt = jQuery('#edit-options').val();
        if (optionsTxt) {
          // Store it in the recent box
          recentOptionsCookie += "---" + optionsTxt
          jQuery.cookie("recentOptions", recentOptionsCookie, { expires: 7 });
          recentSelectBox.append(jQuery('<option></option>').val(optionsTxt).html(optionsTxt));
          options = eval('(' + optionsTxt + ')');
        }
        Drupal.media.popups.mediaBrowser(Drupal.behaviors.mediaTest.mediaSelected, options);
        return false;
      });
    },

    mediaSelected: function(selectedMedia) {
      var result = JSON.stringify(selectedMedia);
        jQuery('#edit-result').val(result);
    }
  }

EOF;

  drupal_add_js($js, array('type' => 'inline'));
  return $form;
}

/**
 * Adds properties to the passed in file that are needed by the media browser JS code.
 */
function media_browser_build_media_item($file) {
  $preview = media_get_thumbnail_preview($file);
  $file->preview = drupal_render($preview);
  $file->url = file_create_url($file->uri);
}

/**
 * Base class for media browser plugins.
 *
 * This is basically just API documentation.
 */
class MediaBrowserPlugin {

  // Plugin info array.
  var $info;

  // Browser instance parameters.
  var $params;

  /**
   * Set up the plugin class.
   *
   * @param $info
   *   An array of plugin info from hook_media_browser_plugin_info()
   *   implementations.
   * @param $params
   *   An array of parameters which came in is $_GET['params']. The expected
   *   parameters are still being defined.
   *   - types: array of media types to support
   *   - multiselect: boolean; TRUE enables multiselect
   */
  function __construct($info, $params) {
    $this->info = $info;
    $this->params = $params;
  }

  /**
   * Provide a render array to display the plugin in a media browser.
   *
   * This render array will be a jQuery tab in the media browser.
   *
   * Some elements are special:
   *  - #settings: Drupal.settings.media.browser.$key (where key is the array key).
   *  - #callback: If provided, will make the tab an "ajax" tab.
   *
   * Example:
   * @code
   *  return array(
   *    '#attached' => array(
   *      'js' => array(
   *         drupal_get_path('module', 'media') . '/js/plugins/media.library.js',
   *       ),
   *     ),
   *     '#settings' => array(
   *       'viewMode' => 'thumbnails',
   *       'getMediaUrl' => url('media/browser/list'),
   *     ),
   *     '#markup' => '<div>Library goes here</div>',
   * );
   * @endcode
   *
   * @return
   *  Renderable array.
   */
  function view() {
    return array('#markup' => '');
  }

}
