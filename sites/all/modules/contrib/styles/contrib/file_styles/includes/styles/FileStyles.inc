<?php

/**
 * @file styles/contrib/file_styles/includes/styles/FileStyles.inc
 * Styles definitions for file styles.
 */

class FileStyles extends StylesDefault {
  public $fid;
  public $uri;
  public $title;
  public $alt;

  public $width;
  public $height;

  public $float;

  public $imagecachePreset = '';
  public $link;

  public $wrapperType = 'span';
  public $classes = array('styles', 'file-styles');

  public $streamWrapperInstance;

  function getStreamWrapperInstance() {
    $streamWrapperInstance = $this->get('stream_wrapper_instance');
    if (!isset($streamWrapperInstance)) {
      $streamWrapperInstance = $this->setStreamWrapperInstance(file_stream_wrapper_get_instance_by_uri($this->getUri()));
    }
    return $streamWrapperInstance;
  }
  function setStreamWrapperInstance($value) {
    return $this->set('stream_wrapper_instance', $value);
  }

  function getFid() {
    return $this->get('fid');
  }
  function setFid($value) {
    return $this->set('fid', $value);
  }
  function setObject($value) {
    $variables = $this->getVariables();
    if (!isset($value->override) && isset($variables['entity']) && isset($variables['entity']->override)) {
      $value->override = $variables['entity']->override;
    }
    return $this->set('object', $value);
  }
  function getUri() {
    $uri = $this->get('uri');
    if (isset($uri)) {
      return $uri;
    }
    // If we don't have a URI yet, then try to find it from the object.
    $object = $this->getObject();
    if (isset($object->uri)) {
      return $object->uri;
    }
  }
  function setUri($value) {
    return $this->set('uri', $value);
  }
  function getTitle() {
    return $this->override('title');
  }
  function setTitle($value) {
    // @TODO: Token support.
    return $this->set('title', $value);
  }
  function getAlt() {
    return $this->override('alt');
  }
  function setAlt($value) {
    return $this->set('alt', $value);
  }
  function getWidth() {
    return $this->override('width');
  }
  function setWidth($value) {
    return $this->set('width', $value);
  }
  function getHeight() {
    return $this->override('height');
  }
  function setHeight($value) {
    return $this->set('height', $value);
  }
  function getImageStyle() {
    return $this->get('imageStyle');
  }
  function setImageStyle($value) {
    return $this->set('imageStyle', $value);
  }
  function getLink() {
    return $this->get('link');
  }
  function setLink($value) {
    return $this->set('link', $value);
  }
  function getFloat() {
    return $this->override('float');
  }
  function setFloat($value) {
    if ($value) {
      $this->setPrefix('<span class="styles file-styles file-styles-float-'. filter_xss($value) .'">');
    }
    else {
      $this->setPrefix('<span class="styles file-styles">');
    }
    return $this->set('float', $value);
  }

  function setImageUri($value) {
    return $this->set('imageUri', $value);
  }
  function getImageUri() {
    if ($imageUri = $this->get('imageUri')) {
      return $imageUri;
    }
    return $this->getUri();
  }

  // Allow WYSIWYG to override the values.
  public function override($attribute) {
    $object = $this->getObject();
    if (isset($object->override) && is_array($object->override) && isset($object->override[$attribute])) {
      return $object->override[$attribute];
    }
    return $this->get($attribute);
  }

  // Effect callbacks when rendering.
  function thumbnail($effect) {
    $attributes = array();
    $width = $this->getWidth();
    $height = $this->getHeight();
    if (isset($width)) {
      $attributes['width'] = $width;
    }
    if (isset($height)) {
      $attributes['height'] = $height;
    }

    // We have a .media-image class for WYSIWYG.
    // We can't used ->getClass, because that's restricted.
    $class = $this->override('class');
    if (isset($class)) {
      $attributes['class'] = $class;
    }

    // Set any WYSIWYG prescribed styles.
    $style = $this->override('style');
    $attributes['style'] = isset($style) ? ($style . ';') : '';
    foreach (array('border-width', 'border-style', 'display', 'float', 'margin', 'margin-top', 'margin-right', 'margin-bottom', 'margin-left') as $property) {
      $value = $this->override($property);
      if (isset($value)) {
        $attributes['style'] .= $property . ':' . $value . ';';
      }
    }

    if ($attributes['style'] == '') {
      unset($attributes['style']);
    }

    // Set any additional prescribed attributes.
    // @todo Remove this as a hard-coded list. Note that not everything in
    //   $this->getObject()->override is an HTML attribute.
    foreach (array('id', 'class', 'dir', 'lang') as $attribute) {
      $value = $this->override($attribute);
      if (!empty($value)) {
        $attributes[$attribute] = $value;
      }
    }

    if ($imageUri = $this->getImageUri()) {
      $this->setOutput(theme('file_styles_image', array('image_uri' => $imageUri, 'attributes' => $attributes, 'alt' => $this->getAlt(), 'title' => $this->getTitle(), 'image_style' => $this->getImageStyle(), 'instance' => $this)));
      // Honor any applied links.
      if ($link = $this->getLink()) {
        $this->setOutput(l($this->getOutput(), $link, array('html' => TRUE)));
      }
    }
  }

  function resize($effect) {
    if (isset($effect['width'])) {
      $this->setWidth($effect['width']);
    }
    if (isset($effect['height'])) {
      $this->setHeight($effect['height']);
    }
  }

  function float($effect) {
    $this->setFloat($effect['float']);
  }

  function imageStyle($effect) {
    $this->setImageStyle($effect['image_style']);
  }

  function linkToMedia($effect) {
    // If we're using the media module, then link to its media page.
    if (module_exists('media') && ($fid = $this->getFid())) {
      $link = $this->setLink('media/'. $fid);
    }
    else {
      // Link to the file's external url.
      $uri = $this->getUri();
      $stream_wrapper = file_stream_wrapper_get_instance_by_uri($uri);
      $url = $stream_wrapper->getExternalUrl();
      $link = $this->setLink($url);
    }
    if (!$this->getOutput() && ($title = $this->getTitle())) {
      $this->setOutput(l($title, $link));
    }
  }

  function linkToPath($effect) {
    $link = $this->setLink($effect['path']);
    if (!$this->getOutput() && $link && ($title = $this->getTitle())) {
      $this->setOutput(l($title, $link));
    }
  }

  function teaser($effect) {
    $this->set('file', file_load($this->getFid()));
    $this->setOutput(file_view($this->get('file')));
  }

  /**
   * Override the getObject() method to account for media fields that store
   * their data in a sub-property rather than the field itself.
   */
  function getObject() {
    $object = $this->get('object');
    return !empty($object->file) ? $object->file : $object;
  }

  /**
   * Override the render function to always display a thumbnail in the wysiwyg.
   */
  public function render($reset = FALSE) {
    $object = $this->getObject();
    if (isset($object->override) && is_array($object->override) && isset($object->override['wysiwyg']) && $object->override['wysiwyg']) {
      // Disregard any links pushed ahead.
      $this->pushEffect(array('name' => 'linkToPath', 'settings' => array('path' => NULL)));
      // We ensure that the thumbnail will be applied at the end.
      $this->pushEffect(array('name' => 'thumbnail', 'settings' => array()));
    }
    return parent::render($reset);
  }

}
