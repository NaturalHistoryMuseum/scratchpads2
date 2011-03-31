
var dialog	= window.parent ;
var oEditor = dialog.InnerDialogLoaded() ;

var FCK			  = oEditor.FCK ;
var FCKLang		= oEditor.FCKLang ;
var FCKConfig	= oEditor.FCKConfig ;
var FCKRegexLib	= oEditor.FCKRegexLib ;
var FCKTools	= oEditor.FCKTools ;

dialog.SetAutoSize( true ) ;

// Activate the "OK" button.
dialog.SetOkButton( true ) ;
var oLink = dialog.Selection.GetSelection().MoveToAncestorNode( 'A' ) ;

var selection = "";
if(oEditor.FCK.EditorDocument.selection != null) {
  selection = oEditor.FCK.EditorDocument.selection.createRange().text;
} else {
  selection = oEditor.FCK.EditorWindow.getSelection(); // after this, won't be a string
  selection = "" + selection; // now a string again
}

(function ($) {
  $(document).ready(function() {
    $('#edit-cancel, #edit-insert').hide();
    $('*', document).keydown(function(ev) {
      if (ev.keyCode == 13) {
        // Prevent browsers from firing the click event on the first submit
        // button when enter is used to select.
        return false;
      }
    });

    if ( oLink ) {
      FCK.Selection.SelectNode( oLink ) ;

       // To prevent dubble anchors
      var anchor = linkit_helper.seek_for_anchor($(oLink).attr('href'));
      // Delete the anchor from the URL, this will be added later on anyway
      var href = $(oLink).attr('href').replace('#' + anchor, '');
      
      if(href.length > 0) {
       linkit_helper.search_styled_link(href);
      }

      $('#edit-title').val($(oLink).attr('title'));
      $('#edit-id').val($(oLink).attr('id'));
      $('#edit-class').val($(oLink).attr('class'));
      $('#edit-rel').val($(oLink).attr('rel'));
      $('#edit-accesskey').val($(oLink).attr('accesskey'));
      $('#edit-anchor').val(anchor);
    } else if(selection == "") {
      // Show help text when there is no selection element
      linkit_helper.show_no_selection_text();
    }
  });
})(jQuery);

// The OK button was hit.
function Ok() {
  var sInnerHtml ;

  (function ($) {
    var matches = $('#edit-link--2').val().match(/\[path:(.*)\]/i);
    linlit_url = (matches == null) ? $('#edit-link--2').val() : matches[1];

    var asLinkPath = $('#edit-link--2').val().match(/(.*)\[path:.*\]/i);
    asLinkPath_text = (asLinkPath == null) ? $('#edit-link--2').val() : asLinkPath[1].replace(/^\s+|\s+$/g, '');

    // Add anchor if we have any and make sure there is no "#" before adding the anchor
    // But do not add if there is an anchor in the URL
    var anchor = $('#edit-anchor').val().replace(/#/g,'');
    var hasAnchor = $('#edit-link--2').val().match(/\#/i);

    if(anchor.length > 0 && hasAnchor == null ) {
      linlit_url = linlit_url.concat('#' + anchor);
      asLinkPath_text = asLinkPath_text.concat('#' + anchor);
    }

    if ( linlit_url.length == 0 ) {
      alert(Drupal.t('No URL'));
      return false ;
    }

    oEditor.FCKUndo.SaveUndoStep();

    // If no link is selected, create a new one (it may result in more than one link creation).
    var aLinks = oLink ? [ oLink ] : oEditor.FCK.CreateLink( linlit_url, true ) ;
    
    // If no selection, no links are created, so use the uri as the link text
    var aHasSelection = ( aLinks.length > 0 ) ;
    if ( !aHasSelection )
    {
      if (asLinkPath_text)
        sInnerHtml = asLinkPath_text;  // use matched path

      // Create a new (empty) anchor.
      aLinks = [ oEditor.FCK.InsertElement( 'a' ) ] ;
    }
    
    for ( var i = 0 ; i < aLinks.length ; i++ )
    {
      oLink = aLinks[i] ;

      if ( aHasSelection )
        sInnerHtml = oLink.innerHTML ;		// Save the innerHTML (IE changes it if it is like an URL).

      oLink.href = linlit_url ;
      SetAttribute( oLink, '_fcksavedurl', linlit_url ) ;

      oLink.innerHTML = sInnerHtml ;		// Set (or restore) the innerHTML

      // Let's set the "id" only for the first link to avoid duplication.
      if ( i == 0 )
        SetAttribute( oLink, 'id', $('#edit-id').val() ) ;

      // Advances Attributes
      SetAttribute( oLink, 'title', $('#edit-title').val() ) ;
      SetAttribute( oLink, 'rel', $('#edit-rel').val() ) ;
      SetAttribute( oLink, 'accesskey', $('#edit-accesskey').val() ) ;
      SetAttribute( oLink, 'class', $('#edit-class').val() ) ;
    }

    // Select the (first) link.
    oEditor.FCKSelection.SelectNode( aLinks[0] );
  })(jQuery);
  return true ;
}

function SetAttribute( element, attName, attValue )
{
	if ( attValue == null || attValue.length == 0 )
		element.removeAttribute( attName, 0 ) ;			// 0 : Case Insensitive
	else
		element.setAttribute( attName, attValue, 0 ) ;	// 0 : Case Insensitive
}

function GetAttribute( element, attName, valueIfNull )
{
	var oAtt = element.attributes[attName] ;

	if ( oAtt == null || !oAtt.specified )
		return valueIfNull ? valueIfNull : '' ;

	var oValue = element.getAttribute( attName, 2 ) ;

	if ( oValue == null )
		oValue = oAtt.nodeValue ;

	return ( oValue == null ? valueIfNull : oValue ) ;
}