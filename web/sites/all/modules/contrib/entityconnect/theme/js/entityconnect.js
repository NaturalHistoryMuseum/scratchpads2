(function ($) {
  Drupal.behaviors.entityconnect = {
    'attach': function(context) {
      ref_field_buttons = {};

      // Treatments for each widget type.
      // Autocomplete widget.
      $(".entityconnect-add.autocomplete.single-value", context).each(function() {
        $(this).insertAfter($(this).next().find("label"));
      });
      $(".entityconnect-edit.autocomplete.single-value", context).each(function() {
        $(this).insertAfter($(this).next().find("label"));
      });

      // Autocomplete tags style widget.
      $(".entityconnect-add.textfield", context).each(function() {
        $(this).insertAfter($(this).next().find("label"));
      });
      $(".entityconnect-edit.textfield", context).each(function() {
        $(this).insertAfter($(this).next().find("label"));
      });
      // Select widget.
      $(".entityconnect-add.select", context).each(function() {
        $(this).insertAfter($(this).next().find("label"));
      });
      $(".entityconnect-edit.select.single-value", context).each(function() {
        $(this).insertAfter($(this).next().find("label"));
      });
      // Radios widget.
      $(".entityconnect-edit.radios", context).each(function() {
        $(this).insertBefore($(this).siblings("div.form-type-radios").find("label").first());
      });
      $(".entityconnect-add.radios", context).each(function() {
        $(this).insertBefore($(this).siblings("div.form-type-radios").find("label").first());
      });
      // Checkboxes widget.
      $(".entityconnect-edit.checkboxes", context).each(function() {
        $(this).insertBefore($(this).siblings("div.form-type-checkboxes").find("label").first());
      });
      $(".entityconnect-add.checkboxes", context).each(function() {
        $(this).insertBefore($(this).siblings("div.form-type-checkboxes").find("label").first());
      });

      // Edit button control.
      $(".entityconnect-edit input").click(function() {

        var wrapper = $(this).parents(".entityconnect-edit");

        text = $(wrapper).siblings("[type='text']");
        radio = $(wrapper).siblings("[type='radio']");
        checkbox = $(wrapper).siblings("[type='checkbox']");
        select = $(wrapper).siblings("select");

        if(text.length == 0) {
          text = $(wrapper).siblings().find("[type='text']");
        }
        if(radio.length == 0) {
          radio = $(wrapper).siblings().find("[type='radio']");
        }
        if(checkbox.length == 0) {
          checkbox = $(wrapper).siblings().find("[type='checkbox']");
        }
        if(select.length == 0) {
          select = $(wrapper).siblings().find("select > option:selected");
        }

        if($.trim($(text).val()) == ''
            && $.trim($(radio).val()) == ''
            && $.trim($(select).val()) == ''
            && $.trim($(checkbox).val()) == '') {
          return false;
        }
        return true;
      });
    }
  };
})(jQuery);
