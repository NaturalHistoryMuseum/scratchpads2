jQuery(document).ready(function () {
  jQuery('.femail-signature > p').bind('click', function(e){
    jQuery(this).parent().children('div').toggle('slow');
  });
});