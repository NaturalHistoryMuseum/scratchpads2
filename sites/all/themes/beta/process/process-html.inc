<?php
// Hook into color.module.
if (module_exists('color')) {
  _color_html_alter($vars);
}


