(function($){

  /**
   * ColumnHoverUI
   *
   * This class is used to create and manage the column/cell hover elements
   */
  Drupal.ColumnHoverUI = function(slickgrid){
    /**
     * init
     */
    this.init = function(){
      this.refresh();
    }

    /**
     * refresh
     *
     */
    this.refresh = function(){
      var ops = {
          cssClass: "character-editor-header",
          fill: 'rgba(0, 0, 0, .7)',
          cssStyles: {color: 'white', 'font-size': '10px'},
          spikeLength: 8,
          shrinkToFit: true,
          offsetParent: slickgrid.getContainer(),
          positions: ['bottom']
      };
      $.each(slickgrid.getColumns(), function(i, col){
        if(col.field != "character_entity_field" && col.field != 'sel') {
          $('.' + col.id).bt(col.data.char, ops);
        }
      });
      ops.contentSelector = this.cellHoverCallback;
      $('div.slick-cell:not(.l0, :empty)').bt(ops);
    }

    /**
     * cellHoverCallback
     */
    this.cellHoverCallback = function(){
      var cellText = $(this).html();
      if(cellText.length > 2) {
        return cellText;
      }
      return false;
    }

    this.init();
  }
})(jQuery);