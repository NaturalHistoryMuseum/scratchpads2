(function($){
  // Extend the jQuery object so that we can easily add a form to the slickgrid
  // container.
  $.prototype.slickgrid_add_form = function(form){
    form = $(form);
    $(this).append(form);
    var button_offset = $(this).offset();
    button_offset.top -= 50;
    button_offset.left -= 150;
    $(form).offset(button_offset);
    Drupal.behaviors.AJAX.attach('', Drupal.settings);
  }
  // Extend the jQuery object so that we can easily add slickgrid messages from
  // the server.
  $.prototype.slickgrid_messages = function(messages){
    slickgrid.updateStatus({success: messages.length}, messages);
  }
  // Extend the jQuery object so that we can easily update all the data within
  // the slickgrid.
  $.prototype.slickgrid_refresh = function(){
    slickgrid.reload()
  }
})(jQuery);