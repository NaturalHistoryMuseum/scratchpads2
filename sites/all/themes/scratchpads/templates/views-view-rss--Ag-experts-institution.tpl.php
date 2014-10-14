<?php
/**
 * @file views-view-rss.tpl.php
 * Default template for feed displays that use the RSS style.
 *
 * @ingroup views_templates
 */
global $base_url;
global $language;

$namespaces = $namespaces. " xmlns:ags = 'http://www.purl.org/agmes/1.1/' xmlns:xsi = 'http://www.w3.org/2001/XMLSchema-instance' xmlns:agls = 'http://www.naa.gov.au/recordkeeping/gov_online/agls/1.2' "; 
 print "<?xml"; ?> version="1.0" encoding="utf-8" <?php print "?>"; ?>
<rss version="2.0" xml:base="<?php print $link; ?>"<?php print $namespaces; ?>>
  <channel>
    <title><?php print $title;?></title>
    <link><?php print $link; ?></link>
    <description><?php print $description; ?></description>
    <language><?php print $language->language; ?></language>
    <?php print $channel_elements; 	 ?>
     
<?php
   // print_r($view);
	//exit;
	
   foreach ($view->result as $result){ 
	    $xml="";
		$xml_elements="";
	    $nid=$result->nid;
		$node=node_load($nid);?>
		
	   <item>
	    <title><?php print $node->title;?></title>
    <link><?php print $base_url.'/node/'.$node->title; ?></link> 
	
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


$lang = $node->language;
if ($lang == "")
  $lang = "en";

$xml_elements[] = array (
'key' => 'dc:language',
'attributes' => array('scheme'=> 'dcterms:ISO639-2'),
'value' => $lang,
);

$xml_elements[] = array (
'key' => 'dc:title',
'attributes' => array('xml:lang'=> $lang),
'value' => check_plain($node->title),
);

$xml_elements['ags:organizationName'][] = array (
 'key' => 'ags:fullOrganizationName',
 'value' => check_plain($node->title),
'attributes' => array('xml:lang'=> $lang),
);
$xml_elements['ags:organizationName'][] = array (
 'key' => 'ags:organizationAcronym',
 'value' => check_plain($node->field_ag_acronym[und][0]['value']),
 'attributes' => array('xml:lang'=> $lang),
);

$xml_elements['ags:location'][] = array (
 'key' => 'ags:locationStreet',
 'value' => check_plain($node->field_ag_address[und][0]['value']),
);
$xml_elements['ags:location'][] = array (
 'key' => 'ags:locationCountry',
 'value' => check_plain($node->field_ag_country[und][0]['get_list_from_file']),
);

$xml_elements[] = array (
 'key' => 'ags:organizationType',
 'value' => $node->field_type[und][0][taxonomy_term]->name,
);

$xml_elements[] = array (
 'key' => 'ags:email',
 'value' => $node->field_ag_email[und][0]['email'],
);
$xml_elements[] = array (
 'key' => 'ags:telephone',
 'value' => check_plain($node->field_ag_telephone[und][0]['value']),
);

if (check_plain($node->field_ag_website_url[und][0]['url']) != "") {
  $xml_elements[] = array (
   'key' => 'dc:identifier',
   'attributes' => array('scheme'=>'dcterms:URI'),
   'value' => check_plain($node->field_ag_website_url[und][0]['url']),
  );
} else {
  $xml_elements[] = array (
   'key' => 'dc:identifier',
   'attributes' => array('scheme'=>'dcterms:URI'),
   'value' => $base_url."/node/".$node->title,
  );
}

$xml_elements[] = array (
 'key' => 'dc:description',
 'attributes' => array('xml:lang'=> $lang),
);

$xml_elements[] = array (
 'key' => 'dc:type',
 'value' => "Institution",//strip_tags($node->body),
);


//anto
//anto
$i=0;
foreach($node->field_ag_agris as $subj) {

$term=taxonomy_term_load($subj[$i][tid]);

$xml_elements['dc:subject'][] = array (
   'key' => 'ags:subjectClassification',
   'value' => $term->name,
    'attributes' => array('scheme' => 'ags:ASC'),
);
$i++;
}
//anto
$i=0;
foreach($node->field_ag_agrovoc as $subj) {
$term=taxonomy_term_load($subj[$i][tid]);
  $xml_elements['dc:subject'][] = array (
   'key' => 'ags:subjectThesaurus',
   'value' =>$term->name,
   'attributes' => array('scheme' => 'ags:AGROVOC'),
  );
$i++;
}



$xml .= format_xml_elements($xml_elements);

print $xml; 
?>
</item> <?php }?>

  </channel>
</rss>
