# SIMPLE DIALOG #

This module provides a method to load pages via AJAX into a
modal dialog window that will be overlaid on the screen.

The module implements the jquery ui dialog plugin that is
provided with Drupal 7.

## CONFIGURATION ##

The configuration page is at admin/config/content/simple-dialog.

1) Add simple dialog javascript files to all pages.

By Default this option is selected. This option is here in case
you're trying to limit the amount of data loaded on each page load.
If you're not worried about that you can probably just leave this
enabled. A couple things to note if you disable this setting:

- You will need to add the javascript files to the page manually
if you want to implement the "simple-dialog" class method.
- If you are adding simple dialog links to the page using
theme('simple_dialog'...), the necessary
javascript is added within those functions so you should be okay.

2) Additional Classes

This option allows you to specify custom classes that will also be
used to launch a modal. This can be useful if you want to use a simple
class like 'popup' to launch the modal, or perhaps if you're upgrading
a site from d6 that used the automodal module and you just want to
continue using the automodal class instead of changing all your links.

A space-separated list of classes should be provided with no leading
or trailing spaces.

3) Default Dialog Settings

Provide some default settings for the dialog. Defaults should be
formatted the same way as you would in the "rel" tag of the
link (described below under HTML Implementation)

4) Default Target Selector

Provide a default html element id for the target page (the page that
will be pulled into the dialog). This value will be used if no "name"
attribute is provided in a simple dialog link.

5) Default Dialog Title

Provide a default dialog title. This value will be used if no "title"
attribute is provided in a simple dialog link.

## JAVASCRIPT ##

This module doesn't bring javascript files over from
the target page. If your target html needs javascript to work,
You will need to make sure your javascript is either inline in
the html that's being loaded, or in the head tag of the page
you are on.

## HTML Implementation ##

Add the class 'simple-dialog' to open links in a dialog
You also need to specify 'name="{selector}"' where the {selector}
is the unique id of the container to load from the linked page,
as well as the title attribute which will act as the dialog title.
Any additional jquery ui dialog options can be passed through
the rel tag using the format:
   rel="{option-name1}:{value1};{option-name2}:{value2};"
NOTE: For the position option, if you want to pass in an array of
xy values, use the syntax [{x},{y}] with no quotes or spaces.

example:

<a href="path/to/target/page/to/load"
   class="id-of-element-on-target-page-to-load"
   rel="width:900;resizable:false;position:[center,60]"
   name="content-area" title="My Dialog Title">Link</a>

The available jquery ui dialog options can be found here:

  http://jqueryui.com/demos/dialog

## THEME Function Implementation ##

You can also implement a simple dialog link using the theme
function: theme('simple_dialog_link', $args) where $args contains
the following values:

$args = array(
  // required
  'text' => 'My Link Text',
  'path' => 'path/to/page/to/load',
  'selector' => 'id-of-container-on-target-page-to-load',
  'title' => 'My Dialog Title',
  // optional
  'options' => array(
    'optionName' => 'optionValue', // examples:
    'width' => 900,
    'resizable' => FALSE,
    'position' => 'center', // Position can be a string or:
    'position' => array(60, 'top') // can be an array of xy values
  ),
  'class' => array('class-name-1', 'class-name-2'),
);

For the 'position' option, the value can be a string or an
array of xy values. Per the jquery ui dialog documentation at
http://jqueryui.com/demos/dialog/#option-position:

Specifies where the dialog should be displayed. Possible values:
1) a single string representing position within viewport:
   'center', 'left', 'right', 'top', 'bottom'.
2) an array containing an x,y coordinate pair in pixel offset
   from left, top corner of viewport (e.g. array(350,100))
3) an array containing x,y position string values
   (e.g. array('right','top') for top right corner).

## EXAMPLES ##

Enable the accompanying module "Simple Dialog Example" to see
some examples. It can be found on the modules page in the
module group: Example Modules

## A NOTE ABOUT INPUT FORMATS ##

If you are adding a link that launches the modal window through
a textfield that uses an input format, the settings supplied in
the "rel" attribute might be stripped. This happens if the 'limit
allowed html tags' options is selected for that input format.

