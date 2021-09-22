
/**
 * @fileOverview Better Autocomplete is a flexible jQuery plugin which offers
 * rich text autocompletion, both from local and remote sources.
 *
 * @author Didrik Nordström, http://betamos.se/
 *
 * @version v1.0
 *
 * @requires
 *   <ul><li>
 *   jQuery 1.4+
 *   </li><li>
 *   IE7+ or any decent webkit/gecko-based web browser
 *   </li></ul>
 *
 * @preserve Better Autocomplete v1.0-dev
 * https://github.com/betamos/Better-Autocomplete
 *
 * Copyright 2011, Didrik Nordström, http://betamos.se/
 * Dual licensed under the MIT or GPL Version 2 licenses.
 *
 * Requires jQuery 1.4+
 * http://jquery.com/
 */

/**
 * Create or alter an autocomplete object instance that belongs to
 * the elements in the selection. Make sure there are only text field elements
 * in the selection.
 *
 * @constructor
 *
 * @name jQuery.betterAutocomplete
 *
 * @param {String} method
 *   Should be one of the following:
 *   <ul><li>
 *     init: Initiate Better Autocomplete instances on the text input elements
 *     in the current jQuery selection. They are enabled by default. The other
 *     parameters are then required.
 *   </li><li>
 *     enable: In this jQuery selection, reenable the Better Autocomplete
 *     instances.
 *   </li><li>
 *     disable: In this jQuery selection, disable the Better Autocomplete
 *     instances.
 *   </li><li>
 *     destroy: In this jQuery selection, destroy the Better Autocomplete
 *     instances. It will not be possible to reenable them after this.
 *   </li></ul>
 *
 * @param {String|Object} [resource]
 *   If String, it will become the path for a remote resource. If not, it will
 *   be treated like a local resource. The path should provide JSON objects
 *   upon HTTP requests.
 *
 * @param {Object} [options]
 *   An object with configurable options:
 *   <ul><li>
 *     charLimit: (default=3 for remote or 1 for local resource) The minimum
 *     number of chars to do an AJAX call. A typical use case for this limit is
 *     to reduce server load.
 *   </li><li>
 *     delay: (default=350) The time in ms between last keypress and AJAX call.
 *     Typically used to prevent looking up irrelevant strings while the user
 *     is still typing. Only relevant for remote resources.
 *   </li><li>
 *     caseSensitive: (default=false) If the search should be case sensitive.
 *     If false, query strings will be converted to lowercase.
 *   </li><li>
 *     cacheLimit: (default=256 for remote or 0 for local resource) The maximum
 *     number of result objects to store in the cache. This option reduces
 *     server load if the user deletes characters to check back on previous
 *     results. To disable caching of previous results, set this option to 0.
 *   </li><li>
 *     remoteTimeout: (default=10000) The timeout for remote (AJAX) calls.
 *   </li><li>
 *     crossOrigin: (default=false) Set to true if cross origin requests will
 *     be performed, i.e. that the remote URL has a different domain. This will
 *     force Internet Explorer to use "jsonp" instead of "json" as datatype.
 *   </li><li>
 *     selectKeys: (default=[9, 13]) The key codes for keys which will select
 *     the current highlighted element. The defaults are tab, enter.
 *   </li></ul>
 *
 * @param {Object} [callbacks]
 *   An object containing optional callback functions on certain events. See
 *   {@link callbacks} for details. These callbacks should be used when
 *   customization of the default behavior of Better Autocomplete is required.
 *
 * @returns {Object}
 *   The jQuery object with the same element selection, for chaining.
 */

(function($) {

$.fn.betterAutocomplete = function(method) {

  /*
   * Each method expects the "this" object to be a valid DOM text input node.
   * The methods "enable", "disable" and "destroy" expects an instance of a
   * BetterAutocomplete object as their first argument.
   */
  var methods = {
    init: function(resource, options, callbacks) {
      var $input = $(this),
        bac = new BetterAutocomplete($input, resource, options, callbacks);
      $input.data('better-autocomplete', bac);
      bac.enable();
    },
    enable: function(bac) {
      bac.enable();
    },
    disable: function(bac) {
      bac.disable();
    },
    destroy: function(bac) {
      bac.destroy();
    }
  }, args = Array.prototype.slice.call(arguments, 1);

  // Method calling logic
  this.each(function() {
    switch (method) {
    case 'init':
      methods[method].apply(this, args);
      break;
    case 'enable':
    case 'disable':
    case 'destroy':
      var bac = $(this).data('better-autocomplete');
      if (bac instanceof BetterAutocomplete) {
        methods[method].call(this, bac);
      }
      break;
    default:
      $.error(['Method', method,
          'does not exist in jQuery.betterAutocomplete.'].join(' '));
    }
  });

  // Maintain chainability
  return this;
};

/**
 * The BetterAutocomplete constructor function. Returns a BetterAutocomplete
 * instance object.
 *
 * @private @constructor
 * @name BetterAutocomplete
 *
 * @param {Object} $input
 *   A single input element wrapped in jQuery.
 */
var BetterAutocomplete = function($input, resource, options, callbacks) {

  var lastRenderedQuery = '',
    cache = {}, // Key-valued caching of search results
    cacheOrder = [], // Array of query strings, in the order they are added
    cacheSize = 0, // Keep count of the cache's size
    timer, // Used for options.delay
    activeRemoteCalls = [], // A flat array of query strings that are pending
    disableMouseHighlight = false, // Suppress the autotriggered mouseover event
    inputEvents = {},
    isLocal = ($.type(resource) != 'string'),
    $results = $('<ul />').addClass('better-autocomplete'),
    hiddenResults = true, // $results are hidden
    preventBlurTimer = null; // IE bug workaround, see below in code.

  options = $.extend({
    charLimit: isLocal ? 1 : 3,
    delay: 350, // milliseconds
    caseSensitive: false,
    cacheLimit: isLocal ? 0 : 256, // Number of result objects
    remoteTimeout: 10000, // milliseconds
    crossOrigin: false,
    selectKeys: [9, 13] // [tab, enter]
  }, options);

  callbacks = $.extend({}, defaultCallbacks, callbacks);

  callbacks.insertSuggestionList($results, $input);

  inputEvents.focus = function() {
    // If the blur timer is active, a redraw is redundant.
    preventBlurTimer || redraw(true);
  };

  inputEvents.blur = function() {
    // If the blur prevention timer is active, refocus the input, since the
    // blur event can not be cancelled.
    if (preventBlurTimer) {
      $input.focus();
    }
    else {
      // The input has already lost focus, so redraw the suggestion list.
      redraw();
    }
  };

  inputEvents.keydown = function(event) {
    var index = getHighlighted();
    // If an arrow key is pressed and a result is highlighted
    if ($.inArray(event.keyCode, [38, 40]) >= 0 && index >= 0) {
      var newIndex,
        size = $('.result', $results).length;
      switch (event.keyCode) {
      case 38: // Up arrow
        newIndex = Math.max(0, index - 1);
        break;
      case 40: // Down arrow
        newIndex = Math.min(size - 1, index + 1);
        break;
      }
      disableMouseHighlight = true;
      setHighlighted(newIndex, true);
      return false;
    }
    // A select key has been pressed
    else if ($.inArray(event.keyCode, options.selectKeys) >= 0 &&
             !event.shiftKey && !event.ctrlKey && !event.altKey &&
             !event.metaKey) {
      select();
      return event.keyCode == 9; // Never cancel tab
    }
  };

  inputEvents.keyup = inputEvents.click = reprocess;

  $('.result', $results[0]).live({
    // When the user hovers a result with the mouse, highlight it.
    mouseover: function() {
      if (disableMouseHighlight) {
        return;
      }
      setHighlighted($('.result', $results).index($(this)));
    },
    mousemove: function() {
      // Enable mouseover again.
      disableMouseHighlight = false;
    },
    mousedown: function() {
      select();
      return false;
    }
  });

  // Prevent blur when clicking on group titles, scrollbars etc.,
  // This event is triggered after the others because of bubbling.
  $results.mousedown(function() {
    // Bug in IE where clicking on scrollbar would trigger a blur event for the
    // input field, despite using preventDefault() on the mousedown event.
    // This workaround locks the blur event on the input for a small time.
    clearTimeout(preventBlurTimer);
    preventBlurTimer = setTimeout(function() {
      preventBlurTimer = null;
    }, 50);
    return false;
  });

  /*
   * PUBLIC METHODS
   */

  /**
   * Enable this instance.
   */
  this.enable = function() {
    // Turn off the browser's autocompletion
    $input
      .attr('autocomplete', 'OFF')
      .attr('aria-autocomplete', 'list');
    $input.bind(inputEvents);
  };

  /**
   * Disable this instance.
   */
  this.disable = function() {
    $input
      .removeAttr('autocomplete')
      .removeAttr('aria-autocomplete');
    $results.hide();
    $input.unbind(inputEvents);
  };

  /**
   * Disable and remove this instance. This instance should not be reused.
   */
  this.destroy = function() {
    $results.remove();
    $input.unbind(inputEvents);
    $input.removeData('better-autocomplete');
  };

  /*
   * PRIVATE METHODS
   */

  /**
   * Add an array of results to the cache. Internal methods always reads from
   * the cache, so this method must be invoked even when caching is not used,
   * e.g. when using local results. This method automatically clears as much of
   * the cache as required to fit within the cache limit.
   *
   * @param {String} query
   *   The query to set the results to.
   *
   * @param {Array[Object]} results
   *   The array of results for this query.
   */
  var cacheResults = function(query, results) {
    cacheSize += results.length;
    // Now reduce size until it fits
    while (cacheSize > options.cacheLimit && cacheOrder.length) {
      var key = cacheOrder.shift();
      cacheSize -= cache[key].length;
      delete cache[key];
    }
    cacheOrder.push(query);
    cache[query] = results;
  };

  /**
   * Set highlight to a specific result item
   *
   * @param {Number} index
   *   The result's index, starting at 0.
   *
   * @param {Boolean} [autoScroll]
   *   (default=false) If scrolling of the results list should be automated.
   */
  var setHighlighted = function(index, autoScroll) {
    // Scrolling upwards
    var up = index == 0 || index < getHighlighted(),
      $scrollTo = $('.result', $results)
        .removeClass('highlight')
        .eq(index).addClass('highlight');

    if (!autoScroll) {
      return;
    }
    // Scrolling up, then make sure to show the group title
    if ($scrollTo.prev().is('.group') && up) {
      $scrollTo = $scrollTo.prev();
    }
    // Is $scrollTo partly above the visible region?
    if ($scrollTo.position().top < 0) {
      $results.scrollTop($scrollTo.position().top + $results.scrollTop());
    }
    // Or is it partly below the visible region?
    else if (($scrollTo.position().top + $scrollTo.outerHeight()) >
              $results.height()) {
      $results.scrollTop($scrollTo.position().top + $results.scrollTop() +
          $scrollTo.outerHeight() - $results.height());
    }
  };

  /**
   * Retrieve the index of the currently highlighted result item
   *
   * @returns {Number}
   *   The result's index or -1 if no result is highlighted.
   */
  var getHighlighted = function() {
    return $('.result', $results).index($('.result.highlight', $results));
  };

  /**
   * Select the current highlighted element, if any.
   */
  var select = function() {
    var $result = $('.result', $results).eq(getHighlighted());
    if (!$result.length) {
      return; // No selectable element
    }
    var result = $result.data('result');
    callbacks.select(result, $input);
    // Redraw again, if the callback changed focus or content
    reprocess();
  };

  /**
   * Fetch results asynchronously via AJAX.
   * Errors are ignored.
   *
   * @param {String} query
   *   The query string.
   */
  var fetchResults = function(query) {
    // Synchronously fetch local data
    if (isLocal) {
      cacheResults(query, callbacks.queryLocalResults(query, resource,
                                                      options.caseSensitive));
      redraw();
    }
    // Asynchronously fetch remote data
    else {
      activeRemoteCalls.push(query);
      var url = callbacks.constructURL(resource, query);
      callbacks.beginFetching($input);
      callbacks.fetchRemoteData(url, function(data) {
        var searchResults = callbacks.processRemoteData(data);
        if (!$.isArray(searchResults)) {
          searchResults = [];
        }
        cacheResults(query, searchResults);
        // Remove the query from active remote calls, since it's finished
        activeRemoteCalls = $.grep(activeRemoteCalls, function(value) {
          return value != query;
        });
        if (!activeRemoteCalls.length) {
          callbacks.finishFetching($input);
        }
        redraw();
      }, options.remoteTimeout, options.crossOrigin);
    }
  };

  /**
   * Reprocess the contents of the input field, fetch data and redraw if
   * necessary.
   */
  function reprocess() {
    var query = callbacks.canonicalQuery($input.val(), options.caseSensitive);
    clearTimeout(timer);
    // Indicate that timer is inactive
    timer = null;
    redraw();
    if (query.length >= options.charLimit && !$.isArray(cache[query]) &&
        $.inArray(query, activeRemoteCalls) == -1) {
      // Fetching is required
      $results.empty();
      if (isLocal) {
        fetchResults(query);
      }
      else {
        timer = setTimeout(function() {
          fetchResults(query);
          timer = null;
        }, options.delay);
      }
    }
  };

  /**
   * Redraws the autocomplete list based on current query and focus.
   *
   * @param {Boolean} [focus]
   *   (default=false) Force to treat the input element like it's focused.
   */
  var redraw = function(focus) {
    var query = callbacks.canonicalQuery($input.val(), options.caseSensitive);

    // The query does not exist in db
    if (!$.isArray(cache[query])) {
      lastRenderedQuery = null;
      $results.empty();
    }
    // The query exists and is not already rendered
    else if (lastRenderedQuery !== query) {
      lastRenderedQuery = query;
      renderResults(cache[query]);
      setHighlighted(0);
    }
    // Finally show/hide based on focus and emptiness
    if (($input.is(':focus') || focus) && !$results.is(':empty')) {
      $results.filter(':hidden').show() // Show if hidden
        .scrollTop($results.data('scroll-top')); // Reset the lost scrolling
      if (hiddenResults) {
        hiddenResults = false;
        callbacks.afterShow($results);
      }
    }
    else if ($results.is(':visible')) {
      // Store the scrolling position for later
      $results.data('scroll-top', $results.scrollTop())
        .hide(); // Hiding it resets it's scrollTop
      if (!hiddenResults) {
        hiddenResults = true;
        callbacks.afterHide($results);
      }
    }
  };

  /**
   * Regenerate the DOM content within the results list for a given set of
   * results. Heavy method, use only when necessary.
   *
   * @param {Array[Object]} results
   *   An array of result objects to render.
   */
  var renderResults = function(results) {
    $results.empty();
    var groups = {}; // Key is the group name, value is the heading element.

    $.each(results, function(index, result) {
      if ($.type(result) != 'object') {
        return; // Continue
      }

      var output = callbacks.themeResult(result);
      if ($.type(output) != 'string') {
        return; // Continue
      }

      // Add the group if it doesn't exist
      var group = callbacks.getGroup(result);
      if ($.type(group) == 'string' && !groups[group]) {
        var $groupHeading = $('<li />').addClass('group')
          .append($('<h3 />').html(group))
          .appendTo($results);
        groups[group] = $groupHeading;
      }

      var $result = $('<li />').addClass('result')
        .append(output)
        .data('result', result) // Store the result object on this DOM element
        .addClass(result.addClass);

      // First groupless item
      if ($.type(group) != 'string' &&
          !$results.children().first().is('.result')) {
        $results.prepend($result);
        return; // Continue
      }
      var $traverseFrom = ($.type(group) == 'string') ?
                          groups[group] : $results.children().first();
      var $target = $traverseFrom.nextUntil('.group').last();
      $result.insertAfter($target.length ? $target : $traverseFrom);
    });
  };
};

/*
 * CALLBACK METHODS
 */

/**
 * These callbacks are supposed to be overridden by you when you need
 * customization of the default behavior. When you are overriding a callback
 * function, it is a good idea to copy the source code from the default
 * callback function, as a skeleton.
 *
 * @name callbacks
 * @namespace
 */
var defaultCallbacks = {
  /**
   * @lends callbacks.prototype
   */

  /**
   * Gets fired when the user selects a result by clicking or using the
   * keyboard to select an element.
   *
   * <br /><br /><em>Default behavior: Inserts the result's title into the
   * input field.</em>
   *
   * @param {Object} result
   *   The result object that was selected.
   *
   * @param {Object} $input
   *   The input DOM element, wrapped in jQuery.
   */
  select: function(result, $input) {
    $input.val(result.title);
  },

  /**
   * Given a result object, theme it to HTML.
   *
   * <br /><br /><em>Default behavior: Wraps result.title in an h4 tag, and
   * result.description in a p tag. Note that no sanitization of malicious
   * scripts is done here. Whatever is within the title/description is just
   * printed out. May contain HTML.</em>
   *
   * @param {Object} result
   *   The result object that should be rendered.
   *
   * @returns {String}
   *   HTML output, will be wrapped in a list element.
   */
  themeResult: function(result) {
    var output = [];
    if ($.type(result.title) == 'string') {
      output.push('<h4>', result.title, '</h4>');
    }
    if ($.type(result.description) == 'string') {
      output.push('<p>', result.description, '</p>');
    }
    return output.join('');
  },

  /**
   * Retrieve local results from the local resource by providing a query
   * string.
   *
   * <br /><br /><em>Default behavior: Automatically handles arrays, if the
   * data inside each element is either a plain string or a result object.
   * If it is a result object, it will match the query string against the
   * title and description property. Search is not case sensitive.</em>
   *
   * @param {String} query
   *   The query string, unescaped. May contain any UTF-8 character.
   *   If case insensitive, it already is lowercased.
   *
   * @param {Object} resource
   *   The resource provided in the {@link jQuery.betterAutocomplete} init
   *   constructor.
   *
   * @param {Boolean} caseSensitive
   *   From options.caseSensitive, the searching should be case sensitive.
   *
   * @returns {Array[Object]}
   *   A flat array containing pure result objects. May be an empty array.
   */
  queryLocalResults: function(query, resource, caseSensitive) {
    if (!$.isArray(resource)) {
      // Per default Better Autocomplete only handles arrays
      return [];
    }
    var results = [];
    $.each(resource, function(i, value) {
      switch ($.type(value)) {
      case 'string': // Flat array of strings
        if ((caseSensitive ? value : value.toLowerCase())
            .indexOf(query) >= 0) {
          // Match found
          results.push({ title: value });
        }
        break;
      case 'object': // Array of result objects
        if ($.type(value.title) == 'string' &&
            (caseSensitive ? value.title : value.title.toLowerCase())
            .indexOf(query) >= 0) {
          // Match found in title field
          results.push(value);
        }
        else if ($.type(value.description) == 'string' &&
                 (caseSensitive ? value.description :
                 value.description.toLowerCase()).indexOf(query) >= 0) {
          // Match found in description field
          results.push(value);
        }
        break;
      }
    });
    return results;
  },

  /**
   * Fetch remote result data and return it using completeCallback when
   * fetching is finished. Must be asynchronous in order to not freeze the
   * Better Autocomplete instance.
   *
   * <br /><br /><em>Default behavior: Fetches JSON data from the url, using
   * the jQuery.ajax() method. Errors are ignored.</em>
   *
   * @param {String} url
   *   The URL to fetch data from.
   *
   * @param {Function} completeCallback
   *   This function must be called, even if an error occurs. It takes zero
   *   or one parameter: the data that was fetched.
   *
   * @param {Number} timeout
   *   The preferred timeout for the request. This callback should respect
   *   the timeout.
   *
   * @param {Boolean} crossOrigin
   *   True if a cross origin request should be performed.
   */
  fetchRemoteData: function(url, completeCallback, timeout, crossOrigin) {
    $.ajax({
      url: url,
      dataType: crossOrigin && !$.support.cors ? 'jsonp' : 'json',
      timeout: timeout,
      success: function(data, textStatus) {
        completeCallback(data);
      },
      error: function(jqXHR, textStatus, errorThrown) {
        completeCallback();
      }
    });
  },

  /**
   * Process remote fetched data by extracting an array of result objects
   * from it. This callback is useful if the fetched data is not the plain
   * results array, but a more complicated object which does contain results.
   *
   * <br /><br /><em>Default behavior: If the data is defined and is an
   * array, return it. Otherwise return an empty array.</em>
   *
   * @param {mixed} data
   *   The raw data recieved from the server. Can be undefined.
   *
   * @returns {Array[Object]}
   *   A flat array containing result objects. May be an empty array.
   */
  processRemoteData: function(data) {
    if ($.isArray(data)) {
      return data;
    }
    else {
      return [];
    }
  },

  /**
   * From a given result object, return it's group name (if any). Used for
   * grouping results together.
   *
   * <br /><br /><em>Default behavior: If the result has a "group" property
   * defined, return it.</em>
   *
   * @param {Object} result
   *   The result object.
   *
   * @returns {String}
   *   The group name, may contain HTML. If no group, don't return anything.
   */
  getGroup: function(result) {
    if ($.type(result.group) == 'string') {
      return result.group;
    }
  },

  /**
   * Called when remote fetching begins.
   *
   * <br /><br /><em>Default behavior: Adds the CSS class "fetching" to the
   * input field, for styling purposes.</em>
   *
   * @param {Object} $input
   *   The input DOM element, wrapped in jQuery.
   */
  beginFetching: function($input) {
    $input.addClass('fetching');
  },

  /**
   * Called when fetching is finished. All active requests must finish before
   * this function is called.
   *
   * <br /><br /><em>Default behavior: Removes the "fetching" class.</em>
   *
   * @param {Object} $input
   *   The input DOM element, wrapped in jQuery.
   */
  finishFetching: function($input) {
    $input.removeClass('fetching');
  },

  /**
   * Executed after the suggestion list has been shown.
   *
   * @param {Object} $results
   *   The suggestion list UL element, wrapped in jQuery.
   *
   * <br /><br /><em>Default behavior: Does nothing.</em>
   */
  afterShow: function($results) {},

  /**
   * Executed after the suggestion list has been hidden.
   *
   * @param {Object} $results
   *   The suggestion list UL element, wrapped in jQuery.
   *
   * <br /><br /><em>Default behavior: Does nothing.</em>
   */
  afterHide: function($results) {},

  /**
   * Construct the remote fetching URL.
   *
   * <br /><br /><em>Default behavior: Adds "?q=query" to the path. The query
   * string is URL encoded.</em>
   *
   * @param {String} path
   *   The path given in the {@link jQuery.betterAutocomplete} constructor.
   *
   * @param {String} query
   *   The raw query string. Remember to URL encode this to prevent illegal
   *   character errors.
   *
   * @returns {String}
   *   The URL, ready for fetching.
   */
  constructURL: function(path, query) {
    return path + '?q=' + encodeURIComponent(query);
  },

  /**
   * To ease up on server load, treat similar strings the same.
   *
   * <br /><br /><em>Default behavior: Trims the query from leading and
   * trailing whitespace.</em>
   *
   * @param {String} rawQuery
   *   The user's raw input.
   *
   * @param {Boolean} caseSensitive
   *   Case sensitive. Will convert to lowercase if false.
   *
   * @returns {String}
   *   The canonical query associated with this string.
   */
  canonicalQuery: function(rawQuery, caseSensitive) {
    var query = $.trim(rawQuery);
    if (!caseSensitive) {
      query = query.toLowerCase();
    }
    return query;
  },

  /**
   * Insert the results list into the DOM and position it properly.
   *
   * <br /><br /><em>Default behavior: Inserts suggestion list directly
   * after the input element and sets an absolute position using
   * jQuery.position() for determining left/top values. Also adds a nice
   * looking box-shadow to the list.</em>
   *
   * @param {Object} $results
   *   The UL list element to insert, wrapped in jQuery.
   *
   * @param {Object} $input
   *   The text input element, wrapped in jQuery.
   */
  insertSuggestionList: function($results, $input) {
    $results.width($input.outerWidth() - 2) // Subtract border width.
      .css({
        position: 'absolute',
        left: $input.position().left,
        top: $input.position().top + $input.outerHeight(),
        zIndex: 10,
        maxHeight: '330px',
        // Visually indicate that results are in the topmost layer
        boxShadow: '0 0 15px rgba(0, 0, 0, 0.5)'
      })
      .hide()
      .insertAfter($input);
  }
};

/*
 * jQuery focus selector, required by Better Autocomplete.
 *
 * @see http://stackoverflow.com/questions/967096/using-jquery-to-test-if-an-input-has-focus/2684561#2684561
 */
var filters = $.expr[':'];
if (!filters.focus) {
  filters.focus = function(elem) {
    return elem === document.activeElement && (elem.type || elem.href);
  };
}

})(jQuery);
