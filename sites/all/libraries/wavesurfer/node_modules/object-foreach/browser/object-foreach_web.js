;(function e(t,n,r){function s(o,u){if(!n[o]){if(!t[o]){var a=typeof require=="function"&&require;if(!u&&a)return a(o,!0);if(i)return i(o,!0);throw new Error("Cannot find module '"+o+"'")}var f=n[o]={exports:{}};t[o][0].call(f.exports,function(e){var n=t[o][1][e];return s(n?n:e)},f,f.exports,e,t,n,r)}return n[o].exports}var i=typeof require=="function"&&require;for(var o=0;o<r.length;o++)s(r[o]);return s})({1:[function(require,module,exports){
objectForeach = require('../src/object-foreach.js');

},{"../src/object-foreach.js":2}],2:[function(require,module,exports){
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
},{}]},{},[1])
//@ sourceMappingURL=data:application/json;base64,eyJ2ZXJzaW9uIjozLCJmaWxlIjoiZ2VuZXJhdGVkLmpzIiwic291cmNlcyI6WyJDOlxcVXNlcnNcXGthc3RvclxcRGVza3RvcFxcZXhwZXJpbWVudHNcXG9iamVjdC1mb3JlYWNoXFxkZXZcXGJyb3dzZXJNYWluLmpzIiwiQzpcXFVzZXJzXFxrYXN0b3JcXERlc2t0b3BcXGV4cGVyaW1lbnRzXFxvYmplY3QtZm9yZWFjaFxcc3JjXFxvYmplY3QtZm9yZWFjaC5qcyJdLCJuYW1lcyI6W10sIm1hcHBpbmdzIjoiO0FBQUE7QUFDQTs7QUNEQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0E7QUFDQTtBQUNBO0FBQ0EiLCJzb3VyY2VzQ29udGVudCI6WyJvYmplY3RGb3JlYWNoID0gcmVxdWlyZSgnLi4vc3JjL29iamVjdC1mb3JlYWNoLmpzJyk7XHJcbiIsIi8qKlxyXG4gKiBFeGVjdXRlcyBhIGZ1bmN0aW9uIG9uIGVhY2ggb2YgYW4gb2JqZWN0cyBvd24gZW51bWVyYWJsZSBwcm9wZXJ0aWVzLiBUaGVcclxuICogIGNhbGxiYWNrIGZ1bmN0aW9uIHdpbGwgcmVjZWl2ZSB0aHJlZSBhcmd1bWVudHM6IHRoZSB2YWx1ZSBvZiB0aGUgY3VycmVudFxyXG4gKiAgcHJvcGVydHksIHRoZSBuYW1lIG9mIHRoZSBwcm9wZXJ0eSwgYW5kIHRoZSBvYmplY3QgYmVpbmcgcHJvY2Vzc2VkLiBUaGlzIGlzXHJcbiAqICByb3VnaGx5IGVxdWl2YWxlbnQgdG8gdGhlIHNpZ25hdHVyZSBmb3IgY2FsbGJhY2tzIHRvXHJcbiAqICBBcnJheS5wcm90b3R5cGUuZm9yRWFjaC5cclxuICogQHBhcmFtIHtPYmplY3R9IG9iaiBUaGUgb2JqZWN0IHRvIGFjdCBvbi5cclxuICogQHBhcmFtIHtGdW5jdGlvbn0gY2FsbGJhY2sgVGhlIGZ1bmN0aW9uIHRvIGV4ZWN1dGUuXHJcbiAqIEByZXR1cm5zIHtPYmplY3R9IFJldHVybnMgdGhlIGdpdmVuIG9iamVjdC5cclxuICovXHJcbmZ1bmN0aW9uIG9iamVjdEZvcmVhY2gob2JqLCBjYWxsYmFjaykge1xyXG4gICAgXCJ1c2Ugc3RyaWN0XCI7XHJcbiAgICBPYmplY3Qua2V5cyhvYmopLmZvckVhY2goZnVuY3Rpb24gKHByb3ApIHtcclxuICAgICAgICBjYWxsYmFjayhvYmpbcHJvcF0sIHByb3AsIG9iaik7XHJcbiAgICB9KTtcclxuICAgIHJldHVybiBvYmo7XHJcbn07XHJcbm1vZHVsZS5leHBvcnRzID0gb2JqZWN0Rm9yZWFjaDsiXX0=
;