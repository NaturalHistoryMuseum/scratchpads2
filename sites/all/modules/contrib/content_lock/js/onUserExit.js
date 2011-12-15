/*
 * onUserExit jQuery Plugin (http://www.userfirstinteractive.com/)
 * @author Scott D. Brooks
 * @created by UserFirst Interactive (creations@userfirstinteractive.com)
 *
 * @version 1.1
 *
 * @changelog
 * v 1.0 	->	Starting release [Dec. 27, 2008]
 * v 1.1 	->	Added support for detection of page refresh through F5 key and ctrl+r.
				Added support for forms as well, so submissions don't trigger exit event. [Jan. 25, 2010]
 *
 */
var movingWithinSite 	= false;  // this is the var that determines if the unload was caused by a user leaving, or navigating in the site.
var codeToExecute		= function() {};

function userMovingWithinSite() {
	movingWithinSite = true;
}

// Code to detect refreshing of the page through keyboard use
var ctrlKeyIsDown = false;
function interceptKeyUp(e) {
	if( !e ) {
		if (window.event)
			e = window.event;
		else
			return;
	}

	keyCode = e.keyCode;
	if (keyCode == 17){
		ctrlKeyIsDown = false;
	}
}

function interceptKeyDown(e) {
	if( !e ) {
		if (window.event)
			e = window.event;
		else
			return;
	}

	keyCode = e.keyCode;
	// F5 detected
	if ( keyCode == 116 ) {
		userMovingWithinSite();
	}

	if (keyCode == 17){
		ctrlKeyIsDown = true;
	}

	// then they are pressing Ctrl+R
	if (ctrlKeyIsDown && keyCode == 82){
		userMovingWithinSite();
	}
}

function interceptKeyPress(e) {
	if( !e ) {
		if (window.event)
			e = window.event;
		else
			return;
	}

	var keyCode = e.keyCode ? e.keyCode : e.which ? e.which : void 0;
	if(e.charCode == null || e.charCode == 0 ) {
		// F5 pressed
		if ( keyCode == 116 ) {
			userMovingWithinSite();
		}
	}
}

function attachEventListener( obj, type, func, capture ) {
	if(window.addEventListener) {
		//Mozilla, Netscape, Firefox
		obj.addEventListener( type, func, capture );
	} else {
		//IE
		obj.attachEvent( 'on' + type, func );
	}
}

(function($){
	$.fn.onUserExit = function(options) {
		var defaults = {
			execute:			"",	 // no function assigned by default
      executeConfirm:     "",  // executed when its confirmed
			internalURLs:		""		// used to detect whether the url is internal or not (you can add subdomains to this list so when a user hops between sites, they are still considered to be internal.
		};
		var options 			= $.extend(defaults, options);

		if (options.execute == "") {
			alert("The onUserExit jQuery Plugin has been misconfigured.  Please add the function you wish to execute.");
		}
		if (options.internalURLs == "") {
			alert("The onUserExit jQuery Plugin has been misconfigured.  Please add internal URLs so it know when the user is navigating internally.");
		}
		codeToExecute = options.execute;

		// add onClick function to all internal links
		$("a, button").each(function() {
			var obj = $(this);
			var linkIsInternal = false;
		    var href = obj.attr('href');

			var myInternalURLs = options.internalURLs.split("|");

		    if (!linkIsInternal && href != undefined) {
			for (i = 0; i < myInternalURLs.length; i++) {
			    if (href.indexOf(myInternalURLs[i]) !== -1) {
				linkIsInternal = true;
			    }
			}
		    }

			if (linkIsInternal == true) {
				obj.bind("click", function(){
					userMovingWithinSite();
    			});
			}
		});

		$("form").each(function() {
			var obj = $(this);
			currentonSubmit = obj.attr("onSubmit");
			if (currentonSubmit === undefined) {
				currentonSubmit = "";
			}
			obj.attr("onSubmit", currentonSubmit + " userMovingWithinSite();");
		});

		// for Refresh Detection
		attachEventListener(document,"keydown",interceptKeyDown,true);
		attachEventListener(document,"keyup",interceptKeyUp,true);
		attachEventListener(document,"keypress",interceptKeyPress,true);
    window.onbeforeunload = function(){
      if (movingWithinSite == false) {
        return options.executeConfirm();
      }
    }
	};

  window.onunload = function() {
		// unloading the page when the user is leaving
		if (movingWithinSite == false) {
			codeToExecute();
		}
	};
})(jQuery);
