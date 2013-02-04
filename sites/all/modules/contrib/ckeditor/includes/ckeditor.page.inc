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
      $output = '<p>' . t('CKEditor is highly configurable. The most commonly used features are listed below. You can also adjust CKEditor to your needs by changing the !ckeditor_module_config configuration file.', array(
            '!ckeditor_module_config' => '<code>' . ckeditor_module_path('local') . '/ckeditor.config.js</code>',
              )
          ) .
          '</p><p>' .
          t('It is recommended to not edit the !ckeditor_config_file (!ckeditor_config_path) configuration file that is distributed with CKEditor, because you may overwrite it accidentally when you update the editor.', array(
            '!ckeditor_config_path' => '<code>' . ckeditor_path('local') . '/config.js</code>',
            '!ckeditor_config_file' => '<code>config.js</code>',
              )
          ) .
          '</p>';
      break;

    case 'admin/config/content/ckeditor/editg':
    case 'admin/config/content/ckeditor/add':
      $output = '<p>' . t('The Global Profile allows you to define settings that are common for all profiles. Values defined in other profiles will be appended to the global configuration. This way you can avoid repeating some of the settings that are usually the same for each profile.') . '</p>';
      break;

    case 'admin/config/content/ckeditor':
      $output =
          '<div style="padding:10px;border:1px solid #BEBFB9;margin-bottom:10px;"><p>' .
          t('The CKEditor module allows Drupal to replace textarea fields with CKEditor. CKEditor is an online rich text editor that can be embedded inside web pages. It is a !wysiwyg editor which means that the text edited in it looks as similar as possible to the results end users will see after the document gets published. It brings to the Web popular editing features found in desktop word processors such as Microsoft Word and OpenOffice.org Writer. CKEditor is truly lightweight and does not require any kind of installation on the client computer.', array(
            '!wysiwyg' => '<acronym title="' . t('What You See Is What You Get') . '">' . t('WYSIWYG') . '</acronym>',
              )
          ) .
          '</p><p>' .
          t('Useful links: !ckeditorlink | !devguidelink | !userguidelink.', array(
            '!ckeditorlink' => l(t('CKEditor website'), 'http://ckeditor.com'),
            '!devguidelink' => l(t('Developer\'s Guide'), 'http://docs.cksource.com/CKEditor_3.x/Developers_Guide'),
            '!userguidelink' => l(t('User\'s Guide'), 'http://docs.cksource.com/CKEditor_3.x/Users_Guide')
              )
          ) .
          '</p></div><p>' .
          t('Profiles are linked with input format types. A CKEditor profile defines which buttons are available in the editor, how the editor is displayed, and a few other editor functions. The Global Profile stores some general information about CKEditor.') . '</p>';
      break;

    case 'admin/help#ckeditor':
      $output =
          '<h3>' . t('Introduction') . '</h3>' .
          '<p>' .
          t('The CKEditor module allows Drupal to replace textarea fields with CKEditor. CKEditor is an online rich text editor that can be embedded inside web pages. It is a !wysiwyg editor which means that the text edited in it looks as similar as possible to the results end users will see after the document gets published. It brings to the Web popular editing features found in desktop word processors such as Microsoft Word and OpenOffice.org Writer. CKEditor is truly lightweight and does not require any kind of installation on the client computer.', array(
            '!wysiwyg' => '<acronym title="' . t('What You See Is What You Get') . '">' . t('WYSIWYG') . '</acronym>',
              )
          ) .
          '</p>' .
          '<p>' .
          t('Useful links: !ckeditorlink | !devguidelink | !userguidelink.', array(
            '!ckeditorlink' => l(t('CKEditor website'), 'http://ckeditor.com'),
            '!devguidelink' => l(t('Developer\'s Guide'), 'http://docs.cksource.com/CKEditor_3.x/Developers_Guide'),
            '!userguidelink' => l(t('User\'s Guide'), 'http://docs.cksource.com/CKEditor_3.x/Users_Guide')
              )
          ) .
          '</p>' .
          '<h4>' .
          t('Configuration') .
          '</h4>' .
          '<ol>' .
          '<li>' .
          t('CKEditor profiles can be configured in the !adminpath section. Profiles determine which options are available to users based on the input format system.', array(
            '!adminpath' => '<strong>' . l(t('Administration panel') . ' > ' . t('Configuration') . ' > ' . t('Content Authoring') . ' > ' . t('CKEditor'), 'admin/config/content/ckeditor') . '</strong>'
              )
          ) .
          '</li>' .
          '<li>' .
          t('For the Rich Text Editing to work you also need to configure your !filterlink for the users that may access Rich Text Editing. Either grant those users <strong>Full HTML</strong> access or use the following list of tags in the HTML filter:', array(
            '!filterlink' => l(t('filters'), 'admin/config/content/formats')
              )
          ) .
          '<br /><code>' .
          htmlspecialchars('<a> <p> <span> <div> <h1> <h2> <h3> <h4> <h5> <h6> <img> <map> <area> <hr> <br> <br /> <ul> <ol> <li> <dl> <dt> <dd> <table> <tbody> <th> <tr> <td> <em> <b> <u> <i> <strong> <del> <ins> <sub> <sup> <quote> <blockquote> <pre> <address> <code> <cite> <embed> <object> <param> <strike> <caption> <iframe>') .
          '</code><br />' .
          t('<strong>Note:</strong> be careful when granting users access to create tags like %iframe.<br />If you are going to use CKEditor with the <strong>Filtered HTML</strong> input format, please read the "Setting up filters" section in the !readme file.', array(
            '%iframe' => '<iframe>',
            '!readme' => '<code>' . l(t('README.txt'), $base_url . '/' . drupal_get_path('module', 'ckeditor') . '/README.txt', array('absolute' => TRUE)) . '</code>'
              )
          ) .
          '</li>' .
          '<li>' .
          t('To have better control over line breaks, you should disable the <strong>%settingname</strong> setting in the chosen Text format (recommended).', array(
            '%settingname' => t('Line break converter'),
              )
          ) .
          '</li>' .
          '<li>' .
          t('All configuration options described in the !apidocs that cannot be easily changed in the administration area can be set in the <strong>Advanced Options</strong> section in the CKEditor profile.', array(
            '!apidocs' => l(t('API documentation'), 'http://docs.cksource.com/ckeditor_api/symbols/CKEDITOR.config.html')
              )
          ) .
          '</li>' .
          '</ol>' .
          '<h3>' .
          t('Troubleshooting') .
          '</h3>' .
          '<p>' .
          t('Take a look at !listlink when installing CKEditor.', array(
            '!listlink' => l(t('the list of common problems'), 'http://docs.cksource.com/CKEditor_for_Drupal/Troubleshooting')
              )
          ) .
          ' ' .
          t('If you are looking for more information, have any trouble with the configuration, or found an issue with the CKEditor module, please visit the !officiallink.', array(
            '!officiallink' => l(t('official project page'), 'http://drupal.org/project/ckeditor')
              )
          ) .
          ' ' .
          t('More information about how to customize CKEditor for your theme can be found !herelink.', array(
            '!herelink' => l(t('here'), 'http://drupal.ckeditor.com/tricks')
              )
          ) .
          '</p>' .
          '<h3>' .
          t('Uploading images and files') .
          '</h3>' .
          '<p>' .
          t('There are three ways for uploading files:') .
          '</p>' .
          '<ol>' .
          '<li>' .
          t('By using !ckfinder (commercial), an advanced Ajax file manager.', array(
            '!ckfinder' => l(t('CKFinder'), 'http://ckfinder.com'),
              )
          ) .
          '</li>' .
          '<li>' .
          t('By using a dedicated module like !imcelink.', array(
            '!imcelink' => l(t('IMCE'), 'http://drupal.org/project/imce')
              )
          ) .
          '</li>' .
          '<li>' .
          t('By using the core upload module.') .
          '</li>' .
          '</ol>';

      break;
  }
  return !empty($output) ? $output : '';
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

  $text = $_POST['text'];
  $filters = filter_get_filters();
  $format_filters = filter_list_format($_POST['input_format']);
  $security_filters = ckeditor_security_filters();
  $cache_id = $_POST['input_format'] . ':' . '' . ':' . hash('sha256', $text);

  foreach ((array) $format_filters as $name => $object) {
    //If filter is not security filter, not exists, cannot be called or isn't enabled in selected text format then skip this filter
    if (!isset($security_filters['filters'][$name]) || !isset($filters[$name]) || !isset($filters[$name]['process callback']) || $object->status == 0) {
      continue;
    }

    //Call default CKEditor built-in filter
    if ($name == 'filter_html' && $security_filters['filters']['filter_html'] == 1) {
      preg_match_all("|</?([a-z][a-z0-9]*)(?:\b[^>]*)>|i", $text, $matches);
      if ($matches[1]) {
        $tags = array_unique($matches[1]);
        $text = filter_xss($text, $tags);
      }
      continue;
    }
    $text = $filters[$name]['process callback']($text, $format_filters[$name], $format, '', TRUE, $cache_id);
  }

  echo $text;
}
