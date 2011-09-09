(function($){
  /*var is_control_or_apple_being_held_down = false;
  $(document).ready(function(){
    $(document).bind('keydown', function(event){
      if(event.keyCode == 17 || event.keyCode == 224){
        is_control_or_apple_being_held_down = true;
      }
    });
    $(document).bind('keyup', function(event){
      if(event.keyCode == 17 || event.keyCode == 224){
        is_control_or_apple_being_held_down = false;
      }
    });
  });*/
  Drupal.behaviors.tui = {
    attach: function(context, settings){      
      // OPENING AND CLOSING
      $('li.tui-has-children>div>span', context).unbind('click');
      $('li.tui-has-children>div>span', context).bind('click',function(e){
        if($(this).parent().parent().hasClass('tui-closed') && $(this).parent().parent().hasClass('tui-never-opened')){
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
        } else if (!$(this).parent().parent().hasClass('tui-load')){
          $(this).parent().parent().toggleClass('tui-open');
          $(this).parent().parent().toggleClass('tui-closed');
          $(this).parent().next('ol').toggle();

        }
        e.stopPropagation();
      });
      // CLICK NAME FOR FORM
      $('#tui-tree-container li>div').hover(function(){
        $(this).children('.edit-term-link').css('display', 'inline');
      }, function(){
        $(this).children('.edit-term-link').css('display', 'none');
      });
      // CLICK TO HIGHLIGHT
      $('#tui-tree-container li>div').click(function(){
        function tui_recurse_highlight(parent_tid){
          $('li[data-tui-child-of="'+parent_tid+'"]').each(function(){
            $(this).children('div').each(function(){
              $(this).addClass('tui-highlight');
              tui_recurse_highlight($(this).parent().data('tui-this-term'));
            });
          });
        }
        $('.tui-highlight').removeClass('tui-highlight');
        $(this).addClass('tui-highlight');
        tui_recurse_highlight($(this).parent().data('tui-this-term'));
      });
      // SORTING
      $('#tui-tree-container>ol').nestedSortable({
        cursorAt:{top:15},
        disableNesting: 'no-nest',
        forcePlaceholderSize: true,
        handle: 'div',
        helper: 'clone',
        items: 'li',
        opacity: .8,
        placeholder: 'placeholder',
        revert: 250,
        tabSize: 25,
        tolerance: 'pointer',
        toleranceElement: '> div',
        update:function(event, ui){
          if($('li[data-tui-child-of="'+ui.item.data('tui-child-of')+'"]').length == 1){
            $('li[data-tui-this-term="'+ui.item.data('tui-child-of')+'"]').removeClass('tui-open');
            $('li[data-tui-this-term="'+ui.item.data('tui-child-of')+'"]').removeClass('tui-has-children');
            $('li[data-tui-this-term="'+ui.item.data('tui-child-of')+'"]').addClass('tui-no-children');
          }
          if(ui.item.parent().parent().data('tui-this-term')){
            ui.item.attr('data-tui-child-of', ui.item.parent().parent().data('tui-this-term'));
            ui.item.data('tui-child-of', ui.item.parent().parent().data('tui-this-term'));
          } else {
            ui.item.removeAttr('data-tui-child-of');
            ui.item.removeData('tui-child-of');
          }
          if(ui.item.parent().parent().hasClass('tui-no-children')){
            ui.item.parent().parent().removeClass('tui-no-children');
            ui.item.parent().parent().addClass('tui-open');
            ui.item.parent().parent().addClass('tui-has-children');
            Drupal.attachBehaviors(ui.item.parent().parent().parent());
          }
        }
      });
    }
  }
})(jQuery);