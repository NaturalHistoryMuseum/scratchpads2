<?php
/**
 * @file
 * agridrupal_views_rdf_oai.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function agridrupal_views_rdf_oai_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: menu-databases_advanced-search:ag_experts
  $menu_links['menu-databases_advanced-search:ag_experts'] = array(
    'menu_name' => 'menu-databases',
    'link_path' => 'ag_experts',
    'router_path' => 'ag_experts',
    'link_title' => 'advanced search',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-databases_advanced-search:ag_experts',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-databases_experts:ag_experts-directory',
  );
  // Exported menu link: menu-databases_advanced-search:ag_institutions
  $menu_links['menu-databases_advanced-search:ag_institutions'] = array(
    'menu_name' => 'menu-databases',
    'link_path' => 'ag_institutions',
    'router_path' => 'ag_institutions',
    'link_title' => 'advanced search',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-databases_advanced-search:ag_institutions',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-databases_institutions:ag_institutions-directory',
  );
  // Exported menu link: menu-databases_experts:ag_experts-directory
  $menu_links['menu-databases_experts:ag_experts-directory'] = array(
    'menu_name' => 'menu-databases',
    'link_path' => 'ag_experts-directory',
    'router_path' => 'ag_experts-directory',
    'link_title' => 'Experts',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-databases_experts:ag_experts-directory',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -49,
    'customized' => 1,
  );
  // Exported menu link: menu-databases_institutions:ag_institutions-directory
  $menu_links['menu-databases_institutions:ag_institutions-directory'] = array(
    'menu_name' => 'menu-databases',
    'link_path' => 'ag_institutions-directory',
    'router_path' => 'ag_institutions-directory',
    'link_title' => 'Institutions',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-databases_institutions:ag_institutions-directory',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -50,
    'customized' => 1,
  );
  // Exported menu link: menu-documenttype_advanced-search:ag_documents
  $menu_links['menu-documenttype_advanced-search:ag_documents'] = array(
    'menu_name' => 'menu-documenttype',
    'link_path' => 'ag_documents',
    'router_path' => 'ag_documents',
    'link_title' => 'advanced search',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-documenttype_advanced-search:ag_documents',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-documenttype_catalog:ag_documents-catalog',
  );
  // Exported menu link: menu-documenttype_by-author-corporate:ag_bycorporatebody
  $menu_links['menu-documenttype_by-author-corporate:ag_bycorporatebody'] = array(
    'menu_name' => 'menu-documenttype',
    'link_path' => 'ag_bycorporatebody',
    'router_path' => 'ag_bycorporatebody',
    'link_title' => '...by author (corporate)',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-documenttype_by-author-corporate:ag_bycorporatebody',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
    'parent_identifier' => 'menu-documenttype_browse-catalog:node/30743',
  );
  // Exported menu link: menu-documenttype_by-author-personal:ag_byauthor
  $menu_links['menu-documenttype_by-author-personal:ag_byauthor'] = array(
    'menu_name' => 'menu-documenttype',
    'link_path' => 'ag_byauthor',
    'router_path' => 'ag_byauthor',
    'link_title' => '...by author (personal)',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-documenttype_by-author-personal:ag_byauthor',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'parent_identifier' => 'menu-documenttype_browse-catalog:node/30743',
  );
  // Exported menu link: menu-documenttype_by-conference:ag_byconference
  $menu_links['menu-documenttype_by-conference:ag_byconference'] = array(
    'menu_name' => 'menu-documenttype',
    'link_path' => 'ag_byconference',
    'router_path' => 'ag_byconference',
    'link_title' => '...by conference',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-documenttype_by-conference:ag_byconference',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -45,
    'customized' => 1,
    'parent_identifier' => 'menu-documenttype_browse-catalog:node/30743',
  );
  // Exported menu link: menu-documenttype_by-journal:ag_byjournal
  $menu_links['menu-documenttype_by-journal:ag_byjournal'] = array(
    'menu_name' => 'menu-documenttype',
    'link_path' => 'ag_byjournal',
    'router_path' => 'ag_byjournal',
    'link_title' => '...by journal',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-documenttype_by-journal:ag_byjournal',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 1,
    'parent_identifier' => 'menu-documenttype_browse-catalog:node/30743',
  );
  // Exported menu link: menu-documenttype_by-subject:ag_bysubject
  $menu_links['menu-documenttype_by-subject:ag_bysubject'] = array(
    'menu_name' => 'menu-documenttype',
    'link_path' => 'ag_bysubject',
    'router_path' => 'ag_bysubject',
    'link_title' => '...by subject',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-documenttype_by-subject:ag_bysubject',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
    'parent_identifier' => 'menu-documenttype_browse-catalog:node/30743',
  );
  // Exported menu link: menu-documenttype_by-type:ag_bydoctype
  $menu_links['menu-documenttype_by-type:ag_bydoctype'] = array(
    'menu_name' => 'menu-documenttype',
    'link_path' => 'ag_bydoctype',
    'router_path' => 'ag_bydoctype',
    'link_title' => '...by type',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-documenttype_by-type:ag_bydoctype',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
    'parent_identifier' => 'menu-documenttype_browse-catalog:node/30743',
  );
  // Exported menu link: menu-documenttype_catalog:ag_documents-catalog
  $menu_links['menu-documenttype_catalog:ag_documents-catalog'] = array(
    'menu_name' => 'menu-documenttype',
    'link_path' => 'ag_documents-catalog',
    'router_path' => 'ag_documents-catalog',
    'link_title' => 'Catalog',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-documenttype_catalog:ag_documents-catalog',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 1,
    'weight' => -50,
    'customized' => 1,
  );
  // Exported menu link: menu-reference_conferences:ag_conferences
  $menu_links['menu-reference_conferences:ag_conferences'] = array(
    'menu_name' => 'menu-reference',
    'link_path' => 'ag_conferences',
    'router_path' => 'ag_conferences',
    'link_title' => 'Conferences',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-reference_conferences:ag_conferences',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
  );
  // Exported menu link: menu-reference_corporate-authors:ag_corporate-bodies
  $menu_links['menu-reference_corporate-authors:ag_corporate-bodies'] = array(
    'menu_name' => 'menu-reference',
    'link_path' => 'ag_corporate-bodies',
    'router_path' => 'ag_corporate-bodies',
    'link_title' => 'Corporate authors',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-reference_corporate-authors:ag_corporate-bodies',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
  );
  // Exported menu link: menu-reference_journals:ag_journals
  $menu_links['menu-reference_journals:ag_journals'] = array(
    'menu_name' => 'menu-reference',
    'link_path' => 'ag_journals',
    'router_path' => 'ag_journals',
    'link_title' => 'Journals',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-reference_journals:ag_journals',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
  );
  // Exported menu link: menu-reference_personal-authors:ag_authors
  $menu_links['menu-reference_personal-authors:ag_authors'] = array(
    'menu_name' => 'menu-reference',
    'link_path' => 'ag_authors',
    'router_path' => 'ag_authors',
    'link_title' => 'Personal authors',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-reference_personal-authors:ag_authors',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
  );
  // Exported menu link: menu-reference_subject-vocabularies:ag_vocabularies
  $menu_links['menu-reference_subject-vocabularies:ag_vocabularies'] = array(
    'menu_name' => 'menu-reference',
    'link_path' => 'ag_vocabularies',
    'router_path' => 'ag_vocabularies',
    'link_title' => 'Subject vocabularies',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
      'identifier' => 'menu-reference_subject-vocabularies:ag_vocabularies',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 1,
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('...by author (corporate)');
  t('...by author (personal)');
  t('...by conference');
  t('...by journal');
  t('...by subject');
  t('...by type');
  t('Catalog');
  t('Conferences');
  t('Corporate authors');
  t('Experts');
  t('Institutions');
  t('Journals');
  t('Personal authors');
  t('Subject vocabularies');
  t('advanced search');


  return $menu_links;
}
