(function($) {
  Drupal.behaviors.pensoft = {

    attach : function(context) {
      /**
       * Callback when some references have been added to a textarea
       */
      function insert_references(inserts, settings) {
        // Parse input and add to list of references to add
        var $elem = $('.pensoft_add_multiple_references input');
        var values = [];
        var warning = [];
        var map = {};
        if ($.trim($elem.val()) != "") {
          values = $elem.val().split(",");
        }

        for ( var i = 0; i < inserts.length; i++) {
          var matches = inserts[i].match(/^(\d+):\((.*)\)(\d+)$/);
          if (matches != null && matches.length > 0) {
            var nid = matches[1];
            var year = matches[3];
            var authors = matches[2].split("&&");
            var authors_str = '';
            if (authors.length > 2) {
              authors_str = authors[0] + " " + Drupal.t("et al.");
            } else {
              authors_str = authors.join(" " + Drupal.t("and") + " ");
            }
            inserts[i] = '<reference-citation citation_id="' + nid
                + '" contenteditable="false" title="'
                + Drupal.t('Reference citation. Right click to edit.')
                + '">' + authors_str + " " + year + '</reference-citation>';

            if (Drupal.settings["pensoft"].references_map[nid] == undefined) {
              Drupal.settings["pensoft"].references_map[nid] = true;
              warning.push(inserts[i]);
            }

            values.push(nid);
            map[inserts[i]] = nid;
          }
        }

        $elem.val(values);

        // Warn the user they will be available after save
        var $warn = $('#edit-group_publication_references div.fieldset-content div.pensoft_add_multiple_references_warning');
        if ($warn.length == 0 && warning.length > 0) {
          $warn = $(
              '<div class="pensoft_add_multiple_references_warning warning"><p>'
                  + Drupal
                      .t('The following references have been added inline and will be available once the publication is saved:')
                  + '<p><ul></ul></div>').prependTo(
              $('#edit-group_publication_references div.fieldset-content'));
        }

        for ( var i = 0; i < warning.length; i++) {
          $('ul', $warn).append(
              '<li class="pensoft_add_multiple_references">' + warning[i]
                  + '</li>');
        }
      }

      /**
       * Callback when some figures have been added to a textarea
       */
      function insert_figures(inserts, settings) {
        // Parse input and add to list of figures to add
        var $elem = $('.pensoft_add_multiple_figures input');
        var values = [];
        var warning = [];
        var map = {};
        if ($.trim($elem.val()) != "") {
          values = $elem.val().split(",");
        }

        for ( var i = 0; i < inserts.length; i++) {
          // Get fid
          // var matches =
          // inserts[i].match(/insert_from_view_file_(\d+)([^\d]|$)/);
          var matches = inserts[i].match(/^(\d+):(.+)$/);
          if (matches != null && matches.length > 0) {
            var fid = matches[1];
            var filename = matches[2];
            var tpl = '<fig-citation citation_id="%" contenteditable="false" title="'
              + Drupal.t('Figure citation. These will be re-numbered when the publication is finalised.')
              + '">Figure [filename:~]</fig-citation>';
            if (Drupal.settings["pensoft"].figures_map[fid] != undefined) {
              inserts[i] = tpl.replace('%', fid).replace('~', filename);
            } else {
              inserts[i] = tpl.replace('%', fid).replace('~', filename);
              Drupal.settings["pensoft"].figures_map[fid] = fid;
              warning.push(inserts[i]);
            }
            values.push(fid);
            map[inserts[i]] = fid;
          }
        }

        $elem.val(values);

        // Warn the user they will be available after save
        var $warn = $('#edit-group_publication_figures div.fieldset-content div.pensoft_add_multiple_figures_warning');
        if ($warn.length == 0 && warning.length > 0) {
          $warn = $(
              '<div class="pensoft_add_multiple_figures_warning warning"><p>'
                  + Drupal
                      .t('The following figures have been added inline and will be available once the publication is saved:')
                  + '<p><ul></ul></div>').prependTo(
              $('#edit-group_publication_figures div.fieldset-content'));
        }

        for ( var i = 0; i < warning.length; i++) {
          $('ul', $warn).append(
              '<li class="pensoft_add_multiple_figures">' + warning[i]
                  + '</li>');
        }

      }

      /**
       * Callback when a specimen table has been added to a textarea
       */
      function insert_specimens(inserts, settings) {
        if (inserts.length == 0) {
          return;
        }

        // Read the headers that were selected
        var fields = [];
        var headers = [];
        $(
            'div.view-publication-create-specimens div.view-content th input.psp_header')
            .each(function() {
              var field = $(this).attr('name');
              if ($(this).attr('checked')) {
                headers.push($(this).parent().text());
                fields.push('.pst_' + field);
              }
            });

        if (fields.length == 0) {
          inserts.length = 0;
          return

        }

        var classes = fields.join(',');
        for ( var i = 0; i < inserts.length; i++) {
          var text = '';
          $(inserts[i]).filter(classes).each(function() {
            text = text + $(this).wrap('<div>').parent().html();
          });
          inserts[i] = text;
        }

        inserts.unshift('<th>' + headers.join('</th><th>') + '</th>');
      }

      function init() {
        // Bind to the text area insert from view insert event
        $('div.form-type-textarea textarea:not(.pensoft_processed)', context)
            .bind('insertFromView-insert', function(event, inserts, settings) {
              if (settings.name == 'Pensoft publication references') {
                insert_references(inserts, settings);
              } else if (settings.name == 'Pensoft publication figures') {
                insert_figures(inserts, settings);
              } else if (settings.name == 'Pensoft publication specimens') {
                insert_specimens(inserts, settings);
              }
            });

        $('div.form-type-textarea textarea:not(.pensoft_processed)', context)
            .addClass('pensoft_processed');
      }

      init();

    }
  };
})(jQuery);