<?php

/**
 * Parses data fetched from an IMU database.
 */
class FeedsIMUspecimensParser extends FeedsParser{

  private $state;

  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result){
    // Get config and current state
    $source_config = $source->getConfigFor($this);
    $this->state = $source->state(FEEDS_PARSE);
    // Construct the standard form of the parsed feed
    /*
     * $result = new FeedsParserResult(); $result->title = '';
     * $result->description = ''; $result->link = '';
     */
    // Iterate through the fetcher results
    $total = 0;
    $fetched = $fetcher_result->getRaw();
    $imuHits = $fetched->hits;
    $imuRows = $fetched->rows;
    foreach($imuRows as $index => $row){
      $total++;
      if($index >= $this->state->pointer && $index < ($this->state->pointer + 10)){
        $rows[] = (array)$row;
      }
    }
    $this->state->total = $total;
    // Create a result object and return it.
    $this->state->pointer += count($rows);
    // Report progress.
    $this->state->progress($total, $this->state->pointer);
    // Return the results
    return new FeedsParserResult($rows);
  }

  /**
   * Override parent::getMappingSources().
   */
  public function getMappingSources(){
    return parent::getMappingSources();
  }

  /**
   * Override parent::getSourceElement() to use only lower keys.
   */
  public function getSourceElement(FeedsSource $source, FeedsParserResult $result, $element_key){
    return parent::getSourceElement($source, $result, drupal_strtolower($element_key));
  }

  /**
   * Define defaults.
   */
  public function sourceDefaults(){
    return array();
  }

  /**
   * Source form.
   */
  public function sourceForm($source_config){
    $form = array();
    return $form;
  }

  /**
   * Define default configuration.
   */
  public function configDefaults(){
    return array();
  }

  /**
   * Build configuration form.
   */
  public function configForm(&$form_state){
    $form = array();
    $form['description'] = array(
      '#type' => 'markup',
      '#markup' => t('Use the fetcher to set the host and port of your KEMU server.')
    );
    return $form;
  }

  /**
   * Validate entered query to make sure it works.
   */
  public function configFormValidate(&$config){}
}
