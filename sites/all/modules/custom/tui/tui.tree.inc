<?php

/**
 * Theme the tree
 */
function theme_tui_tree($taxonomy_tree, $active = FALSE){
  // We need to employ a recursive element here, and it may as well be this 
  // function that does it.  We'll recurse into the tree, magicing things to 
  // where they should be.
  $output = '<ul>';
  foreach($taxonomy_tree as $tid => $branch){
    $term = taxonomy_get_term($tid);
    $terms = array(
      $term->tid => $term
    );
    module_invoke_all_by_reference('taxonomy_term_load', $terms);
    $term = array_pop($terms);
    $term_has_children = FALSE;
    if(!is_array($branch) || !count($branch)){
      // Does this term have children
      $term_has_children = db_result(db_query(db_rewrite_sql('SELECT COUNT(*) FROM {term_hierarchy} t WHERE parent = %d', 't', 'tid'), $term->tid));
    }
    $output .= '<li id="tidandvid-' . $term->tid . '-' . $term->vid . '"><p><span class="tui-nodeleaf ';
    if(is_array($branch) && count($branch)){
      $output .= 'tui-node-open';
    }else if($term_has_children){
      $output .= 'tui-node-closed';
    }else{
      $output .= 'tui-leaf';
    }
    $output .= '"><img src="' . base_path() . drupal_get_path('module', 'tui') . '/images/empty.gif" width="9px" height="9px"></span><span class="tui-term';
    if($active == $term->tid){
      $output .= ' active';
    }
    if(isset($term->usage) && ($term->usage == 'invalid' || $term->usage == 'not accepted')){
      $output .= ' usage-invalid';
    }
    $output .= '" id="tid-' . $term->tid . '">' . check_plain($term->name) . '</span></p>';
    if(is_array($branch)){
      $output .= theme_tui_tree($branch, $active);
    }
    $output .= '</li>';
  }
  return $output . '</ul>';
}

/**
 * Invoke all pass by reference.  This doesn't belong here, but here it is for
 * now.
 */
if(!function_exists('module_invoke_all_by_reference')){
  function module_invoke_all_by_reference($hook, &$args){
    $modules = module_list();
    foreach($modules as $module => $values){
      if(function_exists($module . '_' . $hook)){
        $function = $module . '_' . $hook;
        $function($args);
      }
    }
  }
}

/**
 * Return the tid of the next or previous term to the one highlighted
 */
function tui_nextorprevious_tid($nextorprevious, $vid, $tid){
  if(strpos($tid, '-')){
    // We have a tid, we need to get the results relative to this one - easiest
    // way is to move back up the tree.
    // Next will move down into a tree, whilst previous will move
    $tid = array_pop(explode('-', $tid));
    if($nextorprevious == 'tui-next'){
      // NEXT
      $children = taxonomy_get_children($tid, $vid);
      if(count($children)){
        $next_child = array_shift($children);
        echo $next_child->tid;
      }else{
        // We have no children, we need the following siblings.
        $ancestors = taxonomy_get_parents_all($tid);
        foreach($ancestors as $ancestor){
          $next_tid = _tui_get_next_sibling($vid, $ancestor->tid);
          if($next_tid){
            echo $next_tid;
            exit();
          }
        }
        echo $tid;
      }
    }else{
      // PREVIOUS
      // Get the parent of the current term, then get all children, and return
      // the previous term.  If we're already at the root, and previous is
      // pressed, then return the last child of the previous term      
      $parent = db_result(db_query(db_rewrite_sql('SELECT parent FROM {term_hierarchy} t WHERE t.tid = %d', 't', 'tid'), $tid));
      $siblings = taxonomy_get_children($parent, $vid);
      $previous_found = FALSE;
      while($siblings){
        $sibling = array_pop($siblings);
        if($sibling->tid == $tid && count($siblings)){
          $previous_term = array_pop($siblings);
          echo _tui_get_last_child($vid, $previous_term->tid);
          exit();
        }
      }
      // No previous siblings - lets return the parent if we're not at the root.
      // If we are at the root, and there are no previous siblings, then this 
      // must be the first term, so there isn't a previous one!
      if($parent){
        echo $parent;
      }else{
        echo $tid;
      }
    }
  }else{
    // We need either the first, or the last term - taxonomy_get_tree would be 
    // the lazy man's choice here!
    if($nextorprevious == 'tui-next'){
      // NEXT - get the first
      $first_child = array_shift(taxonomy_get_children(0, $vid));
      echo $first_child->tid;
    }else{
      $tid = 0;
      echo _tui_get_last_child($vid, $tid);
    }
  }
}

function _tui_get_next_sibling($vid, $tid){
  $parent = db_result(db_query(db_rewrite_sql('SELECT parent FROM {term_hierarchy} t WHERE t.tid = %d', 't', 'tid'), $tid));
  $siblings = taxonomy_get_children($parent, $vid);
  $next_found = FALSE;
  while($siblings){
    $sibling = array_shift($siblings);
    if($sibling->tid == $tid && count($siblings)){
      $next_term = array_shift($siblings);
      return $next_term->tid;
    }
  }
}

function _tui_get_last_child($vid, $tid){
  // PREVIOUS - get the last (trickier)
  $has_children = TRUE;
  while($has_children){
    $children = taxonomy_get_children($tid, $vid);
    if(count($children)){
      $last_child = array_pop($children);
      $tid = $last_child->tid;
    }else{
      $has_children = FALSE;
    }
  }
  return $tid;
}

/**
 * Get the tree (or part of), and return it for embedding in a page
 */
function tui_get_children($tid_and_vid){
  // Get the tree, then theme it, then return it.
  $parts = explode("-", $tid_and_vid);
  $tree = array();
  $results = db_query(db_rewrite_sql('SELECT tid FROM {term_data} t WHERE tid IN (SELECT tid FROM {term_hierarchy} WHERE parent = %d) ORDER BY weight, name', 't', 'tid'), $parts[1]);
  while($tid = db_result($results)){
    $tree[$tid] = db_result(db_query(db_rewrite_sql('SELECT COUNT(*) FROM {term_hierarchy} t WHERE parent = %d', 't', 'tid'), $tid));
  }
  echo theme('tui_tree', $tree);
}

/**
 * Move a term in a tree
 * 
 * Method is one of child, or sibling, whilst from $term is the term to move,
 * and to is the position to move to (child of, or next term after). 
 */
function tui_move_term($method, $vid, $term, $to){
  // Only continue if term != to, as JS can mess things up occasionaly
  if($term == $to){return;}
  $term_bits = explode("-", $term);
  $term = taxonomy_get_term($term_bits[1]);
  $term->parent = db_result(db_query(db_rewrite_sql('SELECT parent FROM {term_hierarchy} t WHERE t.tid = %d', 't', 'tid'), $term->tid));
  $to_bits = explode("-", $to);
  $to = taxonomy_get_term($to_bits[1]);
  $to->parent = db_result(db_query(db_rewrite_sql('SELECT parent FROM {term_hierarchy} t WHERE t.tid = %d', 't', 'tid'), $to->tid));
  $current_term = (array)$term;
  if($method == 'child'){
    $results = db_query(db_rewrite_sql("SELECT t.tid FROM {term_data} t LEFT JOIN {term_hierarchy} h ON h.tid = t.tid WHERE parent = %d AND t.tid != %d AND vid = %d ORDER BY weight, name"), $to->tid, $term->tid, $vid);
    $siblings = array(
      $term->tid
    );
    while($row = db_fetch_array($results)){
      $siblings[] = $row['tid'];
    }
    $term->parent = $to->tid;
  }else{
    $results = db_query(db_rewrite_sql("SELECT t.tid FROM {term_hierarchy} h, {term_data} t WHERE h.tid = t.tid AND parent = %d AND t.tid != %d AND vid = %d ORDER BY weight, name", 't', 'tid'), $to->parent, $term->tid, $vid);
    while($row = db_fetch_array($results)){
      $siblings[] = $row['tid'];
      if($row['tid'] == $to->tid){
        $siblings[] = $term->tid;
      }
    }
    $term->parent = $to->parent;
  }
  $term = (array)$term;
  taxonomy_save_term($term);
  $weight = 1;
  foreach($siblings as $sibling){
    // Note, we're not using a taxonomy function here for efficieny and speed.
    // A little naughty, but hey, it's only the weight FFS!
    db_query('UPDATE {term_data} SET weight = %d WHERE tid = %d', $weight, $sibling);
    $weight++;
  }
  $_SESSION['tui_moved_terms'][$vid][] = array(
    'term' => $current_term,
    'siblings' => $siblings
  );
}

/**
 * Undo move based on sessino
 */
function tui_undo_move($vid){
  if(isset($_SESSION['tui_moved_terms'][$vid]) && count($_SESSION['tui_moved_terms'][$vid])){
    $old_settings = array_pop($_SESSION['tui_moved_terms'][$vid]);
    $current_term = (array)taxonomy_get_term($old_settings['term']['tid']);
    $current_term['parent'] = $old_settings['term']['parent'];
    $current_term['weight'] = $old_settings['term']['weight'];
    taxonomy_save_term($current_term);
    foreach($old_settings['siblings'] as $sibling){
      // Note, we're not using a taxonomy function here for efficieny and speed.
      // A little naughty, but hey, it's only the weight FFS!
      db_query("UPDATE {term_data} SET weight = %d WHERE tid = %d", $sibling['weight'], $sibling['tid']);
    }
    echo $current_term['tid'];
  }
}

/**
 * 
 */
function tui_delete_term($term_id){
  $term_parts = explode("-", $term_id);
  if($term_parts[0] == 'tid' && is_numeric($term_parts[1])){
    taxonomy_del_term($term_parts[1]);
    // We'll unset the annoying message that gets set!
    drupal_get_messages('status');
  }
}

function tui_get_tree($vid, $tids = array(), &$all_tids = array()){
  $tree = array();
  $all_tids = array();
  $results = db_query(db_rewrite_sql('SELECT tid FROM {term_data} t WHERE tid IN (SELECT tid FROM {term_hierarchy} WHERE parent = 0) AND vid = %d ORDER BY weight, name', 't', 'tid'), $vid);
  while($tid = db_result($results)){
    $tree[$tid] = db_result(db_query(db_rewrite_sql('SELECT COUNT(*) FROM {term_hierarchy} t WHERE parent = %d', 't', 'tid'), $tid));
    $all_tids[$tid] = $tid;
  }
  $num_tids = count($tids);
  foreach($tids as $tid){
    // Get the term, just to make sure it still exists
    $term = taxonomy_get_term($tid);
    if($term){
      // Term exists, now we should get all its ancestors
      $ancestors = taxonomy_get_parents_all($tid);
      // If we have more than one ancestor, then we need to do stuff
      if(count($ancestors) > 1){
        // For every term in the list, we need to show all the children of that
        // term (except for the very last one).
        array_shift($ancestors);
        while($ancestors){
          $term = array_shift($ancestors);
          $result = db_query(db_rewrite_sql('SELECT t.tid FROM {term_hierarchy} h, {term_data} t WHERE parent = %d AND vid = %d AND t.tid = h.tid ORDER BY weight, name', 't', 'tid'), $term->tid, $vid);
          $previous_branch = $branch;
          $branch = array();
          while($row = db_fetch_array($result)){
            $all_tids[$row['tid']] = $row['tid'];
            if($row['tid'] == $previous_tid){
              $branch[$row['tid']] = $previous_branch;
            }else{
              $branch[$row['tid']] = array();
            }
          }
          $previous_tid = $term->tid;
        }
        if(is_array($tree[$term->tid])){
          $tree[$term->tid] = array_merge_recursive_distinct($branch, $tree[$term->tid]);
        }else{
          $tree[$term->tid] = $branch;
        }
      }
    }
  }
  return $tree;
}

/**
 * Straight from php.net.
 * 
 * http://www.php.net/manual/en/function.array-merge-recursive.php#96201
 * 
 * Thanks must go to mark dot roduner at gmail dot com
 * RANT
 * Why doesn't the default array_merge_recursive do this anyway.
 * /RANT
 */
function array_merge_recursive_distinct(){
  $arrays = func_get_args();
  $base = array_shift($arrays);
  if(!is_array($base))
    $base = empty($base) ? array() : array(
      $base
    );
  foreach($arrays as $append){
    if(!is_array($append))
      $append = array(
        $append
      );
    foreach($append as $key => $value){
      if(!array_key_exists($key, $base) and !is_numeric($key)){
        $base[$key] = $append[$key];
        continue;
      }
      if(is_array($value) or is_array($base[$key])){
        $base[$key] = array_merge_recursive_distinct($base[$key], $append[$key]);
      }else if(is_numeric($key)){
        if(!in_array($value, $base))
          $base[] = $value;
      }else{
        $base[$key] = $value;
      }
    }
  }
  return $base;
}

/**
 * 
 */
function tui_get_tree_themed($vid){
  $tids = array();
  foreach($_REQUEST as $key => $value){
    if(is_numeric($key) && $key == $value){
      $tids[] = $key;
    }
  }
  // Check to see if we want to display the most recently added.
  if(arg(3) == 'added'){
    $active = db_result(db_query(db_rewrite_sql('SELECT tid FROM {term_data} t WHERE vid = %d ORDER BY tid DESC LIMIT 1', 't', 'tid'), $vid));
    $tids[] = $active;
  }else{
    $active = FALSE;
  }
  $tree = tui_get_tree($vid, $tids);
  echo theme('tui_tree', $tree, $active);
}
