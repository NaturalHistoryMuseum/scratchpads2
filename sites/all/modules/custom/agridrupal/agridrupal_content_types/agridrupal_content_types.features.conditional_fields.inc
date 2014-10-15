<?php
/**
 * @file
 * agridrupal_content_types.features.conditional_fields.inc
 */

/**
 * Implements hook_conditional_fields_default_fields().
 */
function agridrupal_content_types_conditional_fields_default_fields() {

$term=taxonomy_get_term_by_name('Book', $vocabulary = NULL);
foreach($term as $t)
{
  $Book=$t->tid;
}


$term=taxonomy_get_term_by_name('Book Chapter', $vocabulary = NULL);
foreach($term as $t)
{
  $BookChapter=$t->tid;
}
$term=taxonomy_get_term_by_name('Conference Proceedings', $vocabulary = NULL);
foreach($term as $t)
{
  $ConferenceProceedings=$t->tid;
}
$term=taxonomy_get_term_by_name('Conference Paper', $vocabulary = NULL);
foreach($term as $t)
{
   $ConferencePaper=$t->tid;
}
$term=taxonomy_get_term_by_name('Conference Poster', $vocabulary = NULL);
foreach($term as $t)
{
   $ConferencePoster=$t->tid;
}
$term=taxonomy_get_term_by_name('Conference Presentation', $vocabulary = NULL);
foreach($term as $t)
{
   $ConferencePresentation=$t->tid;
}

$term=taxonomy_get_term_by_name('Journal Article', $vocabulary = NULL);
foreach($term as $t)
{
   $JournalArticle=$t->tid;
}
$term=taxonomy_get_term_by_name('Journal Issue', $vocabulary = NULL);
foreach($term as $t)
{
   $JournalIssue=$t->tid;
}

$term=taxonomy_get_term_by_name('Other', $vocabulary = NULL);
foreach($term as $t)
{
   $Other=$t->tid;
}

$term=taxonomy_get_term_by_name('Brochure', $vocabulary = NULL);
foreach($term as $t)
{
   $Brochure=$t->tid;
}
$term=taxonomy_get_term_by_name('Manuscript', $vocabulary = NULL);
foreach($term as $t)
{
   $Manuscript=$t->tid;
}
$term=taxonomy_get_term_by_name('Presentation', $vocabulary = NULL);
foreach($term as $t)
{
   $Presentation=$t->tid;
}
$term=taxonomy_get_term_by_name('Technical report', $vocabulary = NULL);
foreach($term as $t)
{
   $Technicalreport=$t->tid;
}
$term=taxonomy_get_term_by_name('Training Material', $vocabulary = NULL);
foreach($term as $t)
{
   $TrainingMaterial=$t->tid;
}
	
  $items = array();

  $items[] = array(
    'entity' => 'node',
    'bundle' => 'agdlios',
    'dependent' => 'field_ag_aproxdatepub',
    'dependee' => 'field_ag_chkdatepub',
    'options' => array(
      'state' => 'visible',
      'condition' => 'value',
      'effect' => 'show',
      'effect_options' => array(),
      'element_view' => array(
        1 => 1,
        2 => 2,
        5 => 0,
        3 => 0,
        4 => 0,
      ),
      'element_view_per_role' => 0,
      'element_view_roles' => array(
        3 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        1 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        2 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        6 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        7 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        4 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        5 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
      ),
      'element_edit' => array(
        1 => 1,
        3 => 0,
        2 => 0,
      ),
      'element_edit_per_role' => 0,
      'element_edit_roles' => array(
        3 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        1 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        2 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        6 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        7 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        4 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        5 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
      ),
      'selector' => '',
      'values_set' => 1,
      'value_form' => 0,
      'value' => array(
        0 => array(
          'value' => 0,
        ),
      ),
      'grouping' => 'AND',
      'values' => array(),
    ),
  );

  $items[] = array(
    'entity' => 'node',
    'bundle' => 'agdlios',
    'dependent' => 'field_ag_book',
    'dependee' => 'field_ag_type',
    'options' => array(
      'state' => 'visible',
      'condition' => 'value',
      'effect' => 'show',
      'effect_options' => array(),
      'element_view' => array(
        1 => 1,
        2 => 2,
        5 => 0,
        3 => 0,
        4 => 0,
      ),
      'element_view_per_role' => 0,
      'element_view_roles' => array(
        3 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        1 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        2 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        6 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        7 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        4 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        5 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
      ),
      'element_edit' => array(
        1 => 1,
        3 => 0,
        2 => 0,
      ),
      'element_edit_per_role' => 0,
      'element_edit_roles' => array(
        3 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        1 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        2 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        6 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        7 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        4 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        5 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
      ),
      'selector' => '',
      'values_set' => 1,
      'value_form' => 2,
      'value' => array(
        0 => array(
          'tid' => $BookChapter,
        ),
      ),
      'grouping' => 'AND',
      'values' => array(),
    ),
  );
//added book chapter
$items[] = array(
    'entity' => 'node',
    'bundle' => 'agdlios',
    'dependent' => 'field_ag_booknumber',
    'dependee' => 'field_ag_type',
    'options' => array(
      'state' => 'visible',
      'condition' => 'value',
      'effect' => 'show',
      'effect_options' => array(),
      'element_view' => array(
        1 => 1,
        2 => 2,
        5 => 0,
        3 => 0,
        4 => 0,
      ),
      'element_view_per_role' => 0,
      'element_view_roles' => array(
        3 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        1 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        2 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        6 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        7 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        4 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        5 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
      ),
      'element_edit' => array(
        1 => 1,
        3 => 0,
        2 => 0,
      ),
      'element_edit_per_role' => 0,
      'element_edit_roles' => array(
        3 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        1 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        2 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        6 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        7 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        4 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        5 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
      ),
      'selector' => '',
      'values_set' => 1,
      'value_form' => 2,
      'value' => array(
        0 => array(
          'tid' => $BookChapter,
        ),
      ),
      'grouping' => 'AND',
      'values' => array(),
    ),
  );

//
  $items[] = array(
    'entity' => 'node',
    'bundle' => 'agdlios',
    'dependent' => 'field_ag_confname',
    'dependee' => 'field_ag_type',
    'options' => array(
      'state' => 'visible',
      'condition' => 'value',
      'effect' => 'show',
      'effect_options' => array(),
      'element_view' => array(
        1 => 1,
        2 => 2,
        5 => 0,
        3 => 0,
        4 => 0,
      ),
      'element_view_per_role' => 0,
      'element_view_roles' => array(
        3 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        1 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        2 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        6 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        7 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        4 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        5 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
      ),
      'element_edit' => array(
        1 => 1,
        3 => 0,
        2 => 0,
      ),
      'element_edit_per_role' => 0,
      'element_edit_roles' => array(
        3 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        1 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        2 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        6 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        7 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        4 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        5 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
      ),
      'selector' => '',
      'values_set' => 3,
      'value_form' => '_none',
      'values' => array(
        0 => $ConferencePoster,
        1 => $ConferencePresentation,
        2 => $ConferenceProceedings,
        3 => $ConferencePaper,
        4 => $Other,
      ),
      'grouping' => 'AND',
      'value' => array(),
    ),
  );

  $items[] = array(
    'entity' => 'node',
    'bundle' => 'agdlios',
    'dependent' => 'field_ag_edition',
    'dependee' => 'field_ag_type',
    'options' => array(
      'state' => 'visible',
      'condition' => 'value',
      'effect' => 'show',
      'effect_options' => array(),
      'element_view' => array(
        1 => 1,
        2 => 2,
        5 => 0,
        3 => 0,
        4 => 0,
      ),
      'element_view_per_role' => 0,
      'element_view_roles' => array(
        3 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        1 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        2 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        6 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        7 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        4 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        5 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
      ),
      'element_edit' => array(
        1 => 1,
        3 => 0,
        2 => 0,
      ),
      'element_edit_per_role' => 0,
      'element_edit_roles' => array(
        3 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        1 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        2 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        6 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        7 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        4 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        5 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
      ),
      'selector' => '',
      'values_set' => 3,
      'value_form' => 1,
      'values' => array(
        0 => $Book,
        1 => $Other,
      ),
      'grouping' => 'AND',
      'value' => array(),
    ),
  );

  $items[] = array(
    'entity' => 'node',
    'bundle' => 'agdlios',
    'dependent' => 'field_ag_eissn',
    'dependee' => 'field_ag_type',
    'options' => array(
      'state' => 'visible',
      'condition' => 'value',
      'effect' => 'show',
      'effect_options' => array(),
      'element_view' => array(
        1 => 1,
        2 => 2,
        5 => 0,
        3 => 0,
        4 => 0,
      ),
      'element_view_per_role' => 0,
      'element_view_roles' => array(
        3 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        1 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        2 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        6 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        7 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        4 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        5 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
      ),
      'element_edit' => array(
        1 => 1,
        3 => 3,
        2 => 2,
      ),
      'element_edit_per_role' => 0,
      'element_edit_roles' => array(
        3 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        1 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        2 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        6 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        7 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        4 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        5 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
      ),
      'selector' => '',
      'values_set' => 3,
      'value_form' => '_none',
      'values' => array(
        0 => $Other,
      ),
      'grouping' => 'AND',
      'value' => array(),
    ),
  );

  $items[] = array(
    'entity' => 'node',
    'bundle' => 'agdlios',
    'dependent' => 'field_ag_isbn',
    'dependee' => 'field_ag_type',
    'options' => array(
      'state' => 'visible',
      'condition' => 'value',
      'effect' => 'show',
      'effect_options' => array(),
      'element_view' => array(
        1 => 1,
        2 => 2,
        5 => 0,
        3 => 0,
        4 => 0,
      ),
      'element_view_per_role' => 0,
      'element_view_roles' => array(
        3 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        1 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        2 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        6 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        7 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        4 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        5 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
      ),
      'element_edit' => array(
        1 => 1,
        3 => 0,
        2 => 0,
      ),
      'element_edit_per_role' => 0,
      'element_edit_roles' => array(
        3 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        1 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        2 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        6 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        7 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        4 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        5 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
      ),
      'selector' => '',
      'values_set' => 3,
      'value_form' => '_none',
      'values' => array(
        0 => $Book,
        1 => $ConferenceProceedings,
        2 => $Brochure,
        3 => $Manuscript,
        4 => $Other,
        5 => $Presentation,
        6 => $Technicalreport,
        7 => $TrainingMaterial,
      ),
      'grouping' => 'AND',
      'value' => array(),
    ),
  );

  $items[] = array(
    'entity' => 'node',
    'bundle' => 'agdlios',
    'dependent' => 'field_ag_issn',
    'dependee' => 'field_ag_type',
    'options' => array(
      'state' => 'visible',
      'condition' => 'value',
      'grouping' => 'AND',
      'effect' => 'show',
      'effect_options' => array(),
      'element_view' => array(
        1 => 1,
        2 => 2,
        5 => 0,
        3 => 0,
        4 => 0,
      ),
      'element_view_per_role' => 0,
      'element_view_roles' => array(
        3 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        1 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        2 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        6 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        7 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        4 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        5 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
      ),
      'element_edit' => array(
        1 => 1,
        3 => 0,
      ),
      'element_edit_per_role' => 0,
      'element_edit_roles' => array(
        3 => array(
          1 => 1,
          3 => 0,
        ),
        1 => array(
          1 => 1,
          3 => 0,
        ),
        2 => array(
          1 => 1,
          3 => 0,
        ),
        6 => array(
          1 => 1,
          3 => 0,
        ),
        7 => array(
          1 => 1,
          3 => 0,
        ),
        4 => array(
          1 => 1,
          3 => 0,
        ),
        5 => array(
          1 => 1,
          3 => 0,
        ),
      ),
      'selector' => '',
      'values_set' => 1,
      'value_form' => 14,
      'value' => array(
        0 => array(
          'tid' => $Other,
        ),
      ),
      'values' => array(),
    ),
  );

  $items[] = array(
    'entity' => 'node',
    'bundle' => 'agdlios',
    'dependent' => 'field_ag_journal',
    'dependee' => 'field_ag_link_to_jissue',
    'options' => array(
      'state' => 'visible',
      'condition' => 'value',
      'grouping' => 'AND',
      'effect' => 'show',
      'effect_options' => array(),
      'element_view' => array(
        1 => 1,
        2 => 2,
        5 => 0,
        3 => 0,
        4 => 0,
      ),
      'element_view_per_role' => 0,
      'element_view_roles' => array(
        3 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        1 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        2 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        6 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        7 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        4 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        5 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
      ),
      'element_edit' => array(
        1 => 1,
        3 => 0,
      ),
      'element_edit_per_role' => 0,
      'element_edit_roles' => array(
        3 => array(
          1 => 1,
          3 => 0,
        ),
        1 => array(
          1 => 1,
          3 => 0,
        ),
        2 => array(
          1 => 1,
          3 => 0,
        ),
        6 => array(
          1 => 1,
          3 => 0,
        ),
        7 => array(
          1 => 1,
          3 => 0,
        ),
        4 => array(
          1 => 1,
          3 => 0,
        ),
        5 => array(
          1 => 1,
          3 => 0,
        ),
      ),
      'selector' => '',
      'values_set' => 1,
      'value_form' => 0,
      'value' => array(
        0 => array(
          'value' => 0,
        ),
      ),
      'values' => array(),
    ),
  );

  $items[] = array(
    'entity' => 'node',
    'bundle' => 'agdlios',
    'dependent' => 'field_ag_journal_issue',
    'dependee' => 'field_ag_link_to_jissue',
    'options' => array(
      'state' => 'visible',
      'condition' => 'value',
      'effect' => 'show',
      'effect_options' => array(),
      'element_view' => array(
        1 => 1,
        2 => 2,
        5 => 0,
        3 => 0,
        4 => 0,
      ),
      'element_view_per_role' => 0,
      'element_view_roles' => array(
        3 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        1 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        2 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        6 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        7 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        4 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        5 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
      ),
      'element_edit' => array(
        1 => 1,
        3 => 0,
        2 => 0,
      ),
      'element_edit_per_role' => 0,
      'element_edit_roles' => array(
        3 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        1 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        2 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        6 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        7 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        4 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        5 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
      ),
      'selector' => '',
      'values_set' => 3,
      'value_form' => NULL,
      'values' => array(
        0 => 1,
      ),
      'grouping' => 'AND',
      'value' => array(),
    ),
  );

  $items[] = array(
    'entity' => 'node',
    'bundle' => 'agdlios',
    'dependent' => 'field_ag_link_to_jissue',
    'dependee' => 'field_ag_type',
    'options' => array(
      'state' => 'visible',
      'condition' => 'value',
      'effect' => 'show',
      'effect_options' => array(),
      'element_view' => array(
        1 => 1,
        2 => 2,
        5 => 0,
        3 => 0,
        4 => 0,
      ),
      'element_view_per_role' => 0,
      'element_view_roles' => array(
        3 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        1 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        2 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        6 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        7 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        4 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        5 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
      ),
      'element_edit' => array(
        1 => 1,
        3 => 0,
        2 => 0,
      ),
      'element_edit_per_role' => 0,
      'element_edit_roles' => array(
        3 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        1 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        2 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        6 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        7 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        4 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        5 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
      ),
      'selector' => '',
      'values_set' => 3,
      'value_form' => '_none',
      'values' => array(
        0 => $JournalArticle,
        1 => $JournalIssue,
        2 => $Other,
      ),
      'grouping' => 'AND',
      'value' => array(),
    ),
  );

  $items[] = array(
    'entity' => 'node',
    'bundle' => 'agdlios',
    'dependent' => 'field_ag_number',
    'dependee' => 'field_ag_link_to_jissue',
    'options' => array(
      'state' => 'visible',
      'condition' => 'value',
      'grouping' => 'AND',
      'effect' => 'show',
      'effect_options' => array(),
      'element_view' => array(
        1 => 1,
        2 => 2,
        5 => 0,
        3 => 0,
        4 => 0,
      ),
      'element_view_per_role' => 0,
      'element_view_roles' => array(
        3 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        1 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        2 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        6 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        7 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        4 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        5 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
      ),
      'element_edit' => array(
        1 => 1,
        3 => 0,
      ),
      'element_edit_per_role' => 0,
      'element_edit_roles' => array(
        3 => array(
          1 => 1,
          3 => 0,
        ),
        1 => array(
          1 => 1,
          3 => 0,
        ),
        2 => array(
          1 => 1,
          3 => 0,
        ),
        6 => array(
          1 => 1,
          3 => 0,
        ),
        7 => array(
          1 => 1,
          3 => 0,
        ),
        4 => array(
          1 => 1,
          3 => 0,
        ),
        5 => array(
          1 => 1,
          3 => 0,
        ),
      ),
      'selector' => '',
      'values_set' => 1,
      'value_form' => 0,
      'value' => array(
        0 => array(
          'value' => 0,
        ),
      ),
      'values' => array(),
    ),
  );

  $items[] = array(
    'entity' => 'node',
    'bundle' => 'agdlios',
    'dependent' => 'field_ag_publisher',
    'dependee' => 'field_ag_type',
    'options' => array(
      'state' => 'visible',
      'condition' => 'value',
      'grouping' => 'AND',
      'effect' => 'show',
      'effect_options' => array(),
      'element_view' => array(
        1 => 1,
        2 => 2,
        5 => 0,
        3 => 0,
        4 => 0,
      ),
      'element_view_per_role' => 0,
      'element_view_roles' => array(
        3 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        1 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        2 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        6 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        7 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        4 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        5 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
      ),
      'element_edit' => array(
        1 => 1,
        3 => 0,
      ),
      'element_edit_per_role' => 0,
      'element_edit_roles' => array(
        3 => array(
          1 => 1,
          3 => 0,
        ),
        1 => array(
          1 => 1,
          3 => 0,
        ),
        2 => array(
          1 => 1,
          3 => 0,
        ),
        6 => array(
          1 => 1,
          3 => 0,
        ),
        7 => array(
          1 => 1,
          3 => 0,
        ),
        4 => array(
          1 => 1,
          3 => 0,
        ),
        5 => array(
          1 => 1,
          3 => 0,
        ),
      ),
      'selector' => '',
      'values_set' => 3,
      'value_form' => '_none',
      'values' => array(
        0 => $Book,
        1 => $Other ,
      ),
      'value' => array(),
    ),
  );

  $items[] = array(
    'entity' => 'node',
    'bundle' => 'agdlios',
    'dependent' => 'field_ag_publishplace',
    'dependee' => 'field_ag_type',
    'options' => array(
      'state' => 'visible',
      'condition' => 'value',
      'grouping' => 'AND',
      'effect' => 'show',
      'effect_options' => array(),
      'element_view' => array(
        1 => 1,
        2 => 2,
        5 => 0,
        3 => 0,
        4 => 0,
      ),
      'element_view_per_role' => 0,
      'element_view_roles' => array(
        3 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        1 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        2 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        6 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        7 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        4 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        5 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
      ),
      'element_edit' => array(
        1 => 1,
        3 => 0,
      ),
      'element_edit_per_role' => 0,
      'element_edit_roles' => array(
        3 => array(
          1 => 1,
          3 => 0,
        ),
        1 => array(
          1 => 1,
          3 => 0,
        ),
        2 => array(
          1 => 1,
          3 => 0,
        ),
        6 => array(
          1 => 1,
          3 => 0,
        ),
        7 => array(
          1 => 1,
          3 => 0,
        ),
        4 => array(
          1 => 1,
          3 => 0,
        ),
        5 => array(
          1 => 1,
          3 => 0,
        ),
      ),
      'selector' => '',
      'values_set' => 3,
      'value_form' => '_none',
      'values' => array(
        0 => $Book,
        1 => $Other ,
      ),
      'value' => array(),
    ),
  );

  $items[] = array(
    'entity' => 'node',
    'bundle' => 'agdlios',
    'dependent' => 'field_ag_volume',
    'dependee' => 'field_ag_link_to_jissue',
    'options' => array(
      'state' => 'visible',
      'condition' => 'value',
      'grouping' => 'AND',
      'effect' => 'show',
      'effect_options' => array(),
      'element_view' => array(
        1 => 1,
        2 => 2,
        5 => 0,
        3 => 0,
        4 => 0,
      ),
      'element_view_per_role' => 0,
      'element_view_roles' => array(
        3 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        1 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        2 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        6 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        7 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        4 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        5 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
      ),
      'element_edit' => array(
        1 => 1,
        3 => 0,
      ),
      'element_edit_per_role' => 0,
      'element_edit_roles' => array(
        3 => array(
          1 => 1,
          3 => 0,
        ),
        1 => array(
          1 => 1,
          3 => 0,
        ),
        2 => array(
          1 => 1,
          3 => 0,
        ),
        6 => array(
          1 => 1,
          3 => 0,
        ),
        7 => array(
          1 => 1,
          3 => 0,
        ),
        4 => array(
          1 => 1,
          3 => 0,
        ),
        5 => array(
          1 => 1,
          3 => 0,
        ),
      ),
      'selector' => '',
      'values_set' => 1,
      'value_form' => 0,
      'value' => array(
        0 => array(
          'value' => 0,
        ),
      ),
      'values' => array(),
    ),
  );

  $items[] = array(
    'entity' => 'node',
    'bundle' => 'agdlios',
    'dependent' => 'field_pages',
    'dependee' => 'field_ag_type',
    'options' => array(
      'state' => 'visible',
      'condition' => 'value',
      'effect' => 'show',
      'effect_options' => array(),
      'element_view' => array(
        1 => 1,
        2 => 2,
        5 => 0,
        3 => 0,
        4 => 0,
      ),
      'element_view_per_role' => 0,
      'element_view_roles' => array(
        3 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        1 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        2 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        6 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        7 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        4 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
        5 => array(
          1 => 1,
          2 => 2,
          5 => 0,
          3 => 0,
          4 => 0,
        ),
      ),
      'element_edit' => array(
        1 => 1,
        3 => 0,
        2 => 0,
      ),
      'element_edit_per_role' => 0,
      'element_edit_roles' => array(
        3 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        1 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        2 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        6 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        7 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        4 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
        5 => array(
          1 => 1,
          3 => 0,
          2 => 0,
        ),
      ),
      'selector' => '',
      'values_set' => 3,
      'value_form' => '_none',
      'values' => array(
        0 => $JournalArticle,
        1 => $BookChapter,
        2 => $Other,
      ),
      'grouping' => 'AND',
      'value' => array(),
    ),
  );

  return $items;
}
