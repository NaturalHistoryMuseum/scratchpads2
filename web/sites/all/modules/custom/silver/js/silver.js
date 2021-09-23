(function($){
  // Extend the jQuery object to allow us to redirect.
  $.prototype.silver_goto = function(url){
    if(parent.document.location.hash.indexOf('admin/import') > 1){
      parent.document.location.hash = parent.document.location.hash.replace('admin/import', url);
    } else {
      parent.document.location.pathname = parent.document.location.pathname.replace('admin/import', url);
    }
  }
})(jQuery);