(function ($) {

Drupal.behaviors.initModalFormsContact = {
  attach: function (context, settings) {
    $("a[href*='/contact'], a[href*='?q=contact']", context).once('init-modal-forms-contact', function () {
      this.href = this.href.replace(/contact/,'modal_forms/nojs/contact');
    }).addClass('ctools-use-modal ctools-modal-modal-popup-medium');
  }
};

})(jQuery);
