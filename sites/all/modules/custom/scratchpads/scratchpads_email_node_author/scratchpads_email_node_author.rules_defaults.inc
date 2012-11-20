<?php
/**
 * @file
 * scratchpads_email_node_author.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function scratchpads_email_node_author_default_rules_configuration() {
  $items = array();
  $items['rules_comment_email'] = entity_import('rules_config', '{ "rules_comment_email" : {
      "LABEL" : "Comment email",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules", "comment" ],
      "ON" : [ "comment_insert" ],
      "DO" : [
        { "mail" : {
            "to" : [ "comment:node:author:mail" ],
            "subject" : "A new comment \\u0022[comment:title]\\u0022 has been posted on \\u0022[comment:node:title]\\u0022.",
            "message" : "A new comment \\u0022[comment:title]\\u0022 has been posted on \\u0022[comment:node:title]\\u0022.\\r\\n\\r\\n[comment:node:url]\\r\\n\\r\\nYour site robot.",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  return $items;
}
