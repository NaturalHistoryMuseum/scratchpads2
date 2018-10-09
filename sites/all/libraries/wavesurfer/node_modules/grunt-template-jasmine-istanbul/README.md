[![NPM version][npm-image]][npm-url]
[![build status][travis-image]][travis-url]
[![Downloads][downloads-image]][downloads-url]


# Code coverage template mix-in for [grunt-contrib-jasmine](https://github.com/gruntjs/grunt-contrib-jasmine), using [istanbul](https://github.com/gotwarlost/istanbul)

## Installation

```
npm install grunt-template-jasmine-istanbul --save-dev
```

## Template Options

### templateOptions.coverage
Type: `String`
Mandatory.

The file path where to store the `coverage.json`.

### templateOptions.report
Type: `String | Object | Array`
Mandatory.

If a `String` is given, it will be used as the path where a HTML report is generated.
If an `Object` is given, it must have the properties `type` and `options`, where `type` is a `String` and `options` an `Object`.
`type` and `options` are used to create the report by passing it to `istanbul`s [`Report.create(type, options)`](http://gotwarlost.github.com/istanbul/public/apidocs/classes/Report.html).
For example, if you want to generate a Cobertura report at `bin/coverage/cobertura`, use this:

````js
report: {
	type: 'cobertura',
	options: {
		dir: 'bin/coverage/cobertura'
	}
}
````

If an `Array` is given, it must consist of `Object`s of the form just described.

The supported types are:

 * [`html`](http://gotwarlost.github.com/istanbul/public/apidocs/classes/HtmlReport.html)
 * [`text`](http://gotwarlost.github.com/istanbul/public/apidocs/classes/TextReport.html)
 * [`text-summary`](http://gotwarlost.github.com/istanbul/public/apidocs/classes/TextSummaryReport.html)
 * [`lcov`](http://gotwarlost.github.com/istanbul/public/apidocs/classes/LcovReport.html)
 * [`lcovonly`](http://gotwarlost.github.com/istanbul/public/apidocs/classes/LcovOnlyReport.html)
 * [`cobertura`](http://gotwarlost.github.com/istanbul/public/apidocs/classes/CoberturaReport.html)

### templateOptions.files

Type: `String|Array`
Default: `**/*`

A globbing pattern or multiple patterns for the source files to instrument.
All source files that do match will be instrumented, those who don't won't.
E.g. `['**/*', '!src/main/js/uninteresting.js']` will result in all source files being instrumented except `src/main/js/uninteresting.js`.

### templateOptions.replace

Type: `Boolean|Function`
Default: `true`

Whether or not the `src` scripts are replaced by the paths to their instrumented versions.
This is useful when you want the mixed-in template to work with the original sources, and you want to serve the instrumented sources by redirecting request on the server side.
If you don't want the sources to be replaced, set it to `false`.
If it is a function, it receives the arguments `ìnstrumentedSource` and `source` which represent the URI to the instrumented file and the uninstrumented file respectively, relative to the directory specified by [`outfile`](https://github.com/gruntjs/grunt-contrib-jasmine#optionsoutfile).

### templateOptions.thresholds
Type: `Object`
Default: `undefined`

Thresholds for any of the metrics that Istanbul measures.
If a threshold is not met, a warning is emitted.
See example below for available metrics.

### templateOptions.template
Type: `String | Object`
Default: jasmine's default template

The template to mix-in coverage.

### templateOptions.templateOptions
Type: `Object`
Default: `undefined`

The options to pass to the mixed-in template.

## Examples

There are multiple examples at a [example repository](https://github.com/maenu/grunt-template-jasmine-istanbul-example).

### Simple

Have a look at [this example](https://github.com/maenu/grunt-template-jasmine-istanbul-example).

```js
// Example configuration
grunt.initConfig({
	jasmine: {
		coverage: {
			src: ['src/main/js/*.js'],
			options: {
				specs: ['src/test/js/*.js'],
				template: require('grunt-template-jasmine-istanbul'),
				templateOptions: {
					coverage: 'bin/coverage/coverage.json',
					report: 'bin/coverage',
                    thresholds: {
                        lines: 75,
                        statements: 75,
                        branches: 75,
                        functions: 90
                    }
				}
			}
		}
	}
}
```

### RequireJS

Have a look at [this example](https://github.com/maenu/grunt-template-jasmine-istanbul-example/tree/requirejs).
Note that you need to configure the `baseUrl` to point to the instrumented sources, as described in the section [below](https://github.com/maenu/grunt-template-jasmine-istanbul#a-single-arequirement).

```js
grunt.initConfig({
    jasmine: {
        coverage: {
            src: ['src/main/js/*.js'],
            options: {
                specs: ['src/test/js/*.js'],
                template: require('grunt-template-jasmine-istanbul'),
                templateOptions: {
                    coverage: 'bin/coverage/coverage.json',
                    report: 'bin/coverage',
                    template: require('grunt-template-jasmine-requirejs'),
                    templateOptions: {
                        requireConfig: {
                            baseUrl: '.grunt/grunt-contrib-jasmine/src/main/js/'
                        }
                    }
                }
            }
        }
    }
}
```

#### Is it really that easy?

No.

Setting `baseUrl` to that location may screw up your whole configuration, because paths relative to the original sources are broken.
Therefore, if this happens to you, instead of directly loading the instrumented sources, set `replace: false`, intercept request to the original sources and redirect them to the instrumented versions.
You can do this on both the [client side](https://github.com/maenu/grunt-template-jasmine-istanbul-example/tree/requirejs-client), or the [server side](https://github.com/maenu/grunt-template-jasmine-istanbul-example/tree/requirejs-server).
Look at the corresponding `Grunfile.js` files and be filled with horror: Yes, this is nasty, but it (seems to) works.

## Mixed-in Templates

### The Idea

Do you have another template you want to use, but you also want to collect code coverage at the same time?
Then you can use a mixed-in template, that's what they are for.
The idea behind a mixed-in template is simple:
Istanbul generates code coverage information by instrumenting the sources before they are run and by generating reports after they have run.
Therefore this templates acts as a test pre- and post-processor, but it doesn't interfere with the actual running of the tests.
This makes it possible to use another template as a mix-in template to run the tests, defined by `templateOptions.template` and can be configured with `templateOptions.templateOptions`.

### A Single Requirement

A mixed-in template needs to load the instrumented sources in order for the coverage reports to be correctly generated.
This template copies instrumented versions of the sources to a temporary location at `.grunt/grunt-contrib-jasmine/`.
If your mixed-in template simply includes the sources, as the default template does, you don't need to account for that, since this template replaces the `src` option with the paths to the instrumented versions.
If your mixed-in template loads the sources differently, e.g. directly from the file system, you may need to reconfigure the mixed-in template.

## Change Log
 * v0.5.0, 01.06.16, Upgrade `istanbul`, `grunt` and dev deps to work with the latest version of Grunt 1.0, use caret qualifier for versions
 * v0.4.0, 09.11.15, Upgrade `istanbul` and removed `lodash` to only use `lodash.template`.
 * v0.3.4, 28.06.15, closes [#40](https://github.com/maenu/grunt-template-jasmine-istanbul/issues/40), Don't throw error in reporter if coverage was not run.
 * v0.3.3, 27.01.15, closes [#36](https://github.com/maenu/grunt-template-jasmine-istanbul/issues/36), require lodash 3
 * v0.3.2, 23.01.15, closes [#33](https://github.com/maenu/grunt-template-jasmine-istanbul/issues/33) and [#28](https://github.com/maenu/grunt-template-jasmine-istanbul/issues/28)
 * v0.3.1, 02.02.14, closes [#14](https://github.com/maenu/grunt-template-jasmine-istanbul/issues/14) and [#17](https://github.com/maenu/grunt-template-jasmine-istanbul/issues/17), added `files` option and function support for `replace` option
 * v0.3.0, 02.02.14, fixes [#18](https://github.com/maenu/grunt-template-jasmine-istanbul/issues/18) and [#26](https://github.com/maenu/grunt-template-jasmine-istanbul/issues/26), now requires grunt-contrib-jasmine v0.6.0
 * v0.2.6, 02.02.14, merged [#13](https://github.com/maenu/grunt-template-jasmine-istanbul/issues/13) from @llacroix, windows paths are converted to URIs
 * v0.2.5, 10.08.13, reporter is now moved to and loaded from jasmine's temporary directory, fixes [#11](https://github.com/maenu/grunt-template-jasmine-istanbul/issues/11)
 * v0.2.4, 26.05.13, merged [#12](https://github.com/maenu/grunt-template-jasmine-istanbul/issues/12) from @kayhadrin, instrumented versions of files loaded via absolute paths on windows are now created at a valid path
 * v0.2.3, 12.05.13, merged `thresholds` from @larsthorup [#9](https://github.com/maenu/grunt-template-jasmine-istanbul/issues/9) which can abort a build with too low coverage
 * v0.2.2, 11.05.13, added `replace` option, so it can be prevented that the original `src` option is replaced with their instrumented versions

[npm-image]: https://img.shields.io/npm/v/grunt-template-jasmine-istanbul.svg?style=flat-square
[npm-url]: https://npmjs.org/package/grunt-template-jasmine-istanbul
[travis-image]: https://img.shields.io/travis/maenu/grunt-template-jasmine-istanbul/master.svg?style=flat-square
[travis-url]: https://travis-ci.org/maenu/grunt-template-jasmine-istanbul
[downloads-image]: http://img.shields.io/npm/dm/grunt-template-jasmine-istanbul.svg?style=flat-square
[downloads-url]: https://npmjs.org/package/grunt-template-jasmine-istanbul
