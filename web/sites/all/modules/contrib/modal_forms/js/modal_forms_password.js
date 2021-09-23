(function ($) {

Drupal.behaviors.initModalFormsPassword = {
  attach: function (context, settings) {
    $("a[href*='/user/password'], a[href*='?q=user/password']", context).once('init-modal-forms-password', function () {
      this.href = this.href.replace(/user\/password/,'modal_forms/nojs/password');
    }).addClass('ctools-use-modal ctools-modal-modal-popup-small');
  }
};

})(jQuery);
