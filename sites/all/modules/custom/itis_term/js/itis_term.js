(function($){
  // itis_term object.
  Drupal.itis_term = Drupal.itis_term || {};
  Drupal.itis_term.update_name_field = function(event){
    var taxon_name = '';
    if($('#edit-field-unit-indicator1 select').val() != '_none') {
      taxon_name += $('#edit-field-unit-indicator1 select').val() + ' ';
    }
    if($('#edit-field-unit-name1 input').val()) {
      taxon_name += $.trim($('#edit-field-unit-name1 input').val()) + ' ';
    }
    if($('#edit-field-unit-indicator2 select').val() != '_none') {
      taxon_name += $('#edit-field-unit-indicator2 select').val() + ' ';
    }
    if($('#edit-field-unit-name2 input').val()) {
      taxon_name += $.trim($('#edit-field-unit-name2 input').val()) + ' ';
    }
    if($('#edit-field-unit-indicator3 select').val() != '_none') {
      taxon_name += $('#edit-field-unit-indicator3 select').val() + ' ';
    }
    if($('#edit-field-unit-name3 input').val()) {
      taxon_name += $.trim($('#edit-field-unit-name3 input').val()) + ' ';
    }
    if($('#edit-field-unit-indicator4 select').val() != '_none') {
      taxon_name += $('#edit-field-unit-indicator4 select').val() + ' ';
    }
    if($('#edit-field-unit-name4 input').val()) {
      taxon_name += $.trim($('#edit-field-unit-name4 input').val()) + ' ';
    }
    $('[name="name"]').attr('value', $.trim(taxon_name));
  }
  // Attach behaviours.
  Drupal.behaviors.itis_term = {attach: function(context, settings){
    $('#edit-field-unit-name1', context).keyup(Drupal.itis_term.update_name_field);
    $('#edit-field-unit-name2', context).keyup(Drupal.itis_term.update_name_field);
    $('#edit-field-unit-name3', context).keyup(Drupal.itis_term.update_name_field);
    $('#edit-field-unit-name4', context).keyup(Drupal.itis_term.update_name_field);
    $('#edit-field-unit-indicator1', context).change(Drupal.itis_term.update_name_field);
    $('#edit-field-unit-indicator2', context).change(Drupal.itis_term.update_name_field);
    $('#edit-field-unit-indicator3', context).change(Drupal.itis_term.update_name_field);
    $('#edit-field-unit-indicator4', context).change(Drupal.itis_term.update_name_field);
  }}
})(jQuery);