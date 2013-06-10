jQuery(document).ready(function($){

  // Hide fields on page load if checkbox is ticked
  if($('input#edit-field-hide-unused-fields-und').is(":checked")) {
    $('.empty_text_field').hide();
  }
  
  // count the number of textareas
  var matches = 0;
  $("textarea").each(function(){
    matches++;  
  });
  
  // Show/hide fields when checkbox is ticked  
  $("input#edit-field-hide-unused-fields-und").click(function(){
    
    // ckeditor fields
    for( var c = 0; c < matches; c++) {
      var string = '#scayt_' + c;
      if($(string).length != 0) {
        var a = $(string).contents().find('.scayt-enabled').text();
        var the_id = $(string).closest("td").attr("id");
        var real_id1 = the_id.substring(13);
        var real_id2 = real_id1.substring(0, real_id1.length - 12);
        if(a != '') {
          $('#' + real_id2).removeClass('empty_text_field');
        } else {
          $('#' + real_id2).addClass('empty_text_field');
        }
      }
    }
    
    // normal textfields (plain text)
    $("textarea").each(function(){
      a = $(this).val();
      the_id = $(this).attr("id");
      var type_check = the_id.substring(0, 10);
      if(($(this).is(':visible')) && (type_check == 'edit-field')) {
        var real_id = the_id.substring(0, the_id.length - 12);
        if(a != '') {
          $('#' + real_id).removeClass('empty_text_field');
        } else {
          $('#' + real_id).addClass('empty_text_field');
        }
      }
    });

    if($('input#edit-field-hide-unused-fields-und').is(":checked")) {
      $('.empty_text_field').hide();
    } else {
      $('.empty_text_field').show();
    }
    
  

  });

});