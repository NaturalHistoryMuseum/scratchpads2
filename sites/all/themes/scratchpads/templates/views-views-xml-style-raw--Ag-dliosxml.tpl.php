<?php
// $Id: views-views-xml-style-raw.tpl.php,v 1.1.2.3 2010/03/22 09:30:30 allisterbeharry Exp $
/**
 * @file views-views-xml-style-raw.tpl.php
 * Default template for the Views XML style plugin using the raw schema
 *
 * - $view: The View object.
 * - $rows: Array of row objects as rendered by _views_xml_render_fields 
 *
 * @ingroup views_templates
 */

$base = "ags:resource";
// $root ="ags:resources xmlns:ags=\"http://www.fao.org/agris/agmes/schemas/1.1/ags\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:agls=\"http://www.naa.gov.au/recordkeeping/gov_online/agls/1.2\" xmlns:dcterms=\"http://purl.org/dc/terms/\"";
//$stylesheet="<xsl:stylesheet xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\" xmlns:skos=\"http://www.w3.org/2004/02/skos/core#\" xmlns:rdfs=\"http://www.w3.org/2000/01/rdf-schema#\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:foaf=\"http://xmlns.com/foaf/0.1/\" xmlns:aos=\"http://aims.fao.org/aos/\">";
//$stylesheet .=" <xsl:output method=\"xml\" media-type=\"rdf/xml\" /> <xsl:template match=\"*[contains(local-name(),'xml:')]\"> <xsl:element name=\"{substring-after(name(),'xml:')}\"> <xsl:apply-templates>";
//$stylesheetclose="</xsl:apply-templates></xsl:element></xsl:template></xsl:stylesheet>";
$root ="ags:resources xmlns:ags=\"http://purl.org/agmes/1.1/\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:agls=\"http://www.naa.gov.au/recordkeeping/gov_online/agls/1.2\" xmlns:dcterms=\"http://purl.org/dc/terms/\"";
$plaintext_output = $options["plaintext_output"];
$skip_empty = $options["skip_empty_fields"];
$content_type = ($options['content_type'] == 'default') ? 'text/xml' : $options['content_type'];
$header = $options['header'];

$xml =  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
//$xml .= $stylesheet;

if ($header) $xml.= $header.""; 
$xml .= "<$root>";
$count=0;
 $language_def= language_default();
foreach($rows as $row) {
  $node=node_load($view->result[$count]->nid); //VP 20-07-2010: $count instead of 0 as index 
  //VP: for agridrupal: find repository node for center code
  $nid=$node->nid;
  $query ="SELECT field_ag_arn_value FROM  field_data_field_ag_arn" ;
  $rep=db_query($query)->fetchField();
  $rest = substr($rep, 0,2);
  $year = date("Y");
  $code=substr($rep, 2);
  $prog = str_pad($nid, 5, '0', STR_PAD_LEFT); //VP 20-07-2010: $node->nid instead of $prog
  $arn=$rest.$year.$code.$prog;

  $xml .= ($options['element_output'] == 'nested') ? "  <$base ags:ARN=\"$arn\">": "  <$base";

	// useful generally: set a default language, so tha if some xml:lang attributes are empty
	// we have either the node language or the current language
	//$nodeLang = "en";
	//$lang = array();
	//foreach($node->field_ag_language as $lan) {
  	//  $lang[]=$node->field_ag_language[$item][value];
  //}
	//if ($lang[0] != "")
	//  $nodeLang = $lang[0];
	//if ($nodeLang == "")
	//  $nodeLang = $language->language;
    $rawcount=0;
	$iLanAbs=0;
	$iLanAlt=0;
	$iLanLan=0;
    $items=array();
    
   foreach($row as $id => $object) {	
	   //anto apr 2010 skip if empty

		if ((trim(check_plain(strip_tags($object->content))) == '') || (trim(check_plain(strip_tags($object->content))) == '\n') || (trim(check_plain(strip_tags($object->content))) == ',')){
			//print_r($object->content);
			//exit;
		//if (($object->content == '') && ($skip_empty)) {
		 // write nothing
		}else{	
	   $key = _views_xml_strip_illegal_xml_name_chars(check_plain(strip_tags($object->label)));
		 $value = ($plaintext_output ? trim(check_plain(strip_tags($object->content))) : $object->content);
		 
		$item['attributes']=array();
		$sub['attributes']=array();
		$sub=array();
		$lan='';
	 if (($key=="ags:creatorPersonal")||($key=="ags:creatorCorporate")||($key=="ags:publisherName")||($key=="ags:publisherPlace")||($key=="ags:subjectThesaurus")||($key=="ags:subjectClassification")||($key=="dcterms:abstract")||($key=="ags:availabilityLocation")||($key=="ags:availabilityNumber")||($key=="dcterms:alternative")||($key=="agrovoc_uri")||($key=="ags:creatorPersonal")||($key=="dc:language"))
	 
	//if (($object->is_multiple==TRUE)) 
	{
       //  $separator = $field->options['separator'];
	
		     //valeria: multiple fields bug: separator is ;
         $separator = ";";
		
        foreach (explode($separator, $value) as $value) {
			      
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
					 case "agrovoc_uri":
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
				     case "ags:subjectClassification":
					 		// $sub: array for the nested element
						$sub['key'] = $key;
						$sub['value'] = substr($value,0,3);
						$sub['attributes'] = array('scheme'=>'ags:ASC');
						 // array for the containing element
						$item['key'] = "dc:subject";
						$item['value'] = array('0' => $sub); // the value can contain an array (for the nested element)
						$items[] = $item;
						break;
					 case "dcterms:abstract":
					    // $sub: array for the nested element
						
						
						$sub['key'] = $key;
						$sub['value'] = $value;
						
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
					if (($object->id =="upload")||($object->id =="field_ag_resource_url")){
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
				case "dcterms:dateIssued":
             // $sub: array for the nested element
                   $sub['key'] = $key;
					$year = substr($value,0,4); 
					$month = substr($value,5,2);
					$value=$year."-".$month;
					$sub['key'] = $key;
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
				    $item['key'] = $key;
					if (($object->id =="upload")||($object->id =="field_ag_resource_url")){
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
				        $query ="SELECT `field_ag_language_get_list_from_file` FROM `field_data_field_ag_language`  WHERE entity_id =:nid and delta=:delta";
                        $lan= db_query($query,array(':nid' => $nid, ':delta' => $iLanLan))->fetchField();
						$iLanLan++;
			            $item['value'] = $lan;
				        $items[] = $item;
				        break;	
				case "dc:title":
				        
					    $query ="SELECT `field_ag_langtitle_get_list_from_file` FROM `field_data_field_ag_langtitle` WHERE entity_id =:nid" ;
                        $lan= db_query($query,array(':nid' => $nid))->fetchField();
						$item['key'] = $key;
						$item['value'] = $value; // the value can contain an array (for the nested element)
						$item['attributes'] = array('xml:lang'=>$lan);//put the language of abstract but it is not a view field
						$items[] = $item;
						break;		
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
					 case "ags:subjectThesaurus":
                        $query ="SELECT `i18n_tsid` FROM `taxonomy_term_data` WHERE name =:name and vid=16" ;
						$tid= db_query($query,array(':name' => $object->content))->fetchField();
						$query ="SELECT `name`,`language` FROM `taxonomy_term_data` WHERE i18n_tsid =:i18n_tsid and vid=16" ;
						$resultAgr= db_query($query,array(':i18n_tsid' => $tid));
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
					 case "agrovoc_uri":
                        $key="ags:subjectThesaurus";
					 	$uri="http://aims.fao.org/aos/agrovoc/c_".$value;
						$sub['key'] = $key;
						$sub['value'] = $uri;
						$sub['attributes'] = array('scheme'=>'ags:AGROVOC');
						// array for the containing element
						$item['key'] = "dc:subject";
						$item['value'] = array('0' => $sub); // the value can contain an array (for the nested element)
						$items[] = $item;
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
						
						$sub['key'] = $key;
						$sub['value'] = $value;
						$query ="SELECT `field_ag_langtext_get_list_from_file` FROM `field_data_field_ag_langtext` WHERE entity_id =:nid and delta=:delta" ;
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
						$item['value'] = $value;
						$items[] = $item;
                }	
				
      }
      
    
  

}
}
$xml .= format_xml_nested_elements($items);
$xml .= ($options['element_output'] == 'nested') ? "  </$base>": "/>";
$count=$count+1;
}

  	
	
	if ($view->override_path)       // inside live preview
    print htmlspecialchars($xml);

	$xml .= "</ags:resources>";	
   
	drupal_add_http_header('Content-Type', $content_type);
	print $xml;
	exit;
//** nested elements
function format_xml_nested_elements($array) {
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
				  $arrNested=$value['value'][0];
			  if (is_array($arrNested[1])==true){
			         $output .= '>' . (is_array($arrNested) ? format_xml_elements($arrNested) : check_plain($arrNested)) . '</' . $value['key'] . ">\n";}
			  else{
			  $output .= '>' . (is_array($value['value']) ? format_xml_elements($value['value']) : check_plain($value['value'])) . '</' . $value['key'] . ">\n";}
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
