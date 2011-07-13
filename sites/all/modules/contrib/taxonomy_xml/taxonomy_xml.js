/**
 * I don't know what this function wrapper mojo is up to
 */
 
(function ($) {

/**
 * UI addition to enhance the import source switcher
 */
Drupal.behaviors.taxonomy_xml = {
  attach: function (context, settings) {
  
    $('#edit-data-source').addClass('filtered');
    $('#edit-source-selector').change(
      /*
      When the select changes, we change the class of the containing fieldset.
      This then uses css to hide the unwanted form elements.
      */
      function(){
        var methods = new Array('none', 'upload-file', 'url', 'service');
        for(var m in methods) {
          $('#edit-data-source').removeClass(methods[m]);
        }
        $('#edit-data-source').addClass(this.value)
          .animate({opacity:.5}, 200)
          .animate({opacity:1}, 200)
      }
    );
    // Trigger the filter to update the current display
    $('#edit-source-selector').trigger('change');
  }
}

})(jQuery);
