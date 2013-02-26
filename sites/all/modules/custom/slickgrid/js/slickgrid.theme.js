/**
 * Theme functions
 */
(function($){
  // Theme a slickgrid filter
  Drupal.theme.prototype.slickgridFilter = function($input, type){
    return $('<span class="slickgrid-filter slickgrid-filter-' + type + '"></span>').click(function(){
      $input.focus()
    }) // Make it a bit more uable - click anywhere on the span to set the
        // focus on the input
    .append($('<span></span>').append($input));
  };
  // Theme the messages
  Drupal.theme.prototype.slickgridMessages = function(messages){
    var $ul = $('<ul class="slickgrid-messages">');
    $.each(messages, function(i, message){
      $('<li class="' + message.type + '">' + message.message + '</li>').appendTo($ul);
    });
    return $ul;
  };
  /**
   * Provide the HTML to create the modal dialog.
   */
  Drupal.theme.prototype.SlickgridModal = function(){
    var html = ''
    html += '<div id="ctools-modal-slickgrid" class="popups-box">';
    html += '  <div class="ctools-modal-content ctools-slickgrid-modal-content">';
    html += '    <div class="popups-container">';
    html += '     <div class="modal-header popups-title">';
    html += '       <span id="modal-title" class="modal-title"></span>';
    html += '       <span class="popups-close"><a class="close" href="#">' + Drupal.CTools.Modal.currentSettings.closeImage + '</a></span>';
    html += '       <div class="clear-block"></div>';
    html += '     </div>';
    html += '     <div id="modal-content" class="modal-content popups-body"></div>';
    html += '   </div>';
    html += '  </div>';
    html += '</div>';
    return html;
  }
})(jQuery);
