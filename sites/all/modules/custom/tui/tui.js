Drupal.tui = new Object;

Drupal.tui.init = function(context) {
  $('.tui-term', context).click(function (){
    $('.tui-added-original').removeClass('tui-added-original');
    if(!Drupal.tui.form_changed){
      Drupal.tui.display_form(this);
    }else{
      Drupal.tui.form_changed = false;
      $('#tabs').before('<div class="message error" style="margin-bottom:5px">'+Drupal.t('You may have unsaved data, please save it first.')+'</p></div>');
    }
  });
  $('#tabs > ul', context).tabs();
  $('#tabs > ul > li, #tui-tree-links img', context).bt({positions: 'top',fill: 'rgba(0, 0, 0, .7)',cssStyles: {color: 'white', 'font-size': '14px', width: 'auto'},closeWhenOthersOpen: true,spikeLength: 10,strokeWidth: 0});
  $('.tui-node-closed', context).click(function(){Drupal.tui.click_closed($(this).parent().parent().attr('id'));});
  $('.tui-node-open', context).click(function(){Drupal.tui.click_open($(this).parent().parent().attr('id'));});
  $('#tui-tree-subcontainer li').draggable({helper:'clone',cursorAt:{left:1, top:1},handle:'> p > .tui-term',opacity:0.8,delay:200,distance:10,start:function(event, ui){Drupal.tui.drag_start(event, ui);}});
  $(window).resize(function(){Drupal.tui.resize_frame();$('#tui-form').width($('#tui').width()-$('#tui-tree').width()-2);});
  $('#tui-tree-links img', context).mouseup(function(){Drupal.tui.click_buttonclick($(this).attr('id'));});
  $('#tui-tree').resizable({handles:'e',resize:function(){$('#tui-form').width($('#tui').width()-$('#tui-tree').width());},minWidth:250});
  $('#taxonomy-form-term *', context).change(function(){Drupal.tui.form_changed = true;});
  $('#taxonomy-form-term *', context).keypress(function(){Drupal.tui.form_changed = true;return event.keyCode!=13;});
  $('#tui-tree-subcontainer').mouseover(function(){document.onselectstart = function(){ return false;}});
  $('#tui-tree-subcontainer').mouseout(function(){document.onselectstart = function(){ return true;}});
  Drupal.tui.form_changed = false;
  Drupal.tui.resize_frame();
}

Drupal.tui.search_return_press = function(event){
  var keyCode = event.keyCode ? event.keyCode : event.which ? event.which : event.charCode;
  if (keyCode == 13) {
    if($('#autocomplete .selected')[0] && $('#autocomplete .selected')[0].autocompleteValue != $('#autocomplete .selected div').html() && $('#autocomplete .selected')[0].autocompleteValue.indexOf(':')){
      Drupal.tui.search_submit_success([$('#autocomplete .selected')[0].autocompleteValue.substring(0, $('#autocomplete .selected')[0].autocompleteValue.indexOf(':'))]);
      return false;
    } else {
      $('#edit-tui-search-input').val($('#autocomplete .selected div').html());
      return Drupal.tui.search_submit();
    }
  }  
  else{
    return true;
  }
}

Drupal.tui.search_submit = function(){
  $.getJSON(Drupal.settings.tui.callbacks.search+"/"+Drupal.settings.tui.vocabulary+"/"+escape($('#edit-tui-search-input').val()),undefined, function(data){Drupal.tui.search_submit_success(data);});
  return false;
}

Drupal.tui.search_submit_success = function(data){
  $.each(data, function(index, value){
    Drupal.settings.tui.opentids[value] = value;
  });
  if(data.length){
    Drupal.tui.click_buttonclick('tui-search');
    Drupal.tui.searchtids = data;
    Drupal.tui.reload_tree();
  } else {
    $('#edit-tui-search-input').effect("highlight", {color:'#ff0000'}, 3000);
  }
}

Drupal.tui.click_buttonclick = function(img_clicked){
  switch(img_clicked){
    case 'tui-search':
      if(!Drupal.tui.search_is_transitioning){
        if(Drupal.tui.search_is_displayed){
          Drupal.tui.search_is_displayed = false;
          Drupal.tui.search_is_transitioning = true;
          $('#edit-tui-search-input').unbind('blur');
          $('#tui-search-form-container').fadeOut(500, function(){
            Drupal.tui.search_is_transitioning = false;
          });
        } else {
          Drupal.tui.search_is_displayed = true;
          Drupal.tui.search_is_transitioning = true;
          $('.bt-wrapper').hide();
          $('#edit-tui-search-input').val('');
          $('#tui-search-form-container').fadeIn(500, function(){
            Drupal.tui.search_is_transitioning = false;
          });
          $('#edit-tui-search-input').focus();
          $('.tui-modal').click(function(){
            Drupal.tui.click_buttonclick('tui-search');
          });
        }
      }
      break;
    case 'tui-add':
      $('#'+Drupal.tui.term_id).addClass('tui-added-original');
      Drupal.tui.term_id = 'new-'+Drupal.settings.tui.vocabulary+'-'+Drupal.tui.term_id;
      Drupal.tui.display_form(false);
      Drupal.tui.add_clicked = true;
      break;
    case 'tui-delete':
      if(Drupal.tui.term_id){
        $('#tui-tree-links').append('<div id="dialog" title=""></div>');
        $('#dialog').attr('title', Drupal.settings.tui.dialog.dlete.title);
        $('#dialog').html(Drupal.settings.tui.dialog.dlete.content);
        $('#tui-dialog-term-name').html($('.tui-term.active').html());
        $('#dialog').dialog({modal:true,buttons:{"Cancel":function(){$(this).dialog("close");},"OK":function(){Drupal.tui.do_delete(Drupal.tui.term_id);$('#tui-form-container').html('');$('#tui-name-editing').html('');$(this).dialog("close");}},width:'450px',height:'150px'});
      }
      break;
    case 'tui-import':
      alert('Import is coming soon!');
      break;
    case 'tui-link':
      if(!Drupal.tui.link_is_displayed){
        Drupal.tui.link_is_displayed = true;
        $('#tui-link-box').fadeIn(500);
        $('#tui-link-back').focus(function(){
          this.select();
        });
        $('#tui-link-back').focus();        
      } else {
        Drupal.tui.link_is_displayed = false;
        $('#tui-link-box').hide();
        $('#tui-link-box').unbind('focus');
      }
      break;
    case 'tui-resort':
      if(Drupal.tui.form_being_displayed){
        $.ajax({cache:false,url:Drupal.settings.tui.callbacks.restoresort+"/"+Drupal.tui.form_being_displayed,success:function(data){Drupal.tui.reload_tree();}});
      } else {
        alert(Drupal.t('Please click on a term first'));
      }
      break;
    case 'tui-next':
    case 'tui-previous':
      if(!Drupal.tui.form_changed){
        Drupal.tui.selected_tab = $('.ui-tabs-selected > a').attr('href');
        $.ajax({cache:false,url:Drupal.settings.tui.callbacks.nextorprevious+"/"+img_clicked+"/"+Drupal.settings.tui.vocabulary+"/"+Drupal.tui.term_id,success:function(data){Drupal.tui.term_id = "tid-"+data;if(!$('#'+Drupal.tui.term_id).length){Drupal.settings.tui.opentids[data] = data;Drupal.tui.update_link();Drupal.tui.show_form_after_tree_rebuild = true;Drupal.tui.reload_tree();}else{Drupal.tui.display_form($('#' + Drupal.tui.term_id));Drupal.tui.scrollto($('#' + Drupal.tui.term_id));}}});
      }else{
        Drupal.tui.form_changed = false;
        $('#tabs').before('<div class="message error" style="margin-bottom:5px">'+Drupal.t('You may have unsaved data, please save it first.')+'</p></div>');
      }
      break;
    case 'tui-undo':
      $.ajax({cache:false,url:Drupal.settings.tui.callbacks.undo+"/"+Drupal.settings.tui.vocabulary,success:function(data){if(data){Drupal.tui.searchtids=[data];Drupal.tui.term_id = "tid-"+data;Drupal.settings.tui.opentids[data] = data;Drupal.tui.update_link();Drupal.tui.reload_tree();}}});
      break;
  }
}

Drupal.tui.do_delete = function(term_id){
  $.ajax({cache:false,url:Drupal.settings.tui.callbacks.dlete+"/"+term_id,success:function(data){Drupal.tui.reload_tree();}});
}

Drupal.tui.resize_frame = function(){
  if($('#tui-tree-subcontainer ul').height() > ($(window).height()-70)){
    $('#tui-tree-subcontainer').css('height', ($(window).height()-70)+'px');
    $('#tui-tree-subcontainer').css('overflow-y', 'scroll');
  } else {
    $('#tui-tree-subcontainer').css('height', 'auto');
    $('#tui-tree-subcontainer').css('overflow-y', 'visible');
  }
}

Drupal.tui.update_link = function(){
  var tids_string = '';
  for(var i in Drupal.settings.tui.opentids){
    tids_string += Drupal.settings.tui.opentids[i]+"%2C";
  }
  $('#tui-link-back').val(Drupal.settings.tui.callbacks.page+'/'+tids_string.substring(0, tids_string.length-3));
}

Drupal.tui.drag_start = function(event, ui){
  $('.tui-term.active').removeClass('active');
  $('#tui-tree-subcontainer').css('height', 'auto');
  $('#tui-tree-subcontainer').css('overflow-y', 'visible');
  delete Drupal.tui.term_id;
  $(event.currentTarget).addClass("tui-added-original");
  $('#tui-tree-subcontainer .tui-nodeleaf, #tui-tree-subcontainer p').droppable({tolerance:'pointer',greedy:true,over:function(event, ui){Drupal.tui.drop_over(event, ui);},deactivate:function(event, ui){Drupal.tui.drop_deactivate(event, ui);}});
}

Drupal.tui.drop_deactivate = function(event, ui){
  if(!Drupal.tui.waiting_for_reply){
    $('#tui-tree-subcontainer .tui-nodeleaf, #tui-tree-subcontainer p').droppable("destroy");
    Drupal.tui.waiting_for_reply = true;
    var tid_array = Drupal.tui.this_id.split('-');
    tid_to_add = 'tid-'+tid_array[1];
    Drupal.settings.tui.opentids[tid_to_add] = tid_to_add;
    Drupal.tui.searchtids = [tid_array[1]];
    $.ajax({cache:false,url:Drupal.settings.tui.callbacks.move+"/"+Drupal.tui.parentorsibling+"/"+Drupal.settings.tui.vocabulary+"/"+Drupal.tui.this_id+"/"+Drupal.tui.parent_or_sibling_id,success:function(data){Drupal.tui.reload_tree();}});
    Drupal.tui.display_form($('#'+Drupal.tui.this_id));
  }
}

Drupal.tui.drop_over = function(event, ui){
  Drupal.tui.this_id = $(ui.draggable).attr('id');
  if($(ui.element).parent().attr('id') == ''){
    Drupal.tui.parentorsibling = 'sibling';
    Drupal.tui.parent_or_sibling_id = $(ui.element).parent().parent().attr('id');
  } else {
    Drupal.tui.parentorsibling = 'child';
    Drupal.tui.parent_or_sibling_id = $(ui.element).parent().attr('id');
  }
  $('.tui-added').remove();
  if($(ui.element)[0].nodeName == 'P'){
    $(ui.element).append('<ul class="tui-added"><li>'+$(ui.draggable).html()+'</li></ul>');    
  } else {
    $(ui.element).parent().parent().after('<li class="tui-added">'+$(ui.draggable).html()+'</li>');    
  }
}

Drupal.tui.click_closed = function(vid_and_tid){  
  $('#'+vid_and_tid+' > p > span.tui-nodeleaf').removeClass('tui-node-closed');
  $('#'+vid_and_tid+' > p > span.tui-nodeleaf').addClass('tui-node-open');
  $('#'+vid_and_tid+' > p > span.tui-nodeleaf').unbind('click');
  $.ajax({cache:false,url:Drupal.settings.tui.callbacks.tree+"/"+vid_and_tid,success:function(data){Drupal.tui.tree_success($('#'+vid_and_tid), data);}});
}

Drupal.tui.click_open = function(vid_and_tid){
  $('#'+vid_and_tid+' > p > span.tui-nodeleaf').removeClass('tui-node-open');
  $('#'+vid_and_tid+' > p > span.tui-nodeleaf').addClass('tui-node-closed');
  Drupal.tui.remove_tid(vid_and_tid);
  $('#'+vid_and_tid).children('ul').remove();
  jQuery.each(Drupal.behaviors, function() {
    this($('#'+vid_and_tid));
  });
}

Drupal.tui.remove_tid = function(vid_and_tid){
  jQuery.each($('#'+vid_and_tid+' > ul .tui-term'), function(idx, obj){
    tid_to_remove = $(obj).attr('id').substring(4);
    delete Drupal.settings.tui.opentids[tid_to_remove];
  });
  Drupal.tui.update_link();
}

Drupal.tui.add_tid = function(vid_and_tid){
  jQuery.each($('#'+vid_and_tid+' > ul .tui-term'), function(idx, obj){
    tid_to_add = $(obj).attr('id').substring(4);
    Drupal.settings.tui.opentids[tid_to_add] = tid_to_add;
  });
  Drupal.tui.update_link();
}

Drupal.tui.tree_success = function(html_object, data){
  $(html_object).append(data);
  jQuery.each(Drupal.behaviors, function() {
    this(html_object);
  });
  Drupal.tui.add_tid($(html_object).attr('id'));
}

Drupal.tui.full_tree_success = function(data){
  Drupal.tui.waiting_for_reply = false;
  $('#tui-tree-subcontainer').html(data);
  jQuery.each(Drupal.behaviors, function() {
    this('#tui-tree-subcontainer');
  });
  if(Drupal.tui.show_form_after_tree_rebuild){
    Drupal.tui.show_form_after_tree_rebuild = false;
    Drupal.tui.display_form($('#'+Drupal.tui.term_id));
    Drupal.tui.scrollto($('#'+Drupal.tui.term_id));
  }
  else if(Drupal.tui.form_being_displayed){
    $('#'+Drupal.tui.form_being_displayed).addClass('active');
  }
  else if(Drupal.tui.display_new_term){
    Drupal.tui.display_new_term = false;
    Drupal.tui.display_form($('.active'));
    Drupal.tui.scrollto($('.active'));
  }
  if(Drupal.tui.searchtids){
    position = $('#tui').offset();
    $('html,body').animate({scrollTop:position.top-30}, 1000);
    //Drupal.tui.display_form($('#tid-'+Drupal.tui.searchtids[0]));
    var highest_position = 10000000;
    var highest_element = false;
    $.each(Drupal.tui.searchtids, function(index, value){
      $('#tid-'+value).effect("highlight", {}, 8000);
      if($('#tid-'+value).position().top < highest_position){
        highest_position = $('#tid-'+value).position().top;
        highest_element = $('#tid-'+value);
      }
    });
    if(highest_element){
      Drupal.tui.scrollto(highest_element);
    }
    Drupal.tui.searchtids = false;
  }
}

Drupal.tui.scrollto = function(element){
  $('#tui-tree-subcontainer').animate({scrollTop:$(element).position().top+$('#tui-tree-subcontainer').scrollTop()-35}, 500);
}

Drupal.tui.form_success = function(data){
  $('#tui-form-container').html(data);
  jQuery.each(Drupal.behaviors, function() {
    this($('#tui-form-container'));
  });
  if(Drupal.tui.selected_tab){
    $('#tabs > ul').tabs('select', Drupal.tui.selected_tab);
  }
  $('.loading').removeClass('loading');
  var name_not_found = true;
  var term_name = '';
  var i='0';
  while(name_not_found && i<10){
    if(i>0){
      id = '#edit-name-'+i;      
    } else {
      id = '#edit-name';     
    }
    if($(id).val()){
      $('#tui-name-editing').html($(id).val());
      // Another one for the f'ing ITIS module.
      if($('#itis_term_unit_name1 input').val()){
        $('#tui-name-editing').html($('#itis_term_unit_name1 input').val()+' '+$('#itis_term_unit_name2 input').val()+' '+$('#itis_term_unit_name3 input').val()+' '+$('#itis_term_unit_name4 input').val())
      }
      name_not_found = false;
    }
    i ++;
  }
}

Drupal.tui.display_form = function(element){
  if($(element).attr('id')){
    Drupal.settings.tui.opentids[$(element).attr('id').substring(4)] = $(element).attr('id').substring(4);
  }
  Drupal.tui.add_clicked = false;
  if(element){
    $('.tui-term').removeClass('active');
    Drupal.tui.form_being_displayed = $(element).attr('id');
    $(element).addClass('active');
    $(element).addClass('loading');
    if($(element).attr('id')){
      Drupal.tui.term_id = $(element).attr('id');
    }
  }
  $.ajax({cache:false,url:Drupal.settings.tui.callbacks.form+"/"+Drupal.tui.term_id,success:function(data){Drupal.tui.form_success(data);}}); 
}

Drupal.tui.reload_tree = function(){
  var callback_url = Drupal.settings.tui.callbacks.full_tree+"/"+Drupal.settings.tui.vocabulary;
  if(Drupal.tui.add_clicked){
    callback_url += "/added";
  }
  $.ajax({type:'POST',cache:false,url:callback_url,success:function(data){Drupal.tui.full_tree_success(data);},data:Drupal.settings.tui.opentids});
}

Drupal.behaviors.tui = function(context){
  Drupal.tui.init(context);
};
