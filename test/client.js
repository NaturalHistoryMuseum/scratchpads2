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
      frameStack: [],
      find(selector, Constructor) {
        console.log(selector);
        return Element.find(this, selector, Constructor);
      },
      async withFrame(id, fn) {
        //await this.find(xpath(`//iframe[${id + 1}]`));
        await this.switchToFrame(id);
        this.frameStack.push(id);

        try {
          return await fn(proxyPromise(this));
        } finally {
          // This is kind of broken in geckodriver (2019-01-23);
          // If the actual frame closes itself before we call this,
          // everything just hangs
          // await this.switchToParentFrame();

          // Here is the workaround:
          // Switch to topmost frame
          await this.switchToFrame(null);
          // Frames are tracked in frameStack; remove the last item
          this.frameStack.pop();
          // Descend down into the desired frame by iterating the frame stack and switching to each
          for(const id of this.frameStack) {
            await this.switchToFrame(id);
          }
        }
      },
      async click(selector) {
        return (await this.find(selector)).click();
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
