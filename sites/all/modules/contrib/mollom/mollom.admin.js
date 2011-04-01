// $Id: mollom.admin.js,v 1.2 2010/10/13 00:18:52 dries Exp $
(function ($) {

/**
 * Filters blacklist entries.
 */
Drupal.behaviors.mollomBlacklistFilter = {
  attach: function (context) {
    var self = this;
    $('#mollom-blacklist', context).once('mollom-blacklist-filter', function () {
      // Prepare a list of all entries to optimize performance. Each key is a
      // blacklisted text and each value is an object containing the
      // corresponding table row, context, and match.
      self.entries = {};
      $(this).find('tr:has(.mollom-blacklist-text)').each(function () {
        var $row = $(this);
        self.entries[$row.find('.mollom-blacklist-text').text()] = {
          context: $row.children('.mollom-blacklist-context').attr('class').match(/value-(\w+)/)[1],
          match: $row.children('.mollom-blacklist-match').attr('class').match(/value-(\w+)/)[1],
          row: $row.get(0)
        };
      });

      // Attach the instant text filtering behavior.
      var $filterText = $('#mollom-blacklist-filter-text', context);
      var $filterContext = $('#mollom-blacklist-filter-context', context);
      var $filterMatch = $('#mollom-blacklist-filter-match', context);

      self.lastSearch = {};
      var filterRows = function () {
        // Prepare static variables and conditions only once.
        var i, text, visible, changed;
        var search = {
          // Blacklist entries are stored in lowercase, so to get any filter
          // results, the entered text must be converted to lowercase, too.
          text: $filterText.val().toLowerCase(),
          context: $filterContext.val(),
          match: $filterMatch.val()
        };
        // Immediately cancel processing if search values did not change.
        changed = false;
        for (i in search) {
          if (search[i] != self.lastSearch[i]) {
            changed = true;
            break;
          }
        }
        if (!changed) {
          return;
        }
        self.lastSearch = search;
        // Blacklists can contain thousands of entries, so we use a simple
        // for...in loop instead of jQuery.each() to save many function calls.
        // Likewise, we directly apply the 'display' style, since
        // jQuery.fn.hide() and jQuery.fn.show() call into jQuery.fn.animate(),
        // which is useless for this purpose.
        for (text in self.entries) {
          visible = (search.text.length == 0 || text.indexOf(search.text) != -1);
          visible = visible && (search.context.length == 0 || self.entries[text].context == search.context);
          visible = visible && (search.match.length == 0 || self.entries[text].match == search.match);
          if (visible) {
            self.entries[text].row.style.display = '';
          }
          else {
            self.entries[text].row.style.display = 'none';
          }
        }
      };
      $filterText.bind('keyup change', filterRows);
      $filterContext.change(filterRows);
      $filterMatch.change(filterRows);
    });
  }
};

})(jQuery);
