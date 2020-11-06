<?php
$node = (object)array(
  'uid' => '2',
  'title' => 'About us',
  'log' => '',
  'status' => '1',
  'comment' => '1',
  'promote' => '0',
  'sticky' => '0',
  'type' => 'page',
  'language' => 'und',
  'tnid' => '0',
  'translate' => '0',
  'revision_uid' => '2',
  'field_taxonomic_name' => array(),
  'body' => array(
    'und' => array(
      0 => array(
        'value' => '<p>For further information on the Scratchpads, please see the <a href="http://scratchpads.org/about">Scratchpads website</a>.</p>',
        'summary' => '<p>For further information on the Scratchpads, please see the <a href="http://scratchpads.org/about">Scratchpads website</a>.</p>',
        'format' => 'filtered_html',
        'safe_value' => ' <p>For further information on the Scratchpads, please see the <a href="http://scratchpads.org/about">Scratchpads website</a>.</p>',
        'safe_summary' => ' <p>For further information on the Scratchpads, please see the <a href="http://scratchpads.org/about">Scratchpads website</a>.</p>'
      )
    )
  ),
  'field_media' => array(),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'foaf:Document'
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title'
      )
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created'
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601'
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified'
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601'
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded'
      )
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator'
      ),
      'type' => 'rel'
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name'
      )
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies'
      ),
      'datatype' => 'xsd:integer'
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date'
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601'
    )
  ),
  'cid' => '0',
  'comment_count' => '0',
  'name' => 'username',
  'picture' => '0',
  'data' => 'a:1:{s:7:"contact";i:1;}'
);
