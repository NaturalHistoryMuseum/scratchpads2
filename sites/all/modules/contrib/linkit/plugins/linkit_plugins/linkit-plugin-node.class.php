<?php
/**
 * @file
 * Define Linkit node plugin class.
 */
class LinkitPluginNode extends LinkitPluginEntity {

  /**
   * If we are inclunding unpublished nodes in the search results, and this
   * entity is unpublished, we need to tell the user this.
   *
   * @return a string which will be used as the search result label for this
   *   item.
   */
  function buildLabel($entity) {
    $label = parent::buildLabel($entity);
    if ($this->conf['include_unpublished'] && $entity->status == NODE_NOT_PUBLISHED) {
      $label .= '<span class="status"> - ' . t('unpublished') . '</span>';
    }
    return $label;
  }

  /**
   * Returns a string with CSS classes that will be added to the search result
   * row for this item.
   */
  function buildRowClass($entity) {
    if ($this->conf['include_unpublished'] && $entity->status == NODE_NOT_PUBLISHED) {
      return 'unpublished-node';
    }
  }

  /**
   * Start a new EntityFieldQuery instance.
   */
  function getQueryInstance() {
    // Call the parent getQueryInstance method.
    parent::getQueryInstance();
    // If we don't want to include unpublished nodes, add a condition on status.
    if ($this->conf['include_unpublished'] == 0) {
      $this->query->propertyCondition('status', NODE_PUBLISHED);
    }
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
    // Get the parent settings form.
    $form = parent::buildSettingsForm();

    $form[$this->plugin['name']]['include_unpublished'] = array(
      '#title' => t('Include unpublished nodes'),
      '#type' => 'checkbox',
      '#default_value' => isset($this->conf['include_unpublished']) ? $this->conf['include_unpublished'] : 0,
    );

    return $form;
  }
}