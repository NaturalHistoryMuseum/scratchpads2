(function($){
  $(document).ready(
      function(){
        if(Drupal.settings.liveform) {
          Drupal.liveform = new Object;
          Drupal.liveform.otherusers = new Array();
          Drupal.liveform.ws = Drupal.websockets.getsocket();
          Drupal.liveform.ws.onopen = function(){
          }
          $('#liveform-checkbox').click(
              function(){
                if($('#liveform-checkbox:checked').length) {
                  Drupal.liveform.ws.send('LIVEFORM:Announce\n'
                      + Drupal.settings.liveform.user);
                  Drupal.liveform.ws.send('LIVEFORM:Announce\n'
                      + Drupal.settings.liveform.user);

                } else {
                  Drupal.liveform.ws.send('LIVEFORM:Denounce\n'
                      + Drupal.settings.liveform.user);
                }
              });
          Drupal.liveform.setusermessage = function(){
            var set_message = false;
            if(Drupal.liveform.otherusers.length) {
              var other_users_html = '<ul>';
              for( var i in Drupal.liveform.otherusers) {
                if(Drupal.liveform.otherusers[i]) {
                  set_message = true;
                  other_users_html += '<li>' + Drupal.liveform.otherusers[i]
                      + '</li>'
                }
              }
              other_users_html += '</ul>';
            }
            if(!set_message) {
              other_users_html = Drupal.settings.liveform.no_user_msg;
            }
            $('#liveform-users').html(other_users_html);
          }
          Drupal.liveform.ws.onmessage = function(e){
            try {
              var lines = e.data.split('\n');
              var firstline = lines.splice(0, 1);
              var nextlines = lines.join('\n');
              if(firstline == 'LIVEFORM:Announce'
                  || firstline == 'LIVEFORM:Announce-reply') {
                var otheruser = nextlines.split(':');
                if(!Drupal.liveform.otherusers[otheruser[0]]) {
                  Drupal.liveform.otherusers[otheruser[0]] = otheruser[1];
                }
                if(firstline == 'LIVEFORM:Announce') {
                  if($('#liveform-checkbox:checked').length) {
                    // Unknown bug means we announce twice.
                    Drupal.liveform.ws.send('LIVEFORM:Announce-reply\n'
                        + Drupal.settings.liveform.user);
                    Drupal.liveform.ws.send('LIVEFORM:Announce-reply\n'
                        + Drupal.settings.liveform.user);
                  }
                }
                Drupal.liveform.setusermessage();
              } else if(firstline == 'LIVEFORM:Denounce') {
                var otheruser = nextlines.split(':');
                delete Drupal.liveform.otherusers[otheruser[0]];
                Drupal.liveform.setusermessage();
              } else if(firstline == 'LIVEFORM:Ping') {
                var lines = nextlines.split('\n');
                var firstline = lines.splice(0, 1);
                var nextlines = lines.join('\n');
                if($('#' + firstline)) {
                  if($('#liveform-checkbox:checked').length) {
                    $('#' + firstline).val(nextlines);
                  }
                }
              }
            } catch(e) {
            }
          };
          $('.form-text,.form-textarea').keyup(
              function(){
                if($('#liveform-checkbox:checked').length) {
                  if($(this).attr('id')) {
                    Drupal.liveform.ws.send('LIVEFORM:Ping\n'
                        + $(this).attr('id') + '\n' + $(this).val());
                  }
                }
              });
        }
      });
})(jQuery);