<?php

/**
 * @file
 * Provide views field handler for workflow.module.
 */

/**
 * Field handler to provide simple renderer that allows linking to a CC license deed.
 */
class creativecommons_views_handler_field_attribution extends views_handler_field_url {

  /**
   * Implementation of query().
   *
   * Adapted from views_handler_field_url.
   */
  function query() {
    $this->ensure_my_table();

    // Add the field.
    $this->url_alias = $this->query->add_field($this->table_alias, 'attributionURL');
    $this->name_alias = $this->query->add_field($this->table_alias, 'attributionName');
  }

  /**
   * Implementation of render().
   *
   * Could use render_altered via $this->options['alt']['text'], but overriding
   * instead to add RDFa info...
   */
  function render($values) {
    $attribution_name = $values->{$this->name_alias};
    $attribution_url = $values->{$this->url_alias};
    $attribution = '';

    // Adapated from views_handler_field_url
    if (!empty($this->options['display_as_link']) && $attribution_url && creativecommons_metadata_is_available('attributionURL')) {
      $attributes['rel'] = 'cc:attributionURL';

      if ($attribution_name && creativecommons_metadata_is_available('attributionName')) {
        $attributes['property'] = 'cc:attributionName';
        $text = $attribution_name;
      }
      else {
        $text = $attribution_url;
      }
      $attribution = l($text, $attribution_url, array('attributes' => $attributes));
    }
    elseif (creativecommons_metadata_is_available('attributionName')) {
      $attribution = '<span property="cc:attributionName">' . $attribution_name . '</span>';
    }


    // Return output, if any
    if ($attribution) {
      $url = url('node/' . $values->nid, array('absolute' => TRUE));
      return '<span xmlns:cc="http://creativecommons.org/ns#" xmlns:dc="http://purl.org/dc/elements/1.1/" about="' . $url . '">' . $attribution . '</span>';
    }
  }
}

