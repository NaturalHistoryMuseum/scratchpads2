<?php
/**
 * @file
 * scratchpads_biography.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function scratchpads_biography_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'biography';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'Biography';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Biographies';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access user profiles';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'There are currently no recorded biographies.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: User: Country (field_user_country) */
  $handler->display->display_options['relationships']['field_user_country_iso2']['id'] = 'field_user_country_iso2';
  $handler->display->display_options['relationships']['field_user_country_iso2']['table'] = 'field_data_field_user_country';
  $handler->display->display_options['relationships']['field_user_country_iso2']['field'] = 'field_user_country_iso2';
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = '';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  /* Field: User: Other/given name(s)  */
  $handler->display->display_options['fields']['field_user_given_names']['id'] = 'field_user_given_names';
  $handler->display->display_options['fields']['field_user_given_names']['table'] = 'field_data_field_user_given_names';
  $handler->display->display_options['fields']['field_user_given_names']['field'] = 'field_user_given_names';
  $handler->display->display_options['fields']['field_user_given_names']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_user_given_names']['element_type'] = 'span';
  /* Field: User: Family name */
  $handler->display->display_options['fields']['field_user_family_name']['id'] = 'field_user_family_name';
  $handler->display->display_options['fields']['field_user_family_name']['table'] = 'field_data_field_user_family_name';
  $handler->display->display_options['fields']['field_user_family_name']['field'] = 'field_user_family_name';
  $handler->display->display_options['fields']['field_user_family_name']['label'] = '';
  $handler->display->display_options['fields']['field_user_family_name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['text'] = '<h3><a href=\'/user/[uid]\'>[field_user_given_names] [field_user_family_name]</a></h3>';
  $handler->display->display_options['fields']['field_user_family_name']['element_type'] = 'span';
  $handler->display->display_options['fields']['field_user_family_name']['element_label_colon'] = FALSE;
  /* Field: User: Country */
  $handler->display->display_options['fields']['field_user_country']['id'] = 'field_user_country';
  $handler->display->display_options['fields']['field_user_country']['table'] = 'field_data_field_user_country';
  $handler->display->display_options['fields']['field_user_country']['field'] = 'field_user_country';
  $handler->display->display_options['fields']['field_user_country']['element_type'] = 'span';
  $handler->display->display_options['fields']['field_user_country']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_user_country']['hide_empty'] = TRUE;
  /* Field: User: Institution */
  $handler->display->display_options['fields']['field_user_institution']['id'] = 'field_user_institution';
  $handler->display->display_options['fields']['field_user_institution']['table'] = 'field_data_field_user_institution';
  $handler->display->display_options['fields']['field_user_institution']['field'] = 'field_user_institution';
  $handler->display->display_options['fields']['field_user_institution']['element_type'] = 'span';
  $handler->display->display_options['fields']['field_user_institution']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_user_institution']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_user_institution']['hide_empty'] = TRUE;
  /* Field: User: ORCID identifier */
  $handler->display->display_options['fields']['field_orcid']['id'] = 'field_orcid';
  $handler->display->display_options['fields']['field_orcid']['table'] = 'field_data_field_orcid';
  $handler->display->display_options['fields']['field_orcid']['field'] = 'field_orcid';
  $handler->display->display_options['fields']['field_orcid']['label'] = 'ORCID';
  $handler->display->display_options['fields']['field_orcid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_orcid']['alter']['path'] = 'http://orcid.org/[field_orcid]';
  $handler->display->display_options['fields']['field_orcid']['alter']['external'] = TRUE;
  $handler->display->display_options['fields']['field_orcid']['element_type'] = 'span';
  $handler->display->display_options['fields']['field_orcid']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_orcid']['hide_empty'] = TRUE;
  /* Field: User: Keywords */
  $handler->display->display_options['fields']['field_user_keywords']['id'] = 'field_user_keywords';
  $handler->display->display_options['fields']['field_user_keywords']['table'] = 'field_data_field_user_keywords';
  $handler->display->display_options['fields']['field_user_keywords']['field'] = 'field_user_keywords';
  $handler->display->display_options['fields']['field_user_keywords']['element_type'] = 'span';
  $handler->display->display_options['fields']['field_user_keywords']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['field_user_keywords']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_user_keywords']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_user_keywords']['delta_offset'] = '0';
  /* Field: User: Biography */
  $handler->display->display_options['fields']['field_biography']['id'] = 'field_biography';
  $handler->display->display_options['fields']['field_biography']['table'] = 'field_data_field_biography';
  $handler->display->display_options['fields']['field_biography']['field'] = 'field_biography';
  $handler->display->display_options['fields']['field_biography']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_biography']['type'] = 'text_trimmed';
  $handler->display->display_options['fields']['field_biography']['settings'] = array(
    'trim_length' => '300',
  );
  /* Filter criterion: User: Make this profile public? (field_make_this_profile_public_) */
  $handler->display->display_options['filters']['field_make_this_profile_public__value']['id'] = 'field_make_this_profile_public__value';
  $handler->display->display_options['filters']['field_make_this_profile_public__value']['table'] = 'field_data_field_make_this_profile_public_';
  $handler->display->display_options['filters']['field_make_this_profile_public__value']['field'] = 'field_make_this_profile_public__value';
  $handler->display->display_options['filters']['field_make_this_profile_public__value']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['field_make_this_profile_public__value']['group'] = 1;
  /* Filter criterion: Filter by name */
  $handler->display->display_options['filters']['combine']['id'] = 'combine';
  $handler->display->display_options['filters']['combine']['table'] = 'views';
  $handler->display->display_options['filters']['combine']['field'] = 'combine';
  $handler->display->display_options['filters']['combine']['ui_name'] = 'Filter by name';
  $handler->display->display_options['filters']['combine']['operator'] = 'contains';
  $handler->display->display_options['filters']['combine']['group'] = 1;
  $handler->display->display_options['filters']['combine']['exposed'] = TRUE;
  $handler->display->display_options['filters']['combine']['expose']['operator_id'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['combine']['expose']['operator'] = 'combine_op';
  $handler->display->display_options['filters']['combine']['expose']['identifier'] = 'combine';
  $handler->display->display_options['filters']['combine']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['combine']['fields'] = array(
    'field_user_given_names' => 'field_user_given_names',
    'field_user_family_name' => 'field_user_family_name',
  );
  /* Filter criterion: User: Keywords (field_user_keywords) */
  $handler->display->display_options['filters']['field_user_keywords_tid']['id'] = 'field_user_keywords_tid';
  $handler->display->display_options['filters']['field_user_keywords_tid']['table'] = 'field_data_field_user_keywords';
  $handler->display->display_options['filters']['field_user_keywords_tid']['field'] = 'field_user_keywords_tid';
  $handler->display->display_options['filters']['field_user_keywords_tid']['value'] = '';
  $handler->display->display_options['filters']['field_user_keywords_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_user_keywords_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_user_keywords_tid']['expose']['operator_id'] = 'field_user_keywords_tid_op';
  $handler->display->display_options['filters']['field_user_keywords_tid']['expose']['label'] = 'Keywords';
  $handler->display->display_options['filters']['field_user_keywords_tid']['expose']['operator'] = 'field_user_keywords_tid_op';
  $handler->display->display_options['filters']['field_user_keywords_tid']['expose']['identifier'] = 'field_user_keywords_tid';
  $handler->display->display_options['filters']['field_user_keywords_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['field_user_keywords_tid']['vocabulary'] = 'user_keywords';
  /* Filter criterion: Countries: Name - list */
  $handler->display->display_options['filters']['name_list']['id'] = 'name_list';
  $handler->display->display_options['filters']['name_list']['table'] = 'countries_country';
  $handler->display->display_options['filters']['name_list']['field'] = 'name_list';
  $handler->display->display_options['filters']['name_list']['relationship'] = 'field_user_country_iso2';
  $handler->display->display_options['filters']['name_list']['group'] = 1;
  $handler->display->display_options['filters']['name_list']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_list']['expose']['operator_id'] = 'name_list_op';
  $handler->display->display_options['filters']['name_list']['expose']['label'] = 'Country';
  $handler->display->display_options['filters']['name_list']['expose']['operator'] = 'name_list_op';
  $handler->display->display_options['filters']['name_list']['expose']['identifier'] = 'name_list';
  $handler->display->display_options['filters']['name_list']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  $handler->display->display_options['filters']['name_list']['configuration'] = '1';
  $handler->display->display_options['filters']['name_list']['filter'] = array(
    'enabled' => '0',
    'continents' => array(),
  );
  /* Filter criterion: User: Biography (field_biography) */
  $handler->display->display_options['filters']['field_biography_value']['id'] = 'field_biography_value';
  $handler->display->display_options['filters']['field_biography_value']['table'] = 'field_data_field_biography';
  $handler->display->display_options['filters']['field_biography_value']['field'] = 'field_biography_value';
  $handler->display->display_options['filters']['field_biography_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_biography_value']['group'] = 1;
  $handler->display->display_options['filters']['field_biography_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_biography_value']['expose']['operator_id'] = 'field_biography_value_op';
  $handler->display->display_options['filters']['field_biography_value']['expose']['label'] = 'Biography';
  $handler->display->display_options['filters']['field_biography_value']['expose']['operator'] = 'field_biography_value_op';
  $handler->display->display_options['filters']['field_biography_value']['expose']['identifier'] = 'field_biography_value';
  $handler->display->display_options['filters']['field_biography_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );
  /* Filter criterion: User: Areas of professional interest  (field_areas_of_professional_inte) */
  $handler->display->display_options['filters']['field_areas_of_professional_inte_value']['id'] = 'field_areas_of_professional_inte_value';
  $handler->display->display_options['filters']['field_areas_of_professional_inte_value']['table'] = 'field_data_field_areas_of_professional_inte';
  $handler->display->display_options['filters']['field_areas_of_professional_inte_value']['field'] = 'field_areas_of_professional_inte_value';
  $handler->display->display_options['filters']['field_areas_of_professional_inte_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_areas_of_professional_inte_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_areas_of_professional_inte_value']['expose']['operator_id'] = 'field_areas_of_professional_inte_value_op';
  $handler->display->display_options['filters']['field_areas_of_professional_inte_value']['expose']['label'] = 'Areas of professional interest';
  $handler->display->display_options['filters']['field_areas_of_professional_inte_value']['expose']['operator'] = 'field_areas_of_professional_inte_value_op';
  $handler->display->display_options['filters']['field_areas_of_professional_inte_value']['expose']['identifier'] = 'field_areas_of_professional_inte_value';
  $handler->display->display_options['filters']['field_areas_of_professional_inte_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'biographies';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Biographies';
  $handler->display->display_options['menu']['weight'] = '50';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $export['biography'] = $view;

  return $export;
}
