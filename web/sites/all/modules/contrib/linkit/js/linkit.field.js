/**
 * @file
 * Linkit field ui functions
 */

// Create the linkit namespaces.
Drupal.linkit = Drupal.linkit || {};
Drupal.linkit.field = Drupal.linkit.field || {};

(function ($) {

Drupal.behaviors.linkit_field = {
  attach : function(context, settings) {
    // If there is no fields, just stop here.
    if (settings.linkit.fields == null) {
      return false;
    }
    $.each(settings.linkit.fields, function(field) {
      $('#' + field, context).once('linkit_field', function() {
        $('.linkit-field-' + field).click(function() {
          // We dont have an editor here, but we need to give this instance a
          // name.
          Drupal.linkit.setEditorName('field');
          // Set the name of the editor field, this is just for CKeditor.
          Drupal.linkit.setEditorField(field);

          // Only care about selection if the element is a textarea.
          if ($('textarea#' + field).length) {
            var selection = Drupal.behaviors.linkit_field.getSelection($('#' + field).get(0));
            // Save the selection.
            Drupal.linkit.setEditorSelection(selection);
          }

          Drupal.linkit.dialog.buildDialog(settings.linkit.url.field);
          return false;
        });
      });
    });
  },

  /**
   * Get field selection.
   */
  getSelection : function(e) {
    // Mozilla and DOM 3.0
    if ('selectionStart' in e) {
        var l = e.selectionEnd - e.selectionStart;
        return { start: e.selectionStart, end: e.selectionEnd, length: l, text: e.value.substr(e.selectionStart, l) };
    }
    // IE
    else if(document.selection) {
        e.focus();
        var r = document.selection.createRange(),
          tr = e.createTextRange(),
          tr2 = tr.duplicate();
        tr2.moveToBookmark(r.getBookmark());
        tr.setEndPoint('EndToStart',tr2);

        if (r == null || tr == null) {
          return { start: e.value.length, end: e.value.length, length: 0, text: '' };
        }

        // For some reason IE doesn't always count the \n and \r in the length
        var text_part = r.text.replace(/[\r\n]/g,'.'),
          text_whole = e.value.replace(/[\r\n]/g,'.'),
          the_start = text_whole.indexOf(text_part, tr.text.length);
        return { start: the_start, end: the_start + text_part.length, length: text_part.length, text: r.text };
    }
    // Browser not supported
    else {
      return { start: e.value.length, end: e.value.length, length: 0, text: '' };
    }
  },

   /**
   * Replace the field selection.
   */
  replaceSelection : function (e, selection, text) {
    var start_pos = selection.start;
    var end_pos = start_pos + text.length;
    e.value = e.value.substr(0, start_pos) + text + e.value.substr(selection.end, e.value.length);
  },

   /**
   * Replace the field value.
   */
  replaceFieldValue : function (e, text) {
    e.value = text;
  }
};

})(jQuery);
