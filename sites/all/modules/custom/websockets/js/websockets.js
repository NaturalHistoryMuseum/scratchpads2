(function($){
  $(document).ready(function (){
    WEB_SOCKET_SWF_LOCATION = Drupal.settings.websockets.swfurl;
    Drupal.websockets = new Object;
    Drupal.websockets.ws = new WebSocket("ws://"+Drupal.settings.websockets.host+":"+Drupal.settings.websockets.port);
    Drupal.websockets.ws.onopen = function() {
      Drupal.websockets.ws.send("DATABASE_PATH:"+Drupal.settings.websockets.filepath);
      Drupal.websockets.ws.send("DRUPAL_USER:"+Drupal.settings.websockets.user);
    };
    Drupal.websockets.ws.onmessage = function(e) {};      
    $(".form-submit").click(function(){
      Drupal.websockets.ws.send($('#websockets-test').val());
      return false;
    });
  });
})(jQuery);