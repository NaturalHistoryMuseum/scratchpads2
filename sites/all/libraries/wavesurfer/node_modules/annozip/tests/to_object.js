'use strict';

var generate = require('annogenerate');

generate.isZip = function() {
    return zip(generate.array(), generate.array());
};

var fuzz = require('annofuzz')(generate);
var deepeq = require('annoops').deepeq;

var zip = require('../');


fuzz(zip.toObject, function(op, a) {
    var res = op(a);

    return a.filter(function(v) {
        return deepeq(res[v[0]], v[1]);
    }).length === a.length;
}, 100);
