(function ($) {

Drupal.behaviors.initColorboxImageModule = {
  attach: function (context, settings) {
    // Image Attach Functionality
    $('div.image-attach-body > a, ul.images a', context).once('init-colorbox-image-module-processed', function () {
      var $img = $('.image', this);
      if ($img.length === 0) {
        return true;
      }

      // Find derivative
      var matches = $img.attr('class').match(/image\-(\w+)/);
      if (matches === null) {
        return true;
      }
      var derivative = matches[1];

      // Create link path
      var path_replacement = settings.colorbox.image_derivative == '_original' ? '' : '.' + settings.colorbox.image_derivative;
      var href = $img.attr('src').replace('.' + derivative, path_replacement);

      // Modify link to image
      this.href = href;
      // Add rel tag to group
      this.rel = 'image-gallery';
      // Add image link title
      this.title = $img.attr('title');
      // Colorbox it
      $(this).addClass('colorbox');
    });
  }
};

})(jQuery);
