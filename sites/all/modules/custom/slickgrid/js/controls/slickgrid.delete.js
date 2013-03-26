/**
 * Controller for deleting items
 */
(function($){
  function SlickGridDelete(grid, $container){
  }
  // Slick.Controls.Undo
  $.extend(true, window, {Slick: {Controls: {Delete: SlickGridDelete}}});
})(jQuery);