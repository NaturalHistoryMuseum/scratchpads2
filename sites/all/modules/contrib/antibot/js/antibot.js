/**
 * @file
 *   Unlock protected forms by resetting the form action to the path that
 *   it should be, only if the current user is verified to be human.
 */
(function ($) {
  Drupal.antibot = {};

  Drupal.behaviors.antibot = {
    attach: function (context) {
      // Assume the user is not human, despite JS being enabled.
      Drupal.settings.antibot.human = false;

      // Wait for a mouse to move, indicating they are human.
      $('body').mousemove(function() {
        // Unlock the forms.
        Drupal.antibot.unlockForms();
      });

      // Wait for a touch move event, indicating that they are human.
      $('body').bind('touchmove', function() {
        // Unlock the forms.
        Drupal.antibot.unlockForms();
      });

      // A tab or enter key pressed can also indicate they are human.
      $('body').keydown(function(e) {
        if ((e.keyCode == 9) || (e.keyCode == 13)) {
          // Unlock the forms.
          Drupal.antibot.unlockForms();
        }
      });
    }
  };

  /**
   * Revert the action on the protected forms to what it was originally
   * set to.
   */
  Drupal.antibot.unlockForms = function() {
    // Act only if we haven't yet verified this user as being human.
    if (!Drupal.settings.antibot.human) {
      // Iterate all antibot forms that we need to unlock.
      for (var id in Drupal.settings.antibot.forms) {
        // Switch the action to the original value.
        $('form#' + id).attr('action', Drupal.settings.antibot.forms[id].action);
        
        // Check if a key is required.
        if (Drupal.settings.antibot.forms[id].key) {
          // Inject the key value.
          $('form#' + id).find('input[name="antibot_key"]').val(Drupal.settings.antibot.forms[id].key);
        }
      }
      // Mark this user as being human.
      Drupal.settings.antibot.human = true;
    }
  };

})(jQuery);
