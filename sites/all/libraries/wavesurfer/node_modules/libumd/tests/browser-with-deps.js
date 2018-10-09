'use strict';
var assert = require('assert');
var umdify = require('../');
var utils = require('./utils');

var code;
module.exports = function() {
    utils.read(function(data) {
        code = umdify(data, {
            globalAlias: 'test',
        });

        utils.read(function(data) {
            code += umdify(data, {
                globalAlias: 'testDeps',
                deps: {
                    'default': ['test']
                }
            });

            code += '\nwindow.testDeps();';

            utils.runInPhantom(code, function(msg) {
                console.log('browser-with-deps ok');
                assert.equal(msg, 'executed');
            });
        }, 'with-deps.js');

    }, 'browser.js');
}
