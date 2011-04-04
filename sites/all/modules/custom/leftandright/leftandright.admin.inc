<?php

/**
 * Display form for adding and editing vocabularies.
 */
function leftandright_rebuild_tree_form($form, &$form_state, $edit = array()){
  return array(
    'information' => array(
      '#markup' => t('Information here, including info about whether or not the tree actually needs rebuilding.')
    ),
    'actions' => array(
      '#type' => 'actions',
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Rebuild')
      )
    )
  );
}