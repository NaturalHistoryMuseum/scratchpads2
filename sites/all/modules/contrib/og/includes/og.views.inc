<?php


/**
 * @file
 * Provides support for the Views module.
 */

/**
 * Implements hook_views_data().
 */
function og_views_data() {
  $data = array();

  $data['og']['table']['group'] = t('Group');

  // Advertise this table as a possible base table
  $data['og']['table']['base'] = array(
    'field' => 'gid',
    'title' => t('Group'),
    'help' => t("Groups can be different entities that have other content associated with them."),
  );

  $data['og']['gid'] = array(
    'title' => t('Group ID'),
    'help' => t('The group unique ID.'),
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
      'handler' => 'og_handler_argument_group_audience_gid',
    ),
  );

  $data['og']['entity_type'] = array(
    'title' => t("Group's entity type"),
    'help' => t('The entity type of the group.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['og']['etid'] = array(
    'title' => t("Group's entity ID"),
    'help' => t('The entity ID of the group.'),
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

  $data['og']['state'] = array(
    'title' => t("Group's state"),
    'help' => t('The state of the group.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'og_handler_filter_group_state',
      'numeric' => TRUE,
      'allow empty' => FALSE,
      'help' => t('Select the state of the group.'),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['og']['created'] = array(
    'title' => t('Group date'),
    'help' => t('The date the group was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['field_data_group_audience']['user_roles'] = array(
    'group' => t('Group'),
    'title' => t('Group user roles'),
    'help' => t('Show all the groups a user belongs to in a group.'),
    // This is a dummy field, so point it to a real field that we need - the
    // group ID
    'real field' => 'group_audience_gid',
    'field' => array(
      'handler' => 'og_handler_field_user_roles',
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


  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function og_views_handlers() {
  return array(
    // Base class field.
    'og_handler_field_prerender_list' => array(
     'parent' => 'views_handler_field_prerender_list',
    ),
    // Field
    'og_handler_field_group_audience_state' => array(
      'parent' => 'views_handler_field',
    ),
    'og_handler_field_user_roles' => array(
      'parent' => 'og_handler_field_prerender_list',
    ),
    'og_handler_field_group_permissions' => array(
      'parent' => 'og_handler_field_prerender_list',
    ),
    // Filter
    'og_handler_filter_group_state' => array(
      'parent' => 'views_handler_filter_in_operator',
    ),
    'og_handler_filter_group_audience_state' => array(
      'parent' => 'views_handler_filter_in_operator',
    ),
    // Relationship
    'og_handler_relationship' => array(
      'parent' => 'views_handler_relationship',
    ),
    // Arguments
    'og_handler_argument_group_audience_gid' => array(
      'parent' => 'views_handler_argument_numeric',
    ),
  );
}


/**
 * Implements hook_views_data_alter().
 */
function og_views_data_alter(&$data) {
  $items = module_invoke_all('og_views_relationship');

  foreach ($items as $item) {
    $entity = entity_get_info($item['entity']);
    $title = t('@entity group', array('@entity' => ucfirst($entity['label'])));
    $help = t('Add information on groups that belong to the @entity entity.', array('@entity' => ucfirst($entity['label'])));

    // Add the group relationship.
    $data[$item['views table']]['og_rel'] = array(
      'group' => t('Group'),
      'title' => $title,
      'help' => $help,
      'relationship' => array(
        'entity' => $item['entity'],
        'handler' => 'og_handler_relationship',
        'label' => t('group'),
        'base' => 'og',
        'base field' => 'etid',
        'relationship field' => $item['join field'],
      ),
    );
  }

  // TODO: Maybe move field overrides to field_views_data_alter().
  // TODO: Override also field revisions table.

  // Override group audience - "gid" (group ID) field argument.
  $data['field_data_group_audience']['group_audience_gid']['argument']['handler'] = 'og_handler_argument_group_audience_gid';

  $data['field_data_group_audience']['group_audience_gid']['field']['handler'] = 'views_handler_field_numeric';

  // Override group audience - "created".
  foreach (array('field', 'filter', 'sort', 'argument') as $type) {
    $data['field_data_group_audience']['group_audience_created'][$type]['handler'] = 'views_handler_' . $type . '_date';
  }

  // Override group audience - "state" filter and add a field.

  $data['field_data_group_audience']['group_audience_state']['filter']['handler'] = 'og_handler_filter_group_audience_state';
  $data['field_data_group_audience']['group_audience_state']['field']['handler'] = 'og_handler_field_group_audience_state';
}
