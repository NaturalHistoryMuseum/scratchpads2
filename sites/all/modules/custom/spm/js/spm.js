jQuery(document).ready(function($){
  
  // Create an object for each vertical tab
  // This will be used to store the number of non-empty associated fields
  var obj = {}; 
  var matches = 0;
  $("textarea").each(function(){
    matches++;      
    var textarea_id = $(this).attr("id"); 
    var field_set_id = $('#' + textarea_id).closest("fieldset").attr("id");
    obj[field_set_id] = 0;   
  });
  
  // Add a count of non-empty fields to the object
  $("textarea").each(function(){
    a = $(this).val();
    the_id = $(this).attr("id");
    var type_check = the_id.substring(0, 10);
    if(type_check == 'edit-field') {
      var real_id = the_id.substring(0, the_id.length - 12);
      if(a != '') {            
        var this_fieldset_id = $('#' + real_id).closest("fieldset").attr("id");
        obj[this_fieldset_id] = obj[this_fieldset_id] +1;           
      } 
    }
  });
  var m = checkmap();
  check_tabs();
  show_and_hide();
    
  
  // Show/hide fields and tabs when checkbox is ticked  
  $("input#edit-field-hide-unused-fields-und").click(function(){
       
    // reset object count
    for(var propt in obj){
      obj[propt] = 0;   
    }   
    // check which fields are empty
    check_ckeditor_fields();
    check_textarea_fields();   
    // check which tabs are empty
    check_tabs();     
    // show and hide fields tabs
    show_and_hide();
       
  });
  
  // Hide/show tabs and fields
  function show_and_hide(){ 
    if($('input#edit-field-hide-unused-fields-und').is(":checked")) {
      $('.empty_text_field').hide();
      $('.empty_tab').hide();
      $('.empty_map').hide();
    } else {
      $('.empty_text_field').show();
      $('.empty_tab').show();
      $('.empty_map').show();     
      // needed for maps 
      $("#active-vertical-tab").trigger('click');
    }        
  }
 
  // Check ckeditor fields to see if they are empty,
  // then add or remove the css classes
  function check_ckeditor_fields(){  
    for( var c = 0; c < matches; c++) {
      var string = '#scayt_' + c;
      if($(string).length != 0) {
        var a = $(string).contents().find('.scayt-enabled').text();
        var the_id = $(string).closest("td").attr("id");
        var real_id1 = the_id.substring(13);
        var real_id2 = real_id1.substring(0, real_id1.length - 12);
        if(a != '') {
          $('#' + real_id2).removeClass('empty_text_field');         
          var this_fieldset_id = $(string).closest("fieldset").attr("id");
          obj[this_fieldset_id] = obj[this_fieldset_id] +1;                 
        } 
        else {
          $('#' + real_id2).addClass('empty_text_field');
        }              
      }
    }
  }
  
  // Checks plaintext fields to see if they are empty,
  // then add or removes the css classes
  function check_textarea_fields(){   
    $("textarea").each(function(){
      a = $(this).val();
      the_id = $(this).attr("id");
      var type_check = the_id.substring(0, 10);
      if(($(this).is(':visible')) && (type_check == 'edit-field')) {
        var real_id = the_id.substring(0, the_id.length - 12);
        if(a != '') {
          $('#' + real_id).removeClass('empty_text_field');         
          var this_fieldset_id = $('#' + real_id).closest("fieldset").attr("id");
          obj[this_fieldset_id] = obj[this_fieldset_id] +1;              
        } else {
          $('#' + real_id).addClass('empty_text_field');
        }
      }
    });
    
  }
    
  // Add 'empty_tab' class to tabs that have no content
  function check_tabs(){
    
    var is_map = checkmap();   
    // Convert the fieldset object into a simple array
    var simple_array = new Array();    
    for(var propt in obj){    
      // Override for maps tab
      if ((propt == 'node_spm_form_group_eco_and_distro') && (is_map)){      
        obj[propt] = 1;
      }    
      simple_array.push(obj[propt]);      
    }    
    // Add 'empty_tab' class to tabs that have no content
    $counting = 0;
    $("li.vertical-tab-button").each(function(){
      if (simple_array[$counting] == 0){     
        $(this).addClass('empty_tab');       
      }
      else {        
        $(this).removeClass('empty_tab');       
      }    
      $counting++;
    });
  }
  
  // check to see if the map field has any values
  function checkmap(){    
    var v1 = $(".field_spm_map-polygon").val();
    var v2 = $("select#edit-field-spm-map-und-map-map-children-gm3-region option:selected").val();       
    if ((v1 != '') || (typeof v2 != 'undefined')){             
      $("#edit-field-spm-map").removeClass('empty_map');
      return true; 
    }    
    else{  
      $("#edit-field-spm-map").addClass('empty_map'); 
      return false;     
    }    
  }
   
});