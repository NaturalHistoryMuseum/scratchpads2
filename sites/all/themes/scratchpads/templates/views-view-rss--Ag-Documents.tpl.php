<?php
/**
 * @file views-view-rss.tpl.php
 * Default template for feed displays that use the RSS style.
 *
 * @ingroup views_templates
 */
global $base_url;
global $language;

$namespaces = $namespaces. " xmlns:ags = 'http://www.purl.org/agmes/1.1/' xmlns:xsi = 'http://www.w3.org/2001/XMLSchema-instance' xmlns:agls = 'http://www.naa.gov.au/recordkeeping/gov_online/agls/1.2'"; 
 print "<?xml"; ?> version="1.0" encoding="utf-8" <?php print "?>"; ?>
<rss version="2.0" xml:base="<?php print $link; ?>"<?php print $namespaces; ?>>
  <channel>
    <title><?php print $title;?></title>
    <link><?php print $link; ?></link>
    <description><?php print $description; ?></description>
    <language><?php print $language->language; ?></language>
    <?php print $channel_elements; 	 ?>
     
<?php
    //print_r($view->row_plugin);
	//exit;
	
   foreach ($view->result as $result){ 
	    $xml="";
		$xml_elements="";
	    $nid=$result->nid;
		$node=node_load($nid);?>
		
	   <item>
	    <title><?php print $node->title;?></title>
    <link><?php print $base_url.'/node/'.$node->title; ?></link> ?>
<description><?php print trim(strip_tags($node->body[und][0]['value']));?></description>
<?php



$dateTime = $node->created;
$dateRFC = date("r", $dateTime);

$xml_elements[] = array (
'key' => 'pubDate',
'value' => $dateRFC ,
);
$i=0;
foreach($node->field_ag_agrovoc as $subj) {

$term=taxonomy_term_load($subj[$i][tid]);

$xml_elements[] = array(
  'key'=>'category',
  'attributes' => array('domain'=>$base_url."/taxonomy/term/".$term->name),
   'value' => $term->name,
    );
$i++;
}
//patch for old records with wrong language:
$lang = $node->field_ag_language[und][0][get_list_from_file];



$xml_elements[] = array (
'key' => 'dc:language',
'attributes' => array('xsi:type'=> 'dcterms:ISO639-2'),
'value' => $lang,
);

$xml_elements[] = array (
'key' => 'dc:title',
'attributes' => array('xml:lang'=> $lang),//lingua del titolo
'value' => check_plain($node->title),
);

// below: reivew: it should be nested in dc:title
/*
$xml_elements[''] = array (
'key' => 'ags:titleSupplement',
'attributes' => array('xml:lang'=> $lang),
'value' => check_plain($result->field_ag_subtitle[0]['value']),
);
*/

$xml_elements[] = array (
'key' => 'dc:identifier',
'attributes' => array('xsi:type'=>'dcterms:URI'),
'value' => $base_url.'/node/'.$nid,
);
$xml_elements[] = array (
'key' => 'dc:identifier',
'attributes' => array('xsi:type'=>'ags:ISBN'),
'value' => check_plain($node->field_ag_isbn[und][0]['value']),
);
$xml_elements[] = array (
'key' => 'dc:identifier',
'attributes' => array('xsi:type'=>'ags:ISSN'),
'value' => check_plain($node->field_ag_issn[und][0]['value']),
);



$xml_elements[] = array (
'key' => 'dc:type',
'value' => $node->field_ag_type[und][0]['name'],
);

$xml_elements['dc:date'][] = array (
'key' => 'dcterms:dateIssued',
'value' => substr(check_plain($node->field_ag_datepub[und][0]['value']), 0, 4),
);
 
$xml_elements['dc:description'][] = array (
'key' => 'dcterms:abstract',
'value' => strip_tags($node->field_ag_abstract[und][0]['value']),
);

$xml_elements['dc:description'][] = array (
'key' => 'ags:descriptionEdition',
'value' => check_plain($node->field_ag_edition[und][0]['value']),
);

$xml_elements['dc:description'][] = array (
'key' => 'ags:descriptionNotes',
'value' => check_plain($node->field_notes[und][0]['value']),
);

foreach($node->field_test[und] as $auth) {
  
  $query ="SELECT field_ag_author_nid FROM field_data_field_ag_author where entity_id=:entity_id";
  $auth=db_query($query,array(':entity_id' => $auth['value']))->fetchField();
  $n = node_load($auth);
   
  $xml_elements['dc:creator'][] = array (
   'key' => 'ags:creatorPersonal',
   'value' => $n->title,
     );
	 
}

foreach($node->field_corporateauthor[und] as $auth) {
   $query ="SELECT field_ag_corauthor_nid FROM field_data_field_ag_corauthor where entity_id=:entity_id";
  $auth=db_query($query,array(':entity_id' => $auth['value']))->fetchField();
 	$n = node_load($auth);
  $xml_elements['dc:creator'][] = array (
   'key' => 'ags:creatorCorporate',
   'value' => $n->title,
  );
  
}
foreach($node->field_ag_confname[und] as $auth) {
  $n = node_load($auth['value']);
  $xml_elements['dc:creator'][] = array (
   'key' => 'ags:creatorConference',
   'value' => $n->title,
  );
}

// if Journal article or journal issue
if (check_plain($node->field_ag_type[und][0]['name']) == "Journal Article" || check_plain($node->field_ag_type[und][0]['name']) == "Journal Issue") {
  $query ="SELECT field_ag_journal_nid FROM field_data_field_ag_journal where entity_id=:entity_id";
  $journal=db_query($query,array(':entity_id' => $node->field_ag_journal[und][0]['value']))->fetchField();
 	$n = node_load($journal);
  
  $xml_elements['ags:citation'][] = array (
   'key' => 'ags:citationTitle',
   'value' => $n->title,
  );
  //find issn volume number and date pub for the journal
  $xml_elements['ags:citation'][] = array (
   'key' => 'ags:citationIdentifier',
   'value' => $n->field_ag_issn[und][0]['value'],
  );

  if (check_plain($node->field_ag_volume[und][0]['value']) != "") {
   $xml_elements['ags:citation'][] = array (
     'key' => 'ags:citationNumber',
     'value' => check_plain($node->field_ag_volume[und][0]['value']) . " (" . check_plain($node->field_ag_number[und][0]['value']) . ")",
   );
  }

  $xml_elements['ags:citation'][] = array (
   'key' => 'ags:citationChronology',
   'value' => substr(check_plain($node->field_ag_datepub[und][0]['value']), 0, 4),
  );
}

if (check_plain($node->field_ag_type[und][0]['name']) == "Book Chapter") {
  $query ="SELECT field_ag_book_nid FROM field_data_field_ag_book where entity_id=:entity_id";
  $book=db_query($query,array(':entity_id' => $node->field_ag_book[und][0]['value']))->fetchField();
 	
  $n = node_load($book);
  $xml_elements[] = array (
  'key' => 'dc:source',
  'value' => $n->title,
  );
}

$xml_elements['dc:format'][] = array (
'key' => 'dcterms:extent',
'value' => check_plain($node->field_ag_pagination[und][0]['value']),
);
$xml_elements['dc:format'][] = array (
'key' => 'dcterms:medium',
'value' => check_plain($node->field_ag_form[und][0]['value']),
);
 
$xml_elements['agls:availability'][] = array (
'key' => 'ags:availabilityLocation',
'value' => check_plain($node->field_ag_phlocation[und][0]['value']),
);

$xml_elements['agls:availability'][] = array (
'key' => 'ags:availabilityNumber',
'value' => check_plain($node->field_ag_locator[und][0]['value']),
);

$xml_elements['dc:publisher'][] = array (
'key' => 'ags:publisherName',
'value' => check_plain($node->field_ag_publisher[und][0]['value']),
);

$xml_elements['dc:publisher'][] = array (
'key' => 'ags:publisherPlace',
'value' => check_plain($node->field_ag_publishplace[und][0]['value']),
);

foreach($node->field_ag_agris[und] as $subj) {
  $xml_elements['dc:subject'][] = array (
   'key' => 'ags:subjectClassification',
   'value' => $subj['name'],
   'attributes' => array('xsi:type' => 'ags:ASC'),
  );
}


  foreach($node->field_ag_agrovoc[und] as $agrovoc){
        $tid=$agrovoc['tid'] ;
		//find uri lang and term name
		$uri="http://aims.fao.org/aos/agrovoc/c_". $v;
  						
  						
  				  
  $xml_elements['dc:subject'][] = array (
   'key' => 'ags:subjectThesaurus',
   'value' => $agrovocTerm,
   'attributes' => array('xsi:type' => 'ags:AGROVOC', 'xml:lang' => $agrovocLang),
  );
  $xml_elements['dc:subject'][] = array (
   'key' => 'ags:subjectThesaurus',
   'value' => $uri,
   'attributes' => array('xsi:type' => 'ags:AGROVOC'),
  );
  $xml_elements[] = array (
   'key' => 'dc:subject',
   'value' => $uri,
   'attributes' => array('xsi:type' => 'dcterms:URI', 'scheme' => 'ags:AGROVOC'),
  );
}

$xml .= format_xml_elements($xml_elements);

print $xml; 
?>
</item> <?php }?>

  </channel>
</rss>
