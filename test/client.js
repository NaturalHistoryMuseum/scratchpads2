const Element = require('./element');
const WebDriver = require('webdriver');
const proxyPromise = require('proxymise');
const { xpath } = require('./selectors');

// A wrapper for wd client that returns `Element` instances instead of element ids
class Client {
  constructor(wdClient) {
    const client = Object.create(wdClient);
    console.dir(Client.prototype);
    return Object.assign(client, {
      find(strategy, selector, Constructor) {
        return Element.find(this, strategy, selector, Constructor);
      },
      async withFrame(id, fn) {
        await this.find(xpath(`//iframe[${id + 1}]`));
        await this.switchToFrame(id);

        try {
          return await fn(this);
        } finally {
          await this.switchToParentFrame();
        }
      },
      async click(strategy, selector = null) {
        if (selector === null) {
          selector = strategy;
          strategy = 'css selector';
        }

        return (await this.find(strategy, selector)).click();
      }
    });
  }
}

Client.run = async function run(config, fn) {
  const client = await WebDriver.newSession(config);
  const window = proxyPromise(Promise.resolve(new Client(client)));

  try {
    return await fn(window);
  } finally {
    await client.deleteSession();
  }
}

module.exports = Client;
