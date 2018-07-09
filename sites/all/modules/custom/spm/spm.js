(function ($) {
  Drupal.behaviors.spm = {
    attach: function (context, settings) {
      $(document).ready(function() {

        var dialog_id = Drupal.settings.spm.dialog_id;

        // set width and height first
        $(dialog_id).css('width', Drupal.settings.spm.dialog_width + 'px');
        $(dialog_id + ' .content').css('max-height', Drupal.settings.spm.dialog_max_height + 'px');

        var winWidth = $(window).width();
        var winHeight = $(window).height();
        var boxWidth = Drupal.settings.spm.dialog_width;
        var boxHeight = $(dialog_id).outerHeight(true);
        var top = ((winHeight - boxHeight) / 2) - 50;
        var left = (winWidth - boxWidth) / 2;

        // message box
        $(dialog_id).css('z-index', '9999');
        $(dialog_id).css('position', 'fixed');
        $(dialog_id).css('left', left + 'px');
        $(dialog_id).css('top', top + 'px');

        // dragging options
        if(Drupal.settings.spm.dialog_drag == 'drag_title'){
          $(dialog_id).draggable({
            handle: '.title',
            scroll: true,
            scrollSensitivity: 150
          });
          $(dialog_id + ' .title').css('cursor', 'move');
        }
        else if(Drupal.settings.spm.dialog_drag == 'drag_all'){
          $(dialog_id).draggable({
            scroll: true,
            scrollSensitivity: 150
          });
          $(dialog_id).css('cursor', 'move');
        }

        // set close button
        // set close button
        $(dialog_id + ' .close').click(function(){
          $(dialog_id).remove();
        });

        // put the message box in body instead of the content div to make sure the position is correct
        $(dialog_id).appendTo('body');

        // display dialog
        $(dialog_id).fadeIn(500);

        // Auto fade out.
        var fadeOutTimes = [
          ['status', Drupal.settings.spm.fadeout_status],
          ['error', Drupal.settings.spm.fadeout_error],
          ['warning', Drupal.settings.spm.fadeout_warning]
        ];
        fadeOutMessages(dialog_id, fadeOutTimes);
        function fadeOutMessages(dialog_id, fadeOutTimes) {
          fadeOutTimes.sort(function(a, b) {
            return a[1].localeCompare(b[1]);
          });
          for(var i = 0; i < fadeOutTimes.length; i++) {
            if (i < fadeOutTimes.length - 1) {
              fadeOutMessage(dialog_id, fadeOutTimes[i][0], fadeOutTimes[i][1], false);
            }
            else {
              var last = true;
              if ($(dialog_id).find('.messages').length != 1 && fadeOutTimes[0][1] == 0) {
                last = false;
              }
              fadeOutMessage(dialog_id, fadeOutTimes[i][0], fadeOutTimes[i][1], last);
            }
          }
          // Fadeout message helper.
          function fadeOutMessage(dialog_id, type, timeout, last) {
            if (timeout > 0) {
              if (last) {
                $(dialog_id).delay(timeout).fadeOut('slow', function(){
                  $(this).remove();
                });
              }
              else {
                $(dialog_id).find('.messages.' + type).delay(timeout).fadeOut('slow', function(){
                  $(this).remove();
                });
              }
            }
          }
        }
      });
    }

  };

}(jQuery));
