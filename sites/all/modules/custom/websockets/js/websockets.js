(function($){
  $(document).ready(function (){
    WEB_SOCKET_SWF_LOCATION = Drupal.settings.websockets.swfurl;
    Drupal.websockets = new Object;
    Drupal.websockets.getsocket = function(){
      var ws = new WebSocket("ws://"
          + Drupal.settings.websockets.host + ":"
          + Drupal.settings.websockets.port);
      ws.onopen = function(){
        Drupal.websockets.ws.send("DATABASE_PATH:"
            + Drupal.settings.websockets.filepath);
        Drupal.websockets.ws.send("DRUPAL_USER:"
            + Drupal.settings.websockets.user);
      };
      return ws;
    }
  })
})(jQuery);