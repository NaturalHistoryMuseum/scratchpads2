(function($){
  // Attaches the AJAX behavior to Views exposed filter forms and key View links
  Drupal.behaviors.slickgridAdmin = {};
  Drupal.behaviors.slickgridAdmin.attach = function(){
    var dependencies = {filter: 'collapsibleFilter', formatter: 'collapsibleFormatter',}
    $.each(dependencies, function(select, value){
      $('select.' + select).change(function(){
        if(select == 'filter') {
          var dependent = 'formatter';
        } else {
          var dependent = 'filter';
        }
        $dependentSelect = $(this).parents('tr').find('select.' + dependent);
        if($(this).val() == value) {
          $dependentSelect.val(dependencies[dependent]);
        } else if($dependentSelect.val() == dependencies[dependent]) {
          $dependentSelect.val('');
        }
      });
    });
  };
})(jQuery);