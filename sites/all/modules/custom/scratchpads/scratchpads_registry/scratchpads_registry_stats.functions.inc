<?php

/**
 * Create an organization that a Scratchpad is linked to.
 */
function scratchpads_registry_stats_create_organization($uuid){
  $organization = new GBIF\Organization($uuid);
}