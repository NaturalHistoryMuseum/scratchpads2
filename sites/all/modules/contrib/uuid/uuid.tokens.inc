<?php

/**
 * @file
 * Builds placeholder replacement tokens for uuid-related data.
 */

/**
 * Implements hook_token_info().
 */
function uuid_token_info() {
  $tokens['tokens']['node']['uuid'] = array(
    'name' => t("Node UUID"),
    'description' => t("The UUID of the node."),
  );

  $tokens['tokens']['user']['uuid'] = array(
    'name' => t("User UUID"),
    'description' => t("The UUID of the user."),
  );

  $tokens['tokens']['file']['uuid'] = array(
    'name' => t("File UUID"),
    'description' => t("The UUID of the file."),
  );

  if (module_exists('taxonomy')) {
    $tokens['tokens']['term']['uuid'] = array(
      'name' => t("Term UUID"),
      'description' => t("The UUID of the term."),
    );
  }

  if (module_exists('commnet')) {
    $tokens['tokens']['commnet']['uuid'] = array(
      'name' => t("Comment UUID"),
      'description' => t("The UUID of the commnet."),
    );
  }

  return $tokens;
}

/**
 * Implements hook_tokens().
 */
function uuid_tokens($type, $tokens, array $data = array(), array $options = array()) {
  switch ($type) {
    case 'node':
      $node = $data['node'];
      foreach ($tokens as $name => $original) {
        if ($name == 'uuid') {
         $replacements[$original] = $node->uuid;
        }
      }
      break;

    case 'user':
      $user = $data['user'];
      foreach ($tokens as $name => $original) {
        if ($name == 'uuid') {
         $replacements[$original] = $user->uuid;
        }
      }
      break;

    case 'file':
      $file = $data['file'];
      foreach ($tokens as $name => $original) {
        if ($name == 'uuid') {
         $replacements[$original] = $file->uuid;
        }
      }
      break;

    case 'term':
      $term = $data['term'];
      foreach ($tokens as $name => $original) {
        if ($name == 'uuid') {
         $replacements[$original] = $term->uuid;
        }
      }
      break;

    case 'comment':
      $comment = $data['comment'];
      foreach ($tokens as $name => $original) {
        if ($name == 'uuid') {
         $replacements[$original] = $comment->uuid;
        }
      }
      break;
  }

  return $replacements;
}