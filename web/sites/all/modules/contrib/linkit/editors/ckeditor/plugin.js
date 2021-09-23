/**
 * @file
 * Plugin for inserting links with Linkit.
 */

(function ($) {
  CKEDITOR.plugins.add( 'linkit', {

    requires : [ 'fakeobjects' ],

    init: function( editor ) {

      // Add Button.
      editor.ui.addButton( 'linkit', {
        label: 'Linkit',
        command: 'linkit',
        icon: this.path + 'linkit.png'
      });

      // Add Command.
      editor.addCommand( 'linkit', {
        // FOR ACF in ckeditor 4.1+, allow everything.
        allowedContent: 'a[*]{*}(*)',
        exec : function () {

          // Set the editor object.
          Drupal.linkit.setEditor(editor);
          // Set which editor is calling the dialog script.
          Drupal.linkit.setEditorName('ckeditor');
          // Set the name of the editor field, this is just for CKeditor.
          Drupal.linkit.setEditorField(editor.name);

          var linkitCache = Drupal.linkit.getLinkitCache();
          // Unlock the selecton for IE.
          if (CKEDITOR.env.ie && typeof linkitCache.selection !== 'undefined') {
            linkitCache.selection.unlock();
          }

          var selection = editor.getSelection(),
              element = null;

          // If we have selected a link element, we what to grab its attributes
          // so we can inserten them into the Linkit form in the  dialog.
          if ((element = CKEDITOR.plugins.linkit.getSelectedLink(editor)) && element.hasAttribute('href')) {
            selection.selectElement(element);
          }
          else {
            element = null;
          }

          // Save the selection.
          Drupal.linkit.setEditorSelection(selection);

          var linkitCache = Drupal.linkit.getLinkitCache();

          // Lock the selecton for IE.
          if (CKEDITOR.env.ie) {
            linkitCache.selection.lock();
          }

          // Save the selected element.
          Drupal.linkit.setEditorSelectedElement(element);

          var path = Drupal.settings.linkit.url.ckeditor;
          Drupal.linkit.dialog.buildDialog(path);
        }
      });

      // Add it to the rightclick menu
      if (editor.addMenuGroup)
      {
        editor.addMenuGroup("Linkit", 100);
      }

      if (editor.addMenuItems)
      {
        editor.addMenuItems( {
          linkit: {
            label: 'Linkit',
            command: 'linkit',
            icon: this.path + 'linkit.png',
            group : 'Linkit',
            order : 0
          }
        });
      }

      if (editor.contextMenu)
      {
        editor.contextMenu.addListener(function(element, selection) {
          if (!element || element.isReadOnly() || (selection.getSelectedText().length < 1 && !element.is('a')))
            return null;
          return { linkit: CKEDITOR.TRISTATE_ON };
        });
      }

      // Register an extra fucntion, this will be used in the popup.
      editor._.linkitFnNum = CKEDITOR.tools.addFunction( insertLink, editor );
    }

  });

  CKEDITOR.plugins.linkit = {
    getSelectedLink : function( editor )
    {
      try
      {
        var selection = editor.getSelection();
        if ( selection.getType() == CKEDITOR.SELECTION_ELEMENT )
        {
          var selectedElement = selection.getSelectedElement();
          if ( selectedElement.is( 'a' ) )
            return selectedElement;
        }

        var range = selection.getRanges( true )[ 0 ];
        range.shrink( CKEDITOR.SHRINK_TEXT );
        var root = range.getCommonAncestor();
        return root.getAscendant( 'a', true );
      }
      catch( e ) { return null; }
    }
  };

  /**
   *
   */
  function insertLink(data, editor) {
    var linkitCache = Drupal.linkit.getLinkitCache(),
        selection = editor.getSelection();

    data.path = CKEDITOR.tools.trim(data.path);

    // Browser need the "href" for copy/paste link to work. (CKEDITOR ISSUE #6641)
    data.attributes['data-cke-saved-href'] = data.path;

    if (!linkitCache.selectedElement) {
      // We have not selected any link element so lets create a new one.
      var range = selection.getRanges(1)[0];
      if (range.collapsed) {
        var content = (Drupal.linkitCache.link_tmp_title) ? Drupal.linkitCache.link_tmp_title : data.path;
        var text = new CKEDITOR.dom.text(content , editor.document );
        range.insertNode(text);
        range.selectNodeContents(text);
      }

      // Delete all attributes that are empty.
      data.attributes.href = data.path;
      for (name in data.attributes) {
        (data.attributes[name]) ? null : delete data.attributes[name];
      }
      // Apply style.
      var style = new CKEDITOR.style({element : 'a', attributes : data.attributes});
      style.type = CKEDITOR.STYLE_INLINE;
      style.applyToRange(range);
      range.select();
    }
    else {
      // We are editing an existing link, so just overwrite the attributes.
      var element = linkitCache.selectedElement;

      element.setAttribute('href', data.path);
      element.setAttribute('data-cke-saved-href', data.path);
      for (name in data.attributes) {
        data.attributes[name] ?
          element.setAttribute(name, data.attributes[name]) :
          element.removeAttribute(name);
      }
    }
    // Unlock the selection.
    selection.unlock();
    delete linkitCache.selectedElement;
  };

})(jQuery);
