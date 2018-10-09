(function e(t,n,r){function s(o,u){if(!n[o]){if(!t[o]){var a=typeof require=="function"&&require;if(!u&&a)return a(o,!0);if(i)return i(o,!0);throw new Error("Cannot find module '"+o+"'")}var f=n[o]={exports:{}};t[o][0].call(f.exports,function(e){var n=t[o][1][e];return s(n?n:e)},f,f.exports,e,t,n,r)}return n[o].exports}var i=typeof require=="function"&&require;for(var o=0;o<r.length;o++)s(r[o]);return s})({1:[function(require,module,exports){
objectMerge = require('../src/object-merge.js');

},{"../src/object-merge.js":4}],2:[function(require,module,exports){
/*
License gpl-3.0 http://www.gnu.org/licenses/gpl-3.0-standalone.html
*/
/*jslint
    evil: true,
    node: true
*/
'use strict';
/**
 * Clones non native JavaScript functions, or references native functions.
 * @author <a href="mailto:matthewkastor@gmail.com">Matthew Kastor</a>
 * @param {Function} func The function to clone.
 * @returns {Function} Returns a clone of the non native function, or a
 *  reference to the native function.
 */
function cloneFunction(func) {
    var out, str;
    try {
        str = func.toString();
        if (/\[native code\]/.test(str)) {
            out = func;
        } else {
            out = eval('(function(){return ' + str + '}());');
        }
    } catch (e) {
        throw new Error(e.message + '\r\n\r\n' + str);
    }
    return out;
}
module.exports = cloneFunction;
},{}],3:[function(require,module,exports){
/**
 * Executes a function on each of an objects own enumerable properties. The
 *  callback function will receive three arguments: the value of the current
 *  property, the name of the property, and the object being processed. This is
 *  roughly equivalent to the signature for callbacks to
 *  Array.prototype.forEach.
 * @param {Object} obj The object to act on.
 * @param {Function} callback The function to execute.
 * @returns {Object} Returns the given object.
 */
function objectForeach(obj, callback) {
    "use strict";
    Object.keys(obj).forEach(function (prop) {
        callback(obj[prop], prop, obj);
    });
    return obj;
};
module.exports = objectForeach;
},{}],4:[function(require,module,exports){
/*
License gpl-3.0 http://www.gnu.org/licenses/gpl-3.0-standalone.html
*/
/*jslint
    white: true,
    vars: true,
    node: true
*/
function ObjectMergeOptions(opts) {
    'use strict';
    opts = opts || {};
    this.depth = opts.depth || false;
    // circular ref check is true unless explicitly set to false
    // ignore the jslint warning here, it's pointless.
    this.throwOnCircularRef = 'throwOnCircularRef' in opts && opts.throwOnCircularRef === false ? false : true;
}
/*jslint unparam:true*/
/**
 * Creates a new options object suitable for use with objectMerge.
 * @memberOf objectMerge
 * @param {Object} [opts] An object specifying the options.
 * @param {Object} [opts.depth = false] Specifies the depth to traverse objects
 *  during merging. If this is set to false then there will be no depth limit.
 * @param {Object} [opts.throwOnCircularRef = true] Set to false to suppress
 *  errors on circular references.
 * @returns {ObjectMergeOptions} Returns an instance of ObjectMergeOptions
 *  to be used with objectMerge.
 * @example
 *  var opts = objectMerge.createOptions({
 *      depth : 2,
 *      throwOnCircularRef : false
 *  });
 *  var obj1 = {
 *      a1 : {
 *          a2 : {
 *              a3 : {}
 *          }
 *      }
 *  };
 *  var obj2 = {
 *      a1 : {
 *          a2 : {
 *              a3 : 'will not be in output'
 *          },
 *          a22 : {}
 *      }
 *  };
 *  objectMerge(opts, obj1, obj2);
 */
function createOptions(opts) {
    'use strict';
    var argz = Array.prototype.slice.call(arguments, 0);
    argz.unshift(null);
    var F = ObjectMergeOptions.bind.apply(ObjectMergeOptions, argz);
    return new F();
}
/*jslint unparam:false*/
/**
 * Merges JavaScript objects recursively without altering the objects merged.
 * @namespace
 * @author <a href="mailto:matthewkastor@gmail.com">Matthew Kastor</a>
 * @param {ObjectMergeOptions} [opts] An options object created by 
 *  objectMerge.createOptions. Options must be specified as the first argument
 *  and must be an object created with createOptions or else the object will
 *  not be recognized as an options object and will be merged instead.
 * @param {Object} shadows [[shadows]...] One or more objects to merge. Each
 *  argument given will be treated as an object to merge. Each object
 *  overwrites the previous objects descendant properties if the property name
 *  matches. If objects properties are objects they will be merged recursively
 *  as well.
 * @returns {Object} Returns a single merged object composed from clones of the
 *  input objects.
 * @example
 *  var objectMerge = require('object-merge');
 *  var x = {
 *      a : 'a',
 *      b : 'b',
 *      c : {
 *          d : 'd',
 *          e : 'e',
 *          f : {
 *              g : 'g'
 *          }
 *      }
 *  };
 *  var y = {
 *      a : '`a',
 *      b : '`b',
 *      c : {
 *          d : '`d'
 *      }
 *  };
 *  var z = {
 *      a : {
 *          b : '``b'
 *      },
 *      fun : function foo () {
 *          return 'foo';
 *      },
 *      aps : Array.prototype.slice
 *  };
 *  var out = objectMerge(x, y, z);
 *  // out.a will be {
 *  //         b : '``b'
 *  //     }
 *  // out.b will be '`b'
 *  // out.c will be {
 *  //         d : '`d',
 *  //         e : 'e',
 *  //         f : {
 *  //             g : 'g'
 *  //         }
 *  //     }
 *  // out.fun will be a clone of z.fun
 *  // out.aps will be equal to z.aps
 */
function objectMerge(shadows) {
    'use strict';
    var objectForeach = require('object-foreach');
    var cloneFunction = require('clone-function');
    // this is the queue of visited objects / properties.
    var visited = [];
    // various merge options
    var options = {};
    // gets the sequential trailing objects from array.
    function getShadowObjects(shadows) {
        var out = shadows.reduce(function (collector, shadow) {
                if (shadow instanceof Object) {
                    collector.push(shadow);
                } else {
                    collector = [];
                }
                return collector;
            }, []);
        return out;
    }
    // gets either a new object of the proper type or the last primitive value
    function getOutputObject(shadows) {
        var out;
        var lastShadow = shadows[shadows.length - 1];
        if (lastShadow instanceof Array) {
            out = [];
        } else if (lastShadow instanceof Function) {
            try {
                out = cloneFunction(lastShadow);
            } catch (e) {
                throw new Error(e.message);
            }
        } else if (lastShadow instanceof Object) {
            out = {};
        } else {
            // lastShadow is a primitive value;
            out = lastShadow;
        }
        return out;
    }
    // checks for circular references
    function circularReferenceCheck(shadows) {
        // if any of the current objects to process exist in the queue
        // then throw an error.
        shadows.forEach(function (item) {
            if (item instanceof Object && visited.indexOf(item) > -1) {
                throw new Error('Circular reference error');
            }
        });
        // if none of the current objects were in the queue
        // then add references to the queue.
        visited = visited.concat(shadows);
    }
    function objectMergeRecursor(shadows, currentDepth) {
        if (options.depth !== false) {
            currentDepth = currentDepth ? currentDepth + 1 : 1;
        } else {
            currentDepth = 0;
        }
        if (options.throwOnCircularRef === true) {
            circularReferenceCheck(shadows);
        }
        var out = getOutputObject(shadows);
        /*jslint unparam: true */
        function shadowHandler(val, prop, shadow) {
            if (out[prop]) {
                out[prop] = objectMergeRecursor([
                    out[prop],
                    shadow[prop]
                ], currentDepth);
            } else {
                out[prop] = objectMergeRecursor([shadow[prop]], currentDepth);
            }
        }
        /*jslint unparam:false */
        function shadowMerger(shadow) {
            objectForeach(shadow, shadowHandler);
        }
        // short circuits case where output would be a primitive value
        // anyway.
        if (out instanceof Object && currentDepth <= options.depth) {
            // only merges trailing objects since primitives would wipe out
            // previous objects, as in merging {a:'a'}, 'a', and {b:'b'}
            // would result in {b:'b'} so the first two arguments
            // can be ignored completely.
            var relevantShadows = getShadowObjects(shadows);
            relevantShadows.forEach(shadowMerger);
        }
        return out;
    }
    // determines whether an options object was passed in and
    // uses it if present
    // ignore the jslint warning here too.
    if (arguments[0] instanceof ObjectMergeOptions) {
        options = arguments[0];
        shadows = Array.prototype.slice.call(arguments, 1);
    } else {
        options = createOptions();
        shadows = Array.prototype.slice.call(arguments, 0);
    }
    return objectMergeRecursor(shadows);
}
objectMerge.createOptions = createOptions;
module.exports = objectMerge;
},{"clone-function":2,"object-foreach":3}]},{},[1])
//@ sourceMappingURL=data:application/json;base64,eyJ2ZXJzaW9uIjozLCJmaWxlIjoiZ2VuZXJhdGVkLmpzIiwic291cmNlcyI6WyJDOi9Vc2Vycy9rYXN0b3IvRG9jdW1lbnRzL0dpdEh1Yi9vYmplY3QtbWVyZ2UvZGV2L2Jyb3dzZXJNYWluLmpzIiwiQzovVXNlcnMva2FzdG9yL0RvY3VtZW50cy9HaXRIdWIvb2JqZWN0LW1lcmdlL25vZGVfbW9kdWxlcy9jbG9uZS1mdW5jdGlvbi9zcmMvY2xvbmUtZnVuY3Rpb24uanMiLCJDOi9Vc2Vycy9rYXN0b3IvRG9jdW1lbnRzL0dpdEh1Yi9vYmplY3QtbWVyZ2Uvbm9kZV9tb2R1bGVzL29iamVjdC1mb3JlYWNoL3NyYy9vYmplY3QtZm9yZWFjaC5qcyIsIkM6L1VzZXJzL2thc3Rvci9Eb2N1bWVudHMvR2l0SHViL29iamVjdC1tZXJnZS9zcmMvb2JqZWN0LW1lcmdlLmpzIl0sIm5hbWVzIjpbXSwibWFwcGluZ3MiOiI7QUFBQTtBQUNBOztBQ0RBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTs7QUM3QkE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBOztBQ2pCQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQSIsInNvdXJjZXNDb250ZW50IjpbIm9iamVjdE1lcmdlID0gcmVxdWlyZSgnLi4vc3JjL29iamVjdC1tZXJnZS5qcycpO1xyXG4iLCIvKlxyXG5MaWNlbnNlIGdwbC0zLjAgaHR0cDovL3d3dy5nbnUub3JnL2xpY2Vuc2VzL2dwbC0zLjAtc3RhbmRhbG9uZS5odG1sXHJcbiovXHJcbi8qanNsaW50XHJcbiAgICBldmlsOiB0cnVlLFxyXG4gICAgbm9kZTogdHJ1ZVxyXG4qL1xyXG4ndXNlIHN0cmljdCc7XHJcbi8qKlxyXG4gKiBDbG9uZXMgbm9uIG5hdGl2ZSBKYXZhU2NyaXB0IGZ1bmN0aW9ucywgb3IgcmVmZXJlbmNlcyBuYXRpdmUgZnVuY3Rpb25zLlxyXG4gKiBAYXV0aG9yIDxhIGhyZWY9XCJtYWlsdG86bWF0dGhld2thc3RvckBnbWFpbC5jb21cIj5NYXR0aGV3IEthc3RvcjwvYT5cclxuICogQHBhcmFtIHtGdW5jdGlvbn0gZnVuYyBUaGUgZnVuY3Rpb24gdG8gY2xvbmUuXHJcbiAqIEByZXR1cm5zIHtGdW5jdGlvbn0gUmV0dXJucyBhIGNsb25lIG9mIHRoZSBub24gbmF0aXZlIGZ1bmN0aW9uLCBvciBhXHJcbiAqICByZWZlcmVuY2UgdG8gdGhlIG5hdGl2ZSBmdW5jdGlvbi5cclxuICovXHJcbmZ1bmN0aW9uIGNsb25lRnVuY3Rpb24oZnVuYykge1xyXG4gICAgdmFyIG91dCwgc3RyO1xyXG4gICAgdHJ5IHtcclxuICAgICAgICBzdHIgPSBmdW5jLnRvU3RyaW5nKCk7XHJcbiAgICAgICAgaWYgKC9cXFtuYXRpdmUgY29kZVxcXS8udGVzdChzdHIpKSB7XHJcbiAgICAgICAgICAgIG91dCA9IGZ1bmM7XHJcbiAgICAgICAgfSBlbHNlIHtcclxuICAgICAgICAgICAgb3V0ID0gZXZhbCgnKGZ1bmN0aW9uKCl7cmV0dXJuICcgKyBzdHIgKyAnfSgpKTsnKTtcclxuICAgICAgICB9XHJcbiAgICB9IGNhdGNoIChlKSB7XHJcbiAgICAgICAgdGhyb3cgbmV3IEVycm9yKGUubWVzc2FnZSArICdcXHJcXG5cXHJcXG4nICsgc3RyKTtcclxuICAgIH1cclxuICAgIHJldHVybiBvdXQ7XHJcbn1cclxubW9kdWxlLmV4cG9ydHMgPSBjbG9uZUZ1bmN0aW9uOyIsIi8qKlxyXG4gKiBFeGVjdXRlcyBhIGZ1bmN0aW9uIG9uIGVhY2ggb2YgYW4gb2JqZWN0cyBvd24gZW51bWVyYWJsZSBwcm9wZXJ0aWVzLiBUaGVcclxuICogIGNhbGxiYWNrIGZ1bmN0aW9uIHdpbGwgcmVjZWl2ZSB0aHJlZSBhcmd1bWVudHM6IHRoZSB2YWx1ZSBvZiB0aGUgY3VycmVudFxyXG4gKiAgcHJvcGVydHksIHRoZSBuYW1lIG9mIHRoZSBwcm9wZXJ0eSwgYW5kIHRoZSBvYmplY3QgYmVpbmcgcHJvY2Vzc2VkLiBUaGlzIGlzXHJcbiAqICByb3VnaGx5IGVxdWl2YWxlbnQgdG8gdGhlIHNpZ25hdHVyZSBmb3IgY2FsbGJhY2tzIHRvXHJcbiAqICBBcnJheS5wcm90b3R5cGUuZm9yRWFjaC5cclxuICogQHBhcmFtIHtPYmplY3R9IG9iaiBUaGUgb2JqZWN0IHRvIGFjdCBvbi5cclxuICogQHBhcmFtIHtGdW5jdGlvbn0gY2FsbGJhY2sgVGhlIGZ1bmN0aW9uIHRvIGV4ZWN1dGUuXHJcbiAqIEByZXR1cm5zIHtPYmplY3R9IFJldHVybnMgdGhlIGdpdmVuIG9iamVjdC5cclxuICovXHJcbmZ1bmN0aW9uIG9iamVjdEZvcmVhY2gob2JqLCBjYWxsYmFjaykge1xyXG4gICAgXCJ1c2Ugc3RyaWN0XCI7XHJcbiAgICBPYmplY3Qua2V5cyhvYmopLmZvckVhY2goZnVuY3Rpb24gKHByb3ApIHtcclxuICAgICAgICBjYWxsYmFjayhvYmpbcHJvcF0sIHByb3AsIG9iaik7XHJcbiAgICB9KTtcclxuICAgIHJldHVybiBvYmo7XHJcbn07XHJcbm1vZHVsZS5leHBvcnRzID0gb2JqZWN0Rm9yZWFjaDsiLCIvKlxyXG5MaWNlbnNlIGdwbC0zLjAgaHR0cDovL3d3dy5nbnUub3JnL2xpY2Vuc2VzL2dwbC0zLjAtc3RhbmRhbG9uZS5odG1sXHJcbiovXHJcbi8qanNsaW50XHJcbiAgICB3aGl0ZTogdHJ1ZSxcclxuICAgIHZhcnM6IHRydWUsXHJcbiAgICBub2RlOiB0cnVlXHJcbiovXHJcbmZ1bmN0aW9uIE9iamVjdE1lcmdlT3B0aW9ucyhvcHRzKSB7XHJcbiAgICAndXNlIHN0cmljdCc7XHJcbiAgICBvcHRzID0gb3B0cyB8fCB7fTtcclxuICAgIHRoaXMuZGVwdGggPSBvcHRzLmRlcHRoIHx8IGZhbHNlO1xyXG4gICAgLy8gY2lyY3VsYXIgcmVmIGNoZWNrIGlzIHRydWUgdW5sZXNzIGV4cGxpY2l0bHkgc2V0IHRvIGZhbHNlXHJcbiAgICAvLyBpZ25vcmUgdGhlIGpzbGludCB3YXJuaW5nIGhlcmUsIGl0J3MgcG9pbnRsZXNzLlxyXG4gICAgdGhpcy50aHJvd09uQ2lyY3VsYXJSZWYgPSAndGhyb3dPbkNpcmN1bGFyUmVmJyBpbiBvcHRzICYmIG9wdHMudGhyb3dPbkNpcmN1bGFyUmVmID09PSBmYWxzZSA/IGZhbHNlIDogdHJ1ZTtcclxufVxyXG4vKmpzbGludCB1bnBhcmFtOnRydWUqL1xyXG4vKipcclxuICogQ3JlYXRlcyBhIG5ldyBvcHRpb25zIG9iamVjdCBzdWl0YWJsZSBmb3IgdXNlIHdpdGggb2JqZWN0TWVyZ2UuXHJcbiAqIEBtZW1iZXJPZiBvYmplY3RNZXJnZVxyXG4gKiBAcGFyYW0ge09iamVjdH0gW29wdHNdIEFuIG9iamVjdCBzcGVjaWZ5aW5nIHRoZSBvcHRpb25zLlxyXG4gKiBAcGFyYW0ge09iamVjdH0gW29wdHMuZGVwdGggPSBmYWxzZV0gU3BlY2lmaWVzIHRoZSBkZXB0aCB0byB0cmF2ZXJzZSBvYmplY3RzXHJcbiAqICBkdXJpbmcgbWVyZ2luZy4gSWYgdGhpcyBpcyBzZXQgdG8gZmFsc2UgdGhlbiB0aGVyZSB3aWxsIGJlIG5vIGRlcHRoIGxpbWl0LlxyXG4gKiBAcGFyYW0ge09iamVjdH0gW29wdHMudGhyb3dPbkNpcmN1bGFyUmVmID0gdHJ1ZV0gU2V0IHRvIGZhbHNlIHRvIHN1cHByZXNzXHJcbiAqICBlcnJvcnMgb24gY2lyY3VsYXIgcmVmZXJlbmNlcy5cclxuICogQHJldHVybnMge09iamVjdE1lcmdlT3B0aW9uc30gUmV0dXJucyBhbiBpbnN0YW5jZSBvZiBPYmplY3RNZXJnZU9wdGlvbnNcclxuICogIHRvIGJlIHVzZWQgd2l0aCBvYmplY3RNZXJnZS5cclxuICogQGV4YW1wbGVcclxuICogIHZhciBvcHRzID0gb2JqZWN0TWVyZ2UuY3JlYXRlT3B0aW9ucyh7XHJcbiAqICAgICAgZGVwdGggOiAyLFxyXG4gKiAgICAgIHRocm93T25DaXJjdWxhclJlZiA6IGZhbHNlXHJcbiAqICB9KTtcclxuICogIHZhciBvYmoxID0ge1xyXG4gKiAgICAgIGExIDoge1xyXG4gKiAgICAgICAgICBhMiA6IHtcclxuICogICAgICAgICAgICAgIGEzIDoge31cclxuICogICAgICAgICAgfVxyXG4gKiAgICAgIH1cclxuICogIH07XHJcbiAqICB2YXIgb2JqMiA9IHtcclxuICogICAgICBhMSA6IHtcclxuICogICAgICAgICAgYTIgOiB7XHJcbiAqICAgICAgICAgICAgICBhMyA6ICd3aWxsIG5vdCBiZSBpbiBvdXRwdXQnXHJcbiAqICAgICAgICAgIH0sXHJcbiAqICAgICAgICAgIGEyMiA6IHt9XHJcbiAqICAgICAgfVxyXG4gKiAgfTtcclxuICogIG9iamVjdE1lcmdlKG9wdHMsIG9iajEsIG9iajIpO1xyXG4gKi9cclxuZnVuY3Rpb24gY3JlYXRlT3B0aW9ucyhvcHRzKSB7XHJcbiAgICAndXNlIHN0cmljdCc7XHJcbiAgICB2YXIgYXJneiA9IEFycmF5LnByb3RvdHlwZS5zbGljZS5jYWxsKGFyZ3VtZW50cywgMCk7XHJcbiAgICBhcmd6LnVuc2hpZnQobnVsbCk7XHJcbiAgICB2YXIgRiA9IE9iamVjdE1lcmdlT3B0aW9ucy5iaW5kLmFwcGx5KE9iamVjdE1lcmdlT3B0aW9ucywgYXJneik7XHJcbiAgICByZXR1cm4gbmV3IEYoKTtcclxufVxyXG4vKmpzbGludCB1bnBhcmFtOmZhbHNlKi9cclxuLyoqXHJcbiAqIE1lcmdlcyBKYXZhU2NyaXB0IG9iamVjdHMgcmVjdXJzaXZlbHkgd2l0aG91dCBhbHRlcmluZyB0aGUgb2JqZWN0cyBtZXJnZWQuXHJcbiAqIEBuYW1lc3BhY2VcclxuICogQGF1dGhvciA8YSBocmVmPVwibWFpbHRvOm1hdHRoZXdrYXN0b3JAZ21haWwuY29tXCI+TWF0dGhldyBLYXN0b3I8L2E+XHJcbiAqIEBwYXJhbSB7T2JqZWN0TWVyZ2VPcHRpb25zfSBbb3B0c10gQW4gb3B0aW9ucyBvYmplY3QgY3JlYXRlZCBieSBcclxuICogIG9iamVjdE1lcmdlLmNyZWF0ZU9wdGlvbnMuIE9wdGlvbnMgbXVzdCBiZSBzcGVjaWZpZWQgYXMgdGhlIGZpcnN0IGFyZ3VtZW50XHJcbiAqICBhbmQgbXVzdCBiZSBhbiBvYmplY3QgY3JlYXRlZCB3aXRoIGNyZWF0ZU9wdGlvbnMgb3IgZWxzZSB0aGUgb2JqZWN0IHdpbGxcclxuICogIG5vdCBiZSByZWNvZ25pemVkIGFzIGFuIG9wdGlvbnMgb2JqZWN0IGFuZCB3aWxsIGJlIG1lcmdlZCBpbnN0ZWFkLlxyXG4gKiBAcGFyYW0ge09iamVjdH0gc2hhZG93cyBbW3NoYWRvd3NdLi4uXSBPbmUgb3IgbW9yZSBvYmplY3RzIHRvIG1lcmdlLiBFYWNoXHJcbiAqICBhcmd1bWVudCBnaXZlbiB3aWxsIGJlIHRyZWF0ZWQgYXMgYW4gb2JqZWN0IHRvIG1lcmdlLiBFYWNoIG9iamVjdFxyXG4gKiAgb3ZlcndyaXRlcyB0aGUgcHJldmlvdXMgb2JqZWN0cyBkZXNjZW5kYW50IHByb3BlcnRpZXMgaWYgdGhlIHByb3BlcnR5IG5hbWVcclxuICogIG1hdGNoZXMuIElmIG9iamVjdHMgcHJvcGVydGllcyBhcmUgb2JqZWN0cyB0aGV5IHdpbGwgYmUgbWVyZ2VkIHJlY3Vyc2l2ZWx5XHJcbiAqICBhcyB3ZWxsLlxyXG4gKiBAcmV0dXJucyB7T2JqZWN0fSBSZXR1cm5zIGEgc2luZ2xlIG1lcmdlZCBvYmplY3QgY29tcG9zZWQgZnJvbSBjbG9uZXMgb2YgdGhlXHJcbiAqICBpbnB1dCBvYmplY3RzLlxyXG4gKiBAZXhhbXBsZVxyXG4gKiAgdmFyIG9iamVjdE1lcmdlID0gcmVxdWlyZSgnb2JqZWN0LW1lcmdlJyk7XHJcbiAqICB2YXIgeCA9IHtcclxuICogICAgICBhIDogJ2EnLFxyXG4gKiAgICAgIGIgOiAnYicsXHJcbiAqICAgICAgYyA6IHtcclxuICogICAgICAgICAgZCA6ICdkJyxcclxuICogICAgICAgICAgZSA6ICdlJyxcclxuICogICAgICAgICAgZiA6IHtcclxuICogICAgICAgICAgICAgIGcgOiAnZydcclxuICogICAgICAgICAgfVxyXG4gKiAgICAgIH1cclxuICogIH07XHJcbiAqICB2YXIgeSA9IHtcclxuICogICAgICBhIDogJ2BhJyxcclxuICogICAgICBiIDogJ2BiJyxcclxuICogICAgICBjIDoge1xyXG4gKiAgICAgICAgICBkIDogJ2BkJ1xyXG4gKiAgICAgIH1cclxuICogIH07XHJcbiAqICB2YXIgeiA9IHtcclxuICogICAgICBhIDoge1xyXG4gKiAgICAgICAgICBiIDogJ2BgYidcclxuICogICAgICB9LFxyXG4gKiAgICAgIGZ1biA6IGZ1bmN0aW9uIGZvbyAoKSB7XHJcbiAqICAgICAgICAgIHJldHVybiAnZm9vJztcclxuICogICAgICB9LFxyXG4gKiAgICAgIGFwcyA6IEFycmF5LnByb3RvdHlwZS5zbGljZVxyXG4gKiAgfTtcclxuICogIHZhciBvdXQgPSBvYmplY3RNZXJnZSh4LCB5LCB6KTtcclxuICogIC8vIG91dC5hIHdpbGwgYmUge1xyXG4gKiAgLy8gICAgICAgICBiIDogJ2BgYidcclxuICogIC8vICAgICB9XHJcbiAqICAvLyBvdXQuYiB3aWxsIGJlICdgYidcclxuICogIC8vIG91dC5jIHdpbGwgYmUge1xyXG4gKiAgLy8gICAgICAgICBkIDogJ2BkJyxcclxuICogIC8vICAgICAgICAgZSA6ICdlJyxcclxuICogIC8vICAgICAgICAgZiA6IHtcclxuICogIC8vICAgICAgICAgICAgIGcgOiAnZydcclxuICogIC8vICAgICAgICAgfVxyXG4gKiAgLy8gICAgIH1cclxuICogIC8vIG91dC5mdW4gd2lsbCBiZSBhIGNsb25lIG9mIHouZnVuXHJcbiAqICAvLyBvdXQuYXBzIHdpbGwgYmUgZXF1YWwgdG8gei5hcHNcclxuICovXHJcbmZ1bmN0aW9uIG9iamVjdE1lcmdlKHNoYWRvd3MpIHtcclxuICAgICd1c2Ugc3RyaWN0JztcclxuICAgIHZhciBvYmplY3RGb3JlYWNoID0gcmVxdWlyZSgnb2JqZWN0LWZvcmVhY2gnKTtcclxuICAgIHZhciBjbG9uZUZ1bmN0aW9uID0gcmVxdWlyZSgnY2xvbmUtZnVuY3Rpb24nKTtcclxuICAgIC8vIHRoaXMgaXMgdGhlIHF1ZXVlIG9mIHZpc2l0ZWQgb2JqZWN0cyAvIHByb3BlcnRpZXMuXHJcbiAgICB2YXIgdmlzaXRlZCA9IFtdO1xyXG4gICAgLy8gdmFyaW91cyBtZXJnZSBvcHRpb25zXHJcbiAgICB2YXIgb3B0aW9ucyA9IHt9O1xyXG4gICAgLy8gZ2V0cyB0aGUgc2VxdWVudGlhbCB0cmFpbGluZyBvYmplY3RzIGZyb20gYXJyYXkuXHJcbiAgICBmdW5jdGlvbiBnZXRTaGFkb3dPYmplY3RzKHNoYWRvd3MpIHtcclxuICAgICAgICB2YXIgb3V0ID0gc2hhZG93cy5yZWR1Y2UoZnVuY3Rpb24gKGNvbGxlY3Rvciwgc2hhZG93KSB7XHJcbiAgICAgICAgICAgICAgICBpZiAoc2hhZG93IGluc3RhbmNlb2YgT2JqZWN0KSB7XHJcbiAgICAgICAgICAgICAgICAgICAgY29sbGVjdG9yLnB1c2goc2hhZG93KTtcclxuICAgICAgICAgICAgICAgIH0gZWxzZSB7XHJcbiAgICAgICAgICAgICAgICAgICAgY29sbGVjdG9yID0gW107XHJcbiAgICAgICAgICAgICAgICB9XHJcbiAgICAgICAgICAgICAgICByZXR1cm4gY29sbGVjdG9yO1xyXG4gICAgICAgICAgICB9LCBbXSk7XHJcbiAgICAgICAgcmV0dXJuIG91dDtcclxuICAgIH1cclxuICAgIC8vIGdldHMgZWl0aGVyIGEgbmV3IG9iamVjdCBvZiB0aGUgcHJvcGVyIHR5cGUgb3IgdGhlIGxhc3QgcHJpbWl0aXZlIHZhbHVlXHJcbiAgICBmdW5jdGlvbiBnZXRPdXRwdXRPYmplY3Qoc2hhZG93cykge1xyXG4gICAgICAgIHZhciBvdXQ7XHJcbiAgICAgICAgdmFyIGxhc3RTaGFkb3cgPSBzaGFkb3dzW3NoYWRvd3MubGVuZ3RoIC0gMV07XHJcbiAgICAgICAgaWYgKGxhc3RTaGFkb3cgaW5zdGFuY2VvZiBBcnJheSkge1xyXG4gICAgICAgICAgICBvdXQgPSBbXTtcclxuICAgICAgICB9IGVsc2UgaWYgKGxhc3RTaGFkb3cgaW5zdGFuY2VvZiBGdW5jdGlvbikge1xyXG4gICAgICAgICAgICB0cnkge1xyXG4gICAgICAgICAgICAgICAgb3V0ID0gY2xvbmVGdW5jdGlvbihsYXN0U2hhZG93KTtcclxuICAgICAgICAgICAgfSBjYXRjaCAoZSkge1xyXG4gICAgICAgICAgICAgICAgdGhyb3cgbmV3IEVycm9yKGUubWVzc2FnZSk7XHJcbiAgICAgICAgICAgIH1cclxuICAgICAgICB9IGVsc2UgaWYgKGxhc3RTaGFkb3cgaW5zdGFuY2VvZiBPYmplY3QpIHtcclxuICAgICAgICAgICAgb3V0ID0ge307XHJcbiAgICAgICAgfSBlbHNlIHtcclxuICAgICAgICAgICAgLy8gbGFzdFNoYWRvdyBpcyBhIHByaW1pdGl2ZSB2YWx1ZTtcclxuICAgICAgICAgICAgb3V0ID0gbGFzdFNoYWRvdztcclxuICAgICAgICB9XHJcbiAgICAgICAgcmV0dXJuIG91dDtcclxuICAgIH1cclxuICAgIC8vIGNoZWNrcyBmb3IgY2lyY3VsYXIgcmVmZXJlbmNlc1xyXG4gICAgZnVuY3Rpb24gY2lyY3VsYXJSZWZlcmVuY2VDaGVjayhzaGFkb3dzKSB7XHJcbiAgICAgICAgLy8gaWYgYW55IG9mIHRoZSBjdXJyZW50IG9iamVjdHMgdG8gcHJvY2VzcyBleGlzdCBpbiB0aGUgcXVldWVcclxuICAgICAgICAvLyB0aGVuIHRocm93IGFuIGVycm9yLlxyXG4gICAgICAgIHNoYWRvd3MuZm9yRWFjaChmdW5jdGlvbiAoaXRlbSkge1xyXG4gICAgICAgICAgICBpZiAoaXRlbSBpbnN0YW5jZW9mIE9iamVjdCAmJiB2aXNpdGVkLmluZGV4T2YoaXRlbSkgPiAtMSkge1xyXG4gICAgICAgICAgICAgICAgdGhyb3cgbmV3IEVycm9yKCdDaXJjdWxhciByZWZlcmVuY2UgZXJyb3InKTtcclxuICAgICAgICAgICAgfVxyXG4gICAgICAgIH0pO1xyXG4gICAgICAgIC8vIGlmIG5vbmUgb2YgdGhlIGN1cnJlbnQgb2JqZWN0cyB3ZXJlIGluIHRoZSBxdWV1ZVxyXG4gICAgICAgIC8vIHRoZW4gYWRkIHJlZmVyZW5jZXMgdG8gdGhlIHF1ZXVlLlxyXG4gICAgICAgIHZpc2l0ZWQgPSB2aXNpdGVkLmNvbmNhdChzaGFkb3dzKTtcclxuICAgIH1cclxuICAgIGZ1bmN0aW9uIG9iamVjdE1lcmdlUmVjdXJzb3Ioc2hhZG93cywgY3VycmVudERlcHRoKSB7XHJcbiAgICAgICAgaWYgKG9wdGlvbnMuZGVwdGggIT09IGZhbHNlKSB7XHJcbiAgICAgICAgICAgIGN1cnJlbnREZXB0aCA9IGN1cnJlbnREZXB0aCA/IGN1cnJlbnREZXB0aCArIDEgOiAxO1xyXG4gICAgICAgIH0gZWxzZSB7XHJcbiAgICAgICAgICAgIGN1cnJlbnREZXB0aCA9IDA7XHJcbiAgICAgICAgfVxyXG4gICAgICAgIGlmIChvcHRpb25zLnRocm93T25DaXJjdWxhclJlZiA9PT0gdHJ1ZSkge1xyXG4gICAgICAgICAgICBjaXJjdWxhclJlZmVyZW5jZUNoZWNrKHNoYWRvd3MpO1xyXG4gICAgICAgIH1cclxuICAgICAgICB2YXIgb3V0ID0gZ2V0T3V0cHV0T2JqZWN0KHNoYWRvd3MpO1xyXG4gICAgICAgIC8qanNsaW50IHVucGFyYW06IHRydWUgKi9cclxuICAgICAgICBmdW5jdGlvbiBzaGFkb3dIYW5kbGVyKHZhbCwgcHJvcCwgc2hhZG93KSB7XHJcbiAgICAgICAgICAgIGlmIChvdXRbcHJvcF0pIHtcclxuICAgICAgICAgICAgICAgIG91dFtwcm9wXSA9IG9iamVjdE1lcmdlUmVjdXJzb3IoW1xyXG4gICAgICAgICAgICAgICAgICAgIG91dFtwcm9wXSxcclxuICAgICAgICAgICAgICAgICAgICBzaGFkb3dbcHJvcF1cclxuICAgICAgICAgICAgICAgIF0sIGN1cnJlbnREZXB0aCk7XHJcbiAgICAgICAgICAgIH0gZWxzZSB7XHJcbiAgICAgICAgICAgICAgICBvdXRbcHJvcF0gPSBvYmplY3RNZXJnZVJlY3Vyc29yKFtzaGFkb3dbcHJvcF1dLCBjdXJyZW50RGVwdGgpO1xyXG4gICAgICAgICAgICB9XHJcbiAgICAgICAgfVxyXG4gICAgICAgIC8qanNsaW50IHVucGFyYW06ZmFsc2UgKi9cclxuICAgICAgICBmdW5jdGlvbiBzaGFkb3dNZXJnZXIoc2hhZG93KSB7XHJcbiAgICAgICAgICAgIG9iamVjdEZvcmVhY2goc2hhZG93LCBzaGFkb3dIYW5kbGVyKTtcclxuICAgICAgICB9XHJcbiAgICAgICAgLy8gc2hvcnQgY2lyY3VpdHMgY2FzZSB3aGVyZSBvdXRwdXQgd291bGQgYmUgYSBwcmltaXRpdmUgdmFsdWVcclxuICAgICAgICAvLyBhbnl3YXkuXHJcbiAgICAgICAgaWYgKG91dCBpbnN0YW5jZW9mIE9iamVjdCAmJiBjdXJyZW50RGVwdGggPD0gb3B0aW9ucy5kZXB0aCkge1xyXG4gICAgICAgICAgICAvLyBvbmx5IG1lcmdlcyB0cmFpbGluZyBvYmplY3RzIHNpbmNlIHByaW1pdGl2ZXMgd291bGQgd2lwZSBvdXRcclxuICAgICAgICAgICAgLy8gcHJldmlvdXMgb2JqZWN0cywgYXMgaW4gbWVyZ2luZyB7YTonYSd9LCAnYScsIGFuZCB7YjonYid9XHJcbiAgICAgICAgICAgIC8vIHdvdWxkIHJlc3VsdCBpbiB7YjonYid9IHNvIHRoZSBmaXJzdCB0d28gYXJndW1lbnRzXHJcbiAgICAgICAgICAgIC8vIGNhbiBiZSBpZ25vcmVkIGNvbXBsZXRlbHkuXHJcbiAgICAgICAgICAgIHZhciByZWxldmFudFNoYWRvd3MgPSBnZXRTaGFkb3dPYmplY3RzKHNoYWRvd3MpO1xyXG4gICAgICAgICAgICByZWxldmFudFNoYWRvd3MuZm9yRWFjaChzaGFkb3dNZXJnZXIpO1xyXG4gICAgICAgIH1cclxuICAgICAgICByZXR1cm4gb3V0O1xyXG4gICAgfVxyXG4gICAgLy8gZGV0ZXJtaW5lcyB3aGV0aGVyIGFuIG9wdGlvbnMgb2JqZWN0IHdhcyBwYXNzZWQgaW4gYW5kXHJcbiAgICAvLyB1c2VzIGl0IGlmIHByZXNlbnRcclxuICAgIC8vIGlnbm9yZSB0aGUganNsaW50IHdhcm5pbmcgaGVyZSB0b28uXHJcbiAgICBpZiAoYXJndW1lbnRzWzBdIGluc3RhbmNlb2YgT2JqZWN0TWVyZ2VPcHRpb25zKSB7XHJcbiAgICAgICAgb3B0aW9ucyA9IGFyZ3VtZW50c1swXTtcclxuICAgICAgICBzaGFkb3dzID0gQXJyYXkucHJvdG90eXBlLnNsaWNlLmNhbGwoYXJndW1lbnRzLCAxKTtcclxuICAgIH0gZWxzZSB7XHJcbiAgICAgICAgb3B0aW9ucyA9IGNyZWF0ZU9wdGlvbnMoKTtcclxuICAgICAgICBzaGFkb3dzID0gQXJyYXkucHJvdG90eXBlLnNsaWNlLmNhbGwoYXJndW1lbnRzLCAwKTtcclxuICAgIH1cclxuICAgIHJldHVybiBvYmplY3RNZXJnZVJlY3Vyc29yKHNoYWRvd3MpO1xyXG59XHJcbm9iamVjdE1lcmdlLmNyZWF0ZU9wdGlvbnMgPSBjcmVhdGVPcHRpb25zO1xyXG5tb2R1bGUuZXhwb3J0cyA9IG9iamVjdE1lcmdlOyJdfQ==
