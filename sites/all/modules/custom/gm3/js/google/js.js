

window.google = window.google || {};
google.maps = google.maps || {};
(function() {


  var modules = google.maps.modules = {};
  google.maps.__gjsload__ = function(name, text) {
    modules[name] = text;
  };
})();
