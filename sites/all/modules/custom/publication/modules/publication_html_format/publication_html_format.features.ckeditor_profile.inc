<?php

/**
 * @file
 * pensoft_publication.features.ckeditor_profile.inc
 */
/**
 * Implements hook_ckeditor_profile_defaults().
 */
function publication_html_format_ckeditor_profile_defaults(){
  $data = array(
    'Publication' => array(
      'name' => 'Publication',
      'settings' => array(
        'filters' => array(
          'filter_html' => 0
        ),
        'ss' => '2',
        'default' => 't',
        'show_toggle' => 't',
        'skin' => 'kama',
        'uicolor' => 'default',
        'uicolor_textarea' => '<p>
	Click the <strong>UI Color Picker</strong> button to set your color preferences.</p>
',
        'uicolor_user' => 'default',
        'toolbar' => '[
    [\'Bold\',\'Italic\',\'Underline\'],
    [\'JustifyLeft\',\'JustifyCenter\',\'JustifyRight\',\'JustifyBlock\'],
    [\'NumberedList\',\'BulletedList\',\'-\',\'Subscript\',\'Superscript\',\'Blockquote\'],
    [\'insert_from_view_pensoft_publication_references\',\'insert_from_view_pensoft_publication_figures\',\'pensoft_publication_insert_table\'],
    [\'insert_from_view_pensoft_publication_specimens\'],
    \'/\',
    [\'SelectAll\',\'Cut\',\'Templates\',\'Paste\',\'PasteText\',\'PasteFromWord\'],
    [\'SpellChecker\',\'Scayt\'],
    [\'Undo\',\'Redo\',\'Find\',\'Replace\',\'-\',\'RemoveFormat\'],
    [\'Table\',\'HorizontalRule\',\'SpecialChar\'],
    [\'Maximize\']
]',
        'expand' => 't',
        'width' => '100%',
        'lang' => 'en',
        'auto_lang' => 't',
        'language_direction' => 'default',
        'enter_mode' => 'p',
        'shift_enter_mode' => 'br',
        'font_format' => 'p;div;pre;address;h1;h2;h3;h4;h5;h6',
        'custom_formatting' => 'f',
        'formatting' => array(
          'custom_formatting_options' => array(
            'indent' => 'indent',
            'breakBeforeOpen' => 'breakBeforeOpen',
            'breakAfterOpen' => 'breakAfterOpen',
            'breakAfterClose' => 'breakAfterClose',
            'breakBeforeClose' => 0,
            'pre_indent' => 0
          )
        ),
        'css_mode' => 'self',
        'css_path' => '%h' . drupal_get_path('module', 'publication_html_format') . '/css/publication_html_format_editor.css',
        'css_style' => 'theme',
        'styles_path' => '',
        'filebrowser' => 'none',
        'filebrowser_image' => '',
        'filebrowser_flash' => '',
        'UserFilesPath' => '%b%f/',
        'UserFilesAbsolutePath' => '%d%b%f/',
        'ckeditor_load_method' => 'ckeditor.js',
        'ckeditor_load_time_out' => '0',
        'forcePasteAsPlainText' => 'f',
        'html_entities' => 't',
        'scayt_autoStartup' => 't',
        'theme_config_js' => 'f',
        'js_conf' => '',
        'loadPlugins' => array(
          'autogrow' => array(
            'name' => 'autogrow',
            'desc' => 'Auto Grow plugin',
            'path' => '%editor_path%plugins/autogrow/',
            'buttons' => FALSE,
            'default' => 'f'
          ),
          'insert_from_view_pensoft_publication_specimens' => array(
            'name' => 'insert_from_view_pensoft_publication_specimens',
            'desc' => 'This plugin allows the selection and insertion of items picked from the view textarea_insert_select_specimen_table',
            'path' => '%base_path%sites/all/modules/custom/insert_from_view/plugins/insert_from_view/',
            'buttons' => array(
              'insert_from_view_pensoft_publication_specimens' => array(
                'icon' => '../../../../../../../sites/all/modules/custom/publication/modules/publication_html_format/images/specimens.png',
                'label' => 'Add specimen table'
              )
            )
          ),
          'insert_from_view_pensoft_publication_figures' => array(
            'name' => 'insert_from_view_pensoft_publication_figures',
            'desc' => 'This plugin allows the selection and insertion of items picked from the view publication_citations_figures',
            'path' => '%base_path%sites/all/modules/custom/insert_from_view/plugins/insert_from_view/',
            'buttons' => array(
              'insert_from_view_pensoft_publication_figures' => array(
                'icon' => '../../../../../../../sites/all/modules/custom/publication/modules/publication_html_format/images/figures.png',
                'label' => 'Add Figure citation'
              )
            )
          ),
          'insert_from_view_pensoft_publication_references' => array(
            'name' => 'insert_from_view_pensoft_publication_references',
            'desc' => 'This plugin allows the selection and insertion of items picked from the view publication_citations_references',
            'path' => '%base_path%sites/all/modules/custom/insert_from_view/plugins/insert_from_view/',
            'buttons' => array(
              'insert_from_view_pensoft_publication_references' => array(
                'icon' => '../../../../../../../sites/all/modules/custom/publication/modules/publication_html_format/images/references.png',
                'label' => 'Add Reference citation'
              )
            )
          ),
          'pensoft_publication_insert_table' => array(
            'name' => 'pensoft_publication_insert_table',
            'desc' => 'This plugin allows to insert references to tables in the entitie\'s field_publication_table field',
            'path' => '%base_path%sites/all/modules/custom/publication/modules/publication_html_format/plugins/insert_table/',
            'buttons' => array(
              'pensoft_publication_insert_table' => array(
                'icon' => '../../images/tables.png',
                'label' => 'Insert reference to tables'
              )
            )
          ),
          'pensoft_publication_reference_editor' => array(
            'name' => 'pensoft_publication_reference_editor',
            'desc' => 'This plugin allows editing the text of references',
            'path' => '%base_path%sites/all/modules/custom/publication/modules/publication_html_format/plugins/reference_editor/',
            'buttons' => FALSE,
          ),
          'tableresize' => array(
            'name' => 'tableresize',
            'desc' => 'Table Resize plugin',
            'path' => '%editor_path%plugins/tableresize/',
            'buttons' => FALSE,
            'default' => 't'
          )
        )
      ),
      'input_formats' => array(
        'publication_html' => 'Publication HTML'
      )
    )
  );
  return $data;
}