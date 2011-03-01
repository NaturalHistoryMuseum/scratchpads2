// jQuery $ thingy
(function($){
Drupal.twitterscript = Drupal.twitterscript || {};	
function autolink(s){
  var hlink = /\s(ht|f)tp:\/\/([^ \,\;\:\!\)\(\"\'\<\>\f\n\r\t\v])+/g;
  return (s.replace (hlink, function ($0,$1,$2){
    s = $0.substring(1,$0.length); 
    // remove trailing dots, if any
    while (s.length>0 && s.charAt(s.length-1)=='.') 
      s=s.substring(0,s.length-1);
    // add hlink
    return " " + s.link(s); 
  }));
}
Drupal.behaviors.twitterscript = {
 attach: function (context, settings){
  $('.twitterscript', context).each(function(){
    var twitterscript = this;
    $.getJSON('http://search.twitter.com/search.json?rpp=5&q=' + escape($(this).html()) + '&callback=?', function(data){
      var html_to_embed = '<ul class="twitterscript-list">';
      var no_results = true;
      $.each(data.results, function(){
        no_results = false;
        html_to_embed += '<li><a href="http://twitter.com/'+this.from_user+'"><img src="'+this.profile_image_url+'"/></a>'+ autolink(this.text) +'</li>';
      });
      if(no_results){
        html_to_embed += '<li>There are no results for "<em>'+$(twitterscript).html()+'</em>"</li>';
      }
      html_to_embed += '</ul>';
      $(twitterscript).replaceWith(html_to_embed);
    });
  });
 }
}
// jQuery $ thingy
})(jQuery);