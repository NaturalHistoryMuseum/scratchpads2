<?php
// $Id: taxonomy_csv.import.admin.inc,v 1.1.2.5 2011/01/06 18:08:27 danielkm Exp $

/**
 * @file
 * Create taxonomy csv import form and validate user input.
 */

/**
 * Invoke associated files.
 * taxonomy_csv.import.api.inc is invoked only when form is submitted.
 */
  $module_dir = drupal_get_path('module', 'taxonomy_csv');
  require_once("$module_dir/taxonomy_csv.api.inc");

/**
 * Generates the taxonomy CSV import form.
 *
 * Form contain five fieldsets:
 * - 1. What to import ?
 *   - 1. Content of the source
 *   - 2. Keep order of terms by adding a weight
 * - 2. Where are items to import ?
 *   - 1. Source select
 *   - 2. Source file or text area
 * - 3. How is your source formatted ?
 *   - 1. Source delimiter
 *   - 2. Source enclosure
 *   - 3. Line and utf8 checks
 * - 4. Which vocabulary to import into (destination) ?
 *   - 1. Destination type
 *   - 2. Vocabulary choice
 *   - 3. Vocabulary hierarchy set or check
 * - 5. When a term exist, what to do with it ?
 *   - 1. Previous or existing terms
 *   - 2. Specific import options depending on source content
 * - 6. Advanced options
 *   - 1. How to be notified
 *
 * As what will become existing terms depends on what is imported, dynamic
 * options are used: only possible parameters are shown. All options are
 * displayed if javascript is not activated.
 *
 * @ingroup forms
 * @see taxonomy_csv_form_import_validate()
 * @see taxonomy_csv_form_import_submit()
 * @see _taxonomy_csv_form_import_default_values_validate()
 * @see _taxonomy_csv_form_import_default_values_submit()
 */
function taxonomy_csv_form_import($form_state) {
  // Remember previous values to use specially when reloading form.
  // If not reloading form, use saved values if exist, else recommended ones.
  // Warning: specific values 'text', 'path' and 'url are not saved here.
  $list_recommended_values = _taxonomy_csv_values('import_default_ui');
  $list_previous_values = array();
  foreach ($list_recommended_values as $key => $value) {
    $list_previous_values[$key] = isset($form_state['values'][$key]) ?
      $form_state['values'][$key] :
      variable_get("taxonomy_csv_{$key}", $value);
  }

  $list_import_format = _taxonomy_csv_values('import_format');

  $list_external_format = taxonomy_csv_formats_get();

  $list_import_delimiter = array(
    'comma'            => t('« , » (Comma)'),
    'semicolon'        => t('« ; » (Semicolon)'),
    'tabulation'       => t('«   » (Tabulation)'),
    'space'            => t('«   » (Space)'),
    'currency_sign'    => t('« ¤ » (Currency sign)'),
    'soft_tab'         => t('«    » (Soft tab: two spaces or more)'),
    'custom_delimiter' => t('Custom delimiter'),
  );

  $list_import_enclosure = array(
    'none'             => t('None'),
    'quotation'        => t('« " » (Quotation mark)'),
    'custom_enclosure' => t('Custom enclosure'),
  );

  $list_vocabulary_target = array(
    'autocreate' => t('Autocreate a new vocabulary'),
    // @todo Finish to duplicate a vocabulary.
    // 'duplicate'  => t('Duplicate an existing vocabulary'),
    'existing'   => t('Import in an existing vocabulary'),
  );

  $list_import_option = _taxonomy_csv_values('import_option');

  // Build form.

  // Needed to upload file.
  $form = array(
    '#attributes' => array(
      'enctype' => 'multipart/form-data',
    )
  );

  // Warn when modules incompatible with taxonomy_csv are enabled.
  // Problems can't be reproduced with a memory of 256 MB, even with big
  // taxonomies. Only pathauto slows down import process.
  // See http://drupal.org/node/495548
  // See http://drupal.org/node/447852
  // See http://drupal.org/node/540916 (Pathauto).
  // @todo To be rechecked when these modules will be released for Drupal 7.
  $issues_modules = array(
    'taxonomynode',
    'taxonomy_vtn',
    'pathauto',
  );
  foreach ($issues_modules as $value) {
    if (module_exists($value) && empty($form_state['post'])) {
      drupal_set_message(t('<strong>Warning</strong>: An incompatible module is running.') . '<br />'
      . t('Memory or compatibility problems have been reported with these modules when under Drupal 6: !modules', array(
        '!modules' => '"' . implode('", "', $issues_modules) . '"',
      )) . '<br />'
      . t('It is advised to increase server and php memory temporary (no problem reported with 256 MB) or to disable these modules manually in !modules_section. Settings are not lost when you disable a module (and not uninstall it). After import process, you can decrease memory and reactivate modules.', array(
        '!modules_section' => l(t("modules section"), 'admin/build/modules'),
      )), 'warning');
      break;
    }
  }

  // Advertise as it's a developpement release.
  $form['drupal_7_info'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Warning'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#description' => '<strong>' . t('Vocabulary management has changed between Drupal 6 and Drupal 7.') . '</strong>' . '<br />'
      . t('In Drupal 7, <a href="!link_synonym">synonyms</a> and <a href="!link_related">related terms</a> have been removed and replaced by fields.', array(
        '!link_synonym' => url('http://drupal.org/node/224333#no-synonyms-taxonomy'),
        '!link_related' => url('http://drupal.org/node/224333#related_terms'),
      )) . '<br />'
      . t('Comments on this replacement and on the integration in Taxonomy CSV (use schemas or fields, functions, options...) are <a href="!link_comment">welcomed</a>.', array(
        '!link_comment' => url('http://drupal.org/project/taxonomy_csv'),
      )),
  );

  // Advertise that javascript is not enabled.
  if (!isset($_COOKIE['has_js'])) {
    $form['javascript'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('WARNING'),
      '#collapsible' => TRUE,
      '#collapsed'   => FALSE,
    );
    $form['javascript']['info'] = array(
      '#type'        => 'item',
      '#title'       => t('Javascript is not activated!'),
      '#description' => t("As you see this notice, javascript is disabled on your browser. All fields are shown, but only those matching your choices needs to be set. Others won't be used. If you want specific examples and options, activate javascript or see <a href=\"!more_help_link\"> advanced help</a>.", array('!more_help_link' => url('admin/help/taxonomy_csv'))),
      '#required'    => TRUE,
    );
  }

  $form['tab'] = array(
    '#type'        => 'vertical_tabs',
    '#default_tab' => 'format',
  );

  $form['tab']['format'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('1. What do you want to import?'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );

  $form['tab']['format']['import_format'] = array(
    '#type'          => 'select',
    '#title'         => '',
    '#options'       => $list_import_format,
    '#default_value' => $list_previous_values['import_format'],
  );

  if (isset($_COOKIE['has_js'])) {
    // Generic form for import format help.
    foreach ($list_import_format as $key => $value) {
      $form['tab']['format'][$key] = array(
        '#type'   => 'item',
        '#states' => array(
          'visible' => array(
            ':input[name=import_format]' => array('value' => $key),
          ),
        ),
      );
      // Complete generic form with specific help for external import formats.
      if (in_array($key, array_keys($list_external_format))) {
        $form['tab']['format'][$key]['#description'] = $list_external_format[$key]['description'] . '<br /><br />'
        . '<strong><em>' . t('Format:') . '</em></strong><br />'
        . '<code>' . $list_external_format[$key]['description_format'] . '</code><br />'
        . '<strong><em>' . t('Example:') . '</em></strong><br />'
        . '<code>' . $list_external_format[$key]['description_example'] . '</code><br />' . '<br />'
        . $list_external_format[$key]['description_long'];
      }
    }

    // Complete generic form with specific help for each import format.
    $form['tab']['format']['alone_terms']['#description'] = t('Only the term in the first column of each line is imported. Additional columns are ignored.');

    $form['tab']['format']['def_links']['#description'] = t('Allow to import full term definitions and links.') . '<br />'
      . t('Format: term name, term id, vocabulary id, term description, weight, number of synonyms, number of first level parents, number of first level children, number of related terms, list of synonyms, list of first level parents ids, list of first level children ids, list of related terms ids, list of vocabulary ids of related terms.') . '<br/>'
      . t('Only term name should be defined. Other values can be empty. Ids are not internal term ids or vocabulary ids, but a unique identifiant. Ids can be a number or a name. In most case, you can use true name. In fact, term ids need to be specific only for duplicate term names in order to identify each item. So for duplicates, you can use term name with a serial number. Main term id is only needed when term is a duplicate one.') . '<br />'
      . t("With this import format, destination is determined by source. If third column is empty, a new vocabulary will be created. If it's a name or a number, a vocabulary will be created if it doesn't exist. This process is used for related terms too. If vocabulary of a related term is not defined, vocabulary of main term is used. Warning: It's not recommended to change the vocabulary of a term with links.");

    $form['tab']['format']['flat']['#description'] = t('All items will be imported as terms.');

    $form['tab']['format']['tree_structure']['#description'] = t('Allow to create a tree structure (geography, classification...).') . '<br />'
      . t('Format: The first term is imported as a root level parent, the second as child of first term, the third as child of second term and so on. The lower child is the last term of a line. Others are hierarchical parents.') . '<br />'
      . t('To repeat previously imported items on each line is not needed: import process can manage one term by line.') . ' ' . t('If terms are repeated each line and lines are ordered, result and speed are same as with one term by line format.');

    $form['tab']['format']['polyhierarchy']['#description'] = t('Allow to create a polyhierarchical structure (genealogy, complex nomenclatures...).') . '<br />'
      . t('Format: The first term is imported as a root level parent, the second as child of first term, the third as child of second term and so on. The lower child is the last term of a line. Others are hierarchical parents.');

    $form['tab']['format']['parents']['#description'] = t('Allow to create a polyhierarchical taxonomy (genealogy...).') . '<br />'
      . t('First item is imported as a term and next ones as parents of first term. Unlike structure import, all parents are first level parents.');

    $form['tab']['format']['children']['#description'] = t('Allow to create a polyhierarchical taxonomy (genealogy...).') . '<br />'
      . t('First item is imported as a term and next ones as children of first term. Unlike structure import, all children are first level children.');

    $form['tab']['format']['relations']['#description'] = t('Allow to create relations between the term in the first column and next terms of the line.');

    $form['tab']['format']['synonyms']['#description'] = t('Allow to import synonyms of terms. Each line contains a term in the first column and next items are matching synonyms.');

    $form['tab']['format']['definitions']['#description'] = t('Import a full term definition') . '<br />'
      . t('Format: term name, weight, description, list of synonyms.');

    $form['tab']['format']['descriptions']['#description'] = t('Allow to import descriptions of terms. The term is in the first column and the matching description is in the second column.');

    $form['tab']['format']['weights']['#description'] = t('Allow to import a weight of a term. The term is in the first column and the matching weight is in the second column.');
  }
  $form['tab']['format']['info'] = array(
    '#type'        => 'item',
    '#description' => t('See <a href="!more_help_link">advanced help</a> for more informations about import types.', array('!more_help_link' => url('admin/help/taxonomy_csv'))),
  );

  $form['tab']['format']['keep_order'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Keep order of terms'),
    '#default_value' => $list_previous_values['keep_order'],
    '#description'   => t('If checked, a weight will be added to each term, so order of lines will be preserved. In unchecked, order will be alphabetic.') . '<br />' .
    t('Note: With polyhierarchical vocabulary (multiple parents for a term) and duplicate terms, unpredictable results may occur, because weight of a term is incremented each time it appears and a term can only have one weight, without context.') . '<br />' .
    t('This option is not available with some formats as Weight and Definitions.'),
    '#states' => array(
      'invisible' => array(
        ':input[name=import_format]' => array('value' => 'weights'),
      ),
    ),
  );

  $form['tab']['import'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('2. Where are items to import?'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );

  $form['tab']['import']['source_choice'] = array(
    '#type'          => 'select',
    '#title'         => '',
    '#options'       => array(
      'text' => t('In the below text area'),
      'path' => t('In a local file'),
      'url'  => t('In a distant file'),
    ),
    '#default_value' => $list_previous_values['source_choice'],
  );

  $form['tab']['import']['text'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Terms to import'),
    '#rows'          => 5,
    '#cols'          => 80,
    '#default_value' => isset($form_state['values']['text']) ?
        $form_state['values']['text'] :
        '',
    '#description'   => t('Write your csv formatted terms directly in this text area.'),
  );

  $form['tab']['import']['path'] = array(
    '#type'          => 'file',
    '#title'         => t('CSV file'),
    '#description'   => t('Browse to the file') . '<br >'
      . (($max_size = parse_size(ini_get('upload_max_filesize'))) ?
          t('Due to server restrictions, the <strong>maximum upload file size is !max_size</strong>. Files that exceed this size will be disregarded.', array('!max_size' => format_size($max_size))) . '<br >'
          . t('Use "distant file" import to go beyond, even with local file.') :
          ''),
  );

  $form['tab']['import']['url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('CSV file'),
    '#description'   => t('Enter the url (http, ftp, file, path...)'),
  );

  foreach (array('text', 'path', 'url') as $value) {
    $form['tab']['import'][$value]['#states'] = array(
      'visible' => array(
        ':input[name=source_choice]' => array('value' => $value),
      ),
    );
  }

  $form['tab']['csv_format'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('3. How is your source formatted?'),
    '#description' => t('Notice: either you import terms by a file or by a text area, the csv format is the same. Default delimiter is a comma ("<strong><code> , </code></strong>"). Default enclosure is none, but quotation mark ("<strong><code> " </code></strong>") is automatically managed.'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );

  $form['tab']['csv_format']['import_delimiter'] = array(
    '#type'          => 'select',
    '#title'         => t('CSV value delimiter'),
    '#options'       => $list_import_delimiter,
    '#default_value' => $list_previous_values['import_delimiter'],
    '#description'   => t("Choose the delimiter used in the CSV file you want to import. Tabulation can't be used with text area import."),
  );

  $form['tab']['csv_format']['import_delimiter_soft_tab_width'] = array(
    '#type'          => 'select',
    '#title'         => t('Soft tab width'),
    '#options'       => array(
      2 => 2,
      3 => 3,
      4 => 4,
      5 => 5,
      6 => 6,
      7 => 7,
      8 => 8,
      9 => 9,
    ),
    '#default_value' => $list_previous_values['import_delimiter_soft_tab_width'],
    '#description'   => t("Specify number of spaces of the soft tab delimiter. These spaces will be replaced with a true tab."),
    '#states' => array(
      'visible' => array(
        ':input[name=import_delimiter]' => array('value' => 'soft_tab'),
      ),
    ),
  );

  $form['tab']['csv_format']['import_delimiter_custom'] = array(
    '#type'          => 'textfield',
    '#title'         => 'Custom delimiter',
    '#default_value' => $list_previous_values['import_delimiter_custom'],
    '#size'          => 2,
    '#maxlength'     => 1,
    '#description'   => t('Specify your custom delimiter.'),
    '#states' => array(
      'visible' => array(
        ':input[name=import_delimiter]' => array('value' => 'custom_delimiter'),
      ),
    ),
  );

  $form['tab']['csv_format']['import_enclosure'] = array(
    '#type'          => 'select',
    '#title'         => t('CSV value enclosure'),
    '#options'       => $list_import_enclosure,
    '#default_value' => $list_previous_values['import_enclosure'],
    '#description'   => t('Choose the enclosure used in the CSV file you want to import.'),
  );

  $form['tab']['csv_format']['import_enclosure_custom'] = array(
    '#type'          => 'textfield',
    '#title'         => 'Custom enclosure',
    '#default_value' => $list_previous_values['import_enclosure_custom'],
    '#size'          => 2,
    '#maxlength'     => 1,
    '#description'   => t('Specify your custom enclosure.'),
    '#states' => array(
      'visible' => array(
        ':input[name=import_enclosure]' => array('value' => 'custom_enclosure'),
      ),
    ),
  );

  $form['tab']['csv_format']['check_line'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Check lines'),
    '#default_value' => $list_previous_values['check_line'],
    '#description'   => t('Format of each line of your input (order of items, format of names, duplicate terms...) can be checked if you are not sure of your vocabulary.'),
  );

  $form['tab']['csv_format']['check_utf8'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Check UTF-8 format'),
    '#description'   => t('File needs to be UTF-8 formatted. You can disable this check if you are sure that your file is encoded correctly. Desactivation is needed with some server configurations too. This option is not used with a textarea import.') . ' '
      . t('ISO-8859-1 and ISO-8859-15 files are automatically converted.'),
    '#states' => array(
      'invisible' => array(
        ':input[name=source_choice]' => array('value' => 'text'),
      ),
    ),
  );
  if (function_exists('mb_detect_encoding')) {
    $form['tab']['csv_format']['check_utf8']['#default_value'] = $list_previous_values['check_utf8'];
  }
  else {
    $form['tab']['csv_format']['check_utf8']['#default_value'] = FALSE;
    $form['tab']['csv_format']['check_utf8']['#disabled'] = TRUE;
    $form['tab']['csv_format']['check_utf8']['#description'] .= t('This checkbox is currently disabled, because iconv, GNU recode or mbstring for PHP are not installed on your server.');
  }

  $form['tab']['destination'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('4. Which vocabulary do you want to import into?'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#description' => t('Terms can be imported into a new vocabulary or in an existing one. You can choose to duplicate an existing vocabulary too in order to check import. You might want to !add-new-vocab.', array(
        '!add-new-vocab' => l(t('add a new vocabulary'), 'admin/structure/taxonomy/add', array('query' => drupal_get_destination())),
      )) . '<br />'
      . t('With some import formats as "Fields and links", vocabulary destination is defined by source content and this option is not used.'),
  );

  $form['tab']['csv_format']['locale_custom'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Manually set locale'),
    '#default_value' => $list_previous_values['locale_custom'],
    '#size'          => 60,
    '#maxlength'     => 255,
    '#description'   => t("To set locale can resolve some import issues. Choose the locale you use, for example 'en_DK.utf8', and it will be automatically set before import and reset after. Let empty when no problem occurs: import will use your default locale."),
  );

  $list_vocabularies = taxonomy_vocabulary_get_names();

  if (count($list_vocabularies) == 0) {
    $form['tab']['destination']['#description'] .= '<br /><br />'
      . t("As there isn't any vocabulary, terms will be imported in a new automatically created vocabulary.");

    $form['tab']['destination']['vocabulary_target'] = array(
      '#type'  => 'value',
      '#value' => 'autocreate',
    );

    $form['tab']['destination']['vocabulary_id'] = array(
      '#type'  => 'value',
      '#value' => 0,
    );
  }
  else {
    $form['tab']['destination']['vocabulary_target'] = array(
      '#type'          => 'select',
      '#options'       => $list_vocabulary_target,
      '#default_value' => $list_previous_values['vocabulary_target'],
      '#description'   => t('Choose vocabulary where you want to import your items.'),
    );

    $form['tab']['destination']['vocabulary_id'] = array(
      '#type'          => 'select',
      '#title'         => t('Vocabulary choice'),
      '#options'       => array(
        'choose_vocabulary' => t('[Choose an existing vocabulary]'),
      ),
      '#default_value' => $list_previous_values['vocabulary_id'],
      '#description'   => t('The vocabulary you want to import the file into.'),
      '#states' => array(
        'invisible' => array(
          ':input[name=vocabulary_target]' => array('value' => 'autocreate'),
        ),
      ),
    );
    foreach ($list_vocabularies as $vocabulary) {
      $form['tab']['destination']['vocabulary_id']['#options'][$vocabulary->vid] = $vocabulary->name;
    }
  }

  $form['tab']['destination']['info_hierarchy'] = array(
    '#type'        => 'item',
    '#title'       => t('What is the type of vocabulary?'),
  );
  $form['tab']['destination']['check_hierarchy'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Automatically check vocabulary hierarchy'),
    '#default_value' => $list_previous_values['check_hierarchy'],
    '#description'   => t('Warning: to calculate true hierarchy of vocabulary is memory intensive. Choose to check automatically only if your vocabulary is little.'),
  );
  $form['tab']['destination']['set_hierarchy'] = array(
    '#type'          => 'radios',
    // '#title is needed to avoid a #states bug.
    '#title'         => '',
    '#options'       => array(
      0 => t('no parent (flat)'),
      1 => t('single parent (tree)'),
      2 => t('multiple parents (polyhierarchy)'),
    ),
    '#default_value' => $list_previous_values['set_hierarchy'],
    '#states' => array(
      // @todo Change invisible with disabled when #states will work, and move
      // it above, and directly include title.
      'invisible' => array(
        ':input[name=check_hierarchy]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['tab']['import_options'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('5. When a term exists, what to do with it?'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#description' => t('What will existing or previous imported terms become when a term with same name will be imported?'),
  );

  if (isset($_COOKIE['has_js'])) {
    $list_import_allowed = _taxonomy_csv_values('import_allowed');
    foreach ($list_import_format as $key => $value) {
      $form['tab']['import_options']["existing_items_$key"] = array(
        '#type'          => 'radios',
        '#options'       => array_intersect_key($list_import_option, array_flip($list_import_allowed[$key])),
        '#default_value' => $list_previous_values['existing_items'],
        // '#description is needed to avoid a #states bug.
        '#description'   => '',
        '#states'        => array(
          'visible' => array(
            ':input[name=import_format]' => array('value' => $key),
          ),
        ),
      );
      // Preselection if import format has only one option.
      if (count($form['tab']['import_options']["existing_items_$key"]['#options']) == 1) {
        $form['tab']['import_options']["existing_items_$key"]['#default_value'] = array_pop(array_keys($form['tab']['import_options']["existing_items_$key"]['#options']));
      }
    }

    // Generic form for import format help.
    foreach ($list_import_format as $key => $value) {
      $form['tab']['import_options'][$key] = array(
        '#type'   => 'item',
        '#markup' => t('Help for import format:') . ' ' . $value,
        '#states' => array(
          'visible' => array(
            ':input[name=import_format]' => array('value' => $key),
          ),
        ),
      );

      // Complete generic form with specific help for external import formats.
      if (in_array($key, array_keys($list_external_format))) {
        $form['tab']['import_options'][$key]['#description'] = $list_external_format[$key]['import_options_help'];
      }
    }

    // Complete generic form with specific help for each import format.
    $form['tab']['import_options']['alone_terms']['#description'] = t('This option indicates whether existing terms with the same name should be updated or ignored. If ignored, duplicates may be created.');

    $form['tab']['import_options']['def_links']['#description'] = t('This option indicates whether and how existing terms with the same name should be updated or ignored.') . '<br />'
      . t('By nature, only one option can be chosen with this format.') . '<br />'
      . '<ul>
          <li>' . t('"<em>Update and merge"</em>: update term or create it if not exists and merge eventual item with new one.') . '</li>
        </ul>';

    $form['tab']['import_options']['flat']['#description'] = t('This option indicates whether existing terms with the same name should be updated or ignored. If ignored, duplicates may be created.');

    $form['tab']['import_options']['tree_structure']['#description'] = t('This option indicates whether and how existing terms with the same name should be updated or ignored.') . '<br />'
      . t('This option is used with last term of the line.') . '<br />'
      . '<ul>
          <li>' . t('<em>"Update and replace"</em>: each parent replaces eventual older ones.') . '</li>
          <li>' . t('<em>"Ignore and create"</em>: child is always created and eventually its parents if they don\'t exist.') . ' ' . t('It is recommended if you use duplicate terms.') . '</li>
          <li>' . t('<em>"Ignore existings"</em>: child is always created and eventually its parents if they don\'t exist.') . ' ' . t('This mode is slightly faster, but lines must be ordered because no check are made on existing terms.') . '</li>
        </ul>';

    $form['tab']['import_options']['polyhierarchy']['#description'] = t('This option indicates whether and how existing terms with the same name should be updated or ignored.') . '<br />'
      . t('By nature, only one option can be chosen with this format.') . '<br />'
      . '<ul>
          <li>' . t('<em>"Update and merge"</em>: each child and parents are merged with old ones, except if the child has the same name than parent, in which case a new term is created, because a child cannot be a parent of itself. Warning: on next lines, direct children of this term name will be attached to the first imported term.') . '</li>
        </ul>';

    $form['tab']['import_options']['parents']['#description'] = t('This option indicates whether and how existing terms with the same name should be updated or ignored.') . '<br />'
      . '<ul>
          <li>' . t('<em>"Update and merge"</em>: each child and parents are merged with old ones. No duplicate are created.') . '</li>
          <li>' . t('<em>"Update and replace"</em>: each parent replaces eventual older ones.') . '</li>
          <li>' . t('<em>"Ignore and create"</em>: child is always created and eventually its parents if they don\'t exist.') . '</li>
        </ul>';

    $form['tab']['import_options']['children']['#description'] = t('This option indicates whether and how existing terms with the same name should be updated or ignored.') . '<br />'
      . '<ul>
          <li>' . t('<em>"Update and merge"</em>: each child and parents are merged with old ones.') . '</li>
          <li>' . t('<em>"Update and replace"</em>: each parent replaces eventual older ones.') . '</li>
          <li>' . t('<em>"Ignore and create"</em>: child is always created and eventually its parents if they don\'t exist.') . '</li>
        </ul>';

    $form['tab']['import_options']['relations']['#description'] = t('This option indicates whether existing terms with the same name should be updated or ignored.
    <p>For example, if existing related terms of term < <code>Drupal</code> > are < <code>Free</code> > and < <code>Open source</code> > and an imported line in the csv file is < <code>"Drupal","Knowledge management","Free"</code> >, then:') . '<br >'
      . '<ul>
          <li>' . t('<em>"Update and merge"</em> choice makes related terms of < <code>Drupal</code> > are now < <code>Free</code> >, < <code>Open source</code> > and < <code>Knowledge management</code> >;') . '</li>
          <li>' . t('<em>"Update and replace"</em> choice makes related terms of < <code>Drupal</code> > are now < <code>Knowledge management</code> > and < <code>Free</code> >;') . '</li>
          <li>' . t('<em>"Ignore and create"</em> choice makes two < <code>Drupal</code> > terms, one with existing related and other items and another one with the imported related terms < <code>Knowledge management</code> > and < <code>Free</code> >, which one has not been duplicated.') . '</li>
        </ul></p>';

    $form['tab']['import_options']['synonyms']['#description'] = t('This option indicates whether and how existing terms with the same name should be updated or ignored.') . '<br />'
      . '<ul>
          <li>' . t('<em>"Update and merge"</em>: update term or create it if not exists and merge eventual synonyms with new ones. Always remove duplicate synonyms.') . '</li>
          <li>' . t('<em>"Update and replace"</em>: update term or create it if not exists and replace eventual synonyms with new ones.') . '</li>
          <li>' . t('<em>"Ignore existings"</em>: term is always created.') . '</li>
        </ul>';

    $form['tab']['import_options']['definitions']['#description'] = t('This option indicates whether existing terms with the same name should be updated or ignored. If ignored, duplicates may be created.') . '<br />'
      . '<ul>
          <li>' . t('<em>"Update and merge"</em>: update term or create it if not exists and merge eventual description with new one.') . '</li>
          <li>' . t('<em>"Update and replace"</em>: update term or create it if not exists and replace eventual description with new one.') . '</li>
          <li>' . t('<em>"Ignore existings"</em>: term is always created.') . '</li>
        </ul>';

    $form['tab']['import_options']['descriptions']['#description'] = t('This option indicates whether and how existing terms with the same name should be updated or ignored.') . '<br />'
      . '<ul>
          <li>' . t('<em>"Update and merge"</em>: update term or create it if not exists and merge eventual description with new one.') . '</li>
          <li>' . t('<em>"Update and replace"</em>: update term or create it if not exists and replace eventual description with new one.') . '</li>
          <li>' . t('<em>"Ignore existings"</em>: term is always created.') . '</li>
        </ul>';

    $form['tab']['import_options']['weights']['#description'] = t('This option indicates whether and how existing terms with the same name should be updated or ignored.');
  }
  else {
    $form['tab']['import_options']['help_no_javascript'] = array(
      '#type'        => 'item',
      '#description' => t("As you see this notice, javascript is not activated on your browser. Only options matching your source content and vocabulary destination needs to be set. Others won't be used. If you want specific examples and options, activate javascript or see <a href=\"!more_help_link\"> advanced help</a>.", array('!more_help_link' => url('admin/help/taxonomy_csv'))),
    );
    // Use this form only if no javascript or in a second step wizard.
    $form['tab']['import_options']['existing_items'] = array(
      '#type'          => 'radios',
      '#options'       => array_intersect_key($list_import_option, array_flip(array(
        TAXONOMY_CSV_EXISTING_UPDATE,
        TAXONOMY_CSV_EXISTING_UPDATE_MERGE,
        TAXONOMY_CSV_EXISTING_UPDATE_REPLACE,
        TAXONOMY_CSV_EXISTING_IGNORE,
        TAXONOMY_CSV_EXISTING_IGNORE_CREATE,
        TAXONOMY_CSV_EXISTING_IGNORE_ALL,
        TAXONOMY_CSV_EXISTING_IGNORE_QUICK,
      ))),
      '#default_value' => $list_previous_values['existing_items'],
      // '#description' below in order to use same help with javascript.
      '#description'   => t('This option allows to set what previous imported terms will become if a new line contains the same terms. Usually, it indicates an error or a unoptimized source, unless you allow duplicates.') . '<br />'
    . t('This option is used too with existing terms in the target vocabulary. Recommended value is to update and merge. If you choose to ignore previous or existing terms, the vocabulary will have duplicate terms.') . '<br />'
    . t('Some choices may be currently disabled.'),
    );
  }

  // Specific options to import relations.
  $form['tab']['import_options']['relations'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Specific settings of:') . ' ' . t('Related terms'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#description' => t('Set these options only if you import:') . ' ' . t('Related terms') . ' .',
    '#states' => array(
      'visible' => array(
        ':input[name=import_format]' => array('value' => 'relations'),
      ),
    ),
  );
  $form['tab']['import_options']['relations']['relations_create_subrelations'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Import subrelations'),
    '#default_value' => $list_previous_values['relations_create_subrelations'],
    '#description'   => t('This checkbox allows to import subrelations of related terms and not only relations of first column term with others.') . '<br />'
      . t('For example, with the line < <code>"Paris","London","Bern","Roma"</code> >, default import is to make a link between < <code>Paris</code> > and each of three terms. There is no link between < <code>London</code> > and < <code>Bern</code> > nor < <code>Roma</code> >. Checking this option creates not only relations with first term, but all subrelations too: < <code>London</code> > and < <code>Bern</code> >, < <code>London</code> > and < <code>Roma</code> > and finally < <code>Bern</code> > and < <code>Roma</code> >.'),
  );
  // Internal use only.
  $form['tab']['import_options']['relations']['relations_all_vocabularies'] = array(
    '#type'          => 'value',
    '#title'         => t('Make relations with existing terms of all vocabularies'),
    '#default_value' => $list_previous_values['relations_all_vocabularies'],
    '#description'   => t("This checkbox allows to create relations with existing terms in other vocabularies if they don't exist in selected vocabulary."),
    '#disabled'      => TRUE,
  );

  // External specific options.
  // @todo when needed.
  foreach ($list_external_format as $key => $value) {
  }

  $form['tab']['advanced_options'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('6. Informations on process'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#description' => t('All these options influe on memory and time process. The more information you want, the more power you need.') . ' '
      . t("It's recommended to reduce displayed infos when imported vocabulary is big (from 1000 or 10000 lines depending on the server)."),
  );

  $form['tab']['advanced_options']['result_choices'] = array(
    '#type'          => 'checkboxes',
    '#options'       => array(
      'result_stats' => t('Basic stats on imported terms'),
      'result_terms' => t('List of imported terms'),
    ),
    '#default_value' => array(
      $list_previous_values['result_stats'],
      $list_previous_values['result_terms'],
    ),
  );

  $form['tab']['advanced_options']['result_level'] = array(
    '#type'          => 'select',
    '#title'         => t('Log level'),
    '#options'       => array(
      'first'    => t('Only first warning or notice'),
      'warnings' => t('Warnings only'),
      'notices'  => t('Warnings and notices'),
      'infos'    => t('Warnings, notices and informations'),
    ),
    '#default_value' => $list_previous_values['result_level'],
  );

  $form['tab']['advanced_options']['result_type'] = array(
    '#type'          => 'radios',
    '#title'         => t('Group informations'),
    '#options'       => array(
      'by_message'  => t('By message (compact view)'),
      'by_line'     => t('By line (list view)'),
      // 'by_collapse' => t('By line (collapsible view)'),
    ),
    '#default_value' => $list_previous_values['result_type'],
    '#states' => array(
      'invisible' => array(
        ':input[name=result_level]' => array('value' => 'first'),
      ),
    ),
  );

  $form['tab']['advanced_options']['info'] = array(
    '#type'        => 'item',
    '#description' => t('Warning: to display warnings, notices and informations, especially by line, can help you to detect issues when submitted list of terms is not clean and when you choose to check lines, but it is memory intensive.'),
  );

  $form['import_submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Import'),
  );

  $form['import_default_values'] = array(
    '#type'     => 'submit',
    '#value'    => t('Reset to defaults'),
    '#validate' => array('_taxonomy_csv_form_import_default_values_validate'),
    '#submit'   => array('_taxonomy_csv_form_import_default_values_submit'),
  );

  return $form;
}

/**
 * Handles CSV import form validation.
 *
 * @see taxonomy_csv_form_import()
 */
function taxonomy_csv_form_import_validate($form, &$form_state) {
  // Invoke taxonomy_csv.import.api.
  $module_dir = drupal_get_path('module', 'taxonomy_csv');
  require_once("$module_dir/import/taxonomy_csv.import.api.inc");

  $options = &$form_state['values'];

  // 1. Preload text or file in order to check it.
  // When source is path, need to preload here.
  if ($options['source_choice'] == 'path') {
    $options['file'] = file_save_upload('path');
    // fopen and fseek need a real path.
    if ($options['file']) {
      $options['file']->filepath = drupal_realpath($options['file']->uri);
    }
  }
  $messages = _taxonomy_csv_import_input_preload($options);

  // 2, Simplify values to be compatible with api checks.
  // Define true delimiter.
  $delimiter = array(
    'comma'            => ',',
    'semicolon'        => ';',
    'tabulation'       => "\t",
    'space'            => ' ',
    'currency_sign'    => '¤',
    'soft_tab'         => str_repeat(' ', $options['import_delimiter_soft_tab_width']),
    'custom_delimiter' => $options['import_delimiter_custom'],
  );
  $options['delimiter'] = $delimiter[$options['import_delimiter']];

  // Define true enclosure.
  $enclosure = array(
    'none'             => '',
    'quotation'        => '"',
    'custom_enclosure' => $options['import_enclosure_custom'],
  );
  $options['enclosure'] = $enclosure[$options['import_enclosure']];

  // Define existing items choice.
  if (isset($_COOKIE['has_js'])) {
    $import_format = $options['import_format'];
    $options['existing_items'] = $options["existing_items_$import_format"];
  }

  // Define result preferences.
  foreach ($options['result_choices'] as $key => $value) {
    $options[$key] = $value;
  }

  // 3. Make api checks and eventually update options by reference.
  $messages = array_merge($messages, _taxonomy_csv_import_check_options($options));

  // Use form set error for api errors.
  foreach (array(
      'delimiter'   => 'import_delimiter',
      'enclosure'   => 'import_enclosure',
    ) as $key => $value) {
    if (isset($message[$key])) {
      $message[$value] = $message[$key];
      unset($message[$key]);
    }
  }

  // 4. Make non api checks.
  if (($options['import_delimiter'] == 'custom_delimiter')
      && (empty($options['import_delimiter_custom']))) {
    $messages['import_delimiter_custom'] = t('You choose to use a custom delimiter, but your delimiter is empty.');
  }

  if (($options['import_enclosure'] == 'custom_enclosure')
      && (empty($options['import_enclosure_custom']))) {
    $messages['import_enclosure_custom'] = t('You choose to use a custom enclosure, but your enclosure is empty.');
  }

  if (($options['import_delimiter'] == 'custom_delimiter')
      && (drupal_strlen($options['import_delimiter_custom']) > 1)) {
    $messages['import_delimiter_custom'] = t('Delimiter should have only one character.');
  }
  if (($options['import_enclosure'] == 'custom_enclosure')
      && (drupal_strlen($options['import_enclosure_custom']) > 1)) {
    $messages['import_enclosure_custom'] = t('Enclosure should have only zero or one character.');
  }

  // 5. Finish validatation of form.
  foreach ($messages as $item => $message) {
    form_set_error(check_plain($item), filter_xss($message));
  }
}

/**
 * Validate options of imported vocabulary or line.
 *
 * @param $options
 *   Array of options.
 *
 * @return
 *   Array of messages errors if any.
 *   By reference options are cleaned and completed.
 */
function _taxonomy_csv_import_check_options(&$options) {
  $messages = array();

  // Delimiter or enclosure greater than one character are useless with fgetcsv,
  // except with soft tab.
  if ($options['delimiter'] == ''
      || ((drupal_strlen($options['delimiter']) > 1) && (trim($options['delimiter'], ' ')) != '')
    ) {
    $messages['delimiter'] = t('Delimiter should be a one character string or a soft tab (two spaces or more).');
  }
  if (drupal_strlen($options['enclosure']) == 0) {
    // With fgetcsv, empty enclosure bugs, so use default quote enclosure.
    $options['enclosure'] = '"';
  }
  elseif (drupal_strlen($options['enclosure']) > 1) {
    $messages['enclosure'] = t('Enclosure lenght cannot be greater than one character.');
  }
  if ($options['delimiter'] == $options['enclosure']) {
    $messages['delimiter'] = t('Delimiter and enclosure cannot be same character.');
  }

  // Clean locale if any.
  $options['locale_custom'] = trim($options['locale_custom']);

  // Calculates number of lines to be imported.
  if (!isset($options['file'])
      || !is_object($options['file'])
      || count(file($options['file']->filepath)) == 0) {
    $messages['total_lines'] = t('No term to import. Import finished.');
  }

  if (!in_array($options['vocabulary_target'], array(
      'autocreate',
      'duplicate',
      'existing',
    ))) {
    $messages['vocabulary_target'] = t('Destination target should be "autocreate", "duplicate" or "existing".');
  }

  if ($options['vocabulary_target'] == 'duplicate'
      || $options['vocabulary_target'] == 'existing') {
    $list_vocabularies = taxonomy_get_vocabularies();
    if (!isset($list_vocabularies[$options['vocabulary_id']])) {
      $messages['vocabulary_id'] = t("You choose to use an existing vocabulary, but you haven't choose it.");
    }
  }

  $list_import_format = _taxonomy_csv_values('import_format');
  $list_import_option = _taxonomy_csv_values('import_option');
  $list_import_allowed = _taxonomy_csv_values('import_allowed');
  if (!array_key_exists($options['import_format'], $list_import_format)) {
    $messages['import_format'] = t('Source content "!import_format" is not managed.', array(
      '!import_format' => $list_import_format[$options['import_format']]
    ));
  }
  elseif (!in_array($options['existing_items'], $list_import_allowed[$options['import_format']])) {
    $messages['existing_items'] = t('Import option "%existing_items" cannot be used with source content "!import_format".', array(
      '%existing_items' => $list_import_option[$options['existing_items']],
      '!import_format'  => $list_import_format[$options['import_format']],
    ));
  }

  if (!$options['existing_items']) {
    $messages['existing_items'] = t('Please set what will become existing terms.');
  }
  elseif (!array_key_exists($options['existing_items'], $list_import_option)) {
    $messages['existing_items'] = t('Import option "!existing_items" is not managed.', array(
      '!existing_items' => $list_import_option[$options['existing_items']]
    ));
  }

  if ($options['check_hierarchy']
      && ($options['set_hierarchy'] < 0 || $options['set_hierarchy'] > 2)) {
    $messages['set_hierarchy'] = t('You need to set hierarchy level if hierarchy check of vocabulary is disabled.');
  }

  return $messages;
}

/**
 * Handles CSV import form submission and launch batch set.
 *
 * @see taxonomy_csv_form_import()
 */
function taxonomy_csv_form_import_submit($form, &$form_state) {
  // Remember last preferences and prepare only options to be sent to Api.
  foreach (array(
      'import_format',
      'keep_order',
      'source_choice',
      'import_delimiter',
      'import_delimiter_soft_tab_width',
      'import_delimiter_custom',
      'import_enclosure',
      'import_enclosure_custom',
      'check_line',
      'check_utf8',
      'locale_custom',
      'vocabulary_target',
      'vocabulary_id',
      'check_hierarchy',
      'set_hierarchy',
      'existing_items',
      // Specific to relations import.
      'relations_create_subrelations',
      'relations_all_vocabularies',
      // General options.
      'result_stats',
      'result_terms',
      'result_level',
      'result_type',
    ) as $option) {
    variable_set("taxonomy_csv_$option", $form_state['values'][$option]);
    $options[$option] = $form_state['values'][$option];
  }
  // Finish to prepare $options. Unset useless options for api.
  if ($options['source_choice'] == 'text') {
    $options['text'] = &$form_state['values']['text'];
    unset($form_state['values']['text']);
  }
  unset($options['import_delimiter']);
  unset($options['import_delimiter_soft_tab_width']);
  unset($options['import_delimiter_custom']);
  unset($options['import_enclosure']);
  unset($options['import_enclosure_custom']);
  $options['delimiter'] = $form_state['values']['delimiter'];
  $options['enclosure'] = $form_state['values']['enclosure'];
  $options['file'] = $form_state['values']['file'];
  $options['path'] = $form_state['values']['path'];
  $options['url']  = $form_state['values']['url'];
  $options['check_options'] = FALSE; // Already done.
  $options['result_display'] = TRUE;

  // Prepares process batch (will be automatically processed when returns).
  taxonomy_csv_vocabulary_import($options);
}

/**
 * Restore recommended default values in the import form. Empty validate hook.
 */
function _taxonomy_csv_form_import_default_values_validate($form, &$form_state) {
}

/**
 * Restore recommended default values in the import form.
 */
function _taxonomy_csv_form_import_default_values_submit($form, &$form_state) {
  foreach (_taxonomy_csv_values('import_default_ui') as $option => $value) {
    variable_set("taxonomy_csv_$option", $value);
  }
  unset($form_state['values']);
  unset($form_state['storage']);
}
