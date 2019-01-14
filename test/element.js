const { waitFor } = require('./utils');

class Element {
  constructor(client, id) {
    if(client instanceof Element) {
      id = client.id;
      client = client.client;
    }

    this.client = client;
    this.id = id;
  }

  static async find(client, selType, selector, Constructor = Element) {
    if(typeof selType === 'object') {
      Constructor = selector || Constructor;
      selector = selType.selector;
      selType = selType.strategy;
    }
    const id = await waitFor(() => client.findElement(selType, selector));

    return new Constructor(client, id);
  }

  sendKeys(keys) {
    return this.client.elementSendKeys(this.id, keys);
  }

  async click() {
    await this.client.elementClick(this.id);
    return this;
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
