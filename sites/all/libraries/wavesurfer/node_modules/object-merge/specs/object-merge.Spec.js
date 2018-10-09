var objectMerge = require('../src/object-merge.js');
var fs = require('fs');
var path = require('path');
var specPath = path.resolve(__dirname, '../browser/tests/object-merge.test.js');
var specCode = fs.readFileSync(specPath, "utf8");
eval(specCode);
