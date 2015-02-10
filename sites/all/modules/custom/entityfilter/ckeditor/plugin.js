/*
 * polyfill for IE9 to allow for multiple arguments in setTimeout
 * http://stackoverflow.com/questions/12404528/ie-parameters-get-undefined-when-using-them-in-settimeout
 */
if(document.all && !window.setTimeout.isPolyfill) {
  var __nativeST__ = window.setTimeout;
  window.setTimeout = function(vCallback, nDelay){
    var aArgs = Array.prototype.slice.call(arguments, 2);
    return __nativeST__(vCallback instanceof Function ? function(){
      vCallback.apply(null, aArgs);
    } : vCallback, nDelay);
  };
  window.setTimeout.isPolyfill = true;
}
if(document.all && !window.setInterval.isPolyfill) {
  var __nativeSI__ = window.setInterval;
  window.setInterval = function(vCallback, nDelay){
    var aArgs = Array.prototype.slice.call(arguments, 2);
    return __nativeSI__(vCallback instanceof Function ? function(){
      vCallback.apply(null, aArgs);
    } : vCallback, nDelay);
  };
  window.setInterval.isPolyfill = true;
}
/*
 * Helper class needed to handle entityfilter. This class is a singleton for
 * each instance of CKEDITOR. @param {Object} editor An instance of a CKEDITOR
 * @returns {null}
 */
function CKEDITOR_entityfilter(editor){
  this.editor = editor;
  this.observe = 0;
  this.timeout_id = null;
  if(CKEDITOR_entityfilter.caller !== CKEDITOR_entityfilter.get_instance) {
    throw new Error("This object cannot be instanciated");
  }
}
/*
 * Collection of pairs editor id / instance of CKEDITOR_entityfilter @type Array
 */
CKEDITOR_entityfilter.instances = [];
/*
 * Delay of the timeout between the last key pressed and the ajax query. It's
 * use to prevent ajax flooding when user types fast. @type Number
 */
CKEDITOR_entityfilter.timeout_delay = 500;
/*
 * Method used to get an instance of CKEDITOR_entityfilter linked to an instance
 * of CKEDITOR. Its design is based on the singleton design pattern. @param
 * {Object} editor An instance of a CKEDITOR @returns An instance of
 * CKEDITOR_entityfilter
 */
CKEDITOR_entityfilter.get_instance = function(editor){
  // we browse our collection of instances
  for( var i in this.instances) {
    // if we find an CKEDITOR instance in our collection
    if(this.instances[i].id === editor.id) {
      // we return the instance of CKEDITOR_entityfilter that match
      return this.instances[i].instance;
    }
  }
  // if no match was found, we add a row in our collection with the current
  // CKEDITOR id and we instanciate CKEDITOR_entityfilter
  this.instances.push({
    id: editor.id,
    instance: new CKEDITOR_entityfilter(editor)
  });
  // we return the instance of CKEDITOR_entityfilter that was just created
  return this.instances[this.instances.length - 1].instance;
};
/*
 * This methods sends an ajax query to Drupal CKEDITOR_entityfilter module and
 * retrieve matching resuilts.
 */
CKEDITOR_entityfilter.prototype.get_entityfilterresults = function(selection){
  if(null !== this.timeout_id) {
    clearTimeout(this.timeout_id);
  }
  this.timeout_id = setTimeout(this.timeout_callback, CKEDITOR_entityfilter.timeout_delay, [this, selection]);
}
/*
 * This methods send an ajax query to Drupal CKEDITOR_entityfilter
 */
CKEDITOR_entityfilter.prototype.timeout_callback = function(args){
  var entityfilter = args[0];
  var selection = args[1];
  var editor = entityfilter.editor;
  var range = selection.getRanges()[0];
  var text_left_of_cursor = range.startContainer.$.textContent.substring(0, range.endOffset)
  if(text_left_of_cursor.indexOf('[entity') > -1) {
    var search_str = text_left_of_cursor.substring(text_left_of_cursor.lastIndexOf('['));
    // Ensure there are no spaces in there, else we've probably already handled
    // this brace (or it's not even an entity brace
    if(search_str.indexOf(' ') == -1) {
      var startOffset = parseInt(range.startOffset - search_str.length) || 0;
      var element = range.startContainer.$;
      jQuery.get(Drupal.settings.basePath + 'ckeditor/entityfilter', {
        typed: search_str
      }, function(response){
        jQuery('.entityfilter').remove();
        if(response) {
          jQuery('<div class="entityfilter">' + response.html + '</div>').insertAfter(jQuery('#' + editor.element.getId()).parent());
        }
        jQuery('.entityfilter div ul li').click(function(e){
          jQuery('.entityfilter').remove();
          var ranges = editor.getSelection().getRanges();
          var entityfilter = CKEDITOR_entityfilter.get_instance(editor);
          // Shorten text node
          element.textContent = element.textContent.substr(0, startOffset);
          var inserttext = '[' + jQuery(this).data('textcontent');
          element.textContent = element.textContent + inserttext;
          if(jQuery.browser.msie) {
            // https://drupal.org/node/2033739
          } else {
            // FIXME - the cursor is not returned to the correct position.
            editor.focus();
            var additional = inserttext.length - search_str.length;
            ranges[0].setStart(ranges[0].startContainer, ranges[0].startOffset + additional);
            ranges[0].setEnd(ranges[0].endContainer, ranges[0].endOffset + additional);
            editor.getSelection().selectRanges(ranges);
          }
        });
      });
    }
  }
};
(function($){
  CKEDITOR.plugins.add('entityfilter', {
    icons: '',
    init: function(editor){
      var entityfilter = CKEDITOR_entityfilter.get_instance(editor);
      editor.on('instanceReady', function(e){
        jQuery('.entityfilter').remove();
        var editable = editor.editable();
        editable.attachListener(editable, 'keypress', function(e){
          entityfilter.get_entityfilterresults(this.editor.getSelection());
        });
      });
    }
  });
})(jQuery);