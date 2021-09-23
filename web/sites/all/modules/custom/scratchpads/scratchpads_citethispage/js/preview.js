(function($){
  // This opens the overlay with the preview
  Drupal.ajax.prototype.commands.scratchpadsCitethispagePreview = function(ajax, response, status) {
    var width = Math.floor(jQuery(document).width() * 0.8);
    var height = Math.floor(jQuery(window).height() * 0.8);
    var img = $('<img />').attr('src', response.path);
    var div = $('<div />').css({
      width: (width < 1024 ? width : 1024).toString() + "px",
      height: height.toString() + "px",
      overflow: 'scroll'
    }).append(img);
    $("<div id='scratchpads_citethispage_preview' title='Preview' />")
      .append(div)
      .appendTo('body')
      .dialog({
        modal: true,
        width: 'auto',
        height: 'auto'
      });
  };  
})(jQuery);