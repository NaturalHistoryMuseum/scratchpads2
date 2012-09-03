/**
 * @file
 * Handles the JS for the views file browser.
 *
 * Note that this does not currently support multiple file selection
 */

(function ($) {

Drupal.behaviors.mediaViews = {
  attach: function (context, settings) {

    // Disable the links on media items list
    $('.view-content ul.media-list-thumbnails a').click(function() {
      return false;
    });

    // Catch the click on a media item
    $('.view-content .media-item').bind('click', function () {
      // Remove all currently selected files
      $('.view-content .media-item').removeClass('selected');
      // Set the current item to active
      $(this).addClass('selected');
      // Add this FID to the array of selected files
      var fid = $(this).closest('a[data-fid]').data('fid');

      // Because the files are added using drupal_add_js() and due to the fact
      // that drupal_get_js() runs a drupal_array_merge_deep() which re-numbers
      // numeric key values, we have to search in Drupal.settings.media.files
      // for the matching file ID rather than referencing it directly.
      var selectedFiles = new Array();
      for (index in Drupal.settings.media.files) {
        if (Drupal.settings.media.files[index].fid == fid) {
          selectedFiles.push(Drupal.settings.media.files[index]);

          // If multiple tabs contains the same file, it will be present in the
          // files-array multiple times, so we break out early so we don't have
          // it in the selectedFiles array multiple times.
          // This would interfer with multi-selection, so...
          break;
        }
      }
      Drupal.media.browser.selectMedia(selectedFiles);
    });
  }
}

}(jQuery));
