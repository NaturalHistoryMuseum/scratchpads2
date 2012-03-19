/**
 * @file
 * Linkit ckeditor dialog helper.
 */

Drupal.linkit.editorDialog.ckeditor = {};

(function ($) {

Drupal.linkit.editorDialog.ckeditor = {
  init : function() {},

  /**
   * Prepare the dialog after init.
   */
  afterInit : function () {
    var linkitCache = Drupal.linkit.getLinkitCache();

    // If we have selected a link element, lets populate the fields in the
    // dialog with the values from that link element.
    if (linkitCache.selectedElement) {
      link = {
        path: (linkitCache.selectedElement  && (linkitCache.selectedElement.data('cke-saved-href') || linkitCache.selectedElement.getAttribute('href'))) || '',
        attributes: {}
      },
      // Get all attributes that have fields in the dialog.
      additionalAttributes = Drupal.linkit.dialog.additionalAttributes();

      for (var i = 0; i < additionalAttributes.length; i++) {
        link.attributes[additionalAttributes[i]] =
        linkitCache.selectedElement.getAttribute(additionalAttributes[i]);
      };

      // Populate the fields.
      Drupal.linkit.dialog.populateFields(link);
    }
  },

  /**
   * Insert the link into the editor.
   *
   * @param {Object} link
   *   The link object.
   */
  insertLink : function(link) {
    var linkitCache = Drupal.linkit.getLinkitCache();
    CKEDITOR.tools.callFunction(linkitCache.editor._.linkitFnNum, link, linkitCache.editor);
  }
};

})(jQuery);