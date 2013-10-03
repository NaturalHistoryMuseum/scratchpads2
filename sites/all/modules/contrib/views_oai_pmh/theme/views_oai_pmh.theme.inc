<?php

/**
 * @file
 * Theme related functions for processing our output style plugins.
 *
 * Views bug: http://drupal.org/node/593336
 */
//agrisap

function theme_views_oai_pmh_row_agrisap_fields(&$vars) {
  $view = $vars['view'];
  $language_def= language_default();
  // Loop through the fields for this view.
  $previous_inline = FALSE;
  $vars['fields'] = array(); // ensure it's at least an empty array.
  $row_index = $view->row_index;
  $iLanAbs=0;
  $iLanAlt=0;
  $iLanLan=0;
  
  foreach ($view->field as $id => $field) {
	  $item['attributes']=array();
	  $sub['attributes']=array();
	  $sub=array();
	  
    // render this even if set to exclude so it can be used elsewhere.
    $field_output = $view->style_plugin->get_field($row_index, $id);
	$value=$field_output;
	$nid=$view->result[$row_index]->nid;
    $empty = $field_output !== 0 && empty($field_output);
	
    if (empty($field->options['exclude']) && (!$empty || (empty($field->options['hide_empty']) && empty($vars['options']['hide_empty'])))) {
      $key = check_plain(_views_oai_pmh_xml_tag_clean($view->field[$id]->label()));
      if (($field->multiple==TRUE)) {
         $separator = $field->options['separator'];
        foreach (explode($separator, $field_output) as $value) {
         	 // VP: add a switch for nested elements and attributes (FOR MULTIPLE FIELDS)
					// example with ags:subjectThesaurus
					switch ($key) {
					 
					  case "ags:creatorPersonal":
					  case "ags:creatorCorporate":
					  case "ags:creatorConference":
                        // $sub: array for the nested element
						$sub['key'] = $key;
						$sub['value'] = $value;
						// array for the containing element
						$item['key'] = "dc:creator";
						$item['value'] = array('0' => $sub); // the value can contain an array (for the nested element)
						$items[] = $item;
						break;
				     case "ags:publisherName":
					 case "ags:publisherPlace":
					 		// $sub: array for the nested element
						$sub['key'] = $key;
						$sub['value'] = $value;
						// array for the containing element
						$item['key'] = "dc:publisher";
						$item['value'] = array('0' => $sub); // the value can contain an array (for the nested element)
						$items[] = $item;	
						break;
					 case "ags:agrovoc_uri":
                        $keyUri="ags:subjectThesaurus";
					 	$uri="http://aims.fao.org/aos/agrovoc/c_".$value;
						$sub['key'] = $keyUri;
						$sub['value'] = $uri;
						$sub['attributes'] = array('scheme'=>'ags:AGROVOC');
						// array for the containing element
						$item['key'] = "dc:subject";
						$item['value'] = array('0' => $sub); // the value can contain an array (for the nested element)
						$items[] = $item;
						break;	
					 case "ags:subjectThesaurus":
                        					
						$query ="SELECT `tid` FROM `taxonomy_term_data` WHERE name =:name and vid=16" ;
						$tid= db_query($query,array(':name' => $value))->fetchField();
						$query="select f.language as language,CAST(f.translation AS CHAR(1000) CHARACTER SET utf8) as name  from locales_target f, locales_source s , taxonomy_term_data t where s.location like '%".$tid."%' and s.lid=f.lid and t.tid =:tid and vid=16";
						$resultAgr= db_query($query,array(':tid' => $tid));
						     $sub['key'] = $key;
							$sub['value'] = $value;
							$sub['attributes'] = array('scheme'=>'ags:AGROVOC','xml:lang'=>$language_def->language);
							// array for the containing element
							$item['key'] = "dc:subject";
							$item['value'] = array('0' => $sub); // the value can contain an array (for the nested element)
							$items[] = $item;
                        foreach($resultAgr as $rec ){
							$sub['key'] = $key;
							$sub['value'] = $rec->name;
							$sub['attributes'] = array('scheme'=>'ags:AGROVOC','xml:lang'=>$rec->language);
							// array for the containing element
							$item['key'] = "dc:subject";
							$item['value'] = array('0' => $sub); // the value can contain an array (for the nested element)
							$items[] = $item;
					    }
						break;	
				     case "ags:subjectClassification":
					 		// $sub: array for the nested element
						$sub['key'] = $key;
						$sub['value'] = $value;
						$sub['attributes'] = array('scheme'=>'ags:ASC');
						 // array for the containing element
						$item['key'] = "dc:subject";
						$item['value'] = array('0' => $sub); // the value can contain an array (for the nested element)
						$items[] = $item;
						break;
					 case "dcterms:abstract":
					    // $sub: array for the nested element
						$lan="";
						$sub['key'] = $key;
						$sub['value'] = $value;
						// add language
						$nid=$view->result[$row_index]->nid;
						$query ="SELECT `field_ag_langtext_get_list_from_file` FROM `field_data_field_ag_langtext`  WHERE entity_id =:nid and delta=:delta";
                        $lan= db_query($query,array(':nid' => $nid, ':delta' => $iLanAbs))->fetchField();
                        $iLanAbs++;
						$sub['attributes'] = array('xml:lang'=>$lan);//put the language of abstract but it is not a view field
						
						 // array for the containing element
						$item['key'] = "dc:description";
						$item['value'] = array('0' => $sub); // the value can contain an array (for the nested element)
						$items[] = $item;
						break;
					 case "dcterms:alternative":
					   
						$sub['key'] = $key;
						$sub['value'] = $value; // the value can contain an array (for the nested element)
						$query ="SELECT `field_ag_langsubt_get_list_from_file` FROM `field_data_field_ag_langsubt` WHERE entity_id =:nid and delta=:delta" ;
                        $lan= db_query($query,array(':nid' => $nid, ':delta' => $iLanAlt))->fetchField();
						$iLanAlt++;
						$item['attributes'] = array('xml:lang'=>$lan);//put the language of abstract but it is not a view field
						$item['key'] = "dc:title";
						$item['value'] = array('0' => $sub); // the value can contain an array (for the nested element)
						$items[] = $item;
						break;		
					 case "ags:descriptionNotes":
				     case "ags:descriptionEdition":
					    // $sub: array for the nested element
						$sub['key'] = $key;
						$sub['value'] = $value;
						
						 // array for the containing element
						$item['key'] = "dc:description";
						$item['value'] = array('0' => $sub); // the value can contain an array (for the nested element)
						$items[] = $item;	
						break;		
					 case "ags:availabilityLocation":
					 case "ags:availabilityNumber":
					    // $sub: array for the nested element
						$sub['key'] = $key;
						$sub['value'] = $value;
						// array for the containing element
						$item['key'] = "agls:availability";
						$item['value'] = array('0' => $sub); // the value can contain an array (for the nested element)
						$items[] = $item;	
					 	break;
					 case "ags:citationTitle":
					 case "ags:citationNumber":
					 case "ags:citationChronology":
					  case "ags:citationIdentifier":
						$sub['key'] = $key;
						$sub['value'] = $value;
						if ($key=="ags:citationIdentifier"){
						    $attr['scheme']="ags:ISSN";
						    $sub['attributes']=$attr;
					     }
						// array for the containing element
						$item['key'] = "ags:citation";
						$item['value'] = array('0' => $sub);  // the value can contain an array (for the nested element)
						$items[] = $item;
						break;
					case "dc:language":
						$cstrvalue=trim($value); 
						if (empty($cstrvalue)){
						    break;}
				        $item['key'] = $key;
			            $attr['scheme']="ags:ISO639-1";
			            $item['attributes']=$attr;
				        $query ="SELECT `field_ag_language_get_list_from_file` FROM `field_data_field_ag_language`  WHERE entity_id =:nid and delta=:delta";
                        $lan= db_query($query,array(':nid' => $nid, ':delta' => $iLanLan))->fetchField();
						$iLanLan++;
			            $item['value'] = $lan;
				        $items[] = $item;
				        break;	
					case "dc:identifier":	
				  //if issn or isbn or e-issn
				    $item['key'] = $key;
					if (($field->field =="upload")||($field->field =="field_ag_resource_url")){
					   $item['attributes']= array('scheme'=>'dcterms:URI');
				     }else{
						 $item['attributes']=  array('scheme'=>'ags:ISBN');}
					$item['value'] = $value;
					$items[] = $item;
					break;
						
				// ...	
					 default: // for non-nested elements
							$item['key'] = $key;
							$item['value'] = $value;
							$items[] = $item;
         }
        }
      }
      else {
        // VP: add a switch for nested elements and attributes (FOR SINGLE FIELDS)
  			// example with dcterms:dateIssued
  			
  			switch ($key) {
				   case "dcterms:abstract":
					    // $sub: array for the nested element
						$lan="";
						$sub['key'] = $key;
						$sub['value'] = $value;
						// add language
						$nid=$view->result[$row_index]->nid;
						$query ="SELECT `field_ag_langtext_get_list_from_file` FROM `field_data_field_ag_langtext`  WHERE entity_id =:nid and delta=:delta";
                        $lan= db_query($query,array(':nid' => $nid, ':delta' => $iLanAbs))->fetchField();
                        $iLanAbs++;
						$sub['attributes'] = array('xml:lang'=>$lan);//put the language of abstract but it is not a view field
						
						 // array for the containing element
						$item['key'] = "dc:description";
						$item['value'] = array('0' => $sub); // the value can contain an array (for the nested element)
						$items[] = $item;
						break;
				  case "dcterms:alternative":
					   
						$sub['key'] = $key;
						$sub['value'] = $value; // the value can contain an array (for the nested element)
						$query ="SELECT `field_ag_langsubt_get_list_from_file` FROM `field_data_field_ag_langsubt` WHERE entity_id =:nid and delta=:delta" ;
                        $lan= db_query($query,array(':nid' => $nid, ':delta' => $iLanAlt))->fetchField();
						$iLanAlt++;
						$item['attributes'] = array('xml:lang'=>$lan);//put the language of abstract but it is not a view field
						$item['key'] = "dc:title";
						$item['value'] = array('0' => $sub); // the value can contain an array (for the nested element)
						$items[] = $item;
						break;	
  				  case "ags:descriptionNotes":
				  case "ags:descriptionEdition":
					    // $sub: array for the nested element
						$sub['key'] = $key;
						$sub['value'] = $value;
						
						 // array for the containing element
						$item['key'] = "dc:description";
						$item['value'] = array('0' => $sub); // the value can contain an array (for the nested element)
						$items[] = $item;	
						break;		
  				  	
				case "dcterms:dateIssued":
             // $sub: array for the nested element
					$sub['key'] = $key;
					$year = substr($value,0,4); 
					$month = substr($value,5,2);
					$value=$year."-".$month;
					$sub['value'] = $value;
					$attr['scheme']="dcterms:W3CDTF";
					$sub['attributes']=$attr;
                    // array for the containing element
					$item['key'] = "dc:date";
					$item['value'] = array('0' => $sub);  // the value can contain an array (for the nested element)
					$items[] = $item;
					break;
				
				case "dc:identifier":	
				  //if issn or isbn or e-issn
  			            $cstrvalue=trim($value); 
						if (empty($cstrvalue)){
						    break;}
				    $item['key'] = $key;
					if (($field->field =="upload")||($field->field =="field_ag_resource_url")){
					   $item['attributes']= array('scheme'=>'dcterms:URI');
				     }else{
						 $item['attributes']=  array('scheme'=>'ags:ISBN');}
					$item['value'] = $value;
					$items[] = $item;
					break;
				case "dcterms:extent":
				case "dcterms:medium":
             // $sub: array for the nested element
					$sub['key'] = $key;
					$sub['value'] = $value;
					
                    // array for the containing element
					$item['key'] = "dc:format";
					$item['value'] = array('0' => $sub);  // the value can contain an array (for the nested element)
					$items[] = $item;
					break;
				case "dc:language":
				  
				   $item['key'] = $key;
			       $attr['scheme']="ags:ISO639-1";
			       $item['attributes']=$attr;
				   $query ="SELECT `field_ag_language_get_list_from_file` FROM `field_data_field_ag_language`  WHERE entity_id =:nid";
                   $lan= db_query($query,array(':nid' => $nid))->fetchField();
			       $item['value'] = $lan;
				   $items[] = $item;
				   break;
				case "ags:availabilityLocation":
					 case "ags:availabilityNumber":
					    // $sub: array for the nested element
						$sub['key'] = $key;
						$sub['value'] = $value;
						// array for the containing element
						$item['key'] = "agls:availability";
						$item['value'] = array('0' => $sub); // the value can contain an array (for the nested element)
						$items[] = $item;	
					 	break;
					  case "ags:citationTitle":
					 case "ags:citationNumber":
					 case "ags:citationChronology":
					  case "ags:citationIdentifier":
						$sub['key'] = $key;
						$sub['value'] = $value;
						if ($key=="ags:citationIdentifier"){
						    $attr['scheme']="ags:ISSN";
						    $sub['attributes']=$attr;
					     }
						// array for the containing element
						$item['key'] = "ags:citation";
						$item['value'] = array('0' => $sub);  // the value can contain an array (for the nested element)
						$items[] = $item;
						break;
					
					case "dc:title":
					   
						$item['key'] = $key;
						$item['value'] = $value; // the value can contain an array (for the nested element)
						$query ="SELECT `field_ag_langtitle_get_list_from_file` FROM `field_data_field_ag_langtitle` WHERE entity_id =:nid" ;
                        $lan= db_query($query,array(':nid' => $nid))->fetchField();
						
						$item['attributes'] = array('xml:lang'=>$lan);//put the language of abstract but it is not a view field
						$items[] = $item;
						break;	
			    default: // for non-nested elements
						$item['key'] = $key;
						$item['value'] = $field_output;
						$items[] = $item;
         }	
      }
    }
  }
 return format_oai_xml_nested_elements($items);

}



//listsest
function theme_views_oai_pmh_row_ls_fields(&$vars) {
  $view = $vars['view'];

  // Loop through the fields for this view.
  $previous_inline = FALSE;
  $vars['fields'] = array(); // ensure it's at least an empty array.
  $row_index = $view->row_index;
  foreach ($view->field as $id => $field) {
    // render this even if set to exclude so it can be used elsewhere.
    $field_output = $view->style_plugin->get_field($row_index, $id);
	
    $empty = $field_output !== 0 && empty($field_output);
    if (empty($field->options['exclude']) && (!$empty || (empty($field->options['hide_empty']) && empty($vars['options']['hide_empty'])))) {
      $key = check_plain(_views_oai_pmh_xml_tag_clean($view->field[$id]->label()));
      if (isset($field->multiple)) {
        $separator = $field->options['separator'];
        foreach (explode($separator, $field_output) as $value) {
          $item['key'] = $key;
		  $value=strip_tags($value);
          $item['value'] = trim($value);
          $items[] = $item;
        }
      }
      else {
        $item['key'] = $key;
		$value=strip_tags ($field_output);
        $item['value'] = trim($value);
        $items[] = $item;
      }
    }
  }
 return format_xml_elements($items);

}
function theme_views_oai_pmh_row_dc_fields(&$vars) {
  $view = $vars['view'];

  // Loop through the fields for this view.
  $previous_inline = FALSE;
  $vars['fields'] = array(); // ensure it's at least an empty array.
  
  $row_index = $view->row_index;
  foreach ($view->field as $id => $field) {
    // render this even if set to exclude so it can be used elsewhere.
    $field_output = $view->style_plugin->get_field($row_index, $id);
    $empty = $field_output !== 0 && empty($field_output);
    if (empty($field->options['exclude']) && (!$empty || (empty($field->options['hide_empty']) && empty($vars['options']['hide_empty'])))) {
      $key = check_plain(_views_oai_pmh_xml_tag_clean($view->field[$id]->label()));
      if (isset($field->multiple)) {
        $separator = $field->options['separator'];
        foreach (explode($separator, $field_output) as $value) {
                  $cstrvalue=trim($value); 
				  if (empty($cstrvalue)){
					  break;} 
          $item['key'] = $key;
          $item['value'] = $value;
          $items[] = $item;
        }
      }
      else {
        $item['key'] = $key;
        $item['value'] = $field_output;
        $items[] = $item;
      }
    }
  }
 return format_xml_elements($items);

}

function template_preprocess_views_oai_pmh_response(&$vars) {
  global $base_url;
  $vars['oai_response_date'] = format_xml_elements(array('responseDate' => gmstrftime('%Y-%m-%dT%H:%M:%SZ',time())));
  //$vars['oai_response_date'] = format_xml_elements(array('responseDate' => $vars['oai_args']['response_date']));
  $request = array('key' => 'request',
        					 'value' => $base_url);
  $request += !empty($vars['oai_args']['verb']) ?
                array('attributes' => array('verb' => $vars['oai_args']['verb'])): array();
  $vars['oai_request'] = format_xml_elements(array($request));
  $vars['oai_verb'] = !empty($vars['oai_args']['verb']) ? $vars['oai_args']['verb']: '';
  $vars['oai_base_url'] = $base_url;
  $vars['oai_errors'] = format_xml_elements($vars['oai_args']['errors']);

  $vars['oai_content'] = $vars['oai_content'];
  if (!empty($vars['oai_content'])) {
    $vars['oai_resumption_token'] = $vars['resumption_token'];
  }
}

function theme_views_oai_pmh_identify_body(&$vars) {
  global $base_url;
  $view = $vars['view'];
  $path = $view->display_handler->get_path();
  $datestamp = db_query("SELECT min(changed) FROM {node} ")->fetchField();

  $ident = array(
  	'repositoryName' => variable_get('site_name', 'Views OAI'),
    'baseURL' => $base_url . '/' . $path,
    'protocolVersion' => '2.0',
    'adminEmail' => variable_get('site_mail', ini_get('sendmail_from')),
    'earliestDatestamp' => gmstrftime('%Y-%m-%dT%H:%M:%SZ', $datestamp),
    'deletedRecord' => 'no',
    'granularity' => 'YYYY-MM-DDThh:mm:ssZ',
    'compression' => 'yes',
  );

  return format_xml_elements($ident);
}

function theme_views_oai_pmh_metadataformats_body(&$vars) {
  $formats = '';
  $formats .= format_xml_elements(array ( '0'=> array('key'=> 'metadataFormat' ,'value' => array(
  	'metadataPrefix' => 'oai_dc',
	'schema' => 'http://www.openarchives.org/OAI/2.0/oai_dc.xsd',
    'metadataNamespace' => 'http://www.openarchives.org/OAI/2.0/oai_dc/',
  )),
     //anto added metadataform agris ap but in future it need to be dynamic
     '1'=> array('key'=>'metadataFormat' ,'value' => array(
  	'metadataPrefix' => 'agrisap',
	'schema' => 'http://www.openarchives.org/OAI/2.0/oai_ags/',
    'metadataNamespace' => 'http://purl.org/agmes/1.1/',
  ))));
  return $formats;
}

function theme_views_oai_pmh_record_header($vars) {
  global $base_url;
  static $parts = array();
  if (empty($parts)) {
    $parts = parse_url($base_url);
  }
  $nid = $vars['nid'];
  $datestamp = $vars['node_changed'];

  $info['header']['identifier'] = 'oai:' . $parts['host'] . ':' . $nid;
  $info['header']['datestamp'] = gmstrftime('%Y-%m-%dT%H:%M:%SZ', $datestamp);
//  $info['header']['setSpec'] = '';

  return format_xml_elements($info);
}
//agrisap
function theme_views_oai_pmh_agrisap_record(&$vars) {
  $i=$vars['index'];
  $nid=$vars['view']->result[$i]->nid;
  $datestamp = $vars['view']->result[$i]->node_changed;
  $header = theme('views_oai_pmh_record_header', array('nid' => $nid, 'node_changed' => $datestamp));
  $node=node_load($nid); //VP 20-07-2010: $count instead of 0 as index 
  //VP: for agridrupal: find repository node for center code
 
  
        $query ="SELECT name FROM `users` WHERE uid=". $node->uid .";";
	 	$user=db_query($query)->fetchField();  
	    $rest = substr($user, 0,2);
	    $year = date("Y");
	    $code=substr($user, 2); //VP 20-07-2010: 3 -> 2
		$prog = str_pad($nid, 5, '0', STR_PAD_LEFT); //VP 20-07-2010: $node->nid instead of $prog
	    $arn=$rest.$year.$code.$prog;
    return '<record>' .
    $header .
    '<metadata>
      <ags:resources  xmlns:ags="http://purl.org/agmes/1.1/" 
	              xmlns:dc="http://purl.org/dc/elements/1.1/" 
				  xmlns:agls="http://www.naa.gov.au/recordkeeping/gov_online/agls/1.2" 
				  xmlns:dcterms="http://purl.org/dc/terms/"><ags:resource ags:ARN="'.$arn.'">' .
      $vars['fields'] .
      '</ags:resource></ags:resources>
    </metadata>
   </record>';
}
//listeste
function theme_views_oai_pmh_ls_record(&$vars) {
  $tid = $vars['tid'];
  //$datestamp = $vars['node_changed'];
  //$header = theme('views_oai_pmh_record_header', array('tid' => $tid, 'node_changed' => $datestamp));

  return '<set>' .
      $vars['fields'] .
      '</set>';
}
function theme_views_oai_pmh_dc_record(&$vars) {
  $nid = $vars['nid'];
  $datestamp = $vars['node_changed'];
  $header = theme('views_oai_pmh_record_header', array('nid' => $nid, 'node_changed' => $datestamp));

  return '<record>' .
    $header .
    '<metadata>
      <oai_dc:dc  xmlns:oai_dc="http://www.openarchives.org/OAI/2.0/oai_dc/"
                      xmlns:dc="http://purl.org/dc/elements/1.1/"
                      xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                      xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/oai_dc/  http://www.openarchives.org/OAI/2.0/oai_dc.xsd">' .
      $vars['fields'] .
      '</oai_dc:dc>
    </metadata>
   </record>';
}


/**
 * Returns a valid XML tag formed from the given input.
 *
 * @param $tag The string that should be made into a valid XML tag.
 * @return The valid XML tag or an empty string if the string contained no valid
 * XML tag characters.
 */
function _views_oai_pmh_xml_tag_clean($tag) {

  // This regex matches characters that are not valid in XML tags, and the
  // unicode ones that are. We don't bother with unicode, because it would so
  // the preg_replace down a lot.
  static $invalid_tag_chars_regex = '#[^\:A-Za-z_\-.0-9]+#';

  // These characters are not valid at the start of an XML tag:
  static $invalid_start_chars = '-.0123456789';

  // Convert invalid chars to '-':
  $tag = preg_replace($invalid_tag_chars_regex, '-', $tag);

  // Need to trim invalid characters from the start of the string:
  $tag = ltrim($tag, $invalid_start_chars);

  return $tag;
}

function theme_oai_dc_field_mapper_form($variables) {
  $form = $variables['form'];
  foreach (element_children($form) as $key ) {
      $rows[] = array(
        drupal_render($form[$key]['drupal_label']),
        drupal_render($form[$key])
      );
  }
  $header = array(
    t('Drupal field label'),
    t('Dublin Core element name')
  );
  $output = theme('table', array('header' => $header, 'rows' => $rows));

  $output .= drupal_render_children($form);
  return $output;

}
function theme_oai_agrisap_field_mapper_form($variables) {
  $form = $variables['form'];
  foreach (element_children($form) as $key ) {
      $rows[] = array(
        drupal_render($form[$key]['drupal_label']),
        drupal_render($form[$key])
      );
  }
  $header = array(
    t('Drupal field label'),
    t('Agris AP element name')
  );
  $output = theme('table', array('header' => $header, 'rows' => $rows));

  $output .= drupal_render_children($form);
  return $output;

}
//** nested elements
function format_oai_xml_nested_elements($array) {
  $output = '';
  global $keyOld;
  foreach ($array as $key => $value) {
   if (($value['key']== "ags:citation") || ($value['key']== "agls:availability")) {
    if (is_numeric($key)) {
      if ($value['key']) {
      
      if (is_array($value['value'])){
         if ($keyOld!=$value['key'] ){
      	   if ($keyOld !=""){
            $output .= '</' . $keyOld . ">\n";
      	   }
            $output .= ' <' . $value['key'];
	         if (isset($value['attributes']) && is_array($value['attributes'])) {
	          $output .= drupal_attributes($value['attributes']). '>' ;
	        }else
	           {$output .= '>';}
	            $keyOld=$value['key'];
	            }
	       }elseif($keyOld!="") {
	             $output .= '</' . $keyOld . ">\n";
	             $output .= ' <' . $value['key'];
	       if (isset($value['attributes']) && is_array($value['attributes'])) {
	          $output .= drupal_attributes($value['attributes']). '>' ;
	        }else
	           {$output .= '>';}
	            $keyOld=$value['key'];
	       }else{
	       	 $output .= ' <' . $value['key'];
	       if (isset($value['attributes']) && is_array($value['attributes'])) {
	          $output .= drupal_attributes($value['attributes']). '>' ;
	        }else
	           {$output .= '>';}
	         $keyOld=$value['key'];}
        

        if (isset($value['value']) && $value['value'] != '') {
          $output .=  (is_array($value['value']) ? format_xml_elements($value['value']) : check_plain($value['value']));
          
        }
        else {
          $output .= " />\n";
        }
     
    }
    }
    else {
      $output .= ' <' . $key . '>' . (is_array($value) ? format_xml_elements($value) : check_plain($value)) . "</$key>\n";
    }
  
  
   }else{
        if (is_numeric($key)) {
		  if ($value['key']) {
			$output .= ' <' . $value['key'];
			if (isset($value['attributes']) && is_array($value['attributes'])) {
			  $output .= drupal_attributes($value['attributes']);
			}

			if (isset($value['value']) && $value['value'] != '') {
			  $output .= '>' . (is_array($value['value']) ? format_xml_elements($value['value']) : check_plain($value['value'])) . '</' . $value['key'] . ">\n";
			}
			else {
			  $output .= " />\n";
			}
		  }
		}
		else {
		  $output .= ' <' . $key . '>' . (is_array($value) ? format_xml_elements($value) : check_plain($value)) . "</$key>\n";
		}
          }
  
}
if ((($value['key']== "ags:citation") || ($value['key']== "agls:availability")) && (substr($output,strlen($output)-3,3)!=">\n")){
  	  $output .= '</' . $keyOld . ">\n";
  	  $keyOld="";
      }
  return $output;
}
