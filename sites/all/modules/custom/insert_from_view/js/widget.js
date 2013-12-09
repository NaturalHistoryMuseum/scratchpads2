(function($){
  // Store the info for all the insert from view widgets on the page
  var all_info = {};

  /**
   * get_info
   * 
   * Get the machine name, overlay settings and list of nodes 
   * from a text input used to store insert from widget data
   * 
   */
  function get_info($input){
    // Read machine name
    var machine_name = $input.attr('id');
    // Find the overlay settings
    var settings = false;
    for (var i = 0; i < Drupal.settings['insert_from_view'].settings.length; i++){
      if (Drupal.settings['insert_from_view'].settings[i].machine_name == machine_name){
        settings = Drupal.settings['insert_from_view'].settings[i];
        break;
      }
    }
    if (settings === false){
      throw "No settings for insert from view widget " + machine_name;
    }
    // Read the nodes
    var nodes = [];
    var strings = $input.val().split(',');
    for (i in strings){
      var str = strings[i].trim();
      var matches = str.match(/^(\d+)(?::(.*))?$/);
      if (matches === null || typeof(matches[1]) === 'undefined'){
        continue;
      }
      nodes.push({
        nid: matches[1],
        title: matches[2]
      });
    }
    // Set value and return
    info = {
      input: $input,
      container: $input.parent(),
      machine_name: machine_name,
      nodes: nodes,
      settings: settings
    };
    all_info[machine_name] = info;
    return info;
  }
  
  /**
   * populate_input
   * 
   * From an info object update the list of
   * nodes in the associated input field
   */
  function populate_input(info){
    var values = [];
    for (var i in info.nodes){
      var node = info.nodes[i];
      var str = node.nid;
      if (typeof(node.title) !== 'undefined'){
        str = str + ':' + node.title.replace(/,/, ' ');
      }
      values.push(str);
    }
    info.input.attr('value', values.join(','));
  }
  
  /**
   * populate_widget
   * 
   * Populate the widget that replaces the
   * input field
   */
  function populate_widget(info){
    info.widget.empty();
    // Add the 'add more' link
    var text = Drupal.t('add more');
    var $add_more = $('<div><a href="#" title="' + text + '">(' + text + ')</a></div>').appendTo(info.widget);
    $('a', $add_more).click(function(){
      open_overlay(info);
    });
    // And the list of nodes
    for (var index = 0; index < info.nodes.length; index++){
      var node = info.nodes[index];
      var node_obj = Drupal.theme('insertFromViewItem', node.nid, node.title);
      if (index == 0){
        $('a.insert-from-view-up', node_obj).css('display', 'none');
      } else {
        $('a.insert-from-view-up', node_obj).click((function(index){
          return function(){
            var temp = info.nodes[index-1];
            info.nodes[index-1] = info.nodes[index];
            info.nodes[index] = temp;
            populate_input(info);
            populate_widget(info);
          };
        })(index));
      }
      if (index == info.nodes.length -1){
        $('a.insert-from-view-down', node_obj).css('display', 'none');
      } else {
        $('a.insert-from-view-down', node_obj).click((function(index){
          return function(){
            var temp = info.nodes[index+1];
            info.nodes[index+1] = info.nodes[index];
            info.nodes[index] = temp;
            populate_input(info);
            populate_widget(info);
          };
        })(index));
      }
      $('a.insert-from-view-delete', node_obj).click((function(index){
        return function(){
          info.nodes.splice(index,1);
          populate_input(info);
          populate_widget(info);
        };
      })(index));
      info.widget.append(node_obj);
    }
  }
  
  /**
   * open_overlay
   * 
   * Open the overlay
   */
  function open_overlay(info){
    var ajax = new Drupal.ajax('insert-from-view-overlay', info.overlay, {
      url : Drupal.settings.basePath + 'insert-from-view/' + info.machine_name,
      event : 'loadView',
    });
    $.colorbox({
      inline : true,
      iframe : false,
      href : info.overlay,
      width : info.settings.width,
      height : info.settings.height,
      onClosed : function() {
        info.overlay.empty();
      }
    });
    info.ajax = ajax;
    ajax.eventResponse(info.overlay, 'loadView');
  }

  /**
   * Global function invoked from callback
   */
  $.fn.insert_from_view_setup_field_view = function(machine_name, insert_array) {
    var info = all_info[machine_name];
    var $root = $("#insert-from-view-overlay");

    // Handle individual fields
    $('.insert-from-view-row', $root).click(function(e) {
      var elements = [insert_array[$(this).index()]];
      // Give other scripts a chance to react/change the values
      info.input.trigger('insertFromView-insert', [elements, info.settings]);
      // Insert the values
      for (var i = 0; i < elements.length; i++){
        var matches = elements[i].match(/^(\d+)(?::(.*))?$/);
        if (matches === null || typeof matches[1] === 'undefined'){
          continue;
        }
        var nid = matches[1];
        var title = matches[2];
        var dup = false;
        for (var j = 0; j < info.nodes.length; j++){
          if (info.nodes[j].nid == nid){
            dup = true;
            break;
          }
        }
        if (!dup){
          info.nodes.push({
            nid: nid,
            title: title
          });
        }
      }
      populate_input(info);
      populate_widget(info);
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
      $('<input type="button" class="insert-from-view-button form-submit" value="'+ Drupal.t('Insert') + '" />')
      .appendTo($('div.view', $root))
      .mousedown(function() {
        // Gather selected values
        var elements = [];
        $('.insert_from_view_checkbox:checked').each(function() {
          var index = $(this).parents('.insert-from-view-row').index();
          elements.push(insert_array[index]);
        });
        // Give other scripts a chance to react/change the values
        info.input.trigger('insertFromView-insert', [elements, info.settings]);
        // Insert values
        for (var i = 0; i < elements.length; i++){
          var matches = elements[i].match(/^(\d+)(?::(.*))?$/);
          if (matches === null || typeof matches[1] === 'undefined'){
            continue;
          }
          var nid = matches[1];
          var title = matches[2];
          var dup = false;
          for (var j = 0; j < info.nodes.length; j++){
            if (info.nodes[j].nid == nid){
              dup = true;
              break;
            }
          }
          if (!dup){
            info.nodes.push({
              nid: nid,
              title: title
            });
          }
        }
        populate_input(info);
        populate_widget(info);
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
        var ajax = info.ajax;
        var base_url = ajax.options.url;

        // Read filters
        var filters = [];
        $('input[type=text], select', $widgets_root).each(function() {
          var val = $(this).val();
          if (typeof (val) == "string") {
            filters.push(encodeURIComponent($(this).attr('name')) + '=' + encodeURIComponent(val));
          } else {
            for ( var i = 0; i < val.length; i++) {
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

    // Handle pager
    $('ul.pager li a', $root).click(function(e) {
      var ajax = info.ajax;
      var base_url = ajax.options.url;

      var param = $(this).attr('href').replace(/^[^\?]+\?/, '');
      ajax.options.url = base_url + '?' + param;
      ajax.eventResponse($root, 'loadView');
      ajax.options.url = base_url;

      e.stopPropagation();
      return false;
    });
  };
  
  /**
   * Drupal behavior - attach the widget
   * to the relevant input fields.
   */
  Drupal.behaviors.insertFromView = {
    attach: function(context, settings){
      var overlay = $('<div id="insert-from-view-overlay"></div>').appendTo('body');
      jQuery('input.insert-from-view-widget', context).each(function(){
        // Parse the input
        var info = get_info($(this));
        // Hide the input field and add our content
        info.input.css('display', 'none');
        info.widget = $('<div class="insert-from-view-widget"></div>').appendTo(info.container);
        info.overlay = overlay;
        populate_widget(info);
      });
    }
  };
  
  /**
   * Drupal theme - theme each string representing a node. The
   * input are the node id and the title (may be undefined).
   * 
   * The theme function should return a jQuery object.
   * 
   * The returned object may contain <a> elements with the class name:
   * - insert-from-view-up: to move the item up in the list;
   * - insert-from-view-down: to move the item down in the list;
   * - insert-from-view-delete: to remove the item from the list.
   */
  Drupal.theme.prototype.insertFromViewItem = function(nid, title){
    // Text
    var str = nid.toString();
    if (typeof(title) !== 'undefined'){
      str = str + ':' + title;
    }
    // Options
    str = str + ' ';
    str = str + '<a href="#" class="insert-from-view-up" title="' + Drupal.t('Move up') + '">&#9668;</a> ';
    str = str + '<a href="#" class="insert-from-view-down" title="' + Drupal.t('Move down') + '">&#9658;</a> ';
    str = str + '<a href="#" class="insert-from-view-delete" title="' + Drupal.t('Remove') + '">x</a>';
    var $obj = $('<div>' + str + '</div>');
    $obj.css({
      backgroundColor: '#EEE',
      display: 'inline',
      margin: '0 1em 0 0',
      whiteSpace: 'nowrap'
    });
    $('a.insert-from-view-delete', $obj).css({
      textDecoration: 'none',
      fontWeight: 'bold',
      color: '#F00'
    });
    return $obj;
  };
})(jQuery);