/**
 * @file add AJAX loading to the manu admin screen
 *
 * @author dman dan@coders.co.nz
 */
(function ($) {

Drupal.behaviors.bigmenu = {
  attach: function(context) {
    // Add click actions to all the child indicators
    // The bigmenu-processed class will mark that we don't want to attach this behavior twice.
    $('form#bigmenu-overview-form .bigmenu-childindictor').not('.bigmenu-processed')
      .addClass('bigmenu-processed')
      .click(
        function(event){
          // Don't let the normal href_ click happen
          event.stopPropagation();

          // jquery 1.3+
          var parentRow = $(this).parents('tr').get();

          // Find the mlid of the cell we are in.
          var mlid = $('.menu-mlid', parentRow).val();

          // If children have been generated already, just show them again.
          if ($(parentRow).hasClass("bigmenu-generated") && $(parentRow).hasClass("bigmenu-collapsed")) {
            $('.childOf-' + mlid).css('display', '');
            // Indicate we are expanded now
            $(parentRow)
              .removeClass('bigmenu-collapsed')
              .addClass('bigmenu-expanded');
            $('.hide-show', parentRow).html('Hide children');

            return false;
          }

          // Prevent double-clicks
          if ($(parentRow).hasClass("bigmenu-processing")) {
            return;
          }
          // Set throbber, and indicate we are busy
          $(parentRow).addClass("bigmenu-processing");

          // We either expand or contract, depending on current status
          if ($(parentRow).hasClass("bigmenu-collapsed")) {
            // Fetch submenu and expand

            // This clicked item has the href call we need to make built in
            // just add 'js' to the end
            var url = $(this).attr('href') + '/js';

            // ALSO, to deal with Drupal form API form cache, add the form build ID
            // so the background process can update the known fields
            var form = $('.menu-mlid', parentRow).attr('form')
            form_id = $('input[name="form_id"]', form).val();
            form_build_id = $('input[name="form_build_id"]', form).val();
            url += "/" + form_id + "/" + form_build_id

            // Make an ajax call for the child items.
            $.ajax({
              dataType: 'json',
              url: url,
              success: function(data, textStatus, XMLHttpRequest) {
                //data = jQuery.parseJson(data);
                if (data.status) {

                  // Shift the rows into this form.
                  var new_form = $(data.data);

                  // Add each tr in the retrieved form  after the parent row
                  // Tag the added rows so we can find and collapse them later
                  var previousRow = parentRow;
                  $('tr', new_form).each(function(index) {
                      if ($('th', this).length == 0) {
                        $(this).addClass('childOf-' + mlid)
                          .css('opacity', 0.2)
                        //.fadeTo(0, 0.5).css('opacity', 0.1)
                        $(previousRow).after(this)

                        // TODO - an animation of some sort - tr,td cannot set height however
                        previousRow = this
                      }
                  });
                  $('.childOf-' + mlid)
                    .animate({opacity:'1'}, 1500)
                    // don't use fadeIn because tht acts odd on table elements

                  // Attach any required behaviors to the table
                  // thus, the further child expanders, and the tabledrag again.
                  // tabledrag doesn't like running twice, so we have to remove some evidence
                  $('#menu-overview').removeClass('tabledrag-processed');
                  $('#menu-overview .tabledrag-handle').remove();

                  Drupal.attachBehaviors();
                  // Remove tabledrag warning, otherwise it will duplicate for each set of children we show.
                  Drupal.theme.tableDragChangedWarning = function () {
                    return '';
                  };

                  // Indicate we are expanded now
                  $(parentRow)
                    .removeClass('bigmenu-collapsed')
                    .addClass('bigmenu-expanded');
                  $('.hide-show', parentRow).html('Hide children');
                }
                else {
                  // Failure...
                  alert(Drupal.t('AJAX error fetching submenu.'));
                  //$('.bigmenu-childindictor', $(parentRow)).remove();
                }

                // Finished processing, whether success or failure
                $(parentRow)
                  .removeClass('bigmenu-processing')
              },
              error: function(XMLHttpRequest, textStatus, errorThrown) {
                // Failure...
                alert(Drupal.t('Error fetching submenu: @error', { '@error': textStatus }));
              }
            });
          }
          else {
            // This item was already expanded, so a click means it should close.
            // That means hide the kids
            $('.childOf-' + mlid).css('display', 'none');

            // Indicate we are closed now
            $(parentRow)
              .removeClass('bigmenu-processing')
              .removeClass('bigmenu-expanded')
              .addClass('bigmenu-collapsed')
              .addClass('bigmenu-generated')
            $('.hide-show', parentRow).html('Show children');
          }

          return false;
        }
      );
  }
};
})(jQuery);
