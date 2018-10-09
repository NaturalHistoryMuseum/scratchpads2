'use strict';

var path = require('path');
var util = require('util');
var isArray = util.isArray;

var umdify = require('libumd');
var extend = require('xtend');


module.exports = function(grunt) {
    grunt.registerMultiTask('umd', 'Surrounds code with the universal module definition.',
        function() {
            var file = grunt.file;
            var data = extend({}, this.data);
            var options = extend(data, this.options());

            try {
                verifyArguments(options);
            } catch (error) {
                grunt.warn(error, 3);
            }

            options = handleBackwardCompatibility(options);

            try {
                var inputFiles = file.expand(options.src);

                inputFiles.forEach(function(inputFile) {
                    var destination = options.dest || '.',
                        dest;

                    if(path.extname(destination) === '.js') {
                        dest = destination;
                    }
                    else {
                        dest = path.join(destination, inputFile);
                    }

                    file.write(dest, umdify(file.read(inputFile), options));
                });
            } catch (error) {
                grunt.warn(error, 3);
            }
        });
};

function verifyArguments(options) {
    if (!options.src) {
        throw new Error('Missing source file (src).');
    }
}

function handleBackwardCompatibility(options) {
    var dependency,
        dependencyType;

    for(dependencyType in options.deps) {
        if(dependencyType === 'args' || dependencyType === 'default') {
            continue;
        }

        dependency = options.deps[dependencyType];

        if(isArray(dependency)) {
            options.deps[dependencyType] = {
                items: dependency
            };
        }
    }

    return options;
}
