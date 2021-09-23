(function($){
  $(document).ready(function(){
    $('ul.scratchpads-qa h2').click(function(){
      $(this).siblings('.hidden-table').slideToggle(1000);
      return false;
    })
  });
})(jQuery);