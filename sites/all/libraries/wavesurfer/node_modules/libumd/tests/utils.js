'use strict';
var fs = require('fs');
var path = require('path');
var tmp = require('tmp');
var phantom = require('phantom');

exports.read = function(cb, file) {
    var p = path.join(__dirname, 'data', file || 'demo.js');

    fs.readFile(p, {
        encoding: 'utf-8'
    }, function(err, data) {
        if(err) {
            return console.error(err);
        }

        cb(data);
    });
};

exports.runInPhantom = function(code, consoleCb) {
    tmp.file(function(err, path, fd) {
        if(err) {
            return console.error(err);
        }

        fs.writeFile(path, code, function(err) {
            if(err) {
                return console.error(err);
            }

            var phInstance;

            phantom.create().then(function(instance) {
                phInstance = instance;

                return instance.createPage();
            }).then(function(page) {
                page.property('onConsoleMessage', consoleCb);

                page.injectJs(path, function(ok) {
                    if(!ok) {
                        return console.error('Failed to inject js');
                    }
                });

                return page;
            }).then(function() {
                phInstance.exit();
            }).catch(function(err) {
                console.error(err);

                phInstance.exit();
            })
        });
    });
};
