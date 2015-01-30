<?php

/**
 * @file
 *   Translation view and editing pages for localization community.
 */

// = Filter form handling ======================================================

/**
 * Translate form filter.
 *
 * @param $langcode
 *   Language code
 * @param $filters
 *   Array of filter options.
 */
function l10n_community_filter_form($form, &$form_state, $langcode, $filters) {
  $projects = l10n_server_get_projects();

  $translation_options = array(
    L10N_STATUS_ALL => '<' . t('Any') . '>',
    L10N_STATUS_UNTRANSLATED => t('Untranslated'),
    L10N_STATUS_TRANSLATED => t('Translated'),
    L10N_STATUS_IS_TRANSLATION => t('Is translation'),
  );
  $suggestion_options = array(
    L10N_STATUS_ALL => '<' . t('Any') . '>',
    L10N_STATUS_NO_SUGGESTION => t('Has no suggestion'),
    L10N_STATUS_HAS_SUGGESTION => t('Has suggestion'),
    L10N_STATUS_IS_SUGGESTION => t('Is suggestion'),
  );

  // This form can be submitted multiple times to support AJAX flows.
  // So we set an initial default value set for the form values, if there
  // was none submitted already. Otherwise, the submitted values will take
  // on being the defaults.
  if (empty($form_state['values'])) {
    $form_state['values'] = array(
      'project' => isset($filters['project']) ? $filters['project']->title : '',
      'release' => isset($filters['release']) ? $filters['release']        : 'all',
      'context' => isset($filters['context']) ? $filters['context']        : 'all',
      'status' => isset($filters['status'])  ? $filters['status']         : L10N_STATUS_ALL,
      'author' => isset($filters['author'])  ? $filters['author']->name   : '',
      'search' => isset($filters['search'])  ? $filters['search']         : '',
      'limit' => isset($filters['limit'])   ? $filters['limit']          : 10,
      'sid' => isset($filters['sid'])     ? $filters['sid']            : 0,
    );
  }

  $form = array(
    '#prefix' => '<div class="l10n-server-filter clear-block">',
    '#suffix' => '</div>',
  );
  $class_is_default_project = empty($form_state['values']['project']) ? 'is-default' : 'is-not-default';
  $form['project'] = array(
    '#title' => t('Project'),
    '#default_value' => $form_state['values']['project'],
    '#ajax' => array(
      // 'event' missing, Drupal will apply the per-#type defaults.
      'callback' => 'l10n_community_ajax_releases',
      'wrapper' => 'l10n-server-releases',
      'effect' => 'fade',
    ),
    '#attributes' => array('class' => array($class_is_default_project)),
    '#theme_wrappers' => array('form_element', 'l10n_community_filter_form_element'),
  );
  if (($count = count($projects)) <= 30) {
    // Select widget for 1-30 projects.
    $form['project']['#type'] = 'select';
    $form['project']['#options'] = array('' => t('All'));
    foreach ($projects as $project) {
      // URI used to shorten the lookup cycle in filter sanitization.
      $form['project']['#options'][$project->title] = $project->title;
    }
  }
  else {
    // Autocomplete field for more then 30 projects.
    $form['project'] += array(
      '#type' => 'textfield',
      '#autocomplete_path' => 'translate/project-autocomplete',
      '#size' => 20,
    );
  }

  // Always add releases option, so we can update it when
  // needed as the project changes with AJAX.
  $release_options = array('all' => t('All'));
  if (!empty($form_state['values']['project'])) {
    $selected_project = $projects[l10n_community_project_uri_by_title($form_state['values']['project'])];
    $releases = l10n_server_get_releases($selected_project->uri);
    foreach ($releases as $rid => $release) {
      $release_options[$rid] = $release->title;
    }
  }
  if (!isset($release_options[$form_state['values']['release']])) {
    $form_state['values']['release'] = 'all';
  }
  $class_is_default_release = $form_state['values']['release'] == 'all' ? 'is-default' : 'is-not-default';
  $form['release'] = array(
    '#title' => t('Release'),
    '#type' => 'select',
    '#options' => $release_options,
    '#default_value' => $form_state['values']['release'],
    // Wrapper used to replace release options via AJAX.
    '#prefix' => '<div id="l10n-server-releases">',
    '#suffix' => '</div>',
    '#attributes' => array('class' => array($class_is_default_release)),
    '#theme_wrappers' => array('form_element', 'l10n_community_filter_form_element'),
  );

  if (count($contexts = l10n_community_get_contexts()) > 1) {
    $class_is_default_context = $form_state['values']['context'] == 'all' ? 'is-default' : 'is-not-default';
    $form['context'] = array(
      '#title' => t('Context'),
      '#type' => 'select',
      '#options' => array('all' => t('All')) + $contexts,
      '#default_value' => $form_state['values']['context'],
      '#attributes' => array('class' => array($class_is_default_context)),
      '#theme_wrappers' => array('form_element', 'l10n_community_filter_form_element'),
    );
  }

  $form['status'] = array(
    '#title' => t('Status'),
    '#tree' => TRUE,
    '#prefix' => '<div class="' . ($form_state['values']['status'] == L10N_STATUS_ALL ? 'is-default' : 'is-not-default') . '">',
    '#suffix' => '</div>',
    '#display_none' => $form_state['values']['status'] == L10N_STATUS_ALL,
    '#theme_wrappers' => array('form_element', 'l10n_community_filter_form_element'),
  );
  $form['status']['translation'] = array(
    '#type' => 'select',
    '#options' => $translation_options,
    '#default_value' => $form_state['values']['status'] & (L10N_STATUS_TRANSLATED | L10N_STATUS_UNTRANSLATED | L10N_STATUS_IS_TRANSLATION),
  );
  $form['status']['suggestion'] = array(
    '#type' => 'select',
    '#options' => $suggestion_options,
    '#default_value' => $form_state['values']['status'] & (L10N_STATUS_HAS_SUGGESTION | L10N_STATUS_NO_SUGGESTION | L10N_STATUS_IS_SUGGESTION),
  );

  $class_is_default_author = empty($form_state['values']['author']) ? 'is-default' : 'is-not-default';
  $form['author'] = array(
    '#type' => 'textfield',
    '#title' => t('Submitted by'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => $form_state['values']['author'],
    '#size' => 15,
    '#attributes' => array('class' => array($class_is_default_author)),
    '#display_none' => empty($form_state['values']['author']),
    '#theme_wrappers' => array('form_element', 'l10n_community_filter_form_element'),
  );

  $class_is_default_search = empty($form_state['values']['search']) ? 'is-default' : 'is-not-default';
  $form['search'] = array(
    '#title' => t('Contains'),
    '#type' => 'textfield',
    '#default_value' => $form_state['values']['search'],
    '#size' => 20,
    '#attributes' => array('class' => array($class_is_default_search)),
    '#display_none' => empty($form_state['values']['search']),
    '#theme_wrappers' => array('form_element', 'l10n_community_filter_form_element'),
  );

  $class_is_default_sid = empty($form_state['values']['sid']) ? 'is-default' : 'is-not-default';
  $form['sid'] = array(
    '#title' => t('String ID'),
    '#type' => 'textfield',
    '#default_value' => empty($form_state['values']['sid']) ? '' : $form_state['values']['sid'],
    '#size' => 10,
    '#attributes' => array('class' => array($class_is_default_sid)),
    '#display_none' => empty($form_state['values']['sid']),
    '#theme_wrappers' => array('form_element', 'l10n_community_filter_form_element'),
  );

  $class_is_default_limit = $form_state['values']['limit'] == 10 ? 'is-default' : 'is-not-default';
  $form['limit'] = array(
    '#type' => 'select',
    '#title' => t('Limit'),
    '#options' => drupal_map_assoc(array(5, 10, 20, 30, 50)),
    '#default_value' => $form_state['values']['limit'],
    '#attributes' => array('class' => array($class_is_default_limit)),
    '#display_none' => $form_state['values']['limit'] == 10,
    '#theme_wrappers' => array('form_element', 'l10n_community_filter_form_element'),
  );

  $form['buttons'] = array(
    '#prefix' => '<div class="filter-submit">',
    '#suffix' => '</div>',
  );
  $form['buttons']['submit'] = array(
    '#value' => t('Filter'),
    '#type' => 'submit',
  );
  $form['buttons']['reset'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="reset-link">' . l(t('Clear all filters'), 'translate/languages/' . $langcode . '/translate', array('attributes' => array('class' => array('filter-exclude')))) . '</div>',
  );
  return $form;
}

/**
 * Theme function for l10n_community_filter_form elements.
 */
function theme_l10n_community_filter_form_element($variables) {
  $element = $variables['element'];

  // Bubble up a js-hide class to the container.
  $class = '';
  if (!empty($element['#display_none'])) {
    $class = ' js-hide';
  }

  return '<div class="filter-widget' . $class . '">' . $element['#children'] . '</div>';
}

/**
 * Submission handler for filtering form.
 */
function l10n_community_filter_form_submit($form, &$form_state) {

  if ($form_state['values']['op'] == t('Reset')) {
    // Just go with the redirection flow => removes URL params.
    return;
  }

  if ($form_state['values']['op'] == t('Filter')) {
    $filters = l10n_community_build_filter_values($form_state['values']);
    // Redirect keeping the relevant filters intact in the URL.
    $form_state['redirect'] = array($_GET['q'], array('query' => l10n_community_flat_filters($filters)));
  }
}

// = Translation form building =================================================

/**
 * Menu callback: List translations and suggestions.
 */
function l10n_community_translate_page($langcode) {
  drupal_add_css(drupal_get_path('module', 'l10n_community') . '/editor.css');
  drupal_add_js(drupal_get_path('module', 'l10n_community') . '/jquery.worddiff.js');
  drupal_add_js(drupal_get_path('module', 'l10n_community') . '/editor.js');
  drupal_add_js(drupal_get_path('module', 'l10n_community') . '/l10n_community.js');

  $language = l10n_community_get_language($langcode);
  $filters = l10n_community_build_filter_values($_GET);
  l10n_community_add_url_modifiers($langcode, $filters);
  $strings = l10n_community_get_strings($langcode, $filters, $filters['limit']);

  // Add RTL style if the current language's direction is RTL
  if ($language->direction == LANGUAGE_RTL) {
    drupal_add_css(drupal_get_path('module', 'l10n_community') . '/editor-rtl.css');
  }

  // Set the most appropriate title.
  if ($filters['project']) {
    drupal_set_title(t('Translate %project to @language', array('%project' => $filters['project']->title, '@language' => t($language->name))), PASS_THROUGH);
  }
  else {
    drupal_set_title(t('Translate to @language', array('@language' => t($language->name))), PASS_THROUGH);
  }

  // Add missing breadcrumb.
  drupal_set_breadcrumb(
    array(
      l(t('Home'), NULL),
      l(t('Translate'), 'translate'),
      l(t('Explore languages'), 'translate/languages'),
      l(t($language->name), 'translate/languages/' . $langcode),
    )
  );

  // Add the filter form.
  $output = array(
    drupal_get_form('l10n_community_filter_form', $langcode, $filters)
  );

  // Output the actual strings.
  if (!count($strings)) {
    drupal_set_message(t('No strings found with this filter. Try adjusting the filter options.'));
  }
  else {
    $output[] = drupal_get_form('l10n_community_translate_form', $language, $filters, $strings);
  }
  return $output;
}

/**
 * Form callback: List translations and suggestions.
 *
 * @param $form_state
 *   The form state array.
 * @param $language
 *   A language object.
 * @param $filters
 *   An array of filters applied to the strings.
 * @param $strings
 *   The strings to render.
 */
function l10n_community_translate_form($form, &$form_state, $language, $filters, $strings) {
  $pager = theme('pager', array('tags' => NULL, 'element' => 0));

  // The form will show but the submit buttons will only appear if the user has
  // permissions to submit suggestions. This allows the use of this form to review
  // strings in the database.
  $form = array(
    'target' => array(
      '#type' => 'value',
      '#value' => $_GET,
    ),
    'langcode' => array(
      '#type' => 'value',
      '#value' => $language->language,
    ),
    'pager_top' => array(
      '#weight' => -10,
      '#markup' => $pager,
    ),
    'strings' => array(
      '#tree' => TRUE,
      '#theme' => 'l10n_community_translate_table',
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save changes'),
      '#access' => l10n_community_access('submit suggestions'),
    ),
    'pager_bottom' => array(
      '#weight' => 10,
      '#markup' => $pager,
    ),
  );

  // Fill in string values for the editor.
  foreach ($strings as $string) {
    $form['strings'][$string->sid] = _l10n_community_translate_form_item($form_state, $string, $language, $filters);
  }

  return $form;
}

/**
 * Creates the form fragment for a source string.
 */
function _l10n_community_translate_form_item(&$form_state, $source, $language, $filters) {

  if (!$source->translation) {
    // If there is no picked translation yet, simulate with a visible placeholder.
    $source->tid = '0';
    $source->translation = array(t('(not translated)'));
    $source->is_active = '1';
    $source->is_suggestion = '0';
  }
  else {
    $source->translation = l10n_community_unpack_string($source->translation);
  }
  $source_unpacked = $source->value;
  $source->value = l10n_community_unpack_string($source->value);

  $form = array(
    '#string' => $source,
    '#langcode' => $language->language,
    'filters_match' => array(
      '#type' => 'value',
      '#value' => ($source_match = (!empty($filters['search']) && (stripos($source_unpacked, $filters['search']) !== FALSE))),
    ),
    'source' => array(
      'string' => array('#markup' => _l10n_community_translate_render_textarray($source->value)),
    ),
  );

  if (l10n_community_access('submit suggestions')) {
    $form['source']['edit'] = array(
      '#markup' => t('Translate'),
      '#prefix' => '<label title="' . t('Translate') . '">',
      '#suffix' => '</label>',
    );
  }

  // Add the current string (either as approved translation or a mock object
  // for the "untranslated" string).
  $form[$source->tid] = _l10n_community_translate_translation($form_state, $source, $source, $filters, $source_match);

  // When there are suggestions, load them from the database.
  if ($source->has_suggestion) {
    $result = db_query("SELECT t.tid, t.sid, t.translation, t.uid_entered, t.time_entered, t.time_changed, t.is_active, t.is_suggestion, u.name as username FROM {l10n_server_translation} t LEFT JOIN {users} u ON u.uid = t.uid_entered WHERE t.language = :language AND t.sid = :sid AND t.is_active = 1 AND t.is_suggestion = 1 ORDER BY t.time_entered", array(':language' => $language->language, ':sid' => $source->sid));
    foreach ($result as $suggestion) {
      $suggestion->translation = l10n_community_unpack_string($suggestion->translation);
      // Add the suggestion to the list.
      $form[$suggestion->tid] = _l10n_community_translate_translation($form_state, $suggestion, $source, $filters, $source_match);
    }
  }

  // If the user may add new suggestions, display a textarea.
  if (l10n_community_access('submit suggestions')) {
    $textarea = _l10n_community_translate_translation_textarea($source, $language->plurals);
    $form[$textarea->tid] = _l10n_community_translate_translation($form_state, $textarea, $source, $filters, $source_match);
  }
  return $form;
}

/**
 * Generate markup for an unpacked string.
 *
 * @param $textarray
 *   An array of strings as generated by l10n_community_unpack_string().
 * @param $empty
 *   Specific data to include as the data to use when empty.
 */
function _l10n_community_translate_render_textarray($textarray, $empty = '') {
  // data-empty is a proprietary attribute used in editor.css to be displayed when
  // starting to submit a new suggestion.
  $empty = !empty($empty) ? ' data-empty="' . check_plain($empty) . '"' : '';
  return "<span$empty>" . implode("</span><br /><span$empty>", array_map('check_plain', $textarray)) . '</span>';
}

/**
 * Build mock object for new textarea.
 */
function _l10n_community_translate_translation_textarea($source, $nplurals) {
  global $user;

  return (object) array(
    'sid' => $source->sid,
    'tid' => 'new',
    // Fill in with as many items as required. If the source was plural, we
    // need to fill in with a number adequate for this language.
    'translation' => array_fill(0, count($source->value) > 1 ? $nplurals : 1, ''),
    'is_active' => '1',
    'is_suggestion' => '1',
    'uid_entered' => $user->uid,
  );
}

/**
 * Creates the form fragment for a translated string.
 */
function _l10n_community_translate_translation(&$form_state, $string, $source, $filters, $source_match) {
  global $user;

  $is_own = $user->uid == $string->uid_entered;
  $is_active = $string->is_active && !$string->is_suggestion;
  $is_new = $string->tid == 'new';
  $may_moderate = ($is_own ? l10n_community_access('moderate own suggestions') : l10n_community_access('moderate suggestions from others'));

  // This string is a match if it was data from the database and was entered by
  // the searched user or its text included the searched portion.
  $filters_matched = array();
  $filters_to_match = 0;
  if ((int) $string->tid > 0) {
    // If we had a real tid for this, it has a uid_entered and translation loaded.
    if (!empty($filters['author'])) {
      $filters_matched[] = $string->uid_entered == $filters['author']->uid;
      $filters_to_match++;
    }
    if (!empty($filters['search'])) {
      // Search is matched if the source matched the search and the translation
      // matched the user or the translation matched both.
      $filters_matched[] = (!empty($filters['author']) && $source_match) || (stripos(l10n_community_pack_string($string->translation), $filters['search']) !== FALSE);
      $filters_to_match++;
    }
    $filters_matched = array_filter($filters_matched);
  }

  $form = array(
    '#theme' => 'l10n_community_translate_translation',
    'original' => array(
      '#type' => 'value',
      '#value' => $string,
    ),
    'filters_match' => array(
      '#type' => 'value',
      '#value' => ($filters_to_match > 0) && (count($filters_matched) == $filters_to_match),
    ),
  );

  // Active radio box is used to pick the approved translation.
  $form['active'] = array(
    '#type' => 'radio',
    '#theme' => 'l10n_community_translate_radio',
    '#theme_wrappers' => array(),
    '#title' => _l10n_community_translate_render_textarray($string->translation, $is_new ? t('(empty)') : FALSE),
    '#return_value' => $string->tid,
    '#default_value' => $is_active ? $string->tid : NULL,
    '#parents' => array('strings', $string->sid, 'active'),
    // Let a moderator roll back to the current translation even if they
    // would otherwise not have permission to approve such a string.
    '#disabled' => !$may_moderate && !$is_active,
    '#attributes' => array('class' => array('selector')),
  );

  if ($string->tid) {
    if ($may_moderate && $string->tid != 'new') {
      $form['declined'] = array(
        '#type' => 'checkbox',
        '#title' => t('Decline'),
        '#default_value' => !($string->is_active || $string->is_suggestion),
      );
    }
    if ($string->tid == 'new') {
      // Fill in with as many textareas as required to enter translation
      // for this string.
      $form['value'] = array_fill(0, count($string->translation), array(
                         '#type' => 'textarea',
                         '#cols' => 60,
                         '#rows' => 3,
                         '#default_value' => t('<New translation>'),
                       ));
    }
    else {
      if (l10n_community_access('submit suggestions')) {
        $form['edit'] = array(
          '#markup' => t('Edit a copy'),
          '#prefix' => '<label title="' . t('Edit a copy') . '">',
          '#suffix' => '</label>',
        );
      }
      if (isset($string->username)) {
        $title = l10n_community_translate_byline($string->username, $string->uid_entered, $string->time_entered, -1, -1, FALSE);

        $form['author'] = array(
          '#markup' => $title,
          '#prefix' => '<div class="l10n-byline"><a href="' . url('translate/translation-details/' . $string->tid) . '" class="l10n-more-link" title="' . t('Show full history for translation.') . '">',
          '#suffix' => '</a><div class="l10n-more-info"></div></div>',
        );
      }
    }
  }
  return $form;
}

/**
 * Generates the byline containing meta information about a string.
 */
function l10n_community_translate_byline($name, $uid, $time, $medium, $type, $link_user = TRUE) {
  $params = array(
    '!author' => $uid ? ($link_user ? theme('username', array('account' => (object)array('name' => $name, 'uid' => $uid))) : check_plain($name)) : t('unknown user'),
    // Also skip handling time if uid was not specified (for decline entries
    // in the update, which have time for ordering reasons, but no uid).
    '@date' => $time && $uid ? format_date($time) : t('unknown time'),
    '@ago' => $time ? t('@time ago', array('@time' => format_interval(REQUEST_TIME - $time))) : t('no time record available'),
  );
  switch ($type) {
    case L10N_SERVER_ACTION_ADD:
      switch ($medium) {
        case L10N_SERVER_MEDIUM_IMPORT:
          return t('imported by !author <span title="@ago">on @date</span>', $params);
        case L10N_SERVER_MEDIUM_REMOTE:
          return t('remotely submitted by !author <span title="@ago">on @date</span>', $params);
        case L10N_SERVER_MEDIUM_WEB:
          return t('suggested on the web by !author <span title="@ago">on @date</span>', $params);
        case L10N_SERVER_MEDIUM_UNKNOWN:
          return t('suggested by !author <span title="@ago">on @date</span> (source unknown)', $params);
      }
      return;
    case L10N_SERVER_ACTION_READD:
      switch ($medium) {
        case L10N_SERVER_MEDIUM_IMPORT:
          return t('re-imported by !author <span title="@ago">on @date</span>', $params);
        case L10N_SERVER_MEDIUM_REMOTE:
          return t('remotely re-submitted by !author <span title="@ago">on @date</span>', $params);
        case L10N_SERVER_MEDIUM_WEB:
          return t('re-suggested on the web by !author <span title="@ago">on @date</span>', $params);
          // L10N_SERVER_MEDIUM_UNKNOWN does not apply, because we only have that
          // for backwards compatibility and L10N_SERVER_ACTION_READD did not
          // happen with data migrated (at least we did not know about it).
      }
      return;
    case L10N_SERVER_ACTION_APPROVE:
      return t('approved by !author <span title="@ago">on @date</span>', $params);
    case L10N_SERVER_ACTION_DECLINE:
      return t('declined by !author <span title="@ago">on @date</span>', $params);
    case L10N_SERVER_ACTION_DEMOTE:
      return t('demoted by !author <span title="@ago">on @date</span>', $params);
    default:
      // Default byline that work as a click-target to get more information.
      return t('by !author <span title="@ago">on @date</span>', $params);
  }
}

// = Translation form theming ==================================================

/**
 * Main theme function for translation table.
 */
function theme_l10n_community_translate_table($variables) {
  $element = $variables['element'];
  $header = array(
    array(
      'data' => t('Source text'),
      'colspan' => 2,
    ),
    t('Translations'),
  );

  $rows = array();

  foreach (element_children($element) as $key) {
    $rows[] = array(
      array(
        'class' => array('sid'),
        'data' => l('#', $_GET['q'], array('query' => array('sid' => $element[$key]['#string']->sid), 'attributes' => array('title' => t('Direct and permanent link to this string.')))),
      ),
      array(
        'class' => array('source'),
        'data' => theme('l10n_community_translate_source', array('element' => $element[$key])),
      ),
      array(
        'class' => array('translation'),
        'data' => theme('l10n_community_translate_translation_list', array('element' => $element[$key])),
      ),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('l10n-table'))));
}

/**
 * Theme function for a source cell in the translation table.
 */
function theme_l10n_community_translate_source($variables) {
  $element = $variables['element'];
  $output = theme('l10n_community_translate_actions', array('element' => $element['source']));
  $output .= '<label class="l10n-string' . ($element['filters_match']['#value'] ? ' filter-match' : ' filter-no-match') . '">' . drupal_render($element['source']['string']) . '</label>';
  $output .= theme('l10n_community_in_context', array('source' => $element['#string']));

  $output .= '<div class="l10n-usage"><a href="' . url('translate/source-details/' . $element['#string']->sid) . '" class="l10n-more-link" title="' . t('Show list of projects and releases where this text is used.') . '">' . t('Show related projects') . '</a><div class="l10n-more-info"></div></div>';
  return $output;
}

/**
 * Theme appropriate actions for the given string element.
 */
function theme_l10n_community_translate_actions($variables) {
  $element = $variables['element'];
  $actions = '';
  foreach (array('declined', 'edit') as $type) {
    if (isset($element[$type])) {
      $actions .= '<li class="' . $type . '">' . drupal_render($element[$type]) . '</li>';
    }
  }
  if (!empty($actions)) {
    return '<ul class="actions">' . $actions . '</ul>';
  }
  else {
    return '';
  }
}

/**
 * Theme context information for source strings.
 *
 * @param $string
 *   Source string object.
 */
function theme_l10n_community_in_context($variables) {
  $string = $variables['source'];
  if (!empty($string->context)) {
    return '<div class="string-context">' . t('in context: @context', array('@context' => $string->context)) . '</div>';
  }
  return '';
}

/**
 * Theme a suggestion/translation.
 */
function theme_l10n_community_translate_translation($variables) {
  $element = $variables['element'];
  if (!isset($element['#attributes']['class'])) {
    $element['#attributes']['class'] = array();
  }
  $element['#attributes']['class'][] = 'clearfix';
  $element['#attributes']['class'][] = 'translation';

  // Add is-selectable and is-declinable helpers for JS.
  if (!$element['active']['#disabled']) {
    $element['#attributes']['class'][] = 'is-selectable';
  }
  if (isset($element['declined'])) {
    $element['#attributes']['class'][] = 'is-declinable';
  }

  // Add information on whether this matched the filter.
  if ((int) $element['active']['#return_value'] > 0) {
    $element['#attributes']['class'][] = ($element['filters_match']['#value']) ? 'filter-match' : 'filter-no-match';
  }

  switch ($element['active']['#return_value']) {
    case 'new':
      $element['#attributes']['class'][] = 'new-translation';
      break;
    case '0':
      $element['#attributes']['class'][] = 'no-translation';
      // Fallthrough.
    default:
      if ($element['active']['#value'] !== '') {
        $element['#attributes']['class'][] = 'is-active default';
      }
  }

  $output = '<li' . drupal_attributes($element['#attributes']) . '>';
  $output .= theme('l10n_community_translate_actions', array('element' => $element));
  // Add the radio box to pick the active translation.
  $output .= drupal_render($element['active']);

  if (isset($element['author'])) {
    $output .= '<div class="author">' . drupal_render($element['author']) . '</div>';
  }

  if (isset($element['value'])) {
    $output .= drupal_render($element['value']);
  }
  return $output  . '</li>';
}

/**
 * Theme a radio button to pick the active translation.
 */
function theme_l10n_community_translate_radio($variables) {
  $element = $variables['element'];
  _form_set_class($element, array('form-radio'));
  $output = '<input type="radio" ';
  $output .= 'id="' . $element['#id'] . '" ';
  $output .= 'name="' . $element['#name'] . '" ';
  $output .= 'value="' . $element['#return_value'] . '" ';
  $output .= (check_plain($element['#value']) == $element['#return_value']) ? ' checked="checked" ' : ' ';
  $output .= drupal_attributes($element['#attributes']) . ' />';

  if (isset($element['#title'])) {
    $output .= '<label class="l10n-string" for="' . $element['#id'] . '">' . $element['#title'] . '</label>';
  }
  return $output;
}

/**
 * Theme a list of suggestions for a string.
 */
function theme_l10n_community_translate_translation_list($variables) {
  $element = $variables['element'];
  $output = '<ul>';
  foreach (element_children($element) as $child) {
    if (is_numeric($child) || $child == 'new') {
      $output .= drupal_render($element[$child]);
    }
  }
  $output .= '</ul>';
  return $output;
}

// = Translation form submission ===============================================

/**
 * Form submit callback for l10n_community_translate_form().
 *
 * @see l10n_community_translate_form().
 */
function l10n_community_translate_form_submit($form, &$form_state) {
  global $user;

  $langcode = $form_state['values']['langcode'];

  // Iterate outer structure built in l10n_community_translate_form().
  foreach ($form_state['values']['strings'] as $sid => $string) {

    // Iterate inner structure built in _l10n_community_translate_form_item().
    // Form items have numeric $tid values and other keys here.
    foreach ($string as $tid => $options) {

      // Store new suggestion.
      $empty_values = 0;
      // $options['value'] is the result of (a series of) textareas.
      if (isset($options['value']) && is_array($options['value'])) {
        foreach ($options['value'] as $key => $value) {
          if ($value === t('<New translation>')) {
            $options['value'] = '';
            $empty_values++;
          }
        }
        // If we had value in any of the textareas, add new suggestion.
        if ($tid === 'new' && count($options['value']) > $empty_values) {
          $tid = l10n_community_add_suggestion($sid, l10n_community_pack_string($options['value']), $langcode, $user->uid, $user->uid, L10N_SERVER_MEDIUM_WEB);
          if ($tid) {
            if ($string['active'] === 'new') {
              // This new string was selected to be approved, so remember $tid
              // for later, so we can save this as an approved translation.
              $string['active'] = $tid;
              l10n_community_counter(L10N_COUNT_ADDED);
            }
            else {
              l10n_community_counter(L10N_COUNT_SUGGESTED);
            }
          }
          elseif ($tid === FALSE) {
            // We found this as an active string already in the DB.
            l10n_community_counter(L10N_COUNT_DUPLICATE);
          }
        }
      }

      if (is_numeric($tid) && $tid > 0) {
        if ($tid == $string['active']) {
          if ($options['original']->is_suggestion) {
            // $tid is a suggestion that was made active.
            l10n_community_approve_string($langcode, $sid, $tid);
            l10n_community_counter(L10N_COUNT_APPROVED);
          }
        }
        elseif (!empty($options['declined'])) {
          // The decline checkbox for this suggestion was checked.
          l10n_community_counter($options['original']->is_suggestion ? L10N_COUNT_SUGGESTION_DECLINED : L10N_COUNT_DECLINED);
          l10n_community_decline_string($langcode, $sid, $tid, $user->uid);
        }
      }
    }
  }

  // Tell the user what happened.
  l10n_community_update_message();

  // Keep existing filters and other query arguments on form submission.
  $redirect_args = $form_state['values']['target'];
  unset($redirect_args['q']);
  $form_state['redirect'] = array($form_state['values']['target']['q'], array('query' => $redirect_args));
}

// = API functions =============================================================

/**
 * Get strings under some conditions.
 *
 * @param $langcode
 *   Language code to use for the lookup.
 * @param $filters
 *   - 'project'
 *     Project object to look up strings for.
 *   - 'status'
 *     Filter strings by status. See L10N_STATUS_ALL,
 *     L10N_STATUS_UNTRANSLATED, L10N_STATUS_HAS_SUGGESTION and
 *     L10N_STATUS_TRANSLATED.
 *   - 'release'
 *     Release id of the particular project release to filter with.
 *     Use NULL to not filter on releases.
 *   - 'search'
 *     Substring to search for in all source and translation strings.
 *   - 'context'
 *     From Drupal 7, separate contexts are supported. POTX_CONTEXT_NONE is
 *     the default, if the code does not specify a context otherwise.
 * @param $pager
 *   Number of strings to be returned in a pager. Should be NULL if
 *   no pager should be used.
 * @return
 *   An array of string records from database.
 */
function l10n_community_get_strings($langcode, $filters, $pager = NULL) {
  $query = db_select('l10n_server_string', 's')->distinct();
  $query->leftJoin('l10n_server_status_flag', 'ts',
    's.sid = ts.sid AND ts.language = :language', array(':language' => $langcode));
  $query->leftJoin('l10n_server_translation', 't',
    'ts.sid = t.sid AND ts.language = t.language AND t.is_active = 1');
  $query->leftJoin('users', 'u', 'u.uid = t.uid_entered');
  $query
    ->fields('s', array('sid', 'value', 'context'))
    ->fields('t', array('tid', 'language', 'translation', 'uid_entered',
        'time_entered', 'time_changed', 'is_suggestion', 'is_active'))
    ->fields('ts', array('has_suggestion', 'has_translation'));
  $query->addField('u', 'name', 'username');

  // Add sid filtering.
  if (!empty($filters['sid'])) {
    $query->condition('s.sid', $filters['sid']);
  }

  // Add submitted by condition
  if (!empty($filters['author'])) {
    $query->condition('t.uid_entered', $filters['author']->uid);
  }

  // Release restriction.
  $release = empty($filters['release']) || $filters['release'] === 'all' ? NULL : $filters['release'];
  $project = $filters['project'];
  if ($release || $project) {
    $query->innerJoin('l10n_server_line', 'l', 's.sid = l.sid');
    // If we have a release we ignore the project
    if ($release) {
      // Release restriction.
      $query->condition('l.rid', $release);
    }
    elseif ($project) {
      $query->condition('l.pid', $project->pid);
    }
  }

  // Context based filtering.
  if (isset($filters['context']) && $filters['context'] != 'all') {
    // We use 'none' for no context, so '' can be the defaut (for all contexts).
    $context = $filters['context'] == 'none' ? '' : $filters['context'];
    $query->condition('s.context', $context);
  }

  if (!empty($filters['search'])) {
    // Search in the source or target strings.
    $search = $filters['search'];
    $query->condition(db_or()
      ->condition('s.value', '%' . db_like($search) . '%', 'LIKE')
      ->condition('t.translation', '%' . db_like($search) . '%', 'LIKE'));
  }

  // Restriction based on string status by translation / suggestions.
  if (isset($filters['status'])) {
    if ($filters['status'] & L10N_STATUS_UNTRANSLATED) {
      $query->condition(db_or()
        ->condition('ts.has_translation', 0)
        ->isNull('ts.has_translation'));
    }
    elseif ($filters['status'] & L10N_STATUS_TRANSLATED) {
      $query->condition('ts.has_translation', '1');
    }
    elseif ($filters['status'] & L10N_STATUS_IS_TRANSLATION) {
      $query->condition('t.is_suggestion', '0');
    }

    if ($filters['status'] & L10N_STATUS_HAS_SUGGESTION) {
      $query->condition('ts.has_suggestion', '1');
    }
    elseif ($filters['status'] & L10N_STATUS_NO_SUGGESTION) {
      $query->condition(db_or()
        ->condition('ts.has_suggestion', 0)
        ->isNull('ts.has_suggestion'));
    }
    elseif ($filters['status'] & L10N_STATUS_IS_SUGGESTION) {
      $query->condition('t.is_suggestion', '1');
    }
  }

  // We either need a pager or a full result.
  if (isset($pager)) {
    $strings = $query->extend('PagerDefault')->limit($pager)->execute();
  }
  else {
    $strings = $query->execute();
  }
  $result = array();
  foreach ($strings as $string) {
    if ($string->is_suggestion) {
      // This string is not a translation, but we need that as a "parent" to display.
      if (!$string->has_translation) {
        // No parent translation. Pretend this does not exist.
        // The display code will call for the suggestions.
        $string->translation = '';
      }
      else {
	$q = db_select('l10n_server_translation', 't');
	$q->leftJoin('users', 'u', 'u.uid = t.uid_entered');
	$q->fields('t', array('tid', 'translation', 'uid_entered', 'time_entered', 'time_changed', 'is_suggestion', 'is_active'));
	$q->addField('u', 'name', 'username');
	$q->condition('t.language',  $string->language)
	  ->condition('t.sid', $string->sid)
	  ->condition('t.is_active', 1)
	  ->condition('t.is_suggestion', 0);
	$translation = $q->execute()->fetchAssoc();

	if (!empty($translation))
	  {
	    // It does have a translation however, so let's load it, and override.
	    foreach ($translation as $key => $value) {
	      $string->$key = $value;
	    }
	  }
      }
    }
    $result[] = $string;
  }
  return $result;
}

/**
 * Unpacks a string as retrieved from the database.
 *
 * Creates an array out of the string. If it was a single string, the array
 * will have one item. If the string was a plural string, the array will have
 * as many items as the language requires (two for source strings).
 *
 * @param $string
 *   The string with optional separation markers (NULL bytes)
 * @return
 *   An array of strings with one element for each plural form in case of
 *   a plural string, or one element in case of a regular string. This
 *  is called a $textarray elsewhere.
 */
function l10n_community_unpack_string($string) {
  return explode("\0", $string);
}

/**
 * Packs a string for storage in the database.
 *
 * @param $string
 *   An array of strings.
 * @return
 *   A packed string with NULL bytes separating each string.
 */
function l10n_community_pack_string($strings) {
  return implode("\0", $strings);
}

// = AJAX callbacks ============================================================

/**
 * Generate list of projects and releases of where a string appears.
 *
 * We could provide much more information (down to line numbers of files), but
 * usability should also be kept in mind. It is possible to investigate hidden
 * information sources though, like tooltips on the release titles presented.
 *
 * This callback is invoked from JavaScript and is used as an AJAX provider.
 *
 * @param $sid
 *   Source string id.
 * @param $ajax
 *   Whether the request came through AJAX, in which case no page
 *   theming should be applied. Non-AJAX support is a fallback if JS is
 *   turned off.
 */
function l10n_community_string_details($sid = 0, $ajax = 0) {

  $output = '';
  if ($ajax) {
    // Prevent devel module information if doing AJAX.
    $GLOBALS['devel_shutdown'] = FALSE;
  }
  else {
    // Get data about this source string.
    $string = db_query(
      'SELECT value, context FROM {l10n_server_string} WHERE sid = :sid',
      array(':sid' => $sid)
    )->fetchObject();
    $unpacked = l10n_community_unpack_string($string->value);
    $output .= '<h3>' . t('Source string') . '</h3>' . theme('item_list', array('items' => $unpacked));
    $output .= '<h3>' . (empty($string->context) ? t('Used at the following places') : t('Used at the following places in context %context', array('%context' => $string->context))) . '</h3>';
  }

  // List of project releases, where this string is used.
  $result = db_query('SELECT l.pid, p.title project_title, l.rid, r.title release_title, COUNT(l.lineno) as occurrence_count FROM {l10n_server_line} l INNER JOIN {l10n_server_project} p ON l.pid = p.pid INNER JOIN {l10n_server_release} r ON l.rid = r.rid WHERE l.sid = :sid AND p.status = 1 GROUP BY l.rid ORDER BY l.pid, l.rid', array(':sid' => $sid));

  $version_list = array();
  $project_list = array();
  $previous_project = '';
  foreach ($result as $instance) {
    $release_info = $instance->release_title . ' <span title="' . format_plural($instance->occurrence_count, 'Appears once in this release.', 'Appears @count times in this release.') . '">(' . $instance->occurrence_count . ')</span>';
    if ($instance->project_title != $previous_project) {
      if (!empty($version_list)) {
        $project_list[] = join(', ', $version_list);
      }
      $version_list = array('<em>' . $instance->project_title . ':</em> ' . $release_info);
    }
    else {
      $version_list[] = $release_info;
    }
    $previous_project = $instance->project_title;
  }
  $project_list[] = join(', ', $version_list);
  $usage_list = theme('item_list', array('items' => $project_list));
  if ($ajax) {
    print $usage_list;
    exit;
  }
  else {
    $output .= $usage_list;
    return $output;
  }
}

/**
 * Provide full history information about translation.
 *
 * @param $tid
 *   Translation id.
 * @param $ajax
 *   Whether the request came through AJAX, in which case no page
 *   theming should be applied. Non-AJAX support is a fallback if JS is
 *   turned off.
 */
function l10n_community_translation_details($tid = 0, $ajax = 0) {

  // Get data about this translation.
  $string = db_query(
    'SELECT * FROM {l10n_server_translation} WHERE tid = :tid',
    array(':tid' => $tid)
  )->fetchObject();

  $output = '';
  if ($ajax) {
    // Prevent devel module information if doing AJAX.
    $GLOBALS['devel_shutdown'] = FALSE;
  }
  else {
    $unpacked = l10n_community_unpack_string($string->translation);
    $output .= '<h3>' . t('Translation') . '</h3>' . theme('item_list', array('items' => $unpacked)) . '<h3>' . t('History') . '</h3>';
  }

  // List of project releases, where this string is used.
  $result = db_query('SELECT h.*, u.name FROM {l10n_server_translation_history} h INNER JOIN {users} u ON h.uid_action = u.uid WHERE tid = :tid ORDER BY time_action DESC, type_action DESC', array(':tid' => $tid));

  $uid_submitted = 0;
  $history_list = array();
  foreach ($result as $item) {
    $history_list[] = l10n_community_translate_byline($item->name, $item->uid_action, $item->time_action, $item->medium_action, $item->type_action);
    if ($item->type_action == L10N_SERVER_ACTION_ADD) {
      // Remember the first uid who submitted this. This will get overwritten, and since we are going backwards in time,
      // the last value kept will be the first uid who submitted this string. We need this in case it is different to
      // the user we used for attribution, so we can display the difference to the translator/moderator.
      $uid_submitted = $item->uid_action;
    }
  }
  if (!empty($uid_submitted) && ($uid_submitted != $string->uid_entered)) {
    // Turns out the user used for attribution is different to the user used for
    // the string storage.
    // TODO Convert "user_load" to "user_load_multiple" if "$string->uid_entered" is other than a uid.
    // To return a single user object, wrap "user_load_multiple" with "array_shift" or equivalent.
    // Example: array_shift(user_load_multiple(array(), $string->uid_entered))
    $account = user_load($string->uid_entered);
    $author = theme('username', array('account' => $account));
    array_unshift($history_list, t('by !author', array('!author' => $author)));
  }
  if (count($history_list)) {
    $output .= theme('item_list', array('items' => $history_list));
  }
  if ($ajax) {
    print $output;
    exit;
  }
  else {
    return $output;
  }
}
