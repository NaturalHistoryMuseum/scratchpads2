/**
 * Charcter project JS
 */
(function($){
  /**
   * CharacterTreeUI
   *
   * This class represents the character tree used to show/hide characters on the slickgrid
   */
  function CharacterTreeUI(character_tree_mode, context){
    /**
     * init
     */
    this.init = function(){
      var that = this;
      // Settings
      this.spacing = 8;
      this.expanded_width = 200;
      this.collapsed_width = 8;
      this.group_box_height = 32;
      this.mode = character_tree_mode;

      // Info
      this.$slick = $('#slickgrid', context);
      this.$root = this.$slick.closest('div.view-character-editor').parent();
      // Setup
      this.$elem = $('<div id="character-editor-tree"></div>').css({
        float: 'left',
        margin: '0 ' + this.spacing.toString() + 'px 0 0'
      }).prependTo(this.$root);
      $('div.view-character-editor', this.$root).css('margin-left', this.spacing.toString() + "px");
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
          width: this.expanded_width.toString() + "px"
        });
        $('<div>Character Tree</div>').addClass('character-editor-tree-header').appendTo($tree);
        $('<div>&otimes;</div>').addClass('character-editor-tree-close').css({
          display: 'none'
        }).appendTo($tree).fadeIn().click($.proxy(this, 'treeClick'));
        for (var i in Drupal.settings.CharacterTreeUI.liveTree){
          var item = Drupal.settings.CharacterTreeUI.liveTree[i];
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
          if (box.min < 0){
            return;
          }
          // Calculate the group hover tab position
          var hover_elem_top = this.$root.offset().top - this.group_box_height;
          var hover_elem_left = box.min;
          var right_border = true;
          var left_border = true;
          if (hover_elem_left < this.$slick.offset().left){
            hover_elem_left = this.$slick.offset().left;
            left_border = false;
          }
          var hover_elem_width = box.max - hover_elem_left + 6;
          if (hover_elem_left + hover_elem_width > this.$slick.width() + this.$slick.offset().left){
            hover_elem_width = this.$slick.width() + this.$slick.offset().left - hover_elem_left;
            right_border = false;
          }
          if (hover_elem_width > 0){
            var $hover = $('<div></div>').addClass('character-editor-tree-group-hover')
            .html('<span>' + item.label + '</span>')
            .css({
              display: 'none',
              position: 'absolute',
              top: hover_elem_top.toString() + "px",
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
            $hover.appendTo('body').fadeIn(50);
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
        min: -1,
        max: -1
      };
      for(var i in Drupal.settings.CharacterTreeUI.liveTree){
        var child_item = Drupal.settings.CharacterTreeUI.liveTree[i];
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
          child_box.min = header.offset().left;
          child_box.max = child_box.min + header.width();
        }
        if (box.min < 0 || child_box.min < box.min){
          box.min = child_box.min;
        }
        if (child_box.max > box.max){
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
        for (var i in Drupal.settings.CharacterTreeUI.liveTree){
          var child_item = Drupal.settings.CharacterTreeUI.liveTree[i];
          if (child_item.parent == item.id){
            this.setItemStatus(child_item, status, true);
          }
        }
      }
      // Propagate checkbox status to parent
      if (!noup){
        if(!item.visible){
          var p = item.parent;
          while(typeof Drupal.settings.CharacterTreeUI.liveTree[p] !== 'undefined'){
            var parent_item = Drupal.settings.CharacterTreeUI.liveTree[p];
            parent_item.visible = false;
            if (typeof parent_item.input !== 'undefined'){
              parent_item.input.prop('checked', false);
            }
            p = parent_item.parent;
          }
        } else {
          var p = item.parent;
          while(typeof Drupal.settings.CharacterTreeUI.liveTree[p] !== 'undefined'){
            var parent_item = Drupal.settings.CharacterTreeUI.liveTree[p];
            parent_item.visible = true;
            for (var i in Drupal.settings.CharacterTreeUI.liveTree){
              var parent_child_item = Drupal.settings.CharacterTreeUI.liveTree[i];
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
      // XXX we rely on slickgrid module internals :( Specifically the fact that 'columns' is a global.
      var cols = [];
      var hidden_columns = [];
      $(columns).each(function(i, col){
        if (typeof Drupal.settings.CharacterTreeUI.liveTree[col.field] !== 'undefined' ){
          var item = Drupal.settings.CharacterTreeUI.liveTree[col.field];
          if (item.visible){
            cols.push(col);
          } else {
            hidden_columns.push(col.id);
          }
        } else {
          cols.push(col);
        }
      });
      grid.setColumns(cols);
      Drupal.CharacterEditor.initBT();
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
        if (typeof this.initial_resize == 'undefined'){
          $('div.slickgrid-wrapper').css('margin-left', (width + this.spacing).toString() + 'px');
          this.$elem.width(width);
          this.initial_resize = true;
        } else {
          this.animation = $({t: start}).animate({
            t: width
          }, {
            duration: 300,
            easing: 'swing',
            step: function(step) {
              $('div.slickgrid-wrapper').css('margin-left', (step + that.spacing).toString() + 'px');
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

    this.init();
  }

  Drupal.CharacterEditor = {slickgridInit: function(){
    Drupal.CharacterEditor.initBT();
  }, initBT: function(){
    var ops = {
        cssClass: "character-editor-header",
        fill: 'rgba(0, 0, 0, .7)',
        cssStyles: {color: 'white', 'font-size': '10px'},
        spikeLength: 8,
        shrinkToFit: true,
        offsetParent: '#slickgrid',
        positions: ['bottom']
    };
    $.each(grid.getColumns(), function(i, col){
      if(col.field != "character_entity_field") {
        $('.' + col.id).bt(col.data.char, ops);
      }
    });
    ops.contentSelector = Drupal.CharacterEditor.cellHover;
    $('div.slick-cell:not(.l0, :empty)').bt(ops);
  }, cellHover: function(){
    var cellText = $(this).html();
    if(cellText.length > 2) {
      return cellText;
    }
    return false;
  }}
  Drupal.behaviors.characterEditor = {attach: function(context, settings){
    var $slick = $('#slickgrid', context);
    // Overlay width fix
    if(typeof settings.overlay === 'undefined') {
      $slick.parent().width($('#overlay-content').find('#content').width());
    }
    $slick.bind('onSlickgridInit', Drupal.CharacterEditor.slickgridInit);
    $slick.bind('onSlickgridTabChanged', Drupal.CharacterEditor.slickgridInit);
    // Add the character tree widget
    if (typeof Drupal.characterTreeUI === 'undefined'){
      $slick.bind('onSlickgridInit', function(){
        Drupal.settings.CharacterTreeUI.liveTree = Drupal.settings.CharacterTreeUI.tree;
        Drupal.settings.CharacterTreeUI.tree = {};
        Drupal.characterTreeUI = new CharacterTreeUI(Drupal.settings.CharacterTreeUI.mode, context);
        // XXX We rely on slickgrid internals (specifically that 'columns' is a global)
        for (var i in columns){
          if (columns[i].hidden && typeof Drupal.settings.CharacterTreeUI.liveTree[columns[i].id] != 'undefined'){
            Drupal.characterTreeUI.setItemStatus(Drupal.settings.CharacterTreeUI.liveTree[columns[i].id], false);
          }
        }
        Drupal.characterTreeUI.display();
      });
    } else if (!$.isEmptyObject(Drupal.settings.CharacterTreeUI.tree)) {
      var old_tree = Drupal.settings.CharacterTreeUI.liveTree;
      Drupal.settings.CharacterTreeUI.liveTree = Drupal.settings.CharacterTreeUI.tree;
      Drupal.settings.CharacterTreeUI.tree = {};
      for (var i in Drupal.settings.CharacterTreeUI.liveTree){
        var item = Drupal.settings.CharacterTreeUI.liveTree[i];
        if (typeof old_tree[i] !== 'undefined'){
          item.visible = old_tree[i].visible;
        } else if (typeof old_tree[item.parent] !== 'undefined'){
          item.visible = old_tree[item.parent].visible;
        }
      }
      Drupal.characterTreeUI.setColumns();
      Drupal.characterTreeUI.display();
    }
  }}
})(jQuery);