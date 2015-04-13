
Ajax Blocks module


DESCRIPTION

This module gives site administrator the ability to choose which blocks are to be loaded by additional AJAX request
after loading the whole cached page itself. The module is suitable for sites which are mostly static,
and the page caching for anonymous users is a great benefit, but there are some pieces of information that have to be dynamic.

The classical example is Ubercart sites which has to show the cart content in a block.
The other example is showing random pictures or links to nodes (from some view output which randomizes data).

The module is a fork of existing module: http://drupal.org/project/ajaxify_regions

Ajaxified blocks are replaced by placeholders with the original block content wrapped by <noscript> tag
(including the original block contents is optional) during page preprocessing,
and the resulting page can be cached. After page loading, the browser initiates one AJAX request which contains the information
needed to evaluate contents of these blocks on the server side (the information includes block IDs and current page path).
AJAX handler gets the actual (non-cached) block contents for this page and returns the result as JSON.
This process can require less resources than generating the whole page, and the page loading time can be significantly decreased
comparing to the case of disabling this page caching. Search engine bots (and even DoS attacks) will produce less load to the server.
So, blocks with dynamic content (cart, random banners, list of logged in users, etc.) can be viewed on the cached pages.


INSTALLATION

1. Install this module as usual.
2. For every block that has to be loaded by AJAX request, go to this block configuration page
   and turn on 'Load block via AJAX' option in 'AJAX settings' fieldset.
3. Visit the site as anonymous user and check that new request is performed after loading the page.
4. If you want to reduce page flickering, you can set (in css) the appropriate width and height of block content placeholders.


LIMITATIONS

1. Block title must be static string.
2. Block content must not be changed by theme preprocess functions and theme files.


TODO

1. Check page access permissions when loading blocks for that page.
2. Support additional css and js file loading.
3. Support operation mode which doesn't require full Drupal bootstrap.
