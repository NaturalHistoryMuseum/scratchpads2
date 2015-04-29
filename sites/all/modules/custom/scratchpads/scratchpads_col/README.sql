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
     
-- 8. Alter the rank values so that they match the format that the Scratchpad
--    expects

     UPDATE scratchpads_col_terms SET Rank = 'Form' WHERE Rank IN ('form', 'Forma');
     UPDATE scratchpads_col_terms SET Rank = 'Class' WHERE Rank = 'class';
     UPDATE scratchpads_col_terms SET Rank = 'Family' WHERE Rank = 'family';
     UPDATE scratchpads_col_terms SET Rank = 'Genus' WHERE Rank = 'genus';
     UPDATE scratchpads_col_terms SET Rank = 'Kingdom' WHERE Rank = 'kingdom';
     UPDATE scratchpads_col_terms SET Rank = 'Order' WHERE Rank = 'order';
     UPDATE scratchpads_col_terms SET Rank = 'Phylum' WHERE Rank = 'phylum';
     UPDATE scratchpads_col_terms SET Rank = 'Species' WHERE Rank = 'species';
     UPDATE scratchpads_col_terms SET Rank = 'Subvariety' WHERE Rank = 'sub-variety';
     UPDATE scratchpads_col_terms SET Rank = 'Subspecies' WHERE Rank = 'subspecies';
     UPDATE scratchpads_col_terms SET Rank = 'Superfamily' WHERE Rank = 'superfamily';
     UPDATE scratchpads_col_terms SET Rank = 'Variety' WHERE Rank = 'variety';
     
     UPDATE scratchpads_col_synonyms SET Rank = 'Form' WHERE Rank IN ('form', 'Forma');
     UPDATE scratchpads_col_synonyms SET Rank = 'Species' WHERE Rank = 'species';
     UPDATE scratchpads_col_synonyms SET Rank = 'Subspecies' WHERE Rank = 'subspecies';
     UPDATE scratchpads_col_synonyms SET Rank = 'Variety' WHERE Rank = 'variety';
     
-- 9. Alter the languages from three letter codes to two letter codes

     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'aar','aa');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'abk','ab');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'afr','af');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'aka','ak');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'amh','am');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ara','ar');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'arg','an');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'asm','as');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ava','av');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'aym','ay');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'aze','az');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'bak','ba');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'bam','bm');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'bel','be');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ben','bn');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'bis','bi');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'bos','bs');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'bre','br');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'bul','bg');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'cat','ca');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ces','cs');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'cha','ch');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'che','ce');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'chv','cv');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'cor','kw');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'cos','co');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'cre','cr');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'cym','cy');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'dan','da');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'deu','de');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'div','dv');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'dzo','dz');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ell','el');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'eng','en');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'est','et');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'eus','eu');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ewe','ee');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'fao','fo');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'fas','fa');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'fij','fj');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'fin','fi');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'fra','fr');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'fry','fy');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ful','ff');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ger','de');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'gla','gd');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'gle','ga');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'glg','gl');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'glv','gv');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'grn','gn');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'guj','gu');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'hat','ht');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'hau','ha');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'heb','he');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'her','hz');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'hin','hi');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'hmo','ho');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'hrv','hr');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'hun','hu');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'hye','hy');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ibo','ig');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'iii','ii');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'iku','iu');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ind','id');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ipk','ik');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'isl','is');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ita','it');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'jav','jv');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'jpn','ja');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'kal','kl');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'kan','kn');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'kas','ks');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'kat','ka');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'kau','kr');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'kaz','kk');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'khm','km');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'kik','ki');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'kin','rw');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'kir','ky');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'kom','kv');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'kon','kg');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'kor','ko');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'kua','kj');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'kur','ku');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'lao','lo');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'lav','lv');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'lim','li');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'lin','ln');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'lit','lt');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ltz','lb');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'lub','lu');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'lug','lg');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'mah','mh');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'mal','ml');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'mar','mr');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'mkd','mk');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'mlg','mg');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'mlt','mt');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'mon','mn');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'mri','mi');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'msa','ms');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'mya','my');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'nau','na');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'nav','nv');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'nbl','nr');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'nde','nd');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ndo','ng');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'nep','ne');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'nld','nl');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'nno','nn');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'nob','nb');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'nor','no');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'nya','ny');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'oci','oc');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'oji','oj');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ori','or');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'orm','om');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'oss','os');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'pan','pa');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'pol','pl');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'por','pt');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'pus','ps');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'que','qu');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'roh','rm');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ron','ro');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'run','rn');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'rus','ru');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'sag','sg');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'sin','si');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'slk','sk');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'slv','sl');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'sme','se');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'smo','sm');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'sna','sn');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'snd','sd');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'som','so');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'sot','st');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'spa','es');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'sqi','sq');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'srd','sc');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'srp','sr');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ssw','ss');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'sun','su');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'swa','sw');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'swe','sv');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'tah','ty');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'tam','ta');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'tat','tt');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'tel','te');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'tgk','tg');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'tgl','tl');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'tha','th');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'tir','ti');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ton','to');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'tsn','tn');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'tso','ts');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'tuk','tk');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'tur','tr');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'twi','tw');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'uig','ug');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ukr','uk');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'urd','ur');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'uzb','uz');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'ven','ve');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'vie','vi');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'wln','wa');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'wol','wo');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'xho','xh');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'yid','yi');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'yor','yo');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'zha','za');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'zho','zh');
     UPDATE scratchpads_col_terms SET Vernacular_names_COLON__Language = REPLACE(Vernacular_names_COLON__Language, 'zul','zu');

-- 10. Dump the two tables into the load file

/*
     mysqldump {database} scratchpads_col_synonyms scratchpads_col_terms \
     > load.sql
*/