/**
 * Javascript for the scratchpads admin theme
 */
 (function($) {
    Drupal.behaviors.scratchpadsAdmin = {};
    Drupal.behaviors.scratchpadsAdmin.attach = function(context) {

        // Attach the help slider behaviour
        $('.help-shortcut a:not(.scratchpads-admin-processed)', context).click(function() {
            $('.region-help').slideToggle();
            return false;
        }).addClass('scratchpads-admin-processed');
    };
})(jQuery);