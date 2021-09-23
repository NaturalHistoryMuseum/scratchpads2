<?php

/**
 * Field handler to present a link to editt the entity content.
 *
 */
class entity_admin_views_handler_field_link_edit extends entity_admin_views_handler_field_link{

  function render_link($entity, $values){
    $entity_uri = entity_uri($entity->entityType(), $entity);
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = $entity_uri['path'] . '/edit';
    $text = !empty($this->options['text']) ? $this->options['text'] : t('Edit');
    return $text;
  }
}
