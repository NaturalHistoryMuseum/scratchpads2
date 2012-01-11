

/**
 * @file
 * Make Hierarchical Select work in Views' exposed filters form.
 *
 * Views' exposed filters form is a GET form, but since Hierarchical Select
 * really is a combination of various form items, this will result in a very
 * ugly and unnecessarily long GET URL, which also breaks the exposed filters.
 * This piece of JavaScript is a necessity to make it work again, but it will
 * of course only work when JavaScript is enabled!
 */


if (Drupal.jsEnabled) {
  $(document).ready(function(){
    $('.view-filters form').submit(function() {
      // Remove the Hierarchical Select form build id and the form id, to
      // prevent them from ending up in the GET URL.
      $('#edit-hs-form-build-id').remove();

      // Prepare the hierarchical select form elements that are used as
      // exposed filters for a GET submit.
      $('.view-filters form')
      .find('.hierarchical-select-wrapper')
      .trigger('prepare-GET-submit');
    });
  });
}
