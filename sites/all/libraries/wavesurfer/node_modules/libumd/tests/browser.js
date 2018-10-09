'use strict';
var assert = require('assert');
var umdify = require('../');
var utils = require('./utils');

module.exports = function() {
    utils.read(function(data) {
        var code = umdify(data, {
            globalAlias: 'test'
        });

        code += '\nwindow.test();';

        utils.runInPhantom(code, function(msg) {
            console.log('browser ok');
            assert.equal(msg, 'executed');
        });
    }, 'browser.js');
}
