<?php

/**
 * Implements hook_block_view().
 */
function taverna_overlay_page($tid){
  // FIXME - dynamically set the height of the block.
  return array(
    'subject' => t('Run an Ecological Niche Model via BioVeL'),
    'content' => array(
      'iframe' => array(
        '#markup' => '<iframe src="http://player.at.biovel.eu/workflows/5/runs/new?embedded=true&url=' . url('taxonomy/term/' . arg(2) . '/enm.csv', array(
          'absolute' => TRUE
        )) . '" width="100%" height="800px"/>'
      )
    )
  );
}