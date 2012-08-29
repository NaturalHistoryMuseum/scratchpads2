<?php
/**
 * @file
 * Drupal module stream wrapper class (module://{module}/{resource}).
 *
 * Provides support for accessing files in module directories.
 */
class ImageCacheActionsModuleStreamWrapper extends DrupalLocalStreamWrapper {
  /**
   * Parses an uri into its separate components.
   *
   * Example:
   * module://my_module/images/mask.png will return
   * array('module', 'my_module', 'images/mask.png').
   *
   * @return array
   *   An array of strings containing the separate parts of the uri:
   *   - scheme (module)
   *   - module name
   *   - resource name
   */
  protected function parseUri() {
    $scheme = '';
    $module = '';
    $component = '';
    if (!empty($this->uri)) {
      list($scheme, $target) = explode('://', $this->uri, 2);
      if (!empty($target)) {
        $target = trim($target, '\/');
        list($module, $resource) = explode('/', $target, 2);
      }
    }
    return array($scheme, $module, $resource);
  }

  /**
   * Implements abstract public function getDirectoryPath()
   */
  public function getDirectoryPath() {
    list($scheme, $module, $component) = $this->parseUri();
    return drupal_get_path('module', $module);
  }

  /**
   * Returns the local writable target of the resource within the stream.
   *
   * @see DrupalLocalStreamWrapper::getTarget()
   */
  protected function getTarget($uri = NULL) {
    if (!empty($uri)) {
      $this->setUri($uri);
    }
    list($scheme, $module, $component) = $this->parseUri();
    return $component;
  }

  /**
   * Returns the canonical absolute path of the URI, if possible.
   *
   * @see DrupalLocalStreamWrapper::getLocalPath()
   */
  protected function getLocalPath($uri = NULL) {
    if (!empty($uri)) {
      $this->setUri($uri);
    }
    return parent::getLocalPath();
  }

  /**
   * Returns the local writable target of the resource within the stream.
   *
   * Overrides getExternalUrl().
   *
   * Return the HTML URI of a public file.
   */
  function getExternalUrl() {
    $path = str_replace('\\', '/', $this->getTarget());
    return $GLOBALS['base_url'] . '/' . $this->getDirectoryPath() . '/' . drupal_encode_path($path);
  }

  /*
   * Unsupported methods, modes and operations because we are read-only.
   */
  function chmod($mode) {
    return $this->unsupportedError('chmod', STREAM_REPORT_ERRORS);
  }

  public function stream_open($uri, $mode, $options, &$opened_path) {
    if (!in_array($mode, array('r'))) {
      return $this->unsupportedError("stream_open(mode = '$mode')", $options);
    }
    return parent::stream_open($uri, $mode, $options, $opened_path);
  }

  public function stream_lock($operation) {
    if (in_array($operation, array(LOCK_EX))) {
      return $this->unsupportedError('stream_lock(LOCK_EX)', $options);
    }
    return parent::stream_lock($operation);
  }

  public function stream_write($data) {
    return $this->unsupportedError('stream_write', STREAM_REPORT_ERRORS);
  }

  public function stream_flush() {
    return $this->unsupportedError('stream_flush', STREAM_REPORT_ERRORS);
  }

  public function unlink($uri) {
    return $this->unsupportedError('unlink', STREAM_REPORT_ERRORS);
  }

  public function rename($from_uri, $to_uri) {
    return $this->unsupportedError('rename', STREAM_REPORT_ERRORS);
  }

  public function mkdir($uri, $mode, $options) {
    return $this->unsupportedError('mkdir', $options);
  }

  public function rmdir($uri, $options) {
    return $this->unsupportedError('rmdir', $options);
  }

  public function unsupportedError($method, $options) {
    if ($options & STREAM_REPORT_ERRORS) {
      drupal_set_message("ImageCacheActionsModuleStreamWrapper is read-only and does not support $method", 'error');
      watchdog(WATCHDOG_ERROR, "ImageCacheActionsModuleStreamWrapper is read-only and does not support $method");
    }
    return FALSE;
  }
}
