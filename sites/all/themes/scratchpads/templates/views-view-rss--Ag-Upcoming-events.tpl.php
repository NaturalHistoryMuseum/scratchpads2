<?php
/**
 * @file views-view-rss.tpl.php
 * Default template for feed displays that use the RSS style.
 *
 * @ingroup views_templates
 */
global $base_url;
global $language;

$namespaces = $namespaces. " xmlns:ags = 'http://www.purl.org/agmes/1.1/' xmlns:xsi = 'http://www.w3.org/2001/XMLSchema-instance' xmlns:agls = 'http://www.naa.gov.au/recordkeeping/gov_online/agls/1.2' xmlns:ev = 'http://purl.org/rss/1.0/modules/event/'"; 
 print "<?xml"; ?> version="1.0" encoding="utf-8" <?php print "?>"; ?>
<rss version="2.0" xml:base="<?php print $link; ?>"<?php print $namespaces; ?>>
  <channel>
    <title><?php print $title;?></title>
    <link><?php print $link; ?></link>
    <description><?php print $description; ?></description>
    <language><?php print $language->language; ?></language>
    <?php print $channel_elements;	      ?>
     
<?php
    
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
foreach($node->field_ag_tag as $subj) {

$term=taxonomy_term_load($subj[$i][tid]);

$xml_elements[] = array(
  'key'=>'category',
  'attributes' => array('domain'=>$base_url."/taxonomy/term/".$term->name),
   'value' => $term->name,
    );
$i++;
}
//anto
$i=0;
foreach($node->field_ag_tag as $subj) {

$term=taxonomy_term_load($subj[$i][tid]);

  $xml_elements[] = array(
  'key'=>'dc:subject',
   'value' => $term->name,
    );
$i++;
}

$xml_elements[] = array (
'key' => 'dc:identifier',
'value' => $base_url.'/node/'.$node->title,
);
$xml_elements[] = array (
'key' => 'dc:date',
'value' => substr($node->field_ag_date[und][0]['value'], 0, 10),
);

$xml_elements[] = array (
'key' => 'dc:type',
'value' => 'meeting',
);

$xml_elements[] = array (
'key' => 'ags:dateStart',
'value' => substr($node->field_ag_date[und][0]['value'], 0, 10),
'attributes' => array('scheme'=>'dcterms:W3CDTF'),
);

$xml_elements[] = array (
'key' => 'ags:dateEnd',
'value' => substr($node->field_ag_date[und][0]['value2'], 0, 10),
'attributes' => array('scheme'=>'dcterms:W3CDTF'),
);


$xml_elements['ags:location'][] = array(
 'key'=>'ags:locationCity',
 'value'=> $node->field_ag_location[und][0]['value'],
);


$xml_elements['ags:location'][] = array(
 'key'=>'ags:locationCountry',
 'value'=> $node->field_ag_country[und][0]['get_list_from_file'],
);




// EV: compliance:

$xml_elements[] = array (
'key' => 'ev:startdate',
'value' => substr($node->field_ag_date[und][0]['value'], 0, 10),
);

$xml_elements[] = array (
'key' => 'ev:enddate',
'value' => substr($node->field_ag_date[und][0]['value2'], 0, 10),
);

$xml_elements[] = array (
'key' => 'ev:location',
'value' => $node->field_ag_location[und][0]['value'],
);


$xml_elements[] = array (
'key' => 'ev:type',
'value' => 'event',
);



$xml .= format_xml_elements($xml_elements);

print $xml; 
?>
</item> <?php }?>

  </channel>
</rss>
