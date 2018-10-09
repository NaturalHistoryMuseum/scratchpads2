var objectForeach = require('../src/object-foreach.js');
var fs = require('fs');
var path = require('path');
var specPath = path.resolve(__dirname, '../browser/tests/object-foreach.test.js');
var specCode = fs.readFileSync(specPath, "utf8");
eval(specCode);
