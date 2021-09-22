TABLE OF CONTENTS
-----------------
 * Overview
 * More Information and License
 * Requirements
 * Installation Paths
 * Installation / Configuration
 * Installation Troubleshooting
 * Uploading Images and Files
 * Installing CKFinder
 * Managing Plugins
 * Installing Additional Plugins
 * Integrating a Plugin with the CKEditor Module (for Plugin Developers)
 * Setting up Security Filters
 * HTML Filters and Inline Styling
 * Integrating a Custom Security Filter with the CKEditor Module (for Developers)
 * Upgrading Instructions
 * Help & Contribution
 * Credits

Overview
--------
This module allows Drupal to replace textarea fields with CKEditor.
CKEditor is an online rich text editor that can be embedded inside web pages.
It is a WYSIWYG (What You See Is What You Get) editor which means that the
text edited in it looks as similar as possible to the results end users will
see after the document gets published. It brings to the Web popular editing
features found in desktop word processors such as Microsoft Word and
OpenOffice.org Writer. CKEditor is truly lightweight and does not require any
kind of installation on the client computer.

More Information and License
----------------------------
CKEditor - The text editor for the Internet
Copyright (c) 2003-2012, CKSource - Frederico Knabben. All rights reserved.

Licensed under the terms of the GNU Lesser General Public License:
    http://www.opensource.org/licenses/lgpl-license.php

For further information visit:
    http://ckeditor.com/

Requirements
------------
  - Drupal 7.x,
  - PHP 5.2 or greater,
  - CKEditor 3.4 or greater.
    You will need to download CKEditor from the official download site: http://ckeditor.com/download.
    It is recommended to always use the latest CKEditor version available.

Installation Paths
------------------
It is recommended to install the CKEditor for Drupal module in the "sites/all/modules" directory.

When adding the files for standalone CKEditor and CKFinder, you can use one of the following directories:
- "sites/all/modules/ckeditor/"
- "sites/all/libraries/"
and create a "ckeditor" or "ckfinder" directory inside.

The CKEditor module will automatically recognize the proper path to the editor and the file browser.

All installation and configuration instructions in this README file assume that you use the first option and place the CKEditor and CKFinder files in the "sites/all/modules/ckeditor/" directory. If you want to use the "sites/all/libraries/" directory, you will need to adjust the paths given in the instructions accordingly.

Installation / Configuration
----------------------------
Note: these instructions assume that you install the CKEditor for Drupal module in the
      "sites/all/modules" directory (recommended).

   1. Unzip the module files to the "sites/all/modules" directory. It should now
      contain a "ckeditor" directory.
   2. Download standalone CKEditor from http://ckeditor.com/download. Unzip the
      contents of the "ckeditor" directory from the installation package to the
      "sites/all/modules/ckeditor/ckeditor" (or "sites/all/libraries/ckeditor") directory.
      Note: you can skip uploading the "_samples" and "_source" folders.
   3. Enable the module in the "Administration panel > Modules > User Interface" section.
   4. Grant permissions for using CKEditor in the
      "Administration panel > People > Permissions" section.
      Note: In order to enable the file browser, refer to the
            "Installing CKFinder" section.
   5. Adjust CKEditor profiles in the
      "Administration panel > Configuration > Content Authoring > CKEditor" section.
      Profiles determine which options are available to users based on the input format system.
   6. For the Rich Text Editing to work you also need to configure your filters
      for the users that may access Rich Text Editing.
      Either grant those users Full HTML access or use the following tags:
      <a> <p> <span> <div> <h1> <h2> <h3> <h4> <h5> <h6> <img> <map> <area> <hr>
      <br> <br /> <ul> <ol> <li> <dl> <dt> <dd> <table> <tr> <td> <em> <b> <u> <i> <strong>
      <del> <ins> <sub> <sup> <quote> <blockquote> <pre> <address> <code>
      <cite> <embed> <object> <param> <strike> <caption> <tbody>
      To make copying the list easier, below all tags were placed in one line:
      <a> <p> <span> <div> <h1> <h2> <h3> <h4> <h5> <h6> <img> <map> <area> <hr> <br> <br /> <ul> <ol> <li> <dl> <dt> <dd> <table> <tr> <td> <em> <b> <u> <i> <strong> <del> <ins> <sub> <sup> <quote> <blockquote> <pre> <address> <code> <cite> <embed> <object> <param> <strike> <caption> <tbody>
      If you are going to use CKEditor with the Filtered HTML input format,
      please refer to the "HTML Filters and Inline Styling" section.
   7. To have better control over line breaks, you may disable the line break converter
      for a given text format in the "Administration panel > Configuration > Content authoring > Text formats" section (recommended).
   8. Modify the ckeditor.config.js file to adjust it to your needs (optional).
      Configuration options are described here:
      http://docs.cksource.com/ckeditor_api/symbols/CKEDITOR.config.html
      Developer's documentation for CKEditor:
      http://docs.cksource.com/CKEditor_3.x/Developers_Guide
      WARNING: Remember to clear the browser cache after you have modified any of the JavaScript files.
      If you skip this step, you may notice that the browser is ignoring your changes.

Installation Troubleshooting
----------------------------
If CKEditor does not appear on the page, check if all files were extracted correctly.

The "sites/all/modules/ckeditor/ckeditor/" directory should contain the following files:
ckeditor.js, config.js, contents.css
and directories: "skins", "themes", "lang", "images".

The correct directory structure is as follows:
modules               <dir>
   ckeditor           <dir>
      ckeditor.module
      ckeditor.admin.inc
      ...
      ckeditor        <dir>
         _source      <dir>
         images       <dir>
         lang         <dir>
         plugins      <dir>
         skins        <dir>
         themes       <dir>
         COPY_HERE.txt
         ckeditor.js
         ...

As noted above, alternatively the "sites/all/libraries/ckeditor" directory can be used.
The "libraries" directory is the default path when drush is used to download the editor JavaScript code.

If you are still experiencing problems with your CKEditor installation, scroll down to the "Help & Contribution" section.

Uploading Images and Files
--------------------------
There are two methods for uploading files:
- by using a commercial file browser like CKFinder (http://ckfinder.com), an advanced Ajax file manager;
- by using modules like IMCE.

To select a preferred file browser, adjust CKEditor profiles in the
"Administration panel > Configuration > Content Authoring > CKEditor" section.
In the "File browser settings" section you can choose which file browser will be used for each profile.
Note: in order to choose an upload module other than CKFinder, you should install an appropriate Drupal module first.

Installing CKFinder
-------------------
CKFinder is an Ajax-based file manager created by CKEditor developers: http://ckfinder.com/.

   1. Download CKFinder for PHP: http://ckfinder.com/download
   2. Unpack CKFinder to the directory containing the CKEditor module and place it in the
      "sites/all/modules/ckeditor/ckfinder" (or "sites/all/libraries/ckfinder") folder.
      The correct directory structure is as follows:

      modules               <dir>
         ckeditor           <dir>
            ckeditor.module
            ckeditor.admin.inc
            ...
            ckeditor        <dir>
               _source      <dir>
               images       <dir>
               ckeditor.js
               ...
            ckfinder        <dir>
               core         <dir>
               ckfinder.php
               config.php
               ...

   3. Grant the "CKFinder access" permission in the "Administration panel > People > Permissions" section.
      Note: if you do not see this permission, it means that CKEditor did not find CKFinder
      and you have probably uploaded CKFinder into a wrong directory.
   4. Open the CKFinder configuration file (ckfinder/config.php) and do the following:

      I) Remove the CheckAuthentication() function:
        (do not worry, this function is defined in filemanager.config.php, see below)

        function CheckAuthentication()       <- remove it
        {                                    <- remove it
           //WARNING : DO NOT simply...      <- remove it
           ...                               <- remove it
           return false;                     <- remove it
        }                                    <- remove it

      II) Add:

        - for CKFinder installed in the "sites/all/modules/ckeditor/ckfinder" directory:
        require_once '../../../../includes/filemanager.config.php';

        - for CKFinder installed in the "sites/all/libraries/ckfinder" directory:
        require_once '../../../../../modules/ckeditor/includes/filemanager.config.php';

        straight below the following line:

        $baseDir = resolveUrl($baseUrl);

   5. Open the Drupal settings file (sites/default/settings.php) and do the following:

      I) Uncomment the $base_url variable and set the base URL of your website (without the trailing slash).

      II) Uncomment the $cookie_domain variable and set the domain name of your website.

   6. Select CKFinder as a preferred file browser in the
      "Administration panel > Configuration > Content Authoring > CKEditor" section
      (for a selected CKEditor profile scroll down to the "File browser settings" section).
      In the "File browser settings" section you may also change destination folders for files uploaded with CKFinder.

Managing Plugins
----------------
If you want to manage CKEditor plugins for a profile, go to the "Administration panel > Configuration > Content Authoring > CKEditor" section. This section lets you choose plugins relevant for each CKEditor profile from a list.
In order to activate a plugin, go to the "Editor appearance > Plugins" section and select the checkbox next to a required plugin name.

If a plugin contains toolbar buttons, you will need to drag and drop them to an appropriate toolbar position by using the toolbar wizard. If this is the case, the button should be added to the CKEditor toolbar by using the method described below:
- Enter the "Editor appearance > Toolbar" section.
- Check whether the plugin button that you want to use is present in the "Used buttons" sections. If not, find it in the "All buttons" section and drag and drop to the toolbar configuration from the "Used buttons" section.

Alternatively, if you turned off the toolbar wizard and prefer to enter the toolbar configuration manually, you will need to add the plugin button by yourself by appending it to your toolbar definition code.

Please note that some plugins require installing additional modules to work correctly.

Installing Additional Plugins
-----------------------------
The installation process is based on placing the plugin folder in the "plugins" directory of the CKEditor module (usually "sites/all/modules/ckeditor").
The plugin folder should contain at least the plugin.js file that is responsible for the plugin logic.
The plugin description will be displayed in the "Administration panel" if it is added to the plugin.js file by using the following special comment:
/**
 * @file Plugin description
 */
Hint: The "Administration panel" automatically detects the toolbar buttons available in the plugin and adds them to the toolbar wizard.

A plugin can be enabled by using the same method as described above - see the "Managing Plugins" section.

Integrating a Plugin with the CKEditor Module (for Plugin Developers)
---------------------------------------------------------------------
Integrating your application with the CKEditor module by adding a plugin works through a special hook.
An example of the hook is shown below:

function MODULENAME_ckeditor_plugin() {
  return array(
        'plugin_name' => array(
            // Plugin name.
            'name' => 'plugin_name',
            // Plugin description - it will be displayed in the plugins management section of the profile settings.
            'desc' => t('Plugin description'),
            // The full path to the CKEditor plugin directory, trailing slash included.
            'path' => drupal_get_path('module', 'my_module') . '/plugin_dir/',
            // Plugin buttons definition (optional).
            'buttons' => array(
              'button_name' => array('label' => 'Button label', 'icon' => '/path/to/icon/image'),
              'button_name' => array('label' => 'Button label', 'icon' => '/path/to/icon/image'),
              ...
            )
        )
    );
}
Please note that MODULENAME in the code above is the name of the module.

After the hook is used the plugin will automatically appear on the plugin list for each CKEditor profile where you will be able to enable it as described in the "Managing Plugins" section.

Setting up Security Filters
---------------------------
The CKEditor security system protects you from executing malicious code that is already in your database. In plain textareas database content is harmless because it is not executed, but a WYSIWYG editor interprets HTML like a Web browser and thus the content needs to be filtered before it is loaded.

In order to configure the security filters, go to the "Administration panel > Configuration > Content Authoring > CKEditor" section. Enter the profile configuration and go to the "Security" section.

The "Security" section lists all the security filters that are currently supported by the CKEditor for Drupal module along with their status for each text format.

The CKEditor for Drupal module has built-in support for some popular security filter modules which you will need to download and install by yourself first. Visit the official websites for each module in order to get the files and find installation and configuration instructions.

When a filter module is installed, you will be able to configure its security filters and enable for a given text format. The list of active text formats is displayed in the "Security" section along with the links that will take you to the "Administration panel > Configuration > Content authoring > Text formats" section where you will be able to configure the filters for each of the text formats. The filters will then be run on the content during the filtering process.

The "Security Settings" option in the "Security" section lets you choose whether to always run the security filters on CKEditor content (recommended and default option) or run them only when CKEditor is set to start automatically. If you change this setting to only run the filters when CKEditor starts automatically, you will not be protected when toggling manually between a plain textarea and the WYSIWYG editor.

The following security filter modules are currently supported:
 - HTML Purifier - http://drupal.org/project/htmlpurifier
 - htmLawed - http://drupal.org/project/htmLawed
 - Htmltidy - http://drupal.org/project/htmltidy
 - WYSIWYG Filter - http://drupal.org/project/wysiwyg_filter

HTML Filters and Inline Styling
-------------------------------
In the "Administration panel > Configuration > Content Authoring > Text fromats" section, Filtered HTML is the default filter.
Due to security reasons enabling Full HTML is only an option for trusted users.

To take full advantage of using CKEditor you can extend the list of allowed tags in the HTML filter that is enabled in the Filtered HTML input format. If you do not do this, you may notice that a page created in CKEditor looks different after saving.

Unfortunately, even if you extend the list of allowed tags, one problem still remains: Filtered HTML not only strips disallowed tags, but also strips inline style definitions. It basically means that you are unable to apply a different font color, size or family, align images etc. using CKEditor out of the box.

You can solve this problem by creating another input format that will work in a similar way as Filtered HTML (will only allow specified tags), but in a much better way - i.e. it will not strip inline styles that CKEditor is using when
formatting text or images after the page is saved. To create such an input format, you will need an HTML filter. See the list of HTML filter modules that are supported by the CKEditor module in the "Setting up Security Filters" section above.

It is up to you to decide which one to use. Just make sure that you will only allow to use proper inline styles, tags, and attributes.

Integrating a Custom Security Filter with the CKEditor Module (for Developers)
------------------------------------------------------------------------------
Integrating your application with the CKEditor module by adding a security filter works through a special hook.
An example of the hook is shown below:

function MODULENAME_ckeditor_security_filter() {
    return array(
        'security_filter_name' => array(
            // Security filter title - it would be displayed in the "Security > Security filters" section of profile settings.
            'title' => t('Security filter title'),
            // Security filter description - it would be displayed in the "Security > Security filters" section of profile settings.
            'description' => t('Security filter description'),
        )
    );
}
Please note that MODULENAME in the code above is the name of the module.

After the hook is used the security filter will automatically appear on the filters list for each CKEditor profile where you will be able to enable it as described in the "Setting up Filters" section.

Upgrading Instructions
----------------------
This instruction assumes that you are upgrading the CKEditor module [M] and CKEditor (the editor) [E] at the same time.
Instructions specific for module upgrades are tagged with [M]. Steps that must be taken when upgrading CKEditor (the editor) are marked with [E].

   1. [M] Download the latest version of the CKEditor module from http://drupal.org/project/ckeditor (it is advised to read the release notes before going further).
   2. [E] Download the latest version of CKEditor from http://ckeditor.com/download (it is advised to read the "What's New" page before going further: http://ckeditor.com/whatsnew).
   3. [M] Back up your database.
   4. [EM] Place the site in the "Off-line" mode to let the database updates run without interruption and to avoid displaying errors to end users of the site.
   5. [E] If you are using CKFinder, make sure you will not delete it, and move it to a safe place.
   6. [E] If you introduced any changes (e.g. custom toolbar definitions etc.) in the sites/all/modules/ckeditor/ckeditor.config.js file (or sites/all/modules/ckeditor/ckeditor/config.js), write down your changes and add them again after uploading new files.
          In general, try to avoid making any changes to CKEditor's config.js file and add everything to ckeditor.config.js.
   7. Delete old files:
      [EM]* Simply remove the "modules/ckeditor" directory if upgrading both the editor and the module.
      [M] If you are upgrading the module only, remember to leave the "modules/ckeditor/ckeditor" directory untouched.
      [E] When upgrading the editor, remove the contents of the "modules/ckeditor/ckeditor" directory only.
      WARNING: If you do not remove old files and just rename the "ckeditor" directory instead (e.g. to "ckeditor_old"), Drupal may use the module from the renamed "ckeditor_old" directory.
   8. [M] Upload the CKEditor module (extracted files and folders) to the "sites/all/modules" directory.
   9. [E] Upload standalone CKEditor (extracted files and folders from the "ckeditor" directory) to the "sites/modules/ckeditor/ckeditor" directory (i.e. where the COPY HERE.txt file exists).
   10. [E] Restore the CKFinder  files from where you copied them (see step 5).
   11. [E] Apply your modifications to default configuration in the ckeditor.config.js file (see step 6).
   12. [M] Run update.php.
   13. [EM] Put the site back online.

Help & Contribution
-------------------
If you are looking for more information, have any trouble with the configuration of the module
or if you found an issue, please visit the official project page:
  http://drupal.org/project/ckeditor

Having problems? Take a look at the list of common problems when installing CKEditor:
  http://docs.cksource.com/CKEditor_for_Drupal/Troubleshooting

Learn how to adjust CKEditor to your theme and configure the spellchecker:
  http://docs.cksource.com/CKEditor_for_Drupal/Tricks

If you would like to help in the development of the module, we encourage you to join our team.
Any help will be greatly appreciated!

Credits
-------
 - CKEditor for Drupal is currently maintained by the CKEditor team and Jorrit Schippers.
     http://ckeditor.com/

 - CKEditor - The text editor for the Internet
     Copyright (c) 2003-2012, CKSource - Frederico Knabben. All rights reserved.
     http://cksource.com/
