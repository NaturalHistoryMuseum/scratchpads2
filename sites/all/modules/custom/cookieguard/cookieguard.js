(function($){
  // Override
  var cookieHeight = 0;
  $.cookieguard.displayMessage = function(showDelay, hideDelay){
    $.cookieguard.createCSS();
    cookieHeight = $('#cookieGuardMsg').outerHeight();
    $('#cookieGuardMsg').css('top', -cookieHeight);
    $('#cookieGuardMsg').delay(showDelay).show().animate({'top': 0}, $.cookieguard.settings.slideSpeed);
    $('#toolbar').delay(showDelay).animate({'top': cookieHeight}, $.cookieguard.settings.slideSpeed);
    $('body').delay(showDelay).animate({'paddingTop': cookieHeight + parseInt($('body').css('paddingTop'))}, $.cookieguard.settings.slideSpeed);
  };
  // Override
  $.cookieguard.hideMessage = function(hideDelay){
    $('body').css({'paddingTop': parseInt($('body').css('paddingTop')) - cookieHeight});
    $('#toolbar').css({'top': 0});
    $('#cookieGuardMsg').remove();
    if(Drupal.overlay) {
      Drupal.overlay.eventhandlerAlterDisplacedElements();
    }
  }
  $(document).ready(function(){
    $.cookieguard();
    if(Drupal.settings.cookieguard) {
      for( var i in Drupal.settings.cookieguard) {
        $.cookieguard.cookies.add(Drupal.settings.cookieguard[i]['name'], Drupal.settings.cookieguard[i]['keys'], Drupal.settings.cookieguard[i]['description'], Drupal.settings.cookieguard[i]['essential']);
      }
    }
    $.cookieguard.run();
    setTimeout(function(){
      $('#denyCookies').click(function(){
        $.cookie($.cookieguard.settings.cookiePrefix + 'initialised', '1', {expires: 365});
      });
    }, $.cookieguard.settings.cookieDeleteDelay + 20);
  });
})(jQuery);
