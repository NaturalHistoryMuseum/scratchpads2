(function ($) {

Drupal.behaviors.weight = {
  attach: function (context) {
    // Provide the vertical tab summaries.
    $('fieldset#edit-weight-settings', context).drupalSetSummary(function(context) {
      //var vals = [];
      var enabled, vals = [];
      enabled = $('input:radio[name="weight_enabled"]:checked').val();
      vals.push(enabled == 1 ? Drupal.t('Enabled') : Drupal.t('Disabled'));
      if (enabled == 1) {
        vals.push(Drupal.t('Range: !range', {'!range' : $('input:radio[name="weight_range"]:checked').val()}));
        vals.push(Drupal.t('Default: !default', {'!default' : $('#edit-weight-default option:selected').val()}));
      }
      return vals.join(', ');
    });
    $('fieldset#edit-weight', context).drupalSetSummary(function(context) {
      return Drupal.t('Weight: !weight', {'!weight' : $('#edit-weight-weight option:selected').val()});
    });
  }
};

})(jQuery);
