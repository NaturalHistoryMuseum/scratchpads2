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
function gbif_registry_autload($class_name)
{
    if (substr($class_name, 0, 4) == 'GBIF') {
        $class_parts = explode('\\', $class_name);
        @include_once "classes/{$class_parts[1]}.class.inc";
    }
}

/**
 * A specific Drupal class that extends the GBIF\Registry class, but adds
 * specific Drupal functionality like caching.
 */
class GBIFRegistry extends GBIF\Registry
{

    /**
     * Basic singleton code copied straight from php.net
     */
    public static function singleton()
    {
        if (!function_exists('curl_init')) {
            throw new Exception('CURL functions not found');
        }
        if (!isset(self::$instance)) {
            $className = __class__;
            self::$instance = new $className();
      // We try to set the username/password at this point by loading them from
      // a configuration file.
            $ini = @parse_ini_file(drupal_get_path('module', 'gbif_registry') . '/login.ini');
            $ini = variable_get('scratchpads_gbif_registry_ini', $ini);
            if (isset($ini['username']) && isset($ini['password'])) {
                self::$instance->setUser($ini['username']);
                self::$instance->setPassword($ini['password']);
            }
      // We set the Registry URL based on the settings
            self::$instance->setRegistryURL(variable_get('gbif_registry_web_service_url', 'http://api.gbif.org/v1/'));
        }
        return self::$instance;
    }

    /**
     * Override the getRegistryCache() function to allow us to return a Drupal
     * specific RegistryCache class.
     */
    public function getRegistryCache()
    {
        if (!$this->registryCache) {
            $this->registryCache = new DrupalRegistryCache();
        }
        return $this->registryCache;
    }

    /**
     * Override the getNodes() function, to enable caching.
     */
    public function getNodes($tag = false, $name = false)
    {
        $rc = $this->getRegistryCache();
        $cid = "all_nodes:$tag:$name";
        if ($cache = $rc->retrieve($cid)) {
            return $cache;
        }
        $nodes = parent::getNodes();
        $cache_nodes = array();
        foreach ($nodes as $node) {
            $cache_nodes[] = $node;
        }
        if (count($cache_nodes)) {
            $rc->create($cid, $cache_nodes);
        }
        return $cache_nodes;
    }

    /**
     * Same again, but returning installations
     */
    public function getInstallations($tag = false, $name = false, $isoCountryCode = false)
    {
        $rc = $this->getRegistryCache();
        $cid = "all_installations:$tag:$name:$isoCountryCode";
        if ($cache = $rc->retrieve($cid)) {
            return $cache;
        }
        $tis = parent::getInstallations();
        $cache_tis = array();
        foreach ($tis as $ti) {
            $cache_tis[] = $ti;
        }
        if (count($cache_tis)) {
            $rc->create($cid, $cache_tis);
        }
        return $cache_tis;
    }

    /**
     * Override the getOrganizations() function, to enable caching.
     */
    public function getOrganizations($tag = false, $name = false, $isoCountryCode = false)
    {
        $rc = $this->getRegistryCache();
        $cid = "all_organizations:$tag:$name:$isoCountryCode";
        if ($cache = $rc->retrieve($cid)) {
            return $cache;
        }
        $organizations = parent::getOrganizations();
        $cache_organizations = array();
        foreach ($organizations as $organization) {
            $cache_organizations[] = $organization;
        }
        if (count($cache_organizations)) {
            $rc->create($cid, $cache_organizations);
        }
        return $cache_organizations;
    }
}