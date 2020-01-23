<?php

/**
 * 
 */
function hook_dwcarchiver_core_types_alter(&$core_types){
  
}

/**
 * Hook called when an archive is rebuilt - see dwcarchiver_rebuild
 *
 * @param string $did The ID/machine name of the archive (did field in dwcarchiver_archive table)
 * @param object $dwarchiver The dwarchvier object as returned from dwcarchiver_load
 * @return void
 */
function hook_dwcarchiver_rebuild($did, $dwarchiver){

}
