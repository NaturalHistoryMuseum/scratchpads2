(function($){
  $(document).ready(function (){
    $('.form-text').keyup(function(){
      if($(this).attr('id')){
        Drupal.websockets.ws.send($(this).attr('id')+'\n'+$(this).val());
      }
    });
    Drupal.websockets.ws.onmessage = function(e){
      var lines = e.data.split('\n');
      var firstline = lines.splice(0,1);
      var newtext = lines.join('\n');
      if($('#'+firstline)){
        $('#'+firstline).val(newtext);
      }
    };
  });
})(jQuery);