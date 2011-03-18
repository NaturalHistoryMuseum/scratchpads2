(function ($) {

/**
 * @file
 * UI addition to enhance the import source switcher
 *
 */

Drupal.behaviors.filtering_fieldset = {
  attach : function(context) {
    console.log('Setting up filtered fieldset');
    $('.filtering-fieldset').addClass('filtered');
    $('.filtering-fieldset .filtering-selector')
      .addClass('filtering-trigger')
      .change(
      /*
      When the select changes, we change the class of the containing fieldset.
      and hide/show all the contents depending on what was selected.
      */
        function() {
          $('.filtering-fieldset .filtered-fieldset').hide().removeClass('filter-selected');
          console.log('filtering to show ' + $(this).val());
          if ($(this).val()) {
            $('.filtering-fieldset .filtered-fieldset-' + $(this).val() ).show().addClass('filter-selected');
          }
        }
      );
    // Trigger the filter to update the current display
    $('.filtering-fieldset .filtering-selector').trigger('change');
    console.log('Fieldset filtered');
  }
}

})(jQuery);