(function($){
  $(document).ready(function(){
    $('input[name="entity_type_value"]').change(function(event){
      if($('input[name="entity_type_value"]:checked').first().attr('value') == '_taxonomy') {
        $('input[name="search_block_form"]').addClass('form-autocomplete');
      } else {
        $('input[name="search_block_form"]').removeClass('form-autocomplete');
      }
    });
  })
})(jQuery);