<?php

/**
 * Search on the Catalogue of Life
 */
function scratchpads_col_search($search){
  // Do the query, and return the results.
  // We look for exact matches first, if we find any, we return them.
  $id = db_select('_taxon_tree', 't')->condition('name', $search)->fields('t', array(
    'taxon_id',
    'name'
  ))->execute()->fetchAllKeyed();
  if(!$id){
    // We didn't find any exact matches, so we look for names that START with
    // the provided string
    $id = db_select('_taxon_tree', 't')->condition('name', $search . '%', 'LIKE')->fields('t', array(
      'taxon_id',
      'name'
    ))->execute()->fetchAllKeyed();
  }
  if(!$id){
    // We didn't find any exact matches, so we look for names that START with
    // the provided string
    $id = db_select('_taxon_tree', 't')->condition('name', '%' . $search . '%', 'LIKE')->fields('t', array(
      'taxon_id',
      'name'
    ))->execute()->fetchAllKeyed();
  }
  return $id;
}

/**
 * Return a MySQL result set which can be parsed through. Note, the headers are
 * based on the column names in the Query.
 */
function scratchpads_col_download($id, $page = 0){
  $row = db_select('leftandright', 'l')->condition('id', $id)->fields('l', array(
    'rgt',
    'lft'
  ))->execute()->fetchAssoc();
  return db_query("SELECT * FROM (SELECT
  _taxon_tree.name AS `Term name`,
  '' AS `Parent Term Name`,
  COALESCE(taxon_detail.additional_data, '') AS `Term description`,
  _taxon_tree.lsid AS `GUID`,
  parent.lsid AS `Parent GUID`,
  '' AS `Associated accepted name (Name)`,
  '' AS `Associated accepted name (TID)`,
  '' AS `Associated accepted name (GUID)`,
  COALESCE(GROUP_CONCAT(common_name_element.name SEPARATOR '|'), '') AS `Vernacular names: Vernacular name`,
  COALESCE(GROUP_CONCAT(common_name.language_iso SEPARATOR '|'), '') AS `Vernacular names: Language`,
  '' AS `Vernacular names: Source (GUID)`,
  '' AS `Vernacular names: Source (NID)`,
  '' AS `Vernacular names: Source (Title)`,
  '' AS `Vernacular names: Area from which the vernacular name usage originates`,
  '' AS `Vernacular names: Vernacular name remarks`,
  COALESCE(author_string.string, '') AS 'Authors',
  '' AS `Page number`,
  _taxon_tree.rank AS `Rank`,
  '' AS `Reference (GUID)`,
  '' AS `Reference (NID)`,
  '' AS `Reference (Title)`,
  '' AS `Unacceptability Reason`
  FROM
  leftandright
  INNER JOIN
  _taxon_tree ON _taxon_tree.taxon_id = leftandright.id
  INNER JOIN
  _taxon_tree AS parent ON _taxon_tree.parent_id = parent.taxon_id
  LEFT JOIN
  taxon_detail ON _taxon_tree.taxon_id = taxon_detail.taxon_id
  LEFT JOIN
  author_string ON taxon_detail.author_string_id = author_string.id
  LEFT JOIN
  common_name ON _taxon_tree.taxon_id = common_name.taxon_id
  LEFT JOIN
  common_name_element ON common_name.common_name_element_id = common_name_element.id
  WHERE lft >= :lft AND rgt <= :rgt
  GROUP BY _taxon_tree.taxon_id ORDER BY leftandright.lft) AS pointlessalias
  UNION
  SELECT
  TRIM(GROUP_CONCAT(name_element SEPARATOR ' ')) AS `Term name`,
  '' AS `Parent Term Name`,
  '' AS `Term description`,
  '' AS `GUID`,
  _taxon_tree.lsid AS `Parent GUID`,
  '' AS `Associated accepted name (Name)`,
  '' AS `Associated accepted name (TID)`,
  _taxon_tree.lsid AS `Associated accepted name (GUID)`,
  '' AS `Vernacular names: Vernacular name`,
  '' AS `Vernacular names: Language`,
  '' AS `Vernacular names: Source (GUID)`,
  '' AS `Vernacular names: Source (NID)`,
  '' AS `Vernacular names: Source (Title)`,
  '' AS `Vernacular names: Area from which the vernacular name usage originates`,
  '' AS `Vernacular names: Vernacular name remarks`,
  author_string.string AS 'Authors',
  '' AS `Page number`,
  _taxon_tree.rank AS `Rank`,
  '' AS `Reference (GUID)`,
  '' AS `Reference (NID)`,
  '' AS `Reference (Title)`,
  '' AS `Unacceptability Reason`
  FROM
  leftandright
  INNER JOIN
  synonym ON synonym.taxon_id = leftandright.id
  INNER JOIN
  _taxon_tree ON leftandright.id = _taxon_tree.taxon_id
  INNER JOIN
  synonym_name_element ON synonym.id = synonym_name_element.synonym_id
  INNER JOIN
  scientific_name_element ON synonym_name_element.scientific_name_element_id = scientific_name_element.id
  INNER JOIN
  author_string ON author_string.id = synonym.author_string_id
  WHERE lft >= :lft AND rgt <= :rgt
  GROUP BY synonym.id LIMIT " . ($page * 50000) . ", 50000;", array(
    ':lft' => $row['lft'],
    ':rgt' => $row['rgt'],
    ':lft' => $row['lft'],
    ':rgt' => $row['rgt']
  ));
}