(function ($) {

Drupal.behaviors.initModalFormsComment = {
  attach: function (context, settings) {
    $("a[href*='/comment/reply'], a[href*='?q=comment/reply']", context).once('init-modal-forms-comment', function () {
      this.href = this.href.replace(/comment\/reply/,'modal_forms/nojs/comment/reply');
    }).addClass('ctools-use-modal ctools-modal-modal-popup-medium');
  }
};

})(jQuery);