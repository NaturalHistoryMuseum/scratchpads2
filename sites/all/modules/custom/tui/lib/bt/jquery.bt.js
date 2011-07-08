/*
 * @name BeautyTips
 * @desc a tooltips/baloon-help plugin for jQuery
 *
 * @author Jeff Robbins - Lullabot - http://www.lullabot.com
 * @version 0.9.1  (2/15/2009)
 *  
 * @type jQuery
 * @cat Plugins/bt
 * @requires jQuery v1.2+ (not tested on versions prior to 1.2.6)
 *
 * Dual licensed under the MIT and GPL licenses:
 * http://www.opensource.org/licenses/mit-license.php
 * http://www.gnu.org/licenses/gpl.html
 *
 * Encourage development. If you use BeautyTips for anything cool 
 * or on a site that people have heard of, please drop me a note.
 * - jeff ^at lullabot > com
 *
 * No guarantees, warranties, or promises of any kind
 *
 */

/**
 * @credit Inspired by Karl Swedberg's ClueTip
 *    (http://plugins.learningjquery.com/cluetip/), which in turn was inspired
 *    by Cody Lindley's jTip (http://www.codylindley.com)
 *
 * @fileoverview
 * Beauty Tips is a jQuery tooltips plugin which uses the canvas drawing element
 * in the HTML5 spec in order to dynamically draw tooltip "talk bubbles" around
 * the descriptive help text associated with an item. This is in many ways
 * similar to Google Maps which both provides similar talk-bubbles and uses the
 * canvas element to draw them.
 *
 * The canvas element is supported in modern versions of FireFox, Safari, and
 * Opera. However, Internet Explorer needs a separate library called ExplorerCanvas
 * included on the page in order to support canvas drawing functions. ExplorerCanvas
 * was created by Google for use with their web apps and you can find it here:
 * http://excanvas.sourceforge.net/
 *
 * Beauty Tips was written to be simple to use and pretty. All of its options
 * are documented at the bottom of this file and defaults can be overwritten
 * globally for the entire page, or individually on each call.
 *
 * By default each tooltip will be positioned on the side of the target element
 * which has the most free space. This is affected by the scroll position and
 * size of the current window, so each Beauty Tip is redrawn each time it is
 * displayed. It may appear above an element at the bottom of the page, but when
 * the page is scrolled down (and the element is at the top of the page) it will
 * then appear below it. Additionally, positions can be forced or a preferred
 * order can be defined. See examples below.
 *
 * To fix z-index problems in IE6, include the bgiframe plugin on your page
 * http://plugins.jquery.com/project/bgiframe - BeautyTips will automatically
 * recognize it and use it.
 *
 * BeautyTips also works with the hoverIntent plugin
 * http://cherne.net/brian/resources/jquery.hoverIntent.html
 * see hoverIntent example below for usage
 *
 * Usage
 * The function can be called in a number of ways.
 * $(selector).bt();
 * $(selector).bt('Content text');
 * $(selector).bt('Content text', {option1: value, option2: value});
 * $(selector).bt({option1: value, option2: value});
 *
 * For more/better documentation and lots of examples, visit the demo page included with the distribution
 *
 */
jQuery.fn.bt = function(content, options) {

  if (typeof content != 'string') {
    var contentSelect = true;
    options = content;
    content = false;
  }
  else {
    var contentSelect = false;
  }
  
  // if hoverIntent is installed, use that as default instead of hover
  if (jQuery.fn.hoverIntent && jQuery.bt.defaults.trigger == 'hover') {
    jQuery.bt.defaults.trigger = 'hoverIntent';
  }

  return this.each(function(index) {

    var opts = jQuery.extend(false, jQuery.bt.defaults, options);

    // clean up the options
    opts.spikeLength = numb(opts.spikeLength);
    opts.spikeGirth = numb(opts.spikeGirth);
    opts.overlap = numb(opts.overlap);
    
    var ajaxTimeout = false;
    
    /**
     * This is sort of the "starting spot" for the this.each()
     * These are sort of the init functions to handle the call
     */

    if (opts.killTitle) {
      $(this).find('[title]').andSelf().each(function() {
        if (!$(this).attr('bt-xTitle')) {
          $(this).attr('bt-xTitle', $(this).attr('title')).attr('title', '');
        }
      });
    }    
    
    if (typeof opts.trigger == 'string') {
      opts.trigger = [opts.trigger];
    }
    if (opts.trigger[0] == 'hoverIntent') {
      var hoverOpts = $.extend(opts.hoverIntentOpts, {
        over: function() {
          this.btOn();
        },
        out: function() {
          this.btOff();
        }});
      $(this).hoverIntent(hoverOpts);
    
    }
    else if (opts.trigger[0] == 'hover') {
      $(this).hover(
        function() {
          this.btOn();
        },
        function() {
          this.btOff();
        }
      );
    }
    else if (opts.trigger[0] == 'now') {
      // toggle the on/off right now
      // note that 'none' gives more control (see below)
      if ($(this).hasClass('bt-active')) {
        this.btOff();
      }
      else {
        this.btOn();
      }
    }
    else if (opts.trigger[0] == 'none') {
      // initialize the tip with no event trigger
      // use javascript to turn on/off tip as follows:
      // $('#selector').btOn();
      // $('#selector').btOff();
    }
    else if (opts.trigger.length > 1 && opts.trigger[0] != opts.trigger[1]) {
      $(this)
        .bind(opts.trigger[0], function() {
          this.btOn();
        })
        .bind(opts.trigger[1], function() {
          this.btOff();
        });
    }
    else {
      // toggle using the same event
      $(this).bind(opts.trigger[0], function() {
        if ($(this).hasClass('bt-active')) {
          this.btOff();
        }
        else {
          this.btOn();
        }
      });
    }
    
    
    /**
     *  The BIG TURN ON
     *  Any element that has been initiated
     */
    this.btOn = function () {
      if (typeof $(this).data('bt-box') == 'object') {
        // if there's already a popup, remove it before creating a new one.
        this.btOff();
      }

      // trigger preShow function
      opts.preShow.apply(this);
      
      // turn off other tips
      $(jQuery.bt.vars.closeWhenOpenStack).btOff();
      
      // add the class to the target element (for hilighting, for example)
      // bt-active is always applied to all, but activeClass can apply another
      $(this).addClass('bt-active ' + opts.activeClass);

      if (contentSelect && opts.ajaxPath == null) {
        // bizarre, I know
        if (opts.killTitle) {
          // if we've killed the title attribute, it's been stored in 'bt-xTitle' so get it..
          $(this).attr('title', $(this).attr('bt-xTitle'));
        }
        // then evaluate the selector... title is now in place
        content = eval(opts.contentSelector);
        if (opts.killTitle) {
          // now remove the title again, so we don't get double tips
          $(this).attr('title', '');
        }
      }
      
      // ----------------------------------------------
      // All the Ajax(ish) stuff is in this next bit...
      // ----------------------------------------------
      if (opts.ajaxPath != null && content == false) {
        if (typeof opts.ajaxPath == 'object') {
          var url = eval(opts.ajaxPath[0]);
          url += opts.ajaxPath[1] ? ' ' + opts.ajaxPath[1] : '';
        }
        else {
          var url = opts.ajaxPath;
        }
        var off = url.indexOf(" ");
    		if ( off >= 0 ) {
    			var selector = url.slice(off, url.length);
    			url = url.slice(0, off);
    		}
      
        // load any data cached for the given ajax path
        var cacheData = opts.ajaxCache ? $(document.body).data('btCache-' + url.replace(/\./g, '')) : null;
        if (typeof cacheData == 'string') {
          content = selector ? jQuery("<div/>").append(cacheData.replace(/<script(.|\s)*?\/script>/g, "")).find(selector) : cacheData;
        }
        else {
          var target = this;
                    
          // set up the options
          var ajaxOpts = jQuery.extend(false,
          {
            type: opts.ajaxType,
            data: opts.ajaxData,
            cache: opts.ajaxCache,
            url: url,
            complete: function(XMLHttpRequest, textStatus) {
              if (textStatus == 'success' || textStatus == 'notmodified') {
                if (opts.ajaxCache) {
                  $(document.body).data('btCache-' + url.replace(/\./g, ''), XMLHttpRequest.responseText);
                }
                ajaxTimeout = false;
                content = selector ?
      						// Create a dummy div to hold the results
      						jQuery("<div/>")
      							// inject the contents of the document in, removing the scripts
      							// to avoid any 'Permission Denied' errors in IE
      							.append(XMLHttpRequest.responseText.replace(/<script(.|\s)*?\/script>/g, ""))
      
      							// Locate the specified elements
      							.find(selector) :
      
      						// If not, just inject the full result
      						XMLHttpRequest.responseText;
      				   
              }
              else {
                if (textStatus == 'timeout') {
                  // if there was a timeout, we don't cache the result
                  ajaxTimeout = true;
                }
                content = opts.ajaxError.replace(/%error/g, XMLHttpRequest.statusText);
              }
              // if the user rolls out of the target element before the ajax request comes back, don't show it
              if ($(target).hasClass('bt-active')) {
                target.btOn();
              }
            }
          }, opts.ajaxData);
          // do the ajax request
          $.ajax(ajaxOpts);
          // load the throbber while the magic happens
          content = opts.ajaxLoading;
        }
      }
      // </ ajax stuff >
      

      // now we start actually figuring out where to place the tip
      
      var offsetParent = $(this).offsetParent();
      var pos = $(this).btPosition();
      // top, left, width, and height values of the target element
      var top = numb(pos.top) + numb($(this).css('margin-top')); // IE can return 'auto' for margins
      var left = numb(pos.left) + numb($(this).css('margin-left'));
      var width = $(this).outerWidth();
      var height = $(this).outerHeight();
      
      if (typeof content == 'object') {
        // if content is a DOM object (as opposed to text)
        // use a clone, rather than removing the original element
        // and ensure that it's visible
        content = $(content).clone(true).show();
        
      }
      
      // create the tip content div, populate it, and style it
      var $text = $('<div class="bt-content"></div>').append(content).css({padding: opts.padding, position: 'absolute', width: opts.width, zIndex: opts.textzIndex}).css(opts.cssStyles);
      // create the wrapping box which contains text and canvas
      // put the content in it, style it, and append it to the same offset parent as the target
      var $box = $('<div class="bt-wrapper"></div>').append($text).addClass(opts.cssClass).css({position: 'absolute', width: opts.width, zIndex: opts.wrapperzIndex}).appendTo(offsetParent);
      
      // use bgiframe to get around z-index problems in IE6
      // http://plugins.jquery.com/project/bgiframe
      if ($.fn.bgiframe) {
        $text.bgiframe();
        $box.bgiframe();  
      }

      $(this).data('bt-box', $box);

      // see if the text box will fit in the various positions
      var scrollTop = numb($(document).scrollTop());
      var scrollLeft = numb($(document).scrollLeft());
      var docWidth = numb($(window).width());
      var docHeight = numb($(window).height());
      var winRight = scrollLeft + docWidth;
      var winBottom = scrollTop + docHeight;
      var space = new Object();
      space.top = $(this).offset().top - scrollTop;
      space.bottom = docHeight - (($(this).offset().top + height) - scrollTop);
      space.left = $(this).offset().left - scrollLeft;
      space.right = docWidth - (($(this).offset().left + width) - scrollLeft);
      var textOutHeight = numb($text.outerHeight());
      var textOutWidth = numb($text.outerWidth());
      if (opts.positions.constructor == String) {
        opts.positions = opts.positions.replace(/ /, '').split(',');
      }
      if (opts.positions[0] == 'most') {
        // figure out which is the largest
        var position = 'top'; // prime the pump
        for (var pig in space) { // pigs in space!
          position = space[pig] > space[position] ? pig : position;
        }
      }
      else {
        for (var x in opts.positions) {
          var position = opts.positions[x];
          if ((position == 'left' || position == 'right') && space[position] > textOutWidth + opts.spikeLength) {
            break;
          }
          else if ((position == 'top' || position == 'bottom') && space[position] > textOutHeight + opts.spikeLength) {
            break;
          }
        }
      }

      // horizontal (left) offset for the box
      var horiz = left + ((width - textOutWidth) * .5);
      // vertical (top) offset for the box
      var vert = top + ((height - textOutHeight) * .5);
      var animDist = opts.animate ? numb(opts.distance) : 0;
      var points = new Array();
      var textTop, textLeft, textRight, textBottom, textTopSpace, textBottomSpace, textLeftSpace, textRightSpace, crossPoint, textCenter, spikePoint;

      // Yes, yes, this next bit really could use to be condensed
      // each switch case is basically doing the same thing in slightly different ways
      switch(position) {
        case 'top':
          // spike on bottom
          $text.css('margin-bottom', opts.spikeLength + 'px');
          $box.css({top: (top - $text.outerHeight(true) - animDist) + opts.overlap, left: horiz});
          // move text left/right if extends out of window
          textRightSpace = (winRight - opts.windowMargin) - ($text.offset().left + $text.outerWidth(true));
          var xShift = 0;
          if (textRightSpace < 0) {
            // shift it left
            $box.css('left', (numb($box.css('left')) + textRightSpace) + 'px');
            xShift -= textRightSpace;
          }
          // we test left space second to ensure that left of box is visible
          textLeftSpace = ($text.offset().left + numb($text.css('margin-left'))) - (scrollLeft + opts.windowMargin);
          if (textLeftSpace < 0) {
            // shift it right
            $box.css('left', (numb($box.css('left')) - textLeftSpace) + 'px');
            xShift += textLeftSpace;
          }
          textTop = $text.btPosition().top + numb($text.css('margin-top'));
          textLeft = $text.btPosition().left + numb($text.css('margin-left'));
          textRight = textLeft + $text.outerWidth();
          textBottom = textTop + $text.outerHeight();
          textCenter = {x: textLeft + ($text.outerWidth()*opts.centerPointX), y: textTop + ($text.outerHeight()*opts.centerPointY)};
          // points[points.length] = {x: x, y: y};
          points[points.length] = spikePoint = {y: textBottom + opts.spikeLength, x: ((textRight-textLeft) * .5) + xShift, type: 'spike'};
          crossPoint = findIntersectX(spikePoint.x, spikePoint.y, textCenter.x, textCenter.y, textBottom);
          // make sure that the crossPoint is not outside of text box boundaries
          crossPoint.x = crossPoint.x < textLeft + opts.spikeGirth/2 + opts.cornerRadius ? textLeft + opts.spikeGirth/2 + opts.cornerRadius : crossPoint.x;
          crossPoint.x =  crossPoint.x > (textRight - opts.spikeGirth/2) - opts.cornerRadius ? (textRight - opts.spikeGirth/2) - opts.CornerRadius : crossPoint.x;
          points[points.length] = {x: crossPoint.x - (opts.spikeGirth/2), y: textBottom, type: 'join'};
          points[points.length] = {x: textLeft, y: textBottom, type: 'corner'};  // left bottom corner
          points[points.length] = {x: textLeft, y: textTop, type: 'corner'};     // left top corner
          points[points.length] = {x: textRight, y: textTop, type: 'corner'};    // right top corner
          points[points.length] = {x: textRight, y: textBottom, type: 'corner'}; // right bottom corner
          points[points.length] = {x: crossPoint.x + (opts.spikeGirth/2), y: textBottom, type: 'join'};
          points[points.length] = spikePoint;
          break;
        case 'left':
          // spike on right
          $text.css('margin-right', opts.spikeLength + 'px');
          $box.css({top: vert + 'px', left: ((left - $text.outerWidth(true) - animDist) + opts.overlap) + 'px'});
          // move text up/down if extends out of window
          textBottomSpace = (winBottom - opts.windowMargin) - ($text.offset().top + $text.outerHeight(true));
          var yShift = 0;
          if (textBottomSpace < 0) {
            // shift it up
            $box.css('top', (numb($box.css('top')) + textBottomSpace) + 'px');
            yShift -= textBottomSpace;
          }
          // we ensure top space second to ensure that top of box is visible
          textTopSpace = ($text.offset().top + numb($text.css('margin-top'))) - (scrollTop + opts.windowMargin);
          if (textTopSpace < 0) {
            // shift it down
            $box.css('top', (numb($box.css('top')) - textTopSpace) + 'px');
            yShift += textTopSpace;
          }
          textTop = $text.btPosition().top + numb($text.css('margin-top'));
          textLeft = $text.btPosition().left + numb($text.css('margin-left'));
          textRight = textLeft + $text.outerWidth();
          textBottom = textTop + $text.outerHeight();
          textCenter = {x: textLeft + ($text.outerWidth()*opts.centerPointX), y: textTop + ($text.outerHeight()*opts.centerPointY)};
          points[points.length] = spikePoint = {x: textRight + opts.spikeLength, y: ((textBottom-textTop) * .5) + yShift, type: 'spike'};
          crossPoint = findIntersectY(spikePoint.x, spikePoint.y, textCenter.x, textCenter.y, textRight);
          // make sure that the crossPoint is not outside of text box boundaries
          crossPoint.y = crossPoint.y < textTop + opts.spikeGirth/2 + opts.cornerRadius ? textTop + opts.spikeGirth/2 + opts.cornerRadius : crossPoint.y;
          crossPoint.y =  crossPoint.y > (textBottom - opts.spikeGirth/2) - opts.cornerRadius ? (textBottom - opts.spikeGirth/2) - opts.cornerRadius : crossPoint.y;
          points[points.length] = {x: textRight, y: crossPoint.y + opts.spikeGirth/2, type: 'join'};
          points[points.length] = {x: textRight, y: textBottom, type: 'corner'}; // right bottom corner
          points[points.length] = {x: textLeft, y: textBottom, type: 'corner'};  // left bottom corner
          points[points.length] = {x: textLeft, y: textTop, type: 'corner'};     // left top corner
          points[points.length] = {x: textRight, y: textTop, type: 'corner'};    // right top corner
          points[points.length] = {x: textRight, y: crossPoint.y - opts.spikeGirth/2, type: 'join'};
          points[points.length] = spikePoint;
          break;
        case 'bottom':
          // spike on top
          $text.css('margin-top', opts.spikeLength + 'px');
          $box.css({top: (top + height + animDist) - opts.overlap, left: horiz});
          // move text up/down if extends out of window
          textRightSpace = (winRight - opts.windowMargin) - ($text.offset().left + $text.outerWidth(true));
          var xShift = 0;
          if (textRightSpace < 0) {
            // shift it left
            $box.css('left', (numb($box.css('left')) + textRightSpace) + 'px');
            xShift -= textRightSpace;
          }
          // we ensure left space second to ensure that left of box is visible
          textLeftSpace = ($text.offset().left + numb($text.css('margin-left')))  - (scrollLeft + opts.windowMargin);
          if (textLeftSpace < 0) {
            // shift it right
            $box.css('left', (numb($box.css('left')) - textLeftSpace) + 'px');
            xShift += textLeftSpace;
          }
          textTop = $text.btPosition().top + numb($text.css('margin-top'));
          textLeft = $text.btPosition().left + numb($text.css('margin-left'));
          textRight = textLeft + $text.outerWidth();
          textBottom = textTop + $text.outerHeight();
          textCenter = {x: textLeft + ($text.outerWidth()*opts.centerPointX), y: textTop + ($text.outerHeight()*opts.centerPointY)};
          points[points.length] = spikePoint = {x: ((textRight-textLeft) * .5) + xShift, y: 0, type: 'spike'};
          crossPoint = findIntersectX(spikePoint.x, spikePoint.y, textCenter.x, textCenter.y, textTop);
          // make sure that the crossPoint is not outside of text box boundaries
          crossPoint.x = crossPoint.x < textLeft + opts.spikeGirth/2 + opts.cornerRadius ? textLeft + opts.spikeGirth/2 + opts.cornerRadius : crossPoint.x;
          crossPoint.x =  crossPoint.x > (textRight - opts.spikeGirth/2) - opts.cornerRadius ? (textRight - opts.spikeGirth/2) - opts.cornerRadius : crossPoint.x;
          points[points.length] = {x: crossPoint.x + opts.spikeGirth/2, y: textTop, type: 'join'};
          points[points.length] = {x: textRight, y: textTop, type: 'corner'};    // right top corner
          points[points.length] = {x: textRight, y: textBottom, type: 'corner'}; // right bottom corner
          points[points.length] = {x: textLeft, y: textBottom, type: 'corner'};  // left bottom corner
          points[points.length] = {x: textLeft, y: textTop, type: 'corner'};     // left top corner
          points[points.length] = {x: crossPoint.x - (opts.spikeGirth/2), y: textTop, type: 'join'};
          points[points.length] = spikePoint;
          break;
        case 'right':
          // spike on left
          $text.css('margin-left', (opts.spikeLength + 'px'));
          $box.css({top: vert + 'px', left: ((left + width + animDist) - opts.overlap) + 'px'});
          // move text up/down if extends out of window
          textBottomSpace = (winBottom - opts.windowMargin) - ($text.offset().top + $text.outerHeight(true));
          var yShift = 0;
          if (textBottomSpace < 0) {
            // shift it up
            $box.css('top', (numb($box.css('top')) + textBottomSpace) + 'px');
            yShift -= textBottomSpace;
          }
          // we ensure top space second to ensure that top of box is visible
          textTopSpace = ($text.offset().top + numb($text.css('margin-top'))) - (scrollTop + opts.windowMargin);
          if (textTopSpace < 0) {
            // shift it down
            $box.css('top', (numb($box.css('top')) - textTopSpace) + 'px');
            yShift += textTopSpace;
          }
          textTop = $text.btPosition().top + numb($text.css('margin-top'));
          textLeft = $text.btPosition().left + numb($text.css('margin-left'));
          textRight = textLeft + $text.outerWidth();
          textBottom = textTop + $text.outerHeight();
          textCenter = {x: textLeft + ($text.outerWidth()*opts.centerPointX), y: textTop + ($text.outerHeight()*opts.centerPointY)};
          points[points.length] = spikePoint = {x: 0, y: ((textBottom-textTop) * .5) + yShift, type: 'spike'};
          crossPoint = findIntersectY(spikePoint.x, spikePoint.y, textCenter.x, textCenter.y, textLeft);
          // make sure that the crossPoint is not outside of text box boundaries
          crossPoint.y = crossPoint.y < textTop + opts.spikeGirth/2 + opts.cornerRadius ? textTop + opts.spikeGirth/2 + opts.cornerRadius : crossPoint.y;
          crossPoint.y =  crossPoint.y > (textBottom - opts.spikeGirth/2) - opts.cornerRadius ? (textBottom - opts.spikeGirth/2) - opts.cornerRadius : crossPoint.y;
          points[points.length] = {x: textLeft, y: crossPoint.y - opts.spikeGirth/2, type: 'join'};
          points[points.length] = {x: textLeft, y: textTop, type: 'corner'};     // left top corner
          points[points.length] = {x: textRight, y: textTop, type: 'corner'};    // right top corner
          points[points.length] = {x: textRight, y: textBottom, type: 'corner'}; // right bottom corner
          points[points.length] = {x: textLeft, y: textBottom, type: 'corner'};  // left bottom corner
          points[points.length] = {x: textLeft, y: crossPoint.y + opts.spikeGirth/2, type: 'join'};
          points[points.length] = spikePoint;
          break;
      } // </ switch >

      var canvas = $('<canvas width="'+ (numb($text.outerWidth(true)) + opts.strokeWidth*2) +'" height="'+ (numb($text.outerHeight(true)) + opts.strokeWidth*2) +'"></canvas>').appendTo($box).css({position: 'absolute', top: $text.btPosition().top, left: $text.btPosition().left, zIndex: opts.boxzIndex}).get(0);

      // if excanvas is set up, we need to initialize the new canvas element
      if (typeof G_vmlCanvasManager != 'undefined') {
        canvas = G_vmlCanvasManager.initElement(canvas);
      }

      if (opts.cornerRadius > 0) {
        // round the corners!
        var newPoints = new Array();
        var newPoint;
        for (var i=0; i<points.length; i++) {
          if (points[i].type == 'corner') {
            // create two new arc points
            // find point between this and previous (using modulo in case of ending)
            newPoint = betweenPoint(points[i], points[(i-1)%points.length], opts.cornerRadius);
            newPoint.type = 'arcStart';
            newPoints[newPoints.length] = newPoint;
            // the original corner point
            newPoints[newPoints.length] = points[i];
            // find point between this and next
            newPoint = betweenPoint(points[i], points[(i+1)%points.length], opts.cornerRadius);
            newPoint.type = 'arcEnd';
            newPoints[newPoints.length] = newPoint;
          }
          else {
            newPoints[newPoints.length] = points[i];
          }
        }
        // overwrite points with new version
        points = newPoints;

      }

      var ctx = canvas.getContext("2d");
      drawIt.apply(ctx, [points], opts.strokeWidth);
      ctx.fillStyle = opts.fill;
      if (opts.shadow) {
        ctx.shadowOffsetX = 2;
        ctx.shadowOffsetY = 2;
        ctx.shadowBlur = 5;
        ctx.shadowColor =  opts.shadowColor;
      }
      ctx.closePath();
      ctx.fill();
      if (opts.strokeWidth > 0) {
        ctx.shadowColor = 'rgba(0, 0, 0, 0)';
        ctx.lineWidth = opts.strokeWidth;
        ctx.strokeStyle = opts.strokeStyle;
        ctx.beginPath();
        drawIt.apply(ctx, [points], opts.strokeWidth);
        ctx.closePath();
        ctx.stroke();
      }

      if (opts.animate) {
        $box.css({opacity: 0.1});
      }

      $box.css({visibility: 'visible'});

      if (opts.overlay) {
        // EXPERIMENTAL!!!!
        var overlay = $('<div class="bt-overlay"></div>').css({
            position: 'absolute',
            backgroundColor: 'blue',
            top: top,
            left: left,
            width: width,
            height: height,
            opacity: '.2'
          }).appendTo(offsetParent);
        $(this).data('overlay', overlay);
      }

      var animParams = {opacity: 1};
      if (opts.animate) {
        switch (position) {
          case 'top':
            animParams.top = $box.btPosition().top + opts.distance;
            break;
          case 'left':
            animParams.left = $box.btPosition().left + opts.distance;
            break;
          case 'bottom':
            animParams.top = $box.btPosition().top - opts.distance;
            break;
          case 'right':
            animParams.left = $box.btPosition().left - opts.distance;
            break;
        }
        $box.animate(animParams, {duration: opts.speed, easing: opts.easing});
      }
      
      if ((opts.ajaxPath != null && opts.ajaxCache == false) || ajaxTimeout) {
        // if ajaxCache is not enabled or if there was a server timeout,
        // remove the content variable so it will be loaded again from server
        content = false;
      }
      
      // stick this element into the clickAnywhereToClose stack
      if (opts.clickAnywhereToClose) {
        jQuery.bt.vars.clickAnywhereStack.push(this);
        $(document).click(jQuery.bt.docClick);
      }
      
      // stick this element into the closeWhenOthersOpen stack
      if (opts.closeWhenOthersOpen) {
        jQuery.bt.vars.closeWhenOpenStack.push(this);
      }

      // trigger postShow function
      opts.postShow.apply(this);


    }; // </ turnOn() >

    this.btOff = function() {

      // trigger preHide function
      opts.preHide.apply(this);

      var box = $(this).data('bt-box');
      var overlay = $(this).data('bt-overlay');
      if (typeof box == 'object') {
        $(box).remove();
        $(this).removeData('bt-box');
      }
      if (typeof overlay == 'object') {
        $(overlay).remove();
        $(this).removeData('bt-overlay');
      }
      
      // remove this from the stacks
      jQuery.bt.vars.clickAnywhereStack = arrayRemove(jQuery.bt.vars.clickAnywhereStack, this);
      jQuery.bt.vars.closeWhenOpenStack = arrayRemove(jQuery.bt.vars.closeWhenOpenStack, this);

      // trigger postHide function
      opts.postHide.apply(this);
      
      // remove the 'bt-active' and activeClass classes from target
      $(this).removeClass('bt-active ' + opts.activeClass);

    }; // </ turnOff() >

    var refresh = this.btRefresh = function() {
      this.btOff();
      this.btOn();
    };


  }); // </ this.each() >


  function drawIt(points, strokeWidth) {
    this.moveTo(points[0].x, points[0].y);
    for (i=1;i<points.length;i++) {
      if (points[i-1].type == 'arcStart') {
        // if we're creating a rounded corner
        //ctx.arc(round5(points[i].x), round5(points[i].y), points[i].startAngle, points[i].endAngle, opts.cornerRadius, false);
        this.quadraticCurveTo(round5(points[i].x, strokeWidth), round5(points[i].y, strokeWidth), round5(points[(i+1)%points.length].x, strokeWidth), round5(points[(i+1)%points.length].y, strokeWidth));
        i++;
        //ctx.moveTo(round5(points[i].x), round5(points[i].y));
      }
      else {
        this.lineTo(round5(points[i].x, strokeWidth), round5(points[i].y, strokeWidth));
      }
    }
  }; // </ drawIt() >

  /**
   * For odd stroke widths, round to the nearest .5 pixel to avoid antialiasing
   * http://developer.mozilla.org/en/Canvas_tutorial/Applying_styles_and_colors
   */
  function round5(num, strokeWidth) {
    var ret;
    strokeWidth = numb(strokeWidth);
    if (strokeWidth%2) {
      ret = num;
    }
    else {
      ret = Math.round(num - .5) + .5;
    }
    return ret;
  }; // </ round5() >

  /**
   * Ensure that a number is a number... or zero
   */
  function numb(num) {
    return parseInt(num) || 0;
  }; // </ numb() >
  
  /**
   * Remove an element from an array
   */ 
  function arrayRemove(arr, elem) {
    var x, newArr = new Array();
    for (x in arr) {
      if (arr[x] != elem) {
        newArr.push(arr[x]);
      }
    }
    return newArr;
  }; // </ arrayRemove() >

  /**
   * Given two points, find a point which is dist pixels from point1 on a line to point2
   */
  function betweenPoint(point1, point2, dist) {
    // figure out if we're horizontal or vertical
    var y, x;
    if (point1.x == point2.x) {
      // vertical
      y = point1.y < point2.y ? point1.y + dist : point1.y - dist;
      return {x: point1.x, y: y};
    }
    else if (point1.y == point2.y) {
      // horizontal
      x = point1.x < point2.x ? point1.x + dist : point1.x - dist;
      return {x:x, y: point1.y};
    }
  }; // </ betweenPoint() >

  function centerPoint(arcStart, corner, arcEnd) {
    var x = corner.x == arcStart.x ? arcEnd.x : arcStart.x;
    var y = corner.y == arcStart.y ? arcEnd.y : arcStart.y;
    var startAngle, endAngle;
    if (arcStart.x < arcEnd.x) {
      if (arcStart.y > arcEnd.y) {
        // arc is on upper left
        startAngle = (Math.PI/180)*180;
        endAngle = (Math.PI/180)*90;
      }
      else {
        // arc is on upper right
        startAngle = (Math.PI/180)*90;
        endAngle = 0;
      }
    }
    else {
      if (arcStart.y > arcEnd.y) {
        // arc is on lower left
        startAngle = (Math.PI/180)*270;
        endAngle = (Math.PI/180)*180;
      }
      else {
        // arc is on lower right
        startAngle = 0;
        endAngle = (Math.PI/180)*270;
      }
    }
    return {x: x, y: y, type: 'center', startAngle: startAngle, endAngle: endAngle};
  }; // </ centerPoint() >

  /**
   * Find the intersection point of two lines, each defined by two points
   * arguments are x1, y1 and x2, y2 for r1 (line 1) and r2 (line 2)
   * It's like an algebra party!!!
   */
  function findIntersect(r1x1, r1y1, r1x2, r1y2, r2x1, r2y1, r2x2, r2y2) {

    if (r2x1 == r2x2) {
      return findIntersectY(r1x1, r1y1, r1x2, r1y2, r2x1);
    }
    if (r2y1 == r2y2) {
      return findIntersectX(r1x1, r1y1, r1x2, r1y2, r2y1);
    }

    // m = (y1 - y2) / (x1 - x2)  // <-- how to find the slope
    // y = mx + b                 // the 'classic' linear equation
    // b = y - mx                 // how to find b (the y-intersect)
    // x = (y - b)/m              // how to find x
    var r1m = (r1y1 - r1y2) / (r1x1 - r1x2);
    var r1b = r1y1 - (r1m * r1x1);
    var r2m = (r2y1 - r2y2) / (r2x1 - r2x2);
    var r2b = r2y1 - (r2m * r2x1);

    var x = (r2b - r1b) / (r1m - r2m);
	  var y = r1m * x + r1b;

	  return {x: x, y: y};
  }; // </ findIntersect() >

  /**
   * Find the y intersection point of a line and given x vertical
   */
  function findIntersectY(r1x1, r1y1, r1x2, r1y2, x) {
    if (r1y1 == r1y2) {
      return {x: x, y: r1y1};
    }
    var r1m = (r1y1 - r1y2) / (r1x1 - r1x2);
    var r1b = r1y1 - (r1m * r1x1);

    var y = r1m * x + r1b;

    return {x: x, y: y};
  }; // </ findIntersectY() >

  /**
   * Find the x intersection point of a line and given y horizontal
   */
  function findIntersectX(r1x1, r1y1, r1x2, r1y2, y) {
    if (r1x1 == r1x2) {
      return {x: r1x1, y: y};
    }
    var r1m = (r1y1 - r1y2) / (r1x1 - r1x2);
    var r1b = r1y1 - (r1m * r1x1);

    // y = mx + b     // your old friend, linear equation
    // x = (y - b)/m  // linear equation solved for x
    var x = (y - r1b) / r1m;

    return {x: x, y: y};

  }; // </ findIntersectX() >

}; // </ jQuery.fn.bt() >

/**
 * jQuery's compat.js (used in Drupal's jQuery upgrade module, overrides the $().position() function
 *  this is a copy of that function to allow the plugin to work when compat.js is present
 *  once compat.js is fixed to not override existing functions, this function can be removed
 *  and .btPosion() can be replaced with .position() above...
 */
jQuery.fn.btPosition = function() {

  function num(elem, prop) {
    return elem[0] && parseInt( jQuery.curCSS(elem[0], prop, true), 10 ) || 0;
  };

  var left = 0, top = 0, results;

  if ( this[0] ) {
    // Get *real* offsetParent
    var offsetParent = this.offsetParent(),

    // Get correct offsets
    offset       = this.offset(),
    parentOffset = /^body|html$/i.test(offsetParent[0].tagName) ? { top: 0, left: 0 } : offsetParent.offset();

    // Subtract element margins
    // note: when an element has margin: auto the offsetLeft and marginLeft
    // are the same in Safari causing offset.left to incorrectly be 0
    offset.top  -= num( this, 'marginTop' );
    offset.left -= num( this, 'marginLeft' );

    // Add offsetParent borders
    parentOffset.top  += num( offsetParent, 'borderTopWidth' );
    parentOffset.left += num( offsetParent, 'borderLeftWidth' );

    // Subtract the two offsets
    results = {
      top:  offset.top  - parentOffset.top,
      left: offset.left - parentOffset.left
    };
  }

  return results;
}; // </ jQuery.fn.btPosition() >


/**
 * A convenience function to run btOn() (if available)
 * for each selected item
 */
jQuery.fn.btOn = function() {
  return this.each(function(index){
    if ($.isFunction(this.btOn)) {
      this.btOn();
    }
  });
}; // </ $().btOn() >

/**
 * 
 * A convenience function to run btOff() (if available)
 * for each selected item
 */
jQuery.fn.btOff = function() {
  return this.each(function(index){
    if ($.isFunction(this.btOff)) {
      this.btOff();
    }
  });
}; // </ $().btOff() >

jQuery.bt = {};
jQuery.bt.vars = {clickAnywhereStack: [], closeWhenOpenStack: []};

/**
 * This function gets bound to the document's click event
 * It turns off all of the tips in the click-anywhere-to-close stack
 */
jQuery.bt.docClick = function(e) {
  if (!e) {
    var e = window.event;
  };  
  if (!$(e.target).parents().andSelf().filter('.bt-wrapper, .bt-active').length) {
    // if clicked element isn't inside tip, close tips in stack
    $(jQuery.bt.vars.clickAnywhereStack).btOff();
    $(document).unbind('click', jQuery.bt.docClick);
  }
}; // </ docClick() >

/**
 * Defaults for the beauty tips
 *
 * Note this is a variable definition and not a function. So defaults can be
 * written for an entire page by simply redefining attributes like so:
 *
 *   jQuery.bt.defaults.width = 400;
 *
 * This would make all Beauty Tips boxes 400px wide.
 *
 * Each of these options may also be overridden during
 *
 * Can be overriden globally or at time of call.
 *
 */
jQuery.bt.defaults = {
  trigger:         'hover',                // trigger to show/hide tip
                                           // use [on, off] to define separate on/off triggers
                                           // also use space character to allow multiple  to trigger
                                           // examples:
                                           //   ['focus', 'blur'] // focus displays, blur hides
                                           //   'dblclick'        // dblclick toggles on/off
                                           //   ['focus mouseover', 'blur mouseout'] // multiple triggers
                                           //   'now'             // shows/hides tip without event
                                           //   'none'            // use $('#selector').btOn(); and ...btOff();
                                           //   'hoverIntent'     // hover using hoverIntent plugin (settings below)
                                           // note:
                                           //   hoverIntent becomes default if available
                                           
  clickAnywhereToClose: true,              // clicking anywhere outside of the tip will close it 
  closeWhenOthersOpen: false,              // tip will be closed before another opens - stop >= 2 tips being on
                                           
  width:            '200px',               // width of tooltip box
                                           //   when combined with cssStyles: {width: 'auto'}, this becomes a max-width for the text
  padding:          '10px',                // padding for content (get more fine grained with cssStyles)
  spikeGirth:       10,                    // width of spike
  spikeLength:      15,                    // length of spike
  overlap:          0,                     // spike overlap (px) onto target (can cause problems with 'hover' trigger)
  overlay:          false,                 // display overlay on target (use CSS to style) -- BUGGY!
  killTitle:        true,                  // kill title tags to avoid double tooltips

  textzIndex:       9999,                  // z-index for the text
  boxzIndex:        9998,                  // z-index for the "talk" box (should always be less than textzIndex)
  wrapperzIndex:    9997,
  positions:        ['most'],              // preference of positions for tip (will use first with available space)
                                           // possible values 'top', 'bottom', 'left', 'right' as an array in order of
                                           // preference. Last value will be used if others don't have enough space.
                                           // or use 'most' to use the area with the most space
  fill:             "rgb(255, 255, 102)",  // fill color for the tooltip box
  
  windowMargin:     10,                    // space (px) to leave between text box and browser edge

  strokeWidth:      1,                     // width of stroke around box, **set to 0 for no stroke**
  strokeStyle:      "#000",                // color/alpha of stroke

  cornerRadius:     5,                     // radius of corners (px), set to 0 for square corners
  
                    // following values are on a scale of 0 to 1 with .5 being centered
  
  centerPointX:     .5,                    // the spike extends from center of the target edge to this point
  centerPointY:     .5,                    // defined by percentage horizontal (x) and vertical (y)
    
  shadow:           false,                 // use drop shadow? (only displays in Safari and FF 3.1) - experimental
  shadowOffsetX:    2,                     // shadow offset x (px)
  shadowOffsetY:    2,                     // shadow offset y (px)
  shadowBlur:       3,                     // shadow blur (px)
  shadowColor:      "#000",                // shadow color/alpha

  animate:          false,                 // animate show/hide of box - EXPERIMENTAL (buggy in IE)
  distance:         15,                    // distance of animation movement (px)
  easing:           'swing',               // animation easing
  speed:            200,                   // speed (ms) of animation

  cssClass:         '',                    // CSS class to add to the box wrapper div (of the TIP)
  cssStyles:        {},                    // styles to add the text box
                                           //   example: {fontFamily: 'Georgia, Times, serif', fontWeight: 'bold'}
                                               
  activeClass:      'bt-active',           // class added to TARGET element when its BeautyTip is active

  contentSelector:  "$(this).attr('title')", // if there is no content argument, use this selector to retrieve the title

  ajaxPath:         null,                  // if using ajax request for content, this contains url and (opt) selector
                                           // this will override content and contentSelector
                                           // examples (see jQuery load() function):
                                           //   '/demo.html'
                                           //   '/help/ajax/snip'
                                           //   '/help/existing/full div#content'
                                           
                                           // ajaxPath can also be defined as an array
                                           // in which case, the first value will be parsed as a jQuery selector
                                           // the result of which will be used as the ajaxPath
                                           // the second (optional) value is the content selector as above
                                           // examples:
                                           //    ["$(this).attr('href')", 'div#content']
                                           //    ["$(this).parents('.wrapper').find('.title').attr('href')"]
                                           //    ["$('#some-element').val()"]
                                           
  ajaxError:        '<strong>ERROR:</strong> <em>%error</em>',
                                           // error text, use "%error" to insert error from server
  ajaxLoading:     '<blink>Loading...</blink>',  // yes folks, it's the blink tag!
  ajaxData:         {},                    // key/value pairs
  ajaxType:         'GET',                 // 'GET' or 'POST'
  ajaxCache:        true,                  // cache ajax results and do not send request to same url multiple times
  ajaxOpts:         {},                    // any other ajax options - timeout, passwords, processing functions, etc...
                                           // see http://docs.jquery.com/Ajax/jQuery.ajax#options
                                    
  preShow:          function(){return;},       // function to run before popup is built and displayed
  postShow:         function(){return;},       // function to run after popup is built and displayed
  preHide:          function(){return;},       // function to run before popup is removed
  postHide:         function(){return;},       // function to run after popup is removed
  
  hoverIntentOpts:  {                          // options for hoverIntent (if installed)
                      interval: 300,           // http://cherne.net/brian/resources/jquery.hoverIntent.html
                      timeout: 500
                    }
                                               
}; // </ jQuery.bt.defaults >


// @todo
// use larger canvas (extend to edge of page when windowMargin is active)
// add options to shift position of tip vert/horiz and position of spike tip
// create drawn (canvas) shadows
// use overlay to allow overlap with hover
// experiment with making tooltip a subelement of the target
// rework animation system