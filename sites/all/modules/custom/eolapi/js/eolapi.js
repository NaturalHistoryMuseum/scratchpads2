(function ($) {
    // EOLAPI object.
    Drupal.eolapi = Drupal.eolapi || {};
    // Attach behaviors

    Drupal.eolapi.do_the_do = function () {
        if ($('.eolapi-empty').length) {
            var original_this = $('.eolapi-empty').first();
            $.ajax({
                data: $('.eolapi-empty').first().data(), type: 'POST', url: Drupal.settings.eolapi.callback, success: function (data, textStatus, jqXHR) {
                    if (data.length) {
                        $(original_this).parent().parent().parent().html(data);
                    } else {
                        $(original_this).html('No results.');
                    }
                    $(original_this).removeClass('eolapi-empty');
                    Drupal.eolapi.do_the_do();
                }, error: function (data, textStatus, jqXHR) {
                    $(original_this).html('No results.');
                    $(original_this).removeClass('eolapi-empty');
                    Drupal.eolapi.do_the_do();
                }
            });
        }
    }
    Drupal.behaviors.eolapi = {
        attach: function (context, settings) {
            Drupal.eolapi.do_the_do();
            try {
                $('a[href^="' + Drupal.settings.basePath + 'eol/"]', context).each(function () {
                    if ($(this).colorbox) {
                        $(this).attr('href', $(this).attr('href').replace(/\/nojs(\/|$|\?|&|#)/g, '/ajax$1'));
                        $(this).colorbox({ rel: 'eolapi' });
                    }
                });
            } catch (err) { }
        }
    };
})(jQuery);