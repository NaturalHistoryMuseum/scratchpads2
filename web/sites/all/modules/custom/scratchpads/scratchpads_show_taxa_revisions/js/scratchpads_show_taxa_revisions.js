(function($){
  $(document).ready(function(){
    $('.scratchpads-show-taxa-toggle').click(function(){
      $('#scratchpads-show-taxa-table').slideToggle(300);
      var prev = $('.scratchpads-show-taxa-toggle').html();
      $('.scratchpads-show-taxa-toggle').html($('.scratchpads-show-taxa-toggle').data('toggle'));
      $('.scratchpads-show-taxa-toggle').data('toggle', prev);
      return false;
    });
  });
})(jQuery);