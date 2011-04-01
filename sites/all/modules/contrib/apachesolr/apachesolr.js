(function ($) {

Drupal.behaviors.apachesolr = {
  attach: function(context, settings) {
    $('.apachesolr-hidden-facet', context).hide();
    $('<a href="#" class="apachesolr-showhide"></a>').text(Drupal.t('Show more')).click(function() {
      if ($(this).parent().find('.apachesolr-hidden-facet:visible').length == 0) {
        $(this).parent().find('.apachesolr-hidden-facet').show();
        $(this).text(Drupal.t('Show fewer'));
      }
      else {
        $(this).parent().find('.apachesolr-hidden-facet').hide();
        $(this).text(Drupal.t('Show more'));
      }
      return false;
    }).appendTo($(settings.apachesolr_show_more_blocks, context));

    if (settings.apachesolr_facetstyle == 'checkboxes') {
      // Find all facet links and give them a checkbox
      $('.apachesolr-facet', context).each(Drupal.apachesolr.addCheckbox);
      // Find all unclick links and turn them into checkboxes
      $('.apachesolr-unclick', context).each(Drupal.apachesolr.makeCheckbox);
    }
  }
}

Drupal.apachesolr = {}

/**
 * Constructor for a class.
 */
Drupal.apachesolr.Redirect = function(href) {
  this.href = href;
}

/**
 * Method to redirect to the stored href.
 */
Drupal.apachesolr.Redirect.prototype.gotoHref = function() {
  window.location.href = this.href;
}

Drupal.apachesolr.addCheckbox = function() {
  if (!$(this).hasClass('facet-checkbox-processed')) {
    // Create an unchecked checkbox.
    var checkbox = $('<input type="checkbox" class="facet-checkbox" />');
    // Get the href of the link that is this DOM object.
    var href = $(this).attr('href');
    redirect = new Drupal.apachesolr.Redirect(href);
    checkbox.click($.proxy(redirect, 'gotoHref'));
    $(this).before(checkbox).before('&nbsp;');
    $(this).addClass('facet-checkbox-processed');
  }
}

Drupal.apachesolr.makeCheckbox = function() {
  // Create a checked checkbox.
  var checkbox = $('<input type="checkbox" class="facet-checkbox" checked="true" />');
  // Get the href of the link that is this DOM object.
  var href = $(this).attr('href');
  redirect = new Drupal.apachesolr.Redirect(href);
  checkbox.click($.proxy(redirect, 'gotoHref'));
  // Add the checkbox, hide the link.
  $(this).before(checkbox).hide();
}

})(jQuery);