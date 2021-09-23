(function($){
  Drupal.behaviors.jsphylosvg = {attach: function(context, settings){
    var magic_number = 17;
    $('.jsphylosvg', context).each(function(){
      // FIXME - This will need changing when we upgrade jQuery to 1.9
      if($.browser && $.browser.msie) {
        var version = parseInt($.browser.version.substring(0, 1), 10);
        if(version < 9) {
          $(this).append('<h3>' + Drupal.t('You appear to be using an older and unsupported version of Internet Explorer.  In order to view phylogenetic trees, you will need to upgrade.') + '</h3>');
          return;
        }
      }
      var height = 800;
      var width = 800;
      if(Drupal.settings.jsphylosvg[$(this).attr('id')].type != 'circular') {
        height = Drupal.settings.jsphylosvg[$(this).attr('id')].count * magic_number;
      }
      var phylocanvas = new Smits.PhyloCanvas(Drupal.settings.jsphylosvg[$(this).attr('id')].data, $(this).attr('id'), width, height, Drupal.settings.jsphylosvg[$(this).attr('id')].type);
      // Due to a bug in jsPhylo (I think), we need to increase the height for
      // none circular SVGs
      if(height != 800) {
        var elem = $(this).children('svg')[0];
        elem.setAttribute('height', height + magic_number);
      }
      $(this).append('<p class="jsphylosvg-save"><a href="#" id="' + $(this).attr('id') + '-get">' + Drupal.t('Show/Hide data') + '</a> | <a style="display:none;" href="#" id="' + $(this).attr('id') + '-switch">' + Drupal.t('Switch style') + '</a> | <a style="display:none;" href="#" id="' + $(this).attr('id') + '-save">' + Drupal.t('View tree') + '</a> | ' + '<a style="display:none;" href="#" id="' + $(this).attr('id') + '-print">' + Drupal.t('Print tree') + '</a></p>');
      var svgSource = phylocanvas.getSvgSource();
      svgSource = Base64.encode(svgSource);
      $('#' + $(this).attr('id') + '-get').click(function(){
        $(this).parent().parent().children('.jsphylosvg-data').toggle();
        return false;
      });
      if(svgSource) {
        $('#' + $(this).attr('id') + '-save').show();
        $('#' + $(this).attr('id') + '-print').show();
        $('#' + $(this).attr('id') + '-switch').show();
        $('#' + $(this).attr('id') + '-save')[0].href = "data:image/svg+xml;base64," + svgSource;
        $('#' + $(this).attr('id') + '-print').click(function(){
          pwin = window.open("data:image/svg+xml;base64," + svgSource, "_blank");
          setTimeout("pwin.print()", 2000);
        });
        var this_copy = this;
        $('#' + $(this).attr('id') + '-switch').click(function(){
          $(this_copy).empty();
          if(Drupal.settings.jsphylosvg[$(this_copy).attr('id')].type == 'circular') {
            Drupal.settings.jsphylosvg[$(this_copy).attr('id')].type = 'rectangular';
          } else {
            Drupal.settings.jsphylosvg[$(this_copy).attr('id')].type = 'circular';
          }
          Drupal.behaviors.jsphylosvg.attach($('body'));
        });
      }
    });
  }}
})(jQuery);
/**
 * 
 * Base64 encode / decode http://www.webtoolkit.info/
 * 
 */
var Base64 = {
// private property
_keyStr: "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=",
// public method for encoding
encode: function(input){
  var output = "";
  var chr1, chr2, chr3, enc1, enc2, enc3, enc4;
  var i = 0;
  input = Base64._utf8_encode(input);
  while(i < input.length) {
    chr1 = input.charCodeAt(i++);
    chr2 = input.charCodeAt(i++);
    chr3 = input.charCodeAt(i++);
    enc1 = chr1 >> 2;
    enc2 = ((chr1 & 3) << 4) | (chr2 >> 4);
    enc3 = ((chr2 & 15) << 2) | (chr3 >> 6);
    enc4 = chr3 & 63;
    if(isNaN(chr2)) {
      enc3 = enc4 = 64;
    } else if(isNaN(chr3)) {
      enc4 = 64;
    }
    output = output + this._keyStr.charAt(enc1) + this._keyStr.charAt(enc2) + this._keyStr.charAt(enc3) + this._keyStr.charAt(enc4);
  }
  return output;
},
// public method for decoding
decode: function(input){
  var output = "";
  var chr1, chr2, chr3;
  var enc1, enc2, enc3, enc4;
  var i = 0;
  input = input.replace(/[^A-Za-z0-9\+\/\=]/g, "");
  while(i < input.length) {
    enc1 = this._keyStr.indexOf(input.charAt(i++));
    enc2 = this._keyStr.indexOf(input.charAt(i++));
    enc3 = this._keyStr.indexOf(input.charAt(i++));
    enc4 = this._keyStr.indexOf(input.charAt(i++));
    chr1 = (enc1 << 2) | (enc2 >> 4);
    chr2 = ((enc2 & 15) << 4) | (enc3 >> 2);
    chr3 = ((enc3 & 3) << 6) | enc4;
    output = output + String.fromCharCode(chr1);
    if(enc3 != 64) {
      output = output + String.fromCharCode(chr2);
    }
    if(enc4 != 64) {
      output = output + String.fromCharCode(chr3);
    }
  }
  output = Base64._utf8_decode(output);
  return output;
},
// private method for UTF-8 encoding
_utf8_encode: function(string){
  string = string.replace(/\r\n/g, "\n");
  var utftext = "";
  for( var n = 0; n < string.length; n++) {
    var c = string.charCodeAt(n);
    if(c < 128) {
      utftext += String.fromCharCode(c);
    } else if((c > 127) && (c < 2048)) {
      utftext += String.fromCharCode((c >> 6) | 192);
      utftext += String.fromCharCode((c & 63) | 128);
    } else {
      utftext += String.fromCharCode((c >> 12) | 224);
      utftext += String.fromCharCode(((c >> 6) & 63) | 128);
      utftext += String.fromCharCode((c & 63) | 128);
    }
  }
  return utftext;
},
// private method for UTF-8 decoding
_utf8_decode: function(utftext){
  var string = "";
  var i = 0;
  var c = c1 = c2 = 0;
  while(i < utftext.length) {
    c = utftext.charCodeAt(i);
    if(c < 128) {
      string += String.fromCharCode(c);
      i++;
    } else if((c > 191) && (c < 224)) {
      c2 = utftext.charCodeAt(i + 1);
      string += String.fromCharCode(((c & 31) << 6) | (c2 & 63));
      i += 2;
    } else {
      c2 = utftext.charCodeAt(i + 1);
      c3 = utftext.charCodeAt(i + 2);
      string += String.fromCharCode(((c & 15) << 12) | ((c2 & 63) << 6) | (c3 & 63));
      i += 3;
    }
  }
  return string;
}}