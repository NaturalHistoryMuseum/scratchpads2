(function($){
  Drupal.ajax.prototype.commands.citethispagePhantomJSPreview = function(ajax, response, status) {
    var width = Math.floor(jQuery(document).width() * 0.8);
    var height = Math.floor(jQuery(window).height() * 0.8);
    var img = $('<img />').attr('src', response.path);
    var div = $('<div />').css({
      width: (width < 1024 ? width : 1024).toString() + "px",
      height: height.toString() + "px",
      overflow: 'scroll'
    }).append(img);
    $("<div id='citethispage_wget_preview' title='Preview' />")
      .append(div)
      .appendTo('body')
      .dialog({
        modal: true,
        width: 'auto',
        height: 'auto'
      });
  };
})(jQuery);
