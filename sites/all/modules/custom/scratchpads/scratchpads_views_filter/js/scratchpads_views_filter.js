jQuery(document).ready(function($){

  var submitted = 0;  
  var reset = 0;  
  var remove_html = '<a href="#" class="remove_button">Remove Filter</a>';
  var add_reset_html = '<a href="#" class="add_button">Add Filter</a><a href="#" class="reset_button">Reset Form</a>';
  
  setUpHtml();
  setUpEventListers();

  // 
  function setUpHtml(){    
     // Add links to the select menu
     $('#edit-selected-wrapper select').after(add_reset_html);

     // Hide filters and add links
     $("div.views-exposed-widgets input").each(function(){
       if($(this).val() == '') {
         $(this).closest(".views-exposed-widget").hide();
         $(this).closest(".views-widget").append(remove_html);
       }
     });
     //Hide select filters and add links
     $("div.views-exposed-widgets select").each(function(){
       if($(this).val() == 'All') {
         $(this).closest(".views-exposed-widget").hide();
         $(this).closest(".views-widget").append(remove_html);
       }
     });     
   }
   
   function setUpEventListers(){
     // We use 'live' to stay active after Ajax call
     $(".add_button").live("click", function(){
       $('.views-exposed-widget .form-submit').show();
       $("#edit-selected-wrapper select option:selected").each(function(){
         var str = $(this).val();
         if(str != '0') {
           $('.' + str).show();
           $('.' + str).find('.remove_button').show();
           
           $('.reset_button').show();
         }
       });
     });

     // We use 'live' to stay active after Ajax call
     $(".reset_button").live("click", function(){
       $(".views-exposed-widget").hide();
       $('#edit-selected-wrapper').show();
       resetForm();
     });
     
     $('.views-exposed-widget .form-submit').click(function() {
       submitted = 1; 
     });
     
     
     
     // Reset and hide filter
     $('.remove_button').live("click", function(){
       $(this).prev().find('input').val('');
       $(this).prev().find('select').prop('selectedIndex', 0);
       $(this).closest(".views-exposed-widget").hide();
       $(this).hide();
     });
   }
  
  // Reset form and reload results
  function resetForm(){
    reset = 1;
    $('.form-autocomplete').val('');
    $(':input').val('');
    $('select').val('');
    $('.views-exposed-widget .form-submit').trigger('click');
    $('.views-exposed-widget .form-submit').hide();
  }
  
  // We need to reset the form after Ajax call
  $(document).ajaxComplete(function(){
    if (submitted ==1){   
      $('#edit-selected-wrapper select').after(add_reset_html);
      $('.views-submit-button').show();
      $('.reset_button').show();
      $("div.views-exposed-widgets input").each(function(){
        $(this).not(".autocomplete").closest(".views-widget").append(remove_html);
        if($(this).val() == '') {
          $(this).closest(".views-exposed-widget").hide();
        }
      });

      $("div.views-exposed-widgets select").each(function(){
        $(this).closest(".views-widget").append(remove_html);
        if($(this).val() == 'All') {
          $(this).closest(".views-exposed-widget").hide();
        }
      });
    }
    submitted = 0; 
    $('.views-exposed-widget .form-submit').click(function() {
      submitted = 1;   
    });
    // Hide submit button if the form was reset
    if (reset==1){
      $('.views-exposed-widget .form-submit').hide();
      reset=0;
    }
  });

});