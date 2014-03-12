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
        '#markup' => '<iframe src="http://dev.at.biovel.eu/runs/new?embedded=true&version=1&workflow_id=107&input_url=' . url('taxonomy/term/' . arg(2) . '/enm.csv', array(
          'absolute' => TRUE
        )) . '" width="100%" height="800px"/>'
      )
    )
  );
}