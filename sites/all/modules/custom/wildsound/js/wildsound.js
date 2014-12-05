function wildsound_current_time(target_element) {
  var time_elements = document.getElementsByClassName('mejs-currenttime'), i;
    var timestring = time_elements.item(0).innerHTML;
    var timearray = timestring.split(":");
    var timesecs = timearray[0] * 60 + timearray[1]*1;
    document.getElementById(target_element).value= timesecs;
}

function wildsound_update_comment(comment_id, timer) {
  request = new XMLHttpRequest();
  
  request.onreadystatechange = function() {
    if (request.readyState==4 && request.status==200) {
      obj = JSON.parse(request.responseText);
    
      if (obj.status == 'complete') {
        jQuery("article[about$=comment-" + comment_id + "]").html(obj.html);
        Drupal.attachBehaviors();
        clearTimeout(timer);
      }
    }
  };
  
  request.timeout = 20000;
  request.open("GET", "?q=wildsound/comment/" + comment_id, true);
  request.send();
}