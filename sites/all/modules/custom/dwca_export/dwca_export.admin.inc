<?php

/**
 * menu callback
 */
function dwca_export_deliver_archive(){
  if(file_exists('public://dwca.zip')){
    file_transfer('public://dwca.zip', array(
      'Content-Type' => 'application/zip',
      'Last-Modified' => gmdate(DATE_RFC1123, filemtime('public://dwca.zip'))
    ));
  }else{
    drupal_set_message('The Darwincore Archive for this site has not yet been created');
    drupal_goto('/');
  }
}

/**
 * Form function, called by drupal_get_form()
 * in dwca_export_menu().
 */
function dwca_export_config_form($form, &$form_state){
  global $base_url;
  $form['dwca_export_info'] = array(
    '#markup' => '<p>For general information on the DarwinCore Archive format please refer to  ' . l('GBIF - Standards and Tools - Darwin Core Archives', 'http://www.gbif.org/informatics/standards-and-tools/publishing-data/data-standards/darwin-core-archives/') . '</p>'
  );
  $form['dwca_export_execute'] = array(
    '#markup' => '<p>The DarwinCore Archive export can be downloaded from ' . l(url('dwca.zip', array(
      'absolute' => TRUE
    )), 'dwca.zip') . '</p>'
  );
  $form['dwca_export_view_mapping'] = dwca_export_select_view_form();
  $form['#submit'][] = 'dwca_export_config_form_submit';
  return system_settings_form($form);
}

function dwca_export_select_view_form(){
  $views = array(
    '#type' => 'fieldset',
    '#title' => t('View to file mapping')
  ); //'#tree' => TRUE,
  foreach(variable_get(FILE_MAP, 'dwca_export_archive_descriptor_file_map') as $dwca_filename => $view_data){
    if($dwca_filename == 'description'){
      $views[$dwca_filename] = array(
        '#type' => 'fieldset',
        '#title' => t('DESCRIPTIVE DATA TYPES'),
        '#weight' => 3,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        //'#prefix' => '<table>',
        //'#suffix' => '</table>',
        '#tree' => TRUE
      );
      foreach($view_data as $dwca_filename_inner => $view_data_inner){
        $views[$dwca_filename][$dwca_filename_inner] = array(
          '#type' => 'textfield',
          '#title' => t($dwca_filename . '_' . $dwca_filename_inner),
          '#default_value' => $view_data_inner['view_name'],
          '#size' => 60,
          '#description' => t('specify view for ' . $dwca_filename_inner)
        ); //'#prefix' => '<tr>',
        //'#suffix' => '</tr>',
        //TODO: Change collaped discription form to table, layout using <td> and <tr> e.g. try 3 description types per row
      }
    }else{
      $views[$dwca_filename] = array(
        '#type' => 'textfield',
        '#title' => t($dwca_filename),
        '#default_value' => $view_data['view_name'],
        '#size' => 60,
        '#description' => t('specify view for ' . $dwca_filename)
      );
    }
  }
  return $views;
}

/**
 * Submit function for the above form.
 */
function dwca_export_config_form_submit($form, &$form_state){
  $variables = $form_state['input'];
  $save_variables = '';
  $dwca_export_archive_descriptor_file_map = variable_get(FILE_MAP);
  foreach($variables as $key => $value){
    if(array_key_exists($key, $dwca_export_archive_descriptor_file_map)){
      $dwca_export_archive_descriptor_file_map[$key]['view_name'] = $value;
      //$save_variables .= '<p>' . $key . ' ' . $value . '</p>';
    }else{
      $description_key = 'description';
      if(array_key_exists($description_key, $dwca_export_archive_descriptor_file_map)){
        //get the inner array containing the different description data types
        $description_map = $dwca_export_archive_descriptor_file_map[$description_key];
        if(array_key_exists($key, $description_map)){
          $dwca_export_archive_descriptor_file_map[$description_key][$key]['view_name'] = $value;
        }
      }
    }
  }
  variable_del(FILE_MAP);
  variable_set(FILE_MAP, $dwca_export_archive_descriptor_file_map);
}

/**
 * Validate function for the above form.
 * 
 * Reports an error if view name entered by the user does not exist in the
 * database.
 */
function dwca_export_config_form_validate($form, &$form_state){
  $variables = $form_state['input'];
  $dwca_export_archive_descriptor_file_map = variable_get(FILE_MAP);
  $view_names = array();
  $missing_view_names = '';
  $missing_view = false;
  foreach($variables as $key => $value){
    //TODO: Check whether the views for the inner array cotaining all the description views exist
    if(array_key_exists($key, $dwca_export_archive_descriptor_file_map)){
      $view = views_get_view($value);
      // check whether there is a view named with this value 
      if(!$view){
        $view_names[] = $value;
        $missing_view_names .= $value . ', ';
        $missing_view = true;
      }
    }
  }
  if($missing_view){
    form_set_error('', t('VIEW(S) ' . $missing_view_names . ' NOT FOUND. Please input another view name.'));
  }
}