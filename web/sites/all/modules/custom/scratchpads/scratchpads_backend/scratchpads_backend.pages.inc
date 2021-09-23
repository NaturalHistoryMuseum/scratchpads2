<?php

/**
 * Page callback for admin/config/content/comments
 */
function scratchpads_backend_comments_page(){
  return array(
    'intro' => array(
      '#markup' => '<p>' . t('This form lets you set the comment settings for <strong>new nodes</strong> of each content type') . '</p>'
    ),
    'form' => drupal_get_form('scratchpads_backend_comments_form')
  );
}

/**
 * Form submit function
 */
function scratchpads_backend_comments_form_submit($form, &$form_state){
  foreach($form_state['values']['comment_settings'] as $setting){
    variable_set('comment_' . $setting['content_type'], $setting['comment_setting']);
  }
  drupal_set_message(t('Your settings have been saved'), 'status');
}

/**
 * Define the form
 */
function scratchpads_backend_comments_form($form, &$form_state){
  $form['comment_settings'] = array(
    '#tree' => TRUE,
    '#theme' => 'table',
    '#header' => array(
      t('Content type'),
      t('Description'),
      t('Comment settings'),
      t('Advanced settings')
    ),
    '#rows' => array()
  );
  $types = node_type_get_types();
  foreach($types as $type){
    $select = array(
      '#type' => 'select',
      '#title' => '',
      '#default_value' => variable_get('comment_' . $type->type, COMMENT_NODE_OPEN),
      '#options' => array(
        COMMENT_NODE_OPEN => t('Open'),
        COMMENT_NODE_CLOSED => t('Closed'),
        COMMENT_NODE_HIDDEN => t('Hidden')
      )
    );
    $form['comment_settings'][] = array(
      'comment_setting' => &$select,
      'content_type' => array(
        '#type' => 'hidden',
        '#value' => $type->type
      )
    );
    $form['comment_settings']['#rows'][] = array(
      array(
        'data' => array(
          '#markup' => $type->name
        )
      ),
      array(
        'data' => array(
          '#markup' => $type->description
        )
      ),
      array(
        'data' => &$select
      ),
      array(
        'data' => array(
          '#markup' => l('edit', 'admin/structure/types/manage/' . urlencode($type->type))
        )
      )
    );
    unset($select);
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array(
      'scratchpads_backend_comments_form_submit'
    )
  );
  return $form;
}