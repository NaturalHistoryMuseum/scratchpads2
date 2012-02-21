<?php
//function biblio_views_plugins() {
//  return array(
//    'display' => array(
//      'biblio' => array(
//        'title' => t('Biblio Page'),
//        'help' => t(''),
//        'handler' => 'views_plugin_display_page_biblio',
//        'path' => drupal_get_path('module', 'biblio') . '/views', // not necessary for most modules
////        'theme' => 'views_view_row_node',
//        'base' => array('node'), // only works with 'node' as base.
//        'uses options' => TRUE,
//        'type' => 'normal',
//        ),
//      ),
//    'style' => array(
//      // ... list of style plugins,
//      ),
//    'row' => array(
//      // ... list of row style plugins,
//      ),
//    'argument default' => array(
//      // ... list of argument default plugins,
//      ),
//    'argument validator' => array(
//      // ... list of argument validator plugins,
//      ),
//    'access' => array(
//      // ... list of access plugins,
//      ),
//   );
//}

/**
 * Implementation of hook_views_data().
 *
 * Exposes all fields to the views system.
 */
function biblio_views_data() {
  $viewsdata = array();

  /**************** biblio table **************/
  $data = array();
  // everything belongs to the Biblio group
  $data['table']['group'] = t('Biblio');

  $data['citation'] = array(
    'title' => t('Biblio Citation'),
    'help'  => t("Display the complete citation for a given node"),
    'field' => array(
      'handler' => 'biblio_handler_citation',
    ),
  );
  $result = db_query('SELECT f.name,f.type,ftd.title,ft.ftdid FROM {biblio_fields} f
                      INNER JOIN {biblio_field_type} AS ft ON ft.fid = f.fid
                      INNER JOIN {biblio_field_type_data} ftd ON ft.ftdid = ftd.ftdid
                      WHERE ft.tid = 0');

  foreach ($result as $field) {
    $data[$field->name] = array(
      'title'     => $field->title,
      'help'      => "Display the " . $field->title,
      'field'     => array('handler' => 'biblio_handler_field'),
      'sort'      => array('handler' => 'views_handler_sort'),
      'filter'    => array('handler' => 'views_handler_filter_string'),
      'argument'  => array('handler' => 'views_handler_argument_string')
    );

    // for contrib_widgets we use a special handler:
    if ($field->type == 'contrib_widget') {
      $data[$field->name]['field'] = array(
        'handler' => 'biblio_handler_field_contributor',
        'auth_category' => $field->ftdid,
      );
      unset($data[$field->name]['sort']);
      unset($data[$field->name]['filter']);
      unset($data[$field->name]['argument']);
    }
    if ($field->type == 'text_format') {
      $data[$field->name]['field']['handler'] = 'biblio_handler_field_markup';
    }
  }
  $data['biblio_year']['argument']  = array('handler' => 'views_handler_argument_numeric');  // biblio_year is an int
  $data['biblio_year']['filter']    = array('handler' => 'views_handler_filter_numeric');  // biblio_year is an int
  unset($data['biblio_keywords']); // keywords are in a separate table which has it's own handlers

  $data['table']['join'] = array(
    'node' => array(
      // links directly to node via vid
      'left_field' => 'vid',
      'field'      => 'vid',
      'type'       => 'left',
    ),
    'node_revision' => array(
      'left_field' => 'vid',
      'field'      => 'vid',
      'type'       => 'left',
    ),
    );

  $viewsdata['biblio'] = $data;

  /**************** biblio_types table *********************/

  $data = array();
  $data['table']['group'] = t('Biblio');
  $data['table']['join'] = array(
    'node' => array(
      'left_table' => 'biblio',
      'left_field' => 'biblio_type',
      'field' => 'tid',
    ),
    'node_revision' => array(
      'left_table' => 'biblio',
      'left_field' => 'biblio_type',
      'field' => 'tid',
    ),
  );
  $data['name'] =  array(
    'field'   => array('handler' => 'views_handler_field'),
    'sort'    => array('handler' => 'views_handler_sort'),
    'argument'  => array('handler' => 'views_handler_argument_string'),
    'title'   => t('Type of Publication'),
    'help'    => t('The type of publication: Journal, Book, Conference Paper, etc.')
  );
  $data['tid'] =  array(
    'argument' => array(
      'handler' => 'biblio_handler_argument_many_to_one',
      'name table' => 'biblio_types',
      'name field' => 'name',
      'empty name field' => t('No Type'),
      'numeric' => TRUE,
    ),
    'filter'  => array('handler' => 'biblio_handler_filter_biblio_type'),
    'title'   => t('Type of Publication'),
    'help'    => t('The type of publication: Journal, Book, Conference Paper, etc.')
  );

  $viewsdata['biblio_types'] = $data;

  /**************** biblio contributors table **************/

  $data = array();
  $data['table']['group'] = t('Biblio');
  $data['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
      'type'       => 'inner',
    ),
    'node_revision' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    // This is provided for many_to_one argument
    'biblio' => array(
      'field' => 'vid',
      'left_field' => 'vid',
      'type'       => 'inner',
    ),
    'users' => array(
      'left_table' => 'biblio_contributor_data',
      'field' => 'cid',
      'left_field' => 'cid',
    ),
  );
  $data['cid'] = array(
    'title' => t('Author ID'),
    'help' => t('Filter by author id.'),
    'argument' => array(
      'handler'    => 'biblio_handler_argument_many_to_one',
      'name table' => 'biblio_contributor_data',
      'name field' => 'lastname',
      'empty name field' => t('No Author'),
      'numeric' => TRUE,
    ),
  'filter' => array(
      'handler' => 'biblio_handler_filter_contributor',
    )
  );
  $data['rank'] = array(
    'title' => t('Author Rank'),
    'help' => t('Rank defines the author order "0" being the first author, "1" the second and so on.'),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    )
  );
  $data['auth_type'] = array(
    'title' => t('Author Type'),
    'help' => t('Rank defines the type of author Author, Editor, Translator and so on.'),
    'filter' => array(
      'handler' => 'biblio_handler_filter_biblio_contributor_auth_type',
    )
  );

  $viewsdata['biblio_contributor'] = $data;

/**************** biblio_contributor_data table ***********/
  $data = array();
  $data['table']['group'] = t('Biblio');
  $data['table']['base'] = array(
    'field' => 'cid',
    'title' => t('Biblio Authors'),
  );
  $data['table']['join'] = array(
    'biblio_contributor' => array(
      'left_field' => 'cid',
      'field' => 'cid',
    ),
    'node' => array(
      'left_table' => 'biblio_contributor',
      'left_field' => 'cid',
      'field' => 'cid',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'drupal_uid',
    ),
    );
  $data['drupal_uid'] =  array(
    'field'     => array('handler' => 'views_handler_field'),
    'filter'    => array('handler' => 'views_handler_filter'),
    'argument'  => array('handler' => 'views_handler_argument_numeric'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'label' => t('user'),
    ),

    'title'     => t('Drupal UserID'),
    'help'      => t('This is the Drupal user associated with the Biblio Author.')
  );
  $data['cid'] =  array(
    'field'     => array('handler' => 'views_handler_field'),
    'sort'      => array('handler' => 'biblio_handler_sort_contributor_lastname'),
    'filter'    => array('handler' => 'biblio_handler_filter_contributor_lastname'),
    'argument'  => array('handler' => 'views_handler_argument_string'),
    'title'     => t('Author id'),
    'help'      => t('Author id')
  );
  $data['lastname'] =  array(
    'field'     => array('handler' => 'views_handler_field'),
    'sort'      => array('handler' => 'biblio_handler_sort_contributor_lastname'),
    'filter'    => array('handler' => 'biblio_handler_filter_contributor_lastname'),
    'argument'  => array('handler' => 'views_handler_argument_string'),
    'title'     => t('Author last name'),
    'help'      => t('Author last name')
  );
  $data['firstname'] =  array(
    'field'     => array('handler' => 'views_handler_field'),
    'sort'      => array('handler' => 'views_handler_sort'),
    'filter'    => array('handler' => 'views_handler_filter'),
    'argument'  => array('handler' => 'views_handler_argument_string'),
    'title'     => t('Author first name'),
    'help'      => t('Author first name')
  );
  $data['name'] =  array(
    'field'     => array('handler' => 'views_handler_field'),
    'sort'      => array('handler' => 'views_handler_sort'),
    'filter'    => array('handler' => 'views_handler_filter'),
    'argument'  => array('handler' => 'views_handler_argument_string'),
    'title'     => t('Author full name'),
    'help'      => t('Author full name')
  );
  $data['prefix'] =  array(
    'field'     => array('handler' => 'views_handler_field'),
    'sort'      => array('handler' => 'views_handler_sort'),
    'filter'    => array('handler' => 'views_handler_filter'),
    'argument'  => array('handler' => 'views_handler_argument_string'),
    'title'     => t('Author preix'),
    'help'      => t('Attributes which typically proceed the authors first name')
  );
  $data['suffix'] =  array(
    'field'     => array('handler' => 'views_handler_field'),
    'sort'      => array('handler' => 'views_handler_sort'),
    'filter'    => array('handler' => 'views_handler_filter'),
    'argument'  => array('handler' => 'views_handler_argument_string'),
    'title'     => t('Author suffix'),
    'help'      => t('Attributes which typically follow the authors last name.')
  );
  $data['affiliation'] =  array(
    'field'     => array('handler' => 'views_handler_field'),
    'sort'      => array('handler' => 'views_handler_sort'),
    'filter'    => array('handler' => 'views_handler_filter'),
    'argument'  => array('handler' => 'views_handler_argument_string'),
    'title'     => t('Author affiliation'),
    'help'      => t('Organization the author is affiliated with')
  );

  $viewsdata['biblio_contributor_data'] = $data;



/***************** Describe the keyword table *************/

  $data = array();
  $data['table']['group'] = t('Biblio');
  $data['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'node_revision' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'biblio_keyword_data' => array(
      'field' => 'kid',
      'left_field' => 'kid',
    ),
  );
  $data['kid'] = array(
    'title' => t('Keyword ID'),
    'help' => t('The Biblio keyword ID'),
    'field' => array(
      'title' => t('All keywords'),
      'help' => t('Display all keywords associated with a node.'),
      'handler' => 'biblio_handler_field_biblio_keyword_kid',
    ),
    'argument' => array(
      'handler' => 'biblio_handler_argument_many_to_one',
      'name table' => 'biblio_keyword_data',
      'name field' => 'word',
      'empty name field' => t('No Keyword'),
      'numeric' => TRUE,
    ),
    'filter' => array(
      'title' => t('Keyword ID'),
      'handler' => 'biblio_handler_filter_biblio_keyword_kid',
      'numeric' => TRUE,
    ),
    );

  $viewsdata['biblio_keyword'] = $data;

  $viewsdata['biblio_keyword']['nid'] = array(
    'title' => t('Node'),
    'help' => t('Get all nodes tagged with a keyword.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'node',
      'base field' => 'nid',
      'label' => t('node'),
    ),
  );


/***************** Describe the keyword_data table ***********/

  $data = array();
  $data['table']['group'] = t('Biblio');
  $data['table']['base'] = array(
    'field' => 'kid',
    'title' => t('Biblio Keywords'),
  );

  $data['table']['join'] = array(
    'biblio_keyword' => array(
      'left_field' => 'kid',
      'field' => 'kid',
    ),
    'node' => array(
      'left_table' => 'biblio_keyword',
      'left_field' => 'kid',
      'field' => 'kid',
    ),
    'biblio' => array(
      'left_table' => 'biblio_keyword',
      'left_field' => 'kid',
      'field' => 'kid',
    ),
    );
  $data['word'] =  array(
    'field'     => array('handler' => 'biblio_handler_field_biblio_keyword_data_word'),
    'filter'    => array('handler' => 'views_handler_filter_string'),
    'argument'  => array('handler' => 'views_handler_argument_string'),
    'sort'      => array('handler' => 'views_handler_sort'),
    'title'     => t('Keyword'),
  //  'skip base' => array('node', 'node_revision'),
  );
  $data['kid'] =  array(
    'field'     => array('handler' => 'views_handler_field_numeric'),
 //   'filter'    => array('handler' => 'views_handler_filter'),
 //   'argument'  => array('handler' => 'views_handler_argument'),
    'title'     => t('Keyword ID'),
    'skip base' => array('node', 'node_revision'),
  );


  $viewsdata['biblio_keyword_data'] = $data;

  $export_links = module_invoke_all('biblio_export_options');
  foreach ($export_links as $type => $name) {
    $viewsdata['biblio'][$type.'_export'] = array(
        'title' => t('Export link - ' . $name),
        'help'  => t("Provides a link to export the data in ") . $name . t(" format"),
        'field' => array(
          'handler' => 'biblio_handler_field_export_link',
          'group' => t('Biblio'),
          'format' => $type,
          'format name' => $name,
       ),
    );

  }

  return $viewsdata;
}

function template_preprocess_views_view_unformatted__biblio_year(&$vars) {
  $view     = $vars['view'];
  $rows     = $vars['rows'];

  $vars['classes'] = array();
  // Set up striping values.
  foreach ($rows as $id => $row) {
    $vars['classes'][$id] = 'views-row';
    $vars['classes'][$id] .= ' views-row-' . ($id + 1);
    $vars['classes'][$id] .= ' views-row-' . ($id % 2 ? 'even' : 'odd');
    if ($id == 0) {
      $vars['classes'][$id] .= ' views-row-first';
    }
  }
  $vars['classes'][$id] .= ' views-row-last';
}
