<?php
/**
 * @file
 * og.rules_defaults.inc
 */

/**
 * Implementation of hook_default_rules_configuration().
 */
function og_default_rules_configuration() {
  $items = array();
  $items['rules_og_member_active'] = entity_import('rules_config', '{ "rules_og_member_active" : {
    "LABEL" : "OG member subscribe (Active)",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [ "rules", "og" ],
    "ON" : [ "og_user_insert" ],
    "IF" : [ { "data_is" : { "data" : [ "og-membership:state" ], "value" : "1" } } ],
    "DO" : [
      { "mail" : {
          "to" : [ "account:mail" ],
          "subject" : "Your membership request was approved for \'[og_membership:group:label]\'\r\n",
          "message" : "[account:name],\r\n\r\nYou are now a member in the group \'[og_membership:group:label]\' located at [og_membership:group:url]"
        }
      },
      { "mail" : {
          "to" : [ "og-membership:group:manager:mail" ],
          "subject" : "[account:name] joined [og_membership:group:label]",
          "message" : "[og-membership:group:manager:name],\r\n\r\n[account:name] joined [og_membership:group:label] in [og_membership:group:url]"
        }
      }
    ]
  }
}');
  $items['rules_og_member_pending'] = entity_import('rules_config', '{ "rules_og_member_pending" : {
    "LABEL" : "OG member subscribe (Pending)",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [ "rules", "og" ],
    "ON" : [ "og_user_insert" ],
    "IF" : [ { "data_is" : { "data" : [ "og-membership:state" ], "value" : "2" } } ],
    "DO" : [
      { "mail" : {
          "to" : [ "account:mail" ],
          "subject" : "Your membership request for \'[og_membership:group:label]\'",
          "message" : "[account:name],\r\n\r\nYour membership to group \'[og_membership:group:label]\' located at [og_membership:group:url] is pending approval."
        }
      },
      { "mail" : {
          "to" : [ "og-membership:group:manager:mail" ],
          "subject" : "[account:name] membership request [og_membership:group:label]",
          "message" : "[og-membership:group:manager:name],\r\n\r\n[account:name] requests membership for group \'[og_membership:group:label]\' in [og_membership:group:url].\r\n\r\n[og_membership:og-membership-request]"
        }
      }
    ]
  }
}');

  $items['rules_og_group_content_notification'] = entity_import('rules_config', '{ "rules_og_group_content_notification" : {
    "LABEL" : "OG group content notification",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [ "og", "rules" ],
    "ON" : [ "node_insert" ],
    "IF" : [ { "og_entity_is_group_content" : { "entity" : [ "node" ] } } ],
    "DO" : [
      { "og_get_members" : {
          "USING" : { "group_content" : [ "node" ] },
          "PROVIDE" : { "group_members" : { "group_members" : "List of group members" } }
        }
      },
      { "LOOP" : {
          "USING" : { "list" : [ "group-members" ] },
          "ITEM" : { "list_item" : "Current list item" },
          "DO" : [
            { "mail" : {
                "to" : [ "list-item:mail" ],
                "subject" : "New post in group",
                "message" : "Hello [list-item:name],\r\n\r\nThere is a new post called [node:title] that belongs to one of the groups you are subscribed to.\r\n\r\n[site:name]\r\n",
                "from" : [ "" ]
              }
            }
          ]
        }
      }
    ]
  }
}');
  return $items;
}
