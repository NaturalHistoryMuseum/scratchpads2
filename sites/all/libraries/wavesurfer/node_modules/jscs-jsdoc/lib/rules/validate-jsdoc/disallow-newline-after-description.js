module.exports = disallowNewlineAfterDescription;
module.exports.scopes = ['function'];
module.exports.options = {
    disallowNewlineAfterDescription: {allowedValues: [true]}
};

var RE_NEWLINE_AT_THE_END = /\n$/;

/**
 * Disallows newline after description in jsdoc comment
 *
 * @param {(FunctionDeclaration|FunctionExpression)} node
 * @param {Function} err
 */
function disallowNewlineAfterDescription(node, err) {
    var doc = node.jsdoc;
    if (!doc || !doc.tags.length || !doc.description || !doc.description.length) {
        return;
    }

    if (!RE_NEWLINE_AT_THE_END.test(doc.description)) {
        return;
    }

    err('Newline required after description', doc.tags[0].loc.offset - 4);
}
