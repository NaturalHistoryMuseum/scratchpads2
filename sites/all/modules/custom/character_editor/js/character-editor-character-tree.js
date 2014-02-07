(function($){
  /**
   * CharacterTreeUI
   *
   * This class represents the character tree used to show/hide characters on the slickgrid
   */
  Drupal.CharacterTreeUI = function(character_tree_mode, character_tree_width, tree, context, slickgrid){
    /**
     * init
     */
    this.init = function(){
      var that = this;
      // Settings
      this.spacing = 8;
      this.expanded_width = parseInt(character_tree_width);
      this.collapsed_width = 8;
      this.group_box_height = 32;
      this.mode = character_tree_mode;
      this.tree = tree;
      // Info
      this.$slick = $('#slickgrid', context);
      this.$wrapper = this.$slick.closest('div.slickgrid-wrapper');
      this.$root = this.$slick.closest('div.view-character-editor').parent();
      // Setup
      if (this.expanded_width > $(document).width() * 0.75){
        this.expanded_width = Math.floor($(document).width() * 0.75);
      }
      this.$wrapper.css('position', 'relative');
      this.$elem = $('<div id="character-editor-tree"></div>').css({
        float: 'left',
        margin: '0 ' + this.spacing.toString() + 'px 0 0',
        zIndex: 10 // Go above View's tool tip overlay
      }).prependTo(this.$root);
      $('div.view-character-editor', this.$root).css('margin-left', this.spacing.toString() + "px");
      // Apply intial column state
      var slick_columns = slickgrid.getColumns(true);
      for (var i = 0; i < slick_columns.length; i++){
        if (slick_columns[i].hidden && typeof this.tree[slick_columns[i].id] != 'undefined'){
          this.setItemStatus(this.tree[slick_columns[i].id], false);
        }
      }
      this.display();
    }

    /**
     * updateTree
     *
     * Update the tree with a new definition (as sent by server)
     */
    this.updateTree = function(new_tree){
      if ($.isEmptyObject(new_tree)){
        return;
      }
      for (var i in new_tree){
        var item = new_tree[i];
        if (typeof this.tree[i] !== 'undefined'){
          item.visible = this.tree[i].visible;
        } else if (typeof this.tree[item.parent] !== 'undefined'){
          item.visible = this.tree[item.parent].visible;
        }
      }
      this.tree = new_tree;
      this.setColumns();
      this.display();
    }

    /**
     * treeClick
     *
     * Callback when the tree div is clicked. This is used to expand/collapse the tree
     * div.
     */
    this.treeClick = function(e){
      this.toggle();
      if (Drupal.settings.CharacterTreeUI.editable){
        slickgrid.updateSettings('character_tree_mode', this.mode);
      }
      return false;
    }

    /**
     * display
     *
     * Display the character tree. This may be called at any time, and should be called when
     * the tree is updated and/or toggled.
     */
    this.display = function(){
      var that = this;
      if (this.mode == 'collapsed'){
        this.$elem.css('overflow', 'hidden');
        this.$elem.removeClass('character-editor-tree-expanded').addClass('character-editor-tree-collapsed');
        this.$elem.unbind('click');
        this.$elem.click($.proxy(this, 'treeClick'));
        $('div.character-editor-tree', this.$elem).stop().animate({
          opacity: 0
        }, function(){
          $(this).remove();
        });
        $('div.character-editor-tree-arrow', this.$elem).remove();
        $('div.character-editor-tree-resize', this.$elem).remove();
        $('<div></div>').addClass('character-editor-tree-arrow').html('&#9654;').prependTo(this.$elem);
        this.resize(this.collapsed_width);
        $('div.character-editor-tree-arrow', this.$elem).stop().css({
          opacity: 0
        }).animate({
          opacity: 1
        });
      } else {
        var that = this;
        this.$elem.unbind('click');
        if ($('div.character-editor-tree-arrow', this.$elem).length > 0){
          $('div.character-editor-tree-arrow', this.$elem).stop().animate({
            opacity: 0
          }, function(){
            $(this).remove();
            that.$elem.css('overflow', 'auto');
          });
        } else {
          this.$elem.css('overflow', 'auto');
        }
        $('div.character-editor-tree', this.$elem).remove();
        var $tree = $('<div></div>').addClass('character-editor-tree');
        $tree.css({
          overflow: 'auto',
          position: 'relative',
          width: this.expanded_width.toString() + "px"
        });
        $('<div>Character Tree</div>').addClass('character-editor-tree-header').appendTo($tree);
        $('<div>&otimes;</div>').addClass('character-editor-tree-close').css({
          display: 'none'
        }).appendTo($tree).fadeIn().click($.proxy(this, 'treeClick'));
        $('<div></div>').addClass('character-editor-tree-resize')
        .appendTo(this.$elem).on('mousedown', $.proxy(this, 'resizeHandleMouseDown'));
        for (var i in this.tree){
          var item = this.tree[i];
          html = item.label;
          if (item.group){
            html = '<strong>' + html + '</strong>';
          }
          html = '<div><input type="checkbox" />&nbsp;' + html + '</div>';
          var $input = $(html).addClass('character-editor-tree-item').css('margin-left', (item.depth*20).toString() + "px").appendTo($tree);
          item.elem = $input;
          item.input = $('input', $input);
          if (item.visible){
            item.input.prop('checked', true);
          }
          item.elem.click($.proxy(this, 'treeElemClick', item));
          item.elem.bind("contextmenu", $.proxy(this, 'openContextMenu', item));
          $input.hover($.proxy(this, 'treeElemHover', item));
        }
        this.$elem.removeClass('character-editor-tree-collapsed').addClass('character-editor-tree-expanded');
        this.resize(this.expanded_width);
        $tree.appendTo(this.$elem);
      }
    }

    /**
     * treeElemHover
     *
     * Callback when a tree element is hovered
     */
    this.treeElemHover = function(item, event){
      var hoverin = (event.type == 'mouseenter');
      if (item.group){
        if (hoverin){
          var box = this.groupHeaderBoundingBox(item);
          if (box.min === false || box.max === false){
            return;
          }
          // Calculate the group hover tab position
          var hover_elem_left = box.min;
          var right_border = true;
          var left_border = true;
          if (hover_elem_left < 0){
            hover_elem_left = 0;
            left_border = false;
          }
          var hover_elem_width = box.max - hover_elem_left + 6;
          if (hover_elem_left + hover_elem_width > this.$slick.width()){
            hover_elem_width = this.$slick.width() - hover_elem_left;
            right_border = false;
          }
          if (hover_elem_width > 0){
            var $hover = $('<div></div>').addClass('character-editor-tree-group-hover')
            .html('<span>' + item.label + '</span>')
            .css({
              display: 'none',
              position: 'absolute',
              top: (-this.group_box_height).toString() + "px",
              height: this.group_box_height.toString() + "px",
              left: hover_elem_left.toString() + "px",
              width: hover_elem_width.toString() + "px"
            });
            if (!right_border){
              $hover.css('border-right', 0);
              $hover.css('border-top-right-radius', 0);
            }
            if (!left_border){
              $hover.css('border-left', 0);
              $hover.css('border-top-left-radius', 0);
            }
            $hover.appendTo(this.$wrapper).fadeIn(50);
          }
        } else {
          $('.character-editor-tree-group-hover').remove();
        }
      } else {
        var header = $('div.' + item.id, this.$slick);
        if (header.length > 0){
          if (hoverin){
            header.trigger('mouseenter');
          } else {
            header.trigger('mouseleave');
          }
        }
      }
    }

    /**
     * groupHeaderBoundingBox
     *
     * Return the bounding box (min, max) that covers all the slickgrid header
     * rows for columns that are children of the given item
     */
    this.groupHeaderBoundingBox = function(item){
      var box = {
        min: false,
        max: false
      };
      var base = this.$slick.offset().left;
      for(var i in this.tree){
        var child_item = this.tree[i];
        var child_box = {};
        if (child_item.parent != item.id){
          continue;
        }
        if (child_item.group){
          child_box = this.groupHeaderBoundingBox(child_item);
        } else {
          var header = $('div.' + child_item.id, this.$slick);
          if (header.length == 0){
            continue;
          }
          child_box.min = header.offset().left - base;
          child_box.max = child_box.min + header.width();
        }
        if (child_box.min !== false && (box.min === false || child_box.min < box.min)){
          box.min = child_box.min;
        }
        if (child_box.max !== false && (box.max === false || child_box.max > box.max)){
          box.max = child_box.max;
        }
      }
      return box;
    }

    /**
     * treeElemClick
     *
     * Callback when an input element in the tree is clicked
     */
    this.treeElemClick = function(item, event){
      // Check the box if the outer element was clicked
      if (!$(event.target).is('input')){
        item.input.prop('checked', !item.input.prop('checked'));
      }
      var visible = item.input.prop('checked');
      // Remove hovers on the slickgrid header column
      this.treeElemHover(item, {type: 'mouseleave'});
      // Set the status and update the slickgrid columns
      this.setItemStatus(item, visible);
      this.setColumns();
      // Add hover on the slickgrid header column
      this.treeElemHover(item, {type: 'mouseenter'});
      // Cancel event
      event.stopPropagation();
    }

    /**
     * resizeHandleMouseDown
     *
     * Event called on mousedown on the resize handlebar
     */
    this.resizeHandleMouseDown = function(event){
      var last_position = event.pageX;
      var wrapper_diff = this.$wrapper.width() + this.expanded_width;

      $(window).bind('mousemove.characterTree', $.proxy(function(e){
        var delta = e.pageX - last_position;
        if (this.expanded_width + delta < 100 || this.expanded_width + delta > $(document).width() * 0.75 ){
          return;
        }
        last_position = e.pageX;
        this.expanded_width = this.expanded_width + delta;
        this.$wrapper.css('margin-left', (this.expanded_width + this.spacing).toString() + 'px');
        this.$wrapper.width(wrapper_diff - this.expanded_width);
        this.$elem.width(this.expanded_width);
        $('div.character-editor-tree', this.$elem).width(this.expanded_width);
      }, this));
      $(window).bind('mouseup.characterTree', $.proxy(function(){
        $(window).unbind('mousemove.characterTree');
        $(window).unbind('mouseup.characterTree');
        slickgrid.updateSettings('character_tree_width', this.expanded_width);
      }, this));
      event.preventDefault();
    }

    /**
     * setItemStatus
     *
     * Set the checked/unchecked status of an item, and propagate to parents/children
     */
    this.setItemStatus = function(item, status, noup){
      if (item.visible == status){
        return;
      }
      item.visible = status;
      if (typeof item.input !== 'undefined'){
        item.input.prop('checked', status);
      }
      noup = (typeof noup === 'undefined') ? false : noup;
      if (item.group){
        // Propagate checkbox status to children
        for (var i in this.tree){
          var child_item = this.tree[i];
          if (child_item.parent == item.id){
            this.setItemStatus(child_item, status, true);
          }
        }
      }
      // Propagate checkbox status to parent
      if (!noup){
        if(!item.visible){
          var p = item.parent;
          while(typeof this.tree[p] !== 'undefined'){
            var parent_item = this.tree[p];
            parent_item.visible = false;
            if (typeof parent_item.input !== 'undefined'){
              parent_item.input.prop('checked', false);
            }
            p = parent_item.parent;
          }
        } else {
          var p = item.parent;
          while(typeof this.tree[p] !== 'undefined'){
            var parent_item = this.tree[p];
            parent_item.visible = true;
            for (var i in this.tree){
              var parent_child_item = this.tree[i];
              if (parent_child_item.parent == parent_item.id && !parent_child_item.visible){
                parent_item.visible = false;
                break;
              }
            }
            if (typeof parent_item.input !== 'undefined'){
              parent_item.input.prop('checked', parent_item.visible);
            }
            p = parent_item.parent;
          }
        }
      }
    }

    /**
     * setColumns
     *
     * Apply the column settings to the slickgrid.
     */
    this.setColumns = function(){
      var hidden_columns = [];
      var all_columns = slickgrid.getColumns(true);
      for (var i = 0; i < all_columns.length; i++){
        var col = all_columns[i];
        if (typeof this.tree[col.field] !== 'undefined' ){
          var item = this.tree[col.field];
          if (item.visible){
            col.hidden = false;
          } else {
            col.hidden = true;
            hidden_columns.push(col.id);
          }
        } else {
          col.hidden = false;
        }
      }
      slickgrid.setColumns(all_columns);
      Drupal.characterHoverUI.refresh();
      if (Drupal.settings.CharacterTreeUI.editable){
        slickgrid.updateSettings('hidden_columns', hidden_columns);
      }
    }

    /**
     * resize
     *
     * Set the character tree UI width. This animates the resize, and resizes
     * the slickgrid at the same time.
     */
    this.resize = function(width){
      var that = this;
      if (typeof this.animation !== 'undefined'){
        this.animation.stop();
      }
      var start = this.$elem.width();
      if (start != width){
        var wrapper_width = this.$wrapper.width();
        if (typeof this.initial_resize == 'undefined'){
          this.$wrapper.css('margin-left', (width + this.spacing).toString() + 'px');
          this.$wrapper.width(wrapper_width - width + start - this.spacing);
          this.$elem.width(width);
          this.initial_resize = true;
        } else {
          this.animation = $({t: start}).animate({
            t: width
          }, {
            duration: 300,
            easing: 'swing',
            step: function(step) {
              that.$wrapper.css('margin-left', (step + that.spacing).toString() + 'px');
              that.$wrapper.width(wrapper_width - step + start);
              that.$elem.width(step);
            }
          });
        }
      }
      this.$elem.height(this.$slick.parent().height());
    }

    /**
     * toggle
     *
     * Toggle the collapsed/expanded status of the tree UI
     */
    this.toggle = function(){
      this.mode = (this.mode == 'collapsed') ? 'expanded' : 'collapsed';
      this.display();
    }

    /**
     * openContextMenu
     *
     * This is called as an event callback and should open the context menu
     */
    this.openContextMenu = function(item, e){
      // Generate the list of options that apply to this item
      var options = [];
      if (!item.group){
        // Add Go to column
        var $item = $('<div></div>').addClass('character-editor-popup-row')
        .html('Go to column').click($.proxy(function(){
          this.closeContextMenu(item);
          this.goToColumn(item.id);
        }, this));
        options.push($item);
      }
      if (options.length == 0){
        return;
      }
      // Prepare the menu
      var $menu = $('<div id="character-context-menu"></div>').css({
        position: 'absolute',
        zIndex: '100',
        left: e.pageX,
        top: e.pageY,
      }).addClass('character-editor-popup');
      $('<div></div>').addClass('character-editor-popup-header')
      .html(item.label).appendTo($menu);
      // Add the options
      for (var i = 0; i < options.length; i++){
        options[i].appendTo($menu);
      }
      // Add the menu to the document
      $menu.appendTo('body').show();
      // Add an overlay for click-out
      $('<div id="character-context-menu-out"></div>').css({
        position: 'absolute',
        top: '0',
        left: '0',
        width: $(document).width().toString() + "px",
        height: $(document).height().toString() + "px",
        zIndex: '99'
      }).appendTo('body').one('click', $.proxy(function(e){
        this.closeContextMenu(item);
      }, this));
      $(item.elem).css('background', '#DDD');
      e.preventDefault();
    }

    /**
     * closeContextMenu
     */
    this.closeContextMenu = function(item){
      $(item.elem).css('background', '');
      $('#character-context-menu').remove();
      $('#character-context-menu-out').remove();
    }

    /**
     * goToColumn
     *
     * Scroll the editor so that the given column is in view
     */
    this.goToColumn = function(id){
      var $viewport = $('#slickgrid div.slick-viewport');
      var offset = $('div.' + id).offset().left - $viewport.offset().left + $viewport.scrollLeft();
      console.log(offset);
      $viewport.animate({
        scrollLeft: offset
      });
    }

    this.init();
  }
})(jQuery);