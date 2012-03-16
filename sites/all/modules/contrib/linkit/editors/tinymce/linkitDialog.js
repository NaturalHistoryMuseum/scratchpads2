/**
 * @file
 * Linkit tinymce dialog helper.
 */

Drupal.linkit.editorDialog.tinymce = {};

(function ($) {

Drupal.linkit.editorDialog.tinymce = {
  init : function() {},

  /**
   * Prepare the dialog after init.
   */
  afterInit : function () {
    var linkitCache = Drupal.linkit.getLinkitCache(),
        editor = linkitCache.editor, element, link;

      // Restore the selection if the browser is IE.
      if (tinymce.isIE) {
        editor.selection.moveToBookmark(editor.windowManager.bookmark);
      }

    // If we have selected a link element, lets populate the fields in the
    // dialog with the values from that link element.
    if (element = editor.dom.getParent(editor.selection.getNode(), 'A')) {
      link = {
        path: editor.dom.getAttrib(element, 'href'),
        attributes: {}
      };
      // Add attributes to the link object, but only those that are enabled in Linkit.
      tinymce.each(Drupal.linkit.dialog.additionalAttributes(), function(attribute) {
        var value = editor.dom.getAttrib(element, attribute);
        if (value) {
          link.attributes[attribute] = value;
        }
      });
    }
    Drupal.linkit.dialog.populateFields(link);
  },

  /**
   * Insert the link into the editor.
   *
   * @param {Object} link
   *   The link object.
   */
  insertLink : function(data) {
    var linkitCache = Drupal.linkit.getLinkitCache(),
        editor = linkitCache.editor, 
        element = editor.dom.getParent(editor.selection.getNode(), 'A');

    // Restore the selection if the browser is IE.
    if (tinymce.isIE) {
      editor.selection.moveToBookmark(editor.windowManager.bookmark);
    }

    // Set undo begin point.
    editor.execCommand("mceBeginUndoLevel");
    data.attributes.href = data.path;

    // No link element selected, create a new anchor element.
    if (element == null) {
      editor.execCommand("mceInsertLink", false, data.attributes);
    }
    // We are editing an existing link, so just overwrite the attributes.
    else {
      editor.dom.setAttribs(element, data.attributes);
    }

    // Don't move caret if selection was image
    if(element != null) {
      if (element.childNodes.length != 1 || element.firstChild.nodeName != 'IMG') {
        editor.focus();
        editor.selection.select(element);
        editor.selection.collapse(0);
        // Restore the selection if the browser is IE.
        if (tinymce.isIE) {
          editor.selection.moveToBookmark(editor.windowManager.bookmark);
        }
      }
    }
    // Set undo end point.
    editor.execCommand("mceEndUndoLevel");
  }
};

})(jQuery);