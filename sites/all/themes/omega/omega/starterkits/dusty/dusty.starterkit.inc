name = Dusty
description = Rather outdated starterkit leveraging the Ruby Sass preprocessor. Comes with a well organized Sass setup with heavy use of partials.
screenshot = screenshot.png
engine = phptemplate
core = 7.x

; Styles
stylesheets[all][] = css/{{ THEME SANITIZED }}.normalize.css
stylesheets[all][] = css/{{ THEME SANITIZED }}.hacks.css
stylesheets[all][] = css/{{ THEME SANITIZED }}.styles.css
stylesheets[all][] = css/{{ THEME SANITIZED }}.no-query.css

; Scripts
scripts[] = js/{{ THEME SANITIZED }}.behaviors.js
