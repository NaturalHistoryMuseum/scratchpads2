<?php

/**
 * Destination class implementing migration into comments.
 * 
 */
class ScratchpadsMigrateDestinationComment extends MigrateDestinationComment{

  public function import(stdClass $comment, stdClass $row){
    // As we're using the VID, the NID gets set wrong
    // Set back to the orginal NID
    $comment->nid = $row->nid;
    return parent::import($comment, $row);
  }
}
