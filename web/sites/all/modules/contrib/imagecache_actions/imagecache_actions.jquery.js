/* 
 * UI enhancements for the imagecache edit form
 */
(function($){

  /**
   * Check if independent corners are enabled and disable other fields in the UI
   */
  Drupal.canvasactions_roundedcorners_form_disable_fields = function () {
    // To get the right effect, we have to set the 'disabled' attribute on the 
    // field, but set the class on the container item. Tedious.
    if (!$(":checkbox#edit-data-independent-corners-set-independent-corners").attr("checked")){
      $(".form-item-data-radius").removeClass("form-disabled");
      $(".form-item-data-radius input").attr("disabled", false);
      $("#independent-corners-set .form-item").addClass("form-disabled");
      $("#independent-corners-set input").attr("disabled", true);
    } 
    else {
      $(".form-item-data-radius").addClass("form-disabled");
      $(".form-item-data-radius input").attr("disabled", true);
      $("#independent-corners-set .form-item").removeClass("form-disabled");
      $("#independent-corners-set input").attr("disabled", false);
    }
  }
  /**
   * Trigger the update when the form is ready, and add listener to the checkbox
   */
  Drupal.behaviors.canvasactions_roundedcorners = {
    attach: function (context, settings) {
      Drupal.canvasactions_roundedcorners_form_disable_fields();
      $(":checkbox#edit-data-independent-corners-set-independent-corners").change(
        function() {
          Drupal.canvasactions_roundedcorners_form_disable_fields(); 
        }
      );
    }
  }

})(jQuery);

