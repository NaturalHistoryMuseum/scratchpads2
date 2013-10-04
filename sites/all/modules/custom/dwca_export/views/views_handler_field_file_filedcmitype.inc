<?php

/**
 * Field handler to add rendering Darwin Core Metadata Initiative type for files
 *
 */
class views_handler_field_file_filedcmitype extends views_handler_field{

  function render($values){
    $filename = $this->get_value($values);
    $value = NULL;
    if(preg_match('/\.([^\.]+)$/', $filename, $matches)){
      // Try known extensions
      $extension = strtolower(trim($matches[1]));
      $map = array(
        'xml' => 'http://purl.org/dc/dcmitype/Dataset',
        'nex' => 'http://purl.org/dc/dcmitype/Dataset',
        'phy' => 'http://purl.org/dc/dcmitype/Dataset',
        'nhx' => 'http://purl.org/dc/dcmitype/Dataset',
        'jpg' => 'http://purl.org/dc/dcmitype/StillImage',
        'jpeg' => 'http://purl.org/dc/dcmitype/StillImage',
        'png' => 'http://purl.org/dc/dcmitype/StillImage',
        'gif' => 'http://purl.org/dc/dcmitype/StillImage',
        'tif' => 'http://purl.org/dc/dcmitype/StillImage',
        'tiff' => 'http://purl.org/dc/dcmitype/StillImage',
        'bmp' => 'http://purl.org/dc/dcmitype/StillImage'
      );
      if(isset($map[$extension])){
        $value = $map[$extension];
      }
    }
    if(!$value){
      // Otherwise try by mimetype
      $mime = file_get_mimetype($filename);
      if(preg_match('/^image\//', $mime)){
        $value = 'http://purl.org/dc/dcmitype/StillImage';
      }
    }
    if(!$value){
      // For backwards compatibility, we default to StillImage
      // when we can't work out the type.
      $value = 'http://purl.org/dc/dcmitype/StillImage';
    }
    return $this->sanitize_value($value);
  }
}
