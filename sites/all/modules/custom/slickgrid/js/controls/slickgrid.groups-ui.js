(function($){
  function SlickGridGroupsUI(dataView, grid, $container){
    function init(){
      var $nav = $("<span class='slick-groups-nav' />").appendTo($container);
      var icon_prefix = "<span class='ui-state-default ui-corner-all ui-icon-container'><span class='ui-icon ";
      var icon_suffix = "' /></span>";
      $(icon_prefix + "ui-icon-seek-next" + icon_suffix).click(expandAllGroups).appendTo($nav);
      $(icon_prefix + "ui-icon-seek-end" + icon_suffix).click(collapseAllGroups).appendTo($nav);
      $container.children().wrapAll("<div class='slick-groups' />");
    }
    function expandAllGroups(){
      dataView.beginUpdate();
      for( var i = 0; i < dataView.getGroups().length; i++) {
        dataView.expandGroup(dataView.getGroups()[i].value);
      }
      dataView.endUpdate();
    }
    function collapseAllGroups(){
      dataView.beginUpdate();
      for( var i = 0; i < dataView.getGroups().length; i++) {
        dataView.collapseGroup(dataView.getGroups()[i].value);
      }
      dataView.endUpdate();
    }
    init();
  }
  // Slick.Controls.Pager
  $.extend(true, window, {Slick: {Controls: {GroupsUI: SlickGridGroupsUI}}});
})(jQuery);
