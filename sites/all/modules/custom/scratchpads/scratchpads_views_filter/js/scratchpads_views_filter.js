jQuery(document).ready(function($){

  var submitted = 0;
  var reset = 0;
  var html_remove_filter = '<a href="#" class="remove_button">Remove Filter</a>';
  var html_add_reset = '<a href="#" class="add_button">Add Filter</a><a href="#" class="reset_button">Reset Form</a>';

  setUpHtml();
  setUpEventListers();
  reverseDivs();
  reverseLabel();
  removeOptions();
  $('.form-submit').show();

  function setUpHtml(){
    // Add links to the select menu
    $('#edit-selected-wrapper select').after(html_add_reset);
    // Hide filters and add links
    $("div.views-exposed-widgets input").each(function(){
      if($(this).val() == '') {
        $(this).closest(".views-exposed-widget").hide();

        if($(this).closest(".views-exposed-widget").find('.remove_button').length) {
          // do nothing

        } else {
          $(this).closest(".views-widget").after(html_remove_filter);
        }
      }
    });
    // Hide select filters and add links
    $("div.views-exposed-widgets select").each(function(){
      if($(this).val() == 'All') {
        $(this).closest(".views-exposed-widget").hide();
        $(this).closest(".views-widget").after(html_remove_filter);
      }
    });
  }

  function setUpEventListers(){
    // We use 'live' to stay active after Ajax call
    $(".add_button").live("click", function(){
      $('.views-exposed-widget.views-submit-button').show();
      $("#edit-selected-wrapper select option:selected").each(function(){
        var str = $(this).val();
        if(str != '0') {
          $('.' + str).show();
          $('.' + str).find('.remove_button').show();
          $('.reset_button').show();
        }
      });
    });
    $(".reset_button").live("click", function(){
      $(".views-exposed-widget").hide();
      $('#edit-selected-wrapper').show();
      resetForm();
    });
    $('.views-exposed-widget .form-submit').click(function(){
      submitted = 1;
    });
    // Reset and hide filter
    $('.remove_button').live("click", function(){
      $(this).prev().prev().find('input:text').val('');
      $(this).prev().find('select').prop('selectedIndex', 0);
      $(this).closest(".views-exposed-widget").hide();
      $(this).hide();
    });

    $('#txt').live("keydown",function(e){
      if(e.keyCode == 13) {
        submitted = 1;
      }
    })
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

  // reverse the order of the divs within the exposed filter
  function reverseDivs(){
    // $('.views-exposed-widget.dependent-options > div').each(function(){
    $('.views-exposed-widget .views-widget').each(function(){
      $(this).prependTo(this.parentNode);
    });

  }

  function reverseLabel(){
    // $('.views-exposed-widget.dependent-options > label').each(function(){
    $('.views-exposed-widget > label').each(function(){
      $(this).prependTo(this.parentNode);
    });
  }

  // Remove unwanted options from the drop down list
  function removeOptions(){
    $(".views-exposed-widget.dependent-options .views-operator .form-type-select option[value='shorterthan']").remove();
    $(".views-exposed-widget.dependent-options .views-operator .form-type-select option[value='longerthan']").remove();
    $(".views-exposed-widget .views-operator .form-type-select option[value='empty']").remove();
    $(".views-exposed-widget .views-operator .form-type-select option[value='not empty']").remove();
  }

  // We need to reset the form after Ajax call
  $(document).ajaxComplete(function(){
    if(submitted == 1) {

      $('#edit-selected-wrapper select').after(html_add_reset);
      $('.views-submit-button').show();
      $('.reset_button').show();

      $(".views-exposed-widget").hide();
      $('#edit-selected-wrapper').show();

      $("div.views-exposed-widgets input").each(function(){

        if($(this).closest(".views-exposed-widget").find('.remove_button').length == 0) {
          $(this).not(".autocomplete").closest(".views-widget").after(html_remove_filter);
        }

        // -1 if no match
        var is_ajax = $(this).val().indexOf("http://");

        // 

        if(($(this).val() != '') && (is_ajax == '-1')) {
          $(this).closest(".views-exposed-widget").show();
        }
      });
      $("div.views-exposed-widgets .views-widget select").each(function(){
        $(this).closest(".views-widget").after(html_remove_filter);
        if($(this).val() != 'All') {
          $(this).closest(".views-exposed-widget").show();
        }
      });
      $("div.views-exposed-widgets select").each(function(){
        if(($(this).val() == 'empty') || ($(this).val() == 'not empty')) {
          $(this).closest(".views-exposed-widget").show();
        }
      });
      reverseDivs();
      reverseLabel();
      removeOptions();
    }

    submitted = 0;
    $('.views-exposed-widget .form-submit').click(function(){
      submitted = 1;
    });
    // Hide submit button if the form was reset

    if(reset == 1) {
      reverseDivs();
      reverseLabel();
      removeOptions();
      $('.views-exposed-widget.views-submit-button').hide();
      reset = 0;
    }
  });

});