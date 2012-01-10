(function($){
  $(document).ready(function(){
    $('input[name="entity_type_value"]').change(function(event){
      if($('input[name="entity_type_value"]:checked').first().attr('value') == '_taxonomy') {
        $('input[name="search_block_form"]').addClass('form-autocomplete');
      } else {
        $('input[name="search_block_form"]').removeClass('form-autocomplete');
      }
    });
    $('input[name="search_block_form"]').focus(function(event){
      $('input[name="search_block_form"]').parent().parent().siblings('div').find('fieldset.collapsed legend span a').click();
    });
    $('input[name="entity_type_value"]:checked').first().change();
  })
})(jQuery);