<?php
// $Id$

/**
 * Sort handler for entity labels.
 *
 * EntityFieldQuery doesn't support querying labels the same way as other
 * entity metadata, so this handler needs to handle the label as if it were
 * a property.
 */
class efq_views_handler_sort_entity_label extends views_handler_sort {
  function query() {
    $this->query->query->propertyOrderBy($this->definition['label column'], $this->options['order']);
  }
}
