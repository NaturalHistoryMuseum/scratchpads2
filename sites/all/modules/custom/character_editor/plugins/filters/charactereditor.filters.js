(function($){
  function characterTaxonFilter(field){
    /**
     * init
     *
     * Add events to perform the collapsible filter function
     */
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
          slickgrid.setColumnFilter('character_entity_collapse', characterCollapsedItems.join(','), true);
          e.stopImmediatePropagation();
        }
      });
    }

    /**
     * input
     *
     * Return the text filter input
     */
    function input(){
      return $("<input type='text' />");
    }

    init();
    $.extend(this, {'input': input});
  }
  $.extend(true, window, {Slick: {Filter: {CharacterTaxonFilter: characterTaxonFilter}}});
})(jQuery);
