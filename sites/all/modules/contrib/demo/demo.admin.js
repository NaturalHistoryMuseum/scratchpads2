// $Id: demo.admin.js,v 1.1 2010/08/30 14:30:49 sun Exp $
(function ($) {

/**
 * Collapsible snapshots.
 */
Drupal.behaviors.demoCollapse = {
  attach: function (context) {
    $('.demo-snapshots-widget .form-item-filename', context).once('demo-collapse', function () {
      var $item = $(this);
      $item
        .find('label')
          .click(function () {
            $item.find('.description').slideToggle('fast');
          })
          .end()
        .find('.description').hide();
    });
  }
};

})(jQuery);
