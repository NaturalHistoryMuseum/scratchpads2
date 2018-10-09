'use strict';
var assert = require('assert');
var fs = require('fs');
var path = require('path');
var umdify = require('../');
var utils = require('./utils');

module.exports = function() {
    var amdLoaderPath = path.join(__dirname, 'amd_loader.js');
    var code = fs.readFileSync(amdLoaderPath, {
        encoding: 'utf-8'
    });

    utils.read(function(data) {
        code += umdify(data, {
            amdModuleId: 'test',
            globalAlias: 'test'
        });

        utils.read(function(data) {
            code += umdify(data, {
                amdModuleId: 'testDeps',
                globalAlias: 'testDeps',
                deps: {
                    'default': ['test']
                }
            });
            code += '\nrequire([\'testDeps\'], function(testDeps) {/* testDeps(); */});';

            utils.runInPhantom(code, function(msg) {
                console.log('amd-with-deps ok');
                assert.equal(msg, 'executed');
            });
        }, 'with-deps.js');
    }, 'browser.js');
}
