var suite = require('suite.js');
var is = require('annois');
var partial = require('funkit').partial;
var annotate = require('../');

var noDispatch = annotate('noDispatch', 'No dispatch');

var WARNINGS = parseInt(process.env.YIELD_WARNINGS, 10);


if(WARNINGS) {
    noDispatch(); // should yield a warning (no operation)
}

var addNumber = annotate('addNumber').on(is.number, add);
aSuite(addNumber, '', [[is.number]], [], 'addNumber');

if(WARNINGS) {
    suite(addNumber, [
        [1, 'a'], '1a'
    ]);

    addNumber('foo', 'bar'); // should yield a warning
}

var addNumbers = annotate('addNumbers', 'Adds numbers')
    .on(is.number, is.number, add)
    .satisfies(is.number);
aSuite(addNumbers, 'Adds numbers', [[is.number, is.number]], [is.number], 'addNumbers');

suite(addNumbers, [
    [1, 2], 3
]);

var addString = annotate().on(is.string, add);
aSuite(addString, '', [[is.string]], [], '');
suite(addString, [
    ['a', 1], 'a1'
]);

var addStrings = annotate('addStrings', 'Appends two strings')
    .on(is.string, is.string, add);
aSuite(addStrings, 'Appends two strings', [[is.string, is.string]], [], 'addStrings');
suite(addStrings, [
    ['a', 'b'], 'ab'
]);

var addMultiple = annotate('addMultiple', 'Adds multiple')
    .on(is.number, is.number, add)
    .on(is.string, is.string, add);
aSuite(addMultiple, 'Adds multiple', [[is.number, is.number], [is.string, is.string]], [], 'addMultiple');
suite(addMultiple, [
    ['a', 'b'], 'ab',
    [1, 2], 3
]);

var fib = annotate('fib', 'Calculates Fibonacci numbers').on(0, 0).on(1, 1)
    .on(is.number, function(n) {
        return fib(n - 1) + fib(n - 2);
    });
aSuite(fib, 'Calculates Fibonacci numbers', [[0], [1], [is.number]], [], 'fib');
suite(fib, [
    0, 0,
    1, 1,
    2, 1,
    12, 144
]);

if(WARNINGS) {
    fib('foobar'); // should yield a warning
}

var testObject = annotate('testObject', 'Tests object parameter after array')
    .on([is.array], function(o) {
        return o.reduce(function(a, b) {
            return a + b;
        });
    })
    .on(is.object, function(o) {
        return 'object';
    })

suite(testObject, [
    [[1, 2, 3]], 6,
    {a: 5}, 'object'
]);

var clamp = annotate('clamp', 'Clamps given number between given bounds')
    .on(is.number, is.number, function(a, args) {
        return is.number(a) && args[1] <= a;
    }, function(a, min, max) {
        return Math.max(Math.min(a, max), min);
    });

clamp(5, 10, 20); // ok

if(WARNINGS) {
    clamp(5, 10, 2); // should yield a warning
}

suite._amount = 10;
suite._generator = function() {
    return 1;
};
suite(clamp, function(op, a, min, max) {
    op(a, min, max);

    return min <= a && a <= max;
});

var min = annotate('min', 'Returns minimum of the given numbers')
    .on([is.number], Math.min);

if(WARNINGS) {
    min('a', 'b'); // should yield a warning
    min(2, 'a'); // should yield a warning
}

// postcondition
var failClamp = annotate('failClamp', 'Clamps given number between given bounds. Supposed to fail')
    .on(is.number, is.number, is.number, function(a, min) {
        return Math.max(a, min);
    })
    .satisfies(function(res, a, b) {
        return a <= res && res <= b;
    });

if(WARNINGS) {
    failClamp(100, 10, 20); // should yield a warning
}

function aSuite(fn, doc, preconditions, postconditions, name) {
    suite(partial(getMeta, fn), [
        '_doc', doc,
        '_preconditions', preconditions,
        '_postconditions', postconditions,
        '_name', name
    ]);
}

function add(a, b) {
    return a + b;
}

function getMeta(fn, name) {
    return fn[name];
}
