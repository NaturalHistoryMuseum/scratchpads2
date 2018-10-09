module.exports = requireNewlineAfterDescription;
module.exports.scopes = ['function'];
module.exports.options = {
    requireNewlineAfterDescription: {allowedValues: [true]}
};

var RE_NEWLINE_AT_THE_END = /\n$/;

/**
 * Requires newline after description in jsdoc comment
 *
 * @param {(FunctionDeclaration|FunctionExpression)} node
 * @param {Function} err
 */
function requireNewlineAfterDescription(node, err) {
    var doc = node.jsdoc;
    if (!doc || !doc.tags.length || !doc.description || !doc.description.length) {
        return;
    }

    if (!RE_NEWLINE_AT_THE_END.test(doc.description)) {
        err('Newline required after description', doc.description.length + 1);
    }
}
