(function($){
  $(document).ready(function (){
    WEB_SOCKET_SWF_LOCATION = Drupal.settings.websockets.swfurl;
    var ws = new WebSocket("ws://"+Drupal.settings.websockets.host+":"+Drupal.settings.websockets.port);
    ws.onopen = function() {
      ws.send("DATABASE_PATH:"+Drupal.settings.websockets.filepath);  // Sends a message.
    };
    ws.onmessage = function(e) {
      // Receives a message.
      alert(e.data);
    };
    ws.onclose = function() {
      alert("closed");
    };
      
    $(".form-submit").click(function(){
      ws.send($('#websockets-test').val());
      return false;
    });
  });
})(jQuery);