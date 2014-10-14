<?php

/**
 * @file
 * Field handler to provide additional control for the All Taxonomy Terms field.
 */

class views_rss_handler_field_term_node_tid extends views_handler_field_term_node_tid {

  function option_definition() {
    $options = parent::option_definition();
    $options['rss_domain'] = array('default' => 'none');
    $options['rss_include_parents'] = array('default' => 0);
    $options['rss_include_vocabulary'] = array('default' => 0);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // New display type: "RSS <category> element".
    $form['type']['#weight'] = 2;
    $form['separator']['#weight'] = 3;
    $form['type']['#options']['rss_category'] = t('RSS &lt;category&gt; element');

    // Additional options for "domain" attribute.
    $form['rss_domain'] = array(
      '#type' => 'select',
      '#title' => t('<em>Domain</em> attribute'),
      '#description' => t("<em>domain</em> attribute identifies the category's taxonomy.") . ' ' . l('[?]', 'http://www.rssboard.org/rss-profile#element-channel-item-category', array('attributes' => array('title' => t('Need more information?')))),
      '#options' => array(
        'none' => t('None'),
        'path' => t('Path'),
        'alias' => t('URL alias'),
      ),
      '#default_value' => $this->options['rss_domain'],
      '#dependency' => array('radio:options[type]' => array('rss_category')),
      '#weight' => 3,
    );

    // Output terms with their parents.
    // Example: parent2/parent1/term
    $form['rss_include_parents'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include term parents'),
      '#description' => t('Output terms from hierarchical vocabularies together with their parents (slash-delimited).'),
      '#default_value' => $this->options['rss_include_parents'],
      '#dependency' => array('radio:options[type]' => array('rss_category')),
      '#weight' => 4,
    );

    // Output terms with their parents and vocabulary.
    // Example: vocabulary/parent2/parent1/term
    $form['rss_include_vocabulary'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include term vocabulary'),
      '#description' => t('Add vocabulary name before term parents.'),
      '#default_value' => $this->options['rss_include_vocabulary'],
      '#process' => array('ctools_dependent_process'),
      '#dependency_count' => 2,
      '#dependency' => array(
        'radio:options[type]' => array('rss_category'),
        'edit-options-rss-include-parents' => array(1),
      ),
      '#weight' => 5,
    );

    // Hide "Link this field to its term page" checkbox
    // if "RSS <category> element" is selected as "Display type".
    $form['link_to_taxonomy']['#weight'] = 4;
    $form['link_to_taxonomy']['#states'] = array(
      'visible' => array(
        ':input[name="options[type]"]' => array('!value' => 'rss_category'),
      ),
    );
  }

  function pre_render(&$values) {
    parent::pre_render($values);
    // For "RSS <category> element" display type make sure
    // that field value will never be rendered as link.
    if ($this->options['type'] == 'rss_category') {
      foreach ($this->items as $node_nid => $terms) {
        foreach ($terms as $term_tid => $term) {
          if (isset($term['make_link'])) {
            $this->items[$node_nid][$term_tid]['make_link'] = FALSE;
          }
          if ($this->options['rss_domain'] != 'none' && empty($this->options['link_to_taxonomy'])) {
            $this->items[$node_nid][$term_tid]['path'] = 'taxonomy/term/' . $term_tid;
          }
          if ($this->options['rss_include_parents']) {
            $this->items[$node_nid][$term_tid]['parents'] = taxonomy_get_parents_all($term_tid);
          }
          if ($this->options['rss_include_vocabulary']) {
            $this->items[$node_nid][$term_tid]['vocabulary'] = $term['vocabulary'];
          }
        }
      }
    }
  }

  function render_items($items) {
    if (!empty($items)) {
      if ($this->options['type'] == 'rss_category') {
        return implode('', $items);
      }
      // Default render_items().
      return parent::render_items($items);
    }
  }

  function render_item($count, $item) {
    if ($this->options['type'] == 'rss_category') {
      // Basic XML element details.
      $rss_element = array(
        'key' => 'category',
        'value' => $item['name'],
      );
      // Slash-delimited list of all parents with the term at the end.
      // $item['parents'] array contains the current term as well.
      if ($this->options['rss_include_parents'] && isset($item['parents'])) {
        $parents = array();
        // Add vocabulary if required.
        if ($this->options['rss_include_vocabulary'] && isset($item['vocabulary'])) {
          $parents[] = $item['vocabulary'];
        }
        // Add all terms (parent and current one).
        foreach (array_reverse($item['parents']) as $parent) {
          $parents[] = $parent->name;
        }
        if (count($parents)) {
          $rss_element['value'] = implode('/', $parents);
        }
      }
      // Add "domain" attribute if required.
      if ($this->options['rss_domain'] == 'path') {
        $rss_element['attributes']['domain'] = url('<front>', array('absolute' => TRUE)) . $item['path'];
      }
      elseif ($this->options['rss_domain'] == 'alias') {
        $rss_element['attributes']['domain'] = url($item['path'], array('absolute' => TRUE));
      }
      // Prepare raw-rendered values to store in view results.
      $rendered = array(
        '#item' => $item,
        '#markup' => format_xml_elements(array($rss_element)),
        '#rss_element' => $rss_element,
        '#settings' => $this->options,
      );
      // Store raw-rendered values in view results, the same way
      // as values from standard field formatters are added.
      $row_index = $this->view->row_index;
      $field_name = 'field_' . $this->field;
      $this->view->result[$row_index]->{$field_name}[$count] = array(
        'rendered' => $rendered,
        'raw' => $item,
      );
      // Return formatted XML element.
      return format_xml_elements(array($rss_element));
    }
    // Standard Views render_item().
    return parent::render_item($count, $item);
  }

}
