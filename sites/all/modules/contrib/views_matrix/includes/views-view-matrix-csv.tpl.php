<?php
/**
 * @file
 *   Views View Matrix Template
 *
 * Variables:
 *   - $header
 *   - $rows
 *   - $row_classes
 *   - $matrix_attributes
 */
$out = fopen('php://output', 'w');
array_unshift($rows, $header);
foreach($rows as $row_key => $row){
  foreach($row as $column_key => $column){
    if(is_array($column)){
      $rows[$row_key][$column_key] = trim(strip_tags($column['data']));
    } else {
      $rows[$row_key][$column_key] = trim(strip_tags($column));
    }
  }
}
foreach($rows as $row){
  fputcsv($out, $row);
}
exit;