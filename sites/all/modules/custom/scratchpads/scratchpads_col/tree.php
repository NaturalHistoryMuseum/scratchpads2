<?php
/**
 * Builds a branch of a tree adding left and right values and then echoing them.
 * This can be used to build an SQL query to populate a table of the following
 * structure.
 *
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
 */
$options = getopt('u:p:h:d:');
if(empty($options['u']) || empty($options['p']) || empty($options['d'])){
  echo "Usage: php tree.php -u{USERNAME} -p{PASSWORD} [-h{DATABASE HOST}] -d{DATABASE}";
  exit(1);
}
if(empty($options['h'])){
  $options['h'] = 'localhost';
}
//'AhxaeW8aYai6Utud6Ho2eer6Jiepee'
$link = mysqli_connect($options['h'], $options['u'], $options['p'], $options['d']);
mysqli_query($link, 'SET SESSION group_concat_max_len = 1000000');
$results = mysqli_query($link, 'SELECT lft.taxon_id, GROUP_CONCAT(rgt.taxon_id)
  AS children FROM _taxon_tree lft LEFT JOIN _taxon_tree rgt ON rgt.parent_id = 
  lft.taxon_id GROUP BY rgt.parent_id');
$parent_and_children = array();
while(($row = mysqli_fetch_assoc($results)) != FALSE){
  $parent_and_children[$row['taxon_id']] = explode(',', $row['children']);
}
$results = mysqli_query($link, 'SELECT taxon_id FROM _taxon_tree WHERE parent_id = 0');
$row = mysqli_fetch_assoc($results);
$root_id = $row['taxon_id'];
mysqli_close($link);
echo "DELETE FROM leftandright; INSERT INTO leftandright (id, lft, rgt, depth) VALUES ";
// Note, I could dynamically get the ID of the root term.
rebuild_tree($root_id, 1, 0, $parent_and_children);

function rebuild_tree($id, $left, $depth, &$parent_and_children){
  $right = $left + 1;
  if(!empty($parent_and_children[$id])){
    while(($child_id = array_pop($parent_and_children[$id])) != FALSE){
      if($child_id){
        $right = rebuild_tree($child_id, $right, $depth + 1, $parent_and_children);
      }
    }
    unset($parent_and_children[$id]);
  }
  echo "($id , $left , $right, $depth)";
  if($left != 1){
    echo ",\n";
  }
  return $right + 1;
}