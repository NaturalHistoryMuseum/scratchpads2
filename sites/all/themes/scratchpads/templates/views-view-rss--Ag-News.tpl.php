<?php
/**
 * @file views-view-rss.tpl.php
 * Default template for feed displays that use the RSS style.
 *
 * @ingroup views_templates
 */
global $base_url;

$namespaces = $namespaces. " xmlns:ags = 'http://www.purl.org/agmes/1.1/' xmlns:xsi = 'http://www.w3.org/2001/XMLSchema-instance' xmlns:agls = 'http://www.naa.gov.au/recordkeeping/gov_online/agls/1.2'"; 
 print "<?xml"; ?> version="1.0" encoding="utf-8" <?php print "?>"; ?>
<rss version="2.0" xml:base="<?php print $link; ?>"<?php print $namespaces; ?>>
  <channel>
    <title><?php print $title;?></title>
    <link><?php print $link; ?></link>
    <description><?php print $description; ?></description>
    <language><?php print $langcode; ?></language>
    <?php print $channel_elements;?>
     
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
'key' => 'dc:type',
'value' => 'news',
);






$xml .= format_xml_elements($xml_elements);

print $xml; 
?>
</item> <?php }?>

  </channel>
</rss>
