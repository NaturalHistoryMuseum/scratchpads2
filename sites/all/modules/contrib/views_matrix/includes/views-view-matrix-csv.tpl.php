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
// We want to send the csv as a server response so switch the content
// type and stop further processing of the page.
drupal_add_http_header("Content-Type", "text/csv; charset=utf-8");
$out = fopen('php://output', 'w');
array_unshift($rows, $header);
foreach($rows as $row_key => $row){
  foreach($row as $column_key => $column){
    if(is_array($column)){
      $rows[$row_key][$column_key] = trim(strip_tags($column['data']));
    }else{
      $rows[$row_key][$column_key] = trim(strip_tags($column));
    }
  }
}
foreach($rows as $row){
  fputcsv($out, $row);
}
drupal_page_footer();
exit();