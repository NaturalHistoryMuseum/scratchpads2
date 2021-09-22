(function ($) {
  Drupal.behaviors.tree_widget = {
    attach: function (context, settings) {
      $('.options_tree_plus', context).once('tree_widget', function () {
        $(this).click(
          function () {
            // Click the plus/minus buttons
            if ($(this).hasClass('plus')) {
              if ($(this).siblings('ul').first().length) {
                $(this).siblings('ul').first().slideDown();
                $(this).removeClass('plus').addClass('minus');
              } else {
                var original_click = $(this);
                var name = $(this).attr('id').split('__').shift();
                if (name.indexOf('[')) {
                  var name_no_array = name.split('[').shift();
                } else {
                  name_no_array = name;
                }
                $
                  .getJSON(
                    Drupal['settings']['tree_widget'][name_no_array]['callback']
                    + '/'
                    + $(this).siblings('label').first().children(
                      'input').first().val() + '/' + name,
                    function (data) {
                      $(original_click).parent()
                        .append(data[1]["data"]);
                      Drupal.attachBehaviors($(original_click).parent()
                        .children());
                      $(original_click).removeClass('plus').addClass(
                        'minus');
                    });
              }
            } else {
              $(this).siblings('ul').first().slideUp();
              $(this).removeClass('minus').addClass('plus');
            }
          });
      });

      // $('.options_tree_plus', context)

      $('input[type="checkbox"]', context).each(function () {
        checkMaxChoices($(this));
      });
      $('input[type="checkbox"]', context).click(function () {
        checkMaxChoices($(this));
      });
    }
  };
  function checkMaxChoices(item) {
    name = item.attr('name');
    if (name.indexOf('[')) {
      var name_no_array = name.split('[').shift();
    } else {
      name_no_array = name;
    }
    if (Drupal['settings']['tree_widget'][name_no_array] && Drupal['settings']['tree_widget'][name_no_array]['max'] > 0) {
      var checked_count = $('input[name="' + item.attr('name') + '"]:checked').length;
      if (checked_count >= Drupal['settings']['tree_widget'][name_no_array]['max']) {
        $('input[name="' + item.attr('name') + '"]:not(:checked)').attr(
          'disabled', 'disabled');
      } else {
        $('input[name="' + item.attr('name') + '"]:not(:checked)').removeAttr(
          'disabled');
      }
    }
  }
})(jQuery);
