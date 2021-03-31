<?php

/**
 * @file
 * biblio.admin.inc
 *
 *   Handles administrative forms and tasks.
 */

/**
 * Implementation of hook_settings().
 */
function biblio_admin_settings() {
  $version = '$Name$ $Date$';
  $version = str_replace('$', '', $version);

  $form['biblio_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 0,
  );

  $form['general'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('General'),
    '#group' => 'biblio_settings',
    '#weight' => 0,
    '#description' => '',
  );

  $form['general']['biblio_rev'] = array(
    '#markup' => $version,

  );

  $form['general']['biblio_base'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL'),
    '#size' => 20,
    '#default_value' => variable_get('biblio_base', 'biblio'),
    '#description' => t('This sets the base URL used to access the biblio module (e.g. /biblio ).'),
  );
  $form['#biblio_base'] = $form['general']['biblio_base']['#default_value'];
  $form['general']['biblio_base_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Biblio page title'),
    '#size' => 20,
    '#default_value' => variable_get('biblio_base_title', 'Biblio'),
    '#description' => t('The page title shown on the base URL.'),
  );

  $form['general']['biblio_rowsperpage'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of results per page'),
    '#default_value' => variable_get('biblio_rowsperpage', 25),
    '#size' => 6,
    '#maxlength' => 6,
    '#description' => t('This sets the number of results that will be displayed per page.'),
  );
  $form['general']['biblio_view_only_own'] = array(
    '#type' => 'checkbox',
    '#title' => t('Restrict users such that they can only view their own biblio entries'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_view_only_own', 0),
    '#description' => t('This option restricts the users capability to view biblio entries.  They will only be able to see the entries which they have created and own.'),
  );
  $form['general']['biblio_button_hide'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide next button in node form'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_button_hide', 1),
    '#description' => t('If checkbox is set a javascript adds the accesible system class "element-invisible" to the next button in node form. The next button is not needed if javascript is available and listening on changes.'),
  );
  $form['authors'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Authors'),
    '#group' => 'biblio_settings',
    '#weight' => 10,
    '#description' => '',
  );
  $form['authors']['biblio_auto_orphaned_author_delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically delete orphaned authors'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_auto_orphaned_author_delete', 0),
    '#description' => t('Orphaned authors are those which are no longer linked to any entries as the result of a biblio update or delete. This requires a functioning "cron" process.'),
  );
  $form['authors']['biblio_orphan_clean_interval'] = array(
    '#type' => 'radios',
    '#title' => t('Orphaned author cleaning frequency'),
    '#default_value' => variable_get('biblio_orphan_clean_interval', 24 * 60 * 60),
    '#options' => array(
      0 => t('Every CRON run'),
      3600 => t('Hourly'),
      86400 => t('Daily'),
      604800 => t('Weeekly'),
    ),
    '#description' => t('How frequently should we check for and delete orphans.'),
    '#states' => array(
      'invisible' => array(
        'input[name="biblio_auto_orphaned_author_delete"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['authors']['biblio_init_auth_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of contributor fields to initially display on the input form'),
    '#size' => 2,
    '#maxlength' => 2,
    '#default_value' => variable_get('biblio_init_auth_count', 4),
    '#description' => t('Increasing this value will increase the number of input fields displayed in the contributors section of the input form'),
  );
  $form['authors']['biblio_contrib_fields_delta'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of fields added by the "Add more" button on the contributor input form'),
    '#size' => 2,
    '#maxlength' => 2,
    '#default_value' => variable_get('biblio_contrib_fields_delta', 2),
    '#description' => t('This number of blank fields will be added to the contributors section of the input form each time the "Add more" button is pressed.'),
  );
  $form['bibtex'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('BibTeX settings'),
    '#group' => 'biblio_settings',
    '#weight' => 20,
    '#description' => '',
  );
  $form['bibtex']['biblio_hide_bibtex_braces'] = array(
    '#type' => 'checkbox',
    '#title' => t('Retain bibtex\'s {Protected} capitalization in the title string, but hide the braces on display'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_hide_bibtex_braces', 0),
    '#description' => '',
  );
  $form['bibtex']['biblio_remove_double_bibtex_braces'] = array(
    '#type' => 'checkbox',
    '#title' => t('Configure the BibTeX import parser to replace all double braces with single braces.'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_remove_double_bibtex_braces', 0),
    '#description' => t('<strong>{{</strong> will be replaced with <strong>{</strong> and <strong>}}</strong> will be replaced with <strong>}</strong>. This provides compatibility with importing BibTeX from the <a href="http://thomsonreuters.com/web-of-science/">Web of Science</a>. Be aware that this removes support for BibTeX "Protected Capitalization".'),
  );

  $result = db_query("SELECT b.name, bftd.title FROM {biblio_fields} b
                      INNER JOIN {biblio_field_type} bt ON bt.fid=b.fid
                      INNER JOIN {biblio_field_type_data} bftd ON bftd.ftdid=bt.ftdid
                      WHERE bt.tid=0 ORDER by bftd.title ASC ");
  $schema = drupal_get_schema('biblio');
  $keys = array_keys($schema['fields']);
  $options = array();
  $options['nid'] = t('Node ID');
  foreach ($result as $row) {
    $options[$row->name] = $row->title;
  }
  $form['citekey'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Citekey'),
    '#description' => t('You can alter citekey related settings here.'),
    '#group' => 'biblio_settings',
    '#weight' => 30,

  );
  $form['citekey']['biblio_display_citation_key'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show citation key in results'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_display_citation_key', 0),
    '#description' => t('This will output the citekey as the first item in the citation string'),
  );
  $form['citekey']['biblio_auto_citekey'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto generate citekeys if not given'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_auto_citekey', 1),
    '#description' => t('This option will cause "citekey" entries to be automatically generated if a value is not provided.'),
  );
  $form['citekey']['biblio_citekey_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Citekey prefix'),
    '#default_value' => variable_get('biblio_citekey_prefix', ''),
    '#size' => 10,
    '#maxlength' => 10,
    '#description' => t('This text will be combined with the field choosen below to form the auto generated citekey.'),
  );
  $form['citekey']['biblio_citekey_field1'] = array(
    '#type' => 'select',
    '#title' => t('Primary Citekey  field'),
    '#default_value' => variable_get('biblio_citekey_field1', 'nid'),
    '#options' => $options,
    '#description' => t('Select the field to be used when generating citekeys.'),
  );
  $form['citekey']['biblio_citekey_field2'] = array(
    '#type' => 'select',
    '#title' => t('Secondary Citekey field'),
    '#default_value' => variable_get('biblio_citekey_field2', 'nid'),
    '#options' => $options,
    '#description' => t('If the field above has no value this field will be used.'),
  );
  // Allow only users to modify PHP code which have PHP block visibility permissions.
  if (user_access('use PHP for block visibility')) {
    $form['citekey']['biblio_citekey_phpcode'] = array(
      '#type' => 'textarea',
      '#title' => t('PHP code for citekey generation'),
      '#default_value' => variable_get('biblio_citekey_phpcode', ''),
      '#description' => t('Advanced usage only: PHP code that returns the citekey. Should not include &lt;?php ?&gt; delimiters.'),
    );
  }

  $form['biblio_crossref'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('CrossRef Login Information'),
    '#group' => 'biblio_settings',
    '#weight' => 40,
  );
  $link_attrs = array(
    'attributes' => array('target' => '_blank'),
    'absolue' => TRUE,
  );
  $form['biblio_crossref']['biblio_show_crossref_profile_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow users to override these settings on their "My account" page') ,
    '#return_value' => 1,
    '#description' => t('If this is selected, a form similar to this section will be available to the user when they edit their own account information.  This will allow them to override the global preferences set here.') ,
    '#default_value' => variable_get('biblio_show_crossref_profile_form', '1'),
  );
  $form['biblio_crossref']['biblio_crossref_pid'] = array(
    '#type' => 'textfield',
    '#title' => t('CrossRef OpenURL Account ID'),
    '#default_value' => variable_get('biblio_crossref_pid', ''),
    '#description' => t('Enter your complimentary CrossRef OpenURL account ID which you can obtain here !url, OR enter your full CrossRef (colon separated) account:password combination.', array('!url' => l(t('OpenURL Account Request Form'), 'http://www.crossref.org/requestaccount/', $link_attrs))),
  );

  $form['footnotes'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Footnotes'),
    '#description' => t('You can integrate with the !url module here.', array('!url' => l('footnotes', url("http://www.drupal.org/project/footnotes", array('query' => NULL, 'fragment' => NULL, 'absolute' => TRUE))))),
    '#group' => 'biblio_settings',
    '#weight' => 50,
  );
  if (!module_exists('footnotes')) {
    $additional_text = '<div class="admin-dependencies">' . t('Depends on') . ': ' . t('Footnotes') . ' (<span class="admin-disabled">' . t('disabled') . '</span>)</div>';
    $disabled = TRUE;
    variable_set('biblio_footnotes_integration', 0);
  }
  else {
    $additional_text = '<div class="admin-dependencies">' . t('Depends on') . ': ' . t('Footnotes') . ' (<span class="admin-enabled">' . t('enabled') . '</span>)</div>';
    $disabled = FALSE;
  }
  $form['footnotes']['biblio_footnotes_integration'] = array(
    '#type' => 'checkbox',
    '#title' => t('Integration with the footnotes module') . $additional_text,
    '#disabled' => $disabled,
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_footnotes_integration', 0),
    '#description' => t('This will convert &lt;bib&gt; tags into &lt;fn&gt; tags.  This will cause intermingled &lt;bib&gt; and &lt;fn&gt; tags to be sequentially numbered.  For this to work, you must put the &lt;bib&gt; filter ahead of the &lt;fn&gt; filter in the filter chain.  If this option is not set, &lt;bib&gt; and &lt;fn&gt; tags will be handled separately.'),
  );
  $form['isi_wok'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('ISI Web of Knowledge'),
    '#description' => '',
    '#group' => 'biblio_settings',
    '#weight' => 60,
  );
  $form['isi_wok']['biblio_fix_isi_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically replace "Go to ISI" links with the  URL below'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_fix_isi_links', 0),
    '#description' => t('This option automatically replaces any fake "Go to ISI" links with the supplied URL to ISI Web of Knowledge.  Note a subscription with ISI is required for these links to function.'),
  );
  $form['isi_wok']['biblio_isi_url'] = array(
    '#type' => 'textfield',
    '#title' => t('ISI Web of Knowledge URL'),
    '#size' => 128,
    '#maxlength' => 512,
    '#default_value' => variable_get('biblio_isi_url', 'http://apps.isiknowledge.com/InboundService.do?Func=Frame&product=WOS&action=retrieve&SrcApp=EndNote&Init=Yes&SrcAuth=ResearchSoft&mode=FullRecord&UT='),
    '#description' => t('Enter the URL which will replace the "Go to ISI" fake links imported from EndNote'),
  );
  $form['keywords'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Keywords'),
    '#description' => '',
    '#group' => 'biblio_settings',
    '#weight' => 70,
  );
  $form['keywords']['biblio_keyword_sep'] = array(
    '#type' => 'textfield',
    '#title' => t('Keyword separator'),
    '#size' => 2,
    '#default_value' => variable_get('biblio_keyword_sep', ','),
    '#description' => t('Enter the character which will be used to separate multiple keywords in the keyword field'),
  );
  $form['keywords']['biblio_keyword_orphan_autoclean'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically remove orphaned keywords'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_keyword_orphan_autoclean', 1),
    '#description' => t('This option automatically deletes keywords which are no longer associated with any publications (primarily due to the due to the removal of a publication or editing a keyword).'),
  );
  $taxo_mesg = '<div class="admin-dependencies">' . t('Depends on') . ': ' . t('Taxonomy') . ' (<span class="admin-disabled">' . t('disabled') . '</span>)</div>';
  $form['keywords']['biblio_copy_taxo_terms_to_keywords'] = array(
    '#type' => 'checkbox',
    '#title' => t('Copy any selected taxonomy terms to the biblio keyword database'),
    '#return_value' => 1,
    '#default_value' => module_exists('taxonomy') ? variable_get('biblio_copy_taxo_terms_to_keywords', 0) : 0,
    '#disabled' => (!module_exists('taxonomy')),
    '#description' => module_exists('taxonomy') ? t('If this option is selected, the selected taxonomy terms will be copied to the @base_title keyword database and be displayed as keywords (as well as taxonomy terms) for this entry.', array('@base_title', variable_get('biblio_base_title', 'Biblio'))) : $taxo_mesg,
  );
  $form['keywords']['biblio_keyword_freetagging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use keywords from biblio entries as taxonomy "free tags"'),
    '#return_value' => 1,
    '#default_value' => module_exists('taxonomy') ? variable_get('biblio_keyword_freetagging', 0) : 0,
    '#disabled' => (!module_exists('taxonomy')),
    '#description' => module_exists('taxonomy') ? t('This option allows user to add keywords (free tags) to describe their documents. These keywords will be registered as taxonomy.') : $taxo_mesg,
  );
  $vocabularies = module_invoke('taxonomy', 'get_vocabularies');
  // ... and print a form to select the terms in each of them.
  $taxo_options = array();
  $taxo_options[0] = '<' . t('none') . '>';
  if (count($vocabularies)) {
    foreach ($vocabularies as $voc) {
      $taxo_options[$voc->vid] = $voc->name;
    }
    $form['keywords']['biblio_keyword_vocabulary'] = array(
      '#type' => 'select',
      '#title' => t('Vocabulary'),
      '#default_value' => variable_get('biblio_keyword_vocabulary', 0),
      '#options' => $taxo_options,
      '#description' => t('Select vocabulary (category) to use for free tags.'),
      '#multiple' => FALSE,
    // '#disabled' => (!variable_get('biblio_keyword_freetagging', 0)),
    //    '#size' => $multiple ? min(9, count($taxo_options)) : 0,.
      '#weight' => 15,
      '#states' => array(
        'invisible' => array(
          'input[name="biblio_keyword_freetagging"]' => array('checked' => FALSE),
        ),
      ),
    );
  }

  $form['links'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Links'),
    '#group' => 'biblio_settings',
    '#weight' => 80,
  );
  $form['links']['export'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Export Links'),
  );
  $options = array();
  $export_defaults = array();
  $options = module_invoke_all('biblio_export_options');
  if (!empty($options)) {
    $export_defaults = array_combine(array_keys($options), array_keys($options));
  }
  $form['links']['export']['biblio_export_links'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show export links'),
    '#default_value' => array_merge($export_defaults, variable_get('biblio_export_links', $export_defaults)),
    '#options' => $options,
    '#description' => t('You can select which export links to display here.'),
  );
  $form['links']['file_attachments'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('File Attachments'),
  );
  $form['links']['file_attachments']['biblio_file_link_type'] = array(
    '#type' => 'radios',
    '#title' => t('File attachment display'),
    '#default_value' => variable_get('biblio_file_link_type', 'text'),
    '#options' => array(
      'text' => t('Text'),
      'icon' => t('Icon'),
    ),
    '#description' => t('Display file attachments as either the file name or an icon'),
  );

  $form['links']['google'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Lookup Links'),
  );

  $options = module_invoke_all('biblio_lookup_link_settings');
  $options += array('google' => 'Google Scholar');
  $lookup_defaults = array_combine(array_keys($options), array_keys($options));

  $form['links']['google']['biblio_lookup_links'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show lookup links'),
    '#default_value' => array_merge($lookup_defaults, variable_get('biblio_lookup_links', $lookup_defaults)),
    '#options' => $options,
    '#description' => t('You can select which lookup links to display here.'),
  );

  $form['links']['biblio_download_links_to_node'] = array(
    '#type' => 'checkbox',
    '#title' => t('Download links in "List" view link to "Node" view'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_download_links_to_node', 0),
    '#description' => t('If selected, the download links in the "List" view will link to the full "Node" view rather than the file itself, the file can then be downloaded from the node view'),
  );
  $form['links']['biblio_links_target_new_window'] = array(
    '#type' => 'checkbox',
    '#title' => t('Links open in new browser'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_links_target_new_window', 0),
    '#description' => t('This causes related URLs to open in a new browser window'),
  );

  $form['links']['biblio_link_title_url'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hyperlink titles using supplied URL if available'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_link_title_url', 0),
    '#description' => t('Selecting this links the titles to the supplied URL (if available) rather than the "node" view.'),
  );
  $form['links']['author'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Author Links'),
  );
  $form['links']['author']['biblio_author_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hyperlink author names'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_author_links', 1),
    '#description' => t('This creates a hyperlink on author names which, when clicked, will select entries which contain that author'),
  );
  $form['links']['author']['biblio_author_link_profile'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hyperlink author names to author profile page'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_author_link_profile', 0),
    '#description' => t('This creates a hyperlink on author names which, when clicked, will take the user to the authors profile page'),
  );
  $form['links']['author']['biblio_author_link_profile_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to profile page'),
    '#default_value' => variable_get('biblio_author_link_profile_path', 'user/[user:uid]'),
    '#description' => t('Do not include a leading "/"'),
  );

  $form['links']['author']['token_tree'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('user'),
  );

  $form['openurl'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('OpenURL'),
    '#description' => t('You can set an <a href="http://en.wikipedia.org/wiki/OpenURL">openurl</a> link here'),
    '#group' => 'biblio_settings',
    '#weight' => 90,
  );
  $form['openurl']['biblio_show_openurl_profile_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow users to override these settings on their "My account" page') ,
    '#return_value' => 1,
    '#description' => t('If this is selected, a form similar to this section will be available to the user when they edit their own account information.  This will allow them to override the global preferences set here.') ,
    '#default_value' => variable_get('biblio_show_openurl_profile_form', '1'),
  );

  $form['openurl']['biblio_baseopenurl'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenURL Base URL'),
    '#size' => 95,
    '#default_value' => variable_get('biblio_baseopenurl', ''),
    '#description' => t('This sets your institution\'s base <a href="http://en.wikipedia.org/wiki/OpenURL">OpenURL</a> gateway, which is used to generate OpenURL links. To implement a "Universal" OpenURL system, try using OCLC\'s <a href="http://www.oclc.org/productworks/urlresolver.htm">OpenURL Resolver Registry</a> gateway: <a href="http://worldcatlibraries.org/registry/gateway">http://worldcatlibraries.org/registry/gateway</a>'),
  );
  $sid = "Biblio:" . variable_get('site_name', 'Drupal');
  $form['openurl']['biblio_openurl_sid'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenURL Site ID'),
    '#size' => 95,
    '#default_value' => variable_get('biblio_openurl_sid', $sid),
    '#description' => t('This sets your institution\'s site name, some link resolvers will require a specific Site ID in order to process your requests.'),
  );
  $form['openurl']['biblio_openurlimage'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenURL Image'),
    '#size' => 95,
    '#default_value' => variable_get('biblio_openurlimage', ''),
    '#description' => t('Enter a path to your image here, this image will be used as button which when clicked will find the entry via the OpenURL link'),
  );
  // Add profile page settings... this is done in a fucntion so it can be reused elsewhere.
  $form['biblio_profile'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Profile pages'),
    '#group' => 'biblio_settings',
    '#weight' => 100,
  );
  $form['biblio_profile'] += _biblio_get_user_profile_form();
  if (!module_exists('search')) {
    $search_text = '<div class="admin-dependencies">' . t('Depends on') . ': ' . t('Search') . ' (<span class="admin-disabled">' . t('disabled') . '</span>)</div>';
    $search_disabled = TRUE;
  }
  else {
    $search_text = '<div class="admin-dependencies">' . t('Depends on') . ': ' . t('Search') . ' (<span class="admin-enabled">' . t('enabled') . '</span>)</div>';
    $search_disabled = FALSE;
  }
  $form['search'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => $search_text,
    '#title' => t('Search'),
    '#group' => 'biblio_settings',
    '#weight' => 110,
  );
  $form['search']['biblio_search'] = array(
    '#type' => 'checkbox',
    '#disabled' => $search_disabled,
    '#title' => t('Enable a search box on the biblio page.'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_search', 0),
    '#description' => t('Shows a search box on the biblio page that returns drupal search results in the biblio style.'),
  );
  $form['search']['biblio_search_button_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Search button text'),
    '#disabled' => $search_disabled,
    '#size' => 95,
    '#default_value' => variable_get('biblio_search_button_text', t('Biblio search')),
    '#description' => t('This allows you to customize the text on the search button, it defaults to "Biblio search".'),
  );
  $form['search']['biblio_index'] = array(
    '#type' => 'checkbox',
    '#disabled' => $search_disabled,
    '#title' => t('Re-/Index a biblio node when creating or updating.'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_index', 0),
    '#description' => t('A biblio node must be indexed for the drupal search to know its content. You need to check this option if you want to search for a biblio node that you just created or updated. Otherwise you must wait for the cron job to reindex nodes.'),
  );
  $form['search']['biblio_search_form_filter'] = array(
    '#type' => 'checkbox',
    '#disabled' => $search_disabled,
    '#title' => t('Allow search filters on search form.'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_search_form_filter', 1),
    '#description' => t('Unchecking this box will remove search filters from the search form, but may improve performance on biblio search pages.'),
  );
  $form['sort'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Sorting'),
    '#description' => t('You can set the default sorting and ordering for the /biblio page here.'),
    '#group' => 'biblio_settings',
    '#weight' => 120,
  );
  $form['sort']['biblio_sort'] = array(
    '#type' => 'select',
    '#title' => t('Sort by'),
    '#default_value' => variable_get('biblio_sort', 'year'),
    '#options' => array(
      'author'  => t('Author'),
      'keyword' => t('Keyword'),
      'title'   => t('Title'),
      'type'    => t('Type'),
      'year'    => t('Year'),
    ),
    '#description' => t('This is the initial default sort.'),
  );
  $stop_words = 'a,an,is,on,the';
  $form['sort']['biblio_stop_words'] = array(
    '#type' => 'textfield',
    '#title' => t('Words to remove from the beginning of titles prior to sorting'),
    '#size' => 60,
    '#default_value' => variable_get('biblio_stop_words', $stop_words),
    '#description' => t('A comma separated list of (case insensitive) words to strip from the title for sorting purposes.  NOTE: quotation and punctuation are stripped automatically.'),
  );

  $form['sort']['biblio_sort_tabs'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show sort links'),
    '#default_value' => variable_get('biblio_sort_tabs', array('author', 'title', 'type', 'year')),
    '#options' => array(
      'author'  => t('Author'),
      'keyword' => t('Keyword  (<i>Warning: sorting by keyword may produce many duplicates due to the fact that an entry is listed for each keyword attached to it.</i>)'),
      'title'   => t('Title'),
      'type'    => t('Type'),
      'year'    => t('Year'),
    ),
    '#description' => t('You turn the sorting links at the top of the /biblio page here.'),
  );
  $form['sort']['biblio_sort_tabs_style'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show sort links as "tabs"'),
    '#default_value' => variable_get('biblio_sort_tabs_style', 0),
    '#return_value' => 1,
    '#description' => t('This changes the sort links from text links to tabs'),
  );
  $form['sort']['biblio_order'] = array(
    '#type' => 'radios',
    '#title' => t('Order'),
    '#default_value' => variable_get('biblio_order', 'DESC'),
    '#options' => array(
      'DESC' => t('Descending'),
      'ASC' => t('Ascending'),
    ),
  );
  $form['style'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Styling'),
    '#description' => t('You can set the default style for the /biblio page here.'),
    '#group' => 'biblio_settings',
    '#weight' => 130,
  );
  $form['style']['biblio_no_year_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Text to display if no year of publication is available'),
    '#size' => 95,
    '#default_value' => variable_get('biblio_no_year_text', t('Submitted')),
    '#description' => t('The text that is displayed when no date of publication is given or it is deliberately set to <b>9999</b>, it defaults to "Submitted".'),
  );
  $form['style']['biblio_inpress_year_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Text to display if year of publication is set to 9998'),
    '#size' => 95,
    '#default_value' => variable_get('biblio_inpress_year_text', t('In Press')),
    '#description' => t('The text that is displayed when the date of publication is deliberately set to <b>9998</b>, it defaults to "In Press".'),
  );
  if (module_exists('biblio_citeproc')) {
    $form['style']['biblio_citeproc_style'] = array(
      '#type' => 'select',
      '#title' => t('Style'),
      '#default_value' => variable_get('biblio_citeproc_style', 'ieee.csl'),
      '#options' => biblio_get_styles(),
      '#description' => t('Set the bibliographic style of the "list" view.'),
    );
  }
  else {
    $form['style']['biblio_style'] = array(
      '#type' => 'select',
      '#title' => t('Style'),
      '#default_value' => variable_get('biblio_style', 'cse'),
      '#options' => biblio_get_styles(),
      '#description' => t('Set the bibliographic style of the "list" view.'),
    );
  }
  $form['style']['biblio_node_layout'] = array(
    '#type' => 'radios',
    '#title' => t('Node Layout'),
    '#default_value' => variable_get('biblio_node_layout', 'tabular'),
    '#options' => array(
      'orig'    => t('Original'),
      'ft'      => t('Only Fulltext if available'),
      'tabular' => t('Tabular'),
      'cite'    => t('Bibliographic style choosen above'),
    ),
    '#description' => t('This alters the layout of the "node" (full) view.'),
  );
  $form['style']['biblio_annotations'] = array(
    '#type' => 'select',
    '#title' => t('Annotations'),
    '#default_value' => variable_get('biblio_annotations', 'none'),
    '#options' => array(
      'none'           => t('none'),
      'biblio_notes'   => t('notes'),
      'biblio_custom1' => t('custom1'),
      'biblio_custom2' => t('custom2'),
      'biblio_custom3' => t('custom3'),
      'biblio_custom4' => t('custom4'),
      'biblio_custom5' => t('custom5'),
      'biblio_custom6' => t('custom6'),
      'biblio_custom7' => t('custom7'),
    ),
    '#description' => t('Select a field from which an annotation will be displayed below biblo entry in "short" listings'),
    '#multiple' => FALSE,
    '#size' => 0,
  );
  $form['syndication'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Syndication'),
    '#description' => t('You can set the RSS defaults here.'),
    '#group' => 'biblio_settings',
    '#weight' => 140,
  );
  $form['syndication']['biblio_rss'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow RSS feeds of new biblio entries'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_rss', 0),
    '#description' => t('This will create an rss feed of the 10 most recent biblio entries. It will be available at /biblio/rss.xml'),
  );
  $form['syndication']['biblio_rss_number_of_entries'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of items in the RSS feed.'),
    '#default_value' => variable_get('biblio_rss_number_of_entries', 10),
    '#size' => 6,
    '#maxlength' => 6,
    '#description' => t('Limits the number of items in the /biblio/rss.xml feed to this number.'),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'biblio_admin_settings_form_submit';
  // Our submit handler is added after the call to system settings form so that it gets
  // called after system_settings_form_submit, and thus the variables have been stored already
  // and the menu will be rebuilt correctly.
  return ($form);
}

/**
 * Form handler for biblio_admin_settings.
 */
function biblio_admin_settings_form_submit($form, &$form_state) {
  // If ($form_state['values']['biblio_keyword_freetagging'] && $form_state['values']['biblio_keyword_vocabulary']) {
  //    if ($vocabulary = taxonomy_vocabulary_load(variable_get('biblio_keyword_vocabulary', 0))) {
  //      $vocabulary = (array) $vocabulary;
  //      $vocabulary['nodes']['biblio'] = 1;
  //      taxonomy_save_vocabulary($vocabulary);
  //    }
  //  }
  if (($form['#biblio_base'] != $form_state['values']['biblio_base']) ||
      ($form['biblio_profile']['#biblio_show_profile'] != $form_state['values']['biblio_show_profile']) ||
      ($form['biblio_profile']['#biblio_my_pubs_menu'] != $form_state['values']['biblio_my_pubs_menu'])) {
    menu_rebuild();
  }
}

/**
 * Form constructor for the Publication type edit form.
 *
 * @param int $tid
 *   The publication type.
 *
 * @return array $form
 */
function biblio_admin_types_edit_form($form, &$form_state, $tid = 0) {
  // $form['#theme'] = 'biblio_admin_types_edit_form';
  // $form['#tree'] = TRUE;.
  $form['#cache'] = TRUE;
  $tid = (isset($form_state['pub_type']) ? $form_state['pub_type'] : (int) $tid);
  $msg = '<div>' . t('On this page you can set type specific "Titles" and "Hints" which will display on the input form.');
  if ($tid) {
    $msg .= ' ' . t('Checking the "Visible" box will add the field to the input form, checking "Required" will force the user to supply a value for this field and the weight value changes the order which it is rendered on the form with smaller values floating to the top of the form.
    <p> Fields which are grayed out on this page have been set to "common" on the !linktobiblioadmin page.</p>', array('!linktobiblioadmin' => l("admin/config/content/biblio/fields", "admin/config/content/biblio/fields")));

  }
  else {
    $msg .= ' ' . t('Checking the "Common" box will add the field to all the different publication types. Checking "Required" will force the user to supply a value for the field, checking "Autocomplete" will enable AJAX type auto complete look up for the field when the user is entering data and the weight value changes the order which it is rendered on the form with smaller values floating to the top of the form.');
  }
  $msg .= t('Finally, for each author field you can choose a set of author roles. Assigning different roles to authors within the same field, e.g. primary and secondary authors within the authors field, allows to theme them differently.');
  $msg .= '</div>';

  $form['#redirect'] = 'admin/config/content/biblio/fields';
  $form['help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Help'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['help']['message'] = array(
    '#markup' => $msg,
  );
  // Get author types.
  $result = db_query("SELECT * FROM {biblio_contributor_type_data} b");
  $contrib_options = array();
  foreach ($result as $contrib_type) {
    $contrib_options[$contrib_type->auth_type] = $contrib_type->title;
    $contrib_hints[$contrib_type->auth_type] = $contrib_type->hint;
  }

  // First get all of the field info.
  if ($tid) {
    $result = db_query("SELECT bf.*, bftd.*,  bft.vtab, bft.common, bft.visible, bft.autocomplete, bft.weight, bft.required, bt.name AS type_name
                        FROM {biblio_fields} AS bf
                        INNER JOIN {biblio_field_type} AS bft ON bft.fid=bf.fid
                        INNER JOIN {biblio_field_type_data} AS bftd ON bftd.ftdid=bft.ftdid
                        INNER JOIN {biblio_types} AS bt ON bt.tid=bft.tid
                        WHERE bft.tid=:tid", array(':tid' => $tid), array('fetch' => PDO::FETCH_ASSOC));
  }
  else {
    $result = db_query("SELECT bf.*, bftd.*, bft.vtab, bft.common, bft.visible, bft.autocomplete, bft.weight, bft.required
                        FROM {biblio_fields} AS bf
                        INNER JOIN {biblio_field_type} AS bft ON bft.fid=bf.fid
                        INNER JOIN {biblio_field_type_data} AS bftd ON bftd.ftdid=bft.ftdid
                        WHERE bft.tid=:tid", array(':tid' => $tid), array('fetch' => PDO::FETCH_ASSOC));
  }

  foreach ($result as $row) {
    $fields[$row['fid']] = $row;
  }

  $types = db_query("SELECT * from {biblio_types} WHERE visible=1 ORDER BY name ASC");
  $types_options[0] = t('Common');
  foreach ($types as $type) {
    $types_options[$type->tid] = $type->name;
  }

  $form['pub_type'] = array(
    '#title' => t('Publication type'),
    '#type' => 'select',
    '#options' => $types_options,
    '#attributes' => array('onchange' => 'document.getElementById(\'edit-change-type\').click()'),
    '#default_value' => $tid,
  );

  $no_js = (!isset($_COOKIE['has_js']) || empty($_COOKIE['has_js']));

  $form['change_type'] = array(
    '#type'   => 'submit',
    '#value'  => t('Change Publication Type'),
    '#weight' => -10,
    '#prefix' => $no_js ? '' : '<div style="display:none;">',
    '#suffix' => $no_js ? '' : '</div>',
  );

  $form['biblio_tabs'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 0,
  );

  $form['biblio_tabs'] += biblio_node_form_vtabs();

  // $form['configured_flds'] = array('#tree' => 1);
  // show an existing type.
  if ($tid) {
    $typename = $fields[1]['type_name'];
    $form['type_name'] = array(
      '#title' => t('Publication type name'),
      '#type' => 'textfield',
      '#maxlength' => 64,
      '#default_value' => $typename,
      '#required' => TRUE,
    );
    $form['top_message'] = array(
      '#value' => t('Field settings related to @type publications', array('@type' => $typename)),
    );
    $form['type_id'] = array(
      '#type' => 'value',
      '#title' => 'tid',
      '#value' => $tid,
    );
  }
  else {
    $form['top_message'] = array(
      '#value' => t('Field settings common to all publication types'),
    );

  }
  // Resort the fields since the weight may have changed.
  uasort($fields, "biblio_form_sort");
  $vis_comm = $tid ? 'visible' : 'common';
  $options["$vis_comm"] = '';
  $options['required'] = '';
  if ($tid == 0) {
    $options['autocomplete'] = '';
  }
  foreach ($fields as $key => $fld) {
    if ($fld['type'] == 'contrib_widget') {
      continue;
    }
    $def_values[$fld['name']] = array();
    if ($tid) {
      if ($fld['visible']) {
        array_push($def_values[$fld['name']], 'visible');
      }
      if ($fld['required']) {
        array_push($def_values[$fld['name']], 'required');
      }
    }
    else {
      if ($fld['common']) {
        array_push($def_values[$fld['name']], 'common');
      }
      if ($fld['required']) {
        array_push($def_values[$fld['name']], 'required');
      }
      if ($fld['autocomplete']) {
        array_push($def_values[$fld['name']], 'autocomplete');
      }
    }
    $disabled = $tid ? ($fld['common'] ? 1 : 0) : 0;
    $form_state['tab_defaults'][$key]['common'] = $disabled;

    $tab_data[$fld['vtab']][$key]['#parents'] = array('biblio_tabs', $key);
    $tab_data[$fld['vtab']][$key]['name'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($fld['name']),
      '#weight' => $fld['weight'],
    );
    $tab_data[$fld['vtab']][$key]['title'] = array(
      '#type' => 'textfield',
      '#default_value' => $fld['title'],
      '#size' => 15,
      '#weight' => $fld['weight'],
      '#disabled' => $disabled,
    );
    // We need store the default to see if it's changed later.
    $form_state['tab_defaults'][$key]['title']['default'] = $fld['title'];
    $form_state['tab_defaults'][$key]['title']['disabled'] = $disabled;

    $tab_data[$fld['vtab']][$key]['hint'] = array(
      '#type' => 'textfield',
      '#default_value' => $fld['hint'],
      '#size' => 20,
      '#weight' => $fld['weight'],
      '#disabled' => $disabled,
    );
    $form_state['tab_defaults'][$key]['hint']['default'] = $fld['hint'];
    $form_state['tab_defaults'][$key]['hint']['disabled'] = $disabled;

    $tab_data[$fld['vtab']][$key]['weight'] = array(
      '#type' => 'textfield',
      '#default_value' => $fld['weight'],
      '#size' => 2,
      '#weight' => $fld['weight'],
      // '#disabled' => $disabled.
    );
    $tab_data[$fld['vtab']][$key]['checkboxes'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => $def_values[$fld['name']],
      '#weight' => $fld['weight'],
      // '#disabled' => $disabled.
    );
    $tab_data[$fld['vtab']][$key]['ftdid'] = array(
      '#type' => 'value',
      '#value' => $fld['ftdid'],
    );

  }
  if ($tid) {
    $header = array('', t('Field Name'), t('Title'), t('Hint'), t('Visible'), t('Required'), '');
  }
  else {
    $header = array('', t('Field Name'), t('Default Title'), t('Hint'), t('Common'), t('Required'), t('Autocomplete'), '');
  }

  foreach ($tab_data as $key => $rows) {
    $form['biblio_tabs'][$key][] = array(
      '#theme' => 'biblio_field_tab',
      '#header' => $header,
      'rows' => $rows,
    );
  }

  $form['biblio_tabs']['#tree'] = TRUE;
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form handler for biblio_admin_types_edit_form.
 */
function biblio_admin_types_edit_form_submit($form, &$form_state) {
  if ($form_state['triggering_element']['#value'] == t('Change Publication Type')) {
    $form_state['rebuild'] = TRUE;
    $form_state['pub_type'] = $form_state['values']['pub_type'];
    $form_state['input'] = array();
    return;
  }
  $tid = isset($form_state['values']['type_id']) ? $form_state['values']['type_id'] : 0;

  if ($tid) {
    // Save publication type name.
    $type_name_record = array(
      'tid' => $tid,
      'name' => $form_state['values']['type_name'],
    );

    db_update('biblio_types')
      ->fields($type_name_record)
      ->condition('tid', $tid)
      ->execute();
    biblio_locale_refresh_types($tid);
  }

  $hide_other_fields = isset($form_state['values']['hide_other_fields']) ? $form_state['values']['hide_other_fields'] : 0;
  variable_set('biblio_hide_other_fields', $hide_other_fields);

  // Save other field data.
  foreach ($form_state['values']['biblio_tabs'] as $fid => $values) {
    $update = FALSE;
    $val = array();
    $link = array();
    if (is_int($fid)) {
      $common = $form_state['tab_defaults'][$fid]['common'];
      $link['fid'] = $fid;
      $link['tid'] = $tid;
      foreach ($values as $name => $value) {
        $disabled = !empty($form_state['tab_defaults'][$fid][$name]['disabled']) ? TRUE : FALSE;
        if (!$disabled) {
          $default_value = isset($form_state['tab_defaults'][$fid][$name]['default']) ? $form_state['tab_defaults'][$fid][$name]['default'] : '';
          if ($name == 'checkboxes') {
            $link['visible'] = (!empty($value['visible'])) ? 1 : 0;
            $link['required'] = (!empty($value['required'])) ? 1 : 0;
            if ($tid == 0) {
              $link['common']       = (!empty($value['common'])) ? 1 : 0;
              $link['autocomplete'] = (!empty($value['autocomplete'])) ? 1 : 0;
            }
          }
          elseif ($name == 'weight') {
            $link['weight'] = $value;
          }
          else {
            $val[$name] = $common ? $default_value : $value;
            if (!$update) {
              $update = ($default_value != $val[$name]) ? $form_state['values']['biblio_tabs'][$fid]['ftdid'] : FALSE;
            }
          }
        }
      }
      // We just changed a default value, so create a new entry in biblio_field_type_data.
      if ($update == $fid && $link['tid']) {
        $new_ftdid = variable_get('biblio_last_ftdid', 100);
        variable_set('biblio_last_ftdid', $new_ftdid + 1);
        $val['ftdid'] = $new_ftdid;
        $link['ftdid'] = $new_ftdid;
        $link['cust_tdid'] = $new_ftdid;
        drupal_write_record('biblio_field_type_data', $val);

      }
      // We are updating an existing entry.
      elseif ($update >= 100 && $link['tid']) {
        $val['ftdid'] = $form_state['values']['biblio_tabs'][$fid]['ftdid'];
        drupal_write_record('biblio_field_type_data', $val, 'ftdid');

      }
      // Changing the defaults.
      elseif ($update == $fid) {
        $val['ftdid'] = $fid;
        drupal_write_record('biblio_field_type_data', $val, 'ftdid');
      }

      drupal_write_record('biblio_field_type', $link, array('fid', 'tid'));

      if ($tid == 0) {
        if ($link['common']) {
          // $query = "UPDATE {biblio_field_type} SET ftdid = %d, common = %d, visible = %d WHERE fid = %d";
          //        db_query($query, array($fid, 1, 1, $fid));.
          db_update('biblio_field_type')
            ->fields(array('ftdid' => $fid, 'common' => 1, 'visible' => 1))
            ->condition('fid', $fid)
            ->execute();
        }
        // Not common, so change pointer back to customizations if available.
        else {
          $query = "UPDATE {biblio_field_type} SET ftdid = cust_tdid, common = :comm WHERE fid = :fid";
          db_query($query, array(':comm' => 0, ':fid' => $fid));
          // db_update('biblio_field_type')
          //          ->fields(array('ftdid' => $fid, 'common' => 0, 'visible' => 1))
          //          ->condition('fid', $fid)
          //          ->execute();
        }
        // Set the autocomplete bit on this field for all the types
        //      $query = "UPDATE {biblio_field_type} SET autocomplete = %d WHERE fid = %d";
        //      db_query($query, array($link['autocomplete'], $fid));.
        db_update('biblio_field_type')
          ->fields(array('autocomplete' => $link['autocomplete']))
          ->condition('fid', $fid)
          ->execute();
      }
    }
  }
  drupal_set_message(t("The changes have been saved."));

  // Clear the cached pages and menus:
  // menu_rebuild();
  // Refresh translatable field strings.
  biblio_locale_refresh_fields($tid);
}

/**
 * Outputs a page containing "edit" links for each enabled import/export module.
 *
 *   Each link goes to a page allowing the user to modifiy the field mapping for
 *   each of the import/export formats.
 */
function biblio_admin_io_mapper_page() {
  $formats = module_invoke_all('biblio_mapper_options');
  asort($formats);

  foreach ($formats as $key => $value) {
    $rows[] = array(
      $value['title'],
      l(t('edit'), 'admin/config/content/biblio/iomap/edit/' . $key),
    );
  }

  if (count($rows) == 0) {
    drupal_set_message(t('There are no import/export modules enabled. Please go to the !l page and enable at least one.',
        array('!l' => l(t('modules'), "admin/modules"))));
  }

  $header = array(t('Format'), t('Action'));
  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * @param $format
 * @param $exportable
 *
 * @return Ambigous <multitype:string multitype:string  NULL , multitype:number multitype:string  multitype:string Ambigous <> Ambigous <string, unknown>  , multitype:string >
 */
function biblio_admin_io_mapper_form($form, $form_state, $format, $exportable = TRUE) {
  $formats = module_invoke_all('biblio_mapper_options');
  $form = array();
  if (isset($formats[$format])) {
    $form['title'] = array(
      '#prefix' => '<h3>',
      '#markup' => check_plain($formats[$format]['title']) . ' ' . t('file format mapping'),
      '#suffix' => '</h3>',
    );
    $form['fileformat_title'] = array(
      '#type' => 'hidden',
      '#value' => $formats[$format]['title'],
    );

  }
  $form['fileformat'] = array(
    '#type' => 'hidden',
    '#value' => $format,
  );
  $form['fileformat_export'] = array(
    '#type' => 'hidden',
    '#value' => isset($formats[$format]['export']) ? $formats[$format]['export'] : $exportable,
  );

  $form['typemap'] = array(
    '#type' => 'fieldset',
    '#title' => t('Publication types'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['typemap']['#theme'] = 'biblio_admin_type_mapper_form';
  $form['typemap']['#tree'] = TRUE;
  $form['typemap'] += biblio_admin_type_mapper_form($format);

  $form['fieldmap'] = array(
    '#type' => 'fieldset',
    '#title' => t('Data fields'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['fieldmap']['#theme'] = 'biblio_admin_field_mapper_form';
  $form['fieldmap']['#tree'] = TRUE;
  $form['fieldmap'] += biblio_admin_field_mapper_form($format, $exportable);

  return $form;
}

/**
 * @param $variables
 *
 * @return unknown
 */
function theme_biblio_admin_io_mapper_form($variables) {
  $form = $variables['form'];
  $header = $rows = array();
  $output = drupal_render($form['title']);
  $rows[] = array(
    'data' => array(
                  array('data' => drupal_render($form['typemap']), 'style' => 'vertical-align:top'),
                  array('data' => drupal_render($form['fieldmap']), 'style' => 'vertical-align:top'),
    ),
  );
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * @param $format
 * @param $type
 *
 * @return multitype:string NULL multitype:string
 */
function biblio_admin_io_mapper_add_form($form, &$form_state, $format, $type) {
  $formats = module_invoke_all('biblio_mapper_options');

  if ($type == 'pubtype') {
    $title = 'Publication Type';
    $desc = t('This is the name of the type identifier, exactly as it appears in the file');
    $submit = array('biblio_admin_io_mapper_add_form_pubtype_submit');
  }
  if ($type == 'field') {
    $title = 'Field name';
    $desc = t('This is the name of the field identifier, exactly as it appears in the file');
    $submit = array('biblio_admin_io_mapper_add_form_field_submit');
  }
  $form['fileformat'] = array(
    '#type' => 'hidden',
    '#value' => $format,
  );
  $form['fileformat_title'] = array(
    '#type' => 'hidden',
    '#value' => $formats[$format]['title'],
  );
  $form['type_name'] = array(
    '#type' => 'textfield',
    '#title' => 'Publication Type',
    '#required' => TRUE,
    '#description' => t('This is the name of the type identifier, exactly as it appears in the file'),
  );
  $form['type_desc'] = array(
    '#type' => 'textfield',
    '#title' => 'Description',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#submit' => $submit,
  );

  return $form;
}

/**
 * @param $variables
 *
 * @return unknown
 */
function theme_biblio_admin_io_mapper_add_form($variables) {
  $output = '';
  $form = $variables['form'];
  $title = $form['fileformat_title']['#value'];
  drupal_set_title(t('Add new publication type to %title file type', array('%title' => $title)));
  $output .= drupal_render($form['type_name']);
  $output .= drupal_render($form['type_desc']);
  $output .= drupal_render($form['submit']);
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_io_mapper_add_form_pubtype_submit($form, &$form_state) {
  $names = biblio_get_map('type_names', $form_state['values']['fileformat']);
  $names[$form_state['values']['type_name']] = $form_state['values']['type_desc'];
  biblio_set_map('type_names', $form_state['values']['fileformat'], $names);
  $form_state['redirect'] = 'admin/config/content/biblio/iomap/edit/' . $form_state['values']['fileformat'];
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_io_mapper_add_form_field_submit($form, &$form_state) {
  $names = biblio_get_map('field_map', $form_state['values']['fileformat']);
  $names[$form_state['values']['type_name']] = '';
  biblio_set_map('field_map', $form_state['values']['fileformat'], $names);
}

/**
 * @param $format
 *
 * @return Ambigous <multitype:string multitype:string  NULL , multitype:multitype:string  multitype:string unknown number  >
 */
function biblio_admin_type_mapper_form($format) {
  $formats                    = module_invoke_all('biblio_mapper_options');
  $form['#file_format_title'] = isset($formats[$format]) ? $formats[$format]['title'] : '';
  $form['#file_format']       = $format;
  $names                      = biblio_get_map('type_names', $format);
  $map                        = biblio_get_map('type_map', $format);
  ksort($names);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save publication type mapping'),
    '#submit' => array('biblio_admin_type_mapper_form_submit'),
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset publication type mapping to default'),
    '#submit' => array('biblio_admin_type_mapper_form_reset_submit'),
  );

  $result = db_query('SELECT t.* FROM {biblio_types} as t WHERE t.tid > 0');

  foreach ($result as $type) {
    $biblio_type_options[$type->tid] = $type->name;
  }

  $biblio_type_options[0] = t('-none-');
  asort($biblio_type_options);

  $biblio_type_select = array(
    '#type' => 'select',
    '#options' => $biblio_type_options,
  );
  foreach ($names as $key => $value) {
    $biblio_type_select['#default_value'] = (isset($map[$key]) ? $map[$key] : 0);
    $form['type'][$key] = array(
      'format' => array('#markup' => "<b>" . check_plain($key) . "</b> (<i>" . check_plain($value) . "</i>)"),
      'biblio' => $biblio_type_select,
    );

  }
  return $form;
}

/**
 * @param $variables
 *
 * @return unknown
 */
function theme_biblio_admin_type_mapper_form($variables) {
  $form = $variables['form'];
  $title = $form['#file_format_title'];

  foreach (element_children($form['type']) as $key) {
    $rows[] = array(
      drupal_render($form['type'][$key]['format']),
      drupal_render($form['type'][$key]),
    );
  }
  $rows[] = array(
    l('[' . t('Add New') . " $title " . t('Publication Type') . ']', 'admin/config/content/biblio/iomap/' . $form['#file_format'] . '/pubtype/add'),
    l('[' . t('Add New Biblio Publication Type') . ']', 'admin/config/content/biblio/pubtype/new'),
  );
  $header = array(
    $title . ' ' . t('Publication Types'),
    t('Biblio Publication Type'),
  );
  $output = theme('table', array('header' => $header, 'rows' => $rows));

  $output .= drupal_render($form['submit']);
  $output .= drupal_render($form['reset']);

  $output .= drupal_render_children($form);
  return $output;
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_type_mapper_form_submit($form, $form_state) {
  foreach ($form_state['values']['typemap']['type'] as $key => $value) {
    if (is_array($value)) {
      $map[$key] = $value['biblio'];
    }
  }
  biblio_set_map('type_map', $form_state['values']['fileformat'], $map);
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_type_mapper_form_reset_submit($form, $form_state) {
  biblio_reset_map('type_map', $form_state['values']['fileformat']);
  biblio_reset_map('type_names', $form_state['values']['fileformat']);
}

/**
 * @param $format
 * @param $exportable
 *
 * @return Ambigous <multitype:string multitype:string  NULL , multitype:number multitype:string  multitype:string Ambigous <> Ambigous <string, unknown>  , multitype:string >
 */
function biblio_admin_field_mapper_form($format, $exportable = TRUE) {
  $formats = module_invoke_all('biblio_mapper_options');
  $form['#file_format_title'] = isset($formats[$format]) ? $formats[$format]['title'] : '';
  $form['#file_format'] = $format;
  $exportable = isset($formats[$format]['export']) ? $formats[$format]['export'] : $exportable;
  $form['#file_format_export'] = $exportable;
  $map = biblio_get_map('field_map', $format);
  $export_map = biblio_get_map('export_map', $format);
  ksort($map);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save field mapping'),
    '#submit' => array('biblio_admin_field_mapper_form_submit'),
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset field mapping to default'),
    '#submit' => array('biblio_admin_field_mapper_form_reset_submit'),
  );

  $schema = drupal_get_schema('biblio');
  $fieldnames = array_keys($schema['fields']);
  asort($fieldnames);
  $biblio_field_options[''] = t('-none-');
  $biblio_field_options['title'] = 'title';
  foreach ($fieldnames as $field) {
    $biblio_field_options[$field] = $field;
  }

  $biblio_field_select = array(
    '#type' => 'select',
    '#options' => $biblio_field_options,
  );

  foreach ($map as $key => $value) {
    $biblio_field_select['#default_value'] = $map[$key];
    $form['type'][$key] = array(
      'format' => array('#markup' => "<b>" . $key . "</b>"),
      'biblio' => $biblio_field_select,
      '#tree' => 1,
    );
    if ($exportable) {
      $form['type'][$key]['export'] = array(
        '#type' => 'checkbox',
        '#default_value' => isset($export_map[$value]) ? $export_map[$value] : '',
      );
    }

  }
  return $form;
}

/**
 * @param $variables
 *
 * @return unknown
 */
function theme_biblio_admin_field_mapper_form($variables) {
  $form = $variables['form'];
  $title = $form['#file_format_title'];

  foreach (element_children($form['type']) as $key) {
    $row = array();
    $row[] = drupal_render($form['type'][$key]['format']);
    $row[] = drupal_render($form['type'][$key]['biblio']);
    if ($form['#file_format_export']) {
      $row[] = drupal_render($form['type'][$key]);
    }
    $rows[] = $row;
  }
  $header = array(
    $title . ' ' . t('field identifier'),
    t('Biblio schema field'),
  );
  if ($form['#file_format_export']) {
    $header[] = t('Export');
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows));
  // $output .= l('[' . t('Add New') . " $title " . t('Publication Type') . ']','admin/config/content/biblio/fields/typemap/' . $form['#file_format'] . '/add') . ' ';
  // $output .= l('[' . t('Add New Biblio Publication Type') . ']','admin/config/content/biblio/fields/type/new') . '<br />';.
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_field_mapper_form_submit($form, $form_state) {
  foreach ($form_state['values']['fieldmap']['type'] as $key => $value) {
    if (is_array($value)) {
      $map[$key] = $value['biblio'];
      if ($form_state['values']['fileformat_export']) {
        $export_map[$value['biblio']] = $value['export'];
      }
    }
  }
  biblio_set_map('field_map', $form_state['values']['fileformat'], $map);
  if ($form_state['values']['fileformat_export']) {
    biblio_set_map('export_map', $form_state['values']['fileformat'], $export_map);
  }

  // @code
  // elseif ($form_state['values']['op'] == t('Add')) {
  //   $names = biblio_get_map('type_names', $form_state['values']['fileformat']);
  //   $names[$form_state['values']['type_name']] = $form_state['values']['type_desc'];
  //   variable_set('biblio_'.$form_state['values']['fileformat'] .'_type_names', $names);
  // }
  // @endcode
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_field_mapper_form_reset_submit($form, $form_state) {
  biblio_reset_map('field_map', $form_state['values']['fileformat']);
  biblio_reset_map('export_map', $form_state['values']['fileformat']);
}

/**
 * @return unknown
 */
function biblio_admin_types_form() {
  $output = '';
  $result = db_query('SELECT t.* FROM {biblio_types} as t WHERE t.tid > 0 ORDER BY name ASC');
  // $rows[] = array('',t('Fields Common To All Types'),l('edit', 'admin/config/content/biblio/types/edit'),'');.
  foreach ($result as $row) {
    if ($row->tid < 999) {
      $visibility_link_text = $row->visible ? t('hide') : t('show');
      $visibility_link_path = 'admin/config/content/biblio/pubtype/' . ($row->visible ? 'hide' : 'show') . '/' . $row->tid;
      $visibility_link_options['query']['token'] = drupal_get_token($visibility_link_path);
      $rows[] = array(
        $row->tid,
        check_plain($row->name),
        $row->visible ? l(t('edit'), 'admin/config/content/biblio/fields/' . $row->tid) : '',
        l($visibility_link_text, $visibility_link_path, $visibility_link_options),
      );
    }
    else {
      $rows[] = array(
        $row->tid,
        check_plain($row->name),
        l(t('edit'), 'admin/config/content/biblio/fields/' . $row->tid), l(t('delete'), 'admin/config/content/biblio/pubtype/delete/' . $row->tid),
      );
    }
  }
  $header = array(
    t('Type Id'),
    t('Type Name'), array(
      'data' => t('Operations'),
      'colspan' => '2',
    ),
  );
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= l(t('Reset to defaults'), 'admin/config/content/biblio/pubtype/reset', array(
    'query' => array('token' => drupal_get_token('admin/config/content/biblio/pubtype/reset')),
  ));
  return $output;
}

/**
 * @param $form
 * @param $form_state
 *
 * @return multitype:string number NULL
 */
function biblio_admin_types_add_form($form, &$form_state) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Type Name'),
    '#size' => 20,
    '#weight' => 1,
    '#required' => TRUE,
    '#maxlength' => 64,
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#size' => 60,
    '#weight' => 2,
    '#maxlength' => 255,
  );
  $form['type_button'] = array(
    '#type' => 'submit',
    '#value' => t('Create New Type'),
    '#weight' => 3,
  );

  return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_types_add_form_submit($form, &$form_state) {
  $values['name'] = $form_state['values']['name'];
  $values['description'] = $form_state['values']['description'];
  $values['tid'] = variable_get('biblio_max_user_tid', '999') + 1;

  db_insert('biblio_types')
    ->fields($values)
    ->execute();

  variable_set('biblio_max_user_tid', $values['tid']);
  $result = db_query('SELECT * FROM {biblio_field_type} WHERE tid=0', array(), array('fetch' => PDO::FETCH_ASSOC));
  foreach ($result as $row) {
    $row['tid'] = $values['tid'];
    $row['visible'] = 1;
    db_insert('biblio_field_type')
      ->fields($row)
      ->execute();
  }
  // Fill contributor types. Use the first 4 defaults.
  for ($type = 1; $type <= 4; $type++) {
    $ct_vals = array(
      'auth_category' => $type,
      'biblio_type' => $values['tid'],
      'auth_type' => $type,
    );

    db_insert('biblio_contributor_type')
      ->fields($ct_vals)
      ->execute();
  }

  // Refresh publication type string for translation.
  biblio_locale_refresh_types($values['tid']);
  $form_state['redirect'] = 'admin/config/content/biblio/pubtype';

}

/**
 *
 */
function biblio_admin_types_reset() {
  if (!isset($_GET['token']) || !drupal_valid_token($_GET['token'], 'admin/config/content/biblio/pubtype/reset')) {
    return MENU_ACCESS_DENIED;
  }

  module_load_include('install', 'biblio');

  db_delete('biblio_types')
    ->execute();
  _add_publication_types();
  drupal_goto('admin/config/content/biblio/pubtype');
}

/**
 *
 */
function biblio_admin_types_hide() {
  $args = func_get_args();
  if ($args[0] > 0 && is_numeric($args[0])) {
    if (!isset($_GET['token']) || !drupal_valid_token($_GET['token'], 'admin/config/content/biblio/pubtype/hide/' . $args[0])) {
      return MENU_ACCESS_DENIED;
    }
    db_update('biblio_types')
      ->fields(array(
        'visible' => 0,
      ))
      ->condition('tid', $args[0])
      ->execute();
  }
  drupal_goto('admin/config/content/biblio/pubtype');
}

/**
 *
 */
function biblio_admin_types_show() {
  $args = func_get_args();
  if ($args[0] > 0 && is_numeric($args[0])) {
    if (!isset($_GET['token']) || !drupal_valid_token($_GET['token'], 'admin/config/content/biblio/pubtype/show/' . $args[0])) {
      return MENU_ACCESS_DENIED;
    }
    db_update('biblio_types')
      ->fields(array(
        'visible' => 1,
      ))
      ->condition('tid', $args[0])
      ->execute();
  }
  drupal_goto('admin/config/content/biblio/pubtype');
}

/**
 * @param $form
 * @param $form_state
 * @param $tid
 *
 * @return unknown
 */
function biblio_admin_types_delete_form($form, &$form_state, $tid) {
  $existing_msg = '';
  if (isset($tid) && is_numeric($tid)) {
    $row = db_query('SELECT t.* FROM {biblio_types} as t WHERE t.tid = :tid ', array(':tid' => $tid))->fetchObject();
    $num_rows = db_query('SELECT COUNT(*) FROM {biblio} as b WHERE b.biblio_type = :btype', array(':btype' => $row->tid))->fetchField();
    if ($num_rows) {
      $existing_msg = t('There are @count biblio entries of this type, you should change the type of these entries before proceeding otherwise they will be deleted', array(
        '@count' => $num_rows,
      ));
    }
    $form['tid'] = array(
      '#type' => 'value',
      '#value' => $row->tid,
    );
    $output = confirm_form($form, t('Are you sure you want to delete the custom biblio type:  %title ?', array(
      '%title' => $row->name,
    )) . $existing_msg, isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/content/biblio/pubtype', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
    return $output;
  }
  else {
    drupal_goto('admin/config/content/biblio/pubtype');
  }
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_types_delete_form_submit($form, &$form_state) {
  if (!empty($form_state['values']['tid'])) {
    $ftdids = array();
    $query = db_select('biblio_field_type', 'bft');
    $result = $query->fields('bft', array('ftdid'))
      ->condition('tid', $form_state['values']['tid'])
      ->condition('ftdid', 100, '>')
      ->execute();
    foreach ($result as $field) {
      $ftdids[] = $field->ftdid;
    }

    if (!empty($ftdids)) {
      db_delete('biblio_field_type_data')
        ->condition('ftdid', $ftdids, 'IN')
        ->execute();
    }

    db_delete('biblio_types')
      ->condition('tid', $form_state['values']['tid'])
      ->execute();

    db_delete('biblio_field_type')
      ->condition('tid', $form_state['values']['tid'])
      ->execute();

    drupal_set_message(t('Publication type has been deleted.'));
  }

  $form_state['redirect'] = 'admin/config/content/biblio/pubtype';
}

/**
 * @return unknown
 */
function biblio_admin_types_reset_form() {
  $form['reset'] = array(
    '#type' => 'value',
    '#value' => 'reset',
  );
  $output = confirm_form($form, t('Are you sure you want to reset ALL the field definitions to the defaults?'), isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/content/biblio/fields', t('By doing this, you will loose all customizations you have made to the field titles, <b><u>this action cannot be undone</u></b>!'), t('Reset!'), t('Cancel'));
  return $output;
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_types_reset_form_submit($form, &$form_state) {
  module_load_include('install', 'biblio');
  biblio_reset_types();
  $form_state['redirect'] = 'admin/config/content/biblio/fields';
}

/**
 * This functin is used by both the admin/config/content/biblio page and user profile page
 * - if $user is set, then it is being called from the user profile page.
 */
function _biblio_get_user_profile_form($profile_user = FALSE) {
  $form = array();
  $allow_edit = variable_get('biblio_show_user_profile_form', '1');
  $allow_edit = $allow_edit || user_access('administer biblio');

  if (!$profile_user) {
    $form['biblio_show_user_profile_form'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow users to override these settings on their "My account" page') ,
      '#return_value' => 1,
      '#description' => t('If this is selected, a form similar to this section will be available to the user when they edit their own account information.  This will allow them to override the global preferences set here.') ,
      '#default_value' => variable_get('biblio_show_user_profile_form', '1'),
    );
  }
  $form['biblio_show_profile'] = array(
    '#type' => 'checkbox',
    '#title' => ($profile_user) ? t('Show my publications on my profile page') : t('Show publications on users profile pages'),
    '#return_value' => 1,
    '#disabled' => !$allow_edit,
    '#description' => ($profile_user) ? t('Selecting this will create a listing of your publications on your profile page') : t('This sets the site wide default, users may change this in their profile'),
  );
  if ($profile_user) {
    $form['biblio_show_profile']['#default_value'] = (isset($profile_user->data['biblio_show_profile'])) ? $profile_user->data['biblio_show_profile'] : variable_get('biblio_show_profile', '0');
  }
  else {
    $form['biblio_show_profile']['#default_value'] = variable_get('biblio_show_profile', '0');
  }
  $form['#biblio_show_profile'] = $form['biblio_show_profile']['#default_value'];
  $form['biblio_my_pubs_menu'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show "My publications" item in the navigation menu'),
    '#disabled' => !$allow_edit,
    '#return_value' => 1,
    '#description' => '',
  );
  if ($profile_user) {
    $form['biblio_my_pubs_menu']['#default_value'] = ((isset($profile_user->data['biblio_my_pubs_menu'])) ? $profile_user->data['biblio_my_pubs_menu'] : variable_get('biblio_my_pubs_menu', '0'));
  }
  else {
    $form['biblio_my_pubs_menu']['#default_value'] = variable_get('biblio_my_pubs_menu', '0');
  }
  $form['#biblio_my_pubs_menu'] = $form['biblio_my_pubs_menu']['#default_value'];

  if ($profile_user) {
    $form += _biblio_drupal_author_user_map($profile_user, $allow_edit);
  }

  $options = array();
  $options['system'] = t('System default');
  $options = array_merge($options, biblio_get_styles());
  $form['biblio_user_style'] = array(
    '#type' => 'select',
    '#title' => t('Style'),
    '#default_value' => (isset($profile_user->data['biblio_user_style']) ? $profile_user->data['biblio_user_style'] : 'system'),
    '#options' => $options,
    '#description' => t('Set the bibliographic style of the "list" view.'),
  );

  return $form;
}

/**
 *
 */
function _biblio_drupal_author_user_map($profile_user, $allow_edit = TRUE) {
  global $user;

  $db_result = db_query("SELECT cd.lastname, cd.firstname, cd.initials, cd.cid FROM {biblio_contributor_data} cd
      ORDER by cd.lastname ASC ");
  $options = array();
  $options[0] = t('(None)');

  foreach ($db_result as $row) {
    $options[$row->cid] = "$row->lastname, $row->firstname $row->initials ";
  }
  if (isset($profile_user->data['biblio_id_change_count']) && $profile_user->data['biblio_id_change_count'] > 2) {
    $allow_edit = 0;
    $msg = t('This control has been disabled because the author mapping has been changed more than 3 times, please see your system administrator to have it reset.');
  }
  else {
    $msg = t('This will link your profile to the selected author from the biblio database, then all publications containing this author to be displayed on your "Publications" tab.');
  }
  $form['biblio_contributor_id'] = array(
    '#type' => 'select',
    '#title' => t('Link My Profile with this Author from the Biblio database:'),
    '#default_value' => isset($profile_user->data['biblio_contributor_id']) ? $profile_user->data['biblio_contributor_id'] : 0,
    '#disabled' => ($user->uid == 1 || user_access('administer biblio')) ? FALSE : !$allow_edit,
    '#options' => $options,
    '#description' => $msg,
  );
  $form['biblio_id_change_count'] = array(
    '#type' => 'textfield',
    '#size' => 2,
    '#title' => t('Drupal UserID to Biblio AuthorID mapping change count:'),
    '#default_value' => isset($profile_user->data['biblio_id_change_count']) ? $profile_user->data['biblio_id_change_count'] : 0,
    '#disabled' => ($user->uid == 1 || user_access('administer biblio')) ? FALSE : TRUE,
    '#description' => 'When this value is >= 3, you will no longer be able to change your author id mapping.  Contact your system administrator to reset this value.',
  );

  return $form;
}

/**
 * @param $user
 *
 * @return multitype:string number NULL
 */
function _biblio_get_user_doi_form($user) {
  $form['biblio_doi'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('CrossRef Login Information'),
  );
  $link_attrs = array(
    'attributes' => array('target' => '_blank'),
    'absolue' => TRUE,
  );
  $form['biblio_doi']['biblio_crossref_pid'] = array(
    '#type' => 'textfield',
    '#title' => t('CrossRef OpenURL Account ID'),
    '#default_value' => isset($user->data['biblio_crossref_pid']) ? $user->data['biblio_crossref_pid'] : '',
    '#return_value' => 1,
    '#description' => t('Enter your complimentary CrossRef OpenURL account ID which you can obtain here !url, OR enter your full CrossRef (colon separated) account:password combination.', array('!url' => l(t('OpenURL Account Request Form'), 'http://www.crossref.org/requestaccount/', $link_attrs))),
  );
  return $form;
}

/**
 * @param $user
 *
 * @return multitype:string number NULL
 */
function _biblio_get_user_openurl_form($user) {
  $form['openurl'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('OpenURL'),
    '#description' => t('You can set an <a href="http://en.wikipedia.org/wiki/OpenURL">openurl</a> link here'),
  );
  $form['openurl']['biblio_baseopenurl'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenURL Base URL'),
    '#size' => 95,
    '#default_value' => !empty($user->data['biblio_baseopenurl']) ? $user->data['biblio_baseopenurl'] : '',
    '#description' => t('This sets your base <a href="http://en.wikipedia.org/wiki/OpenURL">OpenURL</a> gateway, which is used to generate OpenURL links. To implement a "Universal" OpenURL system, try using OCLC\'s <a href="http://www.oclc.org/productworks/urlresolver.htm">OpenURL Resolver Registry</a> gateway: <a href="http://worldcatlibraries.org/registry/gateway">http://worldcatlibraries.org/registry/gateway</a>'),
  );
  $sid = "Biblio:" . variable_get('site_name', 'Drupal');
  $form['openurl']['biblio_openurl_sid'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenURL Site ID'),
    '#size' => 95,
    '#default_value' => !empty($user->data['biblio_openurl_sid']) ? $user->data['biblio_openurl_sid'] : '',
    '#description' => t('This sets your site name, some link resolvers will require a specific Site ID in order to process your requests.'),
  );
  return $form;
}

/**
 * This function parses the module directory for 'style' files, loads them and
 * calls the info fuction to get some basic information like the short and long
 * names of the style.
 */
function biblio_form_sort($a, $b) {
  $a_weight = (is_array($a) && isset($a['weight'])) ? $a['weight'] : 0;
  $b_weight = (is_array($b) && isset($b['weight'])) ? $b['weight'] : 0;
  if ($a_weight == $b_weight) {
    return 0;
  }
  return ($a_weight < $b_weight) ? -1 : 1;
}

/**
 * @param $name
 *
 * @return string
 */
function biblio_admin_get_query($name) {
  switch ($name) {
    case "author_dup":
      return ('SELECT lastname, firstname, initials, COUNT(lastname) as cnt  FROM {biblio_contributor_data} GROUP BY lastname, firstname, initials HAVING COUNT(*)>1 ORDER BY lastname ASC, firstname ASC, initials ASC ');

    break;
    case "author_by_name":
      return ("SELECT lastname, cid FROM {biblio_contributor_data} where lastname LIKE '%s'");

    break;
    case 'author_pub_count':
      // Return ('SELECT bd.lastname, b.cid,COUNT(*) AS cnt FROM {biblio_contributor} b, {biblio_contributor_data} bd WHERE bd.cid=b.cid GROUP BY b.cid HAVING cnt > 0 ORDER BY lastname ASC');.
      return ('SELECT bd.lastname, b.cid,COUNT(*) AS cnt FROM {biblio_contributor} b, {biblio_contributor_data} bd WHERE bd.cid=b.cid GROUP BY b.cid, bd.lastname HAVING COUNT(*) > 0 ORDER BY lastname ASC');

    break;
  }
}

/**
 * @param $form
 * @param $form_state
 * @param $op
 * @param $id
 *
 * @return void|multitype:string multitype:string number NULL  multitype:string number multitype:string  NULL  multitype:string number boolean NULL  NULL
 */
function biblio_admin_author_types_form($form, &$form_state, $op = NULL, $id = NULL) {

  switch ($op) {
    case 'edit':
      $type = db_query("SELECT * FROM {biblio_contributor_type_data} WHERE auth_type=:atype", array(':atype' => $id))->fetchObject();
      $form['auth_type'] = array(
        '#type' => 'hidden',
        '#value' => $type->auth_type,
      );
      // Fall through and use the same form used for a new entry.
    case 'new':
      $form['title'] = array(
        '#type' => 'textfield',
        '#title' => t('Type Name'),
        '#size' => 20,
        '#weight' => 1,
        '#default_value' => ($op == 'new' ? '' : $type->title),
        '#required' => TRUE,
        '#maxlength' => 64,
      );
      $form['hint'] = array(
        '#type' => 'textfield',
        '#title' => t('Description'),
        '#size' => 60,
        '#weight' => 2,
        '#default_value' => ($op == 'new' ? '' : $type->hint),
        '#maxlength' => 255,
      );
      $form['type_button'] = array(
        '#type' => 'submit',
        '#value' => ($op == 'new') ? t('Create New Type') : t('Save'),
        '#weight' => 3,
        '#submit' => array('biblio_admin_author_types_form_submit'),
      );
      $form['cancel_button'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#weight' => 4,
      );
      $form['#theme'] = '';
      return $form;

    break;
    case 'hide':
      break;

    default:
      return;
  }
}

/**
 * @param $variables
 *
 * @return string
 */
function theme_biblio_admin_author_types_form($variables) {
  $form = $variables['form'];
  // We need this complex query to realize author_types which are not in use (cid is NULL)
  $query = db_select('biblio_contributor_type_data', 'ctd');
  $query->fields('ctd', array('auth_type', 'title', 'hint'))
    ->leftJoin('biblio_contributor', 'bc', 'ctd.auth_type = bc.auth_type');
  $db_result = $query->groupBy('ctd.auth_type')
    ->groupBy('ctd.title')
    ->groupBy('ctd.hint')
    ->orderBy('ctd.auth_type')
    ->execute();

  foreach ($db_result as $row) {
    $ops = l(t('edit'), 'admin/config/content/biblio/author/type/' . $row->auth_type . '/edit');
    // Allow delete only if type not in use.
    if (!isset($row->cid) && $row->auth_type >= 10) {
      $ops .= '&nbsp;&nbsp;';
      $ops .= l(t('delete'), 'admin/config/content/biblio/author/type/' . $row->auth_type . '/delete/');
    }
    $rows[] = array(
      $row->auth_type,
      check_plain($row->title),
      check_plain($row->hint),
      $ops,
    );
  }
  $header = array(
    t('Type Id'),
    t('Contributor Type'),
    t('Description'),
    array('data' => t('Operations'), 'colspan' => '2'),
  );
  $output = '<p>[ ' . l(t('Add New Type'), 'admin/config/content/biblio/author/type/new') . ' ]';
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= '<p>[ ' . l(t('Add New Type'), 'admin/config/content/biblio/author/type/new') . ' ]';
  // $output .= ' [ ' . l(t('Reset all types to defaults'), 'admin/config/content/biblio/authors/reset') . ' ]';.
  return $output;
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_author_types_form_submit($form, &$form_state) {

  if ($form_state['triggering_element']['#value'] == t('Save') || $form_state['triggering_element']['#value'] == t('Create New Type')) {
    $record->title = $form_state['values']['title'];
    $record->hint = $form_state['values']['hint'];
    switch ($form['#id']) {
      case 'biblio-admin-author-types-form-new':
        $record->title = $form_state['values']['title'];
        $record->hint = $form_state['values']['hint'];
        drupal_write_record('biblio_contributor_type_data', $record);
        break;

      case 'biblio-admin-author-types-form-edit':
        $record->auth_type = $form_state['values']['auth_type'];
        drupal_write_record('biblio_contributor_type_data', $record, 'auth_type');
        break;
    }
  }
  $form_state['redirect'] = 'admin/config/content/biblio/author/type';

}

/**
 * @param $form
 * @param $form_state
 * @param $type_id
 */
function biblio_admin_author_type_delete_confirm($form, &$form_state, $type_id) {
  $base = variable_get('biblio_base', 'biblio');
  $type_data = db_query('SELECT * FROM {biblio_contributor_type_data} bctd WHERE bctd.auth_type = :tid ', array(':tid' => $type_id))->fetchObject();
  $form['auth_type'] = array(
    '#type' => 'value',
    '#value' => $type_data->auth_type,
  );

  return confirm_form($form,
    t('Are you sure you want to delete the author type: %title ?', array('%title' => $type_data->title)),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/content/biblio/author/type',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );

}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_author_type_delete_confirm_submit($form, &$form_state) {

  db_delete('biblio_contributor_type_data')
    ->condition('auth_type', $form_state['values']['auth_type'])
    ->execute();

  db_delete('biblio_contributor_type')
    ->condition('auth_type', $form_state['values']['auth_type'])
    ->execute();

  $form_state['redirect'] = 'admin/config/content/biblio/author/type';
}

/**
 * @param $form
 * @param $form_state
 * @param $author_id
 *
 * @return void|multitype:string NULL
 */
function biblio_admin_author_edit_form($form, &$form_state, $author_id) {
  module_load_include('inc', 'biblio', 'includes/biblio.contributors');
  $merge_options = $linked = array();
  if (!isset($form_state['biblio_add_merge_author'])) {
    $form_state['biblio_add_merge_author'] = array();
  }
  $base = variable_get('biblio_base', 'biblio');

  $author = db_query('SELECT * FROM {biblio_contributor_data} b WHERE b.cid = :aid ', array(':aid' => $author_id))->fetchObject();
  if (!$author) {
    drupal_not_found();
    return;
  }
  $base = variable_get('biblio_base', 'biblio');
  $menu = menu_get_active_title();
  $path = (strpos($_GET['q'], 'config')) ? 'admin/config/content/biblio/author/' : $base . '/authors/';

  // $form['#tree'] = TRUE;.
  $form['cid'] = array(
    '#type' => 'value',
    '#value' => $author_id,
  );
  $users = db_query('SELECT uid,name FROM {users} WHERE uid>0 ORDER BY name');
  $options[0] = t('(None)');
  foreach ($users as $user) {
    $options[$user->uid] = $user->name;
  }
  $form['drupal_uid'] = array(
    '#type' => 'select',
    '#title' => t('Drupal User ID'),
    '#options' => $options,
    '#default_value' => $author->drupal_uid,
    '#weight' => 12,
    '#required' => FALSE,
    '#description' => t('If this author has a an account (Drupal User ID) on this web site, you may select it here.  This will help to link the authors publications with other content.'),
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Complete Name'),
    '#default_value' => $author->name,
    '#size' => 100,
    '#weight' => 1,
    '#required' => TRUE,
    '#disabled' => TRUE,
    '#maxlength' => 255,
    '#description' => t('The value in this box will be constructed from the individual name parts fields above.'),
  );
  $form_state['current_name'] = $author->name;

  $form['literal'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not reformat'),
    '#default_value' => $author->literal,
    '#weight' => 1.5,
    '#required' => FALSE,
    '#description' => t('Selecting this will prevent the styles from trying to reformat the contributor name.  The text in the "Complete Name" field will be used as is.'),
  );
  $form['prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Prefix'),
    '#default_value' => $author->prefix,
    '#size' => 20,
    '#weight' => 2,
    '#required' => FALSE,
    '#maxlength' => 128,
  );
  $form['firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name'),
    '#default_value' => $author->firstname,
    '#size' => 20,
    '#weight' => 3,
    '#required' => FALSE,
    '#maxlength' => 128,
  );
  $form['initials'] = array(
    '#type' => 'textfield',
    '#title' => t('Initials'),
    '#default_value' => $author->initials,
    '#size' => 20,
    '#weight' => 4,
    '#required' => FALSE,
    '#maxlength' => 10,
  );
  $form['lastname'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#default_value' => $author->lastname,
    '#size' => 20,
    '#weight' => 5,
    '#required' => FALSE,
    '#maxlength' => 255,
  );
  $form['suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('Suffix'),
    '#default_value' => $author->suffix,
    '#size' => 20,
    '#weight' => 6,
    '#required' => FALSE,
    '#maxlength' => 128,
  );
  $form['affiliation'] = array(
    '#type' => 'textfield',
    '#title' => t('Affiliation'),
    '#default_value' => $author->affiliation,
    '#size' => 100,
    '#weight' => 7,
    '#required' => FALSE,
    '#maxlength' => 256,
    '#description' => t('University, Company or Organization that the author is affiliated with.'),
  );

  $query = db_select('biblio_contributor_data', 'bcd');
  $authors = $query->fields('bcd')
    ->condition('cid', $author_id, '<>')
    ->orderBy('lastname')
    ->execute();

  $query = db_select('biblio_contributor', 'bc');
  $merged_authors = $query->fields('bc')
    ->condition('merge_cid', 0, '>')
    ->execute();
  foreach ($merged_authors as $auth) {
    $merged[$auth->merge_cid] = $auth->cid;
  }

  $linked = biblio_get_linked_contributors($author->cid);

  $radio_options = array('link' => '', 'merge' => '');
  $candidate = FALSE;
  $candidates = array();

  $this_soundx = soundex($author->lastname);

  foreach ($authors as $other_author) {
    $merge_def = $link_def = 0;
    $link_disable = $merge_disable = $retain_disable = FALSE;

    if ((soundex($other_author->lastname) == $this_soundx) ||
        (isset($merged[$other_author->cid]) && $merged[$other_author->cid] == $author->cid) ||
        in_array($other_author->cid, $form_state['biblio_add_merge_author'])) {
      $candidate = TRUE;
      $merge_def = ($other_author->alt_form && $other_author->aka == $author->cid) ? $author->cid : FALSE;
      $retain_def = $other_author->alt_form ? $author->cid : FALSE;
      $retain_disable = $other_author->alt_form ? TRUE : FALSE;
    }

    if (in_array($other_author->cid, $linked)) {
      $candidate = TRUE;
      $link_def = $author->cid;
      $retain_def = 0;
    }

    if ($candidate) {
      $candidate = FALSE;
      $form_state['biblio_add_merge_author'][$other_author->cid] = $other_author->cid;

      $candidates[$other_author->cid]['name'] = array(
        '#markup' => l($other_author->lastname . ($other_author->firstname ? ', ' . $other_author->firstname : ($other_author->initials ? ', ' . $other_author->initials : '')), $path . $other_author->cid . '/edit/'),
        '#markup' => l($other_author->name, $path . $other_author->cid . '/edit/'),
      );
      $candidates[$other_author->cid]['link'] = array(
        '#type' => 'checkbox',
        '#return_value' => $author->cid,
        '#default_value' => $link_def,
        '#disabled' => $link_disable,
        '#parents' => array('candidates', $other_author->cid, 'link'),
        '#states' => array(
          'disabled' => array(
            ':input[name="candidates[' . $other_author->cid . '][merge]"]' => array('checked' => TRUE),
          ),
        ),
      );
      $candidates[$other_author->cid]['merge'] = array(
        '#type' => 'checkbox',
        '#return_value' => $author->cid,
        '#default_value' => $merge_def,
        '#disabled' => $merge_disable,
        '#parents' => array('candidates', $other_author->cid, 'merge'),
        '#states' => array(
          'disabled' => array(
            ':input[name="candidates[' . $other_author->cid . '][link]"]' => array('checked' => TRUE),
          ),
        ),
      );
      $candidates[$other_author->cid]['retain'] = array(
        '#type' => 'checkbox',
        '#return_value' => $author->cid,
        '#default_value' => $retain_def,
        '#disabled' => $retain_disable,
        '#parents' => array('candidates', $other_author->cid, 'retain'),
        '#states' => array(
          'enabled' => array(
            ':input[name="candidates[' . $other_author->cid . '][merge]"]' => array('checked' => TRUE),
          ),
          'unchecked' => array(
            ':input[name="candidates[' . $other_author->cid . '][merge]"]' => array('unchecked' => TRUE),
          ),
        ),
      );
    }
  }

  $form['merge'] = array(
    '#type' => 'fieldset',
    '#theme' => 'biblio_admin_author_edit_merge_table',
    '#title' => t('Author Link / Merge'),
    '#description' => t('Select other author names which will be linked or merged.  Merging removes all the selected authors, then replaces any references to these authors with the author being edited above.  You should only do this if you are sure the other authors represent the same author as the one being edited. <b>IF you do not select "Retain as alternate form" then THIS CANNOT BE UNDONE!</b>'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => !count($candidates),
    '#header' => array(t('Author name'), t('Link'), t('Merge'), t('Retain as "alternate form"')),
    'candidates' => $candidates,
    '#prefix' => '<div id="biblio-author-edit-merge-table">',
    '#suffix' => '</div>',

  );
  $form['merge']['more_authors_search'] = array(
    '#type' => 'textfield',
    '#title' => t('Other authors which could be linked to this author'),
    '#autocomplete_path' => 'biblio/autocomplete/contributor',
    '#weight' => 12,
    '#required' => FALSE,
  );
  $form['merge']['more_authors_add'] = array(
    '#type'     => 'submit',
    '#value'    => t('Add author'),
    '#weight' => 15,
    '#submit'   => array('biblio_admin_author_edit_add_candidate'),
    '#ajax'   => array(
      'callback' => 'biblio_admin_author_edit_add_candidate_callback',
      'wrapper'  => 'biblio-author-edit-merge-table',
    ),

  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}

/**
 *
 */
function biblio_admin_author_edit_add_candidate($form, &$form_state) {
  module_load_include('inc', 'biblio', 'includes/biblio.contributors');
  if (!empty($form_state['values']['more_authors_search'])) {
    if ($cid = biblio_get_cid_by_name($form_state['values']['more_authors_search'])) {
      // We don't want to merge with ourselves :-(.
      if ($cid == $form_state['values']['cid']) {
        drupal_set_message(t('You cannot merge an author with itself'), 'error');
      }
      elseif (in_array($cid, $form_state['biblio_add_merge_author'])) {
        drupal_set_message(t('Author already exists in the merge list'), 'warning');
      }
      else {
        $form_state['biblio_add_merge_author'][$cid] = $cid;
      }
    }
    else {
      drupal_set_message(t('The Author was not found in the database.  You must select an Author from the "auto-complete" list of the Keyword search box.'), 'error');
    }
  }
  else {
    drupal_set_message(t('Cannot add an empty value'), 'error');
  }

  $form_state['rebuild'] = TRUE;

}

/**
 *
 */
function biblio_admin_author_edit_add_candidate_callback($form, $form_state) {
  return $form['merge'];
}

/**
 * @param array $form
 * @param array $form_state
 */
function biblio_admin_author_edit_form_merge_validate($form, &$form_state) {
}

/**
 * @param array $form
 * @param array $form_state
 */
function biblio_admin_author_edit_form_merge_link($form_state) {
  $op = $form_state['triggering_element']['#value'];
  $merge_authors = array();
  $cid = $form_state['values']['cid'];

  foreach ($form_state['values']['candidates'] as $ccid => $options) {

    if ($options['link']) {
      db_update('biblio_contributor_data')
        ->fields(array(
          'aka' => $options['link'],
          'alt_form' => 0,
        ))
        ->condition('cid', $ccid)
        ->execute();
    }
    else {
      db_update('biblio_contributor_data')
        ->fields(array(
          'aka' => 0,
          'alt_form' => 0,
        ))
        ->condition(db_or()->condition('cid', $cid)->condition('cid', $ccid))
        ->execute();
    }

    if ($options['merge']) {
      db_update('biblio_contributor')
        ->fields(array(
          'cid' => $cid,
          'merge_cid' => $ccid,
        ))
        ->condition('cid', $ccid)
        ->execute();
    }
    else {
      db_update('biblio_contributor')
        ->fields(array(
          'cid' => $ccid,
          'merge_cid' => 0,
        ))
        ->condition('merge_cid', $ccid)
        ->execute();
    }

    if ($options['merge'] && $options['retain']) {
      db_update('biblio_contributor_data')
        ->fields(array(
          'alt_form' => $options['retain'] ? $options['retain'] : 0,
          'aka' => $options['retain'] ? $cid : 0,
        ))
        ->condition('cid', $ccid)
        ->execute();
    }
    elseif ($options['merge'] && !$options['retain']) {
      db_delete('biblio_contributor_data')
        ->condition('cid', $ccid)
        ->execute();
    }
  }
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_author_edit_form_link_validate($form, &$form_state) {

}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_author_edit_form_link_submit($form, &$form_state) {
  module_load_include('inc', 'biblio', 'includes/biblio.contributors');
  $linked_authors = array();
  $link_authors = '';

  if (isset($form_state['values']['linked_authors'])) {
    $linked_authors = $form_state['values']['linked_authors'];
  }

  if (isset($form_state['values']['link_authors'])) {
    $link_authors = $form_state['values']['link_authors'];
  }

  foreach ($linked_authors as $key => $value) {
    if ($value == 0) {
      db_update('biblio_contributor_data')
        ->fields(array('aka' => $value))
        ->condition('cid', $key)
        ->execute();
    }
  }

  if (!empty($link_authors)) {
    if (($cid = biblio_get_cid_by_name($link_authors))) {
      db_update('biblio_contributor_data')
        ->fields(array('aka' => $form_state['values']['cid']))
        ->condition('cid', $cid)
        ->execute();
    }
  }

}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_author_edit_form_validate($form, &$form_state) {
  foreach ($form_state['values'] as $key => $value) {
    if (is_string($value)) {
      $form_state['values'][$key] = trim($value);
    }
  }
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_author_edit_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  switch ($op) {
    case t('Save'):
      if ($form_state['values']['drupal_uid'] == 0) {
        $uid = $form['drupal_uid']['#default_value'];
        $cid = 0;
      }
      else {
        $uid = $form_state['values']['drupal_uid'];
        $cid = $form_state['values']['cid'];
      }
      if ($uid) {
        db_update('biblio_contributor_data')
          ->fields(array('drupal_uid' => 0))
          ->condition('drupal_uid', $uid)
          ->execute();
        $result = db_query('SELECT data FROM {users} WHERE uid = :uid', array(':uid' => $uid))->fetchField();
        $data = unserialize($result);
        $data['biblio_contributor_id'] = $cid;
        $v = serialize($data);
        db_update('users')
          ->fields(array('data' => $v))
          ->condition('uid', $uid)
          ->execute();
      }

      $form_state['values']['name'] =
        (!empty($form_state['values']['prefix']) ? $form_state['values']['prefix'] . ' ' : '') .
        (!empty($form_state['values']['firstname']) ? $form_state['values']['firstname'] . ' ' : '') .
        (!empty($form_state['values']['initials']) ? $form_state['values']['initials'] . ' ' : '') .
        (!empty($form_state['values']['lastname']) ? $form_state['values']['lastname'] . ' ' : '') .
        (!empty($form_state['values']['suffix']) ? $form_state['values']['suffix'] : '');
      $form_state['values']['name'] = trim($form_state['values']['name']);
      $form_state['values']['md5'] = md5($form_state['values']['name']);

      drupal_write_record('biblio_contributor_data', $form_state['values'], 'cid');

      if (isset($form_state['values']['candidates']) && !empty($form_state['values']['candidates'])) {
        biblio_admin_author_edit_form_merge_link($form_state);
      }
      break;

    case t('Cancel'):
      break;
  }
}

/**
 *
 * @param  $form
 * @param  $form_state
 *
 * @return array
 */
function biblio_admin_orphans_form($form, &$form_state) {
  module_load_include('inc', 'biblio', 'includes/biblio.contributors');
  $orphans = $options = $names = array();

  $base = variable_get('biblio_base', 'biblio');

  $orphans = biblio_get_orphan_authors();

  foreach ($orphans as $author) {
    $options[$author->cid] = array(
      'author' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $author->name,
          '#href' => $base . '/authors/' . $author->cid . '/edit',
        ),
      ),
      'affiliation' => check_plain($author->affiliation),
    );
    $names[$author->cid] = $author->name;
  }
  $form['names'] = array('#type' => 'hidden', '#value' => $names);

  $header = array(
    'author' => t('Author name'),
    'affiliation' => t('Author affiliation'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete Selected'),
    '#disabled' => (!count($options)),
    '#submit' => array('biblio_admin_orphans_form_submit'),
  );
  $form['delete_all'] = array(
    '#type' => 'submit',
    '#value' => t('Delete ALL'),
    '#disabled' => (!count($options)),
    '#submit' => array('biblio_admin_orphans_form_submit'),
  );
  $form['authors'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No orphaned authors.'),
  );

  return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_orphans_form_validate($form, &$form_state) {
  $check_count = array_filter($form_state['values']['authors']);
  if ($form_state['triggering_element']['#value'] == t('Delete Selected') && count($check_count) == 0) {
    form_set_error('', t('No items selected.'));
  }
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_orphans_form_submit($form, &$form_state) {
  $authors = $names = array();
  // Filter out unchecked authors.
  if ($form_state['triggering_element']['#value'] == t('Delete Selected')) {
    $authors = array_filter($form_state['values']['authors']);
  }
  elseif ($form_state['triggering_element']['#value'] == t('Delete ALL')) {
    $authors = drupal_map_assoc(array_keys($form_state['values']['authors']));
  }

  $names     = array_intersect_key($form_state['values']['names'], $authors);
  $del_names = implode('; ', $names);

  db_delete('biblio_contributor_data')
    ->condition('cid', $authors, 'IN')
    ->execute();

  drupal_set_message(t('The orphaned authors (@author_list) have been deleted.', array('@author_list' => $del_names)));

}

/**
 * @param $form
 * @param $form_state
 *
 * @return multitypeNULL
 */
function biblio_admin_keyword_orphans_form($form, $form_state) {
  $orphans = $keywords = $options = array();

  $base = variable_get('biblio_base', 'biblio');

  $header = array(
    'keyword' => t('Keyword'),
  );

  $query = db_select('biblio_keyword', 'bk');
  $active_kids = $query
    ->fields('bk', array('kid'))
    ->groupBy('kid')
    ->execute()
    ->fetchCol();

  $query = db_select('biblio_keyword_data', 'bkd');
  $all_kids = $query
    ->fields('bkd', array('kid'))
    ->groupBy('kid')
    ->execute()
    ->fetchCol();

  $orphans = array_diff($all_kids, $active_kids);

  if (count($orphans)) {
    $query = db_select('biblio_keyword_data', 'bkd')->extend('PagerDefault')->limit(50);
    // SELECT * FROM {biblio_contributor_data}.
    $query->fields('bkd');
    $query->condition('kid', $orphans, 'IN');
    // pager_query('SELECT * FROM {biblio_keyword_data} WHERE kid NOT IN (SELECT kid FROM {biblio_keyword} GROUP BY kid)', 50);.
    $result = $query->execute();
    foreach ($result as $keyword) {
      $options[$keyword->kid] = array(
        'keyword' => array(
          'data' => array(
            '#type' => 'link',
            '#title' => $keyword->word,
            '#href' => $base . '/keywords/' . $keyword->kid . '/edit',
          ),
        ),
      );
    }
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#disabled' => (!count($options)),
    '#submit' => array('biblio_admin_keyword_orphans_form_submit'),
  );
  $form['words'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No orphaned keywords.'),
  );

  $form['pager'] = array('#markup' => theme('pager'));

  return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_keyword_orphans_form_validate($form, &$form_state) {
  $check_count = array_filter($form_state['values']['words']);
  if (count($check_count) == 0) {
    form_set_error('', t('No items selected.'));
  }
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_keyword_orphans_form_submit($form, &$form_state) {

  $keywords = array_filter($form_state['values']['words']);
  db_delete('biblio_keyword_data')
    ->condition('kid', $keywords, 'IN')
    ->execute();

  drupal_set_message(t('%count orphaned keywords have been deleted.', array('%count' => count($keywords))));

}

/**
 * @param $form
 * @param $form_state
 * @param $keyword_id
 *
 * @return multitype:string number NULL
 */
function biblio_admin_keyword_edit_form($form, &$form_state, $keyword_id) {
  $options = array();
  $keywords = array();
  $destination = NULL;

  $keyword = db_query('SELECT * FROM {biblio_keyword_data} bkd WHERE bkd.kid = :kid ', array(':kid' => $keyword_id))->fetchObject();

  $base = variable_get('biblio_base', 'biblio');
  // $path = (strpos($_GET['q'], 'config')) ? 'admin/config/content/biblio/keywords' : $base . '/keywords';.
  if (isset($_GET['destination'])) {
    $destination = $_GET['destination'];
  }
  if (!$destination && isset($form_state['complete form']['#action'])) {
    $action = drupal_parse_url($form_state['complete form']['#action']);
    $destination = isset($action['query']['destination']) ? $action['query']['destination'] : NULL;
  }
  if (!$destination) {
    $destination = $base . '/keywords';
  }

  $form_state['redirect'] = $destination;

  $form['kid'] = array(
    '#type' => 'value',
    '#value' => $keyword_id,
  );
  $form['word'] = array(
    '#type' => 'textfield',
    '#title' => t('Keyword'),
    '#default_value' => $keyword->word,
    '#size' => 100,
    '#weight' => -10,
    '#required' => TRUE,
    '#maxlength' => 255,
  );

  $result = db_query("SELECT kd.word, kd.kid, count(*) as use_count FROM {biblio_keyword_data} kd
                      LEFT JOIN {biblio_keyword} bk on bk.kid = kd.kid
                      WHERE LOWER(word) LIKE LOWER(:word)
                      AND kd.kid <> :kid
                      GROUP BY kd.kid, kd.word", array(':word' => '%%' . drupal_substr($keyword->word, 0, 5) . '%%', ':kid' => $keyword_id));

  foreach ($result as $keyword) {
    $keywords[] = $keyword;
  }

  if (isset($form_state['biblio_add_merge_keywords'])) {
    $keywords = array_merge($keywords, $form_state['biblio_add_merge_keywords']);
  }

  foreach ($keywords as $keyword) {
    $options[$keyword->kid] = array(
      'keyword' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $keyword->word . ' (' . $keyword->use_count . ')',
          '#href' => $destination . '/' . $keyword->kid . '/edit',
          '#options' => array('query' => array('destination' => $destination)),
        ),
      ),
    );
  }

  $form['merge'] = array(
    '#type' => 'fieldset',
    '#title' => t('Keyword Merge'),
    '#description' => t('If you wish to consolodate references to multiple keywords into single reference to: <b><u>!kw</u></b>, select the others from the list below. This will remove the selected keywords from the database and replace references to them with a reference to: <b><u>!kw</u></b>. You should only do this if you are sure the other keywords represent the same keyword as the one being edited.', array('!kw' => $keyword->word)),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
      // '#theme' => 'biblio_admin_keyword_merge_fieldset'.
  );

  $header = array('keyword' => t('Similar keywords '));

  $form['merge']['merge_words'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No similar keywords automatically detected, use the search box below to manually add others.'),
    '#prefix' => '<div id="biblio-keyword-merge-table">',
    '#suffix' => '</div>',

  );
  $form['merge']['search'] = array(
    '#type' => 'textfield',
    '#title' => t('Keyword search'),
    '#autocomplete_path' => 'biblio/autocomplete/biblio_keywords',
    '#description' => t('Use this field to find other keywords you would like to merge with: <b><u>!kw</u></b>, then click the "Add to merge list" button. (The merge will not happen until the "Save" button is clicked)', array('!kw' => $keyword->word)),
  );
  $form['merge']['add'] = array(
    '#type'   => 'submit',
    '#value'  => t('Add to merge list'),
    '#submit' => array('biblio_add_merge_keyword'),
    '#ajax'   => array(
      'callback' => 'biblio_add_merge_keyword_callback',
      'wrapper'  => 'biblio-keyword-merge-table',
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 6,
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#weight' => 7,
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#weight' => 8,
  );

  return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_add_merge_keyword_callback($form, &$form_state) {
  return $form['merge']['merge_words'];
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_add_merge_keyword($form, &$form_state) {

  if (!empty($form_state['values']['search'])) {
    module_load_include('inc', 'biblio', 'includes/biblio.keywords');
    if ($word = biblio_get_keyword_by_name($form_state['values']['search'])) {
      // We don't want to merge with ourselves :-(.
      if ($word->kid == $form_state['values']['kid']) {
        drupal_set_message(t('You cannot merge a keyword with itself'), 'error');
      }
      elseif (isset($form_state['biblio_add_merge_keywords'][$word->kid])) {
        drupal_set_message(t('Keyword already exists in the merge list'), 'warning');
      }
      else {
        $form_state['biblio_add_merge_keywords'][$word->kid] = $word;
      }
    }
    else {
      drupal_set_message(t('The keyword was not found in the database.  You must select a keyword from the "auto-complete" list of the Keyword search box.'), 'error');
    }
  }
  else {
    drupal_set_message(t('Cannot add an empty value'), 'error');
  }

  $form_state['rebuild'] = TRUE;
}

/**
 * @param $form
 * @param $form_state
 * @param $keyword_id
 */
function biblio_admin_keyword_delete_confirm($form, &$form_state, $keyword_id) {
  $base = variable_get('biblio_base', 'biblio');
  $keyword = db_query('SELECT * FROM {biblio_keyword_data} bkd WHERE bkd.kid = :kid ', array(':kid' => $keyword_id))->fetchObject();
  $form['kid'] = array(
    '#type' => 'value',
    '#value' => $keyword_id,
  );

  return confirm_form($form,
    t('Are you sure you want to delete the keyword: %word from ALL publications?', array('%word' => $keyword->word)),
    isset($_GET['destination']) ? $_GET['destination'] : $base . '/keywords',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );

}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_keyword_delete_confirm_submit($form, &$form_state) {
  $base = variable_get('biblio_base', 'biblio');
  module_load_include('inc', 'biblio', 'includes/biblio.keywords');
  biblio_delete_keyword($form_state['values']['kid']);
  $form_state['redirect'] = $base . '/keywords';
}

/**
 * @param $form
 * @param $form_state
 */
function biblio_admin_keyword_edit_form_submit($form, &$form_state) {
  switch ($form_state['values']['op']) {

    case t('Save'):
      drupal_write_record('biblio_keyword_data', $form_state['values'], 'kid');
      if (isset($form_state['values']['merge_words'])) {
        $kids = array_filter($form_state['values']['merge_words']);
        if (count($kids)) {
          db_update('biblio_keyword')
            ->fields(array('kid' => $form_state['values']['kid']))
            ->condition('kid', $kids, 'IN')
            ->execute();

          db_delete('biblio_keyword_data')
            ->condition('kid', $kids, 'IN')
            ->execute();
        }
      }
      break;

    case t('Delete'):
      $base = variable_get('biblio_base', 'biblio');
      unset($_GET['destination']);
      $form_state['redirect'] = "$base/keyword/" . $form_state['values']['kid'] . '/delete';
      break;

    case t('Cancel'):
      $base = variable_get('biblio_base', 'biblio');
      $form_state['redirect'] = $base . '/keywords';
      break;
  }
}
