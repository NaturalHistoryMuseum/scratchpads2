// $Id: mollom.js,v 1.9 2010/08/04 03:58:46 dries Exp $
(function ($) {

/**
 * Open Mollom privacy policy link in a new window.
 *
 * Required for valid XHTML Strict markup.
 */
Drupal.behaviors.mollomPrivacy = {
  attach: function (context) {
    $('.mollom-privacy a', context).click(function () {
      this.target = '_blank';
    });
  }
};

/**
 * Attach click event handlers for CAPTCHA links.
 */
Drupal.behaviors.mollomCaptcha = {
  attach: function (context, settings) {
    // @todo Pass the local settings we get from Drupal.attachBehaviors(), or
    //   inline the click event handlers, or turn them into methods of this
    //   behavior object.
    $('a.mollom-switch-captcha', context).click(getMollomCaptcha);
  }
};

/**
 * Fetch a Mollom CAPTCHA and output the image or audio into the form.
 */
function getMollomCaptcha() {
  // Get the current requested CAPTCHA type from the clicked link.
  var newCaptchaType = $(this).hasClass('mollom-audio-captcha') ? 'audio' : 'image';

  var context = $(this).parents('form');

  // Extract the Mollom session id and form build id from the form.
  var mollomSessionId = $('input.mollom-session-id', context).val();
  var formBuildId = $('input[name="form_build_id"]', context).val();

  // Retrieve a CAPTCHA:
  $.getJSON(Drupal.settings.basePath + 'mollom/captcha/' + newCaptchaType + '/' + formBuildId + '/' + mollomSessionId,
    function (data) {
      if (!(data && data.content)) {
        return;
      }
      // Inject new CAPTCHA.
      $('.mollom-captcha-content', context).parent().html(data.content);
      // Update session id.
      $('input.mollom-session-id', context).val(data.session_id);
      // Add an onclick-event handler for the new link.
      Drupal.attachBehaviors(context);
      // Focus on the CATPCHA input.
      $('input[name="mollom[captcha]"]', context).focus();
    }
  );
  return false;
}

})(jQuery);
