const Client = require('../client');
const { css, link, partialLink } = require('../selectors');


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

Client.run(config, async (window) => {

  await require('./login')(window);
  // Go to dashboard
  await window.click('#toolbar-link-admin-content');
  await window.find(css('.overlay-element.overlay-active'));

  // Dashboard is inside an iframe
  await window.withFrame(1, async frame => {
    // Media gallery -> add
    await frame.find(css('.node-media_gallery')).click(link('Add')).waitUntilStale();
  });

  // Now media gallery is insdie a different iframe
  await window.withFrame(0, async frame => {
    // Fill in title field
    await frame.find(css('#edit-title')).sendKeys('title');

    // Open media browser
    await frame.click(css('#edit-field-media-und-add-more'));
    await frame.withFrame('#mediaBrowser', async mediaBrowser => {
      // Select file to upload
      await mediaBrowser.find(css('input[type=file]')).sendKeys(require.resolve('./150.png'));

      // Click submit and wait for the file to upload
      await mediaBrowser.click('#edit-submit').waitUntilStale();

      // Select the uploaded image (can't click the actual link for some reason
      // so click its first child)
      await mediaBrowser.find(partialLink('150.png')).click(css('*'));
      await mediaBrowser.click(link('Submit'));
    });

    // Wait for the processing to finish and submit
    await frame.find(css('.mediaBrowserLaunch-processed'));
    await frame.click('#edit-submit');
  });
});
