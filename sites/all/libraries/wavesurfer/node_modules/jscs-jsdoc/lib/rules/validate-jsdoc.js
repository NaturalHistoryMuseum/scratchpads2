var assert = require('assert');

var jsdoc = require('../jsdoc');
var getLineCountBetween = require('../utils').getLineCountBetween;
var getNodeColumn = require('../utils').getNodeColumn;
var esprimaHelpers = require('../esprima-helpers');
var validators = require('./validate-jsdoc/index');

/** @type {string[]} - list of function node types */
var functionNodeTypes = [
    'FunctionDeclaration',
    'FunctionExpression',
    'ArrowFunctionExpression',
];

/** @type {string[]} - list of node types with jsdocs */
var jsdocableTypes = []
    .concat(functionNodeTypes);

/**
 * Rule constructor
 *
 * @this {module:jscs/Rule}
 * @constructor
 */
module.exports = function() {};

module.exports.prototype = {

    /**
     * Load all rules and init them
     *
     * @param {Object} options
     * @throws {Error} If options is not an Object
     */
    configure: function(options) {
        assert(typeof options === 'object', 'jsDoc option requires object value');

        // rules structured by scopes-tags for jsdoc-tags
        var rulesForTags = this._rulesForTags = {};
        // rules structured by scopes for nodes
        var rulesForNodes = this._rulesForNodes = {};

        this._options = options;
        this._optionsList = Object.keys(options);

        // load validators
        this._validators = validators.load(this._optionsList);
        assert(this._validators.length, 'jsDoc plugin was not configured properly');

        // registering validators
        this._validators.forEach(function(v) {
            // check options
            if (v.options) {
                validators.checkOptions(v, options);
            }
            // configure
            if (v.configure) {
                v.configure.call(this, options);
            }
            // index rules by tags and scopes
            (v.scopes || ['']).forEach(function(scope) {
                if (!v.tags) {
                    assert(v.length === 2, 'jsDoc rules: Wrong arity in ' + v._name + ' validator');
                    rulesForNodes[scope] = rulesForNodes[scope] || [];
                    rulesForNodes[scope].push(v);
                    return;
                }
                assert(v.length === 3, 'jsDoc rules: Wrong arity in ' + v._name + ' validator');
                rulesForTags[scope] = rulesForTags[scope] || {};
                v.tags.forEach(function(tag) {
                    var dtag = '@' + tag;
                    rulesForTags[scope][dtag] = rulesForTags[scope][dtag] || [];
                    rulesForTags[scope][dtag].push(v);
                });
            });
        }, this);
    },

    /**
     * @returns {string}
     */
    getOptionName: function() {
        return 'jsDoc';
    },

    /**
     * @param {module:jscs/JsFile} file
     * @param {module:jscs/Errors} errors
     */
    check: function(file, errors) {
        patchNodesInFile(file);
        this._iterate = file.iterate;

        var _this = this;
        var scopes = {
            'function': functionNodeTypes,
        };

        // classic checker
        if (_this._rulesForNodes.file) {
            // call file checkers
            var validators = _this._rulesForNodes.file;
            if (validators) {
                validators.forEach(function(v) {
                    v.call(_this, file, errors);
                });
            }
        }

        // iterate over scopes
        Object.keys(scopes).forEach(function(scope) {

            // skip unused
            if (!_this._rulesForNodes[scope] && !_this._rulesForTags[scope]) {
                return;
            }

            // traverse ast tree and search scope node types
            file.iterateNodesByType(scopes[scope], function(node) {
                // init
                var validators;

                // call node checkers
                validators = _this._rulesForNodes[scope];
                if (validators) {
                    validators.forEach(function(v) {
                        v.call(_this, node, addError);
                    });
                }

                validators = _this._rulesForTags[scope];
                if (!validators || !node.jsdoc || !node.jsdoc.valid) {
                    return;
                }

                // call rule checkers
                node.jsdoc.iterate(function(tag) {
                    if (!validators['@' + tag.id]) {
                        return;
                    }
                    // call tag validator
                    validators['@' + tag.id].forEach(function(v) {
                        v.call(_this, node, tag, fixErrLocation(addError, tag));
                    });
                });

                /**
                 * Send error to jscs
                 *
                 * @param {string} text
                 * @param {number|DocLocation} offset
                 */
                function addError(text, offset) {
                    if (typeof offset === 'object') {
                        if (offset.type) {
                            errors.add(text, offset, 0);
                            return;
                        } else {
                            offset = offset.offset;
                        }
                    }
                    errors.add(text, node.jsdoc.node, offset);
                }

                /**
                 * Generates function with location fixing logic to send error to jscs
                 *
                 * @param {function(string, number|Object)} err
                 * @param {DocTag} tag
                 * @returns {function(string, number|Object)}
                 */
                function fixErrLocation(err, tag) {
                    return function(text, offset) {
                        err(text, offset || tag.loc.offset);
                    };
                }
            });

        });
    },

    /**
     * Caching scope search. todo: move to patchNodesInFile
     *
     * @param {Object} node
     */
    _getReturnStatementsForNode: function(node) {
        if (node.jsdoc.returnStatements) {
            return node.jsdoc.returnStatements;
        }

        var statements = [];
        this._iterate(function(n) {
            if (n && n.type === 'ReturnStatement' && n.argument) {
                if (node === esprimaHelpers.closestScopeNode(n)) {
                    statements.push(n.argument);
                }
            }
        }, node);

        node.jsdoc.returnStatements = statements;
        return statements;
    }
};

/**
 * Extends each node with helper properties
 *
 * @param {Object} file
 */
function patchNodesInFile(file) {
    if (file._jsdocs) {
        return;
    }

    // jsdoc property for nodes
    file.iterateNodesByType(jsdocableTypes, function(node) {
        Object.defineProperty(node, 'jsdoc', {
            get: getJsdoc
        });
    });

    /**
     * Fetchs jsdoc block for this
     *
     * @this {module:esprima/Node}
     * @returns {DocComment}
     */
    function getJsdoc() {
        if (this.hasOwnProperty('_jsdoc')) {
            return this._jsdoc;
        }

        var node = functionNodeTypes.indexOf(this.type) !== -1 ?
            findFirstNodeInLine(this) : this;
        var res = findDocCommentBeforeNode(node, this);

        this._jsdoc = res ? jsdoc.createDocCommentByCommentNode(res) : null;

        return this._jsdoc;
    }

    /**
     * Finds the first node on the same line as passed node
     *
     * @param {?module:esprima/Node} node
     * @returns {?module:esprima/Node}
     */
    function findFirstNodeInLine(node) {
        var parent = node.parentElement;
        if (!parent || getLineCountBetween(parent.getFirstToken(), node.getFirstToken()) > 0) {
            return node;
        }
        return findFirstNodeInLine(parent);
    }

    /**
     * Finds DocComment in file before passed line number
     *
     * @param {?module:esprima/Node} node
     * @returns {?module:esprima/Node}
     */
    function findDocCommentBeforeNode(node, self) {
        var res = node.getFirstToken().getPreviousNonWhitespaceToken();
        if (!res || res.type !== 'CommentBlock' || res.value.charAt(0) !== '*') {
            return null;
        }

        // Indent should be the same
        if (getNodeColumn(res) !== getNodeColumn(node)) {
            return null;
        }

        // IIFE should be on the next line to be sticked
        if ((self.type === 'FunctionExpression' || self.type === 'ArrowFunctionExpression') &&
            self.parentElement.type === 'CallExpression' &&
            getLineCountBetween(res, self.getFirstToken()) > 1
        ) {
            return null;
        }

        return res;
    }

    // mark object as patched
    file._jsdocs = true;
}
