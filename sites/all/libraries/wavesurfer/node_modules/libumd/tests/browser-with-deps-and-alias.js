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
                    'default': [{'test':'alias'}]
                }
            });

            code += '\nwindow.testDeps();';

            utils.runInPhantom(code, function(msg) {
                console.log('browser-with-deps-and-alias ok');
                assert.equal(msg, 'executed');
            });
        }, 'with-deps-and-alias.js');

    }, 'browser.js');
}
