<?php
// $Id: flot-views-style.tpl.php,v 1.1.2.1 2009/09/21 17:00:41 yhahn Exp $

/**
 * @file flot-views-summary-style.tpl.php
 * Template to display a flot summary view.
 *
 * - $element : An array representation of the flot DOM element.
 * - $data: A flotData object.
 * - $options: A flotStyle object.
 */
?>

<div class="views-flot">
  <?php
    $vars = array('element' => $element, 'data' => $data, 'options' => $options, 'zoom' => $addselectionfilter);
    print theme('flot_graph', $vars);
  ?>
</div>
