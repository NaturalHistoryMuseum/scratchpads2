<?php

// OBOE interface class
class OBOEService{

  private $auth_token;

  private $job_name;

  private $job_description;

  private $type;

  private $response;

  private $status;

  private $parameters = array();

  private $inputurl;

  private static $service = 'https://oboe.oerc.ox.ac.uk/';

  // Standard constructor.  Must be passed the auth token for the service, else
  // we'll throw an error.
  function __construct($at = FALSE, $type = FALSE, $job_name = FALSE, $job_description = ''){
    $at = self::get_auth_token($at);
    // Check for the PECL extension.
    if(!function_exists('http_post_data')){throw new OBOEServiceException(t('PECL pecl_http is not installed.'));}
    $this->auth_token = $at;
    if(!$job_name){
      // Generate a job name based on the requesting user, and the time.
    }
    $this->job_description = $job_description;
    if($type){
      $this->type = $type;
    }
  }

  // Set the job type that we intend to run
  // Set additional parameters for this job
  function set_parameters($parameters, $reset = FALSE){
    if($reset){
      $this->parameters = array();
    }
    // inputurl is a special parameter, so we pull it out (if set).
    if(isset($parameters['inputurl'])){
      $this->inputurl = $parameters['inputurl'];
      unset($parameters['inputurl']);
    }
    $this->parameters = $parameters;
  }

  function get_parameters(){
    return $this->parameters;
  }

  // Set and get the inputurl
  function set_inputurl($inputurl){
    $this->inputurl = $inputurl;
  }

  function get_inputurl(){
    return $this->inputurl;
  }

  // Get the response
  function get_response(){
    return $this->response;
  }

  // Validate that we have the correct parameters for the job that we're
  // attempting to run.
  private function validate(){
    return TRUE;
  }

  // Do the do!
  function run(){ // We've failed validation, we should set an error message.
    if(!$this->validate()){throw new OBOEServiceException(t('Failed validation.'));}
    $fields = array(
      'auth_token' => $this->auth_token,
      'job' => array(
        'job_name' => $this->job_name,
        'job_description' => $this->job_description,
        'type' => $this->type,
        'parameters' => $this->parameters
      )
    );
    if(isset($this->inputurl)){
      $fields['job']['inputurl'] = $this->inputurl;
    }
    $this->response = http_post_data($this->service . 'jobs.json', http_build_query($fields));
  }

  // Static function to get an auth_token
  static function get_auth_token($at){
    // Check that we're been given an auth token.
    if(!$at){
      // We try and read the authtoken from a configuration file.
      if(function_exists('drupal_get_path')){
        $ini = @parse_ini_file(drupal_get_path('module', 'oboe') . '/auth_token.ini');
      }else{
        $ini = @parse_ini_file(dirname(__FILE__) . '/auth_token.ini');
      }
      if($ini && isset($ini['auth_token'])){
        $at = $ini['auth_token'];
      }else{
        throw new OBOEServiceException('Auth token is not set');
      }
    }
    return $at;
  }

  // Static function to get a list of job types
  static function get_job_types($at = FALSE){
    $at = self::get_auth_token($at);
    return json_decode(file_get_contents(self::$service . 'types?auth_token=' . $at), TRUE);
  }

  // Static function to get the help data.
  static function get_documentation($at = FALSE){
    $at = self::get_auth_token($at);
    return file_get_contents(self::$service . 'docs?auth_token=' . $at);
  }

  // Static function to get the parameters for a job type.
  static function get_parameters_by_type($at = FALSE, $type = FALSE){
    $at = self::get_auth_token($at);
    if(!$type || !in_array($type, array_keys(self::get_job_types($at)))){
      throw new OBOEServiceException(t('@type is not recognised by OBOE.', array(
        '@type' => $type
      )));
    }else{
      return json_decode(file_get_contents(self::$service . 'parameters/' . $type . '?auth_token=' . $at), TRUE);
    }
  }
}

// Error class for the above.  This allows us to behave a little!
class OBOEServiceException extends Exception{}
// We define our own t() function to allow this code to be used outside of
// Drupal
if(!function_exists('t')){

  function t($string){
    return $string;
  }
}