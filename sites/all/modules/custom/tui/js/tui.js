(function($){
  // Extend the jQuery object to allow us to redirect.
  $.prototype.tui_goto = function(url){
    window.location.href = url;
  }
  // TUI object.
  Drupal.tui = Drupal.tui || {};
  // Resize frame/window function.
  Drupal.tui.resize_frame = function(){
    window.setTimeout(function(){
      var height_offset = 150;
      if($('#tui-tree-subcontainer ol').height() > ($(window).height() - height_offset)) {
        $('#tui-tree-subcontainer').css('height', ($(window).height() - height_offset) + 'px');
        $('#tui-tree-subcontainer').css('overflow-y', 'scroll');
      } else {
        $('#tui-tree-subcontainer').css('height', 'auto');
        $('#tui-tree-subcontainer').css('overflow-y', 'visible');
      }
      $('#tui-form').width($('#tui').width() - ($('#tui-tree').width() + 2));
    }, 100);
    window.setTimeout(function(){
      if($('.tui-highlight').length) {
        Drupal.tui.scrollto($('.tui-highlight').first());
      }
    }, 200);
  }
  // Scroll to a highlighted term.
  Drupal.tui.scrollto = function(element){
    $('#tui-tree-subcontainer').animate({scrollTop: $(element).position().top + $('#tui-tree-subcontainer').scrollTop() - 50}, 500);
  }
  // On load, scroll to the first term, and resize the frames to fit.
  $(document).ready(function(){
    // First load, check if we have one term highlighted, if so, load
    // the form.
    if($('.tui-highlight').length == 1) {
      $('.tui-highlight').children('a').click();
    }
    if($('.tui-highlight').length) {
      Drupal.tui.scrollto($('.tui-highlight').first());
    }
    Drupal.tui.resize_frame();
  });
  // Attach behaviours.
  Drupal.behaviors.tui = {attach: function(context, settings){
    // OVERALL SIZE OF FRAME LISTENER
    $('#tui', context).change(function(){
      Drupal.tui.resize_frame();
    });
    // ADD A TERM LINKS
    Drupal.ajax['tui-add-link'].beforeSend = function(xmlhttprequest, options){
      if($('.tui-highlight').length) {
        if(options.dataType == "json") {
          options.data = "tid=" + $('.tui-highlight').first().parent().data('tui-this-term') + "&" + options.data;
        }
      }
    }
    Drupal.ajax['tui-add-tab-link'].beforeSend = function(xmlhttprequest, options){
      if($('.tui-highlight').length) {
        if(options.dataType == "json") {
          options.data = "tid=" + $('.tui-highlight').first().parent().data('tui-this-term') + "&" + options.data;
        }
      }
    }
    // UPDATE THE NAME LISTENER
    $('#edit-name', context).keyup(function(event){
      $('#tui-name-live h2').html($('#edit-name').val());
    });
    // THIS IS A DIRECT COPY FROM ajax.js, with the resize_frame function
    // added.
    Drupal.ajax["tui-add-link"].commands.insert = function(ajax, response, status){
      var wrapper = response.selector ? $(response.selector) : $(ajax.wrapper);
      var method = response.method || ajax.method;
      var effect = ajax.getEffect(response);
      var new_content_wrapped = $('<div></div>').html(response.data);
      var new_content = new_content_wrapped.contents();
      if(new_content.length != 1 || new_content.get(0).nodeType != 1) {
        new_content = new_content_wrapped;
      }
      switch(method){
        case 'html':
        case 'replaceWith':
        case 'replaceAll':
        case 'empty':
        case 'remove':
          var settings = response.settings || ajax.settings || Drupal.settings;
          Drupal.detachBehaviors(wrapper, settings);
      }
      wrapper[method](new_content);
      if(effect.showEffect != 'show') {
        new_content.hide();
      }
      if($('.ajax-new-content', new_content).length > 0) {
        $('.ajax-new-content', new_content).hide();
        new_content.show();
        $('.ajax-new-content', new_content)[effect.showEffect](effect.showSpeed);
      } else if(effect.showEffect != 'show') {
        new_content[effect.showEffect](effect.showSpeed);
      }
      if(new_content.parents('html').length > 0) {
        var settings = response.settings || ajax.settings || Drupal.settings;
        Drupal.attachBehaviors(new_content, settings);
      }
      Drupal.tui.resize_frame();
    }
    // BUTTON CLICKS
    $('#tui-tree-links img', context).click(function(){
      switch($(this).attr('id')){
        case 'tui-delete':
          $('#tui-click').unbind('click');
          var tids = '';
          if($('.tui-highlight').length) {
            $('.tui-highlight').each(function(){
              if(tids != '') {
                tids += ',';
              }
              tids += $(this).parent().data('tui-this-term');
            });
            $('#tui-tree-form').remove();
            $('#tui-tree-links').after('<div id="tui-tree-form"></div>');
            var element_settings = {};
            element_settings.progress = {'type': 'throbber'};
            element_settings.url = $('#tui-delete').data('url') + tids;
            element_settings.event = 'click';
            Drupal.ajax['tui-click'] = new Drupal.ajax('tui-click', $('#tui-click'), element_settings);
            $('#tui-click').click();
          }
          break;
        case 'tui-search':
          $('#tui-click').unbind('click');
          $('#tui-tree-form').remove();
          $('#tui-tree-links').after('<div id="tui-tree-form"></div>');
          var element_settings = {};
          element_settings.progress = {'type': 'throbber'};
          element_settings.url = $('#tui-search').data('url');
          element_settings.event = 'click';
          Drupal.ajax['tui-click'] = new Drupal.ajax('tui-click', $('#tui-click'), element_settings);
          $('#tui-click').click();
          break;
      }
    });
    // OPENING AND CLOSING
    // Slightly lazy programming here, but makes the Ajax reply easier.
    $('li.tui-has-children>div>span').unbind('click');
    $('li.tui-has-children>div>span').bind('click', function(e){
      if($(this).parent().parent().hasClass('tui-closed') && $(this).parent().parent().hasClass('tui-never-opened')) {
        $(this).parent().parent().removeClass('tui-never-opened');
        $(this).parent().parent().removeClass('tui-closed');
        $(this).parent().parent().addClass('tui-load');
        var clicked_term = $(this);
        $.getJSON(Drupal.settings.tui.callback + "/" + $(this).parent().parent().data('tui-this-term'), function(data){
          clicked_term.parent().parent().children('ol').remove();
          clicked_term.parent().parent().append(data);
          Drupal.tui.resize_frame();
          clicked_term.parent().parent().removeClass('tui-load');
          clicked_term.parent().parent().addClass('tui-open');
          Drupal.attachBehaviors($('li[data-tui-child-of="' + clicked_term.parent().parent().data('tui-this-term') + '"]').parent().parent());
          if(clicked_term.parent().hasClass('tui-highlight')) {
            $('div', clicked_term.parent().parent()).addClass('tui-highlight');
          }
        });
      } else if(!$(this).parent().parent().hasClass('tui-load')) {
        $(this).parent().parent().toggleClass('tui-open');
        $(this).parent().parent().toggleClass('tui-closed');
        $(this).parent().next('ol').toggle();
        Drupal.tui.resize_frame();
      }
      e.stopPropagation();
    });
    // CLICK NAME FOR FORM
    $('#tui-tree-subcontainer li>div').hover(function(){
      $(this).children('.edit-term-link').css('display', 'inline');
    }, function(){
      $(this).children('.edit-term-link').css('display', 'none');
    });
    $('a.edit-term-link', context).click(function(){
      $('.tui-highlight').removeClass('tui-highlight');
      $('.tui-form-display').removeClass('tui-form-display');
      $(this).parent().addClass('tui-highlight tui-form-display');
      // Scrollbars require us to wait for the form to be added to the
      // page.
      window.setTimeout(function(){
        Drupal.tui.resize_frame();
      }, 200);
    });
    // RESIZABLE
    $('#tui-tree', context).resizable({handles: 'e', resize: function(event, ui){
      $('#tui-form').width($('#tui').width() - ($('#tui-tree').width() + 2));
    }, minWidth: 250});
    // RESIZE WINDOW
    $(window).resize(function(){
      Drupal.tui.resize_frame();
      $('#tui-form').width($('#tui').width() - $('#tui-tree').width() - 2);
    });
    $('#tui-tree', context).resize(function(){
      Drupal.tui.resize_frame();
    });
    // CLICK TO HIGHLIGHT
    $('#tui-tree-subcontainer li>div').once().click(function(e){
      function tui_recurse_highlight(parent_tid){
        $('li[data-tui-child-of="' + parent_tid + '"]').each(function(){
          $(this).children('div').each(function(){
            $(this).toggleClass('tui-highlight');
            tui_recurse_highlight($(this).parent().data('tui-this-term'));
          });
        });
      }
      // If we are not holding control or apple, we remove the highlight from
      // other terms.
      if(!e.ctrlKey && !e.metaKey) {
        $('.tui-highlight').removeClass('tui-highlight');
      }
      $(this).toggleClass('tui-highlight');
      tui_recurse_highlight($(this).parent().data('tui-this-term'));
    });
    // SORTING
    $('#tui-tree-subcontainer>ol').nestedSortable({scrollSensitivity: 40, scrollSpeed: 40, cursorAt: {top: 15}, disableNesting: 'no-nest', forcePlaceholderSize: true, handle: 'div', helper: 'clone', items: 'li', opacity: .8, placeholder: 'placeholder', revert: 250, tabSize: 25, tolerance: 'pointer', toleranceElement: '> div', update: function(event, ui){
      if($('li[data-tui-child-of="' + ui.item.data('tui-child-of') + '"]').length == 1) {
        $('li[data-tui-this-term="' + ui.item.data('tui-child-of') + '"]').removeClass('tui-open');
        $('li[data-tui-this-term="' + ui.item.data('tui-child-of') + '"]').removeClass('tui-has-children');
        $('li[data-tui-this-term="' + ui.item.data('tui-child-of') + '"]').addClass('tui-no-children');
      }
      if(ui.item.parent().parent().data('tui-this-term')) {
        ui.item.attr('data-tui-child-of', ui.item.parent().parent().data('tui-this-term'));
        ui.item.data('tui-child-of', ui.item.parent().parent().data('tui-this-term'));
      } else {
        ui.item.removeAttr('data-tui-child-of');
        ui.item.removeData('tui-child-of');
      }
      if(ui.item.parent().parent().hasClass('tui-no-children')) {
        ui.item.parent().parent().removeClass('tui-no-children');
        ui.item.parent().parent().addClass('tui-open');
        ui.item.parent().parent().addClass('tui-has-children');
        Drupal.attachBehaviors(ui.item.parent().parent().parent());
      }
      var tids_sorted = '';
      $(ui.item).parent().children('li').each(function(){
        if(tids_sorted != '') {
          tids_sorted += ',';
        }
        tids_sorted += $(this).data('tui-this-term');
      });
      var ajax_data = {parent_change: {tid: ui.item.data('tui-this-term'), parent: ui.item.data('tui-child-of')}, sort_change: {tids: tids_sorted}};
      var original_nested_sortable = $(this);
      $.ajax({data: ajax_data, type: 'POST', url: Drupal.settings.tui.sort_callback, success: function(data, textStatus, jqXHR){
        if(typeof data == 'string') {
          data = $.parseJSON(data);
        }
        for( var i in data) {
          if(data[i]['command']) {
            if(data[i]['command'] == 'settings') {
              if(data.merge) {
                $.extend(true, Drupal.settings, data.settings);
              }
            }
          }
        }
      }, error: function(data, textStatus, jqXHR){
        original_nested_sortable.nestedSortable('cancel');
        $('#tui-tree-form').html('<div class="messages error"><h2 class="element-invisible">Error message</h2>' + Drupal.t('There has been an error.  Please reload this page.') + '</div>').slideDown();
      }});
    }});
  }}
})(jQuery);