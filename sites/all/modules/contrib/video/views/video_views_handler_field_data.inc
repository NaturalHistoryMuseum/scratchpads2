<?php

/**
 * @file
 * video_handler_field_data.inc
 *
 * Provides a handler for displaying thumbnails within the serialized data column.
 */
class video_views_handler_field_data extends content_handler_field {
  
  function render($values) {
    $values = drupal_clone($values); // Prevent affecting the original.
    $data = unserialize($values->{$this->field_alias});
    $filepath = $data['video_thumb'];
    
    // We're down to a single node here, so we can retrieve the actual field
    // definition for the node type being considered.
    $field = content_fields($this->content_field['field_name'], $values->{$this->aliases['type']});
    $options = $this->options;
    $db_info = content_database_info($field);

    // Build a pseudo-node from the retrieved values.
    $node = drupal_clone($values);
    $node->type = $values->{$this->aliases['type']};
    $node->nid = $values->{$this->aliases['nid']};
    $node->vid = $values->{$this->aliases['vid']};
    // Some formatters need to behave differently depending on the build_mode
    // (for instance: preview), so we provide one.
    $node->build_mode = NODE_BUILD_NORMAL;

    $item = array();
    foreach ($db_info['columns'] as $column => $attributes) {
      $item[$column] = $values->{$this->aliases[$attributes['column']]};
    }

    $item['#delta'] = $field['multiple'] ?  $values->{$this->aliases['delta']} : 0;
    //added for thumbnails this should work.
    $file = pathinfo($filepath);
    $item['filepath'] = $filepath;
    $item['filename'] = $file['basename'];
    $item['filemime'] = file_get_mimetype($file['basename']);
    $item['filesize'] = is_readable($filepath) ? filesize($filepath) : 0;

    // Render items.
    $formatter_name = $options['format'];
    if ($formatter = _content_get_formatter($formatter_name, $field['type'])) {
      if (content_handle('formatter', 'multiple values', $formatter) == CONTENT_HANDLE_CORE) {
        // Single-value formatter.
        $output = content_format($field, $item, $formatter_name, $node);
      }
      else {
        // Multiple values formatter - we actually have only one value to display.
        $output = content_format($field, array($item), $formatter_name, $node);
      }
      return $this->render_link($output, $values);
    }
    return '';
  }
}