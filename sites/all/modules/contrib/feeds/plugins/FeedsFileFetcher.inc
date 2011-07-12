<?php

/**
 * @file
 * Home of the FeedsFileFetcher and related classes.
 */

/**
 * Definition of the import batch object created on the fetching stage by
 * FeedsFileFetcher.
 */
class FeedsFileBatch extends FeedsImportBatch {
  protected $file_path;

  /**
   * Constructor.
   */
  public function __construct($file_path, $feed_nid = 0) {
    $this->file_path = $file_path;
    parent::__construct('', $feed_nid);
  }

  /**
   * Implementation of FeedsImportBatch::getRaw();
   */
  public function getRaw() {
    return file_get_contents(realpath($this->file_path));
  }

  /**
   * Implementation of FeedsImportBatch::getFilePath().
   */
  public function getFilePath() {
    if (!file_exists($this->file_path)) {
      throw new Exception(t('File @filepath is not accessible.', array('@filepath' => $this->file_path)));
    }
    return $this->file_path;
  }
}

/**
 * Fetches data via HTTP.
 */
class FeedsFileFetcher extends FeedsFetcher {

  /**
   * Implementation of FeedsFetcher::fetch().
   */
  public function fetch(FeedsSource $source) {
    $source_config = $source->getConfigFor($this);
    return new FeedsFileBatch($source_config['source'], $source->feed_nid);
  }

  /**
   * Source form.
   */
  public function sourceForm($source_config) {
    $form = $info = array();
    if (!empty($source_config['source']) && file_exists($source_config['source'])) {
      $info = array(
        'path' => $source_config['source'],
        'size' => filesize(realpath($source_config['source'])),
      );
      if (module_exists('mimedetect')) {
        $file = new stdClass;
        $file->filepath = realpath($source_config['source']);
        $info['mime'] = mimedetect_mime($file);
      }
    }
    $form['source'] = array(
      '#type' => empty($this->config['direct']) ? 'value' : 'textfield',
      '#title' => t('File'),
      '#description' => t('Specify a file in the site\'s file system path or upload a file below.'),
      '#default_value' => empty($source_config['source']) ? '' : $source_config['source'],
    );
    $form['upload'] = array(
      '#type' => 'file',
      '#title' => empty($this->config['direct']) ? t('File') : NULL,
      '#description' => empty($source_config['source']) ? t('Select the file to be imported from your local system.') : t('Select a different file to be imported from your local system.'),
      '#theme' => 'feeds_upload',
      '#file_info' => $info,
      '#size' => 10,
    );
    return $form;
  }

  /**
   * Override parent::sourceFormValidate().
   */
  public function sourceFormValidate(&$values) {
    $feed_dir = file_directory_path() .'/feeds';
    file_check_directory($feed_dir, TRUE);

    // If there is a file uploaded, save it, otherwise validate input on
    // file.
    if ($file = file_save_upload('feeds', array(), $feed_dir)) {
      file_set_status($file, FILE_STATUS_PERMANENT);
      $values['source'] = $file->filepath;
    }
    elseif (empty($values['source'])) {
      form_set_error('feeds][source', t('Upload a file first.'));
    }
    // If a file has not been uploaded and $values['source'] is not empty, make
    // sure that this file is within Drupal's files directory as otherwise
    // potentially any file that the web server has access could be exposed.
    elseif (!file_check_location($values['source'], file_directory_path())) {
      form_set_error('feeds][source', t('File needs to point to a file in your Drupal file system path.'));
    }
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'direct' => FALSE,
    );
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = array();
    $form['direct'] = array(
      '#type' => 'checkbox',
      '#title' => t('Supply path to file directly'),
      '#description' => t('For experts. If checked users can specify a path to a file when importing rather than uploading a file. This is useful when files to be imported are already present on server.'),
      '#default_value' => $this->config['direct'],
    );
    return $form;
  }
}
