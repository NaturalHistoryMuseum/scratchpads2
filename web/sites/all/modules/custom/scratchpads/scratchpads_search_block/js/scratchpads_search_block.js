(function($){
  $(document).ready(function(){
    const $searchInput = $('input[name="search_block_form"]');
    const $hidden = $searchInput.closest('form').find('.autocomplete');

    /**
     * Enable or disable the autocomplete for the form
     * @param {boolean} value True to enable, false to disable
     */
    function toggleAutocomplete(value){
      if(!value) {
        // No easy way to remove the autocomplete, so just drop the event handlers
        $searchInput.unbind('keydown keyup blur');
        $searchInput.removeClass('form-autocomplete');
      } else {
        // To reattach, remove the "-processed" class and call attach again
        $searchInput.addClass('form-autocomplete');
        $hidden.removeClass('autocomplete-processed');
        // Remove the extra span that attach creates
        $hidden.next().remove();
        Drupal.behaviors.autocomplete.attach(document);
      }
    }

    // If the "all" facet is checked initially, remove the autocomplete
    if(document.querySelector('#block-search-form input[name="facet"]:checked').value === '_all') {
      toggleAutocomplete(false);
    }

    // Watch for facet change
    $('#block-search-form input[name="facet"]').change(function({ target }){
      // Only run this code on the radio that was just checked
      if(!target.checked) {
        return;
      }

      // Only enable if "Taxonomy" search facet is selected
      toggleAutocomplete(target.value  === '_taxonomy');
    });
  })
})(jQuery);
