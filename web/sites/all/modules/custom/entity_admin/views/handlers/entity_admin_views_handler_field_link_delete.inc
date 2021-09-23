<?php
/**
 * Field handler to present a link to delete the entity content.
 *
 */
class entity_admin_views_handler_field_link_delete extends entity_admin_views_handler_field_link {
  
  function render_link($entity, $values) {
		$entity_uri = entity_uri($entity->entityType(), $entity);
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = $entity_uri['path'] . '/delete';
    $text = !empty($this->options['text']) ? $this->options['text'] : t('Delete');
    
    return $text;
  }
}
