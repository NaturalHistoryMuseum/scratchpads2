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
      async exec(fn, ...args) {
        // Accepting a function that we turn into a string means that the experience is a little nicer
        // for the lib user, but it is kind of magic and may end up breaking code if the user
        // passes a native function or something that makes use of closures.
        // Is it worth the trade-off? Only time will tell...
        const body = `return (${fn.toString()})(...arguments)`;
        return this.executeScript(body, args);
      },
      async withFrame(id, fn) {
        if (typeof id === 'string') {
          // If ID is a string, it's probably a css selector
          // We have to try and get the frame id of it
          // Do this by finding all the iframes in the page and find where the frame with the given selector falls
          // However this is a thing we have to do in the actual browser
          await this.find(id);
          id = await this.exec(selector =>
            Array.from(document.querySelectorAll('iframe')).indexOf(document.querySelector(selector)),
          id);
        }

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
