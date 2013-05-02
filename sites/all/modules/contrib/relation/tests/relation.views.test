<?php
/**
 * @file
 * Tests for Views support in the Relation module.
 */

/**
 * Functional test of Relation's integration with Views.
 */
class RelationViewsTestCase extends RelationTestCase {

  public static function getInfo() {
    return array(
      'name' => 'Relation Views test',
      'description' => 'Tests the Relation Views support.',
      'group' => 'Relation',
    );
  }

  function setUp() {
    parent::setUp('relation', 'relation_entity_collector');
    // While setUp fails for non-existing modules, module_enable() doesn't.
    module_enable(array('views'));
  }

  /**
   * Tests views with relations as a base table.
   */
  function testRelationsAsBaseTable() {
    if (!module_exists('views')) {
      return;
    }

    foreach (array('symmetric', 'directional', 'octopus') as $relation_type) {
      $view = new view;
      $view->base_table = 'relation';
      $handler = $view->new_display('default');
      $handler->display->display_options['relationships']['uid']['id'] = 'uid';
      $handler->display->display_options['relationships']['uid']['table'] = 'relation';
      $handler->display->display_options['relationships']['uid']['field'] = 'uid';
      $handler->display->display_options['fields']['rid']['id'] = 'rid';
      $handler->display->display_options['fields']['rid']['table'] = 'relation';
      $handler->display->display_options['fields']['rid']['field'] = 'rid';
      $handler->display->display_options['fields']['relation_type']['id'] = 'relation_type';
      $handler->display->display_options['fields']['relation_type']['table'] = 'relation';
      $handler->display->display_options['fields']['relation_type']['field'] = 'relation_type';
      $handler->display->display_options['fields']['arity']['id'] = 'arity';
      $handler->display->display_options['fields']['arity']['table'] = 'relation';
      $handler->display->display_options['fields']['arity']['field'] = 'arity';
      $handler->display->display_options['arguments']['rid']['id'] = 'rid';
      $handler->display->display_options['arguments']['rid']['table'] = 'relation';
      $handler->display->display_options['arguments']['rid']['field'] = 'rid';
      $view->set_arguments(array($this->{'rid_' . $relation_type}));
      $view->execute();

      $result = array_shift($view->result);
      $this->assertTrue(empty($view->result));
      $this->assertEqual($result->relation_relation_type, $relation_type);
      switch ($relation_type) {
        case 'symmetric':
          // Relation #1 is of type symmetric and has 2 endpoints.
          $this->assertEqual($result->rid, 1);
          $this->assertEqual($result->relation_arity, 2);
          break;
        case 'directional':
          // Relation #2 is of type directional and has 2 endpoints.
          $this->assertEqual($result->rid, 2);
          $this->assertEqual($result->relation_arity, 2);
          break;
        case 'directional_entitysame':
          // Relation #4 is of type directional_entitysame and has 2 endpoints.
          $this->assertEqual($result->rid, 4);
          $this->assertEqual($result->relation_arity, 2);
          break;
        case 'directional_entitydifferent':
          // Relation #7 is of type directional_entitydifferent and has 2 endpoints.
          $this->assertEqual($result->rid, 7);
          $this->assertEqual($result->relation_arity, 2);
          break;
        case 'octopus':
          // Relation #4 is of type octopus and has 4 endpoints.
          $this->assertEqual($result->rid, 9);
          $this->assertEqual($result->relation_arity, 4);
          break;
      }
    }
  }

  /**
   * Tests views with symmetric relations.
   */
  function testSymmetricRelations() {
    if (!module_exists('views')) {
      return;
    }

    foreach (array(FALSE, TRUE) as $required) {
      $view = new view;
      $handler = $view->new_display('default');
      $handler->display->display_options['relationships']['relation_symmetric_node']['id'] = 'relation_symmetric_node';
      $handler->display->display_options['relationships']['relation_symmetric_node']['table'] = 'node';
      $handler->display->display_options['relationships']['relation_symmetric_node']['field'] = 'relation_symmetric_node';
      $handler->display->display_options['relationships']['relation_symmetric_node']['required'] = $required;
      $handler->display->display_options['fields']['nid']['id'] = 'nid';
      $handler->display->display_options['fields']['nid']['table'] = 'node';
      $handler->display->display_options['fields']['nid']['field'] = 'nid';
      $handler->display->display_options['fields']['nid']['relationship'] = 'relation_symmetric_node';
      $handler->display->display_options['fields']['nid2']['id'] = 'nid2';
      $handler->display->display_options['fields']['nid2']['table'] = 'node';
      $handler->display->display_options['fields']['nid2']['field'] = 'nid';
      $handler->display->display_options['arguments']['nid']['id'] = 'nid';
      $handler->display->display_options['arguments']['nid']['table'] = 'node';
      $handler->display->display_options['arguments']['nid']['field'] = 'nid';

      // The result should be the same for required and non-required for
      // argument node 1: Relation from 1 to 4.
      $view->set_arguments(array($this->node1->nid));
      $view->execute();

      $this->assertEqual(count($view->result), 1);
      $result = array_pop($view->result);
      $this->assertEqual($result->nid, $this->node1->nid);
      $this->assertEqual($result->node_node_nid, $this->node4->nid);

      $view = $view->clone_view();

      // The result should be different for required and non-required for
      // argument node 6: for required, there should be no results. For
      // non-required, node 6 may be returned.
      $view->set_arguments(array($this->node6->nid));
      $view->execute();

      if ($required) {
        $this->assertEqual(count($view->result), 0);
      }
      else {
        $this->assertEqual(count($view->result), 1);
        $result = array_pop($view->result);
        $this->assertEqual($result->nid, $this->node6->nid);
        $this->assertEqual($result->node_node_nid, NULL);
      }
    }
  }

  /**
   * Tests views with directional relations to source, to target and to both.
   */
  function testDirectionalRelations() {
    if (!module_exists('views')) {
      return;
    }

    foreach (array(FALSE, TRUE) as $required) {
      for ($r_index = -1; $r_index < 2; $r_index++) {
        $view = new view;
        $handler = $view->new_display('default');
        $handler->display->display_options['relationships']['relation_directional_node']['id'] = 'relation_directional_node';
        $handler->display->display_options['relationships']['relation_directional_node']['table'] = 'node';
        $handler->display->display_options['relationships']['relation_directional_node']['field'] = 'relation_directional_node';
        $handler->display->display_options['relationships']['relation_directional_node']['required'] = $required;
        $handler->display->display_options['relationships']['relation_directional_node']['r_index'] = $r_index;
        $handler->display->display_options['fields']['nid_source']['id'] = 'nid';
        $handler->display->display_options['fields']['nid_source']['table'] = 'node';
        $handler->display->display_options['fields']['nid_source']['field'] = 'nid';
        $handler->display->display_options['fields']['nid_target']['id'] = 'nid';
        $handler->display->display_options['fields']['nid_target']['table'] = 'node';
        $handler->display->display_options['fields']['nid_target']['field'] = 'nid';
        $handler->display->display_options['fields']['nid_target']['relationship'] = 'relation_directional_node';
        $handler->display->display_options['arguments']['nid']['id'] = 'nid';
        $handler->display->display_options['arguments']['nid']['table'] = 'node';
        $handler->display->display_options['arguments']['nid']['field'] = 'nid';

        // First test: node that has relations. The results should be the same
        // for required and non-required relations.
        $view->set_arguments(array($this->node3->nid));
        $view->execute();

        switch ($r_index) {
          case -1:
            // Directional, both ways.
            $this->assertEqual(count($view->result), 2);
            $targetmatches = array($this->node1->nid => TRUE, $this->node4->nid => TRUE);
            foreach ($view->result as $result) {
              $this->assertEqual($result->nid, $this->node3->nid);
              unset($targetmatches[$result->node_node_nid]);
            }
            $this->assertFalse($targetmatches);
            break;
          case 0:
            // Source. This finds the 3->4 relation.
            $this->assertEqual(count($view->result), 1);
            $this->assertEqual($view->result[0]->nid, $this->node3->nid);
            $this->assertEqual($view->result[0]->node_node_nid, $this->node4->nid);
            break;
          case 1:
            // Target. This finds the 1->3 relation.
            $this->assertEqual(count($view->result), 1);
            $this->assertEqual($view->result[0]->nid, $this->node3->nid);
            $this->assertEqual($view->result[0]->node_node_nid, $this->node1->nid);
            break;
        }

        // Second test: node that has no relations. The results should be that
        // no results are found for the required relation, and 6 / NULL
        // for the optional relation.
        $view = $view->clone_view(); 
        $view->set_arguments(array($this->node6->nid));
        $view->execute();

        if ($required) {
          $this->assertEqual(count($view->result), 0);
        }
        else {
          $this->assertEqual(count($view->result), 1);
          $result = array_pop($view->result);
          $this->assertEqual($result->nid, $this->node6->nid);
          $this->assertEqual($result->node_node_nid, NULL);
        }
      }
    }
  }

  /**
   * Tests views with forward directional relations to source, to target and to
   * both with the same entities types.
   */
  function testForwardDirectionalSameEntityRelations() {
    if (!module_exists('views')) {
      return;
    }

    for ($r_index = -1; $r_index < 2; $r_index++) {
      $view = new view;
      $view->base_table = 'node';
      $handler = $view->new_display('default');
      $handler->display->display_options['relationships']['relation_directional_entitysame_node']['id'] = 'relation_directional_entitysame_node';
      $handler->display->display_options['relationships']['relation_directional_entitysame_node']['table'] = 'node';
      $handler->display->display_options['relationships']['relation_directional_entitysame_node']['field'] = 'relation_directional_entitysame_node';
      $handler->display->display_options['relationships']['relation_directional_entitysame_node']['required'] = 1;
      $handler->display->display_options['relationships']['relation_directional_entitysame_node']['r_index'] = $r_index;
      $handler->display->display_options['fields']['nid']['id'] = 'nid';
      $handler->display->display_options['fields']['nid']['table'] = 'node';
      $handler->display->display_options['fields']['nid']['field'] = 'nid';
      $handler->display->display_options['fields']['nid']['relationship'] = 'relation_directional_entitysame_node';
      $handler->display->display_options['arguments']['nid']['id'] = 'nid';
      $handler->display->display_options['arguments']['nid']['table'] = 'node';
      $handler->display->display_options['arguments']['nid']['field'] = 'nid';
      $view->set_arguments(array($this->node3->nid));
      $view->execute();

      switch ($r_index) {
        case -1:
          // Directional, both ways.
          $this->assertEqual(count($view->result), 3);
          $matches = array($this->node4->nid => TRUE, $this->node4->nid => TRUE, $this->node5->nid => TRUE);
          foreach ($view->result as $result) {
            unset($matches[$result->node_node_nid]);
          }
          $this->assertFalse($matches);
          break;
        case 0:
          // Source. This finds the p3->p4 and p3->p5 relations.
          $this->assertEqual(count($view->result), 2);
          $matches = array($this->node4->nid => TRUE, $this->node5->nid => TRUE);
          foreach ($view->result as $result) {
            unset($matches[$result->node_node_nid]);
          }
          $this->assertFalse($matches);
          break;
        case 1:
          // Target. This finds the p4->p3 relation.
          $this->assertEqual(count($view->result), 1);
          $matches = array($this->node4->nid => TRUE);
          foreach ($view->result as $result) {
            unset($matches[$result->node_node_nid]);
          }
          $this->assertFalse($matches);
          break;
      }
    }
  }

  /**
   * Tests views with reverse directional relations to source, to target and to
   * both with the same entities types.
   */
  function testReverseDirectionalSameEntityRelations() {
    for ($r_index = -1; $r_index < 2; $r_index++) {
      $view = new view;
      $view->base_table = 'node';
      $handler = $view->new_display('default');
      $handler->display->display_options['relationships']['relation_directional_entitysame_node']['id'] = 'relation_directional_entitysame_node';
      $handler->display->display_options['relationships']['relation_directional_entitysame_node']['table'] = 'node';
      $handler->display->display_options['relationships']['relation_directional_entitysame_node']['field'] = 'relation_directional_entitysame_node';
      $handler->display->display_options['relationships']['relation_directional_entitysame_node']['required'] = 1;
      $handler->display->display_options['relationships']['relation_directional_entitysame_node']['r_index'] = $r_index;
      $handler->display->display_options['fields']['nid']['id'] = 'nid';
      $handler->display->display_options['fields']['nid']['table'] = 'node';
      $handler->display->display_options['fields']['nid']['field'] = 'nid';
      $handler->display->display_options['arguments']['nid']['id'] = 'nid';
      $handler->display->display_options['arguments']['nid']['table'] = 'node';
      $handler->display->display_options['arguments']['nid']['field'] = 'nid';
      $handler->display->display_options['arguments']['nid']['relationship'] = 'relation_directional_entitysame_node';
      $view->set_arguments(array($this->node3->nid));
      $view->execute();

      switch ($r_index) {
        case -1:
          // Directional, both ways.
          $this->assertEqual(count($view->result), 3);
          $matches = array($this->node4->nid => TRUE, $this->node5->nid => TRUE, $this->node4->nid => TRUE);
          foreach ($view->result as $result) {
            unset($matches[$result->nid]);
          }
          $this->assertFalse($matches);
          break;
        case 0:
          // Reverse source. This finds the p4->p3 relation.
          $this->assertEqual(count($view->result), 1);
          $matches = array($this->node4->nid => TRUE);
          foreach ($view->result as $result) {
            unset($matches[$result->nid]);
          }
          $this->assertFalse($matches);
          break;
        case 1:
          // Reverse target. This finds the p3->p4 and p3->p5 relations.
          $this->assertEqual(count($view->result), 2);
          $matches = array($this->node4->nid => TRUE, $this->node5->nid => TRUE);
          foreach ($view->result as $result) {
            unset($matches[$result->nid]);
          }
          $this->assertFalse($matches);
          break;
      }
    }
  }

  /**
   * Tests views with forward directional relations to source, to target and to
   * both with different entities types.
   */
  function testForwardDirectionalDifferentEntityRelations() {
    for ($r_index = -1; $r_index < 2; $r_index++) {
      $view = new view;
      $view->base_table = 'users';
      $handler = $view->new_display('default');
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_node']['id'] = 'relation_directional_entitydifferent_node';
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_node']['table'] = 'users';
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_node']['field'] = 'relation_directional_entitydifferent_node';
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_node']['required'] = 1;
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_node']['r_index'] = $r_index;
      $handler->display->display_options['fields']['nid']['id'] = 'nid';
      $handler->display->display_options['fields']['nid']['table'] = 'node';
      $handler->display->display_options['fields']['nid']['field'] = 'nid';
      $handler->display->display_options['fields']['nid']['relationship'] = 'relation_directional_entitydifferent_node';
      $handler->display->display_options['arguments']['uid']['id'] = 'uid';
      $handler->display->display_options['arguments']['uid']['table'] = 'users';
      $handler->display->display_options['arguments']['uid']['field'] = 'uid';
      $view->set_arguments(array($this->user1->uid));
      $view->execute();

      switch ($r_index) {
        case -1:
          // Directional, both ways.
          $this->assertEqual(count($view->result), 2);
          $matches = array($this->node3->nid => TRUE, $this->node4->nid => TRUE);
          foreach ($view->result as $result) {
            unset($matches[$result->node_users_nid]);
          }
          $this->assertFalse($matches);
          break;
        case 0:
          // Source. This finds the u1->p3 and u1->p4 relation.
          $this->assertEqual(count($view->result), 2);
          $matches = array($this->node3->nid => TRUE, $this->node4->nid => TRUE);
          foreach ($view->result as $result) {
            unset($matches[$result->node_users_nid]);
          }
          $this->assertFalse($matches);
          break;
        case 1:
          // Target. This finds no relations.
          $this->assertEqual(count($view->result), 0);
          $matches = array();
          foreach ($view->result as $result) {
            unset($matches[$result->node_users_nid]);
          }
          $this->assertFalse($matches);
          break;
      }
    }
  }

  /**
   * Tests views with reverse directional relations to source, to target and to
   * both with different entities types.
   */
  function testReverseDirectionalDifferentEntityRelations() {
    for ($r_index = -1; $r_index < 2; $r_index++) {
      $view = new view;
      $view->base_table = 'node';
      $handler = $view->new_display('default');
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_user']['id'] = 'relation_directional_entitydifferent_user';
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_user']['table'] = 'node';
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_user']['field'] = 'relation_directional_entitydifferent_user';
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_user']['required'] = 1;
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_user']['r_index'] = $r_index;
      $handler->display->display_options['fields']['nid']['id'] = 'nid';
      $handler->display->display_options['fields']['nid']['table'] = 'node';
      $handler->display->display_options['fields']['nid']['field'] = 'nid';
      $handler->display->display_options['arguments']['uid']['id'] = 'uid';
      $handler->display->display_options['arguments']['uid']['table'] = 'users';
      $handler->display->display_options['arguments']['uid']['field'] = 'uid';
      $handler->display->display_options['arguments']['uid']['relationship'] = 'relation_directional_entitydifferent_user';
      $view->set_arguments(array($this->user1->uid));
      $view->execute();

      switch ($r_index) {
        case -1:
          // Directional, both ways.
          $this->assertEqual(count($view->result), 2);
          $matches = array($this->node3->nid => TRUE, $this->node4->nid => TRUE);
          foreach ($view->result as $result) {
            unset($matches[$result->nid]);
          }
          $this->assertFalse($matches);
          break;
        case 0:
          // Source. This finds no relations.
          $this->assertEqual(count($view->result), 0);
          $matches = array();
          foreach ($view->result as $result) {
            unset($matches[$result->nid]);
          }
          $this->assertFalse($matches);
          break;
        case 1:
          // Target. This finds the u1->p3 and u1->p4 relation.
          $this->assertEqual(count($view->result), 2);
          $matches = array($this->node3->nid => TRUE, $this->node4->nid => TRUE);
          foreach ($view->result as $result) {
            unset($matches[$result->nid]);
          }
          $this->assertFalse($matches);
          break;
      }
    }
  }

  /**
   * Tests views deduplication.
   */
  function testDeduplication() {
    for ($i = 0; $i < 2; $i++) {
      $view = new view;
      $view->base_table = 'node';
      $handler = $view->new_display('default');
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_user']['id'] = 'relation_directional_entitydifferent_user';
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_user']['table'] = 'node';
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_user']['field'] = 'relation_directional_entitydifferent_user';
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_user']['required'] = 1;
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_node']['id'] = 'relation_directional_entitydifferent_node';
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_node']['table'] = 'users';
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_node']['field'] = 'relation_directional_entitydifferent_node';
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_node']['relationship'] = 'relation_directional_entitydifferent_user';
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_node']['required'] = 1;
      $handler->display->display_options['relationships']['relation_directional_entitydifferent_node']['entity_deduplication_right'] = $i;
      $handler->display->display_options['fields']['nid']['id'] = 'nid';
      $handler->display->display_options['fields']['nid']['table'] = 'node';
      $handler->display->display_options['fields']['nid']['field'] = 'nid';
      $handler->display->display_options['fields']['nid_1']['id'] = 'nid_1';
      $handler->display->display_options['fields']['nid_1']['table'] = 'node';
      $handler->display->display_options['fields']['nid_1']['field'] = 'nid';
      $handler->display->display_options['fields']['nid_1']['relationship'] = 'relation_directional_entitydifferent_node';
      $handler->display->display_options['arguments']['uid']['id'] = 'uid';
      $handler->display->display_options['arguments']['uid']['table'] = 'users';
      $handler->display->display_options['arguments']['uid']['field'] = 'uid';
      $handler->display->display_options['arguments']['uid']['relationship'] = 'relation_directional_entitydifferent_user';
      $view->set_arguments(array($this->user1->uid));
      $view->execute();
      $this->assertEqual(count($view->result), 4 - 2 * $i);
      $possible_nids = array($this->node3->nid => TRUE, $this->node4->nid => TRUE);
      $fail = FALSE;
      $all_results = array();
      foreach ($view->result as $result) {
        $this->assertTrue(isset($possible_nids[$result->nid]) && isset($possible_nids[$result->node_users_nid]), 'Correct nid found');
        $this->assertFalse(isset($all_results[$result->nid][$result->node_users_nid]), 'No row duplication');
        $all_results[$result->nid][$result->node_users_nid] = TRUE;
        $fail = $fail || ($i && $result->nid == $result->node_users_nid);
      }
      if ($i) {
        $this->assertFalse($fail, 'Deduplication worked');
      }
    }
  }
}
