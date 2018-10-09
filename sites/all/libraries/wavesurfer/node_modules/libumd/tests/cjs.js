'use strict';

var assert = require('assert');
var path = require('path');

var localeval = require('localeval');

var umdify = require('../');

var read = require('./utils').read;


module.exports = function() {
    triggered({});
    triggered();

    okTemplateName();
    invalidTemplateName();

    okTemplatePath();
    invalidTemplatePath();

    useDefault();
    preserveDefault();

    convertParametersToAlphabet();

    noCode();

    console.log('cjs ok');
};

function triggered(options) {
    read(function(data) {
        var code = umdify(data, options);

        var triggered;
        localeval(code, {
            trigger: function() {
                triggered = true;
            }
        });

        assert(triggered);
    });
}

function okTemplateName() {
    read(function(data) {
        var code = umdify(data, {
            template: 'returnExportsGlobal'
        });

        var triggered;
        localeval(code, {
            trigger: function() {
                triggered = true;
            }
        });

        assert(triggered);
    });
}

function invalidTemplateName() {
    read(function(data) {
        assert.throws(function() {
            umdify(data, {
                template: 'foobar'
            });
        },
        Error);
    });
}

function okTemplatePath() {
    read(function(data) {
        var p = path.join(__dirname, '..', 'templates', 'umd.hbs');

        var code = umdify(data, {
            template: p
        });

        var triggered;
        localeval(code, {
            trigger: function() {
                triggered = true;
            }
        });

        assert(triggered);
    });
}

function invalidTemplatePath() {
    read(function(data) {
        var p = path.join(__dirname, '..', 'templates', 'foo');

        assert.throws(function() {
            umdify(data, {
                template: p
            });
        },
        Error);
    });
}

function useDefault() {
    var dep = 'foobar';
    var code = umdify('foo()', {
        deps: {
            'default': [dep],
        },
    });

    assert(code.indexOf('define(["' + dep + '"]') >= 0);
    assert(code.indexOf('factory(require("' + dep + '"))') >= 0);
    assert(code.indexOf('factory(root["' + dep + '"])') >= 0);
}

function preserveDefault() {
    var dep = 'foobar';
    var code = umdify('foo()', {
        deps: {
            'default': [dep],
            'amd': ['baz', 'bar'],
        },
    });

    assert(code.indexOf('define(["baz","bar"], function (a0,b1) {') >= 0);
    assert(code.indexOf('factory(require("' + dep + '"))') >= 0);
    assert(code.indexOf('factory(root["' + dep + '"])') >= 0);
}

function convertParametersToAlphabet() {
    var code = umdify('foo()', {
        deps: {
            'default': ['foobar'],
            'amd': ['baz', 'bar'],
        },
    });

    assert(code.indexOf('a0,b1') >= 0);
}

function noCode() {
    assert.throws(function() {
        umdify();
    }, function(err) {
        if(err instanceof Error) {
            return true;
        }
    });
}
