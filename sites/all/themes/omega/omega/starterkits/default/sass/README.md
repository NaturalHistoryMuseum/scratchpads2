# Introduction to [Sass](http://sass-lang.com/)
Sass makes CSS fun again. Sass is an extension of CSS3, adding nested rules,
variables, mixins, selector inheritance, and more. It’s translated to well-
formatted, standard CSS using the command line tool or a web-framework plugin.

Sass has two syntaxes. The new main syntax (as of Sass 3) is known as “SCSS”
(for “Sassy CSS”), and is a superset of CSS3’s syntax. This means that every
valid CSS3 stylesheet is valid SCSS as well. SCSS files use the extension .scss.

The second, older syntax is known as the indented syntax (or just “Sass”).
Inspired by Haml’s terseness, it’s intended for people who prefer conciseness
over similarity to CSS. Instead of brackets and semicolons, it uses the
indentation of lines to specify blocks. Although no longer the primary syntax,
the indented syntax will continue to be supported. Files in the indented syntax
use the extension .sass.

Please refer to the [Sass documentation](http://sass-lang.com/docs.html) for
further information about the syntax.

## Barebones Sass Structure
The barebones CSS structure provided in this starterkit uses many of the ideas
discussed in Jonathan [Snook's SMACSS](http://smacss.com) and is intended to
provide a starting point for building modular, scalable CSS using Sass and
Drupal.

Multiple Sass partials are used to help organise the styles, these are combined
by including them in styles.scss which is compiled into styles.css in the css/
directory.

All styles are included in order of specificity, this means that as you go down
the document each section builds upon and inherits sensibly from the previous
ones. This results in less undoing of styles, less specificity problems and
all-round better architected and lighter stylesheets.

The file and directory structure contained in this folder looks something like
this:

### Top level files
These files are the main entry points for the Sass compiler and shouldn't
directly contain any CSS code, instead they only serves to combine the Sass
contained in the partials (see below) through @import directives.

#### {{ THEME SANITIZED }}.styles.scss
This file aggregates all the components into a single file.

#### {{ THEME SANITIZED }}.reset.scss
This file provides a CSS reset.

### Partials
#### variables
This is where you place your Sass variables.

#### abstractions
This is where you place your functions, mixins and extends.

#### base
This is where you place all your basic, raw HTML element styling.

#### components
This is where you place your components.
