<?php

class DwCACore implements Iterator{
  // Store simple attributes from the XML file used when reading the CSV file.
  private $delimitter;

  private $enclosure;

  private $ignoreheader;
  // Store a keyed list of columns.
  private $columns;
  // Store the uri of the zip file so that the object and quickly be initalised
  // rather than requiring the zip to be created on construct.
  private $uri;
  // SimpleXMLElement object containing the contents of the meta.xml file.
  private $meta;
  // The Zip Object for the archive itself.
  private $zip;
  // String containing the directory name (if any) that contains the various
  // files within the archive.
  private $directory;
  // String containing the temporary directory name which can be deleted once
  // done.
  private $tempdir;
  // Resource which points to the core file.  This will be kept open, while the
  // extension files will be opened/closed for each iteration (TODO - 
  // benchmark performance/memory with this).
  private $core;
  // The valid function extracts the next line before the current function can
  // use it.  We therefore store the value of that line here.
  private $current_line;
  // The current position/key
  private $key;
  // The eml for the archive
  private $eml;
  // Record whether or not the instance has been initialized!
  private $initialized;
  // Store an array of extensions so that we don't need to check the XML every
  // time we need them
  private $extensions;
  // Max length of any line.  This affects memory allocation and therefore
  // performance, but may need increasing for compatability with complex DwC-A
  const MAX_LINE_LENGTH = 1000000;

  /**
   * Create a basic iterator for extracting each record from a DwC-A.
   * Essentially denormalises an archive.
   * 
   * @param String $uri URI of a DwC-A Zip file.
   */
  function __construct($uri){
    $this->uri = $uri;
    $this->initialized = FALSE;
  }

  /**
   * Destroy the object, specifically deleting the directory where the content
   * of the zip file has been extracted.
   */
  function __destruct(){
    self::delete_dir($this->tempdir);
  }

  /**
   * Helper function to delete the files
   */
  private static function delete_dir($dirPath){
    if(!is_dir($dirPath)){return;}
    if(substr($dirPath, strlen($dirPath) - 1, 1) != '/'){
      $dirPath .= '/';
    }
    $files = glob($dirPath . '*', GLOB_MARK);
    foreach($files as $file){
      if(is_dir($file)){
        self::delete_dir($file);
      }else{
        unlink($file);
      }
    }
    rmdir($dirPath);
  }

  /**
   * Return the meta variable for use by the DwCAExtension class (and possibly
   * other uses).
   */
  function get_meta(){
    $this->initialize();
    return $this->meta;
  }

  /**
   * Return the duirectory variable for use by the DwCAExtension class (and 
   * possibly other uses).
   */
  function get_directory(){
    return $this->directory;
  }

  /**
   * Return the eml for this archive, or false if there isn't one.
   */
  function get_eml(){
    $this->initialize();
    if(is_null($this->eml)){
      if(($this->eml = $this->meta->attributes()->metadata) != FALSE){
        // the XML string.
        if(($this->eml = new SimpleXMLElement(file_get_contents($this->directory . $this->eml))) == FALSE){throw new Exception('Unable to parse the eml file');}
      }
    }
    return $this->eml;
  }

  /**
   * Return the delimitter variable for use by the DwCAExtension class (and 
   * possibly other uses).
   */
  function get_delimitter(){
    return $this->delimitter;
  }

  /**
   * Return the delimitter variable for use by the DwCAExtension class (and 
   * possibly other uses).
   */
  function get_enclosure(){
    return $this->enclosure;
  }

  /**
   * Return the delimitter variable for use by the DwCAExtension class (and 
   * possibly other uses).
   */
  function get_ignoreheader(){
    return $this->ignoreheader;
  }

  /**
   * Return an array for the current line.
   */
  function current(){
    return $this->current_line;
  }

  /**
   * Return the key for the current entry
   */
  function key(){
    return $this->key;
  }

  /**
   * Return an array of extensions.
   */
  public function get_extensions(){
    $this->initialize();
    $extensions = array();
    foreach($this->extensions as $type => $location){
      $extensions[$type] = new DwCAExtension($this, $location);
    }
    return $extensions;
  }

  /**
   * Read the next CSV line in.
   */
  public function next(){
    // Read the next line in and convert it to an array keyed by the column
    // names.
    $this->current_line = fgetcsv($this->core, 0, $this->delimitter, $this->enclosure);
    if($this->current_line){
      if(count($this->columns) != count($this->current_line)){
        // Some error here about the fact that the column count doesn't match.
      }
      $this->current_line = array_combine($this->columns, $this->current_line);
      // Add each extension to the end of the array.
      foreach($this->extensions as $type => $location){
        $this->current_line[$type] = new DwCAExtension($this, $location, $this->current_line['id']);
      }
      // Increase the key
      $this->key += 1;
    }
  }

  /** 
   * Do we have a line to return?
   */
  public function valid(){
    if($this->current_line != FALSE){return TRUE;}
    return FALSE;
  }

  private function initialize(){
    if($this->initialized){return;}
    // Initialise $this->core to be FALSE - we'll open the correct file when it
    // is actually required.
    $this->core = FALSE;
    // Open the zip file, throwing an error if we fail.
    $zip = new zipArchive();
    if(!$zip->open($this->uri)){throw new Exception('Failed to open DwC-A zip file');}
    // We extract the content of the Zip file to speed up reading of files
    $this->tempdir = tempnam(sys_get_temp_dir(), 'dwca');
    unlink($this->tempdir);
    mkdir($this->tempdir);
    $this->directory = $this->tempdir . '/';
    $zip->extractTo($this->tempdir);
    // Work out the directory name for the archive.
    $num_files = $zip->numFiles;
    for($i = 0; $i < $num_files; $i++){
      $stat = $zip->statIndex($i);
      if(strtolower(substr($stat['name'], -8)) == 'meta.xml'){
        // Work out the directory name from the path of the meta.xml file.
        $this->directory .= substr($stat['name'], 0, strpos(strtolower($stat['name']), 'meta.xml'));
        // Read in the contents of the meta and create the SimpleXML object from
        // the XML string.
        if(($this->meta = new SimpleXMLElement(file_get_contents($this->directory . 'meta.xml'))) == FALSE){throw new Exception('Unable to parse the meta.xml file');}
        foreach($this->meta->extension as $extension){
          $this->extensions[(string)$extension->attributes()->rowType] = (string)$extension->files->location;
        }
        // Get the value of the three simple attributes
        $this->delimitter = (string)$this->meta->core->attributes()->fieldsTerminatedBy;
        if($this->delimitter == '\t'){
          $this->delimitter = "\t";
        }
        $this->enclosure = (string)$this->meta->core->attributes()->fieldsEnclosedBy ? (string)$this->meta->core->attributes()->fieldsEnclosedBy : '"';
        $this->ignoreheader = (string)$this->meta->core->attributes()->ignoreHeaderLines;
        break;
      }
    }
    $this->initialized = TRUE;
  }

  /** 
   * Note, we do most of the processing here, rather than at __construct, as
   * we may not actually iterate over the DwCA so this saves some processing if
   * that happens.
   * 
   * Reset the file pointer to the start, and load in the first line by calling
   * next().
   */
  public function rewind(){
    $this->initialize();
    // Create an array keyed by the index in the file, or column types.
    $this->columns = array(
      (string)$this->meta->core->id->attributes()->index => 'id'
    );
    foreach($this->meta->core->field as $field){
      $this->columns[(string)$field->attributes()->index] = (string)$field->attributes()->term;
    }
    ksort($this->columns);
    // Get a file stream for the core CSV file.
    if(($this->core = fopen($this->directory . $this->meta->core->files->location, 'r')) === FALSE){throw new exception('Unable to load the core data file');}
    // Set the key to be "-1" so that it is increased to "0" when the next()
    // function is called.
    $this->key = -1;
    // Read the next line in.
    $this->next();
    // If we need to ignore the header line, we read in another line, and reset
    // the key.
    if($this->ignoreheader){
      $this->key = -1;
      $this->next();
    }
  }
}

class DwCAExtension implements Iterator{
  // Store simple attributes from the XML file used when reading the CSV file.
  private $delimitter;

  private $enclosure;

  private $ignoreheader;
  // Store the DwCACore object so that we can pull in the meta, directory and
  // other variables.
  private $core;
  // Store a keyed list of columns.
  private $columns;
  // Store the key of the coreid so that we don't need to call array_combine on
  // every row that we read.
  private $coreid_index;
  // Store the coreid that we need to find in the extension file.
  private $coreid;
  // Name of the extension to open
  private $extension_name;
  // Resource which points to the core file.  This will be kept open, while the
  // extension files will be opened/closed for each iteration (TODO -
  // benchmark performance/memory with this).
  private $extension;
  // The valid function extracts the next line before the current function can
  // use it.  We therefore store the value of that line here.
  private $current_line;
  // The current key
  private $key;
  // The current position in an array of lines.  This is needed, as a line may
  // have matched a coreid incorrectly.
  private $position;
  // Max length of any line.  This affects memory allocation and therefore
  // performance, but may need increasing for compatability with complex DwC-A
  const MAX_LINE_LENGTH = 1000000;
  // Max size of any extension file to read in.  If files are bigger than this,
  // then we read in the file line by line.  If smaller, we read in ALL lines
  // at once.
  const MAX_FILE_SIZE_IN_MEMORY = 16000000;

  /**
   * Create a basic iterator for extracting each record from a DwC-A.
   * Essentially denormalises an archive.
   */
  function __construct($core, $extension, $coreid = FALSE){
    $this->core = $core;
    $this->extension_name = $extension;
    $this->coreid = $coreid;
  }

  /**
   * Return an array for the current line.
   */
  function current(){
    return $this->current_line;
  }

  /**
   * Return the key for the current entry
   */
  function key(){
    return $this->key;
  }

  /**
   * Read the next CSV line in.
   */
  public function next(){
    // We switch what we're doing based on whether or not the file had already
    // been opened (which it will have been if it is too big to load into
    // memory.
    $this->position += 1;
    $this->current_line = FALSE;
    if(is_array($this->extension) && count($this->extension) && isset($this->extension[$this->position])){
      // We have an array of lines, we simply take the next line, increase the
      // counter convert to an array.
      if($this->coreid){
        $coreid_not_found = TRUE;
        while($coreid_not_found && isset($this->extension[$this->position])){
          $temp_line = $this->extension[$this->position];
          $temp_line = str_getcsv($temp_line, $this->delimitter, $this->enclosure);
          if($this->coreid && $temp_line[$this->coreid_index] == $this->coreid){
            $this->current_line = $temp_line;
            $this->position -= 1;
            $coreid_not_found = FALSE;
          }
          $this->position += 1;
        }
      }else{
        $coreid_not_found = FALSE;
        $this->current_line = $this->extension[$this->position];
        $this->current_line = str_getcsv($this->current_line, $this->delimitter, $this->enclosure);
      }
      if(!$coreid_not_found){
        $this->key += 1;
        $this->current_line = array_intersect_key($this->current_line, $this->columns);
        $this->current_line = array_combine($this->columns, $this->current_line);
      }
    }elseif(is_object($this->extension)){
      while(($line = fgetcsv($this->extension, 0, $this->delimitter, $this->enclosure)) != FALSE){
        if($line[$this->coreid_index] == $this->coreid || !$this->coreid){
          $this->current_line = array_intersect_key($this->current_line, $this->columns);
          $this->current_line = array_combine($this->columns, $this->current_line);
          $this->key += 1;
          break;
        }
      }
    }
  }

  /**
   * Do we have a line to return?
   */
  public function valid(){
    if($this->current_line != FALSE){return TRUE;}
    return FALSE;
  }

  /**
   * Note, we do most of the processing here, rather than at __construct, as
   * we may not actually iterate over the DwCA so this saves some processing if
   * that happens.
   *
   * Reset the file pointer to the start, and load in the first line by calling
   * next().
   */
  public function rewind(){
    $meta = $this->core->get_meta();
    foreach($meta->extension as $extension){
      if((string)$extension->files->location == $this->extension_name){
        $this->columns = array(
          (string)$extension->coreid->attributes()->index => 'coreid'
        );
        foreach($extension->field as $field){
          $this->columns[(string)$field->attributes()->index] = (string)$field->attributes()->term;
        }
        $this->coreid_index = (string)$extension->coreid->attributes()->index;
        break;
      }
    }
    // Get the basic attributes from the parent object.
    $this->delimitter = $this->core->get_delimitter();
    $this->enclosure = $this->core->get_enclosure();
    $this->ignoreheader = $this->core->get_ignoreheader();
    // Get a file stream for the extension CSV file.
    if(filesize($this->core->get_directory() . $this->extension_name) > DwCAExtension::MAX_FILE_SIZE_IN_MEMORY){
      // The file is too big, so we read it line by line.
      if(($this->extension = fopen($this->core->get_directory() . $this->extension_name, 'r')) === FALSE){throw new exception('Unable to load the extension data file');}
    }else{
      // We load in all of the file into an array, filtering it by the coreid.
      if($this->coreid){
        $this->extension = array_values(preg_grep("/{$this->coreid}/", file($this->core->get_directory() . $this->extension_name)));
      }else{
        $this->extension = file($this->core->get_directory() . $this->extension_name);
      }
    }
    // Set the key to be "-1" so that it is increased to "0" when the next()
    // function is called.
    $this->key = -1;
    $this->position = -1;
    // Read the next line in.
    $this->next();
    // If we need to ignore the header line, we read in another line, and reset
    // the key.
    if($this->ignoreheader){
      $this->key = -1;
      $this->position = -1;
      $this->next();
    }
  }
}