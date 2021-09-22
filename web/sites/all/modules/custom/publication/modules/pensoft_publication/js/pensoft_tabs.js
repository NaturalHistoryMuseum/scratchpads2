(function($) {

  /**
   * This function creates javascript vertical tabs with the given elements
   * under the given root.
   */
  function pensoft_tabs($root, $elements, tab) {
    this.setup_tabs = function() {
      // Clear existing tabs
      $('div.pensoft_tabs', $root).remove();

      // Create tabs
      var $tabs_container = $('<div class="pensoft_tabs"></div>').prependTo(
          $root);
      var $tabs = $('<ul></ul>').appendTo($tabs_container);

      // Populate them
      var selected = null;
      var last = null;
      var click_function = this.on_tab_click;
      var tab_count = 0;
      $elements.each(function(i) {
        var elem_id = tab.get_id(this);
        var title = tab.get_title(this);

        var classname = 'pst_' + i;
        last = classname;

        if (tab.is_new(this)) {
          selected = classname;
          classname = classname + " pst_new";
        }

        var $tab_element = $('<li class="' + classname + '"><a href="'
            + elem_id + '">' + title + '</a></li>');
        $('a', $tab_element).click(click_function);

        $tab_element.appendTo($tabs);
        tab_count++;
      });

      if (selected == null) {
        selected = last;
      }

      if (pensoft_treatment_tab == null
          || tab_count != pensoft_treatment_tab_count) {
        $tabs.find('li.' + selected).find('a').click();
      } else {
        $tabs.find('a[href=' + pensoft_treatment_tab + ']').click();
      }
      pensoft_treatment_tab_count = tab_count;
    }

    this.on_tab_click = function() {
      $root.find('div.pensoft_tabs li').removeClass('pst_selected');
      $elements.hide();
      $(this).parent().addClass('pst_selected');
      var matchc = $(this).attr('href');
      pensoft_treatment_tab = matchc;
      $elements.filter('.' + matchc).show();
      return false;
    }

    // Returns true if at least one of the tab panes is a new entry
    this.has_new_tab = function() {
      return $root.find('div.pensoft_tabs .pst_new').length > 0;
    }

    // Returns true if at least one of the tab panes has saved data
    this.has_complete_tab = function() {
      return $('div.pensoft_tabs li:not(.pst_new)', $root).length > 0
    }

    this.setup_tabs();
  }

  /**
   * This function sets up a relation select section to use vertical tabs
   * 
   */
  function pensoft_relation_select_setup(tabset, $root, settings) {
    var $button = $('#edit-field-publication-treatments-und-add-more', $root);
    var $elements = $('div.form-type-relation-select', $root);

    // Set up the tabbed interace
    settings.is_new = function(elem) {
      return ($(elem).find('.relation-select-entities span.title').length == 0);
    };

    this.pensoft_tabs = new pensoft_tabs($root, $elements, settings);

    /* Remove extraneous 'New treatment' tabs */
    var tabs_to_remove = null;
    if (settings.reload && this.pensoft_tabs.has_complete_tab()) {
      tabs_to_remove = $('div.pensoft_tabs .pst_new', $root);
    } else if ($('div.pensoft_tabs .pst_new', $root).length > 1) {
      tabs_to_remove = $('div.pensoft_tabs .pst_new', $root).slice(0, -1)
    }
    if (tabs_to_remove != null) {
      tabs_to_remove.each(function() {
        var id = '.' + $('a', $(this)).attr('href');
        $(this).remove();
        $(id, $root).css('display', 'none'); // remove();
      });
    }

    // Place the 'Add another item' button after the last tab
    var $fieldset = $root.parents('fieldset');
    var visible = ($fieldset.css('display') != 'none');
    if (!visible)
      $fieldset.show();
    var last = $('div.pensoft_tabs ul li', $root).last();
    $button.css('left', last.position().left + last.outerWidth());
    $button.css('right', 'auto');
    if (!visible)
      $fieldset.hide();

    // Attach select/deselect events to the items
    $('div.relation-select-entities', $root).each(
        function() {
          $(this).unbind("selectItem").bind("selectItem",
              function(e, id, entitydata) {
                $('.field-type-relation-select', $root).fadeOut();
                $('.reload-button input', $root).trigger('refreshForm');
              }).unbind("deselectItem").bind("deselectItem",
              function(e, id, entitydata) {
                $('.field-type-relation-select', $root).fadeOut();
                $('.reload-button input', $root).trigger('refreshForm');
              });
        });

    // Hide the 'search' form for filled entities, and the fields for new
    // entities
    $elements.each(function() {
      if ($(this).find('.relation-select-entities span.title').length == 0) {
        $(
            '.field-name-field-publication-treatment-name,'
                + '.field-name-field-publication-treat-holotype,'
                + '.field-name-field-publication-treat-type,'
                + '.field-name-field-publication-treat-paratype,'
                + '.field-name-field-publication-treat-genus,'
                + '.field-name-field-publication-treat-species,'
                + '.field-name-field-publication-treat-authy,'
                + '.field-publication-treatments-entity-fields', $(this)).css(
            'display', 'none');
      } else {
        $('div.views-exposed-form', $(this)).css('display', 'none');
      }
    });
  }

  Drupal.behaviors.pensoft_tabs = {

    attach : function(context) {

      function init() {
        if (typeof pensoft_treatment_new == 'undefined') {
          pensoft_treatment_new = true;
          pensoft_treatment_tab = null;
          pensoft_treatment_tab_count = 0;
        } else {
          pensoft_treatment_new = false;
        }

        // Treatments section
        var reload = !$('#publication-treatments-container').hasClass(
            'pensoft_processed');
        pensoft_relation_select_setup(
            'treatments',
            $('#publication-treatments-container'),
            {
              new_page : pensoft_treatment_new,
              reload : reload,
              get_id : function(elem) {
                return elem.className
                    .match(/\bform-item-field-publication-treatments-und-\d+\b/);
              },
              get_title : function(elem) {
                var title = $(elem)
                    .find('.relation-select-entities span.title').html();
                if (title == null) {
                  title = 'New treatment';
                } else {
                  title = 'Treatment: ' + title;
                }

                return title;
              }
            });
        $('#publication-treatments-container').addClass('pensoft_processed');
      }

      init();

    }

  };
})(jQuery);
