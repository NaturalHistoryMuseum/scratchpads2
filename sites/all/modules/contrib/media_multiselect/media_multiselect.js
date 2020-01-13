;(function($, undefined) {
  Drupal.behaviors.mediaMultiselect = {
    attach: function(context, settings) {
      for (var base in settings.ajax) {
        var element_settings = settings.ajax[base];
        if (element_settings.event == 'media_select' && !$('#' + base + '.media-multiselect-processed').length) {
          // Bind a click-event to the 'add more' button.
          $('#' + base).click((function(element) { return function(event) {
            var media_settings = settings.media.multi_select.elements[element];

            // Add a new beforeSubmit that adds in our fids
            Drupal.ajax[element].beforeSubmit = function (form_values, element, options) {
              
              console.log(form_values);
              console.log(this.media_multiselect_files);
              
              // Add the fids to the form_values.
              for (var i=0; i<this.media_multiselect_files.length; i++) {
                form_values.unshift({name: 'media_multiselect_fids[]', value: this.media_multiselect_files[i].fid});
              }

              // Call the prototype, so we preseve any existing functionality in there.
              Drupal.ajax.prototype.beforeSubmit.call(this, form_values, element, options)
            }
            
            var button = this;
            // Launch the Media Browser.
            Drupal.media.popups.mediaBrowser(function(files) {
              Drupal.ajax[element].media_multiselect_files = files
              $(button).trigger('media_select');
            }, media_settings.global);

            // Aaaand prevent default.
            event.preventDefault();
          }})(base))

          $('#' + base).addClass('media-multiselect-processed');
        }
      }
    }
  }
})(jQuery);
