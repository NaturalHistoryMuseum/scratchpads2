'use strict';

module.exports = function(grunt) {
    grunt.initConfig({
        umd: {
            'default': {
                src: 'js/*.js',
                dest: 'output', // optional, the src files will be overwritten if omitted
                objectToExport: 'demo',
                globalAlias: 'demo',
                indent: 4
            }
        }
    });

    grunt.registerTask('default', ['umd']);
    grunt.registerTask('nodeps', ['umd:nodeps']);
    grunt.registerTask('noglobalalias', ['umd:noglobalalias']);
    grunt.registerTask('noobjecttoexport', ['umd:noobjecttoexport']);
    grunt.registerTask('onlydest', ['umd:onlydest']);
    grunt.registerTask('rails', ['umd:rails']);
    grunt.registerTask('returnExportsGlobal', ['umd:returnExportsGlobal']);

    grunt.loadTasks('../../tasks');
};
