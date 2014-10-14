<?php

/**
 * @file
 * Create taxonomy csv import form and validate user input.
 */

/**
 * Invoke associated files.
 */
$module_dir = drupal_get_path('module', 'taxonomy_csv');
require_once($module_dir . '/taxonomy_csv.api.inc');

/**
 * Generates the taxonomy CSV import form.
 *
 * Form contain six fieldsets:
 * - 1. What to import ?
 *   - 1. Content of the source
 *   - 2. Specific options of structure/fields/translation
 *   - 3. Keep order of terms by adding a weight
 * - 2. Where are items to import ?
 *   - 1. Source select
 *   - 2. Source file or text area
 * - 3. How is your source formatted ?
 *   - 1. Source delimiter
 *   - 2. Source enclosure
 *   - 3. Description filter format
 *   - 4. Custom fields filter format
 *   - 5. Language of imported terms
 *   - 6. Line and utf8 checks
 * - 4. Which vocabulary to import into (destination) ?
 *   - 1. Destination type
 *   - 2. Vocabulary choice
 *   - 3. Vocabulary internationalization (mode and language)
 *   - 4. Autocreate custom fields
 *   - 5. Deletion of terms
 *   - 6. Vocabulary hierarchy set or check
 * - 5. When a term exist, what to do with it ?
 *   - 1. Previous or existing terms
 *   - 2. Specific import options depending on source content
 * - 6. Advanced options
 *   - 1. Basic stats
 *   - 2. List of terms
 *   - 3. How to be notified
 *
 * @ingroup forms
 * @see taxonomy_csv_import_form_validate()
 * @see taxonomy_csv_import_form_submit()
 * @see taxonomy_csv_import_form_default_values_validate()
 * @see taxonomy_csv_import_form_default_values_submit()
 */
function taxonomy_csv_import_form($form, &$form_state) {
  // User is used to get filter formats the current user has access to.
  global $user;

  $list_recommended_values = _taxonomy_csv_values('import_default_ui');
  $list_previous_values = array();
  foreach ($list_recommended_values as $key => $value) {
    $list_previous_values[$key] = isset($form_state['values'][$key]) ?
      $form_state['values'][$key] :
      variable_get('taxonomy_csv_' . $key, $value);
  }

  $list_import_format = _taxonomy_csv_values('import_format');

  $list_import_delimiter = array(
    'comma'            => t('« , » (Comma)'),
    'semicolon'        => t('« ; » (Semicolon)'),
    'tabulation'       => t('«   » (Tabulation)'),
    'pipe'             => t('« | » (Pipe)'),
    'space'            => t('«   » (Space)'),
    'currency_sign'    => t('« ¤ » (Currency sign)'),
    'soft_tab'         => t('«    » (Soft tab: two spaces or more)'),
    'custom_delimiter' => t('Custom delimiter'),
  );

  $list_import_enclosure = array(
    'none'             => t('None'),
    'quotation'        => t('« " » (Quotation mark)'),
    'quote'            => t("« ' » (Quote)"),
    'custom_enclosure' => t('Custom enclosure'),
  );

  $list_filter_format_custom = array(
    'none' => t("None (Fixed plain text: user can't choose)"),
  );
  foreach (filter_formats($user) as $format) {
    $list_filter_format_custom[$format->format] = $format->name;
  }

  $list_filter_format = $list_filter_format_custom;
  unset($list_filter_format['none']);

  $list_import_language = array(
    'und' => t('Language neutral'),
  );
  if (module_exists('locale')) {
    $list_import_language += locale_language_list('name');
    foreach (locale_language_list('native') as $key => $value) {
      $list_import_language[$key] .= ' [' . $value . ']';
    }
  }

  $list_vocabularies = taxonomy_vocabulary_get_names();

  $list_vocabulary_target = array(
    'autocreate' => t('Autocreate a new vocabulary'),
    'existing'   => t('Import in an existing vocabulary'),
  );

  // Build form.
  $form['tab'] = array(
    '#type'        => 'vertical_tabs',
    '#default_tab' => 'format',
  );

  $form['tab']['format'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('1. What do you want to import?'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#description' => t('Choose the type of import. Help for each type is displayed below when format is selected.') . '<br />'
    . '<strong>' . t('Important') . '</strong>: ' . t('If you have a tree structure, you should import it before fields or translations, and for each translation if you use a full Translate mode.'),
  );

  $form['tab']['format']['import_format'] = array(
    '#type'          => 'radios',
    '#title'         => '',
    '#options'       => $list_import_format,
    '#default_value' => $list_previous_values['import_format'],
  );

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
  }

  // Prepare descriptions.
  $line_format = '<br /><br />'
    . '<strong><em>' . t('Line format') . '</em></strong><br />'
    . '<code>';
  $example = '</code><br /><br />'
    . '<strong><em>' . t('Examples') . '</em></strong>'
    . '<ul><li><code>';
  $li = '</code></li><li><code>';
  $description_long = '</code></li></ul>';

  // Complete generic form with specific help for each import format.
  $form['tab']['format'][TAXONOMY_CSV_FORMAT_STRUCTURE]['#description'] = t('Allow to create a flat vocabulary, a tree structure (geography, classification...) or a polyhierarchy (genealogy, complex nomenclatures...).')
    . $line_format
      . t('term name, child term name, sub-child term name...')
    // Example Flat.
    . '</code><br /><br />'
    . '<strong><em>' . t('Examples (flat)') . '</em></strong>'
    . '<ul><li><code>'
      . t('foo') . $li
      . t('bar, small bar')
    . '</code></li></ul>'
    // Example Tree.
    . '</code><br />'
    . '<strong><em>' . t('Examples (simple tree)') . '</em></strong>'
    . '<ul><li><code>'
      . t('foo, bar, small bar') . $li
      . t('Europe, France, Paris') . $li
      . t('Europe, France, Lyon') . $li
      . t('Europe, UK, London') . $li
      . t('America, USA, Washington') . $li
      . t(', , Hollywood')
    . '</code></li></ul>'
    // Example Polyhierarchy.
    . '</code><br />'
    . '<strong><em>' . t('Examples (Polyhierarchy)') . '</em></strong>'
    . '<ul><li><code>'
      . t('foo, bar, small bar') . $li
      . t('Grand-Mother, Mother, Daughter') . $li
      . t('Grand-Father, Mother, Son') . $li
      . t('Grand-Mother 2, Father, Daughter') . $li
      . t('Grand-Father 2, Father, Son') . $li
      . t(', , Son 2') . $li
      . t(', Uncle') . $li
      . t('Grand-Mother 2, Uncle') . $li
      . t('Father, Son 3')
  . $description_long
      . '<br />'
      . t('The first term is imported as a root level parent, the second as child of first term, the third as child of second term and so on. The lower child is the last term of a line. Others are hierarchical parents.') . '<br />'
      . t('To repeat previously imported items on each line is not needed: import process can manage one term by line.') . ' ' . t('If terms are repeated each line and lines are ordered, result and speed are same as with one term by line format.');

  $form['tab']['format'][TAXONOMY_CSV_FORMAT_FIELDS]['#description'] = t('Allow to choose a custom order of fields on a csv line.')
    . $line_format
      . t('term name/id, field_1, field_2...')
    . $example
      . t('foo, foo field 1, foo field 2') . $li
      . t('bar, bar field 1, bar field 2')
    . $description_long;
      // See below.

  $form['tab']['format'][TAXONOMY_CSV_FORMAT_TRANSLATE]['#description'] = t('Allow to import name and descriptions and their translation.')
    . $line_format
      . '<em>' . t('Vocabulary with Translate mode:') . '</em> ' . t('term name/id, first translated term name, second translated term name...') . '<br />'
      . '<em>' . t('Vocabulary with Localize mode:') . '</em> ' . t('term name/id, first translation of term name... , description, first translation of description...')
    . $example
      . t('foo, bar') . $li
      . t('"United Kingdom", "Royaume-Uni", "Vereinigte Königreich"') . $li
      . t('"Germany", "Allemagne", "A European country", "Un pays européen"') . '<em> ' . t('[vocabulary with Localize mode only]') . '</em> '
    . $description_long
      . t('The term is in the first column followed by its translations. If the i18n mode is Localize, then description and translated descriptions can be added.') . '<br />'
      . '<strong>' . t('Note') . '</strong>:' . '<br />'
      . '<ul>'
        . '<li>' . t('With a vocabulary in Translate mode, a term with an undefined language cannot be translated, so do not forget to choose a language when you import original terms.') . '</li>'
        . '<li>' . t('With a vocabulary in Localize mode, only terms with a undefined language can be translated, so do not set a language when you import original terms.') . '</li>'
      . '</ul>';
  if (!module_exists('i18n_taxonomy')) {
    $form['tab']['format']['import_format']['#options'][TAXONOMY_CSV_FORMAT_TRANSLATE] .= ' [' . t('Unavailable') . ']';
    $form['tab']['format'][TAXONOMY_CSV_FORMAT_TRANSLATE]['#description'] .= '<br />'
      . t('This format is available only if the submodule <a href="!link">Taxonomy internationalization</a> is enabled.', array(
        '!link' => url('http://drupal.org/project/i18n'),
      ));
  }

  // Specific options.
  // Specific options of Structure.
  $form['tab']['format']['specific'][TAXONOMY_CSV_FORMAT_STRUCTURE]['structure_type'] = array(
    '#type'          => 'radios',
    '#title'         => t('Type of structure'),
    '#options'         => array(
      TAXONOMY_CSV_FORMAT_FLAT          => t('Flat vocabulary'),
      TAXONOMY_CSV_FORMAT_TREE          => t('Simple tree'),
      TAXONOMY_CSV_FORMAT_POLYHIERARCHY => t('Polyhierarchy'),
    ),
    '#default_value' => $list_previous_values['structure_type'],
    '#description'   => t('Choose if terms have no parent, one parent only, or multiple parents.'),
    '#states'        => array(
      'visible' => array(
        ':input[name=import_format]' => array('value' => TAXONOMY_CSV_FORMAT_STRUCTURE),
      ),
    ),
  );

  // Specific options of Fields.
  $form['tab']['format']['specific'][TAXONOMY_CSV_FORMAT_FIELDS]['import_fields_format'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Set order of items on a csv line'),
    '#default_value' => $list_previous_values['import_fields_format'],
    '#size'          => 60,
    '#maxlength'     => 1024,
    '#description'   => t('Set custom order of csv input separated by comma.') . '<br />'
      . '<strong>' . t('Notes:') . '</strong>'
      . '<ul>'
        . '<li>' . t("First field need to be 'name' or 'tid'.") . '</li>'
        . '<li>' . t('Items can be repeated for multivalued fields.') . '</li>'
        . '<li>' . t("If you choose to set 'vid' or 'vocabulary_machine_name', the vocabulary should exist before import.") . '</li>'
        . '<li>' . t("It's not recommended to import translations at the same time than parents and term references, unless all of them are in the same vocabulary and have the same language.") . '</li>'
        . '<li>' . t('Duplicate terms are not fully managed for parents (it is recommended to use "Structure" import) and term references.') . '</li>'
      . '</ul>'
      . '<strong>' . t('Examples:') . '</strong>'
      . '<ul>'
        . '<li><code>' . t('name, field_mycustomfield_1_machinename, field_mycustomfield_2_machinename') . '</code></li>'
        . '<li><code>' . t('name, description, field_country, field_year_started') . '</code></li>'
        . '<li><code>' . t('name, description, weight, parent, synonym, synonym, synonym, related_term, related_term, related_term, related_term') . '</code></li>'
      . '</ul>',
    '#states'        => array(
      'visible' => array(
        ':input[name=import_format]' => array('value' => TAXONOMY_CSV_FORMAT_FIELDS),
      ),
    ),
  );

  // Specific options of Translate.
  $form['tab']['format']['specific'][TAXONOMY_CSV_FORMAT_TRANSLATE]['translate_by'] = array(
    '#type'          => 'radios',
    '#title'         => t('Type of source'),
    '#options'         => array(
      'name' => t('First item is the source term name'),
      'tid'  => t('First item is the source term id'),
    ),
    '#default_value' => $list_previous_values['translate_by'],
      '#description'   => t('Choose how to identify the source term.') . ' ' . t("If the source term doesn't exist, it will be created.") . '<ul>'
        . '<li>' . t('To use name is simpler, but cannot be used if vocabulary has duplicate names.') . '</li>'
        . '<li>' . t('To use term id is quicker, but you need to export your vocabulary to get tids.') . '</li>'
      . '</ul>',
    '#states'        => array(
      'visible' => array(
        ':input[name=import_format]' => array('value' => TAXONOMY_CSV_FORMAT_TRANSLATE),
      ),
    ),
  );
  $form['tab']['format']['specific'][TAXONOMY_CSV_FORMAT_TRANSLATE]['translate_languages'] = array(
    '#type'          => 'textfield',
    '#title'         => t('List of languages'),
    '#default_value' => $list_previous_values['translate_languages'],
    '#size'          => 60,
    '#maxlength'     => 1024,
    '#description'   => t('Set the list of languages of terms, for example "<code>en, fr, de</code>".') . '<br />'
      . '<strong>' . ('Notes:') . '</strong>'
      . '<ul>'
        . '<li>' . t('All languages should have been enabled in <a href="!link">Regional and language settings</a> before import.', array('!link' => url('admin/config/regional/language/overview'))) . '</li>'
        . '<li>' . '<strong>' . t('Important') . '</strong>: ' . t('If your taxonomy is structured, you should import the structure <em>before</em>. Furthermore, if the i18n mode is Translate, you should import the structure for each translation before. The translate process only creates translation sets for terms, but does not recreate any structure.') . '</li>'
        . '<li>' . t('With Translate mode, terms with an undefined language (<code>"und"</code>) cannot be translated.') . '</li>'
        . '<li>' . t('With Localize mode, only terms with an undefined language can be translated, so the first language should be "und" and the default language of the site cannot be used.') . '</li>'
      . '</ul>',
    '#states'        => array(
      'visible' => array(
        ':input[name=import_format]' => array('value' => TAXONOMY_CSV_FORMAT_TRANSLATE),
      ),
    ),
  );

  $form['tab']['format']['keep_order'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Keep order of terms'),
    '#default_value' => $list_previous_values['keep_order'],
    '#description'   => t('If checked, a weight will be added to each term, so order of lines will be preserved. If unchecked, order will be alphabetic.') . '<br />' .
    t('Note: With polyhierarchical vocabulary (multiple parents for a term) and duplicate terms, unpredictable results may occur, because weight of a term is incremented each time it appears and a term can only have one weight, without context.') . '<br />' .
    t("You shouldn't use this option if your custom format has a 'weight' field."),
  );

  $form['tab']['import'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('2. Where are items to import?'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );

  $form['tab']['import']['source_choice'] = array(
    '#type'          => 'radios',
    '#title'         => '',
    '#options'       => array(
      'text' => t('In the below text area'),
      'file' => t('In a local file'),
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
    '#states' => array(
      'visible' => array(
        ':input[name=source_choice]' => array('value' => 'text'),
      ),
    ),
  );

  $form['tab']['import']['file'] = array(
    '#type'          => 'file',
    '#title'         => t('CSV file'),
    '#description'   => t('Browse to the file') . '<br >'
      . (($max_size = parse_size(ini_get('upload_max_filesize'))) ?
          t('Due to server restrictions, the <strong>maximum upload file size is !max_size</strong>. Files that exceed this size will be disregarded.', array('!max_size' => format_size($max_size))) . '<br >'
          . t('Use "distant file" import to go beyond, even with local file.') :
          ''),
    '#states' => array(
      'visible' => array(
        ':input[name=source_choice]' => array('value' => 'file'),
      ),
    ),
  );

  $form['tab']['import']['url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('CSV file'),
    '#description'   => t('Enter the url (http, ftp, file, path...)'),
    '#states' => array(
      'visible' => array(
        ':input[name=source_choice]' => array('value' => 'url'),
      ),
    ),
  );

  $form['tab']['csv_format'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('3. How is your source formatted?'),
    '#description' => t('Default delimiter is a comma ("<strong><code>,</code></strong>"). Default enclosure is none, but quotation mark ("<strong><code>"</code></strong>") is automatically managed.') . '<br />'
      . t('Notice') . ': ' . t('Either you import terms by a file or by a text area, the csv format is the same.') . '<br />'
      . '<strong>' . t('Warning') . '</strong>: ' . t('You should enclose any item beginning with a non-ascii letter, such as "É", "ç", "œ" or any non-latin letter.'),
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
    '#states'        => array(
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
    '#states'        => array(
      'visible' => array(
        ':input[name=import_enclosure]' => array('value' => 'custom_enclosure'),
      ),
    ),
  );

  $form['tab']['csv_format']['filter_format'] = array(
    '#type'          => 'select',
    '#title'         => t('Format of the description'),
    '#options'       => $list_filter_format,
    '#default_value' => $list_previous_values['filter_format'],
    '#description'   => t('Choose the default format used for the description. This parameter is set in your imported term even if there is no description.') . '<br />'
      . '<strong>' . t('Notes') . '</strong>:'
      . '<ul>'
        . '<li>' . t('You can select only formats you can use.') . '</li>'
        . '<li>' . t('Check is made by Drupal only when description is displayed.') . '</li>'
        . '<li>' . t('If your vocabulary is localizable and you want to translate description, you should use "plain text" format or change the translation mode of the vocabulary.') . '</li>'
      . '</ul>',
  );

  $form['tab']['csv_format']['filter_format_custom'] = array(
    '#type'          => 'select',
    '#title'         => t('Default format of custom fields'),
    '#options'       => $list_filter_format_custom,
    '#default_value' => $list_previous_values['filter_format_custom'],
    '#description'   => t('Choose the default format used for custom fields.') . '<br />'
      . '<strong>' . t('Notes') . '</strong>:'
      . '<ul>'
        . '<li>' . t('You can select only formats you can use. It can be changed later.') . '</li>'
        . '<li>' . t('Sanitization is made during import.') . '</li>'
      . '</ul>',
  );

  $form['tab']['csv_format']['import_language'] = array(
    '#type'          => 'select',
    '#title'         => t('Default language of terms'),
    '#options'       => $list_import_language,
    '#default_value' => $list_previous_values['import_language'],
    '#description'   => t('Choose the default language of terms.') . '<br />'
      . t('Note') . ': ' . t('For the format "Name and description translation", this option will be used for the translated item.') . '<br />'
      . '<strong>' . t('Warning') . '</strong>: ' . t('You need to check your regional and multilingual settings if you want to see terms in another language than the default one of the vocabulary.'),
    '#states'        => array(
      'invisible' => array(
        ':input[name=import_format]' => array('value' => TAXONOMY_CSV_FORMAT_TRANSLATE),
      ),
    ),
  );
  if (!module_exists('i18n_taxonomy')) {
    $form['tab']['csv_format']['import_language']['#default_value'] = $list_recommended_values['import_language'];
    $form['tab']['csv_format']['import_language']['#disabled'] = TRUE;
    $form['tab']['csv_format']['import_language']['#description'] = t('This option is available only if the submodule <a href="!link">Taxonomy internationalization</a> is enabled.', array(
        '!link' => url('http://drupal.org/project/i18n'),
      ));
  }

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
    '#states'        => array(
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

  $form['tab']['csv_format']['locale_custom'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Manually set locale of the file'),
    '#default_value' => $list_previous_values['locale_custom'],
    '#size'          => 60,
    '#maxlength'     => 255,
    '#description'   => t('To set locale can resolve some import issues related to file or database. Choose the locale you use, for example "<code>en_DK.utf8</code>", and it will be automatically set before import and reset after. Let empty when no problem occurs: import will use your default locale.'),
  );

  $form['tab']['destination'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('4. Which vocabulary do you want to import into?'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#description' => t('Terms can be imported into a new vocabulary or in an existing one. You can choose to duplicate an existing vocabulary too in order to check import. You might want to !add-new-vocab.', array(
        '!add-new-vocab' => l(t('add a new vocabulary'), 'admin/structure/taxonomy/add', array('query' => drupal_get_destination())),
      )) . '<br />'
      . t('If you import Fields and one of them is "vid" or "vocabulary_machine_name", this vocabulary need to exist before import, else error can occur.'),
  );

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
      '#type'          => 'radios',
      '#options'       => $list_vocabulary_target,
      '#default_value' => $list_previous_values['vocabulary_target'],
      '#description'   => t('Choose the vocabulary where you want to import your items.'),
    );

    $form['tab']['destination']['vocabulary_id'] = array(
      '#type'          => 'select',
      '#title'         => t('Vocabulary choice'),
      '#options'       => array(
        0 => t('[Choose an existing vocabulary]'),
      ),
      '#default_value' => $list_previous_values['vocabulary_id'],
      '#description'   => t('The vocabulary you want to import the file into.'),
      '#states'        => array(
        'invisible' => array(
          ':input[name=vocabulary_target]' => array('value' => 'autocreate'),
        ),
      ),
    );
    foreach ($list_vocabularies as $vocabulary) {
      $form['tab']['destination']['vocabulary_id']['#options'][$vocabulary->vid] = $vocabulary->name;
    }
  }

  // Options language and i18n_mode.
  // Module i18n_taxonomy is enabled.
  if (module_exists('i18n_taxonomy')) {
    $form['tab']['destination'] += i18n_translation_mode_element('taxonomy_vocabulary');
    $form['tab']['destination']['i18n_translation']['#states'] = array(
      'visible' => array(
        ':input[name=vocabulary_target]' => array('value' => 'autocreate'),
      ),
    );
    $form['tab']['destination']['i18n_translation']['i18n_mode']['#description'] .= '<br />'
      . t('Note') . ': ' . t('This option can be changed later.');
    // Use previous options.
    $form['tab']['destination']['i18n_translation']['i18n_mode']['#default_value'] = $list_previous_values['i18n_mode'];
    $form['tab']['destination']['i18n_translation']['language']['#default_value'] = ($list_previous_values['i18n_mode'] == 2) ? // I18N_MODE_LANGUAGE.
      $list_previous_values['language'] :
      $list_recommended_values['language'];
  }
  // No internationalization feature.
  else {
    $form['tab']['destination']['i18n_translation'] = array(
      '#type'        => 'item',
      '#title'       => t('Internationalization of vocabulary'),
      '#description' => t('Internationalization options are available only if the submodule <a href="!link">Taxonomy i18n</a> is enabled.',
        array('!link' => url('http://drupal.org/project/i18n'))),
      '#states' => array(
        'visible' => array(
          ':input[name=vocabulary_target]' => array('value' => 'autocreate'),
        ),
      ),
    );
    $form['tab']['destination']['i18n_mode'] = array(
      '#type'  => 'value',
      '#value' => $list_recommended_values['i18n_mode'],
    );
    $form['tab']['destination']['language'] = array(
      '#type'  => 'value',
      '#value' => $list_recommended_values['language'],
    );
  }

  $form['tab']['destination']['import_fields_custom'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Set custom fields of the vocabulary'),
    '#default_value' => $list_previous_values['import_fields_custom'],
    '#size'          => 60,
    '#maxlength'     => 1024,
    '#description'   =>  t('Fields of custom format are automatically attached if they exist, else they are created before.') . ' ' . t('When a field is created, options are set to Drupal defaults for a text field with a unlimited number of values.') . ' ' . t("If you want another type, you have to create it before or set it here with the '|' symbol.") . '<br />'
      . t('Currently only these field types are managed by default:')
      . '<ul>'
        . '<li>' . 'text' . '</li>'
        . '<li>' . 'text_long' . '</li>'
        . '<li>' . 'text_with_summary' . '</li>'
        . '<li>' . 'number_decimal' . '</li>'
        . '<li>' . 'number_integer' . '</li>'
        . '<li>' . 'number_float' . '</li>'
        . '<li>' . 'list_boolean' . '</li>'
        . '<li>' . 'taxonomy_term_reference' . ' <em>' . t('[Note: if value is a number, the field will be linked to the term with that tid; if value is a string, the field will be linked to an existing or a new term with that name.]') . '</em>' . '</li>'
        . '<li>' . 'file' . '</li>'
      . '</ul>'
      . t('Other types are well imported too if their content is a "value".') . '<br />'
      . t('Be careful with "taxonomy_term_reference", because this field allows references to one vocabulary only, so referenced vocabulary is set to current one, which can be an issue if the same field is used somewhere else.') . '<br />'
      . t('Examples:') . '<ul>'
      . '<li><code>' . t('field_mycustomfield_1_machinename, field_mycustomfield_2_machinename|file') . '</code></li>'
      . '<li><code>' . t('field_content, field_reference|taxonomy_term_reference') . '</code></li>'
      . '<li><code>' . t('field_country|text, field_year_started|text') . '</code></li></ul>',
    '#states'        => array(
      'visible' => array(
        ':input[name=import_format]' => array('value' => TAXONOMY_CSV_FORMAT_FIELDS),
      ),
    ),
  );

  $form['tab']['destination']['delete_terms'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Automatically delete all terms of the selected vocabulary before import'),
    '#default_value' => $list_previous_values['delete_terms'],
    '#description'   => t("Warning: You won't be warned before deletion."),
    '#states'        => array(
      'visible' => array(
        ':input[name=vocabulary_target]' => array('value' => 'existing'),
      ),
    ),
  );

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
    '#states'        => array(
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
    '#description' => t('What will existing term become when a term with same name and same language will be imported?'),
  );

  $form['tab']['import_options']['update_or_ignore'] = array(
    '#type'          => 'radios',
    '#options'       => array(
      TAXONOMY_CSV_EXISTING_UPDATE => t('Update it and merge fields (avoid duplicate terms, recommended)'),
      TAXONOMY_CSV_EXISTING_IGNORE => t('Ignore it and create a new term (duplicate terms may be created)'),
    ),
    '#default_value' => $list_previous_values['update_or_ignore'],
    '#description'   => t('This option allows to set what previous imported terms will become if a new line contains the same terms.') . ' ' . t('Usually, it indicates an error or a unoptimized source, unless you allow duplicates.'),
  );
  $form['tab']['import_options']['update_or_ignore_info_polyhierarchy'] = array(
    '#type'        => 'item',
    '#description' => '<strong>' . t('Note') . '</strong>: ' . t('You cannot choose to ignore existing items when you import a polyhierarchical structure.'),
    '#states'      => array(
      'visible' => array(
        ':input[name=import_format]' => array('value' => TAXONOMY_CSV_FORMAT_STRUCTURE),
        ':input[name=structure_type]' => array('value' => TAXONOMY_CSV_FORMAT_POLYHIERARCHY),
      ),
    ),
  );

  $form['tab']['advanced_options'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('6. Informations on process and advanced options'),
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

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Import'),
  );

  $form['actions']['import_default_values'] = array(
    '#type'     => 'submit',
    '#value'    => t('Reset to defaults'),
    '#validate' => array('taxonomy_csv_import_form_default_values_validate'),
    '#submit'   => array('taxonomy_csv_import_form_default_values_submit'),
  );

  return $form;
}

/**
 * Handles CSV import form validation.
 *
 * @see taxonomy_csv_import_form()
 */
function taxonomy_csv_import_form_validate($form, &$form_state) {
  // Invoke taxonomy_csv.import.api.
  $module_dir = drupal_get_path('module', 'taxonomy_csv');
  require_once($module_dir . '/import/taxonomy_csv.import.api.inc');

  $options = &$form_state['values'];

  // 1. Preload text or file in order to check it.
  switch ($options['source_choice']) {
    case 'text':
      $options['url'] = '';
      $options['file'] = '';
      break;
    case 'url':
      $options['text'] = '';
      $options['file'] = '';
      break;
    case 'file':
      $options['text'] = '';
      $options['url'] = '';

      // When source is path, need to preload here.
      $file = file_save_upload('file', array('file_validate_extensions' => array('csv txt')));
      // fopen and fseek need a real path.
      if (!empty($file)) {
        $file->filepath = drupal_realpath($file->uri);
        if (!empty($file->filepath)) {
          $options['file'] = $file;
        }
      }
      break;
  }
  $messages = _taxonomy_csv_import_input_preload($options);

  // 2, Simplify values to be compatible with api checks.
  // Define true delimiter.
  $delimiter = array(
    'comma'            => ',',
    'semicolon'        => ';',
    'tabulation'       => "\t",
    'pipe'             => '|',
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
    'quote'            => "'",
    'custom_enclosure' => $options['import_enclosure_custom'],
  );
  $options['enclosure'] = $enclosure[$options['import_enclosure']];

  // Warning: in API, 'language' is option for terms.
  // This issue is related to subform 'i18n_translation', provided by 'i18n'.
  $options['vocabulary_language'] = $options['language'];
  $options['language'] = $options['import_language'];

  // Translate languages is trimmed array of translate_languages.
  $options['translate_languages'] = (trim($options['translate_languages'] == '')) ?
    array() :
    array_values(array_map('trim', explode(',', drupal_strtolower($options['translate_languages']))));

  // Custom format is trimmed array of import_fields_format.
  $options['fields_format'] = (trim($options['import_fields_format'] == '')) ?
    array() :
    array_values(array_map('trim', explode(',', $options['import_fields_format'])));

  // Custom fields is trimmed array of import_fields_custom.
  $options['fields_custom'] = (trim($options['import_fields_custom'] == '')) ?
    array() :
    array_values(array_map('trim', explode(',', $options['import_fields_custom'])));

  // Define result preferences.
  foreach ($options['result_choices'] as $key => $value) {
    $options[$key] = $value;
  }

  // 3. Make api checks and eventually update options by reference.
  $messages = array_merge($messages, _taxonomy_csv_import_check_options($options));

  // Return i18n options for UI.
  $options['import_language'] = $options['language'];
  $options['language'] = $options['vocabulary_language'];
  unset($options['vocabulary_language']);

  // In UI, translate_languages is a string and not an array.
  $options['translate_languages'] = implode(', ', $options['translate_languages']);

  // Return form set errors for api errors.
  foreach (array(
      'fields_format' => 'import_fields_format',
      'delimiter'     => 'import_delimiter',
      'enclosure'     => 'import_enclosure',
      'fields_custom' => 'import_fields_custom',
      'language'      => 'import_language',
      'vocabulary_language' => 'language',
    ) as $key => $value) {
    if (isset($messages[$key])) {
      $messages[$value] = $messages[$key];
      unset($messages[$key]);
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
  global $user;
  $messages = array();

  // Custom order check.
  // @todo Use a regexp.
  if ($options['import_format'] == TAXONOMY_CSV_FORMAT_FIELDS) {
    if (!is_array($options['fields_format'])) {
      $messages['fields_format'] = t('Field format is an array of machine name fields.');
    }
    elseif ($options['fields_format'] === array()) {
      $messages['fields_format'] = t("Field format can't be empty.");
    }
    elseif (in_array('', $options['fields_format'])) {
      $messages['fields_format'] = t('One of the fields is empty.');
    }
    // Check if name is the first item.
    elseif ($options['fields_format'][0] != 'name' && $options['fields_format'][0] != 'tid') {
      $messages['fields_format'] = t("First field has to be 'name' or 'tid'.");
    }
    elseif (array_map('strtolower', $options['fields_format']) != $options['fields_format']) {
      $messages['fields_format'] = t("Fields have to be in lower case.");
    }
    elseif (str_replace(' ', '', implode(',', $options['fields_format'])) != implode(',', $options['fields_format'])) {
      $messages['fields_format'] = t("Fields can't have spaces.");
    }
    elseif (in_array('weight', $options['fields_format']) && $options['keep_order']) {
      $messages['keep_order'] = t("You can't keep order of terms when you import weights of terms.");
    }
    // Check items that must be unique.
    else {
      foreach (array(
          'tid',
          'vid',
          'name',
          'description',
          'format',
          'weight',
          'language',
          'i18n_tsid',
          'guid',
          'vocabulary_machine_name',
        ) as $item) {
        if (count(array_keys($options['fields_format'], $item)) > 1) {
          $messages['fields_format'] = t("Fields can have only one '!item'.", array(
            '!item' => $item,
          ));
        }
      }
    }
  }

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

  // Check filter format.
  $list_filter_format_custom = array(
    'none' => t("None (Fixed plain text: user can't choose)"),
  );
  foreach (filter_formats($user) as $format) {
    $list_filter_format_custom[$format->format] = $format->name;
  }
  if (!isset($list_filter_format_custom[$options['filter_format_custom']])) {
    $messages['filter_format_custom'] = t('Filter format for custom fields is not authorized.');
  }

  $list_filter_format = $list_filter_format_custom;
  unset($list_filter_format['none']);
  if (!isset($list_filter_format[$options['filter_format']])) {
    $messages['filter_format'] = t('Filter format for description is not authorized.');
  }

  // Clean locale if any.
  $options['locale_custom'] = trim($options['locale_custom']);

  // Replace machine name with vocabulary id.
  if (!is_numeric($options['vocabulary_id'])) {
    $vocabulary = taxonomy_vocabulary_machine_name_load($options['vocabulary_id']);
    $options['vocabulary_id'] = $vocabulary->vid;
  }

  // Calculates number of lines to be imported.
  if (!isset($options['file'])
      || !is_object($options['file'])
      || count(file($options['file']->filepath)) == 0) {
    $messages['total_lines'] = t('No term to import.');
  }

  if (!in_array($options['vocabulary_target'], array(
      'autocreate',
      'existing',
    ))) {
    $messages['vocabulary_target'] = t('Destination target should be "autocreate" or "existing".');
  }

  if ($options['vocabulary_target'] == 'existing') {
    $list_vocabularies = taxonomy_get_vocabularies();
    if (!isset($list_vocabularies[$options['vocabulary_id']])) {
      $messages['vocabulary_id'] = t("You choose to use an existing vocabulary, but you haven't choose it.");
    }
  }

  // Check default language.
  $list_language = array(
    'und' => t('Language neutral'),
  );
  if (module_exists('locale')) {
    $list_language += locale_language_list('name');
    foreach (locale_language_list('native') as $key => $value) {
      $list_language[$key] .= ' [' . $value . ']';
    }
  }
  if (!isset($list_language[$options['language']])) {
    $messages['language'] = t('The chosen language for terms is not enabled.') . ' ' . t('Add it before import in <a href="!link">locale settings</a>.', array(
      '!link' => url('admin/config/regional/language/overview'),
    ));
  }
  if (!isset($list_language[$options['vocabulary_language']])) {
    $messages['vocabulary_language'] = t('The chosen language for vocabulary is not enabled.') . ' ' . t('Add it before import in <a href="!link">locale settings</a>.', array(
      '!link' => url('admin/config/regional/language/overview'),
    ));
  }

  // Check i18n mode.
  if (module_exists('i18n_taxonomy')) {
    if ($options['vocabulary_target'] != 'autocreate') {
      $vocabulary = taxonomy_vocabulary_load($options['vocabulary_id']);
    }
    // Create a pseudo vocabulary to simplify the check of options.
    else {
      $vocabulary = (object) array(
        'i18n_mode' => $options['i18n_mode'],
        'language' => $options['vocabulary_language'],
      );
    }

    if (($vocabulary->i18n_mode == I18N_MODE_NONE) && ($options['language'] != 'und')) {
      $messages['language'] = t('With a vocabulary without language, terms should have a undefined language.');
    }
    if (($vocabulary->i18n_mode == I18N_MODE_LANGUAGE) && ($options['language'] != $vocabulary->language)) {
      $messages['language'] = t('With a fixed language vocabulary, terms should have the same language than the vocabulary.');
    }
    if (($vocabulary->i18n_mode == I18N_MODE_LOCALIZE) && ($options['language'] != 'und')) {
      $messages['language'] = t('With a localizable vocabulary, terms should have a undefined language.');
    }

    // Check accordance of options.
    if ($options['import_format'] == TAXONOMY_CSV_FORMAT_TRANSLATE) {
      // Vocabulary should be localizable or translatable.
      if (($vocabulary->i18n_mode == I18N_MODE_NONE)
          || ($vocabulary->i18n_mode == I18N_MODE_LANGUAGE)
        ) {
        $messages['i18n_mode'] = t('You need to create or to select a <em>localizable</em> or <em>translatable</em> vocabulary if you want to import translations of names and descriptions.');
      }

      $languages_unset = array_diff($options['translate_languages'], array_flip($list_language));
      if (($vocabulary->i18n_mode == I18N_MODE_LOCALIZE || $vocabulary->i18n_mode == I18N_MODE_TRANSLATE)
          && $languages_unset
        ) {
        $messages['translate_languages'] = t('You can use only existing and enabled languages and not "!languages".', array(
          '!languages' => implode('", "', $languages_unset),
        ));
      }
      if ($vocabulary->i18n_mode == I18N_MODE_TRANSLATE && in_array('und', $options['translate_languages'])) {
        $messages['translate_languages'] = t('You cannot use undefined language ("und") with "Translate" i18n mode.');
      }
      if ($vocabulary->i18n_mode == I18N_MODE_LOCALIZE && $options['translate_languages'][0] != 'und') {
        $messages['translate_languages'] = t('First language should be undefined ("und") with "Localize" i18n mode.');
      }
      if ($vocabulary->i18n_mode == I18N_MODE_LOCALIZE && in_array(language_default('language'), $options['translate_languages'])) {
        $messages['translate_languages'] = t('With "Localize" mode, you cannot translate a term into the default language ("!language"). Term in that language will be the first one on each csv line.', array(
          '!language' => language_default('language') . ' = ' . language_default('name'),
        ));
      }
      if (($vocabulary->i18n_mode == I18N_MODE_LOCALIZE) && ($options['filter_format'] != 'plain_text')) {
        // i18n module requires that the description be plain text to translate it.
        $messages['filter_format'] = t('With a localized vocabulary, you need to choose a plain text description. Choose or create a translatable vocabulary to use html descriptions.');
      }
    }
  }
  elseif ($options['i18n_mode'] != 0) {
    $messages['i18n_mode'] = t('"Taxonomy i18n" module need to be enabled to set an internationalization option.');
  }

  // Custom fields check.
  // @todo Use a regexp.
  if ($options['import_format'] == TAXONOMY_CSV_FORMAT_FIELDS) {
    if (!is_array($options['fields_custom'])) {
      $messages['fields_custom'] = t('Custom fields are an array of machine name fields.');
    }
    elseif ($options['fields_custom'] == array()) {
      // Good
    }
    elseif (in_array('', $options['fields_custom'])) {
      $messages['fields_custom'] = t('One of the custom fields is empty.');
    }
    // Name need to be unique..
    elseif (count(array_unique($options['fields_custom'])) != count($options['fields_custom'])) {
      $messages['fields_custom'] = t("Each custom fields sould have a unique machine name.");
    }
    elseif (array_map('strtolower', $options['fields_custom']) != $options['fields_custom']) {
      $messages['fields_custom'] = t("Custom fields have to be in lower case.");
    }
    elseif (str_replace(' ', '', implode(',', $options['fields_custom'])) != implode(',', $options['fields_custom'])) {
      $messages['fields_custom'] = t("Custom fields can't have spaces.");
    }
    else {
      foreach ($options['fields_custom'] as $key => $value) {
        $field_name = (strpos($value, '|') === FALSE) ?
            $value :
            drupal_substr($value, 0, strpos($value, '|'));
        $field_type = (strpos($value, '|') === FALSE) ?
            'text' :
            drupal_substr($value, strpos($value, '|') + 1);
        if (in_array($field_name, array(
            'tid',
            'vid',
            'name',
            'description',
            'format',
            'weight',
            'language',
            'i18n_tsid',
            'vocabulary_machine_name',
            'guid',
            'parent',
          ))) {
          $messages['fields_custom'] = t("Default term fields don't need to be defined.");
        }
      }
    }
  }

  if (!$options['update_or_ignore']) {
    $messages['update_or_ignore'] = t('Please set what will become existing terms.');
  }
  elseif ($options['update_or_ignore'] == TAXONOMY_CSV_EXISTING_IGNORE
      && ($options['import_format'] == TAXONOMY_CSV_FORMAT_POLYHIERARCHY
        || ($options['import_format'] == TAXONOMY_CSV_FORMAT_STRUCTURE && $options['structure_type'] == TAXONOMY_CSV_FORMAT_POLYHIERARCHY)
        )
    ) {
    $messages['update_or_ignore'] = t('You cannot choose to ignore existing items when you import a polyhierarchical structure.');
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
 * @see taxonomy_csv_import_form()
 */
function taxonomy_csv_import_form_submit($form, &$form_state) {
  // Remember last preferences and prepare only options to be sent to Api.
  foreach (array(
      'import_format',
      'structure_type',
      'import_fields_format',
      'translate_by',
      'translate_languages',
      'keep_order',
      'source_choice',
      'import_delimiter',
      'import_delimiter_soft_tab_width',
      'import_delimiter_custom',
      'import_enclosure',
      'import_enclosure_custom',
      'filter_format',
      'filter_format_custom',
      'import_language',
      'check_line',
      'check_utf8',
      'locale_custom',
      'vocabulary_target',
      'vocabulary_id',
      'i18n_mode',
      'language',
      'import_fields_custom',
      'delete_terms',
      'check_hierarchy',
      'set_hierarchy',
      'update_or_ignore',
      // General options.
      'result_stats',
      'result_terms',
      'result_level',
      'result_type',
    ) as $option) {
    variable_set('taxonomy_csv_' . $option, $form_state['values'][$option]);
    $options[$option] = $form_state['values'][$option];
  }

  // Finish to prepare $options. Unset useless options for api.
  if ($options['source_choice'] == 'text') {
    $options['text'] = &$form_state['values']['text'];
    unset($form_state['values']['text']);
  }

  // Translate languages is trimmed array of translate_languages.
  $options['translate_languages'] = (trim($options['translate_languages'] == '')) ?
    array() :
    array_values(array_map('trim', explode(',', drupal_strtolower($options['translate_languages']))));

  unset($options['import_delimiter']);
  unset($options['import_delimiter_soft_tab_width']);
  unset($options['import_delimiter_custom']);
  unset($options['import_enclosure']);
  unset($options['import_enclosure_custom']);
  $options['delimiter'] = $form_state['values']['delimiter'];
  $options['enclosure'] = $form_state['values']['enclosure'];

  unset($options['import_fields_format']);
  unset($options['import_fields_custom']);
  if ($options['import_format'] == TAXONOMY_CSV_FORMAT_FIELDS) {
    $options['fields_format'] = $form_state['values']['fields_format'];
    $options['fields_custom'] = $form_state['values']['fields_custom'];
  }
  else {
    unset($options['fields_format']);
    unset($options['fields_custom']);
  }

  if ($options['import_format'] == TAXONOMY_CSV_FORMAT_STRUCTURE) {
    $options['import_format'] = $options['structure_type'];
  }
  unset($options['structure_type']);

  // Warning: in API, 'language' is option for terms.
  $options['vocabulary_language'] = $form_state['values']['language'];
  $options['language'] = $options['import_language'];
  unset($options['import_language']);

  $options['file'] = $form_state['values']['file'];
  $options['url']  = $form_state['values']['url'];
  // Internal options.
  $options['check_options'] = FALSE; // Already done.
  $options['result_display'] = TRUE;

  // Prepares process batch (will be automatically processed when returns).
  taxonomy_csv_vocabulary_import($options);
}

/**
 * Restore recommended default values in the import form. Empty validate hook.
 *
 * @see taxonomy_csv_import_form()
 */
function taxonomy_csv_import_form_default_values_validate($form, &$form_state) {
}

/**
 * Restore recommended default values in the import form.
 *
 * @see taxonomy_csv_import_form()
 */
function taxonomy_csv_import_form_default_values_submit($form, &$form_state) {
  foreach (_taxonomy_csv_values('import_default_ui') as $option => $value) {
    variable_set("taxonomy_csv_$option", $value);
  }
  unset($form_state['values']);
  unset($form_state['storage']);

  drupal_set_message(t('Import options have been reset to default.'));
}
