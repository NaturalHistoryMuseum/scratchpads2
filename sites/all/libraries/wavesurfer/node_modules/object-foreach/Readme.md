# object-foreach

Executes a function on each of an objects own enumerable properties. The
 callback function will receive three arguments: the value of the current
 property, the name of the property, and the object being processed. This is
 roughly equivalent to the signature for callbacks to
 Array.prototype.forEach.

## Installation

```
npm install object-foreach
```

## Usage

In node:

```
var objectForeach = require('object-foreach');
var x = {
            a : 'v',
            b : 'v',
            c : 'v',
            d : 'v'
        };
        objectForeach(x, function (val, prop, obj) {
            obj[prop] = 'y';
        });
console.log(x); // {a:'y', b:'y', c:'y', d:'y'}
```

In the browser, include `./browser/object-foreach_web.js` in your page. `objectForeach` will
 be available in your page.
