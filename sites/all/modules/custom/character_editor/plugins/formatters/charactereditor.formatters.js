var characterCollapsedItems = [];
var characterCollapsibleFormatter = function(row, cell, value, columnDef, dataContext){
  if(value === null) {
    return;
  }
  var depth = jQuery(value).attr('depth');
  var tree_state = jQuery(value).attr('treestate');
  dataContext._tree_state = tree_state;
  var spacer = "<span style='display:inline-block;height:1px;width:" + (15 * depth) + "px'></span>";
  if(tree_state == 'expanded') {
    return spacer + " <span class='characterToggle toggle collapse'></span>&nbsp;" + value;
  } else if (tree_state == 'collapsed'){
    if (characterCollapsedItems.indexOf(dataContext.id) == -1){
      characterCollapsedItems.push(dataContext.id);
    }
    return spacer + " <span class='characterToggle toggle expand'></span>&nbsp;" + value;
  } else {
    return spacer + " <span class='toggle'></span>&nbsp;" + value;
  }
};