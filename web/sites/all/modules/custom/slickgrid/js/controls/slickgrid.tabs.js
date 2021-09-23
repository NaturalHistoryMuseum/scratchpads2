/**
 * Controller for tabbed columns
 */
(function($){
  function SlickGridTabs(dataView, grid, $container, defaultTab){
    var activeTab;
    var defaultTabIndex = 0;
    function init(){
      constructUI(defaultTab);
      grid.onHeaderContextMenu.subscribe(onHeaderContextMenu);
    }
    function onHeaderContextMenu(e, ui){
      // Hide columns not in this tab for the context menu - otherwise columns
      // in another tab will show as hidden
      $(columns).each(function(i, col){
        if(typeof col.tab !== 'undefined' && col.tab !== activeTab) {
          $('input#columnpicker_' + i, '.slick-columnpicker').parent().hide();
        }
      });
    }
    function handleTabClick(){
      showTab($(this).attr('id'));
      $('.active-tab').removeClass('active-tab');
      $(this).addClass('active-tab');
      slickgrid.updateFilters();
    }
    function showTab(tab){
      activeTab = tab
      var cols = [];
      // Use columns, not grid.getColumns() as it uses visible columns
      $(columns).each(function(i, col){
        if((typeof col.hidden === 'undefined' || col.hidden === false) && (typeof col.tab === 'undefined' || col.tab === tab)) {
          cols.push(col);
        }
      });
      grid.setColumns(cols);
      if(typeof slickgrid !== 'undefined') {
        $(slickgrid.getContainer()).trigger('onSlickgridTabChanged');
      }
    }
    function constructUI(defaultTab){
      var tabs = [];
      $container.empty();
      $(columns).each(function(i, col){
        if(typeof col.tab !== 'undefined') {
          if(tabs.indexOf(col.tab) === -1) {
            if(typeof defaultTab !== 'undefined' && defaultTab == col.tab) {
              defaultTabIndex = tabs.length;
            }
            $("<span class='slickgrid-tab' id='" + col.tab + "'>" + col.tab + "</span>").click(handleTabClick).appendTo($container);
            tabs.push(col.tab);
          }
        }
      });
      $('.slickgrid-tab').eq(defaultTabIndex).addClass('active-tab');
      showTab(tabs[defaultTabIndex]);
    }
    function rebuild(){
      var tab = typeof activeTab === 'undefined' ? defaultTab : activeTab;
      constructUI(tab);
    }
    $.extend(this, {
    // Methods
    "rebuild": rebuild, "showTab": showTab});
    init();
  }
  // Slick.Controls.Tabs
  $.extend(true, window, {Slick: {Controls: {Tabs: SlickGridTabs}}});
})(jQuery);