(function($) {
  var settings = Drupal.settings['pensoft'];
  var $tables = null;
  
  /**
   * Init function
   */
  function init() {
    CKEDITOR.plugins.add("pensoft_publication_insert_table", {
      init : function(editor) {
        editor.ui.addButton("pensoft_publication_insert_table",{
          label : "pensoft_publication_insert_table",
          icon : Drupal.settings.basePath + settings['editor_table_icon'],
          command : "pensoft_publication_insert_table",
        });

        editor.addCommand( 'pensoft_publication_insert_table', {
          exec : function() {
            var title = '<h1>' + Drupal.t('Select table citation(s) to insert') + '</h1>';
            var description = Drupal.t('<div class="info" style="margin: 10px;">The table citation will be inserted as <strong>Table [id:<em>id-of-the-table</em>]</strong>. Once the publication is saved, the tables will be ordered by order of appeareance, and the citations will be renamed accordingly as <strong>Table <em>table-number</em></strong>.</div>');
            $tables = $('<div id="pensoft_select_table">' + title + description 
              + '<table><tr><th>select</th><th>Id</th><th>Title</th><th>Table</th></tr></table></div>').appendTo('body');
            
            // Populate with available tables
            $('#edit-field-publication-tables .form-type-textarea').each(function(i,e) {
              var label = $(this).parents('td').find('.field-name-field-publication-table-title input').val();
              var id = $(this).parents('td').find('.field-name-field-publication-table-id input').val();
              
              if (id == '') {
                return;
              }
              
              // Ensure all text areas are up to date
              if (typeof CKEDITOR != 'undefined') {
                for (var instanceName in CKEDITOR.instances) {
                  CKEDITOR.instances[instanceName].updateElement();
                }
              }              
              
              var content = $.trim($(this).find('textarea').val());
              if (content != '') {   
                // Create the row
                var $row = $('<tr class="pensoft_select_table_row">' + 
                             '<td><input type="checkbox" name="' + id + '"/></td><td>' + 
                             id + '</td><td>' + label + '</td><td>' + content + '</td></tr>'
                 ).appendTo($tables.children('table'));
                
                // Handle clicks: Bypass clicks on links within the row
                $('a', $row).click(function(e) {
                  e.stopPropagation();
                  $row.click();
                  return false;
                });
                
                // Don't propagate clicks on the checkbox
                $('input', $row).click(function(e) {
                  e.stopPropagation();
                });

                // Handle single inserts
                $row.click(function() {
                  editor.insertHtml('<tbls_citation citation_id="' + id + '" contenteditable="false" style="background: #AAA;">Table [id:' + id + ']</tbls_citation>');
                  editor.updateElement();
                  $.colorbox.close();
                });
              }
            });
            
            // Handle multile inserts
            $('<div class="form-item"><input type="button" class="form-submit" value="' + Drupal.t('Insert') + '"/></div>').appendTo($tables).mousedown(function() {
              var elements = [];
              $tables.find('tr.pensoft_select_table_row input:checked').each(function() {
                elements.push('<span>Table. ' + $(this).attr('name') + '</span>');
              });
              
              editor.insertHtml(elements.join(''));
              editor.updateElement();
              $.colorbox.close();
            });
            
            // Launch the colorbox
            $.colorbox({
              inline: true,
              iframe: false,
              href: $tables,
              width: settings['editor_table_width'],
              height: settings['editor_table_height'],
              onClosed: function() {
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
