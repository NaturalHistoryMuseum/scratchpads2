
Drupal.HierarchicalSelectConfigForm = {};

(function ($, cfg) {

cfg.context = function(configId) {
  if (configId === undefined) {
    return $('.hierarchical-select-config-form > *').not('.live-preview');
  }
  else {
    return $('#hierarchical-select-config-form-'+ configId + ' > *').not('.live-preview');
  }
};

cfg.levelLabels = function(configId) {
  var $status = $('.level-labels-status', cfg.context(configId));
  var $enforceDeepest = $('.enforce-deepest input', cfg.context(configId));

  var showHide = function(speed) {
    $affected = $('.level-labels-settings', cfg.context(configId)); 
    if (!$status.is(':checked')) {
      $affected.hide(speed);
    }
    else {
      // For showing/hiding rows, I'm relying on setting the style
      // "display: none" and removing it again. jQuery's show()/hide() leave
      // "display: block" behind and are thereby messing up the table layout.
      if ($enforceDeepest.slice(1, 2).is(':checked')) {
        $affected.find('tr').removeAttr('style');
      }
      else {
        // We need to take special measures if sticky headers are enabled, so
        // handle the show/hide separately when it's enabled.
        if ($affected.find('table.sticky-header').length == 0) {
          $affected.find('tr').slice(0, 2).removeAttr('style'); // Show header tr and root level tr.
          $affected.find('tr').slice(2).attr('style', 'display: none'); // Hide all other tr's.
        }
        else {
          $affected.find('table').show(speed); // Show both tables (the one with the sticky headers and the one with the actual content).
          $affected.find('table').slice(1).find('tr').slice(2).attr('style', 'display: none'); // Show all tr's after the header tr and root level tr of the 2nd table (the one with the actual content).
        }
      }

      // If $status was unchecked previously, the entire div would have been
      // hidden!
      if ($affected.css('display') == 'none') {
        $affected.show(speed);
      }
    }
  };

  $status.click(function() { showHide(200); });
  $enforceDeepest.click(function() { showHide(200); });
  showHide(0);
};

cfg.dropbox = function(configId) {
  var $status = $('.dropbox-status', cfg.context(configId));

  var showHide = function(speed) {
    var $affected = $('.dropbox-title, .dropbox-limit, .dropbox-reset-hs', cfg.context(configId)).parent();
    if ($status.is(':checked')) {
      $affected.show(speed);
    }
    else {
      $affected.hide(speed);
    }
  };

  $status.click(function() { showHide(200); });
  showHide(0);
};

cfg.editability = function(configId) {
  var $status = $('.editability-status', cfg.context(configId));
  var $allowNewLevels = $('.editability-allow-new-levels', cfg.context(configId)); 

  var showHide = function(speed) {
    var $affected = $('.editability-per-level-settings, .form-item:has(.editability-allow-new-levels)', cfg.context(configId));
    var $maxLevels = $('.form-item:has(.editability-max-levels)', cfg.context(configId));
    if ($status.is(':checked')) {
      if ($allowNewLevels.is(':checked')) {
        $affected.add($maxLevels).show(speed);
      }
      else {
        $affected.show(speed);
      }
    }
    else {
      $affected.add($maxLevels).hide(speed);
    }
  };

  var showHideMaxLevels = function(speed) {
    $affected = $('.editability-max-levels', cfg.context(configId)).parent();
    if ($allowNewLevels.is(':checked')) {
      $affected.show(speed);
    }
    else {
      $affected.hide(speed);
    }
  };

  $status.click(function() { showHide(200); });
  $allowNewLevels.click(function() { showHideMaxLevels(200); });
  showHideMaxLevels(0);
  showHide(0);
};

cfg.livePreview = function(configId) {
  // React on changes to any input, except the ones in the live preview.
  $updateLivePreview = $('input', cfg.context(configId))
  .filter(':not(.create-new-item-input):not(.create-new-item-create):not(.create-new-item-cancel)')
  .change(function() {
    // TODO: Do an AJAX submit of the entire form.
  });  
};

$(document).ready(function() {
  for (var id in Drupal.settings.HierarchicalSelect.configForm) {
    var configId = Drupal.settings.HierarchicalSelect.configForm.id;

    cfg.levelLabels(configId);
    cfg.dropbox(configId);
    cfg.editability(configId);
    //cfg.livePreview(configId);
  }
});

})(jQuery, Drupal.HierarchicalSelectConfigForm);
