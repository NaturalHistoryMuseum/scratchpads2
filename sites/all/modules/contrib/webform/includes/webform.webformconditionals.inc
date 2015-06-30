<?php

/**
 * @file
 * Conditional engine to process dependencies within the webform's conditionals.
 */

/**
 * Performs analysis and topological sorting on the conditionals.
 */
class WebformConditionals {
  protected static $conditionals = array();

  protected $node;
  protected $topologicalOrder;
  protected $pageMap;
  protected $childrenMap;

  public $errors;

  /**
   * Creates and caches a WebformConditional for a given node.
   */
  static function factory($node) {
    if (!isset(self::$conditionals[$node->nid])) {
      self::$conditionals[$node->nid] = new WebformConditionals($node);
    }
    return self::$conditionals[$node->nid];
  }

  /**
   * Constructs a WebformConditional.
   */
  function __construct($node) {
    $this->node = $node;
  }

  /**
   * Sorts the conditionals into topological order.
   *
   * The "nodes" of the list are the conditionals, not the components that
   * they operate upon.
   *
   * The webform components must already be sorted into component tree order
   * before calling this method.
   *
   * See http://en.wikipedia.org/wiki/Topological_sorting
   */
  protected function topologicalSort() {
    $components = $this->node->webform['components'];
    $conditionals = $this->node->webform['conditionals'];
    $errors = array();

    // Generate a component to condional map for conditional targets.
    $cid_to_target_rgid = array();
    foreach ($conditionals as $rgid => $conditional) {
      $target_id = $conditional['target'];
      $cid_to_target_rgid[$target_id][$rgid] = $rgid;
      if (count($cid_to_target_rgid[$target_id]) == 2) {
        $component = $components[$target_id];
        $errors[$component['page_num']][] = t('More than one conditional hides or shows component "@name".',
                                              array('@name' => $component['name']));
      }
    }

    // Generate T-Orders for each page
    $new_entry = array('in' => array(), 'out' => array(), 'rgid' => -1);
    $page_num = 0;
    // If the first component is a page break, then no component is on page 1. Create empty arrays for page 1.
    $sorted = array(1 => array());
    $page_map = array(1 => array());
    $component = reset($components);
    while ($component) {
      $cid = $component['cid'];

      // Start a new page, if needed
      if ($component['page_num'] > $page_num ) {
        $page_num = $component['page_num'];
        // Create an empty list that will contain the sorted elements.
        // This list is known as L in the literature.
        $sorted[$page_num] = array();

        // Create an empty list of dependency nodes for this page.
        $nodes = array();
      }

      // Create the pageMap as a side benefit of generating the t-sort.
      $page_map[$page_num][$cid] = $cid;

      // Process component by adding it's conditional data to a component-tree-traversal order an index of:
      // - incoming dependencies = the source components for the conditions for this target component and
      // - outgoing dependencies = components which depend upon the target component
      // Note: Surprisingly, 0 is a valid rgid, as well as a valid rid. Use -1 as a semaphore.
      if (isset($cid_to_target_rgid[$cid])) {
        // The component is the target of conditional(s)
        foreach ($cid_to_target_rgid[$cid] as $rgid) {
          $conditional = $conditionals[$rgid];
          if (!isset($nodes[$cid])) {
            $nodes[$cid] = $new_entry;
          }
          $nodes[$cid]['rgid'] = $rgid;
          foreach ($conditional['rules'] as $rule) {
            if ($rule['source_type'] == 'component') {
              $source_id = $rule['source'];
              if (!isset($nodes[$source_id])) {
                $nodes[$source_id] = $new_entry;
              }
              $nodes[$cid]['in'][$source_id] = $source_id;
              $nodes[$source_id]['out'][$cid] = $cid;
              $source_component = $components[$source_id];
              $source_pid = $source_component['pid'];
              if ($source_pid) {
                if (!isset($nodes[$source_pid])) {
                  $nodes[$source_pid] = $new_entry;
                }
                // The rule source is within a parent fieldset. Create a dependency on the parent.
                $nodes[$source_pid]['out'][$source_id] = $source_id;
                $nodes[$source_id]['in'][$source_pid] = $source_pid;
              }
              if ($source_component['page_num'] > $page_num) {
                $errors[$page_num][] = t('A forward reference from page @from, %from to %to was found.',
                                         array(
                                            '%from' => $source_component['name'],
                                            '@from' => $source_component['page_num'],
                                            '%to' => $component['name'],
                                         ));
              }
            }
          }
        }
      }

      // Fetch the next component, if any.
      $component = next($components);

      // Finish any previous page already processed.
      if (!$component || $component['page_num'] > $page_num) {

        // Create a set of all components which have are not dependent upon anything.
        // This list is known as S in the literature.
        $start_nodes = array();
        foreach ($nodes as $id => $n) {
          if (!$n['in']) {
            $start_nodes[] = $id;
          }
        }

        // Process the start nodes, removing each one in turn from the queue.
        while ($start_nodes) {
          $id = array_shift($start_nodes);
          // If the node represents an actual conditional, it can now be added
          // to the end of the sorted order because anything it depends upon has
          // already been calcuated.
          if ($nodes[$id]['rgid'] >= 0) {
            $sorted[$page_num][] = array(
              'cid' => $id,
              'rgid' => $nodes[$id]['rgid'],
              'name' => $components[$id]['name'],
            );
          }

          // Any other nodes that depend upon this node may now have their dependency
          // on this node removed, since it has now been calculated.
          foreach ($nodes[$id]['out'] as $out_id) {
            unset($nodes[$out_id]['in'][$id]);
            if (!$nodes[$out_id]['in']) {
              $start_nodes[] = $out_id;
            }
          }

          // All out-going dependencies have been handled.
          $nodes[$id]['out'] = array();
        }

        // Check for a cyclic graph (circular dependency)
        foreach ($nodes as $id => $n) {
          if ($n['in'] || $n['out']) {
            $errors[$page_num][] = t('A circular reference involving %name was found.',
                                     array('%name' => $components[$id]['name']));
          }
        }

      } // End finshing previous page

    } // End component loop

    // Create an empty page map for the preview page.
    $page_map[$page_num + 1] = array();

    $this->topologicalOrder = $sorted;
    $this->errors = $errors;
    $this->pageMap = $page_map;
  }

  /**
   * Returns the (possibly cached) topological sort order.
   */
  function getOrder() {
    if (!$this->topologicalOrder) {
      $this->topologicalSort();
    }
    return $this->topologicalOrder;
  }

  /**
   * Returns an index of components by page number.
   */
  function getPageMap() {
    if (!$this->pageMap) {
      $this->topologicalSort();
    }
    return $this->pageMap;
  }
  
  /**
   * Displays and error messages from the previously-generated sort order.
   *
   * User's who can't fix the webform are shown a single, simplified message.
   */
  function reportErrors() {
    $this->getOrder();
    if ($this->errors) {
      if (webform_node_update_access($this->node)) {
        foreach ($this->errors as $page_num => $page_errors) {
          drupal_set_message(format_plural(count($page_errors),
                                           'Conditional error on page @num:',
                                           'Conditional errors on page @num:',
                                           array('@num' => $page_num)) .
                             '<br /><ul><li>' . implode('</li><li>', $page_errors) . '</li></ul>', 'warning');
        }
      }
      else {
         drupal_set_message(t('This form is improperly configured. Contact the administrator.'), 'warning');
      }
    }
  }

  /**
   * Creates and caches a map of the children of a each component.
   *
   * Called after the component tree has been made and then flattened again.
   * Alas, the children data is removed when the tree is flattened. The
   * components are indexecd by cid but in tree order. Because cid's are
   * numeric, they may not appear in IDE's or debuggers in their actual order.
   */
  function getChildrenMap() {
    if (!$this->childrenMap) {
      $map = array();
      foreach ($this->node->webform['components'] as $cid => $component) {
        $pid = $component['pid'];
        if ($pid) {
          $map[$pid][] = $cid;
        }
      }

      $this->childrenMap = $map;
    }
    return $this->childrenMap;
  }

  /**
   * Deletes the value of the given component, plus any descendents.
   */
  protected function deleteFamily(&$input_values, $parent_id) {
    if (isset($input_values[$parent_id])) {
      $input_values[$parent_id] = NULL;
    }
    if (isset($this->childrenMap[$parent_id])) {
      foreach ($this->childrenMap[$parent_id] as $child_id) {
        $this->deleteFamily($input_values, $child_id);
      }
    }
  }

  /**
   * Executes the conditionals on a submission, removing any data which should
   * be hidden.
   */
  function executeConditionals($input_values, $page_num = 0) {
    $this->getOrder();
    $this->getChildrenMap();

    module_load_include('inc', 'webform', 'includes/webform.conditionals');
    
    $components = $this->node->webform['components'];
    $conditionals = $this->node->webform['conditionals'];
    $operators = webform_conditional_operators();


    $first_page = $page_num ? $page_num : 1;
    $last_page = $page_num ? $page_num : count($this->topologicalOrder);
    for ($page = $first_page; $page <= $last_page; $page++) {
      foreach ($this->topologicalOrder[$page] as $conditional_spec) {
        
        $conditional = $conditionals[$conditional_spec['rgid']];
        $conditional_result = TRUE;

        // Execute each comparison callback.
        $conditional_results = array();
        foreach ($conditional['rules'] as $rule) {
          // TODO: Support other source types besides components?
          if ($rule['source_type'] !== 'component') {
            continue;
          }
          $source_component = $components[$rule['source']];
          $source_cid = $source_component['cid'];

          $source_values = array();
          if (isset($input_values[$source_cid])) {
            $component_value = $input_values[$source_cid];
            // For select_or_other components, use only the select values because $source_values must not be a nested array.
            // During preview, the array is already flattened.
            if ($source_component['type'] === 'select' &&
                !empty($source_component['extra']['other_option']) &&
                isset($component_value['select'])) {
              $component_value = $component_value['select'];
            }
            $source_values = is_array($component_value) ? $component_value : array($component_value);
          }

          // Determine the operator and callback.
          $conditional_type = webform_component_property($source_component['type'], 'conditional_type');
          $operator_info = $operators[$conditional_type];

          // Perform the comparison callback and build the results for this group.
          $comparison_callback = $operator_info[$rule['operator']]['comparison callback'];
          $conditional_results[] = $comparison_callback($source_values, $rule['value']);
        }

        // Calculate the and/or result.
        $filtered_results = array_filter($conditional_results);
        $conditional_result = $conditional['andor'] === 'or'
                                  ? count($filtered_results) > 0
                                  : count($filtered_results) === count($conditional_results);

        // Flip the result of the action is to hide.
        if ($conditional['action'] == 'hide') {
          $conditional_result = !$conditional_result;
        }

        if (!$conditional_result) {
          $this->deleteFamily($input_values, $conditional['target']);
        }

      } // End conditinal loop
    } // End page loop

    return $input_values;
  }

 }
