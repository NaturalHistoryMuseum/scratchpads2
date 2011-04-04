<?php
function taxonomy_overview_terms($form, &$form_state, $vocabulary) {
  global $pager_page_array, $pager_total, $pager_total_items;

  // Check for confirmation forms.
  if (isset($form_state['confirm_reset_alphabetical'])) {
    return taxonomy_vocabulary_confirm_reset_alphabetical($form, $form_state, $vocabulary->vid);
  }

  $form['#vocabulary'] = $vocabulary;
  $form['#tree'] = TRUE;
  $form['#parent_fields'] = FALSE;

  $page            = isset($_GET['page']) ? $_GET['page'] : 0;
  $page_increment  = variable_get('taxonomy_terms_per_page_admin', 100);  // Number of terms per page.
  $page_entries    = 0;   // Elements shown on this page.
  $before_entries  = 0;   // Elements at the root level before this page.
  $after_entries   = 0;   // Elements at the root level after this page.
  $root_entries    = 0;   // Elements at the root level on this page.

  // Terms from previous and next pages are shown if the term tree would have
  // been cut in the middle. Keep track of how many extra terms we show on each
  // page of terms.
  $back_step    = NULL;
  $forward_step = 0;

  // An array of the terms to be displayed on this page.
  $current_page = array();

  $delta = 0;
  $term_deltas = array();
  $tree = taxonomy_get_tree($vocabulary->vid);
  $term = current($tree);
  do {
    // In case this tree is completely empty.
    if (empty($term)) {
      break;
    }
    $delta++;
    // Count entries before the current page.
    if ($page && ($page * $page_increment) > $before_entries && !isset($back_step)) {
      $before_entries++;
      continue;
    }
    // Count entries after the current page.
    elseif ($page_entries > $page_increment && isset($complete_tree)) {
      $after_entries++;
      continue;
    }

    // Do not let a term start the page that is not at the root.
    if (isset($term->depth) && ($term->depth > 0) && !isset($back_step)) {
      $back_step = 0;
      while ($pterm = prev($tree)) {
        $before_entries--;
        $back_step++;
        if ($pterm->depth == 0) {
          prev($tree);
          continue 2; // Jump back to the start of the root level parent.
       }
      }
    }
    $back_step = isset($back_step) ? $back_step : 0;

    // Continue rendering the tree until we reach the a new root item.
    if ($page_entries >= $page_increment + $back_step + 1 && $term->depth == 0 && $root_entries > 1) {
      $complete_tree = TRUE;
      // This new item at the root level is the first item on the next page.
      $after_entries++;
      continue;
    }
    if ($page_entries >= $page_increment + $back_step) {
      $forward_step++;
    }

    // Finally, if we've gotten down this far, we're rendering a term on this page.
    $page_entries++;
    $term_deltas[$term->tid] = isset($term_deltas[$term->tid]) ? $term_deltas[$term->tid] + 1 : 0;
    $key = 'tid:' . $term->tid . ':' . $term_deltas[$term->tid];

    // Keep track of the first term displayed on this page.
    if ($page_entries == 1) {
      $form['#first_tid'] = $term->tid;
    }
    // Keep a variable to make sure at least 2 root elements are displayed.
    if ($term->parents[0] == 0) {
      $root_entries++;
    }
    $current_page[$key] = $term;
  } while ($term = next($tree));

  // Because we didn't use a pager query, set the necessary pager variables.
  $total_entries = $before_entries + $page_entries + $after_entries;
  $pager_total_items[0] = $total_entries;
  $pager_page_array[0] = $page;
  $pager_total[0] = ceil($total_entries / $page_increment);

  // If this form was already submitted once, it's probably hit a validation
  // error. Ensure the form is rebuilt in the same order as the user submitted.
  if (!empty($form_state['input'])) {
    $order = array_flip(array_keys($form_state['input'])); // Get the $_POST order.
    $current_page = array_merge($order, $current_page); // Update our form with the new order.
    foreach ($current_page as $key => $term) {
      // Verify this is a term for the current page and set at the current depth.
      if (is_array($form_state['input'][$key]) && is_numeric($form_state['input'][$key]['tid'])) {
        $current_page[$key]->depth = $form_state['input'][$key]['depth'];
      }
      else {
        unset($current_page[$key]);
      }
    }
  }

  // Build the actual form.
  foreach ($current_page as $key => $term) {
    // Save the term for the current page so we don't have to load it a second time.
    $form[$key]['#term'] = (array) $term;
    if (isset($term->parents)) {
      $form[$key]['#term']['parent'] = $term->parent = $term->parents[0];
      unset($form[$key]['#term']['parents'], $term->parents);
    }

    $form[$key]['view'] = array('#type' => 'link', '#title' => $term->name, '#href' => "taxonomy/term/$term->tid");
    if ($vocabulary->hierarchy < 2 && count($tree) > 1) {
      $form['#parent_fields'] = TRUE;
      $form[$key]['tid'] = array(
        '#type' => 'hidden',
        '#value' => $term->tid
      );
      $form[$key]['parent'] = array(
        '#type' => 'hidden',
        // Yes, default_value on a hidden. It needs to be changeable by the javascript.
        '#default_value' => $term->parent,
      );
      $form[$key]['depth'] = array(
        '#type' => 'hidden',
        // Same as above, the depth is modified by javascript, so it's a default_value.
        '#default_value' => $term->depth,
      );
      $form[$key]['weight'] = array(
        '#type' => 'weight',
        '#delta' => $delta,
        '#title_display' => 'invisible',
        '#title' => t('Weight for added term'),
        '#default_value' => $term->weight,
      );
    }
    $form[$key]['edit'] = array('#type' => 'link', '#title' => t('edit'), '#href' => 'taxonomy/term/' . $term->tid . '/edit', '#options' => array('query' => drupal_get_destination()));
  }

  $form['#total_entries'] = $total_entries;
  $form['#page_increment'] = $page_increment;
  $form['#page_entries'] = $page_entries;
  $form['#back_step'] = $back_step;
  $form['#forward_step'] = $forward_step;
  $form['#empty_text'] = t('No terms available. <a href="@link">Add term</a>.', array('@link' => url('admin/structure/taxonomy/' . $vocabulary->machine_name . '/add')));

  if ($vocabulary->hierarchy < 2 && count($tree) > 1) {
    $form['actions'] = array('#type' => 'actions', '#tree' => FALSE);
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save')
    );
    $form['actions']['reset_alphabetical'] = array(
      '#type' => 'submit',
      '#value' => t('Reset to alphabetical')
    );
    $form_state['redirect'] = array($_GET['q'], (isset($_GET['page']) ? array('query' => array('page' => $_GET['page'])) : array()));
  }

  return $form;
}

function theme_taxonomy_overview_terms($variables) {
  $form = $variables['form'];

  $page_increment  = $form['#page_increment'];
  $page_entries    = $form['#page_entries'];
  $back_step     = $form['#back_step'];
  $forward_step  = $form['#forward_step'];

  // Add drag and drop if parent fields are present in the form.
  if ($form['#parent_fields']) {
    drupal_add_tabledrag('taxonomy', 'match', 'parent', 'term-parent', 'term-parent', 'term-id', FALSE);
    drupal_add_tabledrag('taxonomy', 'depth', 'group', 'term-depth', NULL, NULL, FALSE);
    drupal_add_js(drupal_get_path('module', 'taxonomy') . '/taxonomy.js');
    drupal_add_js(array('taxonomy' => array('backStep' => $back_step, 'forwardStep' => $forward_step)), 'setting');
    drupal_add_css(drupal_get_path('module', 'taxonomy') . '/taxonomy.css');
  }
  drupal_add_tabledrag('taxonomy', 'order', 'sibling', 'term-weight');

  $errors = form_get_errors() != FALSE ? form_get_errors() : array();
  $rows = array();
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['#term'])) {
      $term = &$form[$key];

      $row = array();
      $row[] = (isset($term['#term']['depth']) && $term['#term']['depth'] > 0 ? theme('indentation', array('size' => $term['#term']['depth'])) : ''). drupal_render($term['view']);
      if ($form['#parent_fields']) {
        $term['tid']['#attributes']['class'] = array('term-id');
        $term['parent']['#attributes']['class'] = array('term-parent');
        $term['depth']['#attributes']['class'] = array('term-depth');
        $row[0] .= drupal_render($term['parent']) . drupal_render($term['tid']) . drupal_render($term['depth']);
      }
      $term['weight']['#attributes']['class'] = array('term-weight');
      $row[] = drupal_render($term['weight']);
      $row[] = drupal_render($term['edit']);
      $row = array('data' => $row);
      $rows[$key] = $row;
    }
  }

  // Add necessary classes to rows.
  $row_position = 0;
  foreach ($rows as $key => $row) {
    $rows[$key]['class'] = array();
    if (isset($form['#parent_fields'])) {
      $rows[$key]['class'][] = 'draggable';
    }

    // Add classes that mark which terms belong to previous and next pages.
    if ($row_position < $back_step || $row_position >= $page_entries - $forward_step) {
      $rows[$key]['class'][] = 'taxonomy-term-preview';
    }

    if ($row_position !== 0 && $row_position !== count($rows) - 1) {
      if ($row_position == $back_step - 1 || $row_position == $page_entries - $forward_step - 1) {
        $rows[$key]['class'][] = 'taxonomy-term-divider-top';
      }
      elseif ($row_position == $back_step || $row_position == $page_entries - $forward_step) {
        $rows[$key]['class'][] = 'taxonomy-term-divider-bottom';
      }
    }

    // Add an error class if this row contains a form error.
    foreach ($errors as $error_key => $error) {
      if (strpos($error_key, $key) === 0) {
        $rows[$key]['class'][] = 'error';
      }
    }
    $row_position++;
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => $form['#empty_text'], 'colspan' => '3'));
  }

  $header = array(t('Name'), t('Weight'), t('Operations'));
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'taxonomy')));
  $output .= drupal_render_children($form);
  $output .= theme('pager');

  return $output;
}