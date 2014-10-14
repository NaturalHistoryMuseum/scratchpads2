<?php

/**
 * @file
 * Manage messages on results of import or export process.
 */

/**
 * Helper to determine drupal_set_message type level from Drupal watchdog level.
 *
 * As drupal_set_message uses only three types, a mapping is needing when we
 * have only a watchdog level.
 * @see bootstrap.inc
 *
 * @param $watchdog_level
 *   Integer. Watchdog level as defined in bootstrap.inc.
 *
 * @return
 *   drupal_set_message type level.
 */
function _taxonomy_csv_message_watchdog_type($watchdog_level) {
  $mapping = array(
    WATCHDOG_ERROR   => 'error',   // Stop import process.
    WATCHDOG_WARNING => 'error',   // Stop line process and go next.
    WATCHDOG_NOTICE  => 'warning', // Continue current line process.
    WATCHDOG_INFO    => 'status',  // Successfully processed.
    WATCHDOG_DEBUG   => 'status',  // Internal use only.
  );

  return $mapping[$watchdog_level];
}

/**
 * Helper to determine error level of a message code.
 *
 * @param $message_code
 *   Single message code (000 to 799).
 *
 * @return
 *   Level (0 to 7).
 */
function _taxonomy_csv_message_get_level($message_code) {
  $result = intval($message_code / 100);
  return ($result >= WATCHDOG_ERROR && $result <= WATCHDOG_DEBUG) ?
      $result :
      WATCHDOG_ERROR;
}

/**
 * Helper to display result messages.
 *
 * @param $messages
 *   Array of messages array. A message array contains a watchdog level and a
 *   message code. A message code is an integer between 000 and 799.
 *
 * @return
 *   Nothing.
 */
function _taxonomy_csv_message_result($messages) {
  foreach ($messages as $message) {
    $type = key($message);
    drupal_set_message($message[$type], _taxonomy_csv_message_watchdog_type($type));
  }
}

/**
 * Helper to display by line list of result messages.
 *
 * @param $messages_set
 *   Array of message codes by line.
 *   Message code is an integer between 000 and 799.
 *
 * @return
 *   Array of messages.
 */
function _taxonomy_csv_message_result_by_line($messages_set) {
  $messages = array();

  if (is_array($messages_set) && $messages_set) {
    foreach ($messages_set as $line_number => $message_codes) {
      $processed_message_level = _taxonomy_csv_message_get_level(_taxonomy_csv_worst_message($message_codes));
      $processed_message_text  = t('Line #!line_number:', array('!line_number' => $line_number)) . '<br />';

      foreach ($message_codes as $message_code) {
        $processed_message_text .= _taxonomy_csv_message_text($message_code) . '<br />';
      }

      $messages[] = array($processed_message_level => $processed_message_text);
    }
  }

  return $messages;
}

/**
 * Helper to display compact list of result messages.
 *
 * @param $messages_set
 *   Array of operation array of codes, except 691, 692 and 695.
 *   Message code is an integer between 000 and 799.
 *
 * @return
 *   Array of messages.
 */
function _taxonomy_csv_message_result_by_message($messages_set) {
  $messages = array();

  if (is_array($messages_set) && $messages_set) {
    // Convert [line number][message codes]
    //      to [message codes][line number].
    foreach ($messages_set as $line_number => $operation) {
      foreach ($operation as $message_code) {
        $list_messages[$message_code][] = $line_number;
      }
    }
    ksort($list_messages);

    foreach ($list_messages as $message_code => $line_numbers) {
      // Only show line level message with created, updated and unchanged terms.
      if (($message_code != 691) && ($message_code != 692) && ($message_code != 695)) {
        $processed_message_level = _taxonomy_csv_message_get_level($message_code);
        $processed_message_text  = _taxonomy_csv_message_text($message_code) . ' ' . t('Lines:') . '<br />' . implode(', ', $line_numbers) . '.';

      $messages[] = array($processed_message_level => $processed_message_text);
      }
    }
  }

  return $messages;
}

/**
 * Helper to get text of a message with a message code.
 *
 * @param $message_code
 *   Message codes are integer between 000 and 799.
 *
 * @return
 *   Message text string.
 */
function _taxonomy_csv_message_text($message_code) {
  // Static used only for performance reason.
  static $watchdog_messages;

  if (!isset($watchdog_messages)) {
    // These variables are used to simplify strings management and translation.
    $error   = ' ' . t("Import process is stopped.");
    $warning = ' ' . t("This line won't be processed.");
    $notice  = ' ' . t("Line is processed.");

    $watchdog_messages = array(
      // Format: Level, Type of import, Serial.
      300 => t('ERROR'),
      301 => t("Wrong code. Module is not installed correctly. Please reinstall module from a fresh release or submit an issue."),
      302 => t('Unreferenced code. Please reinstall module from a fresh release or submit an issue.'),
      305 => t('Error in options.'),
      306 => t('Unknown import format. Change "Source content" option.') . $error,
      307 => t('Unknown export format. Change "Export format" option.') . $error,
      310 => t("You choose to import a taxonomy by a file, but you don't set its name or its size is greater than the server's limit.") . $error,
      311 => t('Size of your file is null.') . $error,
      312 => t('Unable to write to file.') . $error,
      313 => t('Current line is multiline or contains delimiter, enclosure or unprotected line ending. Change the content of the term, or set appropriate format settings in second tab.'),
      320 => t("Your file can't be converted to utf-8. Please install iconv, GNU recode or mbstring for PHP or convert your file to utf-8 and disable 'Enable utf-8 conversion' option.") . $error,
      321 => t('Despite your option, your file is not recognize as an UTF-8 encoded one. Convert it before submit it.') . $error,
      340 => t("Vocabulary doesn't exist."),
      341 => t("Field doesn't exist in this vocabulary.") . $error,
      352 => t("A parent tid is given, but it hasn't been imported in a previous line.") . $error,
      360 => t('An error occurs during translation of term.') . $error,
      361 => t('You cannot import translated terms into a vocabulary without language or with a fixed language.') . $error,
      385 => t('No term to export.'),
      390 => t('No line to import. Please first check your file and file uploading, else reinstall module from a fresh release or submit an issue.') . $error,
      391 => t('No term to import. Please first check your file and file uploading, else reinstall module from a fresh release or submit an issue.') . $error,
      392 => t('Problem when caching imported term. Please first check your file and file uploading, else reinstall module from a fresh release or submit an issue.') . $error,

      400 => t('WARNING'),
      402 => t('Unable to import a field term.') . $warning,
      405 => t('Unmanaged option.') . $warning,
      408 => t('Number of terms is invalid. Check if all your terms are attached to an existing vocabulary. The module exports only good terms and skips the others automatically.'),
      410 => t("Impossible to get parent of first item, because previous line has less parent(s). You may add one or more parents to current line or change lines order.") . $warning,
      430 => t('Line contains empty items.') . $warning,
      431 => t('Line contains duplicate items.') . $warning,
      432 => t('Line contains an empty term. Only items before this one have been imported.'),
      433 => t("Line contains items matching term name. A term can't be synonym, related, parent or child of itself.") . $warning,
      434 => t('Line contains too many items.') . $warning,
      435 => t('Line contains a translation for an empty description.') . $warning,
      440 => t("Vocabulary doesn't exist. When you duplicate or import into an existing vocabulary, option 'vocabulary_id' should contains true vocabulary id.") . $warning,
      450 => t('Weight in second column is not an allowed number.') . $warning,
      451 => t('One or more items are not numbers.') . $warning,
      452 => t('A term cannot be a parent of itself.') . $warning,
      453 => t('A term has no semantic field parent.'),
      454 => t('Term name or id is longer than 255 characters. Check field and delimiter.') . $warning,
      455 => t('Unable to make a reference to term "0".') . $warning,
      460 => t('Unknown predicate.') . $warning,
      461 => t('No subject.') . $warning,
      462 => t('No predicate.') . $warning,
      463 => t('No object.') . $warning,
      464 => t('No name.') . $warning,
      465 => t('More than three items.') . $warning,
      466 => t('Unmanaged predicate.') . $warning,
      467 => t('Each semantic field should be imported as "vocabulary" or "root_term".') . $warning,
      480 => t('No first column term to import. Empty first column is allowed only with structure or multiple terms import.') . $warning,
      481 => t('No item in second or third column.') . $warning,
      482 => t("Some items aren't present.") . $warning,
      483 => t('Term without name. Name is the only needed field.') . $warning,
      484 => t('No first or second column. They are required.') . $warning,
      490 => t('No line to import.') . $warning,
      491 => t('No item to import.') . $warning,
      492 => t('Nothing to import.') . $warning,
      493 => t('Empty line.') . $warning,
      499 => t('Warnings have been reported on this line.'),

      500 => t('Notice'),
      501 => t('Too many items. Second and next columns will be ignored, as import choice is to ignore them.'),
      510 => t('Line contains empty items. They are ignored.'),
      511 => t('To change vocabulary of a term is not recommended.'),
      512 => t("Line contains items matching first column term. A term can't be related to itself and a synonym may be different to it.") . ' ' . t('Duplicates will be ignored.'),
      513 => t('Line contains empty items.'),
      520 => t('Unable to load the file attached to the term.'),
      530 => t('Line contains empty items. They will be ignored.'),
      531 => t('Line contains duplicate items or repeated soft tabs.') . ' ' . t('Duplicates or repeated soft tabs will be ignored.'),
      532 => t('Line contains duplicate items.') . ' ' . t('Duplicates are allowed.'),
      533 => t("Line contains items matching term name. A term can't be related, parent, child or synonym to itself.") . ' ' . t('Duplicates will be ignored.'),
      535 => t('Line contains duplicate synonyms.') . ' ' . t('Duplicates will be ignored.'),
      536 => t('Line contains duplicate parents.') . ' ' . t('Duplicates will be ignored.'),
      537 => t('Line contains duplicate children.') . ' ' . t('Duplicates will be ignored.'),
      538 => t('Line contains duplicate related terms.') . ' ' . t('Duplicates will be ignored.'),
      541 => t('Too many items. Third and next columns will be ignored, as a term gets only one description and one weight.'),
      542 => t('Too many items. Fifth and next columns will be ignored.'),
      550 => t("No first column, but line can be processed."),
      551 => t('No parent, so it is a root term.'),
      552 => t('Term has been already imported in a previous line.'),
      553 => t('A semantic field cannot be related to terms.'),
      554 => t('A term has many parents and one is a semantic field, what is impossible.'),
      555 => t('A root term has a parent.') . $notice,
      557 => t('A term cannot be related to a semantic field'),
      558 => t('A semantic field cannot be a term synonym.'),
      559 => t('A term can have only one semantic field.'),
      561 => t('No subject.') . $notice,
      562 => t('No predicate.') . $notice,
      563 => t('No object.') . $notice,
      564 => t('Too many items on the line for the selected format. These items will be ignored.') . $notice,
      565 => t('Some items are empty. Previous items will be used.') . $notice,
      570 => t("Line doesn't contain all needed items.") . $notice,
      580 => t('Be careful. This line has only one term and import choice is to replace existing items. So these items will be removed.'),

      600 => t('Info'),
      605 => t('No error in options.'),
      610 => t('New vocabulary has been created.'),
      611 => t('A vocabulary has been duplicated.'),
      632 => t('Line contains duplicate items.') . ' ' . t('Duplicates are allowed.'),
      639 => t('Line contains empty vocabulary for related terms. They will be created in vocabulary of main term.'),
      640 => t('Vocabulary checked.'),
      662 => t('No predicate.'),
      683 => t('Use of a previous line term.'),
      685 => t('No term to process.'),
      691 => t('Saved new term.'),
      692 => t('Updated term.'),
      693 => t('Removed existing term.'),
      694 => t('Updated new term.'),
      695 => t('Unchanged term.'),
      696 => t('Empty line.'),
      697 => t('Command line.'),
      698 => t('Comment line.'),
      699 => t('Items of the line have been successfully imported.'),

      700 => t('Debug.'),
      799 => t('No message.'),
    );
  }

  if (is_int($message_code) && ($message_code >= 0) && ($message_code <= 799)) {
    // Good and referenced code.
    if (isset($watchdog_messages[$message_code])) {
      $message_title = $watchdog_messages[intval($message_code / 100) * 100];
      return $message_title . ' : ' . $watchdog_messages[$message_code];
    }
    // Else unreferenced code.
    return $watchdog_messages[300] . ' : (' . $message_code . ') : ' . $watchdog_messages[302];
  }
  // Else it's a wrong code.
  return $watchdog_messages[300] . ' : (' . $message_code . ') : ' . $watchdog_messages[301];
}
