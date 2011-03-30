<?php
// $Id: template.php,v 1.1.2.1 2010/11/11 14:08:01 danprobo Exp $

function danblog_preprocess_html(&$variables) {
  drupal_add_css(path_to_theme() . '/style.ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 6', '!IE' => FALSE), 'preprocess' => FALSE));
}

