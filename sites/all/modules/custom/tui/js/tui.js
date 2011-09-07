(function($){
  Drupal.tui = new Object();
  Drupal.tui.close_recurse = function(tid){
    $('li[data-tui-child-of="'+tid+'"]').each(function(){
      if($(this).hasClass('tui-has-children') && $(this).hasClass('tui-open')){
        Drupal.tui.close_recurse($(this).data('tui-this-term'));        
      }
      $(this).hide();
    });
  }
  Drupal.tui.open_recurse = function(tid){
    $('li[data-tui-child-of="'+tid+'"]').each(function(){
      if($(this).hasClass('tui-has-children') && $(this).hasClass('tui-open')){
        Drupal.tui.open_recurse($(this).data('tui-this-term'));
      }
      $(this).show();
    });
  }
  
  Drupal.behaviors.tui = {
    attach: function(context, settings){
      // OPENING AND CLOSING
      $('li.tui-has-children div span', context).unbind('click');
      $('li.tui-has-children div span', context).bind('click',function(){
        console.log('Clicked');
        if($(this).parent().parent().hasClass('tui-closed')){
          if($(this).parent().parent().hasClass('tui-never-opened')){
            $(this).parent().parent().removeClass('tui-never-opened');
            $(this).parent().parent().removeClass('tui-closed');
            $(this).parent().parent().addClass('tui-load');
            var clicked_term = $(this);
            $.getJSON(Drupal.settings.tui.callback+"/"+$(this).parent().parent().data('tui-this-term'), function(data){
              clicked_term.parent().parent().append(data);
              clicked_term.parent().parent().removeClass('tui-load');
              clicked_term.parent().parent().addClass('tui-open');
              Drupal.attachBehaviors($('li[data-tui-child-of="'+clicked_term.parent().parent().data('tui-this-term')+'"]').parent().parent());
            });
          } else {
            $(this).parent().parent().removeClass('tui-closed');
            $(this).parent().parent().addClass('tui-open');
            Drupal.tui.open_recurse($(this).parent().parent().data('tui-this-term'));
          }
        } else if ($(this).parent().parent().hasClass('tui-open')){
          $(this).parent().parent().removeClass('tui-open');
          $(this).parent().parent().addClass('tui-closed');
          Drupal.tui.close_recurse($(this).parent().parent().data('tui-this-term'));
        }
      });

      $(document).ready(function(){
        $('.tui>ol').nestedSortable({
          disableNesting: 'no-nest',
          forcePlaceholderSize: true,
          handle: 'div',
          helper: 'clone',
          items: 'li',
          maxLevels: 3,
          opacity: .6,
          placeholder: 'placeholder',
          revert: 250,
          tabSize: 25,
          tolerance: 'pointer',
          toleranceElement: '> div'
        });
      });
      // SORTING
      //$('.tui').nestedSortable();
      /*$('.tui').sortable({
        containment:'.tui',
        cursor:'row-resize',
        grid:[16,5],
        start:function(event, ui){
          console.log(ui);
        },
        stop:function(event, ui){
          var moved = Math.round(ui.position.left/16);
          var proposed_depth = $(ui.item).data('tui-depth') + moved;
          if(proposed_depth <= $(ui.item).prev(':visible').data('tui-depth')+1 && proposed_depth >= $(ui.item).next().data('tui-depth')-1){
            ui.item.data('tui-depth', proposed_depth);
            ui.item.css('margin-left', $(ui.item).data('tui-depth') * 16);
            ui.item.prevAll(':visible').each(function(){
              if($(this).data('tui-depth')<=ui.item.data('tui-depth')){
                if($(this).data('tui-depth')==ui.item.data('tui-depth')){
                  ui.item.data('tui-child-of', $(this).data('tui-child-of'));
                } else {
                  ui.item.data('tui-child-of', $(this).data('tui-this-term'));
                  if($(this).hasClass('tui-no-children')){
                    $(this).addClass('tui-has-children');
                    $(this).addClass('tui-open');                    
                  }
                }
                return false;
              }
            });
          } else {
            var original_color = ui.item.css('color');
            ui.item.css('color', 'red');
            ui.item.animate({
              color:original_color
            }, 3000);
            $(this).sortable('cancel');
          }
        }
      });*/
    }
  }
})(jQuery);