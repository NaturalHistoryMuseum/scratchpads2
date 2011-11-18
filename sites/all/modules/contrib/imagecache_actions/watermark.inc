<?php
// $ID $
/**
 * @file routine for image layering
 * 
 */


/****************************************************************************************************************************************/
/**
 * Note that the below code is laboriously slow - it takes and compares every pixel of the two inputs
 * and calculates a new valus for each of them. this is only a fallback because reliable image toolkit 
 * transparencies was buggy on certain platforms :(
 */

//Niko (http://www.codeguru.com.ua)
class watermark {
  function create_watermark($main_img_obj, $watermark_img_obj, $x_ins, $y_ins, $alpha_level = 100) {
    $alpha_level /= 100;
    # Should this change to match both images?
    #$main_img_obj_w = max(imagesx($main_img_obj), imagesx($watermark_img_obj));
    #$main_img_obj_h = max(imagesy($main_img_obj), imagesy($watermark_img_obj));
    $main_img_obj_w = imagesx($main_img_obj);
    $main_img_obj_h = imagesy($main_img_obj);
    $watermark_img_obj_w = imagesx($watermark_img_obj);
    $watermark_img_obj_h = imagesy($watermark_img_obj);

    $main_img_obj_min_x = $x_ins;
    $main_img_obj_min_y = $y_ins;

    $return_img = imagecreatetruecolor($main_img_obj_w, $main_img_obj_h);
    imagesavealpha($return_img, true);
    imagealphablending($return_img, false);
    

    for ($y = 0; $y < $main_img_obj_h; $y++) {
      for ($x = 0; $x < $main_img_obj_w; $x++) {

        $return_color = NULL;

        $watermark_x = $x - $main_img_obj_min_x;
        $watermark_y = $y - $main_img_obj_min_y;

        $main_rgb = imagecolorsforindex($main_img_obj, imagecolorat($main_img_obj, $x, $y));

        if ($watermark_x >= 0 && $watermark_x < $watermark_img_obj_w && $watermark_y >= 0 && $watermark_y < $watermark_img_obj_h) {
          $watermark_rbg = imagecolorsforindex($watermark_img_obj, imagecolorat($watermark_img_obj, $watermark_x, $watermark_y));

          $watermark_alpha = round(((127 - $watermark_rbg['alpha']) / 127), 2);
          $watermark_alpha = $watermark_alpha * $alpha_level;

          $avg_red = $this->_get_ave_color($main_rgb['red'], $watermark_rbg['red'], $watermark_alpha);
          $avg_green = $this->_get_ave_color($main_rgb['green'], $watermark_rbg['green'], $watermark_alpha);
          $avg_blue = $this->_get_ave_color($main_rgb['blue'], $watermark_rbg['blue'], $watermark_alpha);
          
          // TODO figure out the maths for merging two transparent images
          $new_alpha = min($watermark_rbg['alpha'], $main_rgb['alpha']);
          #$new_alpha = $main_rgb['alpha'];
          #$new_alpha = 0;

          $return_color = $this->_get_image_color($return_img, $avg_red, $avg_green, $avg_blue, $new_alpha);
        }
        else {
          $return_color = imagecolorat($main_img_obj, $x, $y);
        }
        imagesetpixel($return_img, $x, $y, $return_color);
      }
    }
    return $return_img;
  }

  function _get_ave_color($color_a, $color_b, $alpha_level) {
    return round((($color_a * (1 - $alpha_level)) + ($color_b * $alpha_level)));
  }

  function _get_image_color($im, $r, $g, $b, $alpha) {
    $c = imagecolorexactalpha($im, $r, $g, $b, $alpha);
    if ($c != -1)
      return $c;
    $c = imagecolorallocate($im, $r, $g, $b, $alpha);
    if ($c != -1)
      return $c;
    return imagecolorclosest($im, $r, $g, $b, $alpha);
  }
}
/****************************************************************************************************************************************/
