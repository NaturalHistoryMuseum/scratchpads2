(function($){
  Drupal.behaviors.tui = {
    attach: function(context, settings){
      $('.tui p', context).click(function(){
        if($(this).hasClass('tui-never-opened')){
          $(this).removeClass('tui-never-opened');
          console.log($(this));
          alert($(this).data('tui-this-term'));
        }
      });
      /* ADD CONTEXT */
      $('.tinytax ul').sortable({
        connectWith: '.tinytax ul',
        axis: 'y'
      });
    }
  }
})(jQuery);