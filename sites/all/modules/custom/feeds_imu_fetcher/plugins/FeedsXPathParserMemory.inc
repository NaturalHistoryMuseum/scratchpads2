<?php

/**
 * @files
 * Provides the FeedsXPathParserMemory class.
 */
class FeedsXPathParserMemory extends FeedsXPathParserBase{

  /**
   * Implements FeedsXPathParserBase::setup().
   */
  protected function setup($source_config, FeedsFetcherResult $fetcher_result){
    if(!empty($source_config['exp']['tidy'])){
      $config = array(
        'input-xml' => TRUE,
        'wrap' => 0,
        'tidy-mark' => FALSE
      );
      // Default tidy encoding is UTF8.
      $encoding = $source_config['exp']['tidy_encoding'];
      $raw = tidy_repair_string(trim($fetcher_result->getRaw()), $config, $encoding);
    }else{
      $raw = $fetcher_result->getRaw();
    }
    $doc = new DOMDocument();
    $use = $this->errorStart();
    $success = $doc->loadXML($raw);
    unset($raw);
    $this->errorStop($use, $source_config['exp']['errors']);
    if(!$success){throw new Exception(t('There was an error parsing the XML document.'));}
    return $doc;
  }

  protected function getRaw(DOMNode $node){
    return $this->doc->saveXML($node);
  }

  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result){
    // Increase the memory limit to same set for feedsxls
    ini_set('memory_limit', '1500M');
    ini_set('max_execution_time', 240);
    ini_set("default_socket_timeout", 300);
    ini_set("max_input_time", 300);
    $parser_result = parent::parse($source, $fetcher_result);
    return $parser_result;
  }
}