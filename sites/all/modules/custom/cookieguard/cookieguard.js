(function($){
  $.cookieguard.displayMessage = function(showDelay, hideDelay){
    $.cookieguard.createCSS();
    $('#cookieGuardMsg').css('bottom', -$('#cookieGuardMsg').outerHeight());
    $('#cookieGuardMsg').delay(showDelay).show().animate({'bottom': 0}, $.cookieguard.settings.slideSpeed);
  };
  $(document).ready(function(){
    if(Drupal.settings.cookieguard) {
      $.cookieguard();
      for( var i in Drupal.settings.cookieguard) {
        $.cookieguard.cookies.add(Drupal.settings.cookieguard[i]['name'], Drupal.settings.cookieguard[i]['keys'], Drupal.settings.cookieguard[i]['description'], Drupal.settings.cookieguard[i]['essential']);
      }
      $.cookieguard.run();
      setTimeout(function(){
        $('#denyCookies').click(function(){
          $.cookieguard.cookies.create($.cookieguard.settings.cookiePrefix + 'initialised', '2', 365);
        });
      }, $.cookieguard.settings.cookieDeleteDelay + 20);
    }
  });
})(jQuery);