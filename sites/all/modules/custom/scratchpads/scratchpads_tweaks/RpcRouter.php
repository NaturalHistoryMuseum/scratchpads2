<?php

class RpcRouter {
	function test($value){
		return ['You said', $value];
	}

	function getSiteSettings(){
		$colour = variable_get('scratchpads_colour', SCRATCHPADS_DEFAULT_COLOUR);
		if($colour == 'custom'){
			$colour = variable_get('scratchpads_custom_colour');
		}

		return [
			'name' => variable_get('site_name', 'Scratchpad'),
			'theme_color' => '#' . $colour
		];
	}

	function getNode($n) {
		return node_load($n);
	}

	function getAuthor($aid) {
		module_load_include('inc', 'biblio', 'includes/biblio.contributors');
		$author = biblio_get_contributor($aid);
		$firstName = $author->firstname;
		if($i = $author->initials){
			$firstName = $firstName . ' ' . $i;
		}

		return [
			'firstName' => $firstName,
			'lastName' => $author->lastname,
			'suffix' => $author->suffix
		];
	}

	function getTaxonRaw($n){
		return taxonomy_term_load($n);
	}

	function getParent($tid) {
		$term = taxonomy_term_load($tid);
		$vid = $term->vid;
		$field_aan = "field_aan_$vid";
		$aantid = ($term->$field_aan)['und'][0]['tid'];

		return array_keys(taxonomy_get_parents($aantid ?? $tid))[0];
	}

	function getTaxon($n) {
		$term = taxonomy_term_load($n);

		$source = node_load($term->field_reference['und'][0]['nid']);

		return [
			'id' => $n,
			// 'parent_id' => ,
			'name' => $term->name,
			'type' => $term->field_usage['und'][0]['value'],
			'rank' => $term->field_rank['und'][0]['value'],
			'authors' => $term->field_authors['und'][0]['value'],
			'source' => $source->nid
		];
	}

	function getSynonyms($tid) {
		$vid = taxonomy_term_load($tid)->vid;
		$field_aan = "field_aan_$vid";

		$q = new EntityFieldQuery();
		$result = $q->entityCondition('entity_type', 'taxonomy_term')
			//->entityCondition('bundle', [])
			->fieldCondition($field_aan, 'tid', $tid)
			->execute();

		return array_values(
			taxonomy_term_load_multiple(
				array_keys($result['taxonomy_term'])
			)
		);
	}

	function getSource($n) {
		$source = node_load($n);

		$types = [
			'Journal Article' => 'article',
			102 => 'article'
		];

		$t = $source->biblio_type;

		return [
			'type' => $types[$t] ?? $t,
			'year' => $source->biblio_year,
			'journal' => $source->biblio_original_publication,
			'title' => $source->title,
			'language' => $source->biblio_lang,
			'contributors' => array_map(
				function($c){ return $c['cid']; },
				$source->biblio_contributors
			),
			'taxonName' => $source->field_taxonomic_name['und'][0]['tid']
		];

		// s = Source::Bibtex.create!(
		// 	bibtex_type: 'article',
		// 	year: 1838,
		// 	journal: 'Sylva Tellur. 56 (1838).',
		// 	title: 'Sylva Tellur. 56 (1838).',
		// 	authors: [p],
		// 	keywords: [b]
		// )
	}
}


/*


function scratchpads_tweaks_rdf_settings(){
  $ns = 'https://www.w3.org/TR/2020/WD-appmanifest-20200715/#dom-webappmanifest-';

  if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $j = file_get_contents("php://input");
    $body = json_decode($j, true);

    if(isset($body[$ns.'name'])) {
      variable_set('site_name', $body[$ns.'name']);
    }
    if(isset($body[$ns.'theme_color'])) {
      $colour = str_replace('#', '', $body[$ns.'theme_color']);
      variable_set('scratchpads_custom_colour', $colour);
      variable_set('scratchpads_colour', 'custom');
      variable_del('scratchpads_colour_css_file');
    }
  }

  $colour = variable_get('scratchpads_colour', SCRATCHPADS_DEFAULT_COLOUR);
  if($colour == 'custom'){
    $colour = variable_get('scratchpads_custom_colour');
  }

  $output = [
    $ns.'name' => variable_get('site_name', 'Scratchpad'),
    $ns.'theme_color' => '#' . $colour
  ];

  drupal_json_output($output);
}

function scratchpads_tweaks_rdf_taxon_concepts(){
  $out = [];

  foreach(variable_get('biological_vids', []) as $vid => $x) {
    $tree = taxonomy_get_tree($vid, 0, 1);
    $out[] = './' . $tree[0]->tid;
  }
  drupal_json_output($out);
}

function _sp_tax_value($e, $f){
  //$val = isset($e->$f) ? array_values($e->$f)[0] : null;
  $val = field_get_items('taxonomy_term', $e, $f);

  if(isset($val[0])) {
    return $val[0];
  } else {
    return $val;
  }
}

function scratchpads_tweaks_rdf_taxon_concept($term){
  $ns = 'http://rs.tdwg.org/ontology/voc/TaxonConcept#';

  $rels = [];

  $out = [
    $ns.'hasName' => $term->name
  ];

  $aan = _sp_tax_value($term, 'field_aan_'.$term->vid);

  if($aan){
    $rels[] = [
      $ns.'relationshipCategory' => $ns.'isSynonymFor',
      $ns.'toTaxon' => '../' . $aan['tid']
    ];
  }

  $rank = _sp_tax_value($term, 'field_rank');

  if($rank) {
    $out[$ns.'rank'] = $rank['value'];
  }

  //$out['term'] = $term;
  foreach(taxonomy_get_children($term->tid) as $id => $t) {
    $rels[] = [
      $ns.'relationshipCategory' => $ns.'isParentTaxonOf',
      $ns.'toTaxon' => '../'.$id,
      $ns.'hasName' => $t->name
    ];
  }

  foreach(taxonomy_get_parents($term->tid) as $id => $t) {
    $rels[] = [
      $ns.'relationshipCategory' => $ns.'isChildTaxonOf',
      $ns.'toTaxon' => '../'.$id,
      $ns.'hasName' => $t->name
    ];
  }

  $out[$ns.'hasRelationship'] = $rels;

  drupal_json_output($out);
}

*/
