<?php
// Taverna interface class
class TavernaService{

  private $run_name;

  private $run_description;

  private $type;

  private $id;

  private $response;

  private $status;

  private $parameters = array();

  private static $service = 'https://dev.at.biovel.eu/';

  /**
   * Standard constructor for the TavernaService class.  A user should pass either
   * an ID, or an array of run details.
   * 
   * @param unknown_type $run_details
   * 
   * This should either be an ID of a run that has already been created on the
   * Taverna server, or an array of new run details
   */
  function __construct($run_details = FALSE){
    // Check for the PECL extension.
    if(!function_exists('http_post_data')){throw new TavernaServiceException(t('PECL pecl_http is not installed.'));}
    if($run_details){
      if(is_array($run_details)){
        foreach($run_details as $key => $value){
          $this->{$key} = $value;
        }
      }else{
        $this->id = $run_details;
      }
    }
  }

  /**
   * Set username and password for authentication
   */
  /**
   * Set the parameters for a Taverna Job
   * @param array $parameters
   * A keyed array of parameters
   * @param unknown_type $reset
   * Whether to append, or replace all.
   */
  function set_parameters($parameters, $reset = FALSE){
    if($reset){
      $this->parameters = array();
    }
    // run_name is a special parameter, so we pull it out (if set).
    if(isset($parameters['run_name']) && is_string($parameters['run_name'])){
      $this->run_name = $parameters['run_name'];
      unset($parameters['run_name']);
    }
    // run_description is a special parameter, so we pull it out (if set).
    if(isset($parameters['run_description']) && is_string($parameters['run_description'])){
      $this->run_description = $parameters['run_description'];
      unset($parameters['run_description']);
    }
    foreach($parameters as $key => $value){
      $this->parameters[$key] = $value;
    }
  }

  /**
   * Return the parameters that have been set.
   */
  function get_parameters(){
    return $this->parameters;
  }

  /**
   * Set the run name.
   * @param unknown_type $run_name
   */
  function set_run_name($run_name){
    if(is_string($run_name)){
      $this->run_name = $run_name;
    }
  }

  /**
   * Get the run name.
   */
  function get_run_name(){
    return $this->run_name;
  }

  /**
   * Set the run description.
   * @param string $run_description
   */
  function set_run_description($run_description){
    if(is_string($run_description)){
      $this->run_description = $run_description;
    }
  }

  /**
   * Get the run description.
   */
  function get_run_description(){
    return $this->run_description;
  }

  /**
   * Set the run type.  This checks that the type is in the array of run types.
   * @param unknown_type $type
   */
  function set_type($type){
    if(is_string($type) && in_array($type, array_keys(self::get_run_types()))){
      $this->type = $type;
    }
  }

  /**
   * Return the currently set run type.
   */
  function get_type(){
    return $this->type;
  }

  /**
   * Return the response for this run.  If the response isn't set, we update it.
   * @param unknown_type $update
   */
  function get_response($update = FALSE){
    if($this->response && !$update){
      return $this->response;
    }else{
      // We don't have a response set, or we want to update it.
      if($this->id){
        $context = stream_context_create(array(
          'http' => array(
            'method' => 'GET',
            'header' => implode("\r\n", array(
              'Accept: application/json',
              'Authorization: ' . self::get_authentication_string()
            ))
          )
        ));
        $response = file_get_contents(self::$service . 'runs/' . $this->id, FALSE, $context);
        $this->response = json_decode($response, TRUE);
        return $this->response;
      }else{
        return FALSE;
      }
    }
  }

  /**
   * Get the status.
   * Enter description here ...
   */
  function get_status(){
    return $this->status;
  }

  /**
   * Do an interaction
   */
  function interact(){
    $this->get_response(TRUE);
    $raw_data = file_get_contents("php://input");
    $response = http_post_data($this->response['interaction']['reply_uri'], $raw_data, array(
      'timeout' => 10,
      'headers' => array(
        'Content-Type' => 'application/json',
        'Accept' => 'application/json',
        'Authorization' => self::get_authentication_string(),
        'X-Taverna-Interaction-Reply' => $_SERVER['HTTP_X_TAVERNA_INTERACTION_REPLY']
      )
    ), $info);
  }

  /**
   * Submit the run to the Taverna server.
   */
  function run(){
    // Add logic for creating a new run.
    $run_type = $this->get_run_types($this->type);
    $json = array(
      'run' => array(
        'workflow_id' => $run_type['workflow_id'],
        'name' => "{$run_type['name']} ({$this->run_description})"
      )
    );
    if(count($this->get_parameters())){
      $json['run']['inputs_attributes'] = array();
      foreach($this->get_parameters() as $name => $value){
        $json['run']['inputs_attributes'][] = array(
          'name' => $name,
          'value' => $value
        );
      }
    }
    $this->response = http_post_data(self::$service . 'runs', json_encode($json), array(
      'timeout' => 10,
      'headers' => array(
        'Content-Type' => 'application/json',
        'Accept' => 'application/json',
        'Authorization' => self::get_authentication_string()
      )
    ), $info);
    if($this->response){
      $http_reply = http_parse_message($this->response);
      if(isset($http_reply->body)){
        $this->response = json_decode($http_reply->body, TRUE);
        if($info['response_code'] == 201 || $info['response_code'] == 200){
          $this->status = 1;
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  /**
   * Return a download URL for this run.
   */
  function get_download(){
    if($this->id){
      $response = $this->get_response(TRUE);
      $context = stream_context_create(array(
        'http' => array(
          'method' => 'GET',
          'header' => implode("\r\n", array(
            'Accept: application/json',
            'Authorization: ' . self::get_authentication_string()
          ))
        )
      ));
      $file = file_save_data(file_get_contents(self::$service . $response['outputs_zip'], FALSE, $context));
      $file = file_move($file, 'public://taverna-' . $this->id . '.zip');
      $file->filename = 'taverna-' . $this->id . '.zip';
      file_save($file);
      return $file->fid;
    }else{
      return FALSE;
    }
  }

  /**
   * Simple function to return an array of run types as returned from the Taverna
   * service.
   */
  static function get_run_types($type_to_return = FALSE, $reset = FALSE){
    $types = array();
    if(!$reset){
      $data = cache_get('taverna_run_types', 'cache_taverna');
      if($data && $data->data){
        $types = $data->data;
      }
    }
    if(!$types){
      $context = stream_context_create(array(
        'http' => array(
          'method' => 'GET',
          'header' => implode("\r\n", array(
            'Accept: application/json',
            'Authorization: ' . self::get_authentication_string()
          ))
        )
      ));
      $types = array();
      $json = @file_get_contents(self::$service . 'workflows', FALSE, $context);
      if($json){
        $types = json_decode($json, TRUE);
        foreach($types as $key => $type){
          unset($types[$key]);
          $types[str_replace(' ', '', strtolower($type['title']))] = $type;
        }
      }
      cache_set('taverna_run_types', $types, 'cache_taverna', time() + 86400);
    }
    if($type_to_return){
      if(!$reset){
        $data = cache_get('taverna_run_types_' . $type_to_return, 'cache_taverna');
        if($data && $data->data){return $data->data;}
      }
      $context = stream_context_create(array(
        'http' => array(
          'method' => 'GET',
          'header' => implode("\r\n", array(
            'Accept: application/json',
            'Authorization: ' . self::get_authentication_string()
          ))
        )
      ));
      $types[$type_to_return] = json_decode(file_get_contents(self::$service . 'runs/new?workflow_id=' . $types[$type_to_return]['id'], FALSE, $context), TRUE);
      if(isset($types[$type_to_return]['run'])){
        $types[$type_to_return] = $types[$type_to_return]['run'];
      }
      cache_set('taverna_run_types_' . $type_to_return, $types[$type_to_return], 'cache_taverna', time() + 86400);
      return $types[$type_to_return];
    }
    return $types;
  }

  /**
   * Simple function to return an interaction from the portal.
   */
  static function get_interaction($url){
    $context = stream_context_create(array(
      'http' => array(
        'method' => 'GET',
        'header' => implode("\r\n", array(
          'Accept: application/json',
          'Authorization: ' . self::get_authentication_string()
        ))
      )
    ));
    $content = file_get_contents($url, FALSE, $context);
    return str_replace('/assets/taverna_player/application.js', self::$service . 'assets/taverna_player/application.js', $content);
  }

  /**
   * Static function to return a basic authentication string (or better if the
   * portal is upgraded).
   */
  static function get_authentication_string(){
    // We try and read the authtoken from a configuration file.
    if(function_exists('drupal_get_path')){
      $user_and_password = file_get_contents(drupal_get_path('module', 'taverna') . '/auth_token.ini');
    }else{
      $user_and_password = file_get_contents(drupal_get_path('module', 'taverna') . '/auth_token.ini');
    }
    if(!$user_and_password){throw new TavernaServiceException('Auth token is not set');}
    return 'Basic ' . base64_encode(trim($user_and_password));
  }
}
// Error class for the above.  This allows us to behave a little!
class TavernaServiceException extends Exception{}
// We define our own t() function to allow this code to be used outside of
// Drupal
if(!function_exists('t')){

  function t($string){
    return $string;
  }
}
