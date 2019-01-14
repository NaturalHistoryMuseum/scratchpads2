// Element ID is always under this key, it's in the spec.
const ELEMENT = 'element-6066-11e4-a52e-4f735466cecf';

const wait = timeout => new Promise(resolve => setTimeout(resolve, timeout));

// Helper function for turning webdriver responses into
// either an error that gets thrown or the element ID.
const $ = msg => {
  if (msg.error) {
    const e = new Error(msg.message);
    Error.captureStackTrace(e, $);
    throw e;
  }
  if (msg[ELEMENT]) {
    return msg[ELEMENT];
  }
  return msg;
}

// There is theoretically a built in wait timout but I couldn't
// get it working so I hacked one
const waitFor = async (fn, timeout = 10000, interval = 1000) => {
  const start = Date.now();
  while (true) {
    try {
      const res = await fn();
      if (res) {
        return $(res);
      }
    } catch(e) {
      if (Date.now() + interval - start > timeout) {
        console.log('Throwing');
        throw e;
      }
    }

    if (Date.now() + interval - start > timeout) {
      console.log('Throwing');
      throw new Error('Couldn\'t wait');
    }

    console.log('Retrying')
    await wait(interval);
  }
}

module.exports = {
  wait, waitFor, $
};
