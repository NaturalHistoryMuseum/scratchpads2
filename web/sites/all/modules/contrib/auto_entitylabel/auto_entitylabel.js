(function ($) {

Drupal.behaviors.auto_entitylabelFieldsetSummaries = {
  attach: function (context) {
    $('fieldset#edit-auto-entitylabel', context).drupalSetSummary(function (context) {

      // Retrieve the value of the selected radio button
      var ant = $("input[@name=#edit-auto-entitylabel-ant]:checked").val();

      if (ant==0) {
        return Drupal.t('Disabled')
      }
      else if (ant==1) {
        return Drupal.t('Automatic (hide title field)')
      }
      else if (ant==2) {
        return Drupal.t('Automatic (if title empty)')
      }
    });
  }
};

})(jQuery);
