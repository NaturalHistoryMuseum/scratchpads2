 /**
  * Controller for tabbed columns
  */
  (function($) {
      function SlickGridTabs(dataView, grid, $container)
      {
          
          var activeTab;
          
          function init()
          {
              constructUI();
              
              grid.onHeaderContextMenu.subscribe(onHeaderContextMenu);

          }

          function onHeaderContextMenu(e, ui){
            
            // Hide columns not in this tab for the context menu - otherwise columns in another tab will show as hidden
            $(columns).each(function(i, col) {             
              if(typeof col.tab !== 'undefined' && col.tab !== activeTab){
                 $('input#columnpicker_'+i, '.slick-columnpicker').parent().hide();                 
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
           $(columns).each(function(i, col) {             
              if((typeof col.hidden === 'undefined' || col.hidden === false) && (typeof col.tab === 'undefined' || col.tab === tab)){
                cols.push(col);
              }
            });      
            grid.setColumns(cols);
          }

          function constructUI()
          {
              var tabs = [];
              $container.empty();
              
              $(columns).each(function(i, col) {
                if(typeof col.tab !== 'undefined'){
                 if(tabs.indexOf(col.tab) === -1){
                   $("<span class='slickgrid-tab' id='"+col.tab+"'>"+col.tab+"</span>").click(handleTabClick).appendTo($container);
                   tabs.push(col.tab);
                 }                                
                }
              });  

              $('.slickgrid-tab').eq(0).addClass('active-tab');
              showTab(tabs[0]);
              
          }

          init();
      }

      // Slick.Controls.Tabs
      $.extend(true, window, { Slick: { Controls: { Tabs: SlickGridTabs }}});
  })(jQuery);