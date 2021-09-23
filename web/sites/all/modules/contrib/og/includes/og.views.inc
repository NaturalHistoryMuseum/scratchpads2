<?php


/**
 * @file
 * Provides support for the Views module.
 */

/**
 * "Group" entity Views definition.
 */
class OgViewsController extends EntityDefaultViewsController {

  /**
   * Override views_data().
   */
  public function views_data() {
    $data = parent::views_data();

    $data['og']['gid']['argument'] = array(
      'handler' => 'og_handler_argument_group_audience_gid',
    );

    $data['og']['etid'] = array(
      'title' => t('Entity id'),
      'help' => t('The numeric id of the entity.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
       ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
    );

    $data['field_data_group_audience']['og_permissions'] = array(
      'group' => t('Group'),
      'title' => t('Group permissions'),
      'help' => t('Filter by group permissions.'),
      // This is a dummy field, so point it to a real field that we need - the
      // group ID
      'real field' => 'group_audience_gid',
      'field' => array(
        'handler' => 'og_handler_field_group_permissions',
      ),
    );

    $data['og_users_roles'] = array(
      'table' => array(
        'group' => t('OG user roles'),
        'join' => array(
          'og_membership' => array(
            'left_field' => 'gid',
            'field' => 'gid',
          ),
        ),
      ),
    );

    $data['og_users_roles']['rid'] = array(
      'title' => t('Role ID'),
      'help' => t('The OG role ID.'),
      // We provide a dummy field to show the roles a user is part of. See
      // 'og_membership - og_roles'.
      // @todo: Add sort and filter.
      'filter' => array(
        'handler' => 'og_handler_filter_group_roles',
      ),
    );

    return $data;
  }
}

/**
 * "OG membership" entity Views definitions.
 */
class OgMembershipViewsController extends EntityDefaultViewsController {

  /**
   * Override views_data().
   */
  public function views_data() {
    $data = parent::views_data();

    $data['og_membership']['state']['filter'] = array(
      'handler' => 'og_handler_filter_group_state',
      'numeric' => TRUE,
      'allow empty' => FALSE,
      'help' => t('Select the state of the group.'),
    );

    $data['og_membership']['etid'] = array(
      'title' => t('Entity id'),
      'help' => t('The numeric id of the entity.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
       ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
    );

    // Improve relationship text.
    $data['og']['og_membership']['title'] = t('OG membership from OG group');
    $data['og']['og_membership']['help'] =  t('The OG membership associated with OG group via the group ID.');
    $data['og']['og_membership']['relationship']['label'] = t('OG membership from OG group');


    $data['og_membership']['og_roles'] = array(
      'title' => t('OG user roles in group'),
      'help' => t('Show all the roles a user belongs to in a group. Requires a relationship to users to be present.'),
      // This is a dummy field, so point it to a real field that we need - the
      // group ID
      'real field' => 'gid',
      'field' => array(
        'handler' => 'og_handler_field_user_roles',
      ),
    );

    return $data;
  }
}

/**
 * Implements hook_views_data_alter().
 */
function og_views_data_alter(&$data) {
  $group_entities = og_get_all_group_entity();
  $group_content_entities = og_get_all_group_content_entity();

  foreach (entity_get_info() as $entity_type => $info) {
    if (!empty($info['fieldable'])) {

      if (!empty($group_entities[$entity_type])) {
        // OG group relationship.
        $data[$info['base table']]['og_rel'] = array(
          'group' => t('OG group'),
          'title' => t('OG group from @entity', array('@entity' => ucfirst($info['label']))),
          'help' => t('The OG group associated with the @entity entity.', array('@entity' => ucfirst($info['label']))),
          'relationship' => array(
            // Pass the entity to the handler.
            'entity' => $entity_type,
            'handler' => 'og_handler_relationship',
            'label' => t('@entity being the OG group', array('@entity' => $entity_type)),
            'base' => 'og',
            'base field' => 'etid',
            'relationship field' => $info['entity keys']['id'],
          ),
        );

        // OG group's related entity.
        $data['og']['og_related_' . $entity_type] = array(
          'group' => t('OG group'),
          'title' => t('@entity from OG group', array('@entity' => ucfirst($info['label']))),
          'help' => t('The @entity entity that is associated with the OG group.', array('@entity' => $info['label'])),
          'relationship' => array(
            'handler' => 'views_handler_relationship',
            'label' => t('@entity from OG group', array('@entity' => $entity_type)),
            'base' => $info['base table'],
            'base field' => $info['entity keys']['id'],
            'relationship field' => 'etid',
          ),
        );
      }

      if (!empty($group_content_entities[$entity_type])) {
        // OG membership relationship.
        $data[$info['base table']]['og_membership_rel'] = array(
          'group' => t('OG membership'),
          'title' => t('OG membership from @entity', array('@entity' => ucfirst($info['label']))),
          'help' => t('The OG membership associated with the @entity entity.', array('@entity' => ucfirst($info['label']))),
          'relationship' => array(
            // Pass the entity to the handler.
            'entity' => $entity_type,
            'handler' => 'og_handler_relationship',
            'label' => t('OG membership from @entity', array('@entity' => $entity_type)),
            'base' => 'og_membership',
            'base field' => 'etid',
            'relationship field' => $info['entity keys']['id'],
          ),
        );

        // OG membership's related entity.
        $data['og_membership']['og_membership_related_' . $entity_type] = array(
          'group' => t('OG membership'),
          'title' => t('@entity from OG membership', array('@entity' => ucfirst($info['label']))),
          'help' => t('The @entity entity that is associated with the OG membership.', array('@entity' => $info['label'])),
          'relationship' => array(
            'handler' => 'views_handler_relationship',
            'label' => t('@entity from OG membership', array('@entity' => $entity_type)),
            'base' => $info['base table'],
            'base field' => $info['entity keys']['id'],
            'relationship field' => 'etid',
          ),
        );
      }
    }
  }

  // Override group audience - "gid" (group ID) field argument.
  $data['og_membership']['group']['argument']['handler'] = 'og_handler_argument_group_audience_gid';

  // Override group and group membership - "state" filter and add a field.
  foreach (array('og', 'og_membership') as $type) {
    $data[$type]['state']['filter']['handler'] = 'og_handler_filter_group_audience_state';
    $data[$type]['state']['field']['handler'] = 'og_handler_field_group_audience_state';
  }

  // Remove group audience - "state" and "created", as they are deprecated and
  // we have this data in the group membership entity.
  foreach (array('state', 'created') as $type) {
    unset($data['field_data_group_audience']['group_audience_'. $type]);
  }
}

/**
 * Implements hook_views_plugins
 */
function og_views_plugins() {
  return array(
    'argument validator' => array(
      'og' => array(
        'title' => t('Organic groups group'),
        'handler' => 'og_plugin_argument_validate_group',
        'path' => drupal_get_path('module', 'og') . '/includes/views',
      ),
    ),
    'argument default' => array(
      'og_user_groups' => array(
        'title' => t('The OG groups of the logged in user'),
        'handler' => 'og_plugin_argument_default_user_groups',
      ),
    ),
  );
}
