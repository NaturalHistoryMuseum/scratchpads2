(function($){
  Drupal.ajax.prototype.commands.citethispageWgetPreview = function(ajax, response, status) {
    var width = Math.floor(jQuery(document).width() * 0.8);
    var height = Math.floor(jQuery(window).height() * 0.8);
    var iframe = $('<iframe />')
      .attr('width', width)
      .attr('height', height)
      .attr('src', response.path);
    $("<div id='citethispage_wget_preview' title='Preview' />")
      .append(iframe)
      .appendTo('body')
      .dialog({
        modal: true,
        width: 'auto',
        height: 'auto'
      });
  };
})(jQuery);
