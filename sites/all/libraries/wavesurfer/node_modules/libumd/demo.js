'use strict';
var umdify = require('./');

main();

function main() {
    var result = umdify('demo();', {
        deps: {
            // use default only if the module name and the variable the module will be injected with is the same
            'default': ['Foo'],
            // additionally define these if the module name differs from the variable with which it will be used
            'amd': ['foo'],
            'cjs': ['foo']
        }
    });

    console.log(result);
    /**
     (function (root, factory) {
       if (typeof define === 'function' && define.amd) {
         // AMD. Register as an anonymous module unless amdModuleId is set
         define(["foo"], function (a0) {
           return (factory(a0));
         });
       } else if (typeof exports === 'object') {
         // Node. Does not work with strict CommonJS, but
         // only CommonJS-like environments that support module.exports,
         // like Node.
         module.exports = factory(require("foo"));
       } else {
         factory(Foo);
       }
     }(this, function (Foo) {
     
     demo();
     
     }));
     */
}
