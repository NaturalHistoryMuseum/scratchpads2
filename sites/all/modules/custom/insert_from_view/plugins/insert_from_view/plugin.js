(function($) {
  /**
   * Read settings
   */
  var settings = Drupal.settings['insert_from_view'];
  if (settings.init == true) {
    return;
  }
  settings.init = true;
  settings.map = {};
  settings.ajax = {};

  /**
   * Global function invoked from callback
   */
  $.fn.insert_from_view_setup_view = function(key, insert_array) {
    var instance_id = 'insert-from-view-' + key;
    var $root = $('#' + instance_id);

    // Handle individual fields
    $('.insert-from-view-row', $root).click(function(e) {
      var editor = Drupal.insert_from_view_editor;
      var index = $(this).index();
        
      var elements = [insert_array[index]];

      // Give other scripts a chance to react/change the values
      $(editor.container.$).siblings('textarea').trigger('insertFromView-insert', [elements, settings.map[key]]);      

      var row_tag = ['', ''];
      var wrap_tag = ['', ''];
      if (settings.map[key].row_tag != '') {
        row_tag = [' <' + settings.map[key].row_tag + '> ',
                   ' </' + settings.map[key].row_tag + '> '];
      }
      if (settings.map[key].wrap_tag != '') {
        wrap_tag = [' <' + settings.map[key].wrap_tag + '> ',
                   ' </' + settings.map[key].wrap_tag + '> '];
      }
      
      editor.insertHtml(wrap_tag[0] + row_tag[0] + elements.join(row_tag[1] + row_tag[0]) + row_tag[1] + wrap_tag[1]);
      
      editor.updateElement();
        
      $.colorbox.close();
    });
    
    // Bypass clicks on links within the row
    $('.insert-from-view-row a', $root).click(function(e) {
      e.stopPropagation();
      $(this).parents('.insert-from-view-row').click();
      return false;
    });
    
    // Don't propagate clicks on the checkbox
    $('.insert-from-view-row .insert_from_view_checkbox', $root).click(function(e) {
      e.stopPropagation();
    });
        
    // Create & handle multiple inserts
    if ($('.insert_from_view_checkbox', $root).length > 0) {
      $('<input type="button" class="insert-from-view-button form-submit" value="' + Drupal.t('Insert') + '" />')
      .appendTo($('div.view', $root))
      .mousedown(function() {
        var editor = Drupal.insert_from_view_editor;
        var elements = [];
        
        $('.insert_from_view_checkbox:checked').each(function() {
          var index = $(this).parents('.insert-from-view-row').index();
          elements.push(insert_array[index]);
        });

        // Give other scripts a chance to react/change the values
        $(editor.container.$).siblings('textarea').trigger('insertFromView-insert', [elements, settings.map[key]]);

        var row_tag = ['', ''];
        var wrap_tag = ['', ''];
        if (settings.map[key].row_tag != '') {
          row_tag = [' <' + settings.map[key].row_tag + '> ',
                     ' </' + settings.map[key].row_tag + '> '];
        }
        if (settings.map[key].wrap_tag != '') {
          wrap_tag = [' <' + settings.map[key].wrap_tag + '> ',
                     ' </' + settings.map[key].wrap_tag + '> '];
        }
        
        editor.insertHtml(wrap_tag[0] + row_tag[0] + elements.join(row_tag[1] + row_tag[0]) + row_tag[1] + wrap_tag[1]);
        editor.updateElement();

        $.colorbox.close();
        
        return false;
      });
    }
    
    // Handle exposed forms
    var $widgets_root = $('div.views-exposed-form div.views-exposed-widgets', $root);      
    if ($widgets_root.length > 0) {
      var button_label = $('.views-submit-button input', $widgets_root).attr('value');
      $('.views-submit-button input', $widgets_root).css('display', 'none');
      
      var $new_button = $('<input type="button" class="form-submit" value="' + button_label 
          + '" />').appendTo($('.views-submit-button', $widgets_root));
      
      $new_button.mousedown(function(e) {      
        var ajax = settings.ajax[instance_id];
        var base_url = ajax.options.url;
      
        // Read filters
        var filters = [];
        $('input[type=text], select', $widgets_root).each(function() {
          var val = $(this).val();
          if (typeof(val) == "string") {
            filters.push(encodeURIComponent($(this).attr('name')) + '=' + encodeURIComponent(val));
          } else {
            for (var i = 0; i < val.length; i++) {
              filters.push(encodeURIComponent($(this).attr('name')) + '[]=' + encodeURIComponent(val[i]));            
            }
          }
        });
      
        if (filters.length > 0) {
          ajax.options.url = base_url + '?' + filters.join('&');        
        }
        ajax.eventResponse($root, 'loadView');
        ajax.options.url = base_url;
      
        e.stopPropagation();
        return false;
      });
    }
  };

  /**
   * Sets up one instance of a insert from view button
   */
  function setup_instance(set) {
    var instance_id = 'insert-from-view-' + set.key;
   
    // Add the element that will contain the frame and the button used for triggering ajax
    $('body').append('<div id="' + instance_id + '"></div>');
    settings.ajax[instance_id] = new Drupal.ajax(instance_id, $('#' + instance_id), {
      url: Drupal.settings.basePath + 'insert-from-view/' +set.key,
      event: 'loadView',
    });   

    // Add the CKEDITOR plugin
    CKEDITOR.plugins.add("insert_from_view_" + set.key, {
      init : function(editor) {
        editor.ui.addButton("insert_from_view_" + set.key,{
          label : "insert_from_view_" + set.key,
          icon : Drupal.settings.basePath + set.icon,
          command : "insert_from_view_" + set.key
        });

        editor.addCommand( 'insert_from_view_' + set.key, {
          exec : function() {
            $.colorbox({
              inline: true,
              iframe: false,
              href: $('#' + instance_id),
              width: set.width,
              height: set.height,
              onClosed: function() {
                $('#' + instance_id).empty();
              }
            });
            Drupal.insert_from_view_editor = editor;
            
            var ajax = settings.ajax[instance_id];
            var arguments = [];
            $(editor.container.$).siblings('textarea').trigger('insertFromView-load', [arguments, set]);
            if (arguments.length > 0) {
              for (var i = 0; i < arguments.length; i++) {
                arguments[i] = encodeURIComponent(arguments[i]);
              }
              ajax.options.url = Drupal.settings.basePath + 'insert-from-view/' +set.key + '/' + arguments.join('/');
            } else {
              ajax.options.url = Drupal.settings.basePath + 'insert-from-view/' +set.key
            }
            ajax.eventResponse($('#' + instance_id), 'loadView');
          }
        });
      }
    });
  }
  
  /**
   * Init function
   */
  function init() {
    for (var i = 0; i < settings.settings.length; i++) {
      settings.map[settings.settings[i].key] = settings.settings[i];
      setup_instance(settings.settings[i]);    
    }
  }
  
  init();
  
})(jQuery);