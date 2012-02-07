
/**
 * @file
 * Cache system for Hierarchical Select.
 * This cache system takes advantage of the HTML 5 client-side database
 * storage specification to reduce the number of queries to the server. A lazy
 * loading strategy is used.
 */


/**
 * Note: this cache system can be replaced by another one, as long as you
 * provide the following methods:
 *  - initialize()
 *  - status()
 *  - load()
 *  - sync()
 *  - updateHierarchicalSelect()
 *
 * TODO: better documentation
 */

(function ($) {

Drupal.HierarchicalSelect.cache = {};

Drupal.HierarchicalSelect.cache.initialize = function() {
  try {
    if (window.openDatabase) {
      this.db = openDatabase("Hierarchical Select", "3.x", "Hierarchical Select cache", 200000);

      this.db
      // Create the housekeeping table if it doesn't exist yet.
      .transaction(function(tx) {
        tx.executeSql("SELECT COUNT(*) FROM hierarchical_select", [], null, function(tx, error) {
          tx.executeSql("CREATE TABLE hierarchical_select (table_name TEXT UNIQUE, expires REAL)", []);
          console.log("Created housekeeping table.");
        });        
      })
      // Empty tables that have expired, based on the information in the
      // housekeeping table.
      .transaction(function(tx) {
        tx.executeSql("SELECT table_name FROM hierarchical_select WHERE expires < ?", [ new Date().getTime() ], function(tx, resultSet) {
          for (var i = 0; i < resultSet.rows.length; i++) {
            var row = resultSet.rows.item(i);
            var newExpiresTimestamp = new Date().getTime() + 86400;

            tx.executeSql("DELETE * FROM " + row.table_name);
            tx.executeSql("UPDATE hierarchical_select SET expires = ? WHERE table_name = ?", [ newExpiresTimestamp, row.table_name ]);

            console.log("Table "+ row.table_name +" was expired: emptied it. Will expire again in "+ (newExpiresTimestamp - new Date().getTime()) / 3600 +" hours.");
          }
        });
      });
    }
    else {
      this.db = false;
    }
  }
  catch(err) { }
};

Drupal.HierarchicalSelect.cache.status = function() {
  return Drupal.HierarchicalSelect.cache.db !== false;
};

Drupal.HierarchicalSelect.cache.table = function(hsid) {
  return Drupal.settings.HierarchicalSelect.settings[hsid].cacheId;
};

Drupal.HierarchicalSelect.cache.load = function(hsid) {
  // If necessary, create the cache table for the given Hierarchical Select.
  Drupal.HierarchicalSelect.cache.db.transaction(function(tx) {
    var table = Drupal.HierarchicalSelect.cache.table(hsid);

    tx.executeSql("SELECT value FROM "+ table, [], function(tx, resultSet) {
      console.log("" + resultSet.rows.length + " cached items in the " + table + " table.");
    }, function(tx, error) {
      var expiresTimestamp = new Date().getTime() + 86400;

      tx.executeSql("CREATE TABLE "+ table +" (parent REAL, value REAL UNIQUE, label REAL, weight REAL)");
      tx.executeSql("INSERT INTO hierarchical_select (table_name, expires) VALUES (?, ?)", [ table, expiresTimestamp ]);

      console.log("Created table "+ table +", will expire in "+ (expiresTimestamp - new Date().getTime()) / 3600 +" hours.");
    });
  });
};

Drupal.HierarchicalSelect.cache.insertOnDuplicateKeyUpdate = function(table, row) {
//  console.log("storing: value: "+ row.value +", label: "+ row.label +", parent: "+ row.parent +", weight: "+ row.weight);
  Drupal.HierarchicalSelect.cache.db.transaction(function(tx) {
    tx.executeSql("INSERT INTO "+ table +" (parent, value, label, weight) VALUES (?, ?, ?, ?)", [ row.parent, row.value, row.label, row.weight ], null, function(tx, error) {
//      console.log("UPDATING value: "+ row.value +", label: "+ row.label +", parent: "+ row.parent +", weight: "+ row.weight);
      tx.executeSql("UPDATE "+ table +" SET parent = ?, label = ?, weight = ? WHERE value = ?", [ row.parent, row.label, row.weight, row.value ], null, function(tx, error) {
//        console.log("sql error: " + error.message);
      });
    });
  });
};

Drupal.HierarchicalSelect.cache.sync = function(hsid, info) {
  var table = Drupal.HierarchicalSelect.cache.table(hsid);
  for (var id in info) {
    var closure = function(_info, id) {
      Drupal.HierarchicalSelect.cache.insertOnDuplicateKeyUpdate(table, _info[id]);
    } (info, id);
  }  
};

Drupal.HierarchicalSelect.cache.hasChildren = function(hsid, value, successCallback, failCallback) {
  var table = Drupal.HierarchicalSelect.cache.table(hsid);
  Drupal.HierarchicalSelect.cache.db.transaction(function(tx) {
    tx.executeSql("SELECT * FROM "+ table +" WHERE parent = ?", [ value ], function(tx, resultSet) {
      if (resultSet.rows.length > 0) {
        successCallback();
      }
      else {
        failCallback();
      }
    });
  });
};

Drupal.HierarchicalSelect.cache.getSubLevels = function(hsid, value, callback, previousSubLevels) {
  var table = Drupal.HierarchicalSelect.cache.table(hsid);

  var subLevels = new Array();
  if (previousSubLevels != undefined) {
    subLevels = previousSubLevels;
  }

  Drupal.HierarchicalSelect.cache.db.transaction(function(tx) {
    tx.executeSql("SELECT value, label FROM "+ table +" WHERE parent = ? ORDER BY weight", [ value ], function(tx, resultSet) {
      var numChildren = resultSet.rows.length;

      // If there's only one child, check if it has the dummy "<value>-has-no-children" value.
      if (numChildren == 1) {
        var valueOfFirstRow = String(resultSet.rows.item(0).value);
        var isDummy = valueOfFirstRow.match(/^.*-has-no-children$/);
      }

      // Only pass the children if there are any (and not a fake one either).
      if (numChildren && !isDummy) {
        var level = new Array();
        for (var i = 0; i < resultSet.rows.length; i++) {
          var row = resultSet.rows.item(i);
          level[i] = { 'value' : row.value, 'label' : row.label };
          console.log("child of "+ value +": ("+ row.value +", "+ row.label +")");
        }

        subLevels.push(level);

        Drupal.HierarchicalSelect.cache.getSubLevels(hsid, level[0].value, callback, subLevels);
      }
      else {
        if (subLevels.length > 0) {
          callback(subLevels);
        }
        else {
          callback(false);
        }
      }
    });
  });
};

Drupal.HierarchicalSelect.cache.createAndUpdateSelects = function(hsid, subLevels, lastUnchanged) {
  // Remove all levels below the level in which a value was selected, if they
  // exist.
  // Note: the root level can never change because of this!
  $('#hierarchical-select-'+ hsid +'-wrapper .hierarchical-select .selects select').slice(lastUnchanged).remove();

  // Create the new sublevels, by cloning the root level and then modifying
  // that clone.
  var $rootSelect = $('#hierarchical-select-'+ hsid +'-wrapper .hierarchical-select .selects select:first');
  for (var depth in subLevels) {
    var optionElements = $.map(subLevels[depth], function(item) { return '<option value="'+ item.value +'">'+ item.label +'</option>'; });

    var level = parseInt(lastUnchanged) + parseInt(depth);

    $('#hierarchical-select-'+ hsid +'-wrapper .hierarchical-select .selects select:last').after(
      $rootSelect.clone()
      // Update the name attribute.
      .attr('name', $rootSelect.attr('name').replace(/(.*)\d+\]$/, "$1"+ level +"]"))
      // Update the id attribute.
      .attr('id', $rootSelect.attr('id').replace(/(.*-hierarchical-select-selects-)\d+/, "$1"+ level))
      // Remove the existing options and set the new ones.
      .empty().append(optionElements.join(''))
    );
  }
};

Drupal.HierarchicalSelect.cache.updateHierarchicalSelect = function(hsid, value, settings, lastUnchanged, ajaxOptions) {
  // If the selected value has children
  Drupal.HierarchicalSelect.cache.hasChildren(hsid, value, function() {
    console.log("Cache hit.");
    Drupal.HierarchicalSelect.cache.getSubLevels(hsid, value, function(subLevels) {
      Drupal.HierarchicalSelect.preUpdateAnimations(hsid, 'update-hierarchical-select', lastUnchanged, function() {        
        if (subLevels !== false) {
          Drupal.HierarchicalSelect.cache.createAndUpdateSelects(hsid, subLevels, lastUnchanged);              
        }
        else {
          // Nothing must happen: the user selected a value that doesn't
          // have any subLevels.
          $('#hierarchical-select-' + hsid + '-wrapper .hierarchical-select .selects select').slice(lastUnchanged).remove();
        }

        Drupal.HierarchicalSelect.postUpdateAnimations(hsid, 'update-hierarchical-select', lastUnchanged, function() {
          // Reattach the bindings.
          Drupal.HierarchicalSelect.attachBindings(hsid);

          Drupal.HierarchicalSelect.triggerEvents(hsid, 'update-hierarchical-select', settings);

          // The selection of this hierarchical select has changed!
          Drupal.HierarchicalSelect.triggerEvents(hsid, 'change-hierarchical-select', settings);
        });
      });
    });   
  }, function() {
    // This item was not yet requested before, so we still have to perform
    // the dynamic form submit.
    console.log("Cache miss. Querying the server.");
    Drupal.HierarchicalSelect.preUpdateAnimations(hsid, 'update-hierarchical-select', lastUnchanged, function() {
      $.ajax(ajaxOptions); 
    });
  });
};

})(jQuery);
