<?php
/**
 * Works with a Name object to parse out the parts of a name.
 *
 * Example usage:
 *		$parser = new Parser("John Q. Smith");
 *		echo  $parser->getLast() . ", " . $parser->getFirst();
 *		//returns "Smith, John"
 *
 *
 */
class HumanNameParser_Parser {
  private $name;
  private $nameParts = array();
  private $leadingInit;
  private $first;
  private $nicknames;
  private $middle;
  private $last;
  private $suffix;
  private $category;
  private $type;
  private $literal;

  private $suffixes;
  private $prefixes;

  /*
   * Constructor
  *
  * @param	mixed $name	Either a name as a string or as a Name object.
  */
  public function __construct($name = NULL)
  {
    $this->suffixes = array('esq','esquire','jr','sr','2','ii','iii','iv');
    $this->prefixes = array('bar','ben','bin','da','dal','de la', 'de la Rue du', 'de', 'del','der','di',
        'ibn','la','le','san','st','ste','van', 'van der', 'van den', 'vel','von');
    $this->setName($name);
  }

  public function parseName($name = NULL, $category = NULL) {
    $this->literal = 0;
    $this->category = 1;
    $this->type = 1;
    if (is_array($name) && isset($name['name'])) {
      if (isset($name['auth_category']) && !empty($name['auth_category']) && empty($category)) {
        $this->category = $name['auth_category'];
      }
      elseif (!empty($category)) {
        $this->category = $category;
      }
      if (isset($name['auth_type']) && !empty($name['auth_type'])) {
        $this->type = $name['auth_type'];
      }
      $this->nameParts = $name;
      $this->setName($name['name'], $category);
    }
    else {
      $this->nameParts['name'] = $name;
      $this->setName($name, $category);
    }

    return $this->getArray();
  }
  /**
   * Sets name string and parses it.
   * Takes Name object or a simple string (converts the string into a Name obj),
   * parses and loads its constituant parts.
   *
   * @param	mixed $name	Either a name as a string or as a Name object.
   */
  public function setName($name = NULL, $category = NULL){
    if ($name) {
      $this->category == $category;

      if (is_object($name) && get_class($name) == "HumanNameParser_Name") { // this is mostly for testing
        $this->name = $name;
      }
      elseif (is_array($name) && isset($name['name'])) {
        $this->name = new HumanNameParser_Name($name['name']);
        $this->nameParts = $name;
      }
      else {
        $this->name = new HumanNameParser_Name($name);
      }

      $this->leadingInit = "";
      $this->first = "";
      $this->nicknames = "";
      $this->middle = "";
      $this->last = "";
      $this->suffix = "";

      if ($this->category == 5 || $this->type == 5) {
        $this->last = $name;
        $this->literal = TRUE;
      }
      else {
        $this->parse();
      }

    }
  }

  public function getleadingInit() {
    return $this->leadingInit;
  }
  public function getFirst() {
    return $this->first;
  }
  public function getNicknames() {
    return $this->nicknames;
  }

  public function getMiddle() {
    return $this->middle;
  }

  public function getLast() {
    return $this->last;
  }

  public function getSuffix() {
    return $this->suffix;
  }
  public function getName(){
    return $this->name;
  }

  /**
   * returns all the parts of the name as an array
   *
   * @param String $arrType pass 'int' to get an integer-indexed array (default is associative)
   * @return array An array of the name-parts
   */
  public function getArray($arrType = 'assoc') {
    $arr = array();
    $arr['prefix']    = $this->leadingInit;
    $arr['firstname'] = $this->first;
    $arr['nicknames'] = $this->nicknames;
    $arr['initials']  = substr($this->middle, 0, 10);
    $arr['lastname']  = $this->last;
    $arr['suffix']    = $this->suffix;
    $arr['md5']       = md5(json_encode($arr));
    $arr['literal']   = $this->literal;

    if ($arrType == 'assoc') {
      return array_merge($this->nameParts, $arr);
    }
    else if ($arrType == 'int'){
      return array_values($arr);
    }
    else {
      throw new Exception("Array must be associative ('assoc') or numeric ('num').");
    }
  }

  /*
   * Parse the name into its constituent parts.
  *
  * Sequentially captures each name-part, working in from the ends and
  * trimming the namestring as it goes.
  *
  * @return boolean	true on success
  */
  private function parse()
  {
    $suffixes = implode("\.*|\s", $this->suffixes) . "\.*"; // each suffix gets a "\.*" behind it.
    $prefixes = implode(" |", $this->prefixes) . " "; // each prefix gets a " " behind it.

    // The regex use is a bit tricky.  *Everything* matched by the regex will be replaced,
    //	but you can select a particular parenthesized submatch to be returned.
    //	Also, note that each regex requres that the preceding ones have been run, and matches chopped out.
    $nicknamesRegex		= "/ ('|\"|\(\"*'*)(.+?)('|\"|\"*'*\)) /"; // names that starts or end w/ an apostrophe break this
    $suffixRegex			= "/,* *($suffixes)$/";
    $lastRegex 				= "/(?!^)\b([^ ]+ y |$prefixes)*[^ ]+$/u";
    $leadingInitRegex =	"/^(.\.*)(?= \p{L}{2})/"; // note the lookahead, which isn't returned or replaced
    $firstRegex				= "/^[^ ]+/"; //

    // short circuit for a simple single string that would otherwise cause an Exception;
    // we take this as the last name and everything else will be empty (the default)
    if (preg_match('@^\s*(\p{L}+)\s*$@u', $this->name->getStr(), $matches)) {
      $this->last = $matches[1];
      return true;
    }

    // get nickname, if there is one
    $this->nicknames = $this->name->chopWithRegex($nicknamesRegex, 2);

    // get suffix, if there is one
    $this->suffix = $this->name->chopWithRegex($suffixRegex, 1);

    // flip the before-comma and after-comma parts of the name
    $this->name->flip(",");

    // get the last name
    $this->last = $this->name->chopWithRegex($lastRegex, 0);
    if (!$this->last){
      throw new Exception("Couldn't find a last name in '{$this->name->getStr()}'.");
    }

    // get the first initial, if there is one
    $this->leadingInit = $this->name->chopWithRegex($leadingInitRegex, 1);

    // get the first name
    $this->first = $this->name->chopWithRegex($firstRegex, 0);
    if (!$this->first && $this->category != 5){
      throw new Exception("Couldn't find a first name in '{$this->name->getStr()}'");
    }

    // if anything's left, that's the middle name
    $this->middle = $this->name->getStr();
    return true;
  }





}
?>
