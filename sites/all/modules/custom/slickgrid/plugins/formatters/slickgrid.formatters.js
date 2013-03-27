var collapsibleFormatter = function(row, cell, value, columnDef, dataContext){
  if(value === null) {
    return;
  }
  var spacer = "<span style='display:inline-block;height:1px;width:" + (15 * dataContext["indent"]) + "px'></span>";
  var idx = dataView.getIdxById(dataContext.id);
  if(data[idx + 1] && data[idx + 1].indent > data[idx].indent) {
    if(dataContext._collapsed)
      return spacer + " <span class='toggle expand'></span>&nbsp;" + value;
    else
      return spacer + " <span class='toggle collapse'></span>&nbsp;" + value;
  } else
    return spacer + " <span class='toggle'></span>&nbsp;" + value;
};