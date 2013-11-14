<?php

/**
 * @file
 * Builds placeholder replacement tokens for relation data.
 */

/**
 * Implements hook_token_info().
 */
function relation_token_info() {
  $type = array(
    'name' => t('Relation'),
    'description' => t('Tokens related to individual relations.'),
    'needs-data' => 'relation',
  );

  // Core tokens for relations.
  $relation['rid'] = array(
    'name' => t("Relation ID"),
    'description' => t('The unique ID of the relation.'),
  );
  $relation['vid'] = array(
    'name' => t("Revision ID"),
    'description' => t("The unique ID of the relation's latest revision."),
  );
  $relation['relation_type'] = array(
    'name' => t("Relation type"),
    'description' => t("The type of the relation."),
  );
  $relation['relation_type_label'] = array(
    'name' => t("Relation type label"),
    'description' => t("The human-readable name of the relation type."),
  );
//   $relation['title'] = array(
//     'name' => t("Title"),
//     'description' => t("The title of the relation."),
//   );
  $relation['url'] = array(
    'name' => t("URL"),
    'description' => t("The URL of the relation."),
  );
  $relation['edit-url'] = array(
    'name' => t("Edit URL"),
    'description' => t("The URL of the relation's edit page."),
  );

  // Chained tokens for relations.
  $relation['created'] = array(
    'name' => t("Date created"),
    'description' => t("The date the relation was created."),
    'type' => 'date',
  );
  $relation['changed'] = array(
    'name' => t("Date changed"),
    'description' => t("The date the relation was most recently updated."),
    'type' => 'date',
  );
  $relation['author'] = array(
    'name' => t("Author"),
    'description' => t("The author of the relation."),
    'type' => 'user',
  );

  return array(
    'types' => array('relation' => $type),
    'tokens' => array('relation' => $relation),
  );
}

/**
 * Implements hook_tokens().
 */
function relation_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $url_options = array('absolute' => TRUE);
  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'relation' && !empty($data['relation'])) {
    $relation = $data['relation'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Simple key values on the relation.
        case 'rid':
          $replacements[$original] = $relation->rid;
          break;

        case 'vid':
          $replacements[$original] = $relation->vid;
          break;

        case 'relation_type':
          $replacements[$original] = $sanitize ? check_plain($relation->relation_type) : $relation->type;
          break;

        case 'type-name':
          $type_label = relation_get_type_label($relation);
          $replacements[$original] = $sanitize ? check_plain($type_label) : $type_label;
          break;

        case 'url':
          $replacements[$original] = url('relation/' . $relation->rid, $url_options);
          break;

        case 'edit-url':
          $replacements[$original] = url('relation/' . $relation->rid . '/edit', $url_options);
          break;

        // Default values for the chained tokens handled below.
        case 'author':
          if ($relation->uid == 0) {
            $name = variable_get('anonymous', t('Anonymous'));
          }
          else {
            $user = user_load($relation->uid);
            $name = $user->name;
          }
          $replacements[$original] = $sanitize ? filter_xss($name) : $name;
          break;

        case 'created':
          $replacements[$original] = format_date($relation->created, 'medium', '', NULL, $language_code);
          break;

        case 'changed':
          $replacements[$original] = format_date($relation->changed, 'medium', '', NULL, $language_code);
          break;
      }
    }

    if ($author_tokens = token_find_with_prefix($tokens, 'author')) {
      $author = user_load($relation->uid);
      $replacements += token_generate('user', $author_tokens, array('user' => $author), $options);
    }

    if ($created_tokens = token_find_with_prefix($tokens, 'created')) {
      $replacements += token_generate('date', $created_tokens, array('date' => $relation->created), $options);
    }

    if ($changed_tokens = token_find_with_prefix($tokens, 'changed')) {
      $replacements += token_generate('date', $changed_tokens, array('date' => $relation->changed), $options);
    }
  }

  return $replacements;
}
