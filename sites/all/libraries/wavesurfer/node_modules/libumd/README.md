[![build status](https://secure.travis-ci.org/bebraw/libumd.png)](http://travis-ci.org/bebraw/libumd)

# libumd - Wraps given JavaScript code with UMD

## Usage

```js
var umdify = require('libumd');

...

var result = umdify(js, options);
```

> `libumd` doesn't guarantee pretty formatting. It is better to use something like [js-beautify](https://www.npmjs.com/package/js-beautify) to deal with that.

## Options

```js
{
    template: 'path to template or template name', // defaults to 'umd'
    amdModuleId: 'test', // optional AMD module id. defaults to anonymous (not set)
    globalAlias: 'alias', // name of the global variable
    deps: { // dependencies
        // use default only if the module name and the variable the module will be injected with is the same
        'default': ['Foo', 'Bar'],
        // additionally define these if the module name differs from the variable with which it will be used
        // note how we can map dependencies to specific parameters
        amd: ['foo', {'lodash': '_'}],
        cjs: ['foo', 'bar']
    }
}
```

> Check out the [`demo.js`](https://github.com/bebraw/libumd/blob/master/demo.js)

## Default Templates

The library comes with a couple of UMD variants at `/templates`. In addition you may use one of your own as long as it is formatted using Handlebars syntax and follows the same naming conventions as the ones provided with the project.

## Testing

Make sure [PhantomJS](http://phantomjs.org/) is installed and it's within your PATH. Hit `npm test` after that. If the UMD wrapper fails to run against the headless browser, you'll know.

## Contributors

* [Stéphane Bachelier](https://github.com/stephanebachelier) - Use existing `objectToExport` instead of hardcoded value `returnExportsGlobal` for AMD.
* [Simon Harte](https://github.com/SimonHarte) - Made the documentation clearer about the correct usage.
* [Valerii Zinchenko](https://github.com/valerii-zinchenko) - Allowed dependency name to contain a dash. #17
* [@timeiscoffee](https://github.com/timeiscoffee) - Updated UMD templates to the current scheme. #18

## License

`libumd` is available under MIT. See LICENSE for more details.

