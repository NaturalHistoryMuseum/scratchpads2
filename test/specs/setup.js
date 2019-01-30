const Client = require('../client');
const Checkbox = require('../checkbox');
const { css } = require('../selectors');

const withEditor = (window, id, fn) =>
  window.withFrame(id, async () => {
    const body = await window.find(css('body'));

    return fn(body);
  });

const completeStep = (window, fn) =>
  window.withFrame('.overlay-element.overlay-active', async frame => {
    const rtn = fn && await fn(frame);
    const el = await frame.click('#edit-next');
    await el.waitUntilStale(30000);
    return rtn;
  })

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

const login = require('./login');

Client.run(config, async (window) => {
  await login(window);

  // Go to setup
  await window.navigateTo(login.SCRATCHPAD_URL + '/#overlay=setup/0');

  // Page 1
  await completeStep(window);

  // Page 2 - welcome message
  await completeStep(window, frame =>
    withEditor(frame, '.cke_contents iframe',
      body => body.sendKeys('Welcome message')
    )
  );

  // Page 3 - about page
  await completeStep(window, frame =>
    withEditor(frame, 0,
      body => body.sendKeys('About message')
    )
  );

  // Page 4 - License
  await completeStep(window);

  // Page 5 - Category
  await completeStep(window, async frame => {
    const selectors = [
      '#edit-scope-focus-citizen-science',
      '#edit-ecoregion-marine',
      '#edit-geography-globalno-geographic-restriction',
      '#edit-research-domain-biogeography',
      '#edit-taxonomic-area-algaefungi',
    ];

    for (const selector of selectors) {
      await (await frame.find(css(selector), Checkbox)).check(true);
    }
  });

  // Page 6 - Tools
  await window.withFrame(1, async () => {
    // This step takes ages for some reason
    await window.click('#edit-return').waitUntilStale(60000);
  });

  // Wait for success messages
  await window.find(css('.messages.status'));
});

