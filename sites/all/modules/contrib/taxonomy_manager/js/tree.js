
/**
 * @files js for collapsible tree view with some helper functions for updating tree structure
 */

(function ($) {

Drupal.behaviors.TaxonomyManagerTree = {
  attach: function(context, settings) {
    var treeSettings = settings.taxonomytree || [];
    if (treeSettings instanceof Array) {
      for (var i=0; i<treeSettings.length; i++) {
        if (!$('#'+ treeSettings[i].id +'.tm-processed').length) {
          new Drupal.TaxonomyManagerTree(treeSettings[i].id, treeSettings[i].vid, treeSettings[i].parents);
        }
      }
    }  
    //only add throbber for TM sites
    var throbberSettings = settings.TMAjaxThrobber || [];
    if (throbberSettings['add']) {
      if (!$('#taxonomy-manager-toolbar-throbber.tm-processed').length) {
        $('#taxonomy-manager-toolbar-throbber').addClass('tm-processed');
        Drupal.attachThrobber();
        Drupal.attachResizeableTreeDiv();
      } 
    }
  }
}


Drupal.TaxonomyManagerTree = function(id, vid, parents) {
  this.div = $("#"+ id);
  this.ul = $(this.div).children();
  
  this.form = $(this.div).parents('form');
  this.form_build_id = $(this.form).children().children(':input[name="form_build_id"]').val();
  this.form_id = $(this.form).children().children(' :input[name="form_id"]').val();
  this.form_token = $(this.form).children().children(' :input[name="form_token"]').val();
  this.language = this.getLanguage();
  this.treeId = id;
  this.vocId = vid;
  this.formParents = parents;
  this.childFormUrl = Drupal.settings.childForm['url'];
  this.siblingsFormUrl = Drupal.settings.siblingsForm['url'];

  this.attachTreeview(this.ul);
  this.attachSiblingsForm(this.ul);
  this.attachSelectAllChildren(this.ul);
  this.attachLanguageSelector();
  this.attachMsgCloseLink();
  
  //attach term data js, if enabled
  var term_data_settings = Drupal.settings.termData || [];
  if (term_data_settings['url']) {
    Drupal.attachTermData(this.ul);
  }
  $(this.div).addClass("tm-processed");
}

/**
 * adds collapsible treeview to a given list
 */
Drupal.TaxonomyManagerTree.prototype.attachTreeview = function(ul, currentIndex) {
  var tree = this;
  if (currentIndex) {
    ul = $(ul).slice(currentIndex);
  }
  var expandableParent = $(ul).find("div.hitArea");
  $(expandableParent).click(function() {
    var li = $(this).parent();
    tree.loadChildForm(li);
    tree.toggleTree(li);
  });
  $(expandableParent).parent("li.expandable, li.lastExpandable").children("ul").hide();
}

/**
 * toggles a collapsible/expandable tree element by swaping classes
 */
Drupal.TaxonomyManagerTree.prototype.toggleTree = function(node) {
  $(node).children("ul").toggle();
  this.swapClasses(node, "expandable", "collapsable");
  this.swapClasses(node, "lastExpandable", "lastCollapsable");
}

/**
 * helper function for swapping two classes
 */
Drupal.TaxonomyManagerTree.prototype.swapClasses = function(node, c1, c2) {
  if ($(node).hasClass(c1)) {
    $(node).removeClass(c1).addClass(c2);
  } 
  else if ($(node).hasClass(c2)) {
    $(node).removeClass(c2).addClass(c1);
  } 
}


/**
 * loads child terms and appends html to list
 * adds treeview, weighting etc. js to inserted child list
 */
Drupal.TaxonomyManagerTree.prototype.loadChildForm = function(li, update, callback) {
  var tree = this;
  if ($(li).is(".has-children") || update == true) {
    $(li).removeClass("has-children");
    if (update) {
      $(li).children("ul").remove(); 
    }
    var parentId = Drupal.getTermId(li);
    var url = tree.childFormUrl +'/'+ this.treeId +'/'+ this.vocId +'/'+ parentId;
    var param = new Object();
    param['form_build_id'] = this.form_build_id;
    param['form_id'] = this.form_id;
    param['tree_id'] = this.treeId;
    param['form_parents'] = this.formParents;
    param['language'] = this.language;
    
    $.ajax({
      data: param, 
      type: "GET", 
      url: url,
      dataType: 'json',
      success: function(response, status) {
         $(li).append(response.data);
        var ul = $(li).children("ul");
        tree.attachTreeview(ul);
        tree.attachSiblingsForm(ul);
        tree.attachSelectAllChildren(ul);
      
        //only attach other features if enabled!
        var weight_settings = Drupal.settings.updateWeight || [];
        if (weight_settings['up']) {
          Drupal.attachUpdateWeightTerms(li);
        }
        var term_data_settings = Drupal.settings.termData || [];
        if (term_data_settings['url']) {
          Drupal.attachTermDataLinks(ul);
        }
        if (typeof(callback) == "function") {
          callback(li, tree);
        }
      }
    });
  } 
}

/**
 * function for reloading root tree elements
 */
Drupal.TaxonomyManagerTree.prototype.loadRootForm = function(tids) {
  var tree = this;
  var url = this.childFormUrl +'/'+ this.treeId +'/'+ this.vocId +'/0/';
  
  var param = new Object();
  param['form_build_id'] = this.form_build_id;
  param['form_id'] = this.form_id;
  param['tree_id'] = this.treeId;
  param['form_parents'] = this.formParents;
  param['language'] = this.language;
  param['terms_to_expand'] = tids; // can either be a single term id or concatinated ids
    
   $.ajax({
      data: param, 
      type: "GET", 
      url: url,
      dataType: 'json',
      success: function(response, status) {
        $('#'+ tree.treeId).html(response.data); 
        var ul = $('#'+ tree.treeId).children("ul");
        tree.attachTreeview(ul);
        tree.attachSiblingsForm(ul);
        tree.attachSelectAllChildren(ul);
        Drupal.attachUpdateWeightTerms(ul);
        Drupal.attachTermDataLinks(ul);
        if (tid) {
          var termLink = $("#"+ tree.treeId).find(":input[value="+ tid +"]").parent().find("a.term-data-link");
          Drupal.activeTermSwapHighlight(termLink);
        }

        var lang = $('#edit-'+ tree.treeId +'-language').val();
        if (lang != "" && lang != tree.langauge) {
          $(tree.div).parent().siblings("div.taxonomy-manager-tree-top").find("select.language-selector option[value="+ lang +"]").attr("selected", "selected");
        }
      }
  });
}


/**
 * adds link for loading next siblings terms, when click terms get loaded through ahah
 * adds all needed js like treeview, weightning, etc.. to new added terms
 */
Drupal.TaxonomyManagerTree.prototype.attachSiblingsForm = function(ul) {
  var tree = this;
  var url = this.siblingsFormUrl;

  var list = "li.has-more-siblings div.term-has-more-siblings";
  if (ul) {
    list = $(ul).find(list);
  }
  
  $(list).bind('click', function() {
    $(this).unbind("click");
    var li = this.parentNode;
    var all = $('li', li.parentNode);
    var currentIndex = all.index(li);

    var page = Drupal.getPage(li);
    var prev_id = Drupal.getTermId(li);
    var parentId = Drupal.getParentId(li);
    
    url += '/'+ tree.treeId +'/'+ page +'/'+ prev_id +'/'+ parentId;
    
    var param = new Object();
    param['form_build_id'] = tree.form_build_id;
    param['form_id'] = tree.form_id;
    param['tree_id'] = tree.treeId;
    param['form_parents'] = tree.formParents;
    param['language'] = tree.language;
    
    $.ajax({
      data: param, 
      type: "GET", 
      url: url,
      dataType: 'json',
      success: function(response, status) {
        $(list).remove();
        $(li).after(response.data);
        tree.attachTreeview($('li', li.parentNode), currentIndex);
        tree.attachSelectAllChildren($('li', li.parentNode), currentIndex);
      
        //only attach other features if enabled!
        var weight_settings = Drupal.settings.updateWeight || [];
        if (weight_settings['up']) {
          Drupal.attachUpdateWeightTerms($('li', li.parentNode), currentIndex);
        }
        var term_data_settings = Drupal.settings.termData || [];
        if (term_data_settings['url']) {
          Drupal.attachTermDataToSiblings($('li', li.parentNode), currentIndex);
        }
      
        $(li).removeClass("last").removeClass("has-more-siblings");
        $(li).children().children('.term-operations').hide();
        tree.swapClasses(li, "lastExpandable", "expandable");
        tree.attachSiblingsForm($(li).parent());
      }
    });
  });
}


/**
 * helper function for getting out the current page
 */
Drupal.getPage = function(li) { 
  return $(li).find("input:hidden[class=page]").attr("value");
}


/**
 * returns terms id of a given list element
 */
Drupal.getTermId = function(li) {
  return $(li).children().children("input:hidden[class=term-id]").attr("value");
}

/**
 * return term id of a prent of a given list element
 * if no parent exists (root level), returns 0
 */
Drupal.getParentId = function(li) {
  var parentId;
  try {
    var parentLi = $(li).parent("ul").parent("li");
    parentId = Drupal.getTermId(parentLi);
  } catch(e) {
    return 0;
  }
  return parentId;
}

/**
 * update classes for tree view, if list elements get swaped
 */
Drupal.updateTree = function(upTerm, downTerm) {  
  if ($(upTerm).is(".last")) {
    $(upTerm).removeClass("last");
    Drupal.updateTreeDownTerm(downTerm); 
  }
  else if ($(upTerm).is(".lastExpandable")) {
    $(upTerm).removeClass("lastExpandable").addClass("expandable");
    Drupal.updateTreeDownTerm(downTerm); 
  }
  else if ($(upTerm).is(".lastCollapsable")) {
    $(upTerm).removeClass("lastCollapsable").addClass("collapsable");
    Drupal.updateTreeDownTerm(downTerm);  
  }
}

/**
 * update classes for tree view for a list element moved downwards
 */
Drupal.updateTreeDownTerm = function(downTerm) {
  if ($(downTerm).is(".expandable")) {
    $(downTerm).removeClass("expandable").addClass("lastExpandable");
  }
  else if ($(downTerm).is(".collapsable")) {
    $(downTerm).removeClass("collapsable").addClass("lastCollapsable");
  }
  else {
    $(downTerm).addClass("last");
  }
}

/**
 * Adds button next to parent term to select all available child checkboxes
 */
Drupal.TaxonomyManagerTree.prototype.attachSelectAllChildren = function(parent, currentIndex) {
  var tree = this;
  if (currentIndex) {
    parent = $(parent).slice(currentIndex);
  }
  $(parent).find('span.select-all-children').click(function() {
    tree.SelectAllChildrenToggle(this);
  });
}

/**
 * (un-)selects nested checkboxes
 */
Drupal.TaxonomyManagerTree.prototype.SelectAllChildrenToggle = function(span) {
  var tree = this;
  if ($(span).hasClass("select-all-children")) {
    var li = $(span).parents("li:first");
    if ($(li).hasClass("has-children")) {
      this.loadChildForm(li, true, function(li, tree1) {
        tree.swapClasses(li, "expandable", "collapsable");
        tree.swapClasses(li, "lastExpandable", "lastCollapsable");
        var this_span = $(li).find('span.select-all-children:first');
        tree.SelectAllChildrenToggle(this_span);
        return;
      });
    }
    else {
      $(span).removeClass("select-all-children").addClass("unselect-all-children");
      $(span).attr("title", Drupal.t("Unselect all children"));
      $(span).parents("li:first").find('ul:first').each(function() {
        var first_element = $(this).find('.term-line:first');
        $(first_element).parent().siblings("li").find('div.term-line:first :checkbox').attr('checked', true);
        $(first_element).find(' :checkbox').attr('checked', true);
      });
    }
  }
  else {
    $(span).removeClass("unselect-all-children").addClass("select-all-children");
    $(span).parents(".term-line").siblings("ul").find(':checkbox').attr("checked", false);
    $(span).attr("title", Drupal.t("Select all children"));
  }
}

/**
 * language selector
 */
Drupal.TaxonomyManagerTree.prototype.attachLanguageSelector = function() {
  var tree = this;
  var selector = $(tree.div).parent().siblings("div.taxonomy-manager-tree-top").find("select.language-selector");
  $(selector).not(".selector-processed").change(function() {
    tree.language = $(this).val();
    tree.loadRootForm();
  });
  $(selector).addClass("selector-processed");
  
}
Drupal.TaxonomyManagerTree.prototype.getLanguage = function() {
  var lang = $('#edit-taxonomy-manager-top-language').val();
  if (typeof(lang) == "undefined") {
    return "";
  }
  return lang;
}

/**
 * return array of selected terms
 */
Drupal.TaxonomyManagerTree.prototype.getSelectedTerms = function() {
  var terms = new Array();
  $(this.div).find("input[type=checkbox][checked]").each(function() {
    var term = $(this).parents("li").eq(0);
    terms.push(term);
  });
  return terms;
}

/**
 * returns li node for a given term id, if it exists in the tree
 */
Drupal.TaxonomyManagerTree.prototype.getLi = function(termId) {
  return $(this.div).find("input:hidden[class=term-id][value="+ termId +"]").parent().parent();
}

Drupal.TaxonomyManagerTree.prototype.attachMsgCloseLink = function() {
  $('#'+ this.form_id.replace(/_/g, '-')).find('div.messages').once(function() {
    $('<span>&nbsp;</span><a href="">'+ Drupal.t('Close') +'</a>').appendTo(this).click(function() {
      $(this).parent().fadeOut('fast', function() {
        $(this).remove();
      });
      return false;
    });
    // Remove the message after some time...
    $(this).parent().delay(5000).fadeOut('slow', function() {
      $(this).remove();
    });
  });
}

/**
 * attaches a throbber element to the taxonomy manager
 */
Drupal.attachThrobber = function() {
  var div = $('#taxonomy-manager');
  var throbber = $('<img src="'+ Drupal.settings.taxonomy_manager['modulePath'] +'images/ajax-loader.gif" alt="" height="25">');
  throbber.appendTo("#taxonomy-manager-toolbar-throbber").hide();
  throbber.ajaxStart(function(){
      $(this).show();
    })
    .ajaxStop(function(){
      $(this).hide();
    });
}

/**
* makes the div resizeable
*/
Drupal.attachResizeableTreeDiv = function() {
  $('img.div-grippie').each(function() {
    var staticOffset = null;
    var div = $(this).parents("fieldset").parent(); 
    $(this).mousedown(startDrag);  
  
    function startDrag(e) {
      staticOffset = div.width() - e.pageX;
      div.css('opacity', 0.5);
      $(document).mousemove(performDrag).mouseup(endDrag);
      return false;
    }
 
    function performDrag(e) {
      div.width(Math.max(200, staticOffset + e.pageX) + 'px');
      return false;
    }
 
    function endDrag(e) {
      $(document).unbind("mousemove", performDrag).unbind("mouseup", endDrag);
      div.css('opacity', 1);
    }
  });
}


})(jQuery);

