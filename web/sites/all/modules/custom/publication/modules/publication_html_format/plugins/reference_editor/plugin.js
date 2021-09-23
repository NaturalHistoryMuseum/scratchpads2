(function($) {
  var settings = Drupal.settings['pensoft'];

  /**
   * Init function
   */
  function init() {
    CKEDITOR.plugins.add("pensoft_publication_reference_editor", {
      init : function(editor) {
        CKEDITOR.dialog.add('reference_editor_dialog', function(editor) {
          return {
            title : Drupal.t('Reference editor'),
            contents:
            [
              {
                'id' : 'tab1',
                'label' : Drupal.t('Basic settings'),
                elements:
                [
                  {
                    type: 'text',
                    id: 'reference_text',
                    label: 'Reference',
                    validate : CKEDITOR.dialog.validate.notEmpty("Reference cannot be empty"),
                    setup: function(element) {
                      this.setValue(element.getText());
                    },
                    commit: function(element) {
                      element.setText(this.getValue());
                    }
                  }
                ]
              },
              {
                'id' : 'tab2',
                'label' : Drupal.t('Advanced settings'),
                elements:
                [
                  {
                    type: 'text',
                    id: 'citation_id',
                    label: 'Reference Id',
                    validate : CKEDITOR.dialog.validate.notEmpty("Reference Id cannot be empty"),
                    setup: function(element) {
                      this.setValue(element.getAttribute("citation_id"));
                    },
                    commit: function(element) {
                      element.setAttribute('citation_id', this.getValue());
                    }
                  },
                  {
                    type: 'html',
                    html : 'This is the id of the biblio element being referenced.<br/> Changing this here will not automatically add' +
                      ' the reference to the publication you will need to do this manually'
                  }
                ]
              }
              
            ],
            onShow: function() {
              var sel = editor.getSelection();
              var element = sel.getStartElement();
              if (element) {
                element = element.getAscendant('reference-citation', true);
              }
              this.element = element;
              this.setupContent(this.element);
            },
            onOk: function () {
              this.commitContent(this.element);
            }
          }
        });
        
        editor.addCommand('pensoft_publication_edit_reference', new CKEDITOR.dialogCommand('reference_editor_dialog'));
        
        if (editor.contextMenu) {
          editor.addMenuGroup('ReferenceGroup');
          editor.addMenuItem('reference_editor', {
            label : 'Edit Reference',
            group: 'ReferenceGroup',
            icon: Drupal.settings.basePath + settings['editor_reference_editor_icon'],
            command: 'pensoft_publication_edit_reference'
          });
          
          editor.contextMenu.addListener(function(element) {
            if (element) {
              element = element.getAscendant('reference-citation', true);
            }
            if (element && !element.data('cke-realelement')) {
              return {'reference_editor' : CKEDITOR.TRISTATE_OFF};
            }
            return null;
          });
        }        
      }
    });
  }

  init();

})(jQuery);
