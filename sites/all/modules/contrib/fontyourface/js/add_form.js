var fontyourfaceSampleText = '';
var fontyourfaceSampleMarkup = '';

(function ($) {

  Drupal.behaviors.fontyourfaceAddForm = {

    attach:function(context, settings) {

      var selector = $('#edit-css').val();

      fontyourfaceSampleText = $('#edit-sample-text').val();
      fontyourfaceSampleMarkup = $('.fontyourface-view').html();

      $('#edit-sample-text').keyup(function() {

        var markup = fontyourfaceSampleMarkup;
        var updatedText = $('#edit-sample-text').val();

        if (updatedText != fontyourfaceSampleText) {

          markup = markup.split(fontyourfaceSampleText).join(updatedText);
          markup = markup.split(fontyourfaceEncodeURI(fontyourfaceSampleText)).join(fontyourfaceEncodeURI(updatedText));

        } // if

        $('.fontyourface-view').html(markup);

      });

      if ($('#edit-css').length > 0) {

        $('<select id="edit-css-select"><option value="h1, h2, h3, h4, h5, h6">all headers (h1, h2, h3, h4, h5, h6)</option><option value="h1">h1</option><option value="h2">h2</option><option value="h3">h3</option><option value="p, div">standard text (p, div)</option><option value="body">everything (body)</option><option value="">other</option></select>')
          .change(fontyourfaceCssSelectChange)
          .insertBefore('#edit-css');

        if (
          (selector != '') && 
          ($('#edit-css-select option[value="' + selector + '"]').length > 0)
        ) {
          $('#edit-css-select option[value=' + selector + ']').attr('selected', true);
          $('#edit-css').hide();
        } // if
        else {
          $('#edit-css-select option[value=""]').attr('selected', true);
          $('#edit-css').show();
        } // else

      } // if

    } // attach

  } // Drupal.behaviors.fontyourfaceAddForm

  function fontyourfaceCssSelectChange() {

    var selector = $(this).val();

    if (selector == '') {
      $('#edit-css').show();
    } // if
    else {
      $('#edit-css').val(selector);
      $('#edit-css').hide();
    } // else

  } // fontyourfaceCssSelectChange

  function fontyourfaceEncodeURI(text) {
  
    return encodeURIComponent(text)
      .replace(/!/g, '%21').replace(/'/g, '%27').replace(/\(/g, '%28')
      .replace(/\)/g, '%29').replace(/\*/g, '%2A').replace(/%20/g, '+');

  } // fontyourfaceEncodeURI

})(jQuery);
