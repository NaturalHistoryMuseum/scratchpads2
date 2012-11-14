(function ($) {

Drupal.behaviors.hideSubmitBlockit = {
  attach: function(context) {
    $('form').each(function (i) {
      var form = $(this);
      $('input.form-submit', form).click(function (e) {
        var el = $(this);
        el.after('<input type="hidden" name="' + el.attr('name') + '" value="' + el.attr('value') + '" />');
        return true;
      });
    });

    $('form').submit(function (e) {
      var settings = Drupal.settings.hide_submit;
      var inp;
      if (settings.hide_submit_method == 'disable') {
        $('input.form-submit', $(this)).attr('disabled', 'disabled').each(function (i) {
          if (settings.hide_submit_css) {
            $(this).addClass(settings.hide_submit_css);
          }
          if (settings.hide_submit_abtext) {
            $(this).val($(this).val() + ' ' + settings.hide_submit_abtext);
          }
          inp = $(this);
        });

        if (inp && settings.hide_submit_atext) {
          inp.after('<span>' + Drupal.checkPlain(settings.hide_submit_atext) + '</span>');
        }
      }
      else {
        var pdiv = '<div' + (settings.hide_submit_hide_css?' class="' + Drupal.checkPlain(settings.hide_submit_hide_css) + '"':'') + '>' + Drupal.checkPlain(settings.hide_submit_hide_text) + '</div>';
        if (settings.hide_submit_hide_fx) {
          $('input.form-submit', $(this)).fadeOut(100).eq(0).after(pdiv);
          $('input.form-submit', $(this)).next().fadeIn(100);
        }
        else {
          $('input.form-submit', $(this)).css('display', 'none').eq(0).after(pdiv);
        }
      }
      return true;
    });
  }
};

})(jQuery);

