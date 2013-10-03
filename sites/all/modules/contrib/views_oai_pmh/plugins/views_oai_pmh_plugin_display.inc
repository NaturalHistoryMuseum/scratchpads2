<?php

/**
 * @file
 * Contains the OAI-PMH display plugin.
 *
 */

/**
 *
 * We are based on a feed display for compatibility.
 *
 * @ingroup views_display_plugins
 */
class views_oai_pmh_plugin_display extends views_plugin_display_page {
  
  public $oai_args;
  public $oai_errors = array();
  protected $oai_compression = TRUE;

  function init(&$view, &$display, $options= NULL ) {
  
  	 
    parent::init($view, $display, $options);
 
  	 
    // Set the default row style. Ideally this would be part of the option
    // definition, but in this case it's dependent on the view's base table,
    // which we don't know until init().
    //if ($this->oai_args['metadataPrefix']=='agrisap'){
        
        //$view->display['default']->handler  = 'views_oai_pmh_plugin_display';
        //$view->display['views_oai_pmh_biblio_2']->handler  = $view->display['default']->handler;
        //$this->view->current_display = 'views_oai_pmh_ag';
    //}
        
    $row_plugins = views_fetch_plugin_names('row', $this->get_style_type(), array($view->base_table));
    $default_row_plugin = key($row_plugins);
    if ($this->options['row_plugin'] == '') {
      $this->options['row_plugin'] = $default_row_plugin;
    }
  
    
  }

  /**
   * Return the type of styles we require.
   */
  //anto comment 
  function get_style_type() { 
  
  	 return 'oai-pmh'; 
  	 
 
  }

  function &get_oai_args() { return $this->oai_args; }

  function get_oai_errors() { return $this->oai_errors; }
 
  /**
   * Return the sections that can be defaultable.
   */
  function defaultable_sections($section = NULL) {
    if (in_array($section, array( 'style_plugin', 'items_per_page', 'offset', 'use_pager', 'pager_element',))) {
      return FALSE;
    }

    return parent::defaultable_sections($section);
  }

  /**
   * Define the option for this view.
   */
  function option_definition() {
  	
    $options = parent::option_definition();
    
    if (!isset($this->oai_args)) {
      $this->oai_args = $this->parse_oai_request();
    }
       /*anto added */
	     if  (!isset($this->oai_args['metadataPrefix']))  {
       	     if (isset($this->oai_args['style_plugin'])){
       	          $options['style_plugin']['default'] = $this->oai_args['style_plugin'];
       	          
       	          if ($this->oai_args['style_plugin']=='views_oai_pmh_dc'){ 
       	             $_SESSION['metadataPrefix'] = 'oai_dc';
       	             $options['row_plugin']['default'] = 'dc';}
       	             elseif ($this->oai_args['style_plugin']=='views_oai_pmh_ls'){
       	               $_SESSION['metadataPrefix']= 'ls';
       	               $options['row_plugin']['default'] = 'ls';}
					 else{
						 $_SESSION['metadataPrefix']= 'agrisap';
       	                 $options['row_plugin']['default'] = 'agrisap';
					     //$options['displays'] = array('default' => 'views_display','views_oai_pmh_ag' => 'views_display','views_oai_pmh_biblio' => 'views_display');
						 }   
       	        }
       	          elseif (isset($_SESSION['metadataPrefix'])) {
	       	          switch( $_SESSION['metadataPrefix']) {
					      case 'ls':
					        $options['style_plugin']['default'] = 'views_oai_pmh_ls';
					        $options['row_plugin']['default'] = 'ls';
					        
					        break;
					      case 'oai_dc':
					     
					        $options['style_plugin']['default'] = 'views_oai_pmh_dc';
					        $options['row_plugin']['default'] = 'dc';
					        
					        break;
						 case 'agrisap':
						    

					        $options['style_plugin']['default'] = 'views_oai_pmh_agrisap';
					        $options['row_plugin']['default'] = 'agrisap';
					         
					        break;	
	       	                }
       	          }
       }else{ 
		    //anto end added */
			
        $metadataPrefix = isset($this->oai_args['metadataPrefix']) ? $this->oai_args['metadataPrefix'] : 'oai_dc';
		if ($this->oai_args['verb']== 'ListSets')
               { $options['style_plugin']['default'] = 'views_oai_pmh_ls';
	             $options['row_plugin']['default'] = 'ls';
				 $metadataPrefix ='ls';
                }
	    switch($metadataPrefix) {
	      case 'agrisap':
	        $options['style_plugin']['default'] = 'views_oai_pmh_agrisap';
	        $options['row_plugin']['default'] = 'agrisap';
	   
  	
	        //$options['displays'] = array('default' => 'views_display','views_oai_pmh_ag' => 'views_display','views_oai_pmh_biblio' => 'views_display'); 
	      
	        break;
		  case 'ls':
	        $options['style_plugin']['default'] = 'views_oai_pmh_ls';
	        $options['row_plugin']['default'] = 'ls';
	        
	        //$options['displays'] = array('default' => array('views_oai_pmh_biblio_2')); 
	      
	        break;	
	      case 'oai_dc':
	      default:
	        $options['style_plugin']['default'] = 'views_oai_pmh_dc';
	        $options['row_plugin']['default'] = 'dc';
	        
	        break;
	
	    }
       }
   
   	

    $options['items_per_page'] = array('default' => '0');
  //  $options['style_plugin']['default'] = 'views_oai_pmh_dc';
  //  $options['row_plugin']['default'] = 'dc';

    if (isset($options['defaults']['default']['items_per_page'])) {
      $options['defaults']['default']['items_per_page'] = FALSE;
    }
    $options['displays'] = array('default' => array());
    $options['defaults']['fields']['hide_empty'] = array('default' => TRUE);
    $options['defaults']['hide_empty'] = array('default' => TRUE);
    // Overrides for standard stuff:
    $options['defaults']['default']['style_plugin'] = FALSE;
    $options['defaults']['default']['style_options'] = FALSE;
    $options['defaults']['default']['row_plugin'] = FALSE;
    $options['defaults']['default']['row_options'] = FALSE;
	//anto $options['defaults']['default']['display'] = TRUE;
	$options['defaults']['default']['display'] = FALSE;
    
    return $options;
  }

  /**
   * Provide the summary for page options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    // It is very important to call the parent function here:
    parent::options_summary($categories, $options);

    $categories['page']['title'] = t('OAI PMH settings');

  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_form($form, $form_state);

  }

  /**
   * Save the options from the options form.
   */


   /**
   * Execute this display handler.
   *
   */
  function execute() {
    if ($this->oai_compression) {
      ob_start('ob_gzhandler');
    }
    //$tag="test";
		
    drupal_add_http_header('Content-Type', 'application/xml');
	
      
    print $this->view->render();

    if ($this->oai_compression) {
      ob_end_flush();
    }

  }


  /**
   * Render the display.
  */
  function render() {
	  
	 
    return $this->view->style_plugin->render();
  }


  function query() {
  	 if (isset($this->oai_args['errors'])) {
    if (count($this->oai_args['errors'])) {
      $this->view->executed = TRUE;
      return parent::query();
    }
  	 }
    if(isset($_GET['resumptionToken'])) {
    	if(isset($this->oai_args['query'])) {
      
      $this->view->query = $this->oai_args['query'];
      $this->view->query->pager->set_current_page($this->oai_args['current_page']+1 );
    } }
    elseif (count($this->oai_args['errors'])) {
      $this->view->executed = TRUE;
    }
    else {
      $verb = $this->oai_args['verb'];
      if ($this->view->name=="oai_listsets"){
         $verb = "ListSets";}
      if ($verb != "ListSets"){
      $this->view->query->add_field('node', 'nid');
      $this->view->query->add_field('node', 'changed');
      $this->view->query->add_orderby('node', 'nid', 'asc');
      if ($verb == 'GetRecord' && isset($this->oai_args['nid'])) {
        $this->view->query->add_where(0, 'node.nid', $this->oai_args['nid'], '=');
      }
      if ($verb == 'ListIdentifiers' ||  $verb == 'ListRecords') {
        if (isset($this->oai_args['from'])) {
          $from = strtotime($this->oai_args['from']);
          $this->view->query->add_where(0, 'node.changed', $from, '>=');
        }
        if (isset($this->oai_args['until'])) {
          $until = strtotime($this->oai_args['until']);
          $this->view->query->add_where(0, 'node.changed', $until, '<=');
        }
      
      }
    }
    }
    
    
    return parent::query();


  }

  function preview() {
    if (!isset( $this->oai_args['verb']) || empty($this->oai_args['verb'])) {
      $this->oai_args['verb'] = 'ListRecords';
    }
    $this->oai_args['response_date'] = gmstrftime('%Y-%m-%dT%H:%M:%SZ',time());
    $this->oai_args['errors'] = array();

    if (!empty($this->view->live_preview)) {
      return '<pre>' . check_plain($this->view->render()) . '</pre>';
    }
    return $this->view->render();
  }

  function parse_oai_request(){

    $args = array();

    if (!empty($_GET)) {
      $args = $_GET;
      $getarr = explode('&', $_SERVER['QUERY_STRING']);
     // if (count($getarr) != count($args)) {
    //    $this->oai_error('sameArgument');
    //  }
    }
    if (!empty($_POST)) {
      $args = $_POST;
    }

    if (isset($args['q'])) unset($args['q']); // get rid of the Drupal q

    if (!count($args)) {
      $this->oai_error( 'badRequestMethod', $_SERVER['REQUEST_METHOD']);
    }

    if (isset($args['verb']) && !count($this->oai_errors)) {
      switch ($args['verb']) {
        case 'GetRecord':
          $this->oai_check_args($args, array('identifier', 'metadataPrefix'));
          $this->oai_check_metadata_prefix($args);
          if(count($this->oai_errors)) return;
          $args['nid'] = substr(strrchr($args['identifier'], ":"), 1);
          break;
        case 'ListIdentifiers':
        case 'ListRecords':
          $args += $this->oai_check_args($args, array('metadataPrefix'), array('from', 'until', 'set'));
          $this->oai_check_metadata_prefix($args);
          break;
        case 'Identify':
          $this->oai_check_args($args);
          $this->oai_compression = FALSE;
          break;
        case 'ListMetadataFormats':
          $this->oai_check_args($args, array(), array('identifier'));
          if(count($this->oai_errors)) return;
          if (isset($args['identifier'])) {
            $args['nid'] = substr(strrchr($args['identifier'], ":"), 1);
          }
          break;
        case 'ListSets':
		  $this->oai_check_args($args);
          //anto $this->oai_error('noSetHierarchy');
        //  $this->oai_resumption_token();
          break;
        //default:
		//anto
          // we never use compression with errors
          //$this->oai_compression = FALSE;
          //$this->oai_error( 'badVerb', $args['verb']);
          //$args['verb'] = '';
      }
    }
    elseif (!count($this->oai_errors) && (!count($args))) {
      $args['verb'] = '';
      $this->oai_error('noVerb');
      $args['response_date'] = gmstrftime('%Y-%m-%dT%H:%M:%SZ',time());
      $args['errors'] = $this->oai_errors;
      
     // return $args;
    }elseif(isset($args['resumptionToken'])){
    	//anto
    	$args += $this->oai_check_args($args);
    	$args['errors']=$this->oai_errors;
    	$args['verb']='ListRecords';
    	//$this->oai_check_args($args);
    	//$this->oai_resumption_token();
    }
    //$args['response_date'] = gmstrftime('%Y-%m-%dT%H:%M:%SZ',time());
    //$args['errors'] = $this->oai_errors;

    return $args;
  }

  function oai_check_identifier($identifier) {
    return db_query("SELECT nid FROM {node} WHERE nid = :id", array(':id' => $identifier))->fetchField();
  }
  function oai_check_args($args, $required = array(), $optional = array()) {

    unset($args['verb']);

    if (isset($args['resumptionToken']) && count($args) > 1) {
      $this->oai_error('exclusiveArgument');
      return array();
    }
    elseif (isset($args['resumptionToken']) && count($args) == 1){
      return $this->get_oai_resumption_token($args['resumptionToken']);;
    }

    if (count($args) < count($required)) {
      foreach ($required as $arg) {
        if (!isset($args[$arg])) {
          $this->oai_error('missingArgument', $arg);
          return array();
        }
      }
    }

    $possible = array_merge($required, $optional);
    $supplied = array_keys($args);
    if (count($bad_args = array_diff($supplied, $possible))) {
      foreach ($bad_args as $arg) {
        $this->oai_error('badArgument', $arg, $args[$arg]);
      }
      return array();
    }

    if (isset($args['from'])) {
      if (! $fromgran = $this->check_date_format($args['from'])) {
        $this->oai_error('badGranularity', 'from', $args['from']);
      }
    }

    if (isset($args['until'])) {
      if (! $untilgran = $this->check_date_format($args['until'])) {
        $this->oai_error('badGranularity', 'until', $args['until']);
      }
    }

    if ((isset($fromgran) && isset($untilgran)) && ($fromgran != $untilgran) ) {
      $this->oai_error('badGranularity', 'mismatched Granularity', $until);
    }

    if (isset($args['identifier'])) {
      $id = substr(strrchr($args['identifier'], ":"), 1);
      if (!$this->oai_check_identifier($id)) {
        $this->oai_error('idDoesNotExist', 'badArgument', $id);
      }
    }


    return array();
  }

  function oai_check_metadata_prefix($args) {
    if (!isset($args['metadataPrefix'])) {
      $this->oai_error('missingArgument', 'metadataPrefix');
    }
    elseif (array_search($args['metadataPrefix'], array('oai_dc', 'agrisap')) === FALSE) {
      $this->oai_error('cannotDisseminateFormat', 'metadataPrefix', $args['metadataPrefix']);
    }
  }

  function oai_error($code, $argument='', $value='')  {

    $this->oai_compression = FALSE;
    $error = array('key' => 'error', 'attributes' => array('code' => $code));
    switch ($code) {
      case 'badArgument' :
        $error += array(
        	'value' => t("The argument @argument (value=@value) included in the request is not valid.",array('@value' => $value, '@argument' => $argument))
        );
        break;
      case 'badGranularity' :
        $error = array(
          'key' => 'error',
        	'attributes' => array('code' => 'badArgument'),
        	'value' => t("The value @value of the argument '@argument' is not valid.",array('@value' => $value, '@argument' => $argument))
        );
        break;
      case 'badResumptionToken' :
        $error += array('value' => t("The resumptionToken @value does not exist or has already expired.",array('@value' => $value)));
        break;
      case 'badRequestMethod' :
        $error = array(
        	'key' => 'error',
        	'attributes' => array('code' => 'badVerb'),
        	'value' => t("The request method @argument is unknown.",array('@argument' => $argument))
        );
        break;
      case 'badVerb' :
        $error += array('value' => t("The verb @argument provided in the request is illegal.",array('@argument' => $argument)));
        break;
      case 'cannotDisseminateFormat' :
        $error += array('value' => t("The metadata format @value given by @argument is not supported by this repository.",array('@value' => $value, '@argument' => $argument)));
        break;
      case 'exclusiveArgument' :
        $error = array(
        	'key' => 'error',
        	'attributes' => array('code' => 'badArgument'),
        	'value' => t('The usage of resumptionToken as an argument allows no other arguments.')
        );
        break;
      case 'idDoesNotExist' :
        $error += array('value' => t("The value @value of the identifier is illegal for this repository.",array('@value' => $value)));
        break;
      case 'missingArgument' :
        $error = array(
        	'key' => 'error',
        	'attributes' => array('code' => 'badArgument'),
        	'value' => t("The required argument @argument is missing in the request.",array('@argument' => $argument))
        );
        break;
      case 'noRecordsMatch' :
        $error += array('value' => t('The combination of the given values results in an empty list.'));
        break;
      case 'noMetadataFormats' :
        $error += array('value' => t('There are no metadata formats available for the specified item.'));
        break;
      case 'noVerb' :
        $error = array(
        	'key' => 'error',
        	'attributes' => array('code' => 'badVerb'),
        	'value' => t('The request does not provide any verb.')
        );
         break;
      case 'noSetHierarchy' :
        $error += array('value' => t('This repository does not support sets.'));
        break;
      case 'sameArgument' :
        $error = array(
        	'key' => 'error',
        	'attributes' => array('code' => 'badArgument'),
        	'value' => t('Do not use them same argument more than once.')
        );
        break;
      case 'sameVerb' :
        $error = array(
        	'key' => 'error',
        	'attributes' => array('code' => 'badVerb'),
        	'value' => t('Do not use verb more than once.')
        );
        break;
      default:
        $error = array(
        	'key' => 'error',
        	'attributes' => array('code' => 'badArgument'),
        	'value' => t("Unknown error: code: @code, argument: @argument, value: @value",array('@value' => $value, '@argument' => $argument,'@code' => $code))
        );
    }
    $this->oai_errors[] = $error;
    return $this->oai_errors;
  }

  function build_oai_resumption_token() {
    if (count($this->oai_errors)) return;

    $items_per_page = $this->view->query->pager->get_items_per_page();
    $current_page = $this->view->query->pager->get_current_page();
    $total_items = $this->view->query->pager->get_total_items();
    $total_pages = $this->view->query->pager->get_pager_total();
    $cursor = ($items_per_page * $current_page);
    $token = ($current_page == $total_pages - 1) ? NULL : md5(uniqid());

    $token_expire_time = gmstrftime('%Y-%m-%dT%H:%M:%SZ', time()+24*3600);
    $token_attrs = array(
                     'completeListSize' => $total_items,
                     'cursor' => $cursor);
    if ($token) {
      $token_attrs['expirationDate'] = $token_expire_time;
      $data = array(
        'cursor' => $cursor,
        'total_items' => $total_items,
        'current_page' => $current_page,
        'metadataPrefix' => $this->oai_args['metadataPrefix'],
        'query' => $this->view->query,
      );
      $fields = array(
        'serialized' => 1,
        'created' => REQUEST_TIME,
        'expire' => time() + OAI_TOKEN_LIFETIME,
        'data' => serialize($data),
        'cid' => $token
      );
      // doing my own insert here because cache_set was silently eating
      // max_allowed_packet errors and dropping cache inserts.  http://drupal.org/node/542874
      // changing the my.ini setting fixed the error, but I would like users to know about the error.
      //
      // cache_set($token, $cache, 'cache_views_oai_pmh', time()+24*3600);
      db_insert('cache_views_oai_pmh')
          ->fields($fields)
          ->execute();
    }
    return format_xml_elements(array(array('key' => 'resumptionToken',
                   'attributes' => $token_attrs,
                   'value' => $token)));
  }

  function get_oai_resumption_token($token) {
    $cache = cache_get($token, 'cache_views_oai_pmh');
    if (isset($cache->data) && is_array($cache->data)) {
      return $cache->data;
    }
    else {
      $this->oai_error('badResumptionToken', '', $token);
      return array();
    }
  }

  function check_date_format($date) {
    $checkstr1 = '([0-9]{4})-([0-9]{1,2})-([0-9]{1,2})T([0-9]{2}):([0-9]{2}):([0-9]{2})Z$';
    $checkstr2 = '([0-9]{4})-([0-9]{1,2})-([0-9]{1,2}$)';
    if (ereg($checkstr1, $date, $regs)) {
      if (checkdate($regs[2], $regs[3], $regs[1])) {
        return 1;
      }
    }
    elseif (ereg($checkstr2, $date, $regs)) {
      if (checkdate($regs[2], $regs[3], $regs[1])) {
        return 2;
      }
    }
    return FALSE;
  }

 }

