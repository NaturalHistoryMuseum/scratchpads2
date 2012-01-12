<?php

/**
 * @file
 * Plugin that adds custom text to the current search block.
 */

/**
 * Extension of CurrentSearchItem that displays all active items.
 */
class CurrentSearchItemText extends CurrentSearchItem {

  /**
   * Implements CurrentSearchItem::execute().
   */
  public function execute(FacetapiAdapter $adapter) {
    $text = $this->translate('text', $this->settings['text']);
    $data = array('facetapi_adapter' => $adapter);
    $variables = array(
      'text' => filter_xss(token_replace($text, $data)),
      'wrapper' => $this->settings['wrapper'],
      'element' => $this->settings['element'],
      'css' => $this->settings['css'],
      'class' => current_search_get_classes($this->settings['classes'], $data),
      'options' => array('html' => TRUE),
    );
    return array('#markup' => theme('current_search_text', $variables));
  }

  /**
   * Implements CurrentSearchItem::settingsForm().
   */
  public function settingsForm(&$form, &$form_state) {

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text'),
      '#default_value' => $this->settings['text'],
      '#maxlength' => 255,
      '#description' => t('Custom text displayed in the text box. Token replacement patterns are allowed.'),
    );

    // Adds HTML wrapper elements.
    $this->wrapperForm($form, $form_state);

    // Adds token tree.
    $form['tokens'] = $this->getTokenTree(array('facetapi_results'));
  }

  /**
   * Implements CurrentSearchItem::getDefaultSettings().
   */
  public function getDefaultSettings() {
    return array('text' => '') + $this->wrapperDefaults();
  }
}
