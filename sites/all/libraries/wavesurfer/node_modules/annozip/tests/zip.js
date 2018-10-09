'use strict';

var assert = require('assert');

var generate = require('annogenerate');
var fuzz = require('annofuzz')(generate);
var deepeq = require('annoops').deepeq;

var zip = require('../');


// two arrays
fuzz(zip, function(op) {
    var a = generate.array();
    var b = generate.array();
    var res = op(a, b);

    return res.filter(function(v, i) {
        return deepeq(v[0], a[i]) && deepeq(v[1], b[i]);
    }).length === res.length;
}, 100);

assert(deepeq(zip({name: 'Joe', age: 123}), [['name', 'Joe'], ['age', 123]]));

// TODO: figure out a nice invariant for generalized case!!!
