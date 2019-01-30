const css = selector => ({ selector, strategy: 'css selector' });
const xpath = selector => ({ selector, strategy: 'xpath' });
const link = selector => ({ selector, strategy: 'link text' });
const partialLink = selector => ({ selector, strategy: 'partial link text' });
const cast = selector => typeof selector === 'string' ? css(selector) : selector;

module.exports = { css, xpath, link, partialLink, cast };
