;(function($, undefined) {
  Drupal.behaviors.mediaMultiselect = {
    attach: function(context, settings) {
      for (var base in settings.ajax) {
        var element_settings = settings.ajax[base];
        if (element_settings.event == 'media_select' && !$('#' + base + '.media-multiselect-processed').length) {
          var media_settings = settings.media.multi_select.elements[base];
          
          // Bind a click-event to the 'add more' button.
          $('#' + base).click(function(event) {
            // Add a new beforeSubmit that adds in our fids
            Drupal.ajax[base].beforeSubmit = function (form_values, element, options) {
              // Add the fids to the form_values.
              for (var i=0; i<this.media_multiselect_files.length; i++) {
                form_values.push({name: 'media_multiselect_fids[]', value: this.media_multiselect_files[i].fid});
              }

              // Call the prototype, so we preseve any existing functionality in there.
              this.__proto__.beforeSubmit.call(this, form_values, element, options)
            }
            
            var button = this;
            // Launch the Media Browser.
            Drupal.media.popups.mediaBrowser(function(files) {
              Drupal.ajax[base].media_multiselect_files = files
              $(button).trigger('media_select');
            }, media_settings.global);

            // Aaaand prevent default.
            event.preventDefault();
          })

          $('#' + base).addClass('media-multiselect-processed');
        }
      }
    }
  }
})(jQuery);