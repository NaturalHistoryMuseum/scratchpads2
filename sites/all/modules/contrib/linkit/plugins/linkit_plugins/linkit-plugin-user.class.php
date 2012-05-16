<?php
/**
 * @file
 * Define Linkit user plugin class.
 */
class LinkitPluginUser extends LinkitPluginEntity {
  function __construct($plugin, $profile) {
    /**
     * The user entity doesn't add any label in their entity keys as they define a
     * "label callback" instead. Therefor we have to tell which field the user
     * entity have as label.
     */
    $this->entity_field_label = 'name';
    parent::__construct($plugin, $profile);
  }
}