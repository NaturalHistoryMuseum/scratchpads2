
/**
 * @file js for changing weights of terms with Up and Down arrows
 */

(function ($) {

//object to store weights (tid => weight)
var weights = new Object();

Drupal.behaviors.TaxonomyManagerWeights = {
  attach: function(context, settings) {
    var weightSettings = settings.updateWeight || [];
    if (!$('#taxonomy-manager-toolbar.tm-weights-processed').length) {
      $('#taxonomy-manager-toolbar').addClass('tm-weights-processed');
      Drupal.attachUpdateWeightToolbar(weightSettings['up'], weightSettings['down']);
      Drupal.attachUpdateWeightTerms();   
    }  
  }
}

/**
 * adds click events for Up and Down buttons in the toolbar, which
 * allow the moving of selected (can be more) terms
 */
Drupal.attachUpdateWeightToolbar = function(upButton, downButton) {
  var selected;
  var url = Drupal.settings.updateWeight['url'];  
  
  $('#'+ upButton).click(function() {
    selected = Drupal.getSelectedTerms();
    for (var i=0; i < selected.length; i++) {
      var upTerm = selected[i];
      var downTerm = $(upTerm).prev(); 
    
      Drupal.orderTerms(upTerm, downTerm);
    }
    if (selected.length > 0) {
      $.post(url, weights);
    }
  });
  
  
  $('#'+ downButton).click(function() {
    selected = Drupal.getSelectedTerms();
    for (var i=selected.length-1; i >= 0; i--) {
      var downTerm = selected[i];
      var upTerm = $(downTerm).next();
      
      Drupal.orderTerms(upTerm, downTerm);
    }
    if (selected.length > 0) {
      $.post(url, weights);
    }
  });
}

/**
 * adds small up and down arrows to each term
 * arrows get displayed on mouseover
 */
Drupal.attachUpdateWeightTerms = function(parent, currentIndex) {
  var settings = Drupal.settings.updateWeight || [];
  var disable = settings['disable_mouseover'];
 	 
  if (!disable) {
    var url = Drupal.settings.updateWeight['url'];
  
    var termLineClass = 'div.term-line';
    var termUpClass = 'img.term-up';
    var termDownClass = 'img.term-down';
  
    if (parent && currentIndex) {
      parent = $(parent).slice(currentIndex);
    }
    if (parent) {
      termLineClass = $(parent).find(termLineClass);
      termUpClass = $(parent).find(termUpClass);
      termDownClass = $(parent).find(termDownClass);
    }
  
    $(termLineClass).mouseover(function() {
      $(this).find('div.term-operations').show(); 
    });
  
    $(termLineClass).mouseout(function() {
      $(this).find('div.term-operations').hide(); 
    });
  
    $(termUpClass).click(function() {
      var upTerm = $(this).parents("li").eq(0);
      var downTerm = $(upTerm).prev(); 
    
      Drupal.orderTerms(upTerm, downTerm);
      $.post(url, weights);
    
      $(downTerm).find(termLineClass).unbind('mouseover');
      setTimeout(function() {
        $(upTerm).find('div.term-operations').hide();
        $(downTerm).find(termLineClass).mouseover(function() {
          $(this).find('div.term-operations').show();
        });
      }, 1500);
    
    });
  
  
    $(termDownClass).click(function() {
      var downTerm = $(this).parents("li").eq(0);
      var upTerm = $(downTerm).next();
    
      Drupal.orderTerms(upTerm, downTerm);
      $.post(url, weights);
    
      $(upTerm).find(termLineClass).unbind('mouseover');
      setTimeout(function() {
        $(downTerm).find('div.term-operations').hide();
        $(upTerm).find(termLineClass).mouseover(function() {
          $(this).find('div.term-operations').show();
        });
      }, 1500);
    
    });
  }

}

/**
 * return array of selected terms
 */
Drupal.getSelectedTerms = function() {
  var terms = new Array();
  $('.treeview').find("input:checked").each(function() {
    var term = $(this).parents("li").eq(0);
    terms.push(term);
  });
  
  return terms;
}

/**
 * reorders terms
 *   - swap list elements in DOM
 *   - post updated weights to callback in php
 *   - update classes of tree view
 */
Drupal.orderTerms = function(upTerm, downTerm) {
  try {
    Drupal.getTermId(upTerm);
    Drupal.swapTerms(upTerm, downTerm);
    Drupal.swapWeights(upTerm, downTerm);
    Drupal.updateTree(upTerm, downTerm);
  } catch(e) {
    //no next item, because term to update is last child, continue
  }
}

/**
 * simple swap of two elements
 */
Drupal.swapTerms = function(upTerm, downTerm) { 
  $(upTerm).after(downTerm);
  $(downTerm).before(upTerm);
}

/**
 * updating weights of swaped terms
 * if two terms have different weights, then weights are being swapped
 * else, if both have same weights, upTerm gets decreased
 *
 * if prev/next siblings of up/down terms have same weights as current
 * swapped, they have to be updated by de/increasing weight (by 1) to ensure
 * unique position of swapped terms
 */
Drupal.swapWeights = function(upTerm, downTerm) {
  var upWeight = Drupal.getWeight(upTerm);
  var downWeight = Drupal.getWeight(downTerm);
  var downTid = Drupal.getTermId(downTerm);
  var upTid = Drupal.getTermId(upTerm);
  
  //same weight, decrease upTerm
  if (upWeight == downWeight) {
    weights[upTid] = --upWeight;
  }
  //different weights, swap
  else {
    weights[upTid] = downWeight;
    weights[downTid] = upWeight;
  }
  
  //update prev siblings if necessary
  try {
    if (Drupal.getWeight($(upTerm).prev()) >= upWeight) {
      $(upTerm).prevAll().each(function() {
        var id = Drupal.getTermId(this);
        var weight = Drupal.getWeight(this);
        weights[id] = --weight;
      });
    }
  } catch(e) {
    //no prev
  }
  
  //update next siblings if necessary
  try {
    if (Drupal.getWeight($(downTerm).next()) <= downWeight) {
      $(downTerm).nextAll().each(function() {
        var id = Drupal.getTermId(this);
        var weight = Drupal.getWeight(this);
        weights[id] = ++weight;
      });
    }
  } catch(e) {
    //no next
  }

}

/**
 * helper to return weight of a term
 */
Drupal.getWeight = function(li) {
  var id = Drupal.getTermId(li);
  var weight;
  
  if (weights[id] != null) {
    weight = weights[id];
  }
  else {
    weight = $(li).find("input:hidden[class=weight-form]").attr("value");
  }
  
  return weight;
}

})(jQuery);
