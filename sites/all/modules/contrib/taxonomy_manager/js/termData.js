// $Id: termData.js,v 1.1.2.3.2.8.2.13.2.7 2010/11/10 14:38:13 mh86 Exp $

/**
 * @file js support for term editing form for ajax saving and tree updating
 */


(function ($) {
  
//global var that holds the current term link object
var active_term = new Object();

/** 
 * attaches term data form, used after 'Saves changes' submit
 */
Drupal.behaviors.TaxonomyManagerTermData = {
  attach: function(context) {
    if (!$('#taxonomy-term-data-replace').hasClass('processed')) {
      $('#taxonomy-term-data-replace').addClass('processed');
      Drupal.attachTermDataForm();
    }
  }
}

/**
 * attaches Term Data functionality, called by tree.js
 */
Drupal.attachTermData = function(ul) {
  Drupal.attachTermDataLinks(ul);
}

/**
 * adds click events to the term links in the tree structure
 */
Drupal.attachTermDataLinks = function(ul) {
  $(ul).find('a.term-data-link').click(function() {
    Drupal.activeTermSwapHighlight(this);
    var li = $(this).parents("li:first");
    new Drupal.TermData(Drupal.getTermId(li)).load();
    return false;
  });
}

/**
 * attaches click events to next siblings
 */
Drupal.attachTermDataToSiblings = function(all, currentIndex) {
  var nextSiblings = $(all).slice(currentIndex);
  $(nextSiblings).find('a.term-data-link').click(function() {
    var li = $(this).parents("li:first");
    new Drupal.TermData(Drupal.getTermId(li)).load();
    return false;
  });
}

/**
 * adds click events to term data form, which is already open, when page gets loaded
 */
Drupal.attachTermDataForm = function() {
  var tid = $('#taxonomy-term-data').find('input:hidden[name="tid"]').val();
  if (tid) {
    var termLink = $('input[class="term-id"][value="'+ tid +'"]').parent().find("a.term-data-link");
    Drupal.activeTermSwapHighlight(termLink);
    new Drupal.TermData(tid).form();
  }  
}

/**
 * TermData Object
 */
Drupal.TermData = function(tid) {
  this.tid = tid; 
  this.div = $('#taxonomy-term-data');
  this.tidField = $('#edit-load-tid');
  this.tidFieldSubmit = $('#edit-load-tid-submit');
}


/**
 * loads term data form and displays it on the right side
 */
Drupal.TermData.prototype.load = function() {
  // Triggers an AJAX button
  $(this.tidField).val(this.tid);
  $(this.tidFieldSubmit).click();
}

/**
 * adds events to possible operations
 */
Drupal.TermData.prototype.form = function() {
  var termdata = this;
  
  $(this.div).find('#term-data-close span').click(function() {
    termdata.div.children().hide();
  });
  
  $(this.div).find('a.taxonomy-term-data-name-link').click(function() {
    var tid = this.href.split("/").pop();
    new Drupal.TermData(tid).load();
    return false;
  });
  
  $(this.div).find("legend").each(function() {
    var staticOffsetX, staticOffsetY = null;
    var left, top = 0;
    var div = termdata.div; 
    var pos = $(div).position();
    $(this).mousedown(startDrag);  
  
    function startDrag(e) {
      if (staticOffsetX == null && staticOffsetY == null) {
        staticOffsetX = e.pageX;
        staticOffsetY = e.pageY;
      }
      $(document).mousemove(performDrag).mouseup(endDrag);
      return false;
    }
 
    function performDrag(e) {
      left = e.pageX - staticOffsetX;
      top = e.pageY - staticOffsetY;
      $(div).css({position: "absolute", "left": pos.left + left +"px", "top": pos.top + top +"px"});
      return false;
    }
 
    function endDrag(e) {
      $(document).unbind("mousemove", performDrag).unbind("mouseup", endDrag);
    }
  });
}

/**
* hightlights current term
*/
Drupal.activeTermSwapHighlight = function(link) {
  try {
    $(active_term).parent().removeClass('highlightActiveTerm');
  } catch(e) {}
  active_term = link;
  $(active_term).parent().addClass('highlightActiveTerm');
}

})(jQuery);
