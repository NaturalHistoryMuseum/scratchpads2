<?php
// OBOE interface class
class OBOEService{

  private $auth_token;

  private $job_name;

  private $job_description;

  private $type;

  private $id;

  private $response;

  private $status;

  private $parameters = array();

  private $inputurl;

  private static $service = 'https://oboe.oerc.ox.ac.uk/';

  /**
   * Standard constructor for the OBOEService class.  A user should pass either
   * an ID, or an array of job details.
   * 
   * @param unknown_type $job_details
   * 
   * This should either be an ID of a job that has already been created on the
   * OBOE server, or an array of new job details
   * 
   * @param String $at
   * 
   * Auth token.
   */
  function __construct($job_details = FALSE, $at = FALSE){
    // Check for the PECL extension.
    if(!function_exists('http_post_data')){throw new OBOEServiceException(t('PECL pecl_http is not installed.'));}
    $this->auth_token = self::get_auth_token($at);
    if($job_details){
      if(is_array($job_details)){
        foreach($job_details as $key => $value){
          $this->{$key} = $value;
        }
      }else{
        $this->id = $job_details;
      }
    }
  }

  /**
   * Set the parameters for an OBOE Job
   * @param array $parameters
   * A keyed array of parameters
   * @param unknown_type $reset
   * Whether to append, or replace all.
   */
  function set_parameters($parameters, $reset = FALSE){
    if($reset){
      $this->parameters = array();
    }
    // inputurl is a special parameter, so we pull it out (if set).
    if(isset($parameters['inputurl']) && is_string($parameters['inputurl'])){
      $this->inputurl = $parameters['inputurl'];
      unset($parameters['inputurl']);
    }
    // job_name is a special parameter, so we pull it out (if set).
    if(isset($parameters['job_name']) && is_string($parameters['job_name'])){
      $this->job_name = $parameters['job_name'];
      unset($parameters['job_name']);
    }
    // job_name is a special parameter, so we pull it out (if set).
    if(isset($parameters['job_description']) && is_string($parameters['job_description'])){
      $this->job_description = $parameters['job_description'];
      unset($parameters['job_description']);
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
   * Set the input URL for a job.
   * @param unknown_type $inputurl
   */
  function set_inputurl($inputurl){
    if(is_string($inputurl)){
      $this->inputurl = $inputurl;
    }
  }

  /**
   * Get the currently set inputurl.
   */
  function get_inputurl(){
    return $this->inputurl;
  }

  /**
   * Set the job name.
   * @param unknown_type $job_name
   */
  function set_job_name($job_name){
    if(is_string($job_name)){
      $this->job_name = $job_name;
    }
  }

  /**
   * Get the job name.
   */
  function get_job_name(){
    return $this->job_name;
  }

  /**
   * Set the job description.
   * @param string $job_description
   */
  function set_job_description($job_description){
    if(is_string($job_description)){
      $this->job_description = $job_description;
    }
  }

  /**
   * Get the job description.
   */
  function get_job_description(){
    return $this->job_description;
  }

  /**
   * Set the job type.  This checks that the type is in the array of job types.
   * @param unknown_type $type
   */
  function set_type($type){
    if(is_string($type) && in_array($type, array_keys(self::get_job_types()))){
      $this->type = $type;
    }
  }

  /**
   * Return the currently set job type.
   */
  function get_type(){
    return $this->type;
  }

  /**
   * Return the response for this job.  If the response isn't set, we update it.
   * @param unknown_type $update
   */
  function get_response($update = FALSE){
    if($this->response && !$update){
      return $this->response;
    }else{
      // We don't have a response set, or we want to update it.
      if($this->id){
        return json_decode(file_get_contents(self::$service . 'jobs/' . $this->id . '.json?auth_token=' . $this->auth_token), TRUE);
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
   * Submit the job to the OBOE server.
   */
  function run(){
    $this->job_description = $this->job_description ? $this->job_description : date('c');
    $this->job_name = $this->job_name ? $this->job_name : microtime();
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
    $info = array();
    $this->response = http_post_data(self::$service . 'jobs.json', http_build_query($fields), array(
      'timeout' => 10
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
   * Return a download URL for this job.
   */
  function get_download_url(){
    if($this->id){
      return self::$service . 'download/' . $this->id . '?auth_token=' . $this->auth_token;
    }else{
      return FALSE;
    }
  }

  /**
   * Static function to simply return an auth token.  This function will attempt
   * to read an auth token from a file.
   * @param string/boolean $at
   */
  static function get_auth_token($at = FALSE){
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

  /**
   * Simple function to return an array of job types as returned from the OBOE
   * service.
   * @param auth_token.
   */
  static function get_job_types($at = FALSE){
    $at = self::get_auth_token($at);
    return json_decode(file_get_contents(self::$service . 'types?auth_token=' . $at), TRUE);
  }

  /**
   * Simply returns the documentation as provided by the OBOE service.
   * @param unknown_type $at
   * @throws OBOEServiceException
   */
  static function get_documentation($at = FALSE){
    $at = self::get_auth_token($at);
    return file_get_contents(self::$service . 'docs?auth_token=' . $at);
  }

  /**
   * Return information for all job types.
   * @param unknown_type $auth_token
   */
  static function get_information($at = FALSE){
    $at = self::get_auth_token($at);
    $data = json_decode(file_get_contents(self::$service . 'information?auth_token=' . $at), TRUE);
    $jobs = array();
    foreach($data['categories'] as $category){
      foreach($category['jobs'] as $job){
        $job['category'] = $category['name'];
        $jobs[$job['type']] = $job;
      }
    }
    return $jobs;
  }

  /**
   * Return the parameters for a job_type.
   * @param unknown_type $type
   * @param unknown_type $auth_token
   */
  static function get_parameters_by_type($type = FALSE, $at = FALSE){
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