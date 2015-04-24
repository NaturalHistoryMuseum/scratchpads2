-- To recreate the data contained in the file load.sql, you must follow the
-- following procedure. Note:
--  - This procedure can be carried out on any machine - it will output an SQL
--    file that can be loaded later
--  - Comments in /* */ blocks are intended to be executed from the command line

-- 1. Download the latest Catalogue of Life data DVD, and extract the SQL data
--    from the Linux install.
-- 2. Load the SQL data into a database (any database should do, but a clean one
--    is preferable).
-- 3. Create the leftandright table.

     CREATE TABLE leftandright (
       id INT(10),
       lft INT(10),
       rgt INT(10),
       depth INT(10),
       INDEX(lft, rgt),
       INDEX(lft, rgt, depth),
       INDEX(lft,rgt,depth,id),
       INDEX(lft),
       INDEX(rgt),
       INDEX(depth),
       INDEX(id)
     );

-- 4. Create a "Life" term at the root of the tree, and move the other root terms
--    to be children of "Life".

     CREATE TEMPORARY TABLE temp_taxon_tree_data AS SELECT MAX(taxon_id)+1 AS id
      FROM _taxon_tree;
     UPDATE _taxon_tree SET parent_id = SELECT id FROM temp_taxon_tree_data
      WHERE parent_id = 0;
     INSERT INTO _taxon_tree (taxon_id, name, rank, parent_id, lsid,
      number_of_children, total_species_estimation, total_species,
      estimate_source) SELECT id, 'Life', 'life', 0, '', 0, 0, 0, '' FROM
      temp_taxon_tree_data;

-- 5. Populate the leftandright table. Note, you can probably output the SQL
--    directly to MySQL, but outputting to a file makes errors easier to track
--    down.
/*
     php tree.php -u{USERNAME} -h{HOSTNAME} -p{PASSWORD} -d{DATABASE} > tree.sql
     cat tree.sql | mysql {DATABASE}
*/

-- 6. Create flattened tables of the data that we want. Note, this query will
--    take quite some time to run, so be patient.

     CREATE TABLE scratchpads_col_terms AS SELECT
      _taxon_tree.taxon_id AS `taxon_id`,
      leftandright.lft AS `lft`,
      leftandright.rgt AS `rgt`,
      _taxon_tree.name AS `Term_name`,
      COALESCE(taxon_detail.additional_data, '') AS `Term_description`,
      _taxon_tree.lsid AS `GUID`,
      parent.lsid AS `Parent GUID`,
      COALESCE(GROUP_CONCAT(common_name_element.name SEPARATOR '|'), '') AS `Vernacular_names_COLON__Vernacular_name`,
      COALESCE(GROUP_CONCAT(common_name.language_iso SEPARATOR '|'), '') AS `Vernacular_names_COLON__Language`,
      COALESCE(author_string.string, '') AS 'Authors',
      _taxon_tree.rank AS `Rank`
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
     GROUP BY _taxon_tree.taxon_id;
     
     CREATE TABLE scratchpads_col_synonyms AS SELECT
      leftandright.lft AS `lft`,
      leftandright.rgt AS `rgt`,
      TRIM(GROUP_CONCAT(name_element SEPARATOR ' ')) AS `Term_name`,
      _taxon_tree.lsid AS `Parent_GUID`,
      _taxon_tree.lsid AS `Associated accepted_name__OPEN_GUID_CLOSE_`,
      author_string.string AS 'Authors',
      _taxon_tree.rank AS `Rank`
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
     GROUP BY synonym.id;

-- 7. Add indexes to the flattened tables.

     ALTER TABLE scratchpads_col_terms ADD PRIMARY KEY (`taxon_id`);
     ALTER TABLE scratchpads_col_terms ADD INDEX (`lft`);
     ALTER TABLE scratchpads_col_terms ADD INDEX (`rgt`);
     ALTER TABLE scratchpads_col_terms ADD INDEX (`Term name`);
     ALTER TABLE scratchpads_col_terms ADD INDEX (`lft`,`rgt`);
     ALTER TABLE scratchpads_col_synonyms ADD INDEX (`lft`);
     ALTER TABLE scratchpads_col_synonyms ADD INDEX (`rgt`);
     ALTER TABLE scratchpads_col_synonyms ADD INDEX (`lft`,`rgt`);

-- 8. Dump the two tables into the load file

/*
     mysqldump {database} scratchpads_col_synonyms scratchpads_col_terms \
     > load.sql
*/