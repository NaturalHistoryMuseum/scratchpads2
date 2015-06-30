/**
 * @file
 * Enhancements for select list configuration options.
 */

(function ($) {

  "use strict";

  Drupal.behaviors.webformSelectLoadOptions = {};
  Drupal.behaviors.webformSelectLoadOptions.attach = function (context) {

    $('#edit-extra-options-source', context).change(function () {
      var url = Drupal.settings.webform.selectOptionsUrl + '/' + this.value;
      $.ajax({
        url: url,
        success: Drupal.webform.selectOptionsLoad,
        dataType: 'json'
      });
    });
  };

  Drupal.webform = Drupal.webform || {};

  Drupal.webform.selectOptionsOriginal = false;
  Drupal.webform.selectOptionsLoad = function (result) {
    if (Drupal.optionsElement) {
      if (result.options) {
        // Save the current select options the first time a new list is chosen.
        if (Drupal.webform.selectOptionsOriginal === false) {
          Drupal.webform.selectOptionsOriginal = $(Drupal.optionElements[result.elementId].manualOptionsElement).val();
        }
        $(Drupal.optionElements[result.elementId].manualOptionsElement).val(result.options);
        Drupal.optionElements[result.elementId].disable();
        Drupal.optionElements[result.elementId].updateWidgetElements();
      }
      else {
        Drupal.optionElements[result.elementId].enable();
        if (Drupal.webform.selectOptionsOriginal) {
          $(Drupal.optionElements[result.elementId].manualOptionsElement).val(Drupal.webform.selectOptionsOriginal);
          Drupal.optionElements[result.elementId].updateWidgetElements();
          Drupal.webform.selectOptionsOriginal = false;
        }
      }
    }
    else {
      var $element = $('#' + result.elementId);
      $element.webformProp('readonly', result.options);
      if (result.options) {
        $element.val(result.options);
      }
    }
  }

  /**
   * Make a prop shim for jQuery < 1.9.
   */
  $.fn.webformProp = $.fn.webformProp || function (name, value) {
    if (value) {
      return $.fn.prop ? this.prop(name, true) : this.attr(name, true);
    }
    else {
      return $.fn.prop ? this.prop(name, false) : this.removeAttr(name);
    }
  };

})(jQuery);
