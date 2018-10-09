[![build status](https://secure.travis-ci.org/annojs/zip.png)](http://travis-ci.org/annojs/zip)
# annozip - Zips data into structure

´annozip´ provides utilities that make it easier to deal with ´zip´ data structures. Consider the following examples:

```js
var zip = require('annozip');

var a = [0, 1, 2];
var b = ['a', 'b', 'c', 'd'];
var c = zip(a, b); // [[0, 'a'], [1, 'b'], [2, 'c']]

var o = {name: 'Joe', age: 123}
var ozip = zip(o); // [['name', 'Joe'], ['age', 123]]

var o2 = zip.toObject(ozip); // {name: 'Joe', age: 123}
```

## License

MIT. See LICENSE for more details.
