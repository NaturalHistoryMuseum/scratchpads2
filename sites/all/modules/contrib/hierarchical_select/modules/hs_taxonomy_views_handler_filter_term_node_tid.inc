<?php

/**
 * @file
 * This file defines a new filter handler for using Hierarchical Select in
 * exposed Taxonomy term filters.
 */

class hs_taxonomy_views_handler_filter_term_node_tid extends views_handler_filter_term_node_tid {

  function init(&$view, $options) {
    parent::init($view, $options);

    // The following code should logically be wrapped in a
    //   $this->select_type_is_hierarchical_select()
    // check. However, if we'd do this, you wouldn't be able to dynamically
    // switch between Hierarchical Select and another Selection Type.
    // This incurs a possibly unnecessary load (i.e. loading unneeded JS), but
    // is worth it because of the ease of use and because it's likely that
    // Hierarchical Select will be used, otherwise the user shouldn't install
    // this module in the first place.
    // If you can live with a reload of the View edit form, you can wrap the
    // code below in such a check.

    // Add JS and CSS required for Hierarchical Select to work.
    _hierarchical_select_setup_js();

    // Ensure that Drupal.HierarchicalSelect.prepareGETSubmit() gets called.
    require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'hierarchical_select') . '/includes/common.inc';
    hierarchical_select_common_add_views_js();
  }

  function value_form(&$form, &$form_state) {
    // Support limiting of vocabulary by the vocabulary's module name (this is
    // possible when a vocabulary is exported/defined as a feature, see
    // http://drupal.org/node/789556) or by the vocabulary's vid.
    if (isset($this->options['limit_by']) && $this->options['limit_by'] == 'module') {
      foreach (taxonomy_get_vocabularies() as $vid => $vocab) {
        if ($vocab->module == $this->options['module']) {
          $vocabulary = $vocab;
          break;
        }
      }
    }
    else {
      $vocabulary = taxonomy_vocabulary_load($this->options['vid']);
    }

    $view_name  = $this->view->name;
    $filter_id  = $this->options['id'];
    $display_id = _hs_taxonomy_views_get_display_id_for_filter($this->view, $filter_id);
    $optional   = $this->options['expose']['optional'];

    $config_id = "taxonomy-views-$view_name-$display_id-$filter_id";

    // When not exposed: settings form.
    if (empty($form_state['exposed'])) {
      // When the 'Selection type' is 'Hierarchical Select', user our own
      // value_form, otherwise use the parent's class form.
      if ($this->options['type'] == 'textfield' || $this->options['type'] == 'select') {
        parent::value_form($form, $form_state);
      }
      else {
        $config = hierarchical_select_common_config_get($config_id);
        $form['value'] = array(
          '#type'          => 'hierarchical_select',
          '#title'         => t('Select terms from vocabulary @voc', array('@voc' => $vocabulary->name)),
          '#default_value' => !empty($this->value) ? $this->value : array(),
          '#config' => array(
            'module' => 'hs_taxonomy',
            'params' => array(
              'vid'         => $vocabulary->vid,
              'exclude_tid' => NULL,
              'root_term'   => NULL,
            ),
            'save_lineage'    => $config['save_lineage'],
            'enforce_deepest' => $config['enforce_deepest'],
            'entity_count'    => $config['entity_count'],
            'resizable'       => 1,
            'level_labels' => array(
              'status' => $config['level_labels']['status'],
              'labels' => $config['level_labels']['labels'],
            ),
            'dropbox' => array(
              'status'   => $config['dropbox']['status'],
              'title'    => t('Terms to filter by'),
              'limit'    => $config['dropbox']['limit'],
              'reset_hs' => $config['dropbox']['reset_hs'],
            ),
            // Use our custom callback, because this is part of
            // views_ui_config_item_form(), which requires access to the
            // $view object, which will be restored automatically through
            // this special menu callback.
            'path' => "hs_taxonomy_views_json/$view_name/$display_id",
          ),
        );
      }
    }
    // When exposed: filter form.
    else {
      // When the 'Selection type' is 'Hierarchical Select', user our own
      // value_form, otherwise use the parent's class form.
      if (!$this->select_type_is_hierarchical_select()) {
        parent::value_form($form, $form_state);
      }
      else {
        $default_value = (isset($this->value) && !empty($this->value)) ? $this->value : array();

        // Basic settings for the form item.
        $form['value']['#type']          = 'hierarchical_select';
        $form['value']['#default_value'] = $default_value;
        $form['value']['#required']      = !((bool) $optional);

        // Apply the Hierarchical Select configuration to the form item.
        $defaults_override = array(
         'module' => 'hs_taxonomy_views',
         'params' => array(
           'optional'    => (bool) $optional,
           'filter_id'   => $filter_id,
           'vid'         => $vocabulary->vid,
           'exclude_tid' => NULL,
           'root_term'   => NULL,
         ),
         // When the 'Any' option is selected, nothing else should be and it
         // should replace the '<none>' option.
         'special_items' => array(
           HS_TAXONOMY_VIEWS_ANY_OPTION => array('none', 'exclusive'),
         ),
         // This is a GET form, so also render the flat select.
         'render_flat_select' => 1,
         // Use our custom callback.
         'path' => "hs_taxonomy_views_json/$view_name/$display_id",
        );
        hierarchical_select_common_config_apply($form['value'], $config_id, $defaults_override);
      }
    }
  }

  function expose_form_right(&$form, &$form_state) {
    // The form with the "Optional", "Force single" and "Remember" checkboxes.
    parent::expose_form_right($form, $form_state);

    // When the selection type is Hierarchical Select, remove the "Force
    // single" checkbox.
    if ($this->select_type_is_hierarchical_select()) {
      unset($form['expose']['single']);
    }
  }

  function extra_options_form(&$form, &$form_state) {
    parent::extra_options_form($form, $form_state);

    $form['type'] = array(
      '#type'    => 'radios',
      '#title'   => t('Selection type'),
      '#options' => array(
        'select'              => t('Dropdown'),
        'textfield'           => t('Autocomplete'),
        'hierarchical_select' => t('Hierarchical Select'),
      ),
      '#default_value' => $this->options['type'],
    );

    $filter_id  = $form_state['id'];
    $display_id = _hs_taxonomy_views_get_display_id_for_filter($this->view, $filter_id);
    $form['configure_hs'] = array(
      '#prefix'     => '<div class="form-item"><div id="configure-hierarchical-select-link" style="display: inline-block">',
      '#markup'      => l(t('Configure Hierarchical Select'), _hs_taxonomy_views_config_path($this->view->name, $display_id, $filter_id)),
      '#suffix'     => '</div></div>',
      // Doesn't work because #process is not called for #type = markup form items.
      '#process'    => array('views_process_dependency'),
      '#dependency' => array('radio:options[type]' => array('hierarchical_select')),
      // Set #input = TRUE so that #process will be called.
      '#input'      => TRUE,
      // Set #id manually because that is not generated for #type = markup.
      // This is the same id attribute as in #prefix.
      '#id'         => 'configure-hierarchical-select-link',
    );

    // Ensure the configure_hs form item is before the markup_end form item.
    unset($form['markup_end']);
    $form['markup_end'] = array(
      '#markup' => '</div>',
    );
  }

  /**
   * Validate the exposed filter form
   */
  function exposed_validate(&$form, &$form_state) {
    // No validation necessary when the filter is not exposed or when no
    // identifier is set.
    if (empty($this->options['exposed']) || empty($this->options['expose']['identifier'])) {
      return;
    }

    if (!$this->select_type_is_hierarchical_select()) {
      parent::exposed_validate($form, $form_state);
    }
    else {
      $identifier = $this->options['expose']['identifier'];
      $input = $form_state['values'][$identifier];

      // If the HS_TAXONOMY_VIEWS_ANY_OPTION option is selected, then the
      // results should not be filtered at all.
      if (!is_array($input)) { // When filtering by multiple terms.
        if ($input != HS_TAXONOMY_VIEWS_ANY_OPTION) {
          $this->validated_exposed_input = $input;
        }
      }
      else { // When filtering by a single term.
        if (!in_array(HS_TAXONOMY_VIEWS_ANY_OPTION, $input)) {
          $this->validated_exposed_input = $input;
        }
      }
    }
  }

  /**
   * Take input from exposed filters and assign to this handler, if necessary.
   */
  function accept_exposed_input($input) {
    // No need to override the parrent class' accept_exposed_input() method.
    // It returns TRUE (and therefor results in an altered query and therefor
    // filtered view) whenever the filter is not exposed or when a new value
    // was validated.
    // This method checks if $this->validated_exposed_input is set, and if so,
    // results are filtered by those values.
    $rc = parent::accept_exposed_input($input);
    if ($this->select_type_is_hierarchical_select() && !is_array($this->value)) {
      $this->value = array($this->value);
    }
    return $rc;
  }

  function admin_summary() {
    $this->value = (array) $this->value;
    return parent::admin_summary();
  }

  /**
   * Check whether the "Selection type" (in the configuration of the exposed
   * filter) is Hierarchical Select.
   *
   * This function is used in almost every overridden method to determine
   * whether our custom logic should be used or the parent class's, i.e. the
   * parent method in the views_handler_filter_term_node_tid class.
   */
  function select_type_is_hierarchical_select() {
    return $this->options['type'] == 'hierarchical_select';
  }
}
