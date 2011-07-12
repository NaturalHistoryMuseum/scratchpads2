<?php

/**
 * @file
 * Class definition of FeedsFeedNodeProcessor.
 */

/**
 * Creates *feed* nodes from feed items. The difference to FeedsNodeProcessor is
 * that this plugin only creates nodes that are feed nodes themselves.
 */
class FeedsFeedNodeProcessor extends FeedsProcessor {

  /**
   * Implementation of FeedsProcessor::process().
   */
  public function process(FeedsImportBatch $batch, FeedsSource $source) {
    while ($item = $batch->shiftItem()) {

      // If the target item does not exist OR if update_existing is enabled,
      // map and save.
      if (!$nid = $this->existingItemId($batch, $source) || $this->config['update_existing']) {

        // Map item to a node.
        $node = $this->map($batch);

        // If updating populate nid and vid avoiding an expensive node_load().
        if (!empty($nid)) {
          $node->nid = $nid;
          $node->vid = db_result(db_query("SELECT vid FROM {node} WHERE nid = %d", $nid));
        }

        // Save the node.
        node_save($node);

        if ($nid) {
          $batch->updated++;
        }
        else {
          $batch->created++;
        }
      }
    }

    // Set messages.
    if ($batch->created) {
      drupal_set_message(format_plural($batch->created, 'Created @number @type node.', 'Created @number @type nodes.', array('@number' => $batch->created, '@type' => $this->config['content_type'])));
    }
    elseif ($batch->updated) {
      drupal_set_message(format_plural($batch->updated, 'Updated @number @type node.', 'Updated @number @type nodes.', array('@number' => $batch->updated, '@type' => $this->config['content_type'])));
    }
    else {
      drupal_set_message(t('There is no new content.'));
    }
  }

  /**
   * Implementation of FeedsProcessor::clear().
   */
  public function clear(FeedsBatch $batch, FeedsSource $source) {
    // Do not support deleting imported items as we would have to delete all
    // items of the content type we imported which may contain nodes that a
    // user created by hand.
    throw new Exception(t('This configuration does not support deleting imported items.'));
  }

  /**
   * Execute mapping on an item.
   */
  protected function map(FeedsImportBatch $batch, $target_node = NULL) {
    static $included;
    if (!$included) {
      module_load_include('inc', 'node', 'node.pages');
      $included = TRUE;
    }

    // Prepare node object.
    if (!isset($target_node)) {
      $target_node = new stdClass();
    }

    $target_node->type = $this->config['content_type'];
    $target_node->feeds = array();
    // Suppress auto import, we may be creating many feeds
    $target_node->feeds['suppress_import'] = TRUE;
    node_object_prepare($target_node);

    /*
    Assign an aggregated node always to current user.
    @todo This won't work in all cases as the assumption here is that
    import is happening one off when user is logged in. Assumption breaks if
    feed node processor is being used for aggregation on cron time and a
    specific user should still be the owner of the imported feed nodes.
    */
    global $user;
    $target_node->uid = $user->uid;

    // Have parent class do the iterating.
    return parent::map($batch, $target_node);
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'content_type' => '',
      'update_existing' => 0,
      'mappings' => array(),
    );
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $feeds = feeds_importer_load_all();
    $types = array();
    foreach ($feeds as $feed) {
      if ($feed->id != $this->id && !empty($feed->config['content_type'])) {
        $types[$feed->config['content_type']] = node_get_types('name', $feed->config['content_type']);
      }
    }
    if (empty($types)) {
      $types[''] = t('No feed node content type available');
    }
    else {
      $types = array(
        '' => t('Select'),
      ) + $types;
    }
    $form = array();
    $form['content_type'] = array(
      '#type' => 'select',
      '#title' => t('Content type'),
      '#description' => t('Choose node type to create from this feed. Only node types with attached importer configurations are listed here. <strong>Note:</strong> Users with "import !feed_id feeds" permissions will be able to <strong>import</strong> nodes of the content type selected here regardless of the node level permissions. However, users with "clear !feed_id permissions" need to have sufficient node level permissions to delete the imported nodes.', array('!feed_id' => $this->id)),
      '#options' => $types,
      '#default_value' => $this->config['content_type'],
    );
    // @todo Implement real updating.
    $form['update_existing'] = array(
      '#type' => 'checkbox',
      '#title' => t('Replace existing feed nodes'),
      '#description' => t('If an existing node is found for an imported node, replace it. Existing nodes will be determined using mappings that are a "unique target".'),
      '#default_value' => $this->config['update_existing'],
    );
    return $form;
  }

  /**
   * Override setTargetElement to operate on a target item that is a node.
   */
  public function setTargetElement(&$target_node, $target_element, $value) {
    if ($target_element == 'source') {
      // Get the class of the feed node importer's fetcher and set the source
      // property. See feeds_nodeapi() how $node->feeds gets stored.
      $class = get_class($this->feedNodeImporter()->fetcher);
      $target_node->feeds[$class]['source'] = $value;
    }
    elseif ($target_element == 'body') {
      $target_node->teaser = $value;
      $target_node->body = $value;
    }
    elseif (in_array($target_element, array('title', 'status', 'created'))) {
      $target_node->$target_element = $value;
    }
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets = array(
      'title' => array(
        'name' => t('Title'),
        'description' => t('The title of the feed node.'),
       ),
      'status' => array(
        'name' => t('Published status'),
        'description' => t('Whether a feed node is published or not. 1 stands for published, 0 for not published.'),
      ),
      'created' => array(
        'name' => t('Published date'),
        'description' => t('The UNIX time when a node has been published.'),
      ),
      'body' => array(
        'name' => t('Body'),
        'description' => t('The body of the node. The teaser will be the same as the entire body.'),
      ),
      'source' => array(
        'name' => t('Feed source'),
        'description' => t('Depending on the selected fetcher, this could be for example a URL or a path to a file.'),
        'optional_unique' => TRUE,
      ),
    );
    return $targets;
  }

  /**
   * Get nid of an existing feed item node if available.
   */
  protected function existingItemId(FeedsImportBatch $batch, FeedsSource $source) {

    // We only support one unique target: source
    foreach ($this->uniqueTargets($batch) as $target => $value) {
      if ($target == 'source') {
        return db_result(db_query("SELECT fs.feed_nid FROM {node} n JOIN {feeds_source} fs ON n.nid = fs.feed_nid WHERE fs.id = '%s' AND fs.source = '%s'", $this->feedNodeImporter()->id, $value));
      }
    }
    return 0;
  }

  /**
   * Helper for retrieving the importer object for the feed nodes to produce.
   */
  protected function feedNodeImporter() {
   if ($id = feeds_get_importer_id($this->config['content_type'])) {
      return feeds_importer($id);
    }
    else {
      throw new Exception(t('Content type to be created is not a valid Feed content type.'));
    }
  }
}
