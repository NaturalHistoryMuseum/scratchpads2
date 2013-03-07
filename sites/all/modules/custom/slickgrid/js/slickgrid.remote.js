(function($){
  function RemoteModel(viewName){
    // private
    var PAGESIZE = 50;
    var data = {length: 0};
    var sortcol = null;
    var filters = {};
    var sortdir = 1;
    var h_request = null;
    var req = null; // ajax request
    // events
    var onDataLoading = new Slick.Event();
    var onDataLoaded = new Slick.Event();
    var fromPage = 0;
    var toPage = 0;
    var timeoutID = 0;
    var base_url = Drupal.settings.slickgrid.get_data_callback_url + viewName + "/";
    function init(){}
    function isDataLoaded(from, to){
      for( var i = from; i <= to; i++) {
        if(data[i] == undefined || data[i] == null) {
          return false;
        }
      }
      return true;
    }
    function clear(){
      for( var key in data) {
        delete data[key];
      }
      data.length = 0;
      fromPage = 0;
      toPage = 0;
    }
    function ensureData(from, to){
      if(req) {
        req.abort();
        for( var i = req.fromPage; i <= req.toPage; i++)
          data[i * PAGESIZE] = undefined;
      }
      if(from < 0) {
        from = 0;
      }
      fromPage = Math.floor(from / PAGESIZE);
      toPage = Math.floor(to / PAGESIZE);
      while(data[fromPage * PAGESIZE] !== undefined && fromPage < toPage)
        fromPage++;
      while(data[toPage * PAGESIZE] !== undefined && fromPage < toPage)
        toPage--;
      if(fromPage > toPage || ((fromPage == toPage) && (data[fromPage * PAGESIZE] !== undefined && data[fromPage * PAGESIZE] !== null))) {
        // TODO: look-ahead
        return;
      }
      var url = base_url + (fromPage * PAGESIZE) + "/" + PAGESIZE;
      if(sortcol) {
        url += "/" + sortcol + "/" + ((sortdir > 0) ? "asc" : "desc");
      }
      if(h_request != null) {
        clearTimeout(h_request);
      }
      h_request = setTimeout(function(){
        for( var i = fromPage; i <= toPage; i++)
          data[i * PAGESIZE] = null; // null indicates a 'requested but not
        // available yet'
        onDataLoading.notify({from: from, to: to});
        req = $.getJSON(url, filters, onSuccess);
        req.fromPage = fromPage;
        req.toPage = toPage;
      }, 50);
    }
    function onError(fromPage, toPage){
      alert("error loading pages " + fromPage + " to " + toPage);
    }
    function onSuccess(resp){
      var from = fromPage * PAGESIZE;
      var to = from + resp.data.length;
      data.length += resp.data.length;
      for( var i = 0; i < resp.data.length; i++) {
        data[from + i] = resp.data[i];
        data[from + i].index = from + i;
      }
      req = null;
      onDataLoaded.notify({from: 0, to: to});
    }
    function reloadData(from, to){
      for( var i = from; i <= to; i++) {
        if(data[i]) {
          data.length--;
          delete data[i];
        }
      }
      ensureData(from, to);
    }
    function setSort(column, dir){
      sortcol = column;
      sortdir = dir;
      clear();
    }
    function getRowById(id){
      for( var i = 0; i < data.length; i++) {
        if(data[i].id == id) {
          return i;
        }
      }
      return false;
    }
    function setFilters(fltrs){
      var doSetFilters = false;
      var keys = Object.keys(fltrs);
      for( var i in keys) {
        if(fltrs[keys[i]] !== filters[keys[i]]) {
          doSetFilters = true;
          filters[keys[i]] = fltrs[keys[i]];
        }
      }
      if(doSetFilters) {
        var this_copy = this;
        this.clear();
        window.clearTimeout(timeoutID);
        timeoutID = window.setTimeout(function(){
          this_copy.ensureData(0, PAGESIZE);
        }, 700);
      }
    }
    init();
    return {
    // properties
    "data": data,
    // methods
    "clear": clear, "isDataLoaded": isDataLoaded, "ensureData": ensureData, "reloadData": reloadData, "setSort": setSort, "setFilters": setFilters, "getRowById": getRowById,
    // events
    "onDataLoading": onDataLoading, "onDataLoaded": onDataLoaded};
  }
  // Slick.Data.RemoteModel
  $.extend(true, window, {Slick: {Data: {RemoteModel: RemoteModel}}});
})(jQuery);