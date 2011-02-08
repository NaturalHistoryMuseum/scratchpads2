$(document).ready(function () {
  $('.femail-signature > p').bind('click', function(e){
    $(this).parent().children('div').toggle('slow');
  });
});