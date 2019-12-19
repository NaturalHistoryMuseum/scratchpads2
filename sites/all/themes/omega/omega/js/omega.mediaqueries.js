(function ($, window, Drupal) {

  'use strict';

  /**
   * Toggles media-query specific body classes.
   *
   * You can define new media queries to listen to by writing them into the
   * Drupal.settings.omegaSettings.mediaQueries array.
   */
  Drupal.behaviors.omegaMediaQueryClasses = {
    handler: function (name, mql) {
      if (mql.matches) {
        $('body').removeClass(name + '-inactive').addClass(name + '-active');
      }
      else {
        $('body').removeClass(name + '-active').addClass(name + '-inactive');
      }
    },

    attach: function (context, settings) {
      var behavior = this;
      var omegaSettings = settings.omega || {};
      var mediaQueries = omegaSettings.mediaQueries || {};

      $('body', context).once('omega-mediaqueries', function () {
        $.each(mediaQueries, function (index, value) {
          var mql = window.matchMedia(value);

          // Initially, check if the media query applies or not and add the
          // corresponding class to the body.
          behavior.handler(index, mql);

          // React to media query changes and toggle the class names.
          mql.addListener(function (mql) {
            behavior.handler(index, mql);
          });
        });
      });
    }
  };

})(jQuery, window, Drupal);
