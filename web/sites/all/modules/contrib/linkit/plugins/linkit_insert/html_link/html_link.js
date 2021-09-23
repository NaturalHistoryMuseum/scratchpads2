/**
 * @file
 * HTML Link insert plugin for Linkit.
 */
(function ($) {

Drupal.linkit.addInsertPlugin('html_link', {
  insert : function(data) {
    var linkitCache = Drupal.linkit.getLinkitCache(),
    text;

     // Delete all attributes that are empty.
    for (name in data.attributes) {
      (data.attributes[name]) ? null : delete data.attributes[name];
    }

    if (typeof linkitCache.selection != 'undefined' &&
      linkitCache.selection.text.length >= 1) {
      text = linkitCache.selection.text;
    }
    else {
      text = linkitCache.link_tmp_title;
    }

    // Use document.createElement as it is mush fasten then $('<a/>).
    return $(document.createElement('a'))
    .attr(data.attributes)
    .attr('href', data.path)
    .html(text)
    // Convert the element to a string.
    .get(0).outerHTML;
  }
});

})(jQuery);