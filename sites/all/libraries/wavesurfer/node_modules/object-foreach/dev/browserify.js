/**
 * Options for browserify.bundle
 * @namespace
 */
var browserifyBundleOpts = {
    /**
     * When opts.debug is true, add a source map inline to the end of the
     *  bundle. This makes debugging easier because you can see all the original
     *  files if you are in a modern enough browser.
     * @type Boolean
     * @default false
     */
    debug : true,
    /**
     * When opts.insertGlobals is true, always insert process, global,
     *  __filename, and __dirname without analyzing the AST for faster builds but
     *  larger output bundles. Default false.
     * @type Boolean
     * @default false
     */
    insertGlobals : false,
    /**
     * When opts.detectGlobals is true, scan all files for process, global,
     *  __filename, and __dirname, defining as necessary. With this option npm
     *  modules are more likely to work but bundling takes longer. Default true.
     * @type Boolean
     * @default true
     */
    detectGlobals : true,
    /**
     * When opts.standalone is a non-empty string, a standalone module is
     *  created with that name and a umd wrapper.
     * @type String
     */
    standalone : '',
    /**
     * opts.insertGlobalVars will be passed to insert-module-globals as the
     *  opts.vars parameter. (it's properties become global properties available
     *  within the module.)
     * @type Object
     */
    insertGlobalVars : {}
};

var path = require('path');
var fs = require('fs');
var browserify = require('browserify');
var b = browserify();
var browserMain = path.resolve(__dirname, 'browserMain.js');
var outputFile = path.resolve(__dirname, '../browser/object-foreach_web.js');
var writeStream = fs.createWriteStream(outputFile);

b.add(browserMain);
b.bundle(browserifyBundleOpts).pipe(writeStream);



