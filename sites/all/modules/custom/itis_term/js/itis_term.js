(function($){
  // itis_term object.
  Drupal.itis_term = Drupal.itis_term || {};
  Drupal.itis_term.update_name_field = function(event){
    var taxon_name = '';
    if($('[name^="field_unit_name1"]').val()) {
      if($('[name^="field_unit_indicator1"]').val() != '_none') {
        taxon_name += $('[name^="field_unit_indicator1"]').val() + ' ';
      }
      taxon_name += $.trim($('[name^="field_unit_name1"]').val()) + ' ';
      if($('[name^="field_unit_name2"]').val()) {
        if($('[name^="field_unit_indicator2"]').val() != '_none') {
          taxon_name += $('[name^="field_unit_indicator2"]').val() + ' ';
        }
        taxon_name += $.trim($('[name^="field_unit_name2"]').val()) + ' ';
        if($('#edit-field-unit-name3 input').val()) {
          if($('[name^="field_unit_indicator3"]').val() != '_none') {
            taxon_name += $('[name^="field_unit_indicator3"]').val() + ' ';
          }
          taxon_name += $.trim($('[name^="field_unit_name3"]').val()) + ' ';
          if($('[name^="field_unit_name4"]').val()) {
            if($('[name^="field_unit_indicator4"]').val() != '_none') {
              taxon_name += $('[name^="field_unit_indicator4"]').val() + ' ';
            }
            taxon_name += $.trim($('[name^="field_unit_indicator4"]').val()) + ' ';
          }
        }
      }
    }
    $('[name="name"]').attr('value', $.trim(taxon_name));
    $('#tui-name-live h2').html($.trim(taxon_name));
  }
  // Attach behaviours.
  Drupal.behaviors.itis_term = {attach: function(context, settings){
    $('[name^="field_unit_name1"]', context).keyup(Drupal.itis_term.update_name_field);
    $('[name^="field_unit_name2"]', context).keyup(Drupal.itis_term.update_name_field);
    $('[name^="field_unit_name3"]', context).keyup(Drupal.itis_term.update_name_field);
    $('[name^="field_unit_name4"]', context).keyup(Drupal.itis_term.update_name_field);
    $('[name^="field_unit_name1"]', context).change(Drupal.itis_term.update_name_field);
    $('[name^="field_unit_name2"]', context).change(Drupal.itis_term.update_name_field);
    $('[name^="field_unit_name3"]', context).change(Drupal.itis_term.update_name_field);
    $('[name^="field_unit_name4"]', context).change(Drupal.itis_term.update_name_field);
    $('[name^="field_unit_indicator1"]', context).change(Drupal.itis_term.update_name_field);
    $('[name^="field_unit_indicator2"]', context).change(Drupal.itis_term.update_name_field);
    $('[name^="field_unit_indicator3"]', context).change(Drupal.itis_term.update_name_field);
    $('[name^="field_unit_indicator4"]', context).change(Drupal.itis_term.update_name_field);
  }}
})(jQuery);