(function($){
  /*****************************************************************************
   * A sample AJAX data store implementation. Right now, it's hooked up to load
   * all Apple-related Digg stories, but can easily be extended to support and
   * JSONP-compatible backend that accepts paging parameters.
   */
  function RemoteModel(viewName){
    // private
    var PAGESIZE = 50;
    var data = {length: 0};
    var sortcol = null;
    var filters = null;
    var sortdir = 1;
    var h_request = null;
    var req = null; // ajax request
    // events
    var onDataLoading = new Slick.Event();
    var onDataLoaded = new Slick.Event();
    var fromPage = 0;
    var toPage = 0;
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
      if(fromPage > toPage || ((fromPage == toPage) && data[fromPage * PAGESIZE] !== undefined)) {
        // TODO: look-ahead
        return;
      }
      var url = Drupal.settings.slickgrid.get_data_callback_url + viewName + "/" + fromPage;
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
      var to = from + resp.length;
      data.length += resp.length;
      for( var i = 0; i < resp.length; i++) {
        data[from + i] = resp[i];
        data[from + i].index = from + i;
      }
      req = null;
      onDataLoaded.notify({from: 0, to: to});
    }
    function reloadData(from, to){
      for( var i = from; i <= to; i++)
        delete data[i];
      ensureData(from, to);
    }
    function setSort(column, dir){
      sortcol = column;
      sortdir = dir;
      clear();
    }
    function setFilters(fltrs){
      console.log(fltrs);
      filters = fltrs;
    }
    init();
    return {
    // properties
    "data": data,
    // methods
    "clear": clear, "isDataLoaded": isDataLoaded, "ensureData": ensureData, "reloadData": reloadData, "setSort": setSort, "setFilters": setFilters,
    // events
    "onDataLoading": onDataLoading, "onDataLoaded": onDataLoaded};
  }
  // Slick.Data.RemoteModel
  $.extend(true, window, {Slick: {Data: {RemoteModel: RemoteModel}}});
})(jQuery);