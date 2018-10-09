'use strict';

var annotate = require('annotate');
var is = require('annois');


var prop = annotate('prop', 'Returns a function that gets given property').
    on(is.string, function(name) {
        return function(o) {
            return o && o[name];
        };
    });

var values = annotate('values', 'Returns values of the given object').
    on(is.object, function(o) {
        var ret = [];

        for(var k in o) {
            if(o.hasOwnProperty(k)) {
                ret.push(o[k]);
            }
        }

        return ret;
    });

var zip = annotate('zip', 'Converts given input into a zip').
    on(is.object, function(o) {
        return zip(Object.keys(o), values(o));
    }).
    on([is.array], function() {
        var ret = [];
        var args = Array.prototype.slice.call(arguments);
        var i, len;

        for(i = 0, len = Math.min.apply(null, args.map(prop('length'))); i < len; i++) {
            ret.push(extract(i, args));
        }

        return ret;
    }).
    satisfies(isZip);

function extract(idx, arrays) {
    var ret = [];
    var i, len;

    for(i = 0, len = arrays.length; i < len; i++) {
        ret.push(arrays[i][idx]);
    }

    return ret;
}

var toObject = annotate('zip.toObject', 'Converts given zip into an object').
    on(isZip, function(a) {
        var ret = {};

        a.forEach(function(v) {
            ret[v[0]] = v[1];
        });

        return ret;
    }).
    satisfies(is.object);

zip.toObject = toObject;

function isZip(a) {
    var zips = a.filter(function(v) {
        return is.array(v);
    });

    return is.array(a) && zips.length === a.length;
}

module.exports = zip;
