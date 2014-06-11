(function($) {
  var settings = Drupal.settings['pensoft'];
  var $tables = null;

  /**
   * Init function
   */
  function init() {
    CKEDITOR.plugins.add("pensoft_publication_insert_table", {
      init : function(editor) {
        editor.ui.addButton("pensoft_publication_insert_table", {
          label : Drupal.t("Add table citation. Create tables in the Tables section first."),
          icon : Drupal.settings.basePath + settings['editor_table_icon'],
          command : "pensoft_publication_insert_table",
        });

        editor.addCommand('pensoft_publication_insert_table', {
          exec : function() {
            $tables = $('<div id="pensoft_select_table">' + settings.editor_table_header
                + '<table><tr><th>select</th><th>Id</th><th>Title</th><th>Table</th></tr></table></div>').appendTo('body');

            // Populate with available tables
            $('#edit-field-publication-tables .form-type-textarea').each(function(i, e) {
              var label = $(this).parents('td').find('.field-name-field-publication-table-title input').val();
              var id = $(this).parents('td').find('.field-name-field-publication-table-id input').val();

              if (id == '') {
                return;
              }

              // Ensure all text areas are up to date
              if (typeof CKEDITOR != 'undefined') {
                for ( var instanceName in CKEDITOR.instances) {
                  CKEDITOR.instances[instanceName].updateElement();
                }
              }

              var content = $.trim($(this).find('textarea').val());
              if (content != '') {
                // Create the row
                var $row = $('<tr class="pensoft_select_table_row">'
                             + '<td><input type="checkbox" name="'
                             + id + '"/></td><td>' + id
                             + '</td><td>' + label
                             + '</td><td>' + content
                             + '</td></tr>').appendTo($tables.children('table'));

                // Handle clicks: Bypass clicks on links
                // within the row
                $('a', $row).click(function(e) {
                  e.stopPropagation();
                  $row.click();
                  return false;
                });

                // Don't propagate clicks on the
                // checkbox
                $('input', $row).click(function(e) {
                  e.stopPropagation();
                });

                // Handle single inserts
                $row.click(function() {
                  editor.insertElement(CKEDITOR.dom.element.createFromHtml(
                      '<tbls-citation citation_id="'+ id + '" contenteditable="false" title="'
                      + Drupal.t('Table citation. These will be re-numbered when the publication is finalised.')
                      +'">Table [id:' + id + ']</tbls-citation>'));
                  editor.updateElement();
                  $.colorbox.close();
                });
              }
            });

            // Handle multile inserts
            $('<div class="form-item"><input type="button" class="form-submit" value="'
              + Drupal.t('Insert') + '"/></div>').appendTo($tables).mousedown(function() {
              var elements = [];
              $tables.find('tr.pensoft_select_table_row input:checked').each(function() {
                var id = $(this).attr('name');
                var line = '<tbls-citation citation_id="'+ id + '" contenteditable="false" title="'
                + Drupal.t('Table citation. These will be re-numbered when the publication is finalised.')
                + '">Table [id:' + id + ']</tbls-citation>';

                editor.insertElement(CKEDITOR.dom.element.createFromHtml(line));
              });

              editor.updateElement();
              $.colorbox.close();
            });

            // Launch the colorbox
            $.colorbox({
              inline : true,
              iframe : false,
              href : $tables,
              width : settings['editor_table_width'],
              height : settings['editor_table_height'],
              onClosed : function() {
                $tables.remove();
                $tables = null;
              }
            });
          }
        });
      }
    });
  }

  init();

})(jQuery);
