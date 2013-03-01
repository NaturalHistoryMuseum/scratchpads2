<?php
/**
 * @file
 * Tests for Rules support.
 */

/**
 * Functional test of Relation's integration with Rules.
 */
class RelationRulesTestCase extends RelationTestCase {
  public static function getInfo() {
    return array(
      'name' => 'Relation Rules integration test',
      'description' => 'Test the Rules integration.',
      'group' => 'Relation',
    );
  }

  function setUp() {
    parent::setUp('relation');
    // While setUp fails for non-existing modules, module_enable() doesn't.
    module_enable(array('rules'));
  }

  /**
   * Test to create a relation in different ways by executing a rule.
   */
  function testRelationCreateRelation() {
    // We don't want test failures if the Rules module isn't used.
    if (module_exists('rules')) {
      $node = $this->drupalCreateNode(array('type' => 'article'));
      $user = $this->drupalCreateUser();
      $endpoints = array(
        entity_metadata_wrapper('node', $node),
        entity_metadata_wrapper('user', $user),
      );

      // Test to create a relation by setting two endpoints and saving it. This
      // primarily tests the endpoint property set callback.
      $rule = rule();
      $rule->action('entity_create', array(
        'type' => 'relation',
        'param_relation_type' => $this->relation_types['symmetric']['relation_type'],
        'param_author:select' => 'site:current-user',
        'param_endpoints' => $endpoints,
        'entity_created:var' => 'relation',
      ));
      $rule->execute();

      // There is no way for the rule to return the created relation. So get the
      // last inserted id, which should be the relation we are looking for.
      $rid = db_query('SELECT MAX(rid) FROM {relation}')->fetchField();
      // If all went well, we should now have a relation with correct endpoints.
      $relation = relation_load($rid);
      $correct = ($relation->endpoints[LANGUAGE_NONE][0]['entity_id'] == $node->nid) && ($relation->endpoints[LANGUAGE_NONE][1]['entity_id'] == $user->uid);
      $this->assertTrue($correct, 'Relation was created by setting two endpoints from rule context and saving it.');

      // Test to create a relation by first fetching the endpoints from the
      // last relation and then settings them in a new one. This primarily tests
      // the endpoint property get callback.
      $rule = rule();
      // This will load a relation into the context of the rule.
      $rule->action('entity_fetch', array('type' => 'relation', 'id' => $rid));
      $rule->action('entity_create', array(
        'type' => 'relation',
        'param_relation_type' => $this->relation_types['symmetric']['relation_type'],
        'param_author:select' => 'site:current-user',
        // Now, load the endpoints from the fetched relation, into a new relation.
        'param_endpoints:select' => 'entity-fetched:endpoints',
        'entity_created:var' => 'relation',
      ));
      $rule->execute();

      // There is no way for the rule to return the created relation. So get the
      // last inserted id, which should be the relation we are looking for.
      $rid = db_query('SELECT MAX(rid) FROM {relation}')->fetchField();
      $relation = relation_load($rid);
      // The $node and the $user should be the the same as in the last test, since
      // we fetched the endpoits from that relation.
      $correct = ($relation->endpoints[LANGUAGE_NONE][0]['entity_id'] == $node->nid) && ($relation->endpoints[LANGUAGE_NONE][1]['entity_id'] == $user->uid);
      $this->assertTrue($correct, 'Relation was created by fetching endpoints from another relation and saving it.');


      // Tests that relation properties are available in rules. So create a new
      // rule with relation action, and check that the text is available among the
      // "Data selectors" list.

      // Enables UI module for rules.
      module_enable(array('rules_admin'));
      // Resets permissions, because module_enable() doesn't clear static cache.
      $this->checkPermissions(array(), TRUE);
      // Creates new user.
      $user = $this->drupalCreateUser(array('administer rules'));
      $this->drupalLogin($user);

      // Cleares the cache, becaues UI module's menu items don't exists.
      drupal_flush_all_caches();

      // Creates rule.
      $this->drupalGet('admin/config/workflow/rules/reaction/add');
      $post = array(
        'settings[label]' => 'Similar',
        'settings[name]' => 'similar',
        'event' => 'node_view',
      );
      $this->drupalPost(NULL, $post, t('Save'));

      // Adds action.
      $this->clickLink(t('Add action'));
      $post = array(
        'element_name' => 'relation_rules_load_related',
      );
      $this->drupalPost(NULL, $post, t('Continue'));
      $this->assertText('node:relation-directional-node-reverse:0', t('The created relation porperties\' are found.'));
    }
  }

  function testRelationLoadRelatedRules() {
    if (module_exists('rules')) {
      $nid = $this->node1->nid;
      variable_set('relation_rules_test_nid', $nid);
      module_enable(array('relation_rules_test'));
      $this->node6 = $this->drupalCreateNode(array('type' => 'article'));
      for ($i = 1; $i <= 6; $i++) {
        $property = "node$i";
        $node = $this->$property;
        if ($node->nid == $nid) {
          $this->assertFalse((bool) db_query_range('SELECT * FROM {node} WHERE nid = :nid', 0, 1, array(':nid' => $nid))->fetchField(), t('The correct node was deleted'));
        }
        else {
          $nids[] = $node->nid;
        }
      }
      $count = count($nids);
      $this->assertEqual($count, db_query('SELECT COUNT(*) FROM {node} WHERE nid IN (:nids)', array(':nids' => $nids))->fetchField(), t('@count other nodes were found', array('@count' => $count)));
    }
  }
}
