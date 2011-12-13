<?php

class DummySolr {
  function getId() {
    return __CLASS__;
  }

  function getFields() {
    return (object) array(
       'is_uid' =>
      (object) array(
         'type' => 'long',
         'schema' => 'I-S----OF-----',
      ),
       'content' =>
      (object) array(
         'type' => 'text',
         'schema' => 'ITS-V---------',
      ),
       'im_3_field_tags' =>
      (object) array(
         'type' => 'long',
         'schema' => 'I-SM---OF-----',
         'dynamicBase' => 'im_*',
      ),
       'entity_type' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-S----OF----l',
      ),
       'ds_last_comment_or_change' =>
      (object) array(
         'type' => 'tdate',
         'schema' => 'ITS----OF-----',
      ),
       'nodeaccess_ari4jj_node_access_example_view' =>
      (object) array(
         'type' => 'integer',
         'schema' => 'I--M---OF-----',
         'dynamicBase' => 'nodeaccess*',
      ),
       'entity_id' =>
      (object) array(
         'type' => 'tlong',
         'schema' => 'ITS----OF-----',
      ),
       'ds_changed' =>
      (object) array(
         'type' => 'tdate',
         'schema' => 'ITS----OF-----',
      ),
       'id' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-S----OF----l',
      ),
       'timestamp' =>
      (object) array(
         'type' => 'date',
         'schema' => 'I-S----OF----l',
      ),
       'label' =>
      (object) array(
         'type' => 'text',
         'schema' => 'ITS-V--O------',
      ),
       'nodeaccess_ari4jj_node_access_example_edit' =>
      (object) array(
         'type' => 'integer',
         'schema' => 'I--M---OF-----',
         'dynamicBase' => 'nodeaccess*',
      ),
       'ds_created' =>
      (object) array(
         'type' => 'tdate',
         'schema' => 'ITS----OF-----',
      ),
       'ss_name' =>
      (object) array(
         'type' => 'text',
         'schema' => 'ITS-V---------',
      ),
       'path' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-S----OF----l',
      ),
       'taxonomy_names' =>
      (object) array(
         'type' => 'text',
         'schema' => 'IT-MV--O------',
      ),
       'bundle' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-S----OF----l',
      ),
       'tid' =>
      (object) array(
         'type' => 'long',
         'schema' => 'I-SM---OF-----',
      ),
       'is_tnid' =>
      (object) array(
         'type' => 'long',
         'schema' => 'I-S----OF-----',
      ),
       'nodeaccess_ari4jj_node_access_example_author' =>
      (object) array(
         'type' => 'integer',
         'schema' => 'I--M---OF-----',
         'dynamicBase' => 'nodeaccess*',
      ),
       'tm_vid_1_names' =>
      (object) array(
         'type' => 'text',
         'schema' => 'ITSMV---------',
         'dynamicBase' => 'tm_*',
      ),
       'spell' =>
      (object) array(
         'type' => 'textSpell',
         'schema' => 'ITSM----------',
      ),
       'site' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-S----OF----l',
      ),
       'is_comment_count' =>
      (object) array(
         'type' => 'tint',
         'schema' => 'ITS----OF-----',
      ),
       'bundle_name' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-S----OF----l',
      ),
       'hash' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-S----OF----l',
      ),
       'bs_status' =>
      (object) array(
         'type' => 'boolean',
         'schema' => 'I-S----OF----l',
      ),
       'entity_id' =>
      (object) array(
         'type' => 'long',
         'schema' => 'I-S----OF-----',
      ),
       'url' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-S----OF----l',
      ),
       'nodeaccess_all' =>
      (object) array(
         'type' => 'integer',
         'schema' => 'I--M---OF-----',
         'dynamicBase' => 'nodeaccess*',
      ),
       'sort_name' =>
      (object) array(
         'type' => 'sortString',
         'schema' => 'IT-----O-----l',
      ),
       'tags_a' =>
      (object) array(
         'type' => 'text',
         'schema' => 'IT-----O------',
         'dynamicBase' => 'tags_*',
      ),
       'bs_sticky' =>
      (object) array(
         'type' => 'boolean',
         'schema' => 'I-S----OF----l',
      ),
       'bs_promote' =>
      (object) array(
         'type' => 'boolean',
         'schema' => 'I-S----OF----l',
      ),
       'teaser' =>
      (object) array(
         'type' => 'text',
         'schema' => '-TS-----------',
      ),
       'im_vid_1' =>
      (object) array(
         'type' => 'long',
         'schema' => 'I-SM---OF-----',
         'dynamicBase' => 'im_*',
      ),
       'bs_translate' =>
      (object) array(
         'type' => 'boolean',
         'schema' => 'I-S----OF----l',
      ),
       'sort_label' =>
      (object) array(
         'type' => 'sortString',
         'schema' => 'IT-----O-----l',
      ),
       'ss_language' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-S----OF----l',
      ),
       'sm_vid_Tags' =>
      (object) array(
         'type' => 'string',
         'schema' => 'I-SM---OF----l',
         'dynamicBase' => 'sm_*',
      ),
    );
  }

  protected $last_search = array();

  public function search($query = '', $params = array(), $method = 'GET') {
    $this->last_search = array('query' => $query, 'params' => $params, 'method' => $method);
    $response = new stdClass();
    $response->response = array();
    $response->data = '';
    return $response;
  }

  public function getLastSearch() {
    return $this->last_search;
  }
}

