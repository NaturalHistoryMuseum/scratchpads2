# object-merge

Merges JavaScript objects recursively without altering the objects merged.

## Installation

```
npm install object-merge
```

https://npmjs.org/package/object-merge
Source code available at: https://github.com/matthewkastor/object-merge/

## Usage

In Node:

```
var objectMerge = require('object-merge');
var x = {
    a : 'a',
    b : 'b',
    c : {
        d : 'd',
        e : 'e',
        f : {
            g : 'g'
        }
    }
};
var y = {
    a : '`a',
    b : '`b',
    c : {
        d : '`d'
    }
};
var z = {
    a : {
        b : '``b'
    },
    fun : function foo () {
        return 'foo';
    },
    aps : Array.prototype.slice
};
var out = objectMerge(x, y, z);
// out.a will be {
//         b : '``b'
//     }
// out.b will be '`b'
// out.c will be {
//         d : '`d',
//         e : 'e',
//         f : {
//             g : 'g'
//         }
//     }
// out.fun will be a clone of z.fun
// out.aps will be equal to z.aps
```

Merging arrays is not the same as `concat`. When they're merged the arrays are
 handled as objects. This means that indexes are object properties with numeric
 names. Merging `['a']` with `['b']` will give you `['b']` because the two
 arrays both have a property `0` and the last one in overrides the first.
 However, merging `arr1['a', 'b']` with `arr2[1] = 'override'` will give you
 `['a', 'override']` because `arr1` has properties `0` and `1`, while `arr2`
 only has the property `1` which overrides `arr1[1]` in the output.

Merging functions will cause the output function to be a clone of the last
 function merged and it will have all the properties of the merged functions
 recursively merged together. So something like:

```
var func = function () {
    return null;
};
var func2 = function () {
    return 'hello';
};
func.wohoo = 'wohoo';
func.obj = {a:'a'};
func2.wee = 'wee';
func2.obj = {b:'b'};
func2.obj2 = {a:'a'};
var out = objectMerge(func, func2);
```

will give you a function `out` that is a clone of `func2`'s function definition
 but has the properties: `wohoo`, `obj`, `wee`, and `obj2`. `out.obj` will have
 the properties `a` and `b` because the properties of `func` and `func2` are
 merged recursively.
 
### Options

Options exist to do things like limit the depth of object traversal and disable
 errors on detection of circular references. In order to specify the options you
 must create an options object using the provided method `createOptions`, a
 normal object just won't work.

```
var objectMerge = require('object-merge');
var a = {
    'a1' : {
        'a2' : {
            'a3' : {}
        }
    }
};
var b = {
    'b1' : {
        'b2' : {
            'b3' : {}
        }
    }
};
var opts = objectMerge.createOptions({depth : 2});
var res = objectMerge(opts, a, b);
// res will be
// {
//     'a1' : {
//         'a2' : {}
//     },
//     'b1' : {
//         'b2' : {}
//     }
// }
```

See the tests in `browser/tests` for more examples and expected outputs.

In the browser, include `./browser/object-merge_web.js` in your page.
 `objectMerge` will be available in your page.

For full documentation see the docs folder.

## Tests

Tests can be run from the root of this package with

```
npm test
```

There are also browser tests available in the `browser` directory.

## Hacking

There are several other scripts listed in package.json for development and
 hacking on this module. They can be run with `npm run-script` followed by the
 scripts property corresponding to the script you want to run. For example,
 given a script called `buildDocs`, it could be run from the package root by:

```
npm run-script buildDocs
```

## Author

Matthew Kastor
matthewkastor@gmail.com
https://plus.google.com/100898583798552211130

## License

gpl-3.0
http://www.gnu.org/licenses/gpl-3.0-standalone.html