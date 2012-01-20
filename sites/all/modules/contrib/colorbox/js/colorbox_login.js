(function ($) {

Drupal.behaviors.initColorboxLogin = {
  attach: function (context, settings) {
    if (!$.isFunction($.colorbox)) {
      return;
    }
    $("a[href*='/user/login'], a[href*='?q=user/login']", context).once('init-colorbox-login-processed', function () {
      var path = this.href;
      var new_path = path.replace(/user\/login/,'user/login/colorbox')
      var addquery = (path.indexOf('?') !=-1) ? '&' : '?';
      var dest = window.location.pathname.replace(Drupal.settings.basePath, '');

      // If no destination, add one to the current page.
      if (path.indexOf('destination') != -1 || dest == '') {
        this.href = new_path;
      }
      else {
        this.href = new_path + addquery + 'destination=' + dest;
      }
    }).colorbox({
      initialWidth:200,
      initialHeight:200,
      onComplete:function(){
        $('#edit-name').focus();
      }
    });
  }
};

})(jQuery);
