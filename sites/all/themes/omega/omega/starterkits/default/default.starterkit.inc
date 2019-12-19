name = Default
description = Slim bleeding edge starterkit. Uses LibSass instead of the Ruby Sass preprocessor.
screenshot = screenshot.png
engine = phptemplate
core = 7.x

; Styles
stylesheets[all][] = css/{{ THEME SANITIZED }}.reset.css
stylesheets[all][] = css/{{ THEME SANITIZED }}.styles.css

; Scripts
scripts[] = js/{{ THEME SANITIZED }}.behaviors.js
