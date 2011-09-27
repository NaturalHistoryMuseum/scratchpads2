
/**
 * @file shows / hides form elements
 */
 
(function ($) {

Drupal.behaviors.TaxonomyManagerHideForm = {
  attach: function(context, settings) {
    if (!$('#taxonomy-manager-toolbar').hasClass('buttons-processed')) {
      $('#taxonomy-manager-toolbar').addClass('buttons-processed')
      settings = settings.hideForm || [];
      if (settings instanceof Array) {
        for (var i=0; i<settings.length; i++) {
          Drupal.attachHideForm(settings[i].div, settings[i].show_button, settings[i].hide_button);
        }
      }
    }
  }
}

/**
 * adds click events to show / hide button
 */
Drupal.attachHideForm = function(div, show_button, hide_button) {
  var hide = true;
  div = $("#"+ div);
  show_button = $("#"+ show_button);
  hide_button = $("#"+ hide_button);

  // don't hide if there is an error in the form
  $(div).find("input").each(function() {
    if($(this).hasClass("error")) {
      hide = false;
    }
  });
  
  if (!hide) { 
    $(div).show();
  }
  $(show_button).click(function() {
    $(div).toggle();
    return false;
  });
  
  $(hide_button).click(function() {
    $(div).hide();
    return false;
  });
}

})(jQuery);
