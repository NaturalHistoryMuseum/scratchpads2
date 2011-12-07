DROP PROCEDURE IF EXISTS recursivesubtree;
DELIMITER $$
CREATE PROCEDURE recursivesubtree( i_vid INT, i_root INT, i_depth INT, INOUT i_index INT)
BEGIN
  DECLARE i_rows,i_tid,done INT DEFAULT 0;
  SET i_rows = ( SELECT COUNT(*) FROM taxonomy_term_hierarchy WHERE parent=i_root );
  IF i_depth = 0 THEN
    SET @i_index = 0;
    DELETE FROM taxonomy_leftandright WHERE vid = i_vid;
  END IF;
  IF i_rows > 0 THEN
    BEGIN
      DECLARE cur CURSOR FOR
        SELECT
          t.tid
        FROM 
          taxonomy_term_data t INNER JOIN taxonomy_term_hierarchy h ON h.tid = t.tid
        WHERE parent = i_root
        AND vid = i_vid
        ORDER BY name;
      DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;
      OPEN cur;
      WHILE NOT done DO
        FETCH cur INTO i_tid;
        IF NOT done THEN
          SET i_index = i_index + 1;
          INSERT INTO taxonomy_leftandright_temp (tid, vid, lft, depth) VALUES (i_tid, i_vid, i_index, i_depth);
          CALL recursivesubtree( i_vid, i_tid, i_depth + 1, i_index);
          SET i_index = i_index + 1;
          UPDATE taxonomy_leftandright_temp SET rgt = i_index WHERE tid = i_tid;
        END IF;
      END WHILE;
      CLOSE cur;
    END;
  END IF;
END;
$$
DELIMITER ;
DROP PROCEDURE IF EXISTS rebuild_tree;
DELIMITER $$
CREATE PROCEDURE rebuild_tree(vid INT)
BEGIN
	SET @lft = 0;
	SET @@SESSION.max_sp_recursion_depth=100;
	CALL recursivesubtree(vid, 0, 0, @lft);
END
$$
DELIMITER ;