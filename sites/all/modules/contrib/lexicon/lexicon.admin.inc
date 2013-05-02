<?php
/**
 * @file
 * Lexicon administration functions and forms.
 */

/**
 * Introductory stuff for the settings page.
 */
function lexicon_settings_page() {
  $mb_status = extension_loaded('mbstring') ? t('enabled') : t('disabled');
  $overload = ini_get('mbstring.func_overload') ? t('enabled') : t('disabled');
  drupal_set_message(t('Multibyte string support is %status; multibyte function overload is %overload.', array('%status' => $mb_status, '%overload' => $overload)));
  $output = drupal_get_form('lexicon_general_settings_form');
  return $output;
}

/**
 * Lexicon general settings form.
 */
function lexicon_general_settings_form($form, &$form_state) {
  $form = array();

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $options = array();
  $result = db_query('SELECT vid, name FROM {taxonomy_vocabulary} ORDER BY name');

  foreach ($result as $record) {
    $options[$record->vid] = filter_xss_admin($record->name);
  }

  if (!$options) {
    drupal_set_message(t('No vocabularies were found. Until you set up, and select, at least one vocabulary to use as a Lexicon, no lexicon is displayed and no marking of terms in the content of the website can be done.'));
  }

  $form['general']['lexicon_vids'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Which vocabularies act as lexicons.'),
    '#default_value' => variable_get('lexicon_vids', array()),
    '#options' => $options,
    '#description' => t('Select one or more vocabularies which hold terms for your lexicons.'),
    '#multiple' => TRUE,
    '#required' => FALSE,
  );

  $form['general']['lexicon_allow_no_description'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show/mark lexicon terms even if there is no description.'),
    '#default_value' => variable_get('lexicon_allow_no_description', FALSE),
    '#description' => t('By default, Lexicon does not show/mark terms if there is no term description. This setting overrides that. This is useful on free-tagging vocabularies that rarely get descriptions.'),
  );

  $form['lexicon_page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Lexicon Page'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['lexicon_page']['lexicon_page_per_letter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show lexicon across many smaller pages.'),
    '#default_value' => variable_get('lexicon_page_per_letter', FALSE),
    '#description' => t('Do you want to show all terms on one lexicon page or break up the lexicon into a page for each first letter (i.e. many pages).'),
  );

  $form['lexicon_page']['lexicon_separate_letters'] = array(
    '#type' => 'checkbox',
    '#title' => t('Separate letters.'),
    '#default_value' => variable_get('lexicon_separate_letters', FALSE),
    '#description' => t('Separate the terms by the first letters. This will create heading with the letter at the beginning of each section. If the terms are not separated by the first letters, no alphabar will be shown.'),
    '#states' => array(
      'visible' => array(
        ':input[name="lexicon_page_per_letter"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $form['lexicon_page']['lexicon_show_description'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show lexicon term descriptions.'),
    '#default_value' => variable_get('lexicon_show_description', FALSE),
    '#description' => t('Lexicon term descriptions may be large and/or include pictures, therefore the Lexicon can take a long time to load if you include the full descriptions.'),
  );

  $form['lexicon_page']['lexicon_link_to_term_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Link terms to the taxonomy term page.'),
    '#default_value' => variable_get('lexicon_link_to_term_page', FALSE),
    '#description' => t('Do you want the term names to link to their taxonomy term page?'),
  );

  $form['lexicon_page']['lexicon_link_related'] = array(
    '#type' => 'checkbox',
    '#title' => t('Link related terms on the Lexicon page.'),
    '#default_value' => variable_get('lexicon_link_related', TRUE),
    '#description' => t('Do you want terms that are related to link to each other?'),
  );

  $click_options = array(
    t('Show only the single term.'),
    t('Advance the whole lexicon to the term.'),
  );
  $form['lexicon_page']['lexicon_click_option'] = array(
    '#type' => 'select',
    '#title' => t('Clicking on a term link will:'),
    '#options' => $click_options,
    '#default_value' => variable_get('lexicon_click_option', 1),
    '#description' => t('Set the link to show only the single term by using the default taxonomy single term view or go to the lexicon overview.'),
  );

  $form['lexicon_page']['lexicon_show_edit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show "edit" link.'),
    '#default_value' => variable_get('lexicon_show_edit', TRUE),
    '#description' => t('Determines whether or not to show an "edit term" link for each entry.'),
  );

  $form['lexicon_page']['lexicon_show_search'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show "search" link.'),
    '#default_value' => variable_get('lexicon_show_search', TRUE),
    '#description' => t('Determines whether or not to show an "search for term" link for each entry.'),
  );

  $form['lexicon_page']['lexicon_go_to_top_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show "go to top" link.'),
    '#default_value' => variable_get('lexicon_go_to_top_link', FALSE),
    '#description' => t('Determines whether or not to show a "go to top" link after each group of lexicon items per letter.'),
  );

  $form['lexicon_page']['lexicon_go_to_top_link_fragment'] = array(
    '#type' => 'textfield',
    '#title' => t('"Go to top" link anchor name.'),
    '#default_value' => variable_get('lexicon_go_to_top_link_fragment', 'top'),
    '#description' => t('Sets the "go to top" link anchor name to link to.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="lexicon_go_to_top_link"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $form['lexicon_page']['lexicon_local_links_scroll'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use scroll animation for local links.'),
    '#default_value' => variable_get('lexicon_local_links_scroll', FALSE),
    '#description' => t('Determines whether or not to animate scrolling to sections when following local links on the page.'),
  );

  $form['mark_terms'] = array(
    '#type' => 'fieldset',
    '#title' => t('Term marking settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('You have to enable the <em>Lexicon Filter</em> in one or more text formats in the !text_format_settings to enable term marking in content.', array('!text_format_settings' => l(t('Text format settings'), 'admin/config/content/formats'))),
  );

  $form['mark_terms']['lexicon_mark_terms'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mark terms in content.'),
    '#default_value' => variable_get('lexicon_mark_terms', TRUE),
    '#description' => t('Determines whether or not to mark terms in the content.'),
  );

  $form['mark_terms']['match'] = array(
    '#type' => 'fieldset',
    '#title' => t('Term matching'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'invisible' => array(
        ':input[name="lexicon_mark_terms"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $form['mark_terms']['match']['lexicon_match'] = array(
    '#type' => 'select',
    '#title' => t('Match type'),
    '#default_value' => variable_get('lexicon_match', 'b'),
    '#options' => array(
      'b' => t('Word'),
      'lr' => t('Right or left substring'),
      'l' => t('Left substring'),
      'r' => t('Right substring'),
      's' => t('Any substring'),
    ),
    '#description' => t('Choose the match type of lexicon links. "Word" means a word break must occur on both sides of the term. "Right or left" requires a word break on either side. "Left" requires a word break on the left side of the term. "Right" requires a word break on the right. "Any" means any substring will match.'),
  );

  $form['mark_terms']['match']['lexicon_case'] = array(
    '#type' => 'select',
    '#title' => t('Case sensitivity'),
    '#default_value' => variable_get('lexicon_case', '1'),
    '#options' => array(
      t('Case insensitive'),
      t('Case sensitive'),
    ),
    '#description' => t('Match either case sensitive or not. Case sensitive matches are not very resource intensive.'),
  );

  $form['mark_terms']['match']['lexicon_replace_all'] = array(
    '#type' => 'select',
    '#title' => t('Replace matches'),
    '#default_value' => variable_get('lexicon_replace_all', 1),
    '#options' => array(
      t('Only the first match'),
      t('All matches'),
    ),
    '#description' => t('Whether only the first match should be replaced or all matches.'),
  );

  $form['mark_terms']['match']['lexicon_blocking_tags'] = array(
    '#type' => 'textarea',
    '#title' => t('Blocked elements'),
    '#default_value' => variable_get('lexicon_blocking_tags', 'abbr acronym'),
    '#cols' => 60,
    '#rows' => 1,
    '#maxlength' => 512,
    '#description' => t('Which HTML elements (tags) should not include Lexicon links;
      that is, text within these elements will not be scanned for lexicon terms.
      Enter the list separated by a space and do not include < and > characters (e.g. h1 h2).
      To use a %span element to skip text, prefix the class name with a dot (e.g. ".skipping-this").
      All "a," "code," and "pre" elements will be skipped by default.
      ', array('%span' => 'span')),
  );

  $form['mark_terms']['indicator'] = array(
    '#type' => 'fieldset',
    '#title' => t('Link style'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'invisible' => array(
        ':input[name="lexicon_mark_terms"]' => array(
          'checked' => FALSE,
        ),
      ),
    ),
  );

  $indicator_options = array(
    'template' => t('Use the template file: !template_file.', array(
      '!template_file' => '<em>lexicon-mark-term.tpl.php</em>',
    )),
    'superscript' => t('Superscript'),
    'icon' => t('Icon'),
    'iconterm' => t('Icon + Term in span'),
    'term' => t('Term in span'),
    'abbr' => t('Use !type element', array(
      '!type' => l(t('abbr'), 'http://www.w3.org/TR/html401/struct/text.html#edef-ABBR'),
    )),
    'acronym' => t('Use !type element', array(
      '!type' => l(t('acronym'), 'http://www.w3.org/TR/html401/struct/text.html#edef-ACRONYM'),
    )),
    'cite' => t('Use !type element', array(
      '!type' => l(t('cite'), 'http://www.w3.org/TR/html401/struct/text.html#edef-CITE'),
    )),
    'dfn' => t('Use !type element', array(
      '!type' => l(t('dfn'), 'http://www.w3.org/TR/html401/struct/text.html#edef-DFN'),
    )),
  );

  $form['mark_terms']['indicator']['lexicon_replace'] = array(
    '#type' => 'radios',
    '#title' => t('Term Indicator'),
    '#default_value' => variable_get('lexicon_replace', 'template'),
    '#options' => $indicator_options,
    '#description' => t('This determines how the the lexicon term will be marked in the content. The default template renderes a simple hyperlink and gives site developers more control over how terms are marked than the "preset options". The "phrase" items are linked to the standards in case you want to study them.'),
  );

  $form['mark_terms']['indicator']['lexicon_superscript'] = array(
    '#type' => 'textfield',
    '#title' => t('Superscript'),
    '#default_value' =>  variable_get('lexicon_superscript', 'i'),
    '#size' => 15,
    '#maxlength' => 255,
    '#description' => t('Enter the superscript text.'),
    '#states' => array(
      'visible' => array(
        ':input[name="lexicon_replace"]' => array(
          'value' => 'superscript',
        ),
      ),
    ),
  );

  $form['mark_terms']['indicator']['lexicon_icon'] = array(
    '#type' => 'textfield',
    '#title' => t('Lexicon Icon URL'),
    '#default_value' => variable_get('lexicon_icon', drupal_get_path('module', 'lexicon') . '/imgs/lexicon.gif'),
    '#size' => 50,
    '#maxlength' => 255,
    '#description' => t('Enter the URL of the lexicon icon relative to the root ("%root") of your Drupal site.', array('%root' => base_path())),
  );
  // Since #states does not support "OR" a workaround is used:
  // http://api.drupal.org/api/drupal/includes!common.inc/function/
  // drupal_process_states/7#comment-24708.
  $all_options = array_keys($indicator_options);
  $visible = array('icon', 'iconterm');
  foreach (array_diff($all_options, $visible) as $i => $option) {
    $form['mark_terms']['indicator']['lexicon_icon']['#states']['visible'][':input[name="lexicon_replace"], dummy-element' . $i] = array('!value' => $option);
  }

  $form['mark_terms']['indicator']['lexicon_link'] = array(
    '#type' => 'select',
    '#options' => array(
      'none' => t('none'),
      'normal' => t('normal'),
      'absolute' => t('absolute'),
    ),
    '#title' => t('Link type'),
    '#default_value' => variable_get('lexicon_link', 'normal'),
    '#description' => t('You may choose no linking of terms ("none"), standard site linking ("normal"), or "absolute" links. RSS feeds need absolute links to ensure they point back to this site. If you are not providing RSS feeds, it is better to choose one of the other types.'),
  );

  $form['mark_terms']['indicator']['lexicon_term_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Term link class'),
    '#default_value' => variable_get('lexicon_term_class', 'lexicon-term'),
    '#description' => t('This is the style class that will be used for "acronym," "abbr," and "hovertip" links. It should only be used by those with specific site standards.'),
  );

  $form['mark_terms']['indicator']['lexicon_disable_indicator'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow the user to disable lexicon links.'),
    '#default_value' => variable_get('lexicon_disable_indicator', FALSE),
    '#description' => t('Determines whether or not the individual user may disable the Lexicon indicators.'),
  );

  $form['lexicon_clear_filter_cache_on_submit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clear the cache when settings are submitted.'),
    '#default_value' => TRUE,
    '#description' => t('Changes in the filter behaviour are only visible when the filter cache is flushed. This setting ensures that the filter cache is flushed when the settings are submitted.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 5,
  );

  return $form;
}

/**
 * Lexicon general settings form submission function.
 */
function lexicon_general_settings_form_submit($form, &$form_state) {
  variable_set('lexicon_vids', $form_state['values']['lexicon_vids']);
  variable_set('lexicon_disable_indicator', $form_state['values']['lexicon_disable_indicator']);
  variable_set('lexicon_click_option', $form_state['values']['lexicon_click_option']);
  variable_set('lexicon_show_edit', $form_state['values']['lexicon_show_edit']);
  variable_set('lexicon_show_search', $form_state['values']['lexicon_show_search']);
  variable_set('lexicon_go_to_top_link', $form_state['values']['lexicon_go_to_top_link']);
  variable_set('lexicon_go_to_top_link_fragment', $form_state['values']['lexicon_go_to_top_link_fragment']);
  variable_set('lexicon_local_links_scroll', $form_state['values']['lexicon_local_links_scroll']);
  variable_set('lexicon_page_per_letter', $form_state['values']['lexicon_page_per_letter']);
  variable_set('lexicon_separate_letters', $form_state['values']['lexicon_separate_letters']);
  variable_set('lexicon_allow_no_description', $form_state['values']['lexicon_allow_no_description']);
  variable_set('lexicon_show_description', $form_state['values']['lexicon_show_description']);
  variable_set('lexicon_link_to_term_page', $form_state['values']['lexicon_link_to_term_page']);
  variable_set('lexicon_link_related', $form_state['values']['lexicon_link_related']);
  variable_set('lexicon_term_class', $form_state['values']['lexicon_term_class']);
  variable_set('lexicon_mark_terms', $form_state['values']['lexicon_mark_terms']);
  variable_set('lexicon_match', $form_state['values']['lexicon_match']);
  variable_set('lexicon_case', $form_state['values']['lexicon_case']);
  variable_set('lexicon_replace_all', $form_state['values']['lexicon_replace_all']);
  variable_set('lexicon_blocking_tags', $form_state['values']['lexicon_blocking_tags']);
  variable_set('lexicon_link', $form_state['values']['lexicon_link']);
  variable_set('lexicon_replace', $form_state['values']['lexicon_replace']);
  variable_set('lexicon_superscript', $form_state['values']['lexicon_superscript']);
  variable_set('lexicon_icon', $form_state['values']['lexicon_icon']);

  if ($form_state['values']['lexicon_clear_filter_cache_on_submit'] == 1) {
    _lexicon_clear_filter_cache(NULL, TRUE);
  }

  drupal_set_message(t('Configuration has been updated.'));

  return;
}

/**
 *  Lexicon alphabar settings form.
 */
function lexicon_alphabet_form($form, &$form_state) {
  global $language;
  $form = array();

  $form['alphabet'] = array(
    '#type' => 'textarea',
    '#title' => t('Enter all the letters of your alphabet, in the correct order, and in lower case.'),
    '#default_value' => implode(' ', variable_get('lexicon_alphabet', range('a', 'z'))),
    '#description' => t('Separate the letters by a blank.'),
    '#rows' => 1,
  );

  $form['digits'] = array(
    '#type' => 'textarea',
    '#title' => t('Enter all the digits of your alphabet, in the correct order.'),
    '#default_value' => implode(' ', variable_get('lexicon_digits', range('0', '9'))),
    '#description' => t("Separate the digits by a blank. If you don't want terms to start with digits, leave this blank."),
    '#rows' => 1,
  );

  $form['suppress_unused'] = array(
    '#type' => 'checkbox',
    '#title' => t('Suppress unused letters?'),
    '#default_value' => variable_get('lexicon_suppress_unused', FALSE),
    '#description' => t('This will cause unused letters to be omitted from the alphabar.'),
  );

  $ab_seps = array(
    ' ' => t('none'),
    '|' => t('vertical bar (pipe)'),
    '&bull;' => t('bullet'),
    '&#8211;' => t('en-dash (&#8211;)'),
    '&#8212;' => t('em-dash (&#8212;)'),
    '_' => t('underscore'),
  );
  $form['alphabar_separator'] = array(
    '#type' => 'radios',
    '#options' => $ab_seps,
    '#title' => t('Alphabar separator'),
    '#default_value' => variable_get('lexicon_alphabar_separator', '|'),
    '#description' => t('This is the character that will separate the letters in the alphabar.'),
    '#prefix' => '<div class="lexicon_radios">',
    '#suffix' => '</div>',
  );

  $form['alphabar_instruction'] = array(
    '#type' => 'textarea',
    '#title' => t('Alphabar instruction'),
    '#default_value' => variable_get('lexicon_alphabar_instruction', _lexicon_alphabar_instruction_default()),
    '#description' => t('This is the text that will appear immediately below the alphabar.'),
    '#rows' => 1,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 5,
  );

  return $form;
}

/**
 * Lexicon alphabar settings form submission function.
 */
function lexicon_alphabet_form_submit($form, &$form_state) {
  variable_set('lexicon_alphabet', explode(' ', $form_state['values']['alphabet']));
  if ($form_state['values']['digits']) {
    variable_set('lexicon_digits', explode(' ', $form_state['values']['digits']));
  }
  else {
    variable_set('lexicon_digits', array());
  }
  variable_set('lexicon_suppress_unused', $form_state['values']['suppress_unused']);
  variable_set('lexicon_alphabar_separator', $form_state['values']['alphabar_separator']);
  variable_set('lexicon_alphabar_instruction', $form_state['values']['alphabar_instruction']);

  drupal_set_message(t('Configuration saved'), 'status');
}

/**
 * Lexicon path and titles settings form.
 */
function lexicon_paths_and_titles_and_intros_form($form, &$form_state) {
  $form = array();
  $vids = variable_get('lexicon_vids', array());
  $vids_setup = FALSE;

  $form['vids'] = array(
    '#type' => 'value',
    '#value' => $vids,
  );

  foreach ($vids as $vid) {
    // Don't create form-items for vocabularies that have not been setup as
    // Lexicon vocabularies.
    if ($vid != 0) {
      $vids_setup = TRUE;

      $result = db_query('SELECT name FROM {taxonomy_vocabulary} WHERE vid = :vid',
      array(
        ':vid' => $vid,
      ));
      $vocabulary = $result->fetch();
      $vocabulary_name = filter_xss_admin($vocabulary->name);

      $form['paths_and_titles_and_intros' . $vid] = array(
        '#type' => 'fieldset',
        '#title' => t('Path, title and intro settings for %vocabulary_name', array('%vocabulary_name' => $vocabulary_name)),
        '#collapsible' => TRUE,
      );

      $form['paths_and_titles_and_intros' . $vid]['lexicon_path_' . $vid] = array(
        '#type' => 'textfield',
        '#title' => t('The path of the lexicon for this vocabulary'),
        '#default_value' => variable_get('lexicon_path_' . $vid, 'lexicon/' . $vid),
        '#description' => t('Determines the path that is used for the lexicon page for this vocabulary. Default is: <em>%path</em>.', array('%path' => 'lexicon/' . $vid)),
        '#required' => TRUE,
      );

      $form['paths_and_titles_and_intros' . $vid]['lexicon_title_' . $vid] = array(
        '#type' => 'textfield',
        '#title' => t('The title of the lexicon for this vocabulary'),
        '#default_value' => variable_get('lexicon_title_' . $vid, $vocabulary_name),
        '#description' => t('Determines the title that is used for the lexicon page for this vocabulary. Default is: <em>%name</em>.', array('%name' => $vocabulary_name)),
        '#required' => TRUE,
      );

      $introduction_text = '';
      $introduction_text_format = variable_get('lexicon_introduction_' . $vid, NULL);
      if ($introduction_text_format != NULL) {
        $introduction_text = $introduction_text_format['value'];
      }

      $form['paths_and_titles_and_intros' . $vid]['lexicon_introduction_' . $vid] = array(
        '#type' => 'text_format',
        '#title' => t('The optional introduction text for this vocabulary'),
        '#default_value' => $introduction_text,
        '#description' => t('The optional introduction text that is displayed at the top of the Lexicon overview or, when the Lexicon is split over multiple pages, is shown on the Lexicon start page.', array('%name' => $vocabulary_name)),
        '#required' => FALSE,
      );
    }
  }

  if ($vids_setup) {
    $form['lexicon_clear_menu_cache_on_submit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Clear the menu cache when settings are submitted.'),
      '#default_value' => TRUE,
      '#description' => t('Changes in the paths and titles are only visible when the menu cache is flushed. This setting ensures that the menu cache is flushed when the settings are submitted.'),
      '#prefix' => '<div class="lexicon_clear_menu_cache_on_submit">',
      '#suffix' => '</div>',
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
      '#weight' => 5,
    );
  }
  else {
    drupal_set_message(t('No vocabularies were found. Until you set up, and select, at least one vocabulary for Lexicon, no settings can be entered.'));
  }

  return $form;
}

/**
 * Lexicon paths and titles settings form submission function.
 */
function lexicon_paths_and_titles_and_intros_form_submit($form, &$form_state) {
  $vids = $form_state['values']['vids'];

  foreach ($vids as $vid) {
    if ($vid != 0) {
      variable_set('lexicon_path_' . $vid, $form_state['values']['lexicon_path_' . $vid]);
      variable_set('lexicon_title_' . $vid, $form_state['values']['lexicon_title_' . $vid]);
      variable_set('lexicon_introduction_' . $vid, $form_state['values']['lexicon_introduction_' . $vid]);
    }
  }

  if ($form_state['values']['lexicon_clear_menu_cache_on_submit'] == 1) {
    _lexicon_clear_menu_cache();
    _lexicon_clear_filter_cache(NULL, TRUE);
  }

  drupal_set_message(t('Configuration saved'), 'status');
}

/**
 * Lexicon related terms and synonyms settings form.
 */
function lexicon_related_terms_and_synonyms_and_image_form($form, &$form_state) {
  $form = array();

  $vids = variable_get('lexicon_vids', array());
  $vocs = taxonomy_get_vocabularies();
  $vids_machine_names = array();
  $vids_setup = FALSE;

  $form['vids'] = array(
    '#type' => 'value',
    '#value' => $vids,
  );

  foreach ($vids as $vid) {
    // Don't create form-items for vocabularies that have not been setup as
    // Lexicon vocabularies.
    if ($vid != 0) {
      $vids_setup = TRUE;
      // Until appropriate fields have been found there is no setup for related
      // terms, synonyms or image.
      $related_terms_setup = $synonyms_setup = $image_setup = FALSE;

      // Create the option "none" for each select option.
      $options_related_terms = $options_synonyms = $options_image = array(
        '' => t('none'),
      );

      // Get all the instances of fields related to the current vocabulary.
      $instances = field_info_instances('taxonomy_term', $vocs[(int) $vid]->machine_name);

      foreach ($instances as $field) {
        // Check if the field is not "deleted" or else field_info_field call
        // will fail? (api.drupal.org)
        if ($field['deleted'] == "0") {
          $field_info = field_info_field($field['field_name']);
          switch ($field_info["type"]) {
            // If the type is "taxonomy_term_refernence" then it might be a
            // field for "Related terms"
            case "taxonomy_term_reference":
              $options_related_terms[$field['field_name']] = $field['label'];
              $related_terms_setup = TRUE;
              break;

            // If the type is "text" then it might be a field for "Synonyms"
            case "text":
              $options_synonyms[$field['field_name']] = $field['label'];
              $synonyms_setup = TRUE;
              break;

            // If the type is "image" then it might be a field for "Image"
            case "image":
              $options_image[$field['field_name']] = $field['label'];
              $image_setup = TRUE;
              break;

            default:
              break;
          }
        }
      }

      $result = db_query('SELECT name FROM {taxonomy_vocabulary} WHERE vid = :vid',
      array(
        ':vid' => $vid,
      ));
      $vocabulary = $result->fetch();
      $vocabulary_name = filter_xss_admin($vocabulary->name);

       // Don't create a fieldset if there are no settings to configure.
      if ($related_terms_setup || $synonyms_setup || $image_setup) {
        $form['related_terms_and_synonyms_and_image_' . $vid] = array(
          '#type' => 'fieldset',
          '#title' => t('Related terms, synonyms and image settings for %vocabulary_name', array('%vocabulary_name' => $vocabulary_name)),
          '#collapsible' => TRUE,
        );
      }

      if ($related_terms_setup) {
        $form['related_terms_and_synonyms_and_image_' . $vid]['lexicon_related_terms_field_' . $vid] = array(
          '#type' => 'select',
          '#options' => $options_related_terms,
          '#title' => t('Field for related terms'),
          '#default_value' => variable_get('lexicon_related_terms_field_' . $vid, ''),
          '#description' => t('Determines if related terms are shown and which field is used for the related terms. The default value is : <em>none</em>.'),
          '#prefix' => '<div class="lexicon_related_terms">',
          '#suffix' => '</div>',
          '#required' => FALSE,
        );
      }
      else {
        drupal_set_message(t('No fields for related terms for the vocabulary <em> %vocabulary_name </em> were found. Until you set up at least one related terms field of the type "Term reference" for the vocabulary, no field can be selected.', array('%vocabulary_name' => $vocabulary_name)));
      }

      if ($synonyms_setup) {
        $form['related_terms_and_synonyms_and_image_' . $vid]['lexicon_synonyms_field_' . $vid] = array(
          '#type' => 'select',
          '#options' => $options_synonyms,
          '#title' => t('Field for synonyms'),
          '#default_value' => variable_get('lexicon_synonyms_field_' . $vid, ''),
          '#description' => t('Determines if synonyms are shown and which field is used for synonyms. The default value is :') . ' <em>none</em>.',
          '#prefix' => '<div class="lexicon_synonyms">',
          '#suffix' => '</div>',
          '#required' => FALSE,
        );
      }
      else {
        drupal_set_message(t('No fields for synonyms for the vocabulary <em> %vocabulary_name </em> were found. Until you set up at least one synonyms field of the type "Text" for the vocabulary, no field can be selected.', array('%vocabulary_name' => $vocabulary_name)));
      }

      if ($image_setup) {
        $form['related_terms_and_synonyms_and_image_' . $vid]['lexicon_image_field_' . $vid] = array(
          '#type' => 'select',
          '#options' => $options_image,
          '#title' => t('Field for image'),
          '#default_value' => variable_get('lexicon_image_field_' . $vid, ''),
          '#description' => t('Determines if images are shown and which field is used as the image. The default value is :') . ' <em>none</em>.',
          '#prefix' => '<div class="lexicon_image">',
          '#suffix' => '</div>',
          '#required' => FALSE,
        );
      }
      else {
        drupal_set_message(t('No fields for image for the vocabulary <em> %vocabulary_name </em> were found. Until you set up at least one image field of the type "Image" for the vocabulary, no field can be selected.', array('%vocabulary_name' => $vocabulary_name)));
      }
    }
  }

  if ($vids_setup && ($related_terms_setup || $synonyms_setup || $image_setup)) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
      '#weight' => 5,
    );
  }
  else {
    if (!$vids_setup) {
      drupal_set_message(t('No vocabularies were found. Until you set up, and select, at least one vocabulary for Lexicon, no settings can be entered.'));
    }
  }

  return $form;
}

/**
 * Lexicon related terms and synonyms settings form submission function.
 */
function lexicon_related_terms_and_synonyms_and_image_form_submit($form, &$form_state) {
  $vids = $form_state['values']['vids'];
  foreach ($vids as $vid) {
    if ($vid != 0) {
      if (isset($form_state['values']['lexicon_related_terms_field_' . $vid])) {
        variable_set('lexicon_related_terms_field_' . $vid, $form_state['values']['lexicon_related_terms_field_' . $vid]);
      }
      if (isset($form_state['values']['lexicon_synonyms_field_' . $vid])) {
        variable_set('lexicon_synonyms_field_' . $vid, $form_state['values']['lexicon_synonyms_field_' . $vid]);
      }
      if (isset($form_state['values']['lexicon_image_field_' . $vid])) {
        variable_set('lexicon_image_field_' . $vid, $form_state['values']['lexicon_image_field_' . $vid]);
      }
    }
  }

  drupal_set_message(t('Configuration saved'), 'status');
}
