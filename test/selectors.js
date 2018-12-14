const css = selector => ({ selector, strategy: 'css selector' });
const xpath = selector => ({ selector, strategy: 'xpath' });
const link = selector => ({ selector, strategy: 'link text' });

module.exports = { css, xpath, link };
