/**
 * Javascript for the scratchpads admin theme
 */
(function($){
  Drupal.behaviors.scratchpadsAdmin = {};
  Drupal.behaviors.scratchpadsAdmin.attach = function(context){

    // Attach the help slider behaviour
    $('.help-shortcut a:not(.scratchpads-admin-processed)', context).click(function(){
      $('.region-help').slideToggle();
      return false;
    }).addClass('scratchpads-admin-processed');

    $('a.toggler:not(.rubik-processed)', context).each(function(){
      var id = $(this).attr('href').split('#')[1];
      // Target exists, add click handler.
      if($('#' + id).size() > 0) {
        $(this).click(function(){
          toggleable = $('#' + id);
          toggleable.toggle();
          $(this).toggleClass('toggler-active');
          return false;
        });
      }
      // Target does not exist, remove click handler.
      else {
        $(this).addClass('toggler-disabled');
        $(this).click(function(){
          return false;
        });
      }
      // Mark as processed.
      $(this).addClass('rubik-processed');
    });
  };
})(jQuery);