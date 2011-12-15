/**
 * @file
 *   Initialize onUnload scripts.
 */

(function($) {
  window.content_lock_onleave = function  () {
    var nid = Drupal.settings.content_lock.nid;
    var ajax_key = Drupal.settings.content_lock.ajax_key;
    var protocol = $(location).attr('protocol');
    var host  = $(location).attr('host');
    var aurl = protocol+host+Drupal.settings.basePath + 'index.php?q=ajax/content_lock/'+nid+'/canceledit&k='+ajax_key;
    $.ajax({
      url:   aurl,
      async: false,
      cache: false
    });
  }

  window.content_lock_confirm = function () {
    return Drupal.t(Drupal.settings.content_lock.unload_js_message);
  }

  $(document).ready(function() {
    $().onUserExit( {
      execute: content_lock_onleave,
      executeConfirm: content_lock_confirm,
      internalURLs: 'canceledit|trash/confirm|edit'
    });
  });
})(jQuery);
