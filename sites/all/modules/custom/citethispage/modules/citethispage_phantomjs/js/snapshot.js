/**
 * PhantomJS script to generate a snapshot of a given URL.
 * 
 * Expected usage is:
 * snapshot.js url destination [post-data]
 * 
 * This will exit silently (with an error code) on errors
 */
var system = require('system');
if (system.args.length < 3 || system.args > 4) {
  phantom.exit(1);
}

var page = require('webpage').create();
var address = system.args[1];
var output = system.args[2];
var post = system.args.length > 3 ? system.args[3] : null;
var size = "A4";

page.viewportSize = { width: 1024, height: 768 };
if (output.substr(-4) === '.pdf') {
  page.paperSize = {format: 'A4', orientation: 'portrait', margin: '1cm'};
}

var callback = function (status) {
  if (status !== 'success') {
    phantom.exit(1);
  } else {
    window.setTimeout(function () {
      if (output.substr(-4) === '.pdf') {
        var height = page.evaluate(function() {
          return document.height
        });
        page.paperSize = {width: '21cm', height: Math.ceil(2+21*height/1024).toString() + "cm", margin: '1cm'}
      }
      page.render(output);
      phantom.exit(0);
   }, 500);
 }
};

if (post === null) {
  page.open(address, callback);
} else {
  page.open(address, 'post', post, callback);
}
