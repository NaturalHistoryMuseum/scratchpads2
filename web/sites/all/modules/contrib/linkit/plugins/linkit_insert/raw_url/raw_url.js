/**
 * @file
 * HTML Link insert plugin for Linkit.
 */
(function ($) {
Drupal.linkit.addInsertPlugin('raw_url',  {
  insert : function(data, settings) {

    // For link fields we need to remove the first slash.
    if (typeof settings.no_slash != 'undefined' && settings.no_slash) {
      if (data.path.charAt(0) == '/') {
        data.path = data.path.substring(1);
      }
    }

    return data.path;
  }
});

})(jQuery);