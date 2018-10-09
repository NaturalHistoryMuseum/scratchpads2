module.exports = function(grunt) {
	grunt.initConfig({
		meta: {
			package: grunt.file.readJSON('package.json'),
			src: {
				main: 'src/main',
				test: 'src/test',
			},
			bin: {
				coverage: 'bin/coverage'
			},
			temp: {
				integration: '.grunt/integration',
				outfile: '.grunt/outfile'
			},
			doc: 'doc'
		},
		// test template functionality
		nodeunit: {
			template: '<%= meta.src.test %>/js/template.js',
			reporter: '<%= meta.src.test %>/js/reporter.js',
			integration: '<%= meta.src.test %>/js/integration.js',
			outfile: '<%= meta.src.test %>/js/outfile.js',
			threshold: '<%= meta.src.test %>/js/threshold.js'
		},
		jasmine: {
			// test common use-case
			integration: {
				src: ['<%= meta.src.test %>/js/Generator.js'],
				options: {
					specs: ['<%= meta.src.test %>/js/GeneratorTest.js'],
					template: require('./'),
					templateOptions: {
						coverage: '<%= meta.temp.integration %>/coverage.json',
						report: [
							{
								type: 'html',
								options: {
									dir: '<%= meta.temp.integration %>/html'
								}
							},
							{
								type: 'cobertura',
								options: {
									dir: '<%= meta.temp.integration %>/cobertura'
								}
							},
							{
								type: 'text-summary'
							}
						],
						thresholds: {
							lines: 100,
							statements: 100,
							branches: 100,
							functions: 100
						},
						template: '<%= meta.src.test %>/html/integration.tmpl',
						templateOptions: {
							helpers: ['<%= meta.src.test %>/js/integration-helper.js']
						}
					}
				}
			},
			// test that coverage is still collected when outfile is specified, see #33
			outfile: {
				src: ['<%= meta.src.test %>/js/Generator.js'],
				options: {
					specs: ['<%= meta.src.test %>/js/GeneratorTest.js'],
					outfile: '.grunt/runner.html',
					template: require('./'),
					templateOptions: {
						coverage: '<%= meta.temp.outfile %>/coverage.json',
						report: {
							type: 'text-summary'
						},
						template: '<%= meta.src.test %>/html/integration.tmpl',
						templateOptions: {
							helpers: ['<%= meta.src.test %>/js/integration-helper.js']
						}
					}
				}
			},
			// test that threshold can fail the build
			threshold: {
				src: ['<%= meta.src.test %>/js/Generator.js'],
				options: {
					specs: ['<%= meta.src.test %>/js/GeneratorTest.js'],
					template: require('./'),
					templateOptions: {
						coverage: '<%= meta.temp.integration %>/coverage.json',
						report: {
							type: 'text-summary'
						},
						thresholds: {
							lines: 101
						},
						template: '<%= meta.src.test %>/html/integration.tmpl',
						templateOptions: {
							helpers: ['<%= meta.src.test %>/js/integration-helper.js']
						}
					}
				}
			}
		},
		clean: {
			temp: ['.grunt'],
			bin: ['bin']
		},
		yuidoc: {
			compile: {
				name: '<%= meta.package.name %>',
				description: '<%= meta.package.description %>',
				version: '<%= meta.package.version %>',
				options: {
					paths: '<%= meta.src.main %>',
					outdir: '<%= meta.doc %>'
				}
			}
		},
		eslint: {
			target: ['<%= meta.src.main %>/js/*.js', '<%= meta.src.test %>/js/*.js'],
			options: {
			}
		}
	});

	grunt.loadNpmTasks('grunt-contrib-clean');
	grunt.loadNpmTasks('grunt-contrib-jasmine');
	grunt.loadNpmTasks('grunt-contrib-nodeunit');
	grunt.loadNpmTasks('grunt-contrib-yuidoc');
	grunt.loadNpmTasks('gruntify-eslint');

	grunt.registerTask('report', 'Write coverage report', function () {
		var istanbul = require('istanbul');
		var collector = new istanbul.Collector();
		var reporter = istanbul.Report.create('html', {
			dir: grunt.config.process('<%= meta.bin.coverage %>')
		});
		grunt.file.expand(grunt.config.process('<%= meta.bin.coverage %>/coverage-*.json')).forEach(function (file) {
			collector.add(grunt.file.readJSON(file));
		});
		reporter.writeReport(collector, true);
	});

	var WARN = grunt.warn;

	grunt.registerTask('mock:warn:install', 'Install mock for grunt.warn()', function () {
		grunt.warn = function(message) {
			grunt.warn.message = message;
		};
	});
	grunt.registerTask('mock:warn:uninstall', 'Uninstall mock for grunt.warn()', function () {
		grunt.warn = WARN;
	});

	grunt.registerTask('check', ['eslint']);
	grunt.registerTask('doc', 'yuidoc');
	grunt.registerTask('test:template', ['nodeunit:template']);
	grunt.registerTask('test:reporter', ['nodeunit:reporter']);
	grunt.registerTask('test:integration', ['clean:temp', 'jasmine:integration', 'nodeunit:integration']);
	grunt.registerTask('test:outfile', ['clean:temp', 'jasmine:outfile', 'nodeunit:outfile']);
	grunt.registerTask('test:threshold', ['clean:temp', 'mock:warn:install', 'jasmine:threshold', 'nodeunit:threshold', 'mock:warn:uninstall']);
	grunt.registerTask('test', ['test:template', 'test:reporter', 'test:integration', 'test:outfile', 'test:threshold']);
	grunt.registerTask('test:coverage', ['clean:bin', 'test', 'report']);

};
