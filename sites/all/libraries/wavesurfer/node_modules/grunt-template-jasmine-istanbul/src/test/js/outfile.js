/**
 * Tests if the files have been created correctly after running the jasmine
 * integration test.
 */

var grunt = require('grunt');

exports.outfile = {
	'shouldWriteCoverage': function (test) {
		var file = grunt.config.get(
				'jasmine.outfile.options.templateOptions.coverage');
		test.ok(grunt.file.exists(file), 'should write coverage.json');
		test.done();
	}
};
