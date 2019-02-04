const { waitFor } = require('./utils');
const { cast } = require('./selectors');

class Element {
  constructor(client, id) {
    if(client instanceof Element) {
      id = client.id;
      client = client.client;
    }

    this.client = client;
    this.id = id;
  }

  static async find(client, selectorObject, Constructor = Element) {
    const { selector, strategy } = cast(selectorObject);
    const id = await waitFor(() => client.findElement(strategy, selector));

    return new Constructor(client, id);
  }

  sendKeys(keys) {
    return this.client.elementSendKeys(this.id, keys);
  }

  async find(selectorObject, Constructor = Element) {
    const { selector, strategy } = cast(selectorObject);

    const id = await waitFor(() => this.client.findElementFromElement(this.id, strategy, selector))
    return new Constructor(this.client, id);
  }

  async click(finder) {
    const element = finder ? await this.find(finder) : this;

    await this.client.elementClick(element.id);
    return element;
  }

  getAttribute(attribute) {
    return this.client.getElementAttribute(this.id, attribute);
  }

  waitUntilStale(timeout) {
    return waitFor(async () => {
      try {
        await this.client.getElementTagName(this.id);
        console.log('Element not stale');
        return false;
      } catch(e) {
        console.log('Element stale');
        return true;
      }
    }, timeout);
  }
}

module.exports = Element;
