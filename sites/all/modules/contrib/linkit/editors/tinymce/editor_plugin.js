/**
 * @file
 * Plugin for inserting links with Linkit.
 */

(function ($) {

  tinymce.create('tinymce.plugins.linkit', {
    init : function(editor, url) {
      // Register commands
      editor.addCommand('mceLinkit', function() {
        // Set the editor object.
        Drupal.linkit.setEditor(editor);
        // Set which editor is calling the dialog script.
        Drupal.linkit.setEditorName('tinymce');

        // Stores the current editor selection for later restoration. This can
        // be useful since some browsers looses it's selection if a control
        // element is selected/focused inside the dialogs.
        editor.windowManager.bookmark = editor.selection.getBookmark(1);

        var path = Drupal.settings.linkit.url.tinymce;
        Drupal.linkit.dialog.buildDialog(path);
      });

      // Register buttons
      editor.addButton('linkit', {
        title : 'Linkit',
        cmd : 'mceLinkit',
        image : url + '/images/linkit.png'
      });

      /*
      // Uncommented as of #1459832.
      editor.onNodeChange.add(function(ed, cm, n, co) {
        var p = tinymce.DOM.getParent(n, 'A'),
            sel = ed.selection.getContent();
        // Activate button if caret is in an existing anchor.
        cm.setActive('linkit', !!p);
        // If nothing is selected and caret is not in an anchor, disable button.
        cm.setDisabled('linkit', !sel && !p);
      });
      */

      // We need the real contextmenu in order to make this work.
      if (editor && editor.plugins.contextmenu) {
        // Contextmenu gets called - this is what we do.
        editor.plugins.contextmenu.onContextMenu.add(function(th, m, e, col) {
          // Only if selected node is an link do this.
          if (e.nodeName == 'A' || !col) {
            // Remove all options from standard contextmenu.
            m.removeAll();
            th._menu.add({
              title : 'Linkit',
              cmd : 'mceLinkit',
              icon : 'linkit'
            });
            //m.addSeparator();
          }
        });
      }
    },

    getInfo : function() {
      return {
        longname : 'Linkit',
        author : 'Emil Stjerneman',
        authorurl : 'http://www.stjerneman.com',
        infourl : 'http://drupal.org/project/linkit',
        version : tinymce.majorVersion + "." + tinymce.minorVersion
      };
    }
  });

  // Register plugin
  tinymce.PluginManager.add('linkit', tinymce.plugins.linkit);

})(jQuery);