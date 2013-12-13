(function($){
  function characterCollapsibleFilter(field){
    function init(){
      grid.onClick.subscribe(function(e, args){
        if($(e.target).hasClass("characterToggle")) {
          var item = args.grid.getDataItem(args.row);
          if (typeof item._tree_state === 'undefined' || item._tree_state == 'expanded'){
            if (characterCollapsedItems.indexOf(item.id) == -1){
              characterCollapsedItems.push(item.id);
            }
          } else {
            var index = characterCollapsedItems.indexOf(item.id);
            if (index > -1){
              characterCollapsedItems.splice(index, 1);
            }
          }
          slickgrid.setColumnFilter(field, characterCollapsedItems.join(','), true);
          e.stopImmediatePropagation();
        }
      });
    }
    init();
  }
  $.extend(true, window, {Slick: {Filter: {CharacterCollapsible: characterCollapsibleFilter}}});
})(jQuery);