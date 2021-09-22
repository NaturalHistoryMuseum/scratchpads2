<?php

/**
 * Defines a user interface element that will allow a user to interact with tag
 * suggestions provided by a suggestion module.
 * 
 * This hook should return an associative array or arrays, with keys being, 
 * unique prefferably namespaced based on the module defining them.  Each array
 * should include the following elements:
 * 
 * - label: Untranslated human readable label for the UI element.
 * - description: Translated description for the UI element
 * - submit_callback: A callback which should be called to alter the form as
 *   required.  The callback has two arguments as any form submit function 
 *   would.
 * - form_alter_callback: A callback to alter the form to add the required UI
 *   elements
 */
function hook_tag_ui_info(){
  return array(
    'tag_on_save' => array(
      'label' => 'Tag on save',
      'description' => t('Saves all found tags to a node without any user interaction required.'),
      'submit_callback' => 'tag_on_save_tag_update',
      'form_alter_callback' => 'tag_on_save_tag_form_alter'
    )
  );
}

/**
 * Defines a suggestion interface which will take a block of text, and suggest
 * tags to be associated with the text.
 * 
 * This hook should return an associative array of arrays, with keys being
 * unique prefferably namespaced based on the module defining them.  Each array
 * should include the following elements:
 * 
 * - label: Untranslated human readable label for the suggestion interface.
 * - description: Translated description for the suggestion interface.
 * - request_callback: function to call for tag suggestions.  The function takes
 *   at least one argument, the text string, with a second optional argument
 *   being the options array
 * - options
 *   - callback: function to return a form for defining options for the searcher
 *   - keys: the elements of the form that should be passed on to the 
 *     request_callback function.
 */
function hook_tag_suggestion_info(){
  return array(
    'autotag' => array(
      'label' => t('Taxonomy term searcher'),
      'description' => t('Automatically associates terms from a taxonomy if the term appears within the text of the node'),
      'request_callback' => 'autotag_tag_suggestions',
      'options' => array(
        'callback' => 'autotag_tag_options',
        'keys' => array(
          'autotag_vids'
        )
      )
    )
  );
}

/**
 * Enable other modules to tweak the widget modules that can be used by the tag
 * module. This hook should alter the passed in $field_types array (standard
 * drupal alter hook).
 */
function hook_tag_field_types_alter(&$field_types){
  $field_types[] = 'module_that_can_be_used';
}

/**
 * Enable other modules to tweak the widget maps that define how suggestions are
 * saved to a field. This hook should alter the passed in $field_types array
 * (standard drupal alter hook).
 */
function hook_tag_widget_map_alter(&$field_types){
  $field_types['module_that_can_be_used'] = 'taxonomy/text';
}