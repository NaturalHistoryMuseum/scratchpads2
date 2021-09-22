/**
 * Relation select file
 */
(function($){

  var relationSelectItems = {};

  Drupal.behaviors.relationSelect = {

    attach: function(context){

      /**
       * Add event handlers etc.,
       */
      function init(){

        // Close the pop up when document is clicked
        $(document).click(handleCloseClick);

        addFilterEventHandlers(context);

        // Build object containing relation select input
        $('.relation-select-entities:not(.processed)')
            .each(
                function(){

                  var relationType = $(this).attr("data-relation-type");

                  // Make endpoints sortable.
                  if ($(this).attr('data-relation-arity') != 1) {
                    $(this).sortable({placeholder:'rs-placeholder'});
                  }

                  relationSelectItems[$(this).attr("id")] = {
                    element: $(this), // Cache a copy to speed up access
                    maxArity: Drupal.settings.relationSelect[relationType]['maxArity']
                  };

                  addInputEventHandlers($(this));

                  addFilterBundleEventHandlers($(this).parents(
                      '.field-type-relation'), relationType);

                }).addClass('processed');

        addCloseEventHandlers(context);
        addRowEventHandlers(context);

      }

      // Events

      // Event handler: close button clicked
      function addCloseEventHandlers(context){
        
        $('.relation-select-close:not(.processed)', context).click(
            handleCloseClick).addClass('processed');

        $('.view-relation-select-node', context).click(function(e){
          e.stopPropagation();
        })

      }

      function addRowEventHandlers(context){        
        // Check selected & add event handler: row clicked
        $('.views-view-relation-select tr', context).once(
            function(){
              id = $(this).parents('.relation-select-views-output').siblings(
                  '.relation-select-entities').attr("id");
              var entitydata = $(this).attr('data-entity');
              if(getInput(id, entitydata).length) {
                selectItem($(this), id, entitydata);
              }
              $(this).click(function(){
                handleRowClick(id, $(this));
                return false;
              });

            });
      }

      function addInputEventHandlers(context){
        $('div.rs-wrapper:not(.processed)', context).each(function(){
          addInputRemoveLink($(this));
        }).addClass('processed');
      }

      function addInputRemoveLink($input){
        $input.append($('<a class="rs-remove-item">Remove</a>').click(
            handleRemoveClick));
      }

      function addFilterEventHandlers(context){

        // Add submit on change event handlers
        $('.rs-on-change').change(submit);

      }

      function addFilterBundleEventHandlers(context, relationType){
        if(Drupal.settings.relationSelect[relationType]['targetBundles']) {
          var $entityTypeSelect = $('select[name$="entity_type]"]', context);
          var $bundleSelect = $('select[name$="bundle]"]', context);

          if($entityTypeSelect.length && $bundleSelect.length) {

            $entityTypeSelect
                .change(function(){
                  filterBundles(
                      Drupal.settings.relationSelect[relationType]['targetBundles'],
                      $(this).val(), $bundleSelect);
                });

            // Filter bundles on load too
            filterBundles(
                Drupal.settings.relationSelect[relationType]['targetBundles'],
                $entityTypeSelect.val(), $bundleSelect);

          }

        }

      }

      // Event Handlers

      function handleRemoveClick(){
        $input = $('input', $(this).parents('.rs-wrapper'));
        id = $(this).parents('.relation-select-entities').attr("id");
        entitydata = $input.val();
        deselectItem($input, id, entitydata);
        return false;
      }

      function handleCloseClick(){
        close();
      }

      /**
       * An item has been selected
       */
      function handleRowClick(id, $row){
        var entitydata = $row.attr('data-entity');
        $input = getInput(id, entitydata);
        // Is it in the selectedItems array
        if($input.length) {
          // Already in the array so item is being toggled off
          deselectItem($input, id, entitydata. $row);
        } else {

          // Can anymore be selected?
          if(countArity(id) == relationSelectItems[id]['maxArity']) {

            // If only one item is being selected, assume user is changing the
            // item
            if(relationSelectItems[id]['maxArity'] == 2) {
              // Deselect original
              deselectItem($(
                  'input', relationSelectItems[id]['element']), id, entitydata, $row.parents('.view-content').find('tr.selected'));
              // Select new row
              addInput($row, id, entitydata);
              selectItem($row, id, entitydata);              
            }

          } else {
            addInput($row, id, entitydata);
            selectItem($row, id, entitydata);
          }
          // If max arity has been reached, close the popup
          if(countArity(id) == relationSelectItems[id]['maxArity']) {
            close();
          }

        }
      }

      /**
       * Close the drop down
       */
      function close(){
        $('.relation-select-views-output div').empty();
      }

      /**
       * Get input
       */
      function getInput(id, entitydata){
        return $('input[value="' + entitydata + '"]',
            relationSelectItems[id]['element']);
      }

      function countArity(id){
        // Count the arity (number of relations)
        // Always +1 as the edited / new node counts too
        return $('input', relationSelectItems[id]['element']).length + 1;
      }

      /**
       * Create an input
       */
      function addInput($row, id, entitydata){
        // Create a hidden input to mark the new item being selected
        $input = $('<div class="rs-wrapper" />');
        $input.append($('<input type="hidden" value="' + entitydata
            + '" name="'
            + relationSelectItems[id]['element'].attr("data-field-name")
            + '[]" />'));
        $input.append(themeValue($row, entitydata));
        addInputRemoveLink($input);
        relationSelectItems[id]['element'].append($input);
      }

      /**
       * Select a row
       */
      function selectItem($row, id, entitydata){         
        // Trigger custom event
        relationSelectItems[id]['element'].trigger('selectItem', [id, entitydata]);
        $row.addClass('selected');
      }

      /**
       * De-select a row
       */
      function deselectItem($input, id, entitydata, $row){
        // Remove selected class if this is for a row
        if(typeof $row != 'undefined'){
          $row.removeClass('selected');
        }
        // Destroy the form element
        $input.parents('.rs-wrapper').remove();
        relationSelectItems[id]['element'].trigger('deselectItem', [id, entitydata]);
      }

      function themeValue($row, entitydata){

        var entityType = entitydata.split(':').shift();
        var bundle = $row.attr('data-bundle');

        // Try a number of javscript template functions
        // RelationSelect+bundle+Value
        // RelationSelect+entityType+Value
        // RelationSelectValue
        var themeHooksuggestions = ['RelationSelect' + bundle + 'Value',
            'RelationSelect' + entityType + 'Value', 'RelationSelectValue'];
        for(i in themeHooksuggestions) {
          if(typeof Drupal.theme.prototype[themeHooksuggestions[i]] == 'function') {
            return Drupal.theme(themeHooksuggestions[i], $row, entitydata,
                entityType, bundle);
          }
        }
      }

      function submit(){
        $('input[type="submit"]', $(this).parents('div.views-exposed-form'))
            .mousedown();
      }

      function filterBundles(bundles, entityType, $bundleSelect){

        $.each($('option', $bundleSelect), function(i, op){

          if($(op).val() == 'All'
              || bundles[entityType].indexOf($(op).val()) !== -1) {
            $(op).removeAttr("disabled");
          } else {
            $(op).attr("disabled", "disabled");
          }

        });

        // Set the select to the first enabled value
        $bundleSelect
            .val($('option:not(:disabled)', $bundleSelect).eq(0).val());

      }

      init();

    }
  };

  /**
   * Provide the HTML to create the selected item Javascript equivalent of
   * relation_select_value
   */
  Drupal.theme.prototype.RelationSelectValue = function($row, entitydata,
    entityType, bundle){
    var html = '';
    if((label = $('td.label', $row)).length) {
      html += '<span class="title">' + $(label).text() + '</span>';
      html += ' (' + entitydata + ')';
    } else {
      html += entitydata;
    }
    return html;
  }

})(jQuery);
