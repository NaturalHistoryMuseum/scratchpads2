[![build status](https://secure.travis-ci.org/annojs/annotate)](http://travis-ci.org/annojs/annotate)
# annotate - Annotate your JavaScript function definitions

`annotate` allows you to ... guess what ... annotate your functions. For
instance you could document invariants of your function. Or attach a
description to it. It is possible to access this data later on.

This metadata can be used by tools such as [annofuzz](https://github.com/annojs/fuzz)
in order to generate tests. In addition you can access the metadata via REPL.

The usage is quite simple as the following example illustrates:

```javascript
// let's define some function to annotate
function add(a, b) {
    return a + b;
}

// type checkers from annois (https://npmjs.org/package/annois)
var addNumbers = annotate('addNumbers', 'Adds numbers').
    on(is.number, is.number, add);
var addStrings = annotate('addStrings', 'Adds strings').
    on(is.string, is.string, add);

// you can assert invariants too
var addPositive = annotate('addPositive', 'Adds positive').
    on(isPositive, isPositive, add).
    satisfies(isPositive); // postcondition

// it is possible to chain guards
var fib = annotate('fib', 'Calculates Fibonacci numbers').
    on(0, 0).on(1, 1).
    on(is.number, function(n) {
        return fib(n - 1) + fib(n - 2);
    });

// invariants may depend on each other
var clamp = annotate('clamp', 'Clamps given number between given bounds').
    on(is.number, is.number, function(a, args) {
        return is.number(a) && args[1] <= a;
    }, function(a, min, max) {
        return Math.max(Math.min(a, max), min);
    });

// furthermore it is possible to pass a variable amount of args
var min = annotate('min', 'Returns minimum of the given numbers').
    on([is.number], Math.min);

function isPositive(a) {
    return a >= 0;
}
```

The `annotate` function will create a new function that contains the metadata as
properties `_name`, `_doc`, `_preconditions` and `_postconditions`. In case
some pre- or postcondition doesn't pass it won't return and gives a warning
instead.

## Related Projects

* [suite.js](https://github.com/bebraw/suite.js) - Constructs tests based on invariant data (fuzzing)
* [funkit](https://github.com/bebraw/funkit) - Collection of utilities tested using `annotate.js` and `suite.js`

## Acknowledgements

* [Kris Jordan](http://krisjordan.com/)'s [multimethod.js](http://krisjordan.com/multimethod-js) - Provided inspiration for the API

## License

`annotate` is available under MIT. See LICENSE for more details.

