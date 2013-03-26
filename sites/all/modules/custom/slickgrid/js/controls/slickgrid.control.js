(function($){
  // Extend the jQuery object so that we can easily add a form to the slickgrid
  // container.
  $.prototype.slickgrid_add_form = function(something){
    $('#slickgrid').append(something);
    Drupal.behaviors.AJAX.attach('', Drupal.settings);
  }
})(jQuery);