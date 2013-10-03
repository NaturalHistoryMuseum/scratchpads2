<?php

/**
 * @file
 * Provides a wrapper class for media_internet providers.
 */

/**
 * Wraps a media_internet provider to implement the interface of
 * MediaFeedsProvider.
 */
class MediaFeedsInternetProvider extends MediaFeedsProvider {

  protected $provider;

  /**
   * Construcor.
   *
   * @param $provider
   *   The media_internet provider to wrap for workarounds.
   */
  public function __construct($value, $config = array()) {
    parent::__construct($value, $config);
    $this->provider = media_internet_get_provider($this->value);
  }

  /**
   * If required, providers can validate the embedCode.
   */
  public function validate() {
    return $this->provider->validate();
  }

  /**
   * Get the file object that can be validated and saved.
   */
  public function getFileObject() {
    return $this->provider->getFileObject();
  }

  /**
   * Let the provider save the file.
   */
  public function save() {
    return $this->provider->save();
  }
}
