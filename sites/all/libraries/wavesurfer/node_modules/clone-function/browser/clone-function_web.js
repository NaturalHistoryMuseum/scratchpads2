(function e(t,n,r){function s(o,u){if(!n[o]){if(!t[o]){var a=typeof require=="function"&&require;if(!u&&a)return a(o,!0);if(i)return i(o,!0);throw new Error("Cannot find module '"+o+"'")}var f=n[o]={exports:{}};t[o][0].call(f.exports,function(e){var n=t[o][1][e];return s(n?n:e)},f,f.exports,e,t,n,r)}return n[o].exports}var i=typeof require=="function"&&require;for(var o=0;o<r.length;o++)s(r[o]);return s})({1:[function(require,module,exports){
cloneFunction = require('../src/clone-function.js');

},{"../src/clone-function.js":2}],2:[function(require,module,exports){
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
},{}]},{},[1])
//@ sourceMappingURL=data:application/json;base64,eyJ2ZXJzaW9uIjozLCJmaWxlIjoiZ2VuZXJhdGVkLmpzIiwic291cmNlcyI6WyJDOi9Vc2Vycy9rYXN0b3IvRG9jdW1lbnRzL0dpdEh1Yi9jbG9uZS1mdW5jdGlvbi9kZXYvYnJvd3Nlck1haW4uanMiLCJDOi9Vc2Vycy9rYXN0b3IvRG9jdW1lbnRzL0dpdEh1Yi9jbG9uZS1mdW5jdGlvbi9zcmMvY2xvbmUtZnVuY3Rpb24uanMiXSwibmFtZXMiOltdLCJtYXBwaW5ncyI6IjtBQUFBO0FBQ0E7O0FDREE7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBIiwic291cmNlc0NvbnRlbnQiOlsiY2xvbmVGdW5jdGlvbiA9IHJlcXVpcmUoJy4uL3NyYy9jbG9uZS1mdW5jdGlvbi5qcycpO1xyXG4iLCIvKlxyXG5MaWNlbnNlIGdwbC0zLjAgaHR0cDovL3d3dy5nbnUub3JnL2xpY2Vuc2VzL2dwbC0zLjAtc3RhbmRhbG9uZS5odG1sXHJcbiovXHJcbi8qanNsaW50XHJcbiAgICBldmlsOiB0cnVlLFxyXG4gICAgbm9kZTogdHJ1ZVxyXG4qL1xyXG4ndXNlIHN0cmljdCc7XHJcbi8qKlxyXG4gKiBDbG9uZXMgbm9uIG5hdGl2ZSBKYXZhU2NyaXB0IGZ1bmN0aW9ucywgb3IgcmVmZXJlbmNlcyBuYXRpdmUgZnVuY3Rpb25zLlxyXG4gKiBAYXV0aG9yIDxhIGhyZWY9XCJtYWlsdG86bWF0dGhld2thc3RvckBnbWFpbC5jb21cIj5NYXR0aGV3IEthc3RvcjwvYT5cclxuICogQHBhcmFtIHtGdW5jdGlvbn0gZnVuYyBUaGUgZnVuY3Rpb24gdG8gY2xvbmUuXHJcbiAqIEByZXR1cm5zIHtGdW5jdGlvbn0gUmV0dXJucyBhIGNsb25lIG9mIHRoZSBub24gbmF0aXZlIGZ1bmN0aW9uLCBvciBhXHJcbiAqICByZWZlcmVuY2UgdG8gdGhlIG5hdGl2ZSBmdW5jdGlvbi5cclxuICovXHJcbmZ1bmN0aW9uIGNsb25lRnVuY3Rpb24oZnVuYykge1xyXG4gICAgdmFyIG91dCwgc3RyO1xyXG4gICAgdHJ5IHtcclxuICAgICAgICBzdHIgPSBmdW5jLnRvU3RyaW5nKCk7XHJcbiAgICAgICAgaWYgKC9cXFtuYXRpdmUgY29kZVxcXS8udGVzdChzdHIpKSB7XHJcbiAgICAgICAgICAgIG91dCA9IGZ1bmM7XHJcbiAgICAgICAgfSBlbHNlIHtcclxuICAgICAgICAgICAgb3V0ID0gZXZhbCgnKGZ1bmN0aW9uKCl7cmV0dXJuICcgKyBzdHIgKyAnfSgpKTsnKTtcclxuICAgICAgICB9XHJcbiAgICB9IGNhdGNoIChlKSB7XHJcbiAgICAgICAgdGhyb3cgbmV3IEVycm9yKGUubWVzc2FnZSArICdcXHJcXG5cXHJcXG4nICsgc3RyKTtcclxuICAgIH1cclxuICAgIHJldHVybiBvdXQ7XHJcbn1cclxubW9kdWxlLmV4cG9ydHMgPSBjbG9uZUZ1bmN0aW9uOyJdfQ==
