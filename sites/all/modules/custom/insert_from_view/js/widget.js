(function($){
  // Store the info for all the insert from view widgets on the page
  var all_info = {};

  /**
   * NodeList
   * 
   * This class represents an ordered list of nodes.
   * Each node is an object with two properties: 'nid' and
   * 'title', such that 'nid' is always a string (not an int)
   * and 'title' may be undefined.
   * 
   * All functions that take in nodes can be called with a node
   * object, a node in string reprentation ("<nid>[:<title>][,....]")
   * or an array of node objects/node string representations.
   */
  function NodeList(init_str){
    this.nodes = {};
    this.order = [];
    
    /**
     * constructor
     */
    this.init = function(){
      if (typeof(init_str) !== 'undefined'){
        this.addNode(init_str);
      }
    }

    /**
     * Return the number of nodes
     */
    this.length = function(){
      return this.order.length;
    };
    
    /**
     * Return the node at the given position
     * 
     */
    this.getNode = function(index){
      if (this.order.length > index){
        return this.nodes[this.order[index]];
      } else {
        return false;
      }
    };
    
    /**
     * Return true if the give node(s) is/are
     * present in the list, and false otherwise.
     */
    this.hasNode = function(node){
      var nodes = this.toNodes(node);
      for (var i = 0; i < nodes.length; i++){
        if (typeof this.nodes[nodes[i].nid] === 'undefined'){
          return false;
        }
      }
      return true;
    };
    
    /**
     * Adds a node or list of nodes at the end of
     * the list
     */
    this.addNode = function(node){
      var nodes = this.toNodes(node);
      for (var i = 0; i < nodes.length; i++){
        this.nodes[nodes[i].nid] = nodes[i];
        if (this.order.indexOf(nodes[i].nid) === -1){
          this.order.push(nodes[i].nid);
        }
      }
    };
    
    /**
     * Remove a node or list of nodes from the list
     */
    this.removeNode = function(node){
      var nodes = this.toNodes(node);
      for (var i = 0; i < nodes.length; i++){
        if (typeof this.nodes[nodes[i].nid] !== 'undefined'){
          delete this.nodes[nodes[i].nid];
        }
        if (this.order.indexOf(nodes[i].nid) !== -1){
          this.order.splice(this.order.indexOf(nodes[i].nid), 1);
        }
      }
    };
    
    /**
     * Return the list of nodes as a node string representation
     */
    this.toString = function(){
      var values = [];
      for (var i = 0; i < this.order.length; i++){
        var node = this.nodes[this.order[i]];
        var str = node.nid;
        if (typeof node.title !== 'undefined'){
          str = str + ':' + node.title.replace(/,/, ' ');
        }
        values.push(str);
      }
      return values.join(',');
    };
    
    /**
     * Change the order of the nodes
     */
    this.setOrder = function(order){
      this.order = order;
    };
    
    /**
     * Given a node object, a node string representation,
     * an array of node objects or node string representations
     * return an array of node objects.
     * 
     */
    this.toNodes = function(node){
      if (typeof node == 'string') {
        if (node.indexOf(',') !== -1){
          return this.toNodes(node.split(','));
        } else {
          var matches = node.match(/^(\d+)(?::(.*))?$/);
          if (matches === null || typeof(matches[1]) === 'undefined'){
            return [];
          }
          return [{
            nid: matches[1],
            title: matches[2]
          }];
        }
      } else if ($.isArray(node)){
        var nodes = [];
        for (var i = 0; i < node.length; i++){
          nodes = nodes.concat(this.toNodes(node[i]));
        }
        return nodes;
      } else {
        return [node];
      }
    };
    
    this.init();
  }
  
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
    if (typeof Drupal.settings['insert_from_view_' + machine_name] == 'undefined'){
      throw "No settings for insert from view widget " + machine_name;
    }
    settings = Drupal.settings['insert_from_view_' + machine_name];
    // Set value and return
    info = {
      input: $input,
      container: $input.parent(),
      machine_name: machine_name,
      nodes: new NodeList($input.val()),
      settings: settings,
      changed: {
        added: [],
        removed: []
      }
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
    info.input.attr('value', info.nodes.toString());
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
      return false;
    });
    // And the list of nodes
    var $ul = $('<ul class="insert-from-view-sortable"></ul>').appendTo(info.widget);
    for (var index = 0; index < info.nodes.length(); index++){
      var node = info.nodes.getNode(index);
      var node_obj = Drupal.theme('insertFromViewItem', node.nid, node.title);
      $('.insert-from-view-delete', node_obj).click((function(node){
        return function(){
          info.nodes.removeNode(node);
          populate_input(info);
          populate_widget(info);
        };
      })(node));
      var $li = $('<li ifw="' + node.nid + '"></li>').css('float', 'left').appendTo($ul);
      node_obj.appendTo($li);
    }
    $ul.sortable({
      handle: '.insert-from-view-handle',
      cursor: 'move',
      update: function(event, ui){
        var order = $ul.sortable('toArray', {attribute: 'ifw'});
        info.nodes.setOrder(order);
        populate_input(info);
      }
    });
    $('<br/>').css('clear', 'both').appendTo(info.widget);
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
      fixed: true,
      inline : true,
      iframe : false,
      href : info.overlay,
      width : info.settings.width,
      height : info.settings.height,
      onClosed : function() {
        info.overlay.empty();
        info.changed = {
          added:[],
          removed:[]
        }
      }
    });
    info.ajax = ajax;
    info.changed = {
      added: [],
      removed: []
    };
    ajax.eventResponse(info.overlay, 'loadView');
  }

  /**
   * update_overlay_changes
   * 
   * Update the values added/removed from the overlay
   */
  function update_overlay_changes(info){
    // Give other scripts a chance to react/change the values
    info.input.trigger('insertFromView-insert', [info.changed.added, info.settings]);
    info.input.trigger('insertFromView-remove', [info.changed.removed, info.settings]);
    // Insert values
    info.nodes.addNode(info.changed.added);
    info.nodes.removeNode(info.changed.removed);
    populate_input(info);
    populate_widget(info);
    info.changed = {
      added:[],
      removed: []
    }
  }
  
  /**
   * Overlay callback function
   */
  $.fn.insert_from_view_setup_field_view = function(machine_name, insert_array) {
    var info = all_info[machine_name];
    var $root = $("#insert-from-view-overlay");
    // Handle clicking on an individual fields.
    // We only do this if there are no checkboxes as
    // the combination of the two (eg. untick a checkbox, then click
    // on a row) does not have an intuitive result.
    if ($('.insert_from_view_checkbox', $root).length == 0) {
      $('.insert-from-view-row', $root).click(function(e) {
        info.changed.added.push(insert_array[$(this).index()]);
        update_overlay_changes(info);
        $.colorbox.close();
      });
    }
    
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

    // Adds a 'select all' selector. XXX only works for table displays
    if ($('th.views-field-insert-from-view-checkbox', $root).length > 0){
      $('<input type="checkbox" />').change(function(){
        var checked = $(this).prop('checked');
        $(this).closest('table').find('input.insert_from_view_checkbox').prop('checked', checked).change();
      }).appendTo('th.views-field-insert-from-view-checkbox', $root);
      $('input.insert_from_view_checkbox').change(function(){
        if (!$(this).prop('checked')){
          $(this).closest('table').find('th.views-field-insert-from-view-checkbox input').prop('checked', false);
        }
      });
    }

    // Create & handle multiple inserts. This work by updating the
    // existing values - so items can be added as well as removed.
    if ($('.insert_from_view_checkbox', $root).length > 0) {
      // Pre-tick existing values
      $('.insert_from_view_checkbox', $root).each(function(){
        var index = $(this).parents('.insert-from-view-row').index();
        if (info.nodes.hasNode(insert_array[index])){
          $(this).prop('checked', true);
        }
      });
      // Track clicks on all tickboxes
      $('.insert_from_view_checkbox', $root).change(function(){
        var index = $(this).parents('.insert-from-view-row').index();
        var value = insert_array[index];
        // Remove for all change lists
        if (info.changed.added.indexOf(value) != -1){
          info.changed.added.splice(info.changed.added.indexOf(value), 1);
        }
        if (info.changed.removed.indexOf(value) != -1){
          info.changed.removed.splice(info.changed.removed.indexOf(value), 1);
        }
        if (!$(this).prop('checked') && info.nodes.hasNode(value)){
          info.changed.removed.push(value);
        }
        if ($(this).prop('checked') && !info.nodes.hasNode(value)){
          info.changed.added.push(value);
        }
      });
      $('<input type="button" class="insert-from-view-button form-submit" value="'+ Drupal.t('Update') + '" />')
      .appendTo($('div.view', $root))
      .mousedown(function() {
        update_overlay_changes(info);
        $.colorbox.close();
        return false;
      });
    }

    // Handle exposed forms
    var $widgets_root = $('div.views-exposed-form div.views-exposed-widgets', $root);
    if ($widgets_root.length > 0) {
      $widgets_root.closest('form').submit(function(e){
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
   */
  Drupal.theme.prototype.insertFromViewItem = function(nid, title){
    // Text
    var str = '<div class="insert-from-view-item"><span class="insert-from-view-handle">&harr;</span> '
    str = str + '<span class="insert-from-view-item-description">' + nid.toString();
    if (typeof(title) !== 'undefined'){
      str = str + ':' + title;
    }
    str = str + '</span> ';
    str = str + '<span class="insert-from-view-delete" title="' + Drupal.t('Remove') + '">x</span>';
    str = str + '</div>';
    return $(str);
  };
})(jQuery);