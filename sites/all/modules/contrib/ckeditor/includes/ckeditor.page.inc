<?php

/**
 * CKEditor - The text editor for the Internet - http://ckeditor.com
 * Copyright (c) 2003-2012, CKSource - Frederico Knabben. All rights reserved.
 *
 * == BEGIN LICENSE ==
 *
 * Licensed under the terms of any of the following licenses of your
 * choice:
 *
 *  - GNU General Public License Version 2 or later (the "GPL")
 *    http://www.gnu.org/licenses/gpl.html
 *
 *  - GNU Lesser General Public License Version 2.1 or later (the "LGPL")
 *    http://www.gnu.org/licenses/lgpl.html
 *
 *  - Mozilla Public License Version 1.1 or later (the "MPL")
 *    http://www.mozilla.org/MPL/MPL-1.1.html
 *
 * == END LICENSE ==
 *
 * @file
 * CKEditor Module for Drupal 7.x
 *
 * This module allows Drupal to replace textarea fields with CKEditor.
 *
 * CKEditor is an online rich text editor that can be embedded inside web pages.
 * It is a WYSIWYG (What You See Is What You Get) editor which means that the
 * text edited in it looks as similar as possible to the results end users will
 * see after the document gets published. It brings to the Web popular editing
 * features found in desktop word processors such as Microsoft Word and
 * OpenOffice.org Writer. CKEditor is truly lightweight and does not require any
 * kind of installation on the client computer.
 */
function ckeditor_help_delegate($path, $arg) {
    global $base_url;
    switch ($path) {
        case 'admin/config/content/help#description':
            $output = t('Enables the use of CKEditor (a rich text WYSIWYG editor) instead of plain text fields.');
            break;

        case 'admin/config/content/ckeditor/edit/%':
        case 'admin/config/content/ckeditor/add':
            $output = '<p>' . t('CKEditor is highly configurable. The most commonly used features are listed below. You can also adjust CKEditor to your needs by changing the <code>%ckeditor_module_config</code> configuration file.',
                            array(
                                '%ckeditor_module_config' => drupal_get_path('module', 'ckeditor') . '/ckeditor.config.js',
                    )) . '</p>';
            $output .= '<p>' . t('It is recommended to not edit the <code>%ckeditor_config_file</code> (<code>%ckeditor_config_path</code>) configuration file that is distributed with CKEditor, because you may overwrite it accidentally when you update the editor.',
                            array(
                                '%ckeditor_config_path' => ckeditor_path(true) . '/config.js',
                                '%ckeditor_config_file' => 'config.js',
                    )) . '</p>';
            break;

        case 'admin/config/content/ckeditor/editg':
        case 'admin/config/content/ckeditor/add':
            $output = '<p>' . t('The Global Profile allows you to define settings that are common for all profiles. Values defined in other profiles will be appended to the global configuration. This way you can avoid repeating some of the settings that are usually the same for each profile.') . '</p>';
            break;

        case 'admin/config/content/ckeditor':
            $output =
                    '<div style="padding:10px;border:1px solid #BEBFB9;margin-bottom:10px;"><p>' . t('The CKEditor module allows Drupal to replace textarea fields with CKEditor. CKEditor is an online rich text editor that can be embedded inside web pages. It is a !wysiwyg editor which means that the text edited in it looks as similar as possible to the results end users will see after the document gets published. It brings to the Web popular editing features found in desktop word processors such as Microsoft Word and OpenOffice.org Writer. CKEditor is truly lightweight and does not require any kind of installation on the client computer.',
                            array(
                                '!wysiwyg' => '<acronym title="' . t('What You See Is What You Get') . '">' . t('WYSIWYG') . '</acronym>',
                    )) . '</p>' .
                    '<p>' . t('Useful links: !ckeditorlink | !devguidelink | !userguidelink.',
                            array(
                                '!ckeditorlink' => l(t('CKEditor website'), 'http://ckeditor.com'),
                                '!devguidelink' => l(t('CKEditor for Drupal Documentation'), 'http://docs.cksource.com/CKEditor_for_Drupal'),
                                '!userguidelink' => l(t('User\'s Guide'), 'http://docs.cksource.com/CKEditor_3.x/Users_Guide')
                    )) . '</p></div>' .
                    '<p>' . t('Profiles are linked with input format types. A CKEditor profile defines which buttons are available in the editor, how the editor is displayed, and a few other editor functions. The Global Profile stores some general information about CKEditor.') . '</p>';
            break;

        case 'admin/help#ckeditor':
            $output = '<h3>' . t('Introduction') . '</h3>';
            $output .= '<p>' . t('The CKEditor module allows Drupal to replace textarea fields with CKEditor. CKEditor is an online rich text editor that can be embedded inside web pages. It is a !wysiwyg editor which means that the text edited in it looks as similar as possible to the results end users will see after the document gets published. It brings to the Web popular editing features found in desktop word processors such as Microsoft Word and OpenOffice.org Writer. CKEditor is truly lightweight and does not require any kind of installation on the client computer.',
                            array(
                                '!wysiwyg' => '<acronym title="' . t('What You See Is What You Get') . '">' . t('WYSIWYG') . '</acronym>',
                    )) . '</p>';
            $output .= '<p>' . t('Useful links: !ckeditorlink | !devguidelink | !userguidelink.',
                            array(
                                '!ckeditorlink' => l(t('CKEditor website'), 'http://ckeditor.com'),
                                '!devguidelink' => l(t('CKEditor for Drupal Documentation'), 'http://docs.cksource.com/CKEditor_for_Drupal'),
                                '!userguidelink' => l(t('User\'s Guide'), 'http://docs.cksource.com/CKEditor_3.x/Users_Guide')
                                )) . '</p>';
            $output .= '<h4>' . t('Configuration') . '</h4>';
            $output .= '<ol>';
            $output .= '<li>' . t('CKEditor profiles can be configured in the <strong>!adminpath</strong> section. Profiles determine which options are available to users based on the input format system.',
                            array(
                                '!adminpath' => l(t('Administration panel') . ' > ' . t('Configuration') . ' > ' . t('Content Authoring') . ' > ' . t('CKEditor'), 'admin/config/content/ckeditor'),
                    )) . '</li>';
            $output .= '<li>' . t('For the Rich Text Editing to work you also need to configure your !filterlink for the users that may access Rich Text Editing. Either grant those users <strong>Full HTML</strong> access or use the following list of tags in the HTML filter:',
                            array(
                                '!filterlink' => l(t('filters'), 'admin/config/content/formats'),
                    )) .
                   '<br /><code>' . htmlspecialchars('<a> <p> <span> <div> <h1> <h2> <h3> <h4> <h5> <h6> <img> <map> <area> <hr> <br> <br /> <ul> <ol> <li> <dl> <dt> <dd> <table> <tbody> <th> <tr> <td> <em> <b> <u> <i> <strong> <del> <ins> <sub> <sup> <quote> <blockquote> <pre> <address> <code> <cite> <embed> <object> <param> <strike> <caption> <iframe>') . '</code><br />';
            $output .= t('<strong>Note:</strong> be careful when granting users access to create tags like %iframe.',
                            array(
                               '%iframe' => '<iframe>',
                    )) . '</li>';
            $output .= '<li>' . t('To have better control over line breaks, you should disable the <strong>%settingname</strong> setting in the chosen Text format (recommended).',
                            array(
                                '%settingname' => t('Line break converter'),
                    )) . '</li>';
            $output .= '<li>' . t('All configuration options described in the !apidocs that cannot be easily changed in the administration area can be set in the <strong>Advanced Options</strong> section in the CKEditor profile.',
                            array(
                                '!apidocs' => l(t('API documentation'), 'http://docs.cksource.com/ckeditor_api/symbols/CKEDITOR.config.html')
                    )) . '</li>';
            $output .= '</ol>';

            $output .= '<h3>' . t('Troubleshooting') . '</h3>';
            $output .= '<p>';
            $output .= t('Take a look at !listlink when installing CKEditor.',
                            array(
                                '!listlink' => l(t('the list of common problems'), 'http://docs.cksource.com/CKEditor_for_Drupal/Troubleshooting')
                    ));
            $output .= ' ' . t('If you are looking for more information, have any trouble with the configuration, or found an issue with the CKEditor module, please visit the !officiallink.', array('!officiallink' => l(t('official project page'), 'http://drupal.org/project/ckeditor')));
            $output .= ' ' . t('More information about how to customize CKEditor for your theme can be found !herelink.', array('!herelink' => l(t('here'), 'http://drupal.ckeditor.com/tricks')));
            $output .= '</p>';

            break;
    }
    return!empty($output) ? $output : '';
}

/**
 * AJAX callback - XSS filter
 */
function ckeditor_filter_xss() {
    header('Content-Type: text/html; charset=utf-8');
    $GLOBALS['devel_shutdown'] = FALSE;

    if (!isset($_POST['text']) || !is_string($_POST['text']) || !isset($_POST['input_format']) || !is_string($_POST['input_format']) || !isset($_POST['token']) || !drupal_valid_token($_POST['token'], 'ckeditorAjaxCall', TRUE)) {
      exit;
    }

    $format = filter_format_load($_POST['input_format']);
    if ($format == FALSE || !is_object($format) || !filter_access($format)) {
        exit;
    }

    module_load_include('inc', 'ckeditor', 'includes/ckeditor.lib');
    $profile = ckeditor_get_profile($_POST['input_format']);

    if ($profile == FALSE) {
        $settings = array();
        $settings['filters'] = ckeditor_security_filters();
    }
    else {
        $settings = $profile->settings;
    }

    $text = $_POST['text'];
    $filters = filter_get_filters();
    $format_filters = filter_list_format($_POST['input_format']);
    foreach ((array) $settings['filters'] as $name => $status) {
        //If filter not exists, cannot be called or isn't enabled in selected text format then skip this filter
        if (!isset($filters[$name]) || !isset($filters[$name]['process callback']) || !array_key_exists($name, $format_filters) || $format_filters[$name]->status == 0) {
          continue;
        }

        //Call default CKEditor built-in filter
        if ($name == "filter_html" && $status == 1) {
            preg_match_all("|</?([a-z][a-z0-9]*)(?:\b[^>]*)>|i", $text, $matches);
            if ($matches[1]) {
                $tags = array_unique($matches[1]);
                $text = filter_xss($text, $tags);
            }
            continue;
        }

        $text = $filters[$name]['process callback']($text, $format_filters[$name], $_POST['input_format']);
    }

    echo $text;
}
