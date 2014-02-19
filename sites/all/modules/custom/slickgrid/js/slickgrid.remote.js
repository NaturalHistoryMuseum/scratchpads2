(function($){
  /**
   * RemoteModel
   *
   * This class is used to load slickgrid data on demand
   */
  function RemoteModel(viewName, commandHandler){
    /*
     * Settings
     */
    var base_url = Drupal.settings.slickgrid.get_data_callback_url + viewName + "/";

    /*
     * Private variables
     */
    var data = {length: 0};
    var validData = [];
    var lockedRows = {};
    var sortcol = null;
    var filters = {};
    var sortdir = 1;
    var requestCounter = 0;
    var total_row_count = -1;

    /*
     * Events
     */
    var onDataLoading = new Slick.Event();
    var onDataLoaded = new Slick.Event();
    var timeoutID = 0;

    /**
     * isDataLoaded
     *
     * Return TRUE if the data in the given range has been loaded.
     */
    function isDataLoaded(from, to){
      var invalid = getInvalidRange(from, to);
      return !invalid;
    }

    /**
     * clear
     *
     *  Empty all data
     */
    function clear(){
      for( var key in data) {
        delete data[key];
      }
      validData = [];
      data.length = 0;
    }

    /**
     * reloadData
     *
     * Invalidate and reload the data in the given range
     */
    function reloadData(from, to){
      invalidateRange(from, to);
      ensureData(from, to);
    }

    /**
     * ensureData
     *
     * Ensure that the given data is available
     */
    function ensureData(from, to, page_size){
      if (typeof page_size == 'undefined'){
        page_size = 1;
      }
      from = Math.floor(from / page_size) * page_size;
      to = (Math.floor(to / page_size) + 1) * page_size - 1;
      // Find invalid data within the range
      var invalid = getInvalidRange(from, to);
      if (!invalid){
        return;
      }
      from = Math.floor(invalid.from / page_size) * page_size;
      to = (Math.floor(invalid.to / page_size) + 1) * page_size - 1;
      // Create request
      var url = base_url + from.toString() + "/" + (to - from + 1).toString();
      if(sortcol) {
        url += "/" + sortcol + "/" + ((sortdir > 0) ? "asc" : "desc");
      }
      requestCounter++;
      commandHandler.addCommand('reload', {
        dataType: 'json',
        url: url,
        data: filters,
        success: $.proxy(onSuccess, this, from, to, requestCounter),
        error: $.proxy(onError, this, requestCounter),
        slickCommandStart: $.proxy(onLoadStart, this, from, to, requestCounter)
      });
    }

    /**
     * onLoadStart
     *
     * This is invoked by the command handler when the loading actually starts
     */
    function onLoadStart(from, to, request_id){
      // Add the range as valid now, but ensure any locked row is not included.
      addValidRange(from, to, request_id, false);
      for (var i in lockedRows){
        if ((lockedRows[i] == -1 || lockedRows[i] >= request_id) && i >= from && i <= to){
          invalidateRange(i, i);
        }
      }
      onDataLoading.notify({
        from: from,
        to: to
      });
    }

    /**
     * onError
     *
     * This is invoked when the load request has failed
     */
    function onError(request_id){
      removeRequestFromValidData(request_id);
    }

    /**
     * onSuccess
     *
     * This is invoked when the load request has succesfully completed
     */
    function onSuccess(from, to, request_id, resp){
      if (typeof resp.data == 'undefined' || resp.data === null){
        return;
      }
      if (from + resp.data.length - 1 < to){
        // We've hit the last row - store the full count.
        total_row_count = from + resp.data.length;
      }
      to = from + resp.data.length - 1;
      removeRequestFromValidData(request_id);
      addValidRange(from, to, request_id, true);
      for (var i = 0; i < resp.data.length; i++) {
        if (typeof lockedRows[from + i] !== 'undefined'){
          if (lockedRows[from + i] == -1 || lockedRows[from + i] >= request_id){
            continue;
          } else {
            delete lockedRows[from + i];
          }
        }
        if (typeof data[from + i] == 'undefined'){
          data.length++;
        }
        data[from + i] = resp.data[i];
        data[from + i].index = from + i;
      }
      onDataLoaded.notify({from: from, to: to});
    }

    /**
     * getRowById
     *
     * Return the given row from it's id
     */
    function getRowById(id){
      for( var i = 0; i < data.length; i++) {
        if(data[i].id == id) {
          return i;
        }
      }
      return false;
    }

    /**
     * setSort
     *
     */
    function setSort(column, dir){
      sortcol = column;
      sortdir = dir;
      clear();
    }

    /**
     * setFilters
     *
     */
    function setFilters(fltrs, no_reload){
      var doSetFilters = false;
      var keys = Object.keys(fltrs);
      for( var i in keys) {
        if(fltrs[keys[i]] !== filters[keys[i]]) {
          doSetFilters = true;
          filters[keys[i]] = fltrs[keys[i]];
          total_row_count = -1;
        }
      }
      if(doSetFilters && (typeof no_reload == 'undefined' || !no_reload)) {
        window.setTimeout(function(){
          commandHandler.cancelReload();
          clear();
          ensureData(0, 50, 50);
        }, 0);
      }
    }

    /**
     * lockRow
     *
     * Lock a row so it won't be over-written when loading.
     * If a loading occurs while a row is locked, the row
     * will be marked as invalid.
     */
    function lockRow(row){
      lockedRows[row] = -1;
    }

    /**
     * unlockRow
     *
     * Unlock a row. Note that only a load that is queued
     * after a row is unlocked can overwrite the row.
     */
    function unlockRow(row){
      lockedRows[row] = requestCounter;
    }

    /**
     * isRowLocked
     *
     * Return true if the given row is locked
     */
    function isRowLocked(row){
      return typeof lockedRows[row] !== 'undefined';
    }

    /**
     * isRowLoading
     *
     * Return true if the given row is currently loading
     */
    function isRowLoading(row){
      for (var i = 0; i < validData.length; i++){
        if (row >= validData[i].from && row <= validData[i].to){
          return !validData[i].loaded;
        }
      }
      return false;
    }

    /**
     * isRowInvalid
     *
     * Return true if the row is invalid. Note that this will
     * return false if we've started loading but don't yet have
     * the data - use isRowLoading to check for that eventuality.
     */
    function isRowInvalid(row){
      for (var i = 0; i < validData.length; i++){
        if (row >= validData[i].from && row <= validData[i].to){
          return false;
        }
      }
      return true;
    }

    /**
     * removeRequestFromValidData
     *
     * Remove data that was generated by the given request from the list
     * of valid data (unless it is marked as loaded)
     */
    function removeRequestFromValidData(request_id){
      var new_valid = [];
      for (var i = 0; i < validData.length; i++){
        if (validData[i].request_id != request_id || validData[i].loaded){
          new_valid.push(validData[i]);
        }
      }
      validData = new_valid;
    }

    /**
     * getInvalidRange
     *
     * Given a range of data (from, to) return a range that encompasses all invalid
     * data within that range.
     */
    function getInvalidRange(from, to){
      if (total_row_count == 0){
        return false;
      } else if (total_row_count > 0 && to >= total_row_count){
        to = total_row_count - 1;
      }
      for (var i = 0; i < validData.length; i++){
        if (validData[i].to < from){
          continue;
        }
        if (validData[i].from > to){
          break;
        }
        if (validData[i].from <= from){
          if (validData[i].to >= to){
            return false;
          }
          from = validData[i].to + 1;
        }
        if (validData[i].to >= to){
          to = validData[i].from - 1;
        }
      }
      return {
        from: from,
        to: to
      };
    }

    /**
     * invalidateRange
     *
     * Remove the given range from the list of valid data
     */
    function invalidateRange(from, to){
      var new_valid = [];
      for (var i = 0; i < validData.length; i++){
        if (validData[i].to < from){
          new_valid.push(validData[i]);
        } else if (validData[i].from > to){
          new_valid.push(validData[i]);
        } else {
          if (from > validData[i].from){
            new_valid.push({
              from: validData[i].from,
              to: from - 1,
              loaded: validData[i].loaded,
              request_id: validData[i].request_id
            });
          }
          if (to < validData[i].to){
            new_valid.push({
              from: to + 1,
              to: validData[i].to,
              loaded: validData[i].loaded,
              request_id: validData[i].request_id
            });
          }
        }
      }
      validData = new_valid;
    }

    /**
     * addValidRange
     *
     * Add a range to the list of valid data ranges
     */
    function addValidRange(from, to, request_id, loaded){
      var new_valid = [];
      var insert = {
        from: from,
        to: to,
        loaded: loaded,
        request_id: request_id
      };
      for (var i = 0; i < validData.length; i++){
        if (validData[i].to < from){
          new_valid.push(validData[i]);
        } else if (validData[i].from > to){
          if (insert !== false){
            new_valid.push(insert);
            insert = false;
          }
          new_valid.push(validData[i]);
        } else {
          if (validData[i].from < from){
            new_valid.push({
              from: validData[i].from,
              to: from - 1,
              loaded: validData[i].loaded,
              request_id: validData[i].request_id
            });
          }
          if (i+1 < validData.length && validData[i+1].from < to){
            new_valid.push({
              from: from,
              to: validData[i+1].from -1,
              loaded: loaded,
              request_id: request_id
            });
            insert = {
                from: validData[i+1].from,
                to: to,
                loaded: loaded,
                request_id: request_id
            };
          } else {
            new_valid.push(insert);
            insert = false;
          }
          if (validData[i].to > to){
            new_valid.push({
              from: to + 1,
              to: validData[i].to,
              loaded: validData[i].loaded,
              request_id: validData[i].request_id
            });
          }
        }
      }
      if (insert !== false){
        new_valid.push(insert);
      }
      validData = new_valid;
    }
    return {
    // properties
    "data": data,
    // methods
    "lockRow": lockRow, "unlockRow": unlockRow, "isRowLocked": isRowLocked, "isRowLoading": isRowLoading, "isRowInvalid": isRowInvalid, "invalidateRange": invalidateRange, "clear": clear, "isDataLoaded": isDataLoaded, "ensureData": ensureData, "reloadData": reloadData, "setSort": setSort, "setFilters": setFilters, "getRowById": getRowById,
    // events
    "onDataLoading": onDataLoading, "onDataLoaded": onDataLoaded};
  }
  // Slick.Data.RemoteModel
  $.extend(true, window, {Slick: {Data: {RemoteModel: RemoteModel}}});
})(jQuery);