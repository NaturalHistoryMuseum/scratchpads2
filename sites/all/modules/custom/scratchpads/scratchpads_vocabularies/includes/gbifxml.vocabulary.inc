<?php
namespace ScratchpadsVocabularies;
use Exception;
require_once str_replace('gbifxml.vocabulary.inc', 'classes.inc', __FILE__);

/**
 * GbifXmlVocabulary
 * 
 * This class implements ScratchpadsVocabularies\VocabularyInterface for Gbif vocabularies
 * described using an XML file.
 *
 */
class GbifXmlVocabulary extends Persistent implements VocabularyInterface{

  protected $url;

  protected $version;

  protected $structure;

  protected $information;

  /**
   * __construct
   * 
   * Create a new GbifXmlVocabulary from a URL to an
   * XML file (presumably on rs.gbif.org)
   */
  public function __construct($url){
    // Load the information
    $this->url = $url;
    $this->persistentKey($url);
    $this->persistentProperties(array(
      'url',
      'version',
      'structure',
      'information'
    ));
    $this->loadPersistentProperties();
    if($this->structure == NULL){
      $this->refresh();
    }
  }

  /**
   * name()
   *
   * Return the human readable name of this vocabulary
   */
  public function name(){
    return $this->information['title'];
  }

  /**
   * id()
   *
   * Return the id of this vocabulary
  */
  public function id(){
    return $this->url;
  }

  /**
   * version()
   *
   *  Returns the version of the vocabulary
   *  represented by this object.
  */
  public function version(){
    return $this->version;
  }

  /**
   * structure()
   *
   * Return a ScratchpadsVocabularies\VocabularyStructure object
  */
  public function structure(){
    return $this->structure;
  }

  /**
   * description()
   *
   * Return advanced information about this vocabulary
   */
  public function description(){
    return check_plain($this->information['description']) . '<br/>' . l($this->information['uri'], $this->information['uri']);
  }

  /**
   * refresh
   *
   * Re-fetch and parse the source XML
   */
  public function refresh(){
    // See if there is a new version
    $this->dom = simplexml_load_file($this->url);
    if($this->dom === false){throw new Exception("Could not load/parse GbifXmlVocabulary at " . $this->url);}
    $this->version = NULL;
    if(isset($http_response_header)){
      foreach($http_response_header as $header){
        if(preg_match('/^Last-Modified:(.+)$/', $header, $matches)){
          $this->version = $matches[1];
          break;
        }
      }
    }
    if($this->version === NULL){
      $this->version = md5($this->dom->asXML());
    }
    $this->parseXml();
    $this->savePersistentProperties();
  }

  /**
   * parseXml
   * 
   * Parse the loaded XML data in a structure
   */
  protected function parseXml(){
    // Parse the general information
    $this->information = array(
      'uri' => (string)$this->dom->attributes('dc', TRUE)->URI,
      'title' => (string)$this->dom->attributes('dc', TRUE)->title,
      'description' => (string)$this->dom->attributes('dc', TRUE)->description
    );
    // Parse the concepts
    $attr_map = array(
      array(
        'name' => 'identifier',
        'namespace' => 'dc',
        'map' => 'id',
        'required' => TRUE
      ),
      array(
        'name' => 'URI',
        'namespace' => 'dc',
        'map' => 'uri',
        'default' => ''
      ),
      array(
        'name' => 'relation',
        'namespace' => 'dc',
        'map' => 'relation',
        'default' => ''
      ),
      array(
        'name' => 'description',
        'namespace' => 'dc',
        'map' => 'description',
        'default' => ''
      )
    );
    $items = array();
    $item_data = array();
    foreach($this->dom->concept as $element){
      $info = array(
        'terms' => array()
      );
      // Parse the attributes
      foreach($attr_map as $attr => $map_info){
        if(isset($map_info['namespace'])){
          $element_attr = $element->attributes($map_info['namespace'], TRUE)->{$map_info['name']};
        }else{
          $element_attr = $element[$map_info['name']];
        }
        if($element_attr !== NULL){
          $info[$map_info['map']] = (string)($element_attr);
        }else if(!empty($map_info['required'])){
          throw new Exception("Missing required attribute $attr");
        }else if(!empty($map_info['default'])){
          $info[$map_info['map']] = $map_info['default'];
        }
      }
      // Parse the label
      foreach($element->preferred->term as $term){
        $term_title = $term->attributes('dc', TRUE)->title;
        $term_lang = $term->attributes('xml', TRUE)->lang;
        $term_source = $term->attributes('dc', TRUE)->source;
        if($term_title === NULL || $term_lang === NULL){throw new Exception("Missing title/lang for preferred term title");}
        $info['terms'][(string)$term_lang] = array(
          'title' => (string)$term_title,
          'source' => (string)$term_source
        );
      }
      if(empty($info['terms']['en'])){
        throw new Exception("No english translation for term");
      }else{
        $info['label'] = $info['terms']['en']['title'];
        t($info['label']); // Pity we can't easily provide the translations, as we have them at hand!
      }
      // See if we can find the wiki link
      $info['wiki'] = '';
      if (isset($info['uri']) && preg_match('%([^/]+)/([^/]+)$%', $info['uri'], $matches)) {
        $wiki = 'http://terms.gbif.org/wiki/' . $matches[1] . ':' . $matches[2];
        $headers = @get_headers($wiki);
        if (strpos($headers[0], '200') !== FALSE) {
          $info['wiki'] = $wiki;
        }
      }
      $info['render'] = theme('scratchpads_vocabularies_gbif_term', $info);
      $items[$info['id']] = $info['label'];
      $item_data[$info['id']] = $info;
    }
    $this->structure = new VocabularyStructure($items, $item_data);
  }
}