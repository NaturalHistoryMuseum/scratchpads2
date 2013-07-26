jQuery(document).ready(function($){
  var obj = {};
  // Initialise object
  $("div.taxon_field_wrapper").each(function(){
    var this_fieldset_id = $(this).closest("fieldset.vertical-tabs-pane").attr("id");
    if(typeof this_fieldset_id != 'undefined') {
      obj[this_fieldset_id] = 0;
    }
  });
  // Count members of fieldset
  $("div.taxon_field_wrapper").each(function(){
    var this_fieldset_id = $(this).closest("fieldset.vertical-tabs-pane").attr("id");
    if(typeof this_fieldset_id != 'undefined') {
      var no_show = $(this).hasClass('unused_text_field');
      if(no_show == false) {
        obj[this_fieldset_id] = obj[this_fieldset_id] + 1;
      }
    }
  });
  // Add 'non_empty_tab' class to tabs that have content
  var simple_array = new Array();
  for( var propt in obj) {
    simple_array.push(obj[propt]);
  }
  $count = 0;
  $(".field-group-tabs-wrapper li.vertical-tab-button").each(function(){
    if(simple_array[$count] > 0) {
      $(this).addClass('non_empty_tab');
    }
    $count++;
  });
});