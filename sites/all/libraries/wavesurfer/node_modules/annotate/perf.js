#!/usr/bin/env node
var generate = require('annogenerate');
var is = require('annois');

var annotate = require('./');


var addNumbers = annotate('addNumbers', 'Adds given two numbers').
    on(is.number, is.number, add);

main();

function main() {
    var iterations = 30000;

    console.log('Perf test');

    timeit(iterations, [addNumbers, add], [generate.number, generate.number]);
}

function timeit(iterations, fns, generators) {
    fns.forEach(function(fn) {
        var name = fn.name || fn._name;

        console.time(name);

        for(var i = 0, len = iterations; i < len; i++) {
            fn.apply(null, getArgs(generators));
        }

        console.timeEnd(name);
    });
}

function getArgs(generators) {
    return generators.map(function(gen) {
        return gen();
    });
}

function add(a, b) {
    return a + b;
}

