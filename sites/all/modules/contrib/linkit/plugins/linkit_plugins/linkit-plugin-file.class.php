<?php
/**
 * @file
 * Define Linkit file plugin class.
 */
class LinkitPluginFile extends LinkitPluginEntity {

  /**
   * Set the plugin ui title.
   */
  function ui_title() {
    return t('Managed files');
  }

  /**
   * Set the plugin ui description.
   */
  function ui_description() {
    return t('Extend Linkit with file support (Managed files).');
  }

  /**
   * Build an URL based in the path and the options.
   */
  function buildPath($entity, $options = array()) {
    return file_create_url($entity->uri);
  }

  /**
   * Build the search row description.
   *
   * If there is a "result_description", run it thro token_replace.
   *
   * @param object $data
   *   An entity object that will be used in the token_place function.
   *
   * @see token_replace()
   */
  function buildDescription($data) {
    $description_array = array();
    //Get image info.
    $imageinfo = image_get_info($data->uri);

    if ($this->conf['image_extra_info']['thumbnail']) {
      $image = $imageinfo ? theme_image_style(array(
          'width' => $imageinfo['width'],
          'height' => $imageinfo['height'],
          'style_name' => 'linkit_thumb',
          'path' => $data->uri,
        )) : '';
    }

    if ($this->conf['image_extra_info']['dimensions'] && !empty($imageinfo)) {
      $description_array[] = $imageinfo['width'] . 'x' . $imageinfo['height'] . 'px';
    }

    $description_array[] = parent::buildDescription($data);

    if ($this->conf['show_scheme']) {
      $description_array[] = file_uri_scheme($data->uri) . '://';
    }

    $description = (isset($image) ? $image : '') . implode('<br />' , $description_array);

    return $description;
  }

  /**
   * Adds the file scheme to the group if "group_by_scheme" is activated.
   */
  function buildGroup($entity) {
    // The the standard group name.
    $group = parent::buildGroup($entity);
    // Add the scheme.
    if ($this->conf['group_by_scheme']) {
      // Get all stream wrappers.
      $stream_wrapper = file_get_stream_wrappers();
      $group .= ' · ' . $stream_wrapper[file_uri_scheme($entity->uri)]['name'];
    }
    return $group;
  }

  /**
   * Start a new EntityFieldQuery instance.
   */
  function getQueryInstance() {
    // Call the parent getQueryInstance method.
    parent::getQueryInstance();
    // We only what permanent files.
    $this->query->propertyCondition('status', FILE_STATUS_PERMANENT);
  }

  /**
   * Generate a settings form for this handler.
   * Uses the standard Drupal FAPI.
   * The element will be attached to the "data" key.
   *
   * @return
   *   An array containing any custom form elements to be displayed in the
   *   profile editing form.
   */
  function buildSettingsForm() {
    $form = parent::buildSettingsForm();

    $form['entity:file']['show_scheme'] = array(
      '#title' => t('Show file scheme'),
      '#type' => 'checkbox',
      '#default_value' => isset($this->conf['show_scheme']) ? $this->conf['show_scheme'] : array()
    );

    $form['entity:file']['group_by_scheme'] = array(
      '#title' => t('Group files by scheme'),
      '#type' => 'checkbox',
      '#default_value' => isset($this->conf['group_by_scheme']) ? $this->conf['group_by_scheme'] : array(),
    );

    $image_extra_info_options = array(
      'thumbnail' => t('Show thumbnails <em>(using the image style !linkit_thumb_link)</em>', array('!linkit_thumb_link' => l('linkit_thumb', 'admin/config/media/image-styles/edit/linkit_thumb'))),
      'dimensions' => t('Show pixel dimensions'),
    );

    $form['entity:file']['image_extra_info'] = array(
      '#title' => t('Images'),
      '#type' => 'checkboxes',
      '#options' => $image_extra_info_options,
      '#default_value' => isset($this->conf['image_extra_info']) ? $this->conf['image_extra_info'] : array('thumbnail', 'dimensions'),
    );

    return $form;
  }


}