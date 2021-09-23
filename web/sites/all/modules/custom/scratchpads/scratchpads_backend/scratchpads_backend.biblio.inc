<?php

/**
 * scrachpads_backend_biblio_settings_form
 */
function scratchpads_backend_biblio_settings_form($form, &$form_state){
  $options = biblio_get_styles();
  if(isset($form_state['values']['default_style'])){
    $default = $form_state['values']['default_style'];
  }else if(module_exists('biblio_citeproc')){
    $default = variable_get('biblio_citeproc_style', 'ieee.csl');
  }else{
    $default = variable_get('biblio_style', 'cse');
  }
  // Grab the most recent biblio entry to use an example when switching styles
  $efq = new EntityFieldQuery();
  $efq->entityCondition('entity_type', 'node');
  $efq->entityCondition('bundle', 'biblio');
  $efq->propertyOrderBy('created', 'DESC');
  $efq->range(0, 1);
  $efq_results = $efq->execute();
  $example = NULL;
  if(count($efq_results['node'])){
    $example = entity_load_single('node', reset(array_keys($efq_results['node'])));
  }
  // Create the form item for the main style
  $form = array();
  $form['default'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE
  );
  $form['default']['default_style'] = array(
    '#type' => 'select',
    '#title' => t('Citation style'),
    '#description' => t('Style to use when listing citations'),
    '#options' => $options,
    '#default_value' => $default
  );
  if($example){
    $form['default']['default_style']['#ajax'] = array(
      'callback' => 'scratchpads_backend_biblio_default_ajax',
      'wrapper' => 'scratchpads_backend_biblio_default_example',
      'effect' => 'fade',
      'speed' => '200'
    );
    $render = scratchpads_backend_biblio_render_style($example, $default);
    $form['default']['default_example'] = array(
      '#type' => 'markup',
      '#prefix' => '<div id="scratchpads_backend_biblio_default_example" class="form-item">',
      '#suffix' => '</div>',
      '#markup' => '<label>' . t('Example') . '</label> ' . $render
    );
  }
  // Create the form item for the DwCA style
  if(module_exists('dwca_export')){
    if(isset($form_state['values']['dwca_style'])){
      $dwca_default = $form_state['values']['dwca_style'];
    }else if(module_exists('biblio_citeproc')){
      $dwca_default = variable_get('dwca_biblio_citeproc_style', '_use_default');
    }else{
      $dwca_default = variable_get('dwca_biblio_style', '_use_default');
    }
    $dwca_options = array_merge(array(
      '_use_default' => t('(Same as site citation style)')
    ), $options);
    $form['dwca'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE
    );
    $form['dwca']['dwca_style'] = array(
      '#type' => 'select',
      '#title' => t('DwCA export citation style'),
      '#description' => t('Style to use when creating Darwin Core Archives'),
      '#options' => $dwca_options,
      '#default_value' => $dwca_default
    );
    if($example){
      $form['dwca']['dwca_style']['#ajax'] = array(
        'callback' => 'scratchpads_backend_biblio_dwca_ajax',
        'wrapper' => 'scratchpads_backend_biblio_dwca_example',
        'effect' => 'fade',
        'speed' => '200'
      );
      $render_style = ($dwca_default == '_use_default') ? $default : $dwca_default;
      $render = scratchpads_backend_biblio_render_style($example, $render_style);
      $form['dwca']['dwca_example'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="scratchpads_backend_biblio_dwca_example" class="form-item">',
        '#suffix' => '</div>',
        '#markup' => '<label>' . t('Example') . '</label> ' . $render
      );
    }
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save settings'
  );
  return $form;
}

/**
 * scratchpads_backend_biblio_default_ajax
 */
function scratchpads_backend_biblio_default_ajax($form, &$form_state){
  $commands = array(
    ajax_command_replace('#scratchpads_backend_biblio_default_example', render($form['default']['default_example']), array(
      'effect' => 'fade',
      'speed' => '200'
    ))
  );
  if(isset($form['dwca']['dwca_style']) && $form['dwca']['dwca_style']['#default_value'] == '_use_default'){
    $commands[] = ajax_command_replace('#scratchpads_backend_biblio_dwca_example', render($form['dwca']['dwca_example']), array(
      'effect' => 'fade',
      'speed' => '200'
    ));
  }
  return array(
    '#type' => 'ajax',
    '#commands' => $commands
  );
}

/**
 * scratchpads_backend_biblio_dwca_ajax
 */
function scratchpads_backend_biblio_dwca_ajax($form, &$form_state){
  return $form['dwca']['dwca_example'];
}

/**
 * scratchpads_backend_biblio_settings_form_submit
 */
function scratchpads_backend_biblio_settings_form_submit($form, &$form_state){
  if(module_exists('biblio_citeproc')){
    variable_set('biblio_citeproc_style', $form_state['values']['default_style']);
    variable_set('dwca_biblio_citeproc_style', $form_state['values']['dwca_style']);
  }else{
    variable_set('biblio_style', $form_state['values']['default_style']);
    variable_set('dwca_biblio_style', $form_state['values']['dwca_style']);
  }
  drupal_set_message(t('Your settings have been saved'));
}

/**
 * scratchpads_backend_biblio_render_style
 * 
 * theme_biblio_citeproc_style uses a static variable that prevents
 * us from rendering using two different styles during the same page
 * execution, so we need our own renderer.
 */
function scratchpads_backend_biblio_render_style($node, $style){
  // If we're not using citeproc, revert to normal biblio rendering
  if(!module_exists('biblio_citeproc')){return theme('biblio_style', array(
      'node' => $node,
      'style_name' => $style
    ));}
  // Same code as theme_biblio_citeproc_style except the static is a per-style array.
  $citeproc_static = &drupal_static(__FUNCTION__, array());
  global $language;
  $cached = NULL;
  module_load_include('inc', 'biblio_citeproc', 'CSL');
  if(!empty($citeproc_static[$style])){
    $citeproc = $citeproc_static[$style];
  }else{
    $csl_id = ($style) ? $style : biblio_get_style();
    if(strpos($csl_id, '.csl') === FALSE){ // try to convert old style names to csl...
      if(in_array($csl_id, array(
        'ama',
        'apa',
        'cse',
        'ieee',
        'mla',
        'vancouver'
      ))){
        $csl_id .= '.csl';
      }elseif($csl_id == 'chicago'){
        $csl_id = 'chicago-fullnote-bibliography.csl';
      }else{
        $csl_id = '';
        $message = t('An invalid style "@style" was selected, please check your "CiteProc" style settings.', array(
          '@style' => $csl_id
        ));
        drupal_set_message($message, 'error');
      }
    }
    if(!empty($csl_id)){
      $csl = db_query('SELECT parent,csl FROM {biblio_citeproc_styles} WHERE filename = :id', array(
        ':id' => $csl_id
      ))->fetchObject();
      if(!isset($csl->csl)){
        drupal_set_message(t('Biblio-CiteProc could not fetch the style file: @csl_id from the database. Check your CiteProc settings.', array(
          '@csl_id' => $csl_id
        )), 'error');
        return;
      }
      if(!empty($csl->parent)){
        $csl_file_contents = db_query("SELECT csl FROM {biblio_citeproc_styles} WHERE id = :id", array(
          ':id' => $csl->parent
        ))->fetchField();
      }else{
        $csl_file_contents = $csl->csl;
      }
      //    $cslid = $csl_file_name . '-' . $language->language;
      //    $cached = cache_get($cslid, 'cache_biblio_csl_object');
      if(!$cached){
        $citeproc = new citeproc($csl_file_contents, $language->language);
        //      cache_set($cslid, $citeproc, 'cache_biblio_csl_object');
      }else{
        $citeproc = $cached->data;
      }
    }
    $citeproc_static[$style] = $citeproc;
  }
  $styled_node = $citeproc->render($node);
  $coins_data = isset($node->biblio_coins) ? filter_xss($node->biblio_coins, array(
    'span'
  )) : '';
  return ($styled_node . $coins_data);
}