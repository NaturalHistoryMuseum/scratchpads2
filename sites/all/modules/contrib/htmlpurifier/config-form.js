(function ($) {

Drupal.behaviors.htmlpurifierConfigForm = {
  // Makes all configuration links open in new windows; can save lots of grief!
  attach: function (context, settings) {
    $(".hp-config a", context).click(function () {
      window.open(this.href);
      return false;
    });
  }
};

})(jQuery);
