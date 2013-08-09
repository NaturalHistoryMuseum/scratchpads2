/**
 * PhantomJS script to generate a snapshot of a given URL.
 * 
 * This is an extended version of the citethispage script. In order
 * to store our files globally, we handle the naming of the file
 * here.
 * 
 * Expected usage is:
 * scratchpads_citethispage_snapshot.js url dest-url-base dest-file-base post-data [destination]
 * 
 * This will exit silently (with an error code) on errors
 */

// Setup and parse arguments.
var system = require('system');
if (system.args.length < 5 || system.args.length > 6) {
  phantom.exit(1);
}
var url = system.args[1];
var dest_url_base = system.args[2];
var dest_file_base = system.args[3];
var post = system.args[4];
var mode = ((system.args.length == 5) || (system.args[5].length == 0)) ? 'generate' : 'preview';
// Create our unique filename
var dest = null;
var output = null;
if (mode == 'preview') {
  // Just store it there, ignore the other parameters. This is a preview.
  output = system.args[5];
} else {
  var dest_parts = url.match(/^(?:([^:/?#]+):)?(?:\/\/([^/?#]*))?([^?#]*)(?:\?([^#]*))?(?:#(.*))?/);
  var date = new Date();
  dest = dest_parts[2].replace(/[^-a-zA-Z0-9_]+/g, '-');
  dest = dest + "/" + date.getFullYear().toString() + "-" + date.getMonth().toString() + "-" + date.getDay().toString();
  if (typeof dest_parts[3] === 'string' && dest_parts[3].length > 0 && dest_parts[3] != '/') {
    dest = dest + '/' + dest_parts[3].replace(/^\//, '').replace(/[^-a-zA-Z0-9_]+/g, '-');
  } else {
    dest = dest + '/front';
  }
  dest = dest + '.pdf';
  var fs = require('fs');
  output = dest_file_base + '/' + dest;
  var dirname = output.replace(/\/[^\/]+$/, '');
  if (!fs.exists(dirname)) {
    if (!fs.makeTree(dirname)) {
      phantom.exit(1);
    }
  }
  if (fs.exists(output)) {
    var unique = 1;
    var final_dest = dest;
    var final_output = output;
    do {
      final_dest = dest.replace(/(\.[^\.]+)$/, '-' + unique.toString() + '$1');
      final_output = dest_file_base + '/' + final_dest;
      unique++;
    } while (fs.exists(final_output));
    dest = final_dest;
    output = final_output;
  }
}
// Prepage the page
var page = require('webpage').create();
page.viewportSize = { width: 1024, height: 768 };
// Callback when the page is loaded
var callback = function (status) {
  if (status !== 'success') {
    phantom.exit(1);
  } else {
    window.setTimeout(function () {
      if (mode == 'generate') {
        var height = page.evaluate(function() {
          return document.height
        });
        page.paperSize = {width: '21cm', height: Math.ceil(2+21*height/1024).toString() + "cm", margin: '1cm'}
      }
      page.render(output);
      if (mode == 'generate') {
        console.log(dest_url_base + '/' + dest);
      }
      phantom.exit(0);
   }, 500);
 }
};
// Start the page load
if (post === null) {
  page.open(url, callback);
} else {
  page.open(url, 'post', post, callback);
}