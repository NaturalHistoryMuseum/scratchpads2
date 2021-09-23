/*
 * Used to get current playback position form a wavesurfer and enter it into an input field
 */
function wildsound_current_time(target_element){
  document.getElementById(target_element).value = first(window.surfers).getCurrentTime().toString().substring(0, 8);
}

function first(p){for(var i in p)return p[i];}

/*
 * AJAX for getting currrent status of an analysis and updating comment as required
 */
function wildsound_update_comment(comment_id, timer){
  request = new XMLHttpRequest();
  request.onreadystatechange = function(){
    if(request.readyState == 4 && request.status == 200) {
      obj = JSON.parse(request.responseText);
      if(obj.status == 'complete') {
        jQuery("article[about$=comment-" + comment_id + "]").html(obj.html);
        Drupal.attachBehaviors();
        clearTimeout(timer);
      }
    }
  };
  request.timeout = 20000;
  request.open("GET", Drupal.settings.wildsound.wildsound_comment_callback_url + comment_id, true);
  request.send();
}

/*
 * Make wavesurfer sticky at top of page when scrolling
 */
jQuery(window).scroll(function(){
  var fieldItem = jQuery("#wavesurfer_position").parent('.field-item');
  if(!checkVisible(fieldItem.parent('.field-items'))) {
    fieldItem.css("width", "100%");
    fieldItem.css("height", "100px", "important");
    fieldItem.css("position", "fixed");
    fieldItem.css("top", "0px");
    fieldItem.css("left", "0px");
    fieldItem.css("background-color", "white");
    fieldItem.css("z-index", "1");
    fieldItem.css("padding-top", "64px");
    fieldItem.css("padding-bottom", "64px");
  } else {
    fieldItem.css("width", "");
    fieldItem.css("height", "");
    fieldItem.css("position", "");
    fieldItem.css("top", "");
    fieldItem.css("left", "");
    fieldItem.css("padding-top", "");
    fieldItem.css("padding-bottom", "");
  }
});

/*
 * Helper function to test if wavesurfer is currently visible on the page
 */
function checkVisible(elm, eval){
  eval = eval || "visible";
  var vpH = jQuery(window).height(), // Viewport Height
  st = jQuery(window).scrollTop() + 100, // Scroll Top
  y = jQuery(elm).offset().top, elementHeight = jQuery(elm).height();
  if(eval == "visible") return((y < (vpH + st)) && (y > (st - elementHeight)));
  if(eval == "above") return((y < (vpH + st)));
}

jQuery(window).load(function(){
  
});

/*
 * Add regions to wavesurfers when they are drawn
 */
function wildsound_onready(item) {
  if (typeof(Drupal.settings.wildsound) != 'undefined' && typeof(Drupal.settings.wildsound.comments) != 'undefined') {
    for( var comment_id in Drupal.settings.wildsound.comments) {
      var comment = Drupal.settings.wildsound.comments[comment_id];
      eval("window.surfers["+comment.fid+"].addRegion({id: comment.id, start: comment.start, end: comment.end, loop: false, drag: false, resize: false, color: comment.color})");
    }
  }
}
