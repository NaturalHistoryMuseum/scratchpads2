// *************************************************************************
// * CookieGuard v1.0.1
// *
// * (c) 2012 Ultimateweb LTD <info@ultimateweb.co.uk>
// * All Rights Reserved.
// *
// * This program is free software: you can redistribute it and/or modify
// * it under the terms of the GNU General Public License as published by
// * the Free Software Foundation, either version 3 of the License, or
// * (at your option) any later version.
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// * GNU General Public License for more details.
// *
// * GNU General Public License <http://www.gnu.org/licenses/>.
// *
// *************************************************************************
// CHANGELOG
//
// 1.0.1
// - Uses jquery.cookie (as used by Drupal).
//
(function($){
  if(typeof $.cookieguard === 'undefined') {
    $.cookieguard = function(options){
      var defaults = {cookieDeleteDelay: 100, messageShowDelay: 1000, messageHideDelay: null, answeredHideDelay: 2000, slideSpeed: 500, cookiePrefix: 'cookieguard_', alertOfUnknown: true};
      $.cookieguard.settings = $.extend(defaults, options, {'cookiesUsed': new Array(), 'messageHideTimeout': null});
    };
  }
  if(typeof $.cookieguard.cookies === 'undefined') {
    $.cookieguard.cookies = function(){};
  }
  if(typeof $.cookieguard.cookies.add === 'undefined') {
    $.cookieguard.cookies.add = function(name, cookie, description, essential){
      var cookies = cookie.split(",");
      for( var i = 0; i < cookies.length; i++) {
        $.cookieguard.settings.cookiesUsed.push({'name': name, 'cookie': cookies[i], 'description': description, 'essential': essential});
        if(essential) $.cookieguard.cookies.storeAllowedCookie(cookies[i]);
      }
    };
  }
  if(typeof $.cookieguard.cookies.findUnknownCookies === 'undefined') {
    $.cookieguard.cookies.findUnknownCookies = function(){
      var allowedCookies = $.cookieguard.cookies.getAllowedCookies();
      var disallowedCookies = $.cookieguard.cookies.getDisallowedCookies();
      var definedCookies = $.cookieguard.settings.cookiesUsed;
      var allCookies = new Array();
      var unknownCookies = new Array();
      for( var i in $.cookie()) {
        allCookies.push(i);
      }
      for( var i = 0; i < allCookies.length; i++) {
        var cookieFound = false;
        if(allowedCookies != null) {
          for( var j = 0; j < allowedCookies.length; j++) {
            if(allowedCookies[j].indexOf('*') === 0 && allowedCookies[j].match("\\*$") == '*') {
              if(allCookies[i].indexOf(allowedCookies[j].replace('*', '')) > -1) {
                cookieFound = true;
                break;
              }
            } else if(allowedCookies[j].indexOf('*') === 0) {
              if(allCookies[i].match(allowedCookies[j].replace('*', '') + '$') == allowedCookies[j].replace('*', '')) {
                cookieFound = true;
                break;
              }
            } else if(allowedCookies[j].match("\\*$") == "*") {
              if(allCookies[i].indexOf(allowedCookies[j].replace('*', '')) === 0) {
                cookieFound = true;
                break;
              }
            } else if(allowedCookies[j] == allCookies[i]) {
              cookieFound = true;
              break;
            }
          }
        }
        if(!cookieFound) {
          if(disallowedCookies != null) {
            for( var j = 0; j < disallowedCookies.length; j++) {
              if(disallowedCookies[j].indexOf('*') === 0 && disallowedCookies[j].match("\\*$") == '*') {
                if(allCookies[i].indexOf(disallowedCookies[j].replace('*', '')) > -1) {
                  cookieFound = true;
                  break;
                }
              } else if(disallowedCookies[j].indexOf('*') === 0) {
                if(allCookies[i].match(disallowedCookies[j].replace('*', '') + '$') == disallowedCookies[j].replace('*', '')) {
                  cookieFound = true;
                  break;
                }
              } else if(disallowedCookies[j].match("\\*$") == "*") {
                if(allCookies[i].indexOf(disallowedCookies[j].replace('*', '')) === 0) {
                  cookieFound = true;
                  break;
                }
              } else if(disallowedCookies[j] == allCookies[i]) {
                cookieFound = true;
                break;
              }
            }
          }
        }
        if(!cookieFound) {
          if(definedCookies.length > 0) {
            for( var j = 0; j < definedCookies.length; j++) {
              if(definedCookies[j].cookie.indexOf('*') === 0 && definedCookies[j].cookie.match("\\*$") == '*') {
                if(allCookies[i].indexOf(definedCookies[j].cookie.replace(/\*/g, '')) > -1) {
                  cookieFound = true;
                  break;
                }
              } else if(definedCookies[j].cookie.indexOf('*') === 0) {
                if(allCookies[i].match(definedCookies[j].cookie.replace(/\*/g, '') + '$') == definedCookies[j].cookie.replace(/\*/g, '')) {
                  cookieFound = true;
                  break;
                }
              } else if(definedCookies[j].cookie.match("\\*$") == "*") {
                if(allCookies[i].indexOf(definedCookies[j].cookie.replace(/\*/g, '')) === 0) {
                  cookieFound = true;
                  break;
                }
              } else if(definedCookies[j].cookie == allCookies[i]) {
                cookieFound = true;
                break;
              }
            }
          }
        }
        if(!cookieFound) if(allCookies[i].indexOf($.cookieguard.settings.cookiePrefix) === 0) cookieFound = true;
        if(!cookieFound) {
          unknownCookies.push(allCookies[i]);
        }
      }
      return unknownCookies;
    };
  }
  if(typeof $.cookieguard.cookies.storeAllowedCookie === 'undefined') {
    $.cookieguard.cookies.storeAllowedCookie = function(cookie){
      var allowedCookies = $.cookieguard.cookies.getAllowedCookies();
      var allowedCookiesStr;
      if(allowedCookies == null)
        allowedCookiesStr = cookie;
      else {
        allowedCookiesStr = allowedCookies.join(',');
        if($.inArray(cookie, allowedCookies) == -1) allowedCookiesStr = allowedCookiesStr + ',' + cookie;
      }
      $.cookie($.cookieguard.settings.cookiePrefix + 'allowedCookies', allowedCookiesStr, 365);
    };
  }
  if(typeof $.cookieguard.cookies.getAllowedCookies === 'undefined') {
    $.cookieguard.cookies.getAllowedCookies = function(){
      var allowedCookies = $.cookie($.cookieguard.settings.cookiePrefix + 'allowedCookies');
      if(allowedCookies != null) return allowedCookies.split(',');
      return null;
    };
  }
  if(typeof $.cookieguard.cookies.storeDisallowedCookie === 'undefined') {
    $.cookieguard.cookies.storeDisallowedCookie = function(cookie){
      var disallowedCookies = $.cookieguard.cookies.getDisallowedCookies();
      var disallowedCookiesStr;
      if(disallowedCookies == null)
        disallowedCookiesStr = cookie;
      else {
        disallowedCookiesStr = disallowedCookies.join(',');
        if($.inArray(cookie, disallowedCookies) == -1) disallowedCookiesStr = disallowedCookiesStr + ',' + cookie;
      }
      $.cookie($.cookieguard.settings.cookiePrefix + 'disallowedCookies', disallowedCookiesStr, {expires: 365});
    };
  }
  if(typeof $.cookieguard.cookies.getDisallowedCookies === 'undefined') {
    $.cookieguard.cookies.getDisallowedCookies = function(){
      var disallowedCookies = $.cookie($.cookieguard.settings.cookiePrefix + 'disallowedCookies')
      if(disallowedCookies != null) return disallowedCookies.split(',');
      return null;
    };
  }
  if(typeof $.cookieguard.eradicateCookies === 'undefined') {
    $.cookieguard.eradicateCookies = function(){
      var cookiesToDestroy = $.cookieguard.cookies.getDisallowedCookies();
      var cookiesToAllow = $.cookieguard.cookies.getAllowedCookies();
      var allCookies = new Array();
      for( var i in $.cookie()) {
        allCookies.push(i);
      }
      for( var i = 0; i < allCookies.length; i++) {
        var cookieFound = false;
        if(allCookies[i].indexOf($.cookieguard.settings.cookiePrefix) === 0) cookieFound = true;
        if(!cookieFound) {
          if(cookiesToAllow != null) {
            for( var j = 0; j < cookiesToAllow.length; j++) {
              if(cookiesToAllow[j].indexOf('*') === 0 && cookiesToAllow[j].match("\\*$") == '*') {
                if(allCookies[i].indexOf(cookiesToAllow[j].replace('*', '')) > -1) {
                  cookieFound = true;
                  break;
                }
              } else if(cookiesToAllow[j].indexOf('*') === 0) {
                if(allCookies[i].match(cookiesToAllow[j].replace('*', '') + '$') == cookiesToAllow[j].replace('*', '')) {
                  cookieFound = true;
                  break;
                }
              } else if(cookiesToAllow[j].match("\\*$") == "*") {
                if(allCookies[i].indexOf(cookiesToAllow[j].replace('*', '')) === 0) {
                  cookieFound = true;
                  break;
                }
              } else if(cookiesToAllow[j] == allCookies[i]) {
                cookieFound = true;
                break;
              }
            }
          }
          if(!cookieFound) {
            if(cookiesToDestroy != null) {
              for( var j = 0; j < cookiesToDestroy.length; j++) {
                if(cookiesToDestroy[j].indexOf('*') === 0 && cookiesToDestroy[j].match("\\*$") == '*') {
                  if(allCookies[i].indexOf(cookiesToDestroy[j].replace('*', '')) > -1) {
                    $.removeCookie(allCookies[i]);
                    cookieFound = true;
                    break;
                  }
                } else if(cookiesToDestroy[j].indexOf('*') === 0) {
                  if(allCookies[i].match(cookiesToDestroy[j].replace('*', '') + '$') == cookiesToDestroy[j].replace('*', '')) {
                    $.removeCookie(allCookies[i]);
                    cookieFound = true;
                    break;
                  }
                } else if(cookiesToDestroy[j].match("\\*$") == "*") {
                  if(allCookies[i].indexOf(cookiesToDestroy[j].replace('*', '')) === 0) {
                    $.removeCookie(allCookies[i]);
                    cookieFound = true;
                    break;
                  }
                } else if(cookiesToDestroy[j] == allCookies[i]) {
                  $.removeCookie(allCookies[i]);
                  cookieFound = true;
                  break;
                }
              }
            }
          }
        }
      }
    }
  }
  if(typeof $.cookieguard.run === 'undefined') {
    $.cookieguard.run = function(){
      setTimeout(function(){
        var unknownCookies;
        if($.cookieguard.settings.alertOfUnknown)
          unknownCookies = $.cookieguard.cookies.findUnknownCookies();
        else
          unknownCookies = new Array();
        if(!$.cookieguard.hasAnswered()) {
          $.cookieguard.buildMessage(true, $.cookieguard.settings.cookiesUsed, unknownCookies);
          $.cookieguard.displayMessage($.cookieguard.settings.messageShowDelay, $.cookieguard.settings.messageHideDelay);
        } else {
          $.cookieguard.eradicateCookies();
          if(unknownCookies.length > 0) {
            $.cookieguard.buildMessage(false, null, unknownCookies);
            $.cookieguard.displayMessage($.cookieguard.settings.messageShowDelay, $.cookieguard.settings.messageHideDelay);
          }
        }
      }, $.cookieguard.settings.cookieDeleteDelay);
    };
  }
  if(typeof $.cookieguard.hasAnswered === 'undefined') {
    $.cookieguard.hasAnswered = function(){
      if($.cookie($.cookieguard.settings.cookiePrefix + "initialised") != null)
        return true;
      else
        return false;
    };
  }
  if(typeof $.cookieguard.buildMessage === 'undefined') {
    $.cookieguard.buildMessage = function(init, knownCookies, unknownCookies){
      $('body').prepend('<div id="cookieGuardMsg"><div id="cookieGuardMsgInner"><a href="http://cookieguard.eu" target="_blank" id="cookieGuardLink">Click here to get Cookie Guard for your site</a></div></div>');
      if(init) {
        $('#cookieGuardMsgInner').append('This website uses cookies.<br/>');
        if(unknownCookies.length == 0 && $.cookieguard.hasOnlyEssential())
          $('#cookieGuardMsgInner').addClass('onlyEssential').append('<a href="#" id="showCookies">Show</a><a href="#" id="cookieGuardOkay">Okay</a>');
        else
          $('#cookieGuardMsgInner').append('You may choose to block non-essential and unknown cookies.<a href="#" id="showCookies">Show</a><a href="#" id="authoriseCookies">Allow</a><a href="#" id="denyCookies">Block</a>');
      } else {
        $('#cookieGuardMsgInner').append('Cookie Guard has found new cookies.<br/>');
        $('#cookieGuardMsgInner').append('You may choose to block these cookies.<a href="#" id="showCookies">Show</a><a href="#" id="authoriseCookies">Allow</a><a href="#" id="denyCookies">Block</a>');
      }
      $.cookieguard.buildCookieList(init, knownCookies, unknownCookies);
      $('#cookieGuardOkay').click(function(){
        clearTimeout($.cookieguard.settings.messageHideTimeout);
        $.cookieguard.hideMessage(0);
        $.cookie($.cookieguard.settings.cookiePrefix + 'initialised', '1', {expires: 365});
        return false;
      });
      $('#showCookies').click(function(){
        if($(this).text() == 'Show') {
          clearTimeout($.cookieguard.settings.messageHideTimeout);
          $('#cookieList').show();
          $(this).text('Hide');
          var h = $('#cookieList').outerHeight();
          $('#cookieList').css({'overflow': 'hidden', 'height': 0}).animate({'height': h}, $.cookieguard.settings.slideTimer);
        } else if($(this).text() == 'Hide') {
          $(this).text('Show');
          $('#cookieList').animate({'height': 0}, $.cookieguard.settings.slideTimer, null, function(){
            $('#cookieList').hide().attr('style', '').removeAttr('style');
          });
        }
        return false;
      });
      $('#authoriseCookies').click(function(){
        clearTimeout($.cookieguard.settings.messageHideTimeout);
        $('#cookieGuardMsgInner').empty().addClass('msgAllowed').html('The listed cookies have now been allowed on this site.');
        $.cookieguard.hideMessage($.cookieguard.settings.answeredHideDelay);
        if(knownCookies != null) {
          for( var i = 0; i < knownCookies.length; i++)
            $.cookieguard.cookies.storeAllowedCookie(knownCookies[i].cookie);
        }
        if(unknownCookies != null) {
          for( var i = 0; i < unknownCookies.length; i++)
            $.cookieguard.cookies.storeAllowedCookie(unknownCookies[i]);
        }
        $.cookie($.cookieguard.settings.cookiePrefix + 'initialised', '1', {expires: 365});
        return false;
      });
      $('#denyCookies').click(function(){
        clearTimeout($.cookieguard.settings.messageHideTimeout);
        $('#cookieGuardMsgInner').empty().addClass('msgDenied').html('Non-essential and unknown cookies have now been blocked on this site.');
        $.cookieguard.hideMessage($.cookieguard.settings.answeredHideDelay);
        if(knownCookies != null) {
          for( var i = 0; i < knownCookies.length; i++)
            if(knownCookies[i].essential == false) $.cookieguard.cookies.storeDisallowedCookie(knownCookies[i].cookie);
        }
        if(unknownCookies != null) {
          for( var i = 0; i < unknownCookies.length; i++)
            $.cookieguard.cookies.storeDisallowedCookie(unknownCookies[i]);
        }
        $.cookie($.cookieguard.settings.cookiePrefix + 'initialised', '1', {expires: 365});
        $.cookieguard.eradicateCookies();
        return false;
      });
    };
  }
  if(typeof $.cookieguard.buildCookieList === 'undefined') {
    $.cookieguard.buildCookieList = function(init, knownCookies, unknownCookies){
      var essentialCookies = new Array();
      var unessentialCookies = new Array();
      var knownNames = new Array();
      if(knownCookies != null) {
        for( var i = 0; i < knownCookies.length; i++) {
          if($.inArray(knownCookies[i].name, knownNames) == -1) {
            knownNames.push(knownCookies[i].name);
            if(knownCookies[i].essential)
              essentialCookies.push(knownCookies[i]);
            else
              unessentialCookies.push(knownCookies[i]);
          }
        }
      }
      $('#cookieGuardMsgInner').append('<div id="cookieList"/>');
      if(init) {
        $('#cookieList').append('<div class="cookiesHeader">Essential Cookies <span>- The site owner has indicated that these are essential to the running of the site.</span></div>');
        $('#cookieList').append('<ul class="essentialCookies"/>');
        $('#cookieList > ul.essentialCookies').append('<li><div class="cookieName">Cookie Guard</div><div class="cookieDescription"> - This cookie is essential for storing the status of your cookie choices whilst using this site.</div></li>');
        for( var i = 0; i < essentialCookies.length; i++) {
          $('#cookieList > ul.essentialCookies').append('<li><div class="cookieName">' + essentialCookies[i].name + '</div><div class="cookieDescription"> - ' + essentialCookies[i].description + '</div></li>');
        }
      }
      if(unessentialCookies.length > 0) {
        $('#cookieList').append('<div class="cookiesHeader">Non-Essential Cookies <span>- The site owner has approved these cookies but you may turn them off.</span></div>');
        $('#cookieList').append('<ul class="knownCookies" />');
        for( var i = 0; i < unessentialCookies.length; i++) {
          $('#cookieList > ul.knownCookies').append('<li><div class="cookieName">' + unessentialCookies[i].name + '</div><div class="cookieDescription"> - ' + unessentialCookies[i].description + '</div></li>');
        }
      }
      if(unknownCookies != null && unknownCookies.length > 0) {
        $('#cookieList').append('<div class="cookiesHeader">Unknown Cookies <span>- The site owner has not approved these cookies.</span></div>');
        $('#cookieList').append('<ul class="unknownCookies" />');
        for( var i = 0; i < unknownCookies.length; i++) {
          $('#cookieList > ul.unknownCookies').append('<li><div class="cookieName">' + unknownCookies[i] + '</div></li>');
        }
      }
    };
  }
  if(typeof $.cookieguard.displayMessage === 'undefined') {
    $.cookieguard.displayMessage = function(showDelay, hideDelay){
      $.cookieguard.createCSS();
      $('body').attr('marginTop', $('body').css('marginTop')).css('margin', 0).delay(showDelay).animate({'marginTop': $('#cookieGuardMsg').outerHeight()}, $.cookieguard.settings.slideSpeed);
      $('#cookieGuardMsg').css('top', -$('#cookieGuardMsg').outerHeight());
      $('#cookieGuardMsg').delay(showDelay).show().animate({'top': 0}, $.cookieguard.settings.slideSpeed);
      if($.cookieguard.settings.messageHideDelay != null) {
        $.cookieguard.settings.messageHideTimeout = setTimeout(function(){
          $.cookieguard.hideMessage(0);
        }, hideDelay);
      }
    };
  }
  if(typeof $.cookieguard.hideMessage === 'undefined') {
    $.cookieguard.hideMessage = function(hideDelay){
      $('body').delay(hideDelay).animate({'marginTop': $('body').attr('marginTop')}, $.cookieguard.settings.slideSpeed);
      $('#cookieGuardMsg').delay(hideDelay).animate({'top': -$('#cookieGuardMsg').height()}, $.cookieguard.settings.slideSpeed, null, function(){
        $('#cookieGuardMsg').remove();
        $('body').attr('style', '').removeAttr('style');
      });
    }
  }
  if(typeof $.cookieguard.hasOnlyEssential === 'undefined') {
    $.cookieguard.hasOnlyEssential = function(){
      var knownCookies = $.cookieguard.settings.cookiesUsed;
      for( var i = 0; i < knownCookies.length; i++) {
        if(!knownCookies[i].essential) return false;
      }
      return true;
    }
  }
  if(typeof $.cookieguard.createCSS == 'undefined') {
    $.cookieguard.createCSS = function(){
      var style = '<style id="cookieGuardStyles" type="text/css">';
      style += '#cookieGuardMsg { position: absolute; text-align: left; top: 0; left: 0; width: 100%; display: none; border-bottom: 2px solid #5c5c5c; font-size: 12px; font-family: Arial, Helvetica, Sans-Serif; color: #333; background: #e2e2e2 url(http://cookieguard.eu/images/cookieguardicon.png) no-repeat 12px 12px; min-height: 50px; z-index:99999; }';
      style += '#cookieGuardMsgInner { padding: 10px 10px 10px 55px; }';
      style += '#cookieGuardMsg a { text-decoration: none; font-weight: normal; font-style: normal; }';
      style += '#cookieGuardLink { position: absolute; bottom: 5px; right: 10px; font-size: 10px; color: #912b2b; }';
      style += '#showCookies { position: absolute; display: block; left: 415px; top: 13px; border: 1px solid #999; background: #d4d4d4; color: #5b5858; padding: 5px 10px; -webkit-border-radius: 3px; -moz-border-radius: 3px; border-radius: 3px; }';
      style += '#showCookies:hover { border-color: #666; }';
      style += '#authoriseCookies, #cookieGuardOkay { position: absolute; display: block; left: 500px; top: 13px; border: 1px solid #a2bf8e; background: #d1ecbe; color: #384c2a; padding: 5px 10px; -webkit-border-radius: 3px; -moz-border-radius: 3px; border-radius: 3px; }';
      style += '#authoriseCookies:hover { border-color: #6f8f59; }';
      style += '#denyCookies { position: absolute; display: block; left: 560px; top: 13px; border: 1px solid #cc9c9c; background: #ecc1c1; color: #7e5353; padding: 5px 10px; -webkit-border-radius: 3px; -moz-border-radius: 3px; border-radius: 3px; }';
      style += '#denyCookies:hover { border-color: #9e6a6a;}';
      style += '#cookieList { display: none; padding: 10px 60px 0 0; }';
      style += '#cookieList ul { list-style: none; padding-left: 20px; }';
      style += '#cookieList li { padding: 5px 0; }';
      style += '.cookieName { font-weight: bold; display: inline; }';
      style += '.cookieDescription { display: inline; }';
      style += '.cookiesHeader { font-weight: bold; border-bottom: 1px solid #222; color: #222; margin-bottom: 3px; padding-top: 10px; }';
      style += '.cookiesHeader span { font-weight: normal; font-size: 11px; }';
      style += '#cookieGuardMsgInner.msgAllowed, #cookieGuardMsgInner.msgDenied, #cookieGuardMsgInner.onlyEssential { padding-top: 17px; }';
      style += '</style>';
      $('head').append(style);
    }
  }
})(jQuery);
