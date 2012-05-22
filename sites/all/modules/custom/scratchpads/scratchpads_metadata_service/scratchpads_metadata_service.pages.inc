<?php

/**
 * Callback to provide metadata as JSON about this Scratchpad.
 */
function scratchpads_metadata_service_json_page(){}

/**
 * Helper function that builds the metadata about this Scratchpad.
 */
function scratchpads_metadata_service_get_metadata(){
  $entity_info = entity_get_info();
}