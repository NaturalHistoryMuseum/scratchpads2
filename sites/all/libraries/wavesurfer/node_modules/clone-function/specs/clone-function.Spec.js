var cloneFunction = require('../src/clone-function.js');
var fs = require('fs');
var path = require('path');
var specPath = path.resolve(__dirname, '../browser/tests/clone-function.test.js');
var specCode = fs.readFileSync(specPath, "utf8");
eval(specCode);
