/**
 * @file
 * Linkit field dialog helper.
 */

Drupal.linkit.editorDialog.field = {};

(function ($) {

Drupal.linkit.editorDialog.field = {
  /**
   * Insert the link into the field.
   *
   * @param {Object} link
   *   The link object.
   */
  insertLink : function(data) {
    var linkitCache = Drupal.linkit.getLinkitCache(),
      field = $('#' + linkitCache.editorField),
      field_settings = Drupal.settings.linkit.fields[linkitCache.editorField],

      // Call the insert plugin.
      link = Drupal.linkit.insertPlugins[field_settings.insert_plugin].insert(data, field_settings);

    if (typeof linkitCache.selection != 'undefined') {
      // Replace the selection and insert the link there.
      Drupal.behaviors.linkit_field.replaceSelection(field.get(0), linkitCache.selection, link);
    }
    else {
      // Replace the field value.
      Drupal.behaviors.linkit_field.replaceFieldValue(field.get(0), link);
    }
  }
};

})(jQuery);