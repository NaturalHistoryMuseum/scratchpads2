<?php

/**
 * @file
 * Creative Commons Drupal module
 *   Allows content within a site or attached to a node to
 *   be assigned a Creative Commons license.
 *   http://creativecommons.org/license/
 *
 *
 * By: Peter Bull <pbull@ltc.org>
 * 2005-02-28 / digitalbicycle.org / ltc.org
 * This software is released under the terms of the LGPL license, relicensed
 * under the GPL for drupal.org
 *
 * Utilizes code and inspiration from http://cclicense.sourceforge.net/
 *   Originally released by Blake Watters <sbw@ibiblio.org>
 *   under the terms of the LGPL license (now, GPL for drupal.org).
 *
 */

//TODO: 2.x PHP5 (useful for license types? http://us3.php.net/manual/en/language.oop5.late-static-bindings.php)
//TODO: error handling http://api.creativecommons.org/docs/readme_15.html#error-handling
//TODO: 2.x optimize by storing values when functions are called (e.g. is_valid, is_available)?
//TODO: Move Drupal default metadata checking form get_html to separate function, so that the Views API and other functions can use it too (e.g. user_load, node_load stuff)
class creativecommons_license {
  // license attributes
  var $uri;
  var $name;
  var $license_class;
  var $type;
  var $permissions;
  var $metadata;

  // assigned license
  var $nid;


  /**
   * Initialize object
   */
  function __construct($license_uri, $metadata = array()) {
    $this->uri = $license_uri;
    $this->metadata = $metadata;

    // Fetch license information if uri present
    if ($this->uri) {
      $this->fetch();
    }
    // don't fetch a blank license
    else {
      $this->name = t('None (All Rights Reserved)');
      $this->type = '';
    }

    if (!is_array($this->metadata)) {
      // ensure metadata is an array
      $this->metadata = array();
    }

    if (empty($this->metadata['description'])) {
      $this->metadata['description'] = '';
    }
  }


  /**
   * Load from database into object.
   */
  static function for_node($nid) {
    
    $result = db_query("SELECT * FROM {creativecommons_node} cc WHERE cc.nid = :nid", array(':nid' => $nid));
    
    foreach ($result as $row) {    
    
      $uri = $row->license_uri;

      // Load metadata
      $metadata = array();
      foreach ($row as $key => $value) {
        if ($key != 'license_uri' && $key != 'nid') {
          $metadata[$key] = $value;
        }
      }
      $cc = new creativecommons_license($uri, $metadata);
      $cc->nid = $row->nid;
      return $cc;
    }
    return;
  }

  /**
   * Load basic information from uri and XML data from API into object.
   */
  function fetch() {
    // Load basic data from uri
    $uri_parts = explode('/', $this->uri);
    $this->license_class = $uri_parts[3] == 'licenses' ? 'standard' : $uri_parts[3];
    $this->type = $uri_parts[4];
    $this->version = $uri_parts[5];
    $this->jurisdiction = $uri_parts[6];

    // Get license xml from API
    $xml = creativecommons_get_xml('/details?license-uri='. urlencode($this->uri));

    // Parse XML
    $parser = xml_parser_create();
    xml_parser_set_option($parser, XML_OPTION_SKIP_WHITE, 1);
    xml_parser_set_option($parser, XML_OPTION_CASE_FOLDING, 0);
    xml_parse_into_struct($parser, $xml, $values, $tags);
    xml_parser_free($parser);

    // Extract values
    $this->permissions = array();
    $this->permissions['requires'] = array();
    $this->permissions['prohibits'] = array();
    $this->permissions['permits'] = array();

    foreach ($values as $xn) {
      switch ($xn['tag']) {
        case 'error':
          if ($xn['type'] == 'open') {
            $this->error = array();
            $this->error['id'] = $values[1]['value'];
            $this->error['message'] = $values[2]['value'];
            //TODO: should this set the error here?
            $t_args = array(
              '@error-id' => $this->error['id'],
              '%error-message' => $this->error['message'] . ($this->error['id'] == 'invalid' ? ' '. $this->get_name() : ''),
            );
            $message = t('CC API Error (@error-id): %error-message', $t_args);
            drupal_set_message($message, 'error');

          }
        break;

        case 'license-name':
          $this->name = $xn['value'];
          break;

        case 'rdf:RDF':
          if ($xn['type'] == 'open') {
            $this->rdf = array();
            $this->rdf['attributes'] = $xn['attributes'];
          }
          break;

        //TODO: 2.x remove when RDF/XML support is dropped, this creates redundancy, violates DRY
        case 'permits':
        case 'prohibits':
        case 'requires':
          if (!in_array(current($xn['attributes']), $this->permissions[$xn['tag']]))
            $this->permissions[$xn['tag']][] = current($xn['attributes']);
          break;
      }
    }
  }

  /**
   * Sanitize values and check keys. If key is valid metadata
   * type, sanitize the value. Otherwise, unset it. After running this function,
   * all metadata should be safe for output in HTML.
   */
  function check_metadata() {
    if ($this->metadata) {
      $metadata_types = creativecommons_get_metadata_types();
      foreach ($this->metadata as $key => $value) {
        if (array_key_exists($key, $metadata_types)) {
          $this->metadata[$key] = check_plain($value);
        }
        else {
          unset($this->metadata[$key]);
        }
      }
    }
  }

  /**
   * Return full license name.
   */
  function get_name($style = 'full') {
    if ($this->is_valid()) {
      // CCO
      $prefix = ($this->type && $this->license_class != 'publicdomain') ? t('Creative Commons') .' ' : '';

      switch ($style) {
        case 'full':
          $name = $prefix . $this->name;
          break;
        case 'generic':
          $name = creativecommons_generic_license_name($prefix . $this->name);
          break;
        case 'short':
          //TODO: t() ?
          switch ($this->type) {
            case 'zero':
              $name = t('CC0');
              break;
            case 'mark':
              $name = t('PDM');
              break;
            case 'publicdomain';
              $name = 'PD';
              break;
            default:
              $name = 'CC '. drupal_strtoupper($this->type);
          }
          break;
      }

      return $name;
    }
    else {
      return '"'. $this->uri .'"';
    }
  }


  /**
   * Return array of images relating to current license
   * - if ($site_license) then force return of standard license image
   * @param $style -- either button_large, button_small, icons or tiny_icons
   */
  function get_image($style) {

    if (empty($this->type)) {
      $this->type = 'all-rights-reserved';
    }

    $img = array();
    $img_dir = base_path() . drupal_get_path('module', 'creativecommons') .'/images';
    $nc = variable_get('creativecommons_nc_img', '');

    switch ($style) {
      case 'button_large':
        //TODO: missing jp large buttons... not on creativecommons.org
        if ($nc && $nc != 'jp' && strpos($this->type, 'nc') !== FALSE) {
          $filename = $this->type .'.'. $nc;
        }
      case 'button_small':
        // The directory which the icons reside
        $dir = $img_dir .'/'. str_replace('_', 's/', $style) .'/';

        $img[] = '<img src="'. $dir . (isset($filename) ? $filename : $this->type) .'.png" style="border-width: 0pt;" title="'. $this->get_name('full') .'" alt="'. $this->get_name('full') .'"/>';
        break;
      case 'tiny_icons':
        $px = '15';
      case 'icons':
        $name = array(
          'all-rights-reserved' => t('All Rights Reserved'),
          'by' => t('Attribution'),
          'nc' => t('Noncommercial'),
          'nc-eu' => t('Noncommercial (Euro)'),
          'nc-jp' => t('Noncommercial (Yen)'),
          'sa' => t('Share Alike'),
          'nd' => t('No Derivatives'),
          'pd' => t('Public Domain'),
          'mark' => t('Public Domain Mark'),
          'zero' => t('Zero'),
        );
        if (!isset($px)) {
          $px = '32';
        }
        
        //explode to add euro or yen icons
        foreach (explode('-', $this->type) as $filename) {
          
          // NC options
          if ($filename == 'nc' && $nc) {
            $filename .= '-'. $nc;
          }
          
          if($filename == 'rights') {
            $filename = $this->type;
          }
          
          //quick fix for #957584... this entire function needs love
          if ($filename != 'all' && $filename != 'reserved') {
            $img[] = '<img src="'. $img_dir .'/icons/'. $filename .'.png" style="border-width: 0pt; width: '. $px .'px; height: '. $px .'px;" alt="'. $name[$filename] .'"/>';
          }
        }
        break;
    }

    return implode(($style == 'tiny_icons' ? '' : ' '), $img);
  }

  /**
   * Returns true if any metadata fields are non-blank, false otherwise.
   */
  function has_metadata() {
    if ($this->metadata) {
      foreach ($this->metadata as $key => $value) {
        if (!empty($value)) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  /**
   * Returns true if any available metadata fields are non-blank, false otherwise.
   */
  function has_available_metadata() {
    if ($this->metadata) {
      foreach ($this->metadata as $key => $value) {
        if (!empty($value) && creativecommons_metadata_is_available($key)) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  /**
   * Returns true if license set, false otherwise.
   */
  function has_license() {
    return !empty($this->uri);
  }

  /**
   * Returns true if license uri is valid, false otherwise.
   */
  function is_valid() {
    // note: license xml was already extracted in constructor
    return !(isset($this->error) && $this->error['id'] == 'invalid');
  }

  /**
   * Returns true if license is available, false otherwise.
   * A license is available if it has a valid uri and if its license type is
   * available. Blank licenses are 'available'.
   */
  function is_available() {
    // Check if license is valid
    if (!$this->is_valid())
      return FALSE;

    // Check if license type is available
    $available_license_types = creativecommons_get_available_license_types();
    if (!in_array($this->type, $available_license_types))
      return FALSE;

    return TRUE;
  }

  /**
   *
   * @return string
   *   error message for invalid or disabled license
   */
  function get_license_unavailable_message() {
    $args = array('@license-name' => $this->get_name());
    if ($this->is_valid()) {
      return t('The license "@license-name" is not enabled.', $args);
    }
    else {
      return t('"@license-name" is not a valid license.', $args);
    }
  }

  /**
   * Return true if this license allows commercial use, false otherwise.
   */
  function permits_commercial_use() {
    return !is_array($this->permissions['prohibits']) || !in_array('http://creativecommons.org/ns#CommercialUse', $this->permissions['prohibits']);
  }

  /**
   * Return true if this license allows derivative works, false otherwise.
   */
  function permits_derivative_works() {
    return is_array($this->permissions['permits']) && in_array('http://creativecommons.org/ns#DerivativeWorks', $this->permissions['permits']);
  }

  /**
   * Return html containing license link (+ images)
   */
  //TODO: missing dc:creator...
  //TODO: "use an appropriate profile URL in the header of the HTML document" (p. 15, ccREL)
  function get_html($site = FALSE) {

    // Load additional info, used when defaulting to Drupal metadata
    if ($site) {
      $default_title = variable_get('site_name', '');
      $default_attributionURL = url('<front>', array('absolute' => TRUE));
    }
    else {


      $node = node_load($this->nid);
      $user = user_load($node->uid);
      $default_title = $node->title;
      $default_name = $user->name;
      $default_attributionURL = url('user/'. $user->uid, array('absolute' => TRUE));
    }

    $html = "\n<div about=\"". url(($site ? '<front>' : 'node/'. $this->nid), array('absolute' => TRUE)) ."\" instanceof=\"cc:Work\"".
              "\n\txmlns:cc=\"http://creativecommons.org/ns#\"".
              "\n\txmlns:dc=\"http://purl.org/dc/elements/1.1/\"".
              "\n\tclass=\"creativecommons\">\n\t\t";

    // Image

    $img = $this->get_image(variable_get('creativecommons_image_style', 'button_large'));
    if ($img) {
      $attributes['rel'] = 'license';
      $html .= l($img, $this->uri, array('attributes' => $attributes, 'html' => TRUE)) .'<br/>';
    }

    // Main
    $dcmi_types = creativecommons_get_dcmi_types();
    $args = array(
      '@license-name' => $this->get_name(variable_get('creativecommons_text_style', 'full')),
      '@license-uri' => $this->uri,);
      
      //TODO: fix $this->metadata
      /*

      '@dc:type-uri' => 'http://purl.org/dc/dcmitype/'. ($this->metadata['type'] && creativecommons_metadata_is_available('type') ? $this->metadata['type'] : ''),
      '@dc:type-name' => $this->metadata['type'] && creativecommons_metadata_is_available('type') ? drupal_strtolower($dcmi_types[$this->metadata['type']]) : t('Work'),
      '@dc:title' => $this->metadata['title'] && creativecommons_metadata_is_available('title') ? $this->metadata['title'] : $default_title,
      '@cc:attributionName' => $this->metadata['attributionName'] && creativecommons_metadata_is_available('attributionName') ? $this->metadata['attributionName'] : $default_name,
      '@cc:attributionURL' => $this->metadata['attributionURL'] && creativecommons_metadata_is_available('attributionURL') ? $this->metadata['attributionURL'] : $default_attributionURL,
    );
       
*/
    // All rights reserved
    if ($this->type == '') {
      // None (All Rights Reserved)
      $img = $this->get_image(variable_get('creativecommons_image_style', 'button_large'));
      $html .= check_markup(variable_get('creativecommons_arr_text', NULL));
    }
    // Site license, no attribution name
    else if ($site && (!isset($this->metadata['attributionName']) || !creativecommons_metadata_is_available('attributionName'))) {
      // CC0
      if ($this->type == 'zero') {
        $html .= t('To the extent possible under law, the person who associated CC0 with this work, <a rel="cc:attributionURL" href="@cc:attributionURL" property="dc:title">@dc:title</a>, has waived all copyright and related or neighboring rights to this <span rel="dc:type" href="@dc:type-uri">@dc:type-name</span>, although certain works referenced herein may be separately licensed.', $args);
      }
      // PD Mark
      else if ($this->type == 'mark') {
        $html .= t('This <span rel="dc:type" href="@dc:type-uri">@dc:type-name</span>, <a rel="cc:attributionURL" href="@cc:attributionURL" property="dc:title">@dc:title</a>, is free of known copyright restrictions.', $args);
      }
      // Rest
      else {
        $html .= t('This <span rel="dc:type" href="@dc:type-uri">@dc:type-name</span>, <a rel="cc:attributionURL" href="@cc:attributionURL" property="dc:title">@dc:title</a>, is licensed under a <a rel="license" href="@license-uri">@license-name license</a>, although certain works referenced herein may be separately licensed.', $args);
      }
    }
    // Otherwise
    else {
      // CC0
      if ($this->type == 'zero') {
        $html .= t('To the extent possible under law, <a rel="cc:attributionURL" href="@cc:attributionURL" property="cc:attributionName">@cc:attributionName</a> has waived all copyright and related or neighboring rights to this <span rel="dc:type" href="@dc:type-uri">@dc:type-name</span>, <span property="dc:title">@dc:title</span>.', $args);
      }
      // PD Mark
      else if ($this->type == 'mark') {
        $html .= t('This <span rel="dc:type" href="@dc:type-uri">@dc:type-name</span>, <span property="dc:title">@dc:title</span>, by <a rel="cc:attributionURL" href="@cc:attributionURL" property="cc:attributionName">@cc:attributionName</a>, is free of known copyright restrictions.', $args);
      }
      // All Rights Reserved
      else if ($this->type == 'all-rights-reserved') {
        $html .= t('To the extent possible under law, all copyright and related or neighboring rights to this <span rel="dc:type" href="@dc:type-uri">@dc:type-name</span>, <span property="dc:title">@dc:title</span>, by @cc:attributionName are Reserved.', $args);
      }
      // Rest
      else {
        $html .= t('This <span rel="dc:type" href="@dc:type-uri">@dc:type-name</span>, <span property="dc:title">@dc:title</span>, by <a rel="cc:attributionURL" href="@cc:attributionURL" property="cc:attributionName">@cc:attributionName</a> is licensed under a <a rel="license" href="@license-uri">@license-name license</a>.', $args);
      }
    }
   
   
    if(FALSE) {
    // dc:source
    if ($this->metadata['source'] && creativecommons_metadata_is_available('source')) {
      $html .= '<span rel="dc:source" href="'. check_plain($this->metadata['source']) .'"/>';
    }

    // CC+
    if ($this->metadata['morePermissions'] && creativecommons_metadata_is_available('morePermissions')) {
      $html .= ' '. t('There are <a rel="cc:morePermissions" href="@cc:morePermissions">alternative licensing options</a> available.', array('@cc:morePermissions' => $this->metadata['morePermissions']));
    }

    // dc:description
    if ($this->metadata['description'] && creativecommons_metadata_is_available('description')) {
      $html .= '<p>'. t('<span property="dc:description">@dc:description</span>', array('@dc:description' => $this->metadata['description'])) .'</p>';
    }

    // dc:year
    if (($this->metadata['date'] && creativecommons_metadata_is_available('date')) || ($this->metadata['rights'] && creativecommons_metadata_is_available('date'))) {
      $args = array(
        '@dc:date' => creativecommons_metadata_is_available('date') ? $this->metadata['date'] : '',
        '@dc:rights' => creativecommons_metadata_is_available('rights') ? $this->metadata['rights'] : '',
      );
      $html .= '<p>'. t('Copyright &copy; <span property="dc:date">@dc:date</span> <span property="dc:rights">@dc:rights</span>', $args) .'</p>';
    }

    }

    $html .= "\n</div>\n";
    return $html;
  }


  /**
   * Return rdf with license and metadata embedded
   */
  function get_rdf() {

    // must have a license to display rdf
    if (!$this->has_license())
      return;

    // Sanitize metadata
    $this->check_metadata();

    if ($this->rdf) {
      $a = '';
      foreach ($this->rdf['attributes'] as $attr => $val) {
        $a .= " $attr=\"$val\"";
      }
    }
    $rdf = "<rdf:RDF$a>\n";

    // metadata
    $rdf .= "<work rdf:about=\"". url('node/'. $this->nid, array('absolute' => TRUE)) ."\">\n";
    if ($this->has_available_metadata()) {
      foreach ($this->metadata as $key => $value) {
        if ($value && creativecommons_metadata_is_available($key)) {
          $ns = 'dc';

          switch ($key) {
            case 'type':
              $value = "http://purl.org/dc/dcmitype/$value";

            case 'source':
              $rdf .= "<$ns:$key rdf:resource=\"$value\" />\n";
              break;

            case 'rights':
            case 'creator':
              $rdf .= "<$ns:$key><agent><dc:title>$value</dc:title></agent></$ns:$key>\n";
              break;

            case 'attributionName':
            case 'attributionURL':
            case 'morePermissions':
              $ns = 'cc';
            default:
              $rdf .= "<$ns:$key>$value</$ns:$key>\n";
              break;
          }
        }
      }
    }
    $rdf .= "<license rdf:resource=\"". $this->uri ."\" />\n";
    $rdf .= "</Work>\n";

    // permissions
    $rdf .= "<license rdf:about=\"". $this->uri ."\">\n";
    if ($this->permissions) {
      foreach ($this->permissions as $name => $perm) {
        foreach ($perm as $v) {
          $rdf .= "<$name rdf:resource=\"$v\" />\n";
        }
      }
    }

    $rdf .= "</license>\n";
    $rdf .= "</rdf:RDF>";
    return $rdf;
  }


  /**
   * Save to the database.
   * @param $nid - node id
   * @param $op - either 'insert' or 'update'
   */
  function save($nid, $op) {
    $result = NULL;
    if (!$nid) {
      drupal_set_message(t('CC license could not be saved due to missing node argument.'), 'error');
      return;
    }

    if (!$this->is_available()) {
      // note in log, but only once
      $message = $this->get_license_unavailable_message();
      drupal_set_message($message, 'error', FALSE);
      // continue, since the availability is to the user, not to code.
      // if this code got called, there was a reason.  user validation
      // happens elsewhere.
    }

    switch ($op) {
      case 'update':
      //dsm('really going to update');
        // This check exists in case an entry doesn't yet exist for the node
        // (for example, if a node was created before the CC module was
        // setup for that content type)
        //$exists = FALSE;
        //$check_result = db_query('SELECT COUNT(*) as count FROM {creativecommons_node} WHERE nid=:nid', array(':nid' => $nid))->fetchField();;
        /*

        if ($check_result->count == 1) {
          $row = db_fetch_object($check_result);
          if ($row->count == 1) {
            $exists = TRUE;
          }
        }
        

        if ($check_result->count) {
          $args = array($this->uri);
          $fields = array("license_uri='%s'");
          foreach ($this->metadata as $key => $value) {
            $fields[] = $key ."='%s'";
            $args[] = $value;
          }
          */
          //$args[] = $nid;
          //$query = 'UPDATE {creativecommons_node} SET '. implode(', ', $fields) .' WHERE nid=%d';
          
          $num_updated = db_update('creativecommons_node') // Table name no longer needs {}
         ->fields(array('license_uri' => $this->uri))
         ->condition('nid', $nid, '=')
         ->execute();
          
          //$args = array_merge(array($query), $args);
          //$result = call_user_func_array('db_query', $args);
          
        //}
        break;
      // otherwise, insert
      case 'insert':
        //dsm('for some reason, doing an insert');
        $args = array($nid, $this->uri);
        $fields = array('nid', 'license_uri');
        $values = array('%d', "'%s'");
        foreach ($this->metadata as $key => $value) {
          $fields[] = $key;
          $values[] = "'%s'";
          $args[] = $value;
        }
        
        //$query = 'INSERT INTO {creativecommons_node} ('. implode(', ', $fields) .') VALUES ('. implode(', ', $values) .')';
        
        $nid = db_insert('creativecommons_node') // Table name no longer needs {}
        ->fields(array(
        'nid' => $nid,
        'license_uri' => $this->uri,
        'description' => '',
        ))
        ->execute();


        //$args = array_merge(array($query), $args);
        //$result = call_user_func_array('db_query', $args);
        break;
    }
    //TODO: check for error here?
    return $result;
  }
}

