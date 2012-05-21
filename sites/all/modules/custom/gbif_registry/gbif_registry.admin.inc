<?php
/**
 * Return the form which allows us to set the Registry to use.
 */
use GBIF\Registry;

function gbif_registry_admin_form(){
  return system_settings_form(array(
    'gbif_registry_web_service_url' => array(
      '#type' => 'textfield',
      '#title' => t('GBIF Registry web service URL'),
      '#default_value' => variable_get('gbif_registry_web_service_url', 'http://gbrds.gbif.org/registry/')
    )
  ));
}

/**
 * Validate the data sent from the settings form.
 */
function gbif_registry_admin_form_validate(&$form, &$form_state){
  // We check that the Registry URL ends with a slash.
  if(substr($form_state['values']['gbif_registry_web_service_url'], -1) != '/'){
    form_set_error('gbif_registry_web_service_url', t('The web service URL must end with a "/".'));
  }
}

/**
 * Display a single GBIF Registry object
 */
function gbif_registry_view_object($type = FALSE, $id = FALSE){
  if(!$type || !$id){
    return t('An example object is shown below.');
  }else{
    // Create the registry object first to ensure we're using our cache one!
    $registry = GBIFRegistry::singleton();
    if(class_exists('GBIF\\' . $type)){
      $class = gbif_registry_create_instance('GBIF\\' . $type, array(
        $id
      ));
      $rows = array(
        array(
          'Type',
          implode(" ", preg_split('/(?<=\\w)(?=[A-Z])/', $type))
        )
      );
      foreach($class->getAsArray() as $key => $value){
        switch($key){
          default:
            if(is_string($value) && trim($value)){
              $key = implode(" ", preg_split('/(?<=\\w)(?=[A-Z])/', ucfirst($key)));
              $rows[] = array(
                $key,
                $value
              );
            }
        }
      }
      return array(
        'table' => array(
          '#theme' => 'table',
          '#header' => array(
            t('Key'),
            t('Value')
          ),
          '#rows' => $rows
        )
      );
    }
  }
}

/**
 * Dynamically create an instance of a GBIF class.
 */
function gbif_registry_create_instance($class, $params){
  $reflection_class = new ReflectionClass($class);
  return $reflection_class->newInstanceArgs($params);
}

/**
 * Display the test page.
 */
function gbif_registry_test_page(){
  $registry = GBIFRegistry::singleton();
  $class = arg(5) ? ucfirst(arg(5)) : 'Organizations';
  if(method_exists($registry, "get$class")){
    $objects = call_user_func(array(
      $registry,
      "get$class"
    ));
    $rows = array();
    $i = 0;
    if(isset($_GET['offset'])){
      $objects->setOffset($_GET['offset']);
    }
    foreach($objects as $object){
      $rows[] = array(
        'title' => $object->getTitle(),
        'description' => l($object->getHomepage(), $object->getHomepage()),
        'key' => l($object->getKey(), "admin/config/services/gbif_registry/view/" . substr($class, 0, -1) . "/{$object->getKey()}")
      );
      $i++;
      if($i == 20){
        break;
      }
    }
    return array(
      'objects' => array(
        '#type' => 'fieldset',
        '#title' => t($class),
        '#collapsed' => TRUE,
        '#collapsible' => TRUE,
        'table' => array(
          '#theme' => 'table',
          '#header' => array(
            t('Title'),
            t('Homepage'),
            t('Key')
          ),
          '#rows' => $rows
        )
      )
    );
  }
}