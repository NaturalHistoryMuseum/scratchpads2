'use strict';

var EventEmitter = require('events').EventEmitter;
var inherits = require('util').inherits;
var fs = require('fs');
var path = require('path');

var alphabet = require('alphabet').lower;
var handlebars = require('handlebars');
var objectMerge = require('object-merge');
var is = require('annois');
var zip = require('annozip');


var UMD = function UMD(code, options) {
    if(!code) {
        throw new Error('Missing code to convert!');
    }

    EventEmitter.call(this);
    this.code = code;
    this.options = options || {};

    this.template = this.loadTemplate(this.options.template);
};

inherits(UMD, EventEmitter);

UMD.prototype.loadTemplate = function loadTemplate(filepath) {
    var tplPath,
        exists = fs.existsSync;

    if (filepath) {
        if (exists(filepath)) {
            tplPath = filepath;
        }
        else {
            tplPath = path.join(__dirname, 'templates', filepath + '.hbs');

            if (!exists(tplPath)) {
                tplPath = path.join(__dirname, 'templates', filepath);

                if (!exists(tplPath)) {
                    this.emit('error', 'Cannot find template file "' + filepath + '".');
                    return;
                }
            }
        }
    }
    else {
        tplPath = path.join(__dirname, 'templates', 'umd.hbs');
    }

    try {
        return handlebars.compile(fs.readFileSync(tplPath, 'utf-8'));
    }
    catch (e) {
        this.emit('error', e.message);
    }
};

UMD.prototype.generate = function generate() {
    var options = this.options,
        code = this.code,
        ctx = objectMerge({}, options);

    var depsOptions = objectMerge(
        getDependencyDefaults(this.options.globalAlias),
        convertDependencyArrays(options.deps) || {}
    );

    var defaultDeps = depsOptions['default'].items;
    var deps = defaultDeps ? defaultDeps || defaultDeps.items || [] : [];
    var dependency, dependencyType, items, prefix, separator, suffix;

    for (dependencyType in depsOptions) {
        dependency = depsOptions[dependencyType];

        items = dependency.items || defaultDeps || [];

        // extract possible dependency names for objects
        items = items.map(function(item) {
            if(is.object(item)) {
                return Object.keys(item)[0];
            }

            return item;
        });

        prefix = dependency.prefix || '';
        separator = dependency.separator || ', ';
        suffix = dependency.suffix || '';

        ctx[dependencyType + 'Dependencies'] = {
            normal: items.map(function(item){ return 'root["' + item + '"]'; }),
            params: convertToAlphabet(items),
            wrapped: items.map(wrap(prefix, suffix)).join(separator),
        };
    }

    // supports ['dependency'] and [{dependency: 'functionParameter'}]
    ctx.dependencies = deps.map(function(dep) {
        if(is.string(dep)) {
            return dep;
        }

        if(is.object(dep)) {
            return dep[Object.keys(dep)[0]];
        }
    }).filter(id).join(', ');

    deps.join(', ');

    ctx.code = code;

    return this.template(ctx);
};

function convertToAlphabet(items) {
    return items.map(function(_, i) {
        return alphabet[i] + i;
    });
}

function wrap(pre, post) {
    pre = pre || '';
    post = post || '';

    return function (v) {
        return pre + v + post;
    };
}

function convertDependencyArrays(deps) {
    if(!deps) {
        return;
    }

    return zip.toObject(zip(deps).map(function(pair) {
        if(is.array(pair[1])) {
            return [pair[0], {
                items: pair[1]
            }];
        }

        return pair;
    }));
}

function getDependencyDefaults(globalAlias) {
    return {
        'default': {
            items: null,
        },
        amd: {
            items: null,
            prefix: '\"',
            separator: ',',
            suffix: '\"',
        },
        cjs: {
            items: null,
            prefix: 'require(\"',
            separator: ',',
            suffix: '\")',
        },
        global: {
            items: null,
            prefix: globalAlias? globalAlias + '.': '\"',
            separator: ',',
            suffix: '\"',
        }
    };
}

module.exports = function(code, options) {
    var u = new UMD(code, options);

    return u.generate();
};

function id(a) {return a;}
