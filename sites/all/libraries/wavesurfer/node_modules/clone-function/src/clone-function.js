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