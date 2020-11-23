<?php

/**
 * Provides functions that can be executed via the RPC interface (see scratchpads_tweaks_rpc function)
 * External programs can use a JSONRPC client to call these methods transparently, as if they were defined in that program's own codebase.
 * This makes development easier, since there's a predictable 1-to-1 mapping of the way the functions are defined and the way they're called.
 * The inputs and outputs are designed so that consumers of the API need no knowledge of Drupal or other system internals.
 *
 * NB: Drupal database IDs are currently used to identify objects. Should look at alternatives.
 */
class RpcRouter {
	function test($value){
		return ['You said', $value];
	}

	/**
	 * Get the site settings
	 *
	 * Returns:
	 * 	- name => The site name
	 *	- theme_color => The site's default colour
	 * @return object
	 */
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

	/**
	 * Load an author by biblio author id
	 *
	 * Returns:
	 * 	- firstName => The first name of the author (including any prefixes)
	 * 	- lastName => The surname of the author
	 * 	- suffix => Any suffixes
	 * @return object
	 */
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

	/**
	 * Gets the ID of the parent node
	 *
	 * @param number $tid The ID of the node under question
	 * @return number The ID of the parent
	 */
	function getParent($tid) {
		$term = taxonomy_term_load($tid);
		$vid = $term->vid;
		$field_aan = "field_aan_$vid";
		$aantid = ($term->$field_aan)['und'][0]['tid'];

		return array_keys(taxonomy_get_parents($aantid ?? $tid))[0];
	}

	/**
	 * Gets data about a given Taxon
	 *
	 * Returns:
	 * 	- id => The taxon id
	 * 	- name => The taxon name
	 * 	- rank => The taxonomic rank
	 *  - authors => The name(s) of the taxon authors
	 *  - source => The ID of the source associated with this taxon
	 * @param number $n Taxon ID
	 * @return object
	 */
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

	/**
	 * Gets the children of a taxon specified by ID
	 *
	 * Returns an array of objects:
	 * 	- tid => The ID of the child taxon
	 *  - name => The name of the child taxon
	 *
	 * @param number $tid
	 * @return array
	 */
	function getChildren($tid) {
		return array_values(array_map(
			function($term) { return [
				'tid'=>$term->tid,
				'name'=>$term->name
			]; },
			taxonomy_get_children($tid)
		));
	}

	/**
	 * Gets the data required to render a "Taxon overview" page
	 *
	 * Returns an object with the same return values as getTaxon
	 * With the additional fields:
	 * 	- ancestors => An array of all ancestors (tid, name and rank)
	 * 	- tree => The root node of a tree of related taxa.
	 *
	 * Each tree node contains the keys:
	 * 	- tid => The id of the taxon
	 * 	- name => The taxon name
	 *  - children => An array of nodes who are direct descendants
	 *
	 * The nodes in the tree are selected based on these criteria:
	 * 	- All ancestors of the subject taxon
	 *  - All children of those ancestors
	 *  - All children of the subject taxon
	 *
	 * @param number $n
	 * @return Object
	 */
	function getTaxonOverview($n) {
		$t = $this->getTaxon($n);

		$ancestors = taxonomy_get_parents_all($n);

		$t['ancestors'] = array_reverse(array_map(
			function($taxon) {
				return [
					'tid' => $taxon->tid,
					'name' => $taxon->name,
					'rank' => $taxon->field_rank['und'][0]['value']
				];
			},
			$ancestors
		));

		$tree = null;

		foreach($ancestors as $term) {
			$children = array_map(
				function($term) { return [
					'tid'=>$term->tid,'name'=>$term->name]; },
				taxonomy_get_children($term->tid)
			);

			$children = array_values($children);

			$tree = [
				'tid' => $term->tid,
				'name' => $term->name,
				'children' => $tree ? ([$tree] + $children) : $children
			];
		}

		$t['tree'] = $tree;

		return $t;
	}

	/**
	 * Gets a list of synonyms
	 *
	 * Returns array of objects
	 * (Todo: What's the return structure?)
	 *
	 * @param Number Taxon ID $tid
	 * @return Array
	 */
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

	/**
	 * Gets a taxon source (biblio node)
	 *
	 * Returns:
	 * 	- type => The Bibtex type of the source
	 *  - year => The year of the publication
	 *  - journal => The original publication
	 *  - title => The source title
	 *  - language => The source language
	 *  - contributors => An array of contributor IDs
	 *  - taxonName => The ID of the related taxon
	 *
	 * @param number $n Taxon ID
	 * @return Object
	 */
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
	}


	// Load a raw drupal node (only use for debugging)
	function getNode($n) {
		return node_load($n);
	}

	// Load a raw drupal taxon (only use for debugging)
	function getTaxonRaw($n){
		return taxonomy_term_load($n);
	}
}


/*

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
