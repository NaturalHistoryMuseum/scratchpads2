/**
 * @file
 * Colorbox module admin settings js.
 */

(function ($) {

Drupal.behaviors.initColorboxAdminSettings = {
  attach: function (context, settings) {

    $('div.colorbox-custom-settings-activate input.form-radio', context).click(function () {
      if (this.value == 1) {
        $('div.colorbox-custom-settings', context).show();
      }
      else {
        $('div.colorbox-custom-settings', context).hide();
      }
    });
    $('div.colorbox-slideshow-settings-activate input.form-radio', context).click(function () {
      if (this.value == 1) {
        $('div.colorbox-slideshow-settings', context).show();
      }
      else {
        $('div.colorbox-slideshow-settings', context).hide();
      }
    });
    $('div.colorbox-title-trim-settings-activate input.form-radio', context).click(function () {
      if (this.value == 1) {
        $('div.colorbox-title-trim-settings', context).show();
      }
      else {
        $('div.colorbox-title-trim-settings', context).hide();
      }
    });
    $('.colorbox-reset-specific-pages-default', context).click(function (event) {

      event.preventDefault();

      var colorbox_specific_pages_default_value = Drupal.settings.colorbox.specificPagesDefaultValue;

      if (typeof colorbox_specific_pages_default_value !== 'undefined' && colorbox_specific_pages_default_value.length > 0) {
        $("#edit-colorbox-pages").val(colorbox_specific_pages_default_value);
      }
    });
  }
};

})(jQuery);
