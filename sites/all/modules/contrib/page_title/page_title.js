
(function ($) {

Drupal.behaviors.pageTitleFieldsetSummaries = {
  attach: function (context) {
    $('fieldset#edit-page-title', context).drupalSetSummary(function (context) {
      var pt = $('input', context).val();

      return pt ?
        Drupal.t('Page Title: @pt', { '@pt': pt }) :
        Drupal.t('No Page Title');
    });
  }
};


Drupal.behaviors.pageTitleCounter = {
  attach : function(context) {
    $('fieldset#edit-page-title', context).each(function() {
      function getLength(element) { return $(element).val().length; }

      var wrapper = this;

      var inputBox = $('input[name=page_title]', wrapper);

      var valueBox = $('div.description', wrapper)
                      .append('<br/><span class="counter">Characters Entered: <span class="value">0</span></span>')
                      .find('.value')
                      .text(getLength(inputBox));

      $('input[name=page_title]', wrapper).keyup(function(e) { $(valueBox).text(getLength(inputBox)); });
    });
  }

}


})(jQuery);

