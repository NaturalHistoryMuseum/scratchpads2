<?php

/**
 * Implementation of hook_default_rules_configuration().
 */
function og_example_default_rules_configuration() {
  $items = array();
  $items['rules_og_notification'] = entity_import('rules_config', '{ "rules_og_notification" : {
      "LABEL" : "OG notification",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules", "og" ],
      "ON" : [ "node_insert" ],
      "IF" : [
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "group_audience" } }
      ],
      "DO" : [
        { "og_get_members" : {
            "USING" : { "group_audience" : [ "node:group-audience" ] },
            "PROVIDE" : { "group_members" : { "group_members" : "List of group members" } }
          }
        },
        { "LOOP OVER" : [
            { "mail" : {
                "to" : [ "list-item:uid" ],
                "subject" : "New post in group - [node:title]",
                "message" : "Hello [list_item:name],\\r\\n\\r\\n[node:title] was added to a group you are a member of.",
                "from" : [ "site:mail" ]
              }
            }
          ]
        }
      ]
    }
  }');
  return $items;
}
