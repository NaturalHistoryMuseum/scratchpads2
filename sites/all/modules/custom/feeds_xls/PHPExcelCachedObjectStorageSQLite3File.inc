<?php

echo "HERE";exit;

/**
 * A very basic class to extend the PHPExcel_CachedObjectStorage_SQLite3 class,
 * but instead use an SQLite database saved as a file
 * . 
 * @author SDRycroft
 *
 */
class PHPExcel_CachedObjectStorage_SQLite3File extends PHPExcel_CachedObjectStorage_SQLite3 implements PHPExcel_CachedObjectStorage_ICache{

  /**
	 */
  public function __construct(PHPExcel_Worksheet $parent){
    parent::__construct($parent);
    if(is_null($this->_DBHandle)){
      $this->_TableName = str_replace('.', '_', $this->_getUniqueID());
      $_DBName = tempnam(sys_get_temp_dir(), 'PHPExcel_SQLite3File');
      $this->_DBHandle = new SQLite3($_DBName);
      if($this->_DBHandle === false)
        throw new Exception($this->_DBHandle->lastErrorMsg());
      if(!$this->_DBHandle->exec('CREATE TABLE kvp_' . $this->_TableName . ' (id VARCHAR(12) PRIMARY KEY, value BLOB)'))
        throw new Exception($this->_DBHandle->lastErrorMsg());
    }
  }
}