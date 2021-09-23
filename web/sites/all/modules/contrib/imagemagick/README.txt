
-- SUMMARY --

Provides ImageMagick integration.

For a full description of the module, visit the project page:
  http://drupal.org/project/imagemagick
To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/imagemagick


-- REQUIREMENTS --

* ImageMagick (http://www.imagemagick.org) needs to be installed on your server
  and the convert binary needs to be accessible and executable from PHP.

* The PHP configuration must allow invocation of proc_open() (which is
  security-wise identical to exec()).

Consult your server administrator or hosting provider if you are unsure about
these requirements.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Go to Administration » Configuration » Media » Image toolkit and change the
  image toolkit to ImageMagick.

* If the convert binary cannot be found in the default shell path, you need to
  enter the full path to ImageMagick's convert executable, including the
  filename itself.


-- CONTACT --

Current maintainers:
* Daniel F. Kudwien (sun) - http://drupal.org/user/54136

