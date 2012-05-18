<?php
/**
 * Autoload register and associated function to allow us to namespace our GBIF
 * classes (Drupal's autoload functionality does not appear to cope with
 * namespaced classes.
 */
spl_autoload_register('gbif_registry_autload');

/**
 * Autoload function that allows us to load Namespace'd classes.
 */
function gbif_registry_autload($class_name){
  if(substr($class_name, 0, 4) == 'GBIF'){
    $class_parts = explode('\\', $class_name);
    include_once "classes/{$class_parts[1]}.class.inc";
  }
}

/**
 * A specific Drupal class that extends the GBIF\Registry class, but adds
 * specific Drupal functionality like caching.
 */
class GBIFRegistry extends GBIF\Registry{

  /**
   * Override the getNodes() function, to enable caching.
   */
  public function getNodes(){
    $cache = cache_get('all_nodes', 'cache_gbif_registry');
    if($cache){
      if($cache->expire > time()){return $cache->data;}
    }
    $nodes = parent::getNodes();
    $cache_nodes = array();
    foreach($nodes as $node){
      $cache_nodes[] = $node;
    }
    cache_set('all_nodes', $cache_nodes, 'cache_gbif_registry', time() + 3600);
    return $cache_nodes;
  }

  /**
   * Override the getOrganizations() function, to enable caching.
   */
  public function getOrganizations(){
    $cache = cache_get('all_organizations', 'cache_gbif_registry');
    if($cache){
      if($cache->expire > time()){return $cache->data;}
    }
    $organizations = parent::getOrganizations();
    $cache_organizations = array();
    foreach($organizations as $organization){
      $cache_organizations[] = $organization;
    }
    cache_set('all_organizations', $cache_organizations, 'cache_gbif_registry', time() + 3600);
    return $cache_organizations;
  }
}