<?php

/**
 * @file
 * Handles configuration for Feeds: Entity Translation.
 */

/**
 * Adds configuration for Feeds: Entity Translation.
 */
class FeedsEntityTranslation extends FeedsImporter {

  /**
   * Return defaults for feed configuration.
   */
  public function configDefaults() {
   $defaults = parent::configDefaults();
    $defaults['feeds_et'] = array(
      'default_language' => 'und',
    );
    return $defaults;
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $config = $this->getConfig();
    die(print_r($config));
    $languages = language_list('enabled');
    $options = array();

    foreach ($languages[1] as $language_code => $language_info) {
      $options[$language_code] = $language_info->name;
    }
    $form = array();
    $form['feeds_et'] = array(
      '#type' => 'fieldset',
      '#title' => t('Feeds: Entity Translation'),
    );
    $form['feeds_et']['feeds_et_default_language'] = array(
      '#type' => 'select',
      '#title' => t('Default language of entities when imported'),
      '#description' => t(''),
      '#options' => $language_code,
      '#default_value' => $config['feeds_et_default_language'],
    );
    return parent::configForm($form_state) + $form;
  }
}
