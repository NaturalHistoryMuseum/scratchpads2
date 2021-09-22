(function($){
  $(document).ready(function(){
    if(Drupal.settings.chat_room) {
      Drupal.chat_room = new Object;
      Drupal.chat_room.ws = Drupal.websockets.getsocket();
      Drupal.chat_room.ws.onopen = function(){
        // Send twice due to the odd bug we haven't tracked down.
        Drupal.chat_room.ws.send('CHAT_ROOM\nJoins: '+Drupal.settings.chat_room.user.name);
        Drupal.chat_room.ws.send('CHAT_ROOM\nJoins: '+Drupal.settings.chat_room.user.name);
        Drupal.chat_room.append('<p class="chat_room-status">Joins: '+Drupal.settings.chat_room.user.name+'</p>');
      }
      Drupal.chat_room.ws.onmessage = function(e){
        var lines = e.data.split('\n');
        var firstline = lines.splice(0, 1);
        var nextlines = lines.join('\n');
        if(firstline == 'CHAT_ROOM'){
          Drupal.chat_room.append('<p class="chat_room-other">'+nextlines+'</p>');
        }
      }
      Drupal.chat_room.append = function(msg){
        $('#chat_room-messages').append(msg);
      }
      Drupal.chat_room.send = function(msg){
        Drupal.chat_room.ws.send('CHAT_ROOM\n'+Drupal.settings.chat_room.user.name+': '+msg);
        Drupal.chat_room.append('<p class="chat_room-you">'+Drupal.settings.chat_room.user.name+': '+msg+'</p>');
      }
      $('#chat_room-textarea').keyup(function(event){
        if(event.keyCode == 13){
          if(event.metaKey){
            $('#chat_room-textarea').val($('#chat_room-textarea').val()+'\n');
          } else {
            Drupal.chat_room.send($('#chat_room-textarea').val().replace(/\n/g, '<br/>'));
            $('#chat_room-textarea').val('')
          }
        }
      });
    }
  });
})(jQuery);