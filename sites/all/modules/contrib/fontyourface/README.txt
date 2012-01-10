
Welcome to @font-your-face.

Installing @font-your-face:
---------------------------

- Place the entirety of this directory in sites/all/modules/fontyourface
- Navigate to administer >> build >> modules. Enable @font-your-face and one or more of the submodules (font providers) in the group.

Using @font-your-face:
----------------------

- Navigate to administer >> build >> themes >> @font-your-face.
- Click the "Add a new font" link.
- Click the name of a font.
- Enter a CSS selector for the content you want to use the font (or leave it as "body" to use it everywhere)
- Click "Add font"

Known issues:
-------------

Note that Internet Explorer has a limit of 32 CSS files, so using @font-your-face on CSS-heavy sites may require turning on CSS aggregation under administer >> site configuration >> performance.

KERNEST servers are sometimes unreliable, so you may want to download KERNEST fonts and use the Local Fonts module to load them from your server instead. KERNEST provides paid fonts that are not available for use in the API, so those can only be used with the Local Fonts module.

See http://drupal.org/project/fontyourface#support for support options on any issues not mentioned here.