const Client = require('../client');
const { css, link } = require('../selectors');

const argv = require('minimist')(process.argv.slice(2));

function getArg(name) {
  if(argv[name]) {
    return argv[name];
  }
  const e = new Error(`--${name} cli arg required`);
  Error.captureStackTrace(e, getArg);
  throw e;
}

const SCRATCHPAD_URL = getArg('url');
const USERNAME = getArg('username');
const PASSWORD = getArg('password');

const config = {
  path: '/',
  capabilities: {
    browserName: 'firefox',
    timeouts: {
      implicit: 5000
    }
  },
  logLevel: 'debug'
}

module.exports = async (window) => {
  await window.navigateTo(SCRATCHPAD_URL);

  await window.find(link('Log in')).click();

  // FIll login form
  await window.find(css('#edit-name')).sendKeys(USERNAME);
  await window.find(css('#edit-pass')).sendKeys(PASSWORD);
  await window.find(css('#edit-submit')).click();

  // Wait for hello
  await window.find(link('Hello ' + USERNAME));
};

module.exports.SCRATCHPAD_URL = SCRATCHPAD_URL;

if(require.main === module) {
  Client.run(config, module.exports);
}
