<?php

/**
 * citethispage_admin_form
 */
function citethispage_settings_form($form, &$form_state){
  $form['tabs'] = array(
    '#type' => 'vertical_tabs'
  );
  // General settings  
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#group' => 'tabs'
  );
  $form['general']['citethispage_showall'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Cite This Page button on all pages'),
    '#description' => t('If this is not selected, then the Cite This Page button will only show on pages that are supported by one of the page information parsers.'),
    '#default_value' => variable_get('citethispage_showall', 0)
  );
  $warn_default = variable_get('citethispage_warn_logged', array(
    'value' => CITETHISPAGE_DEFAULT_WARN_LOGGED,
    'format' => 'filtered_html'
  ));
  $form['general']['citethispage_warn_logged'] = array(
    '#type' => 'text_format',
    '#title' => 'Message for logged in users',
    '#default_value' => $warn_default['value'],
    '#format' => $warn_default['format']
  );
  // Backend settings
  $form['backend'] = array(
    '#type' => 'fieldset',
    '#title' => t('Archive service providers'),
    '#description' => t('Archive service providers are the external services used to archive a given page as it is at a given time. Here you can order and enable/disable selected archive service providers.'),
    '#group' => 'tabs'
  );
  $handlers = _citethispage_get_archive_handlers(TRUE);
  if(empty($handlers)){
    $form['backend']['header'] = array(
      '#type' => 'markup',
      '#markup' => t('There are no on-demand archive service providers. You should enable archive service provider modules such as the bundled citethispage_webcite or citethispage_archiveis modules.')
    );
  }else{
    $form['backend']['table'] = array(
      '#theme' => 'citethispage_table_drag',
      '#columns' => array(
        'backend_name_' => t('Service backend'),
        'backend_description_' => t('Description'),
        'backend_enabled_' => t('Enabled'),
        'backend_weight_' => t('Weight')
      ),
      '#weight_column' => 'backend_weight_',
      '#table_id' => 'citethispage_backend_table'
    );
    foreach($handlers as $module => $module_info){
      $form['backend']['table'][$module]['backend_weight_' . $module] = array(
        '#type' => 'textfield',
        '#default_value' => $module_info['weight'],
        '#size' => 3
      );
      $form['backend']['table'][$module]['backend_name_' . $module] = array(
        '#type' => 'markup',
        '#markup' => $module_info['title']
      );
      $form['backend']['table'][$module]['backend_description_' . $module] = array(
        '#type' => 'markup',
        '#markup' => $module_info['description']
      );
      $form['backend']['table'][$module]['backend_enabled_' . $module] = array(
        '#type' => 'checkbox',
        '#default_value' => $module_info['enabled']
      );
    }
  }
  // Parser settings
  $form['parsers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page information parsers'),
    '#description' => t('Page information parsers are used to gather citation related information (such as authors) about a particular page. This helps users cite the page properly. Here you can select which parsers should be used for different types of pages.'),
    '#group' => 'tabs'
  );
  $handlers = _citethispage_get_parse_handlers(TRUE);
  if(empty($handlers)){
    $form['parsers']['header'] = array(
      '#type' => 'markup',
      '#markup' => t('There are no page information parsers enabled. This is not required - you can still use "Cite this page" by enabling "Show cite this page button on all pages" in the general settings. However the module works best when such parsers are enabled - you can for instance enable the bundled citethispage_nodes module which will provide information about node pages.')
    );
  }else{
    foreach($handlers as $path => $modules){
      $form['parsers'][$path] = array(
        '#type' => 'fieldset',
        '#title' => t('Handlers for %path', array(
          '%path' => $path
        )),
        '#description' => t('Arrange the handlers that will apply for pages with URL of the type %path', array(
          '%path' => $path
        ))
      );
      $form['parsers'][$path]['table'] = array(
        '#theme' => 'citethispage_table_drag',
        '#columns' => array(
          'parser_name_' . $path . '_' => t('Parser'),
          'parser_description_' . $path . '_' => t('Description'),
          'parser_enabled_' . $path . '_' => t('Enabled'),
          'parser_default_' . $path . '_' => t('Default'),
          'parser_weight_' . $path . '_' => t('Weight')
        ),
        '#weight_column' => 'parser_weight_' . $path . '_',
        '#table_id' => 'citethispage_parser_' . preg_replace('/[^-a-zA-Z0-9_]/', '_', $path)
      );
      foreach($modules as $module => $module_info){
        $form['parsers'][$path]['table'][$module]['parser_weight_' . $path . '_' . $module] = array(
          '#type' => 'textfield',
          '#default_value' => $module_info['weight'],
          '#size' => 3
        );
        $form['parsers'][$path]['table'][$module]['parser_name_' . $path . '_' . $module] = array(
          '#type' => 'markup',
          '#markup' => $module_info['title']
        );
        $form['parsers'][$path]['table'][$module]['parser_description_' . $path . '_' . $module] = array(
          '#type' => 'markup',
          '#markup' => $module_info['description']
        );
        $form['parsers'][$path]['table'][$module]['parser_default_' . $path . '_' . $module] = array(
          '#type' => 'checkbox',
          '#default_value' => $module_info['default']
        );
        $form['parsers'][$path]['table'][$module]['parser_enabled_' . $path . '_' . $module] = array(
          '#type' => 'checkbox',
          '#default_value' => $module_info['enabled']
        );
      }
    }
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration')
  );
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'citethispage') . '/theme/citethispage.css'
  );
  return $form;
}

/**
 * citethispage_admin_form_submit
 */
function citethispage_settings_form_submit($form, &$form_state){
  // Save general settings
  variable_set('citethispage_showall', $form_state['values']['citethispage_showall']);
  variable_set('citethispage_warn_logged', $form_state['values']['citethispage_warn_logged']);
  // Save backend settings
  $handlers = _citethispage_get_archive_handlers(TRUE);
  $status = array();
  foreach($handlers as $module => $module_info){
    $status[$module] = array(
      'enabled' => $form_state['values']['backend_enabled_' . $module],
      'weight' => $form_state['values']['backend_weight_' . $module]
    );
  }
  variable_set('citethispage_archive_handlers', $status);
  // Save parser settings
  $handlers = _citethispage_get_parse_handlers(TRUE);
  $status = array();
  foreach($handlers as $path => $modules){
    foreach($modules as $module => $module_info){
      $status[$path][$module] = array(
        'enabled' => $form_state['values']['parser_enabled_' . $path . '_' . $module],
        'weight' => $form_state['values']['parser_weight_' . $path . '_' . $module],
        'default' => $form_state['values']['parser_default_' . $path . '_' . $module]
      );
    }
  }
  variable_set('citethispage_parse_handlers', $status);
  drupal_set_message(t('The configuration options have been saved.'));
}


