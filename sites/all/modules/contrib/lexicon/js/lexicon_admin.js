
(function($) {
  function lexicon_mark_terms_handler(event) {
	// Disable term marking options and term marking indicator fieldsets if "mark terms in content" is not selected
    if ($("input[name=lexicon_mark_terms]:checked").val() == '1') {
	  $("div.fieldset_term_marking_options").show(1000);
	  $("div.fieldset_term_marking_indicator").show(1000);
	}
	else {
      $("div.fieldset_term_marking_options").hide(1000);
	  $("div.fieldset_term_marking_indicator").hide(1000);
	}
  }

  function lexicon_replace_handler(event) { 
	// Disable superscript field if not selected.
    if ($("input[name=lexicon_replace]:checked").val() == 'superscript') {
      $("input[name=lexicon_superscript]").parents("div.lexicon_superscript").show(1000);
    }
    else {
      $("input[name=lexicon_superscript]").parents("div.lexicon_superscript").hide(1000);
    }

    // Disable icon URL field if not selected.
    var selected = $("input[name=lexicon_replace]:checked").val();
    if (selected == 'icon' || selected == 'iconterm') {
      $("input[name=lexicon_icon]").parents("div.lexicon_icon").show(1000);
    }
    else {
      $("input[name=lexicon_icon]").parents("div.lexicon_icon").hide(1000);
    }

    // Enable the term link class if the selected indicator is not 'icon' or 'template'.
    var selected = $("input[name=lexicon_replace]:checked").val();
    if (selected == 'icon' || selected == 'template') {
      $("input[name=lexicon_term_class]").parents("div.lexicon_term_class").hide(1000);
    }
    else {
      $("input[name=lexicon_term_class]").parents("div.lexicon_term_class").show(1000);
    }
    
    // Disable the term link type and click behavior if the selected indicator is 'template'.
    var selected = $("input[name=lexicon_replace]:checked").val();
    if (selected == 'template') {
      $("select[name=lexicon_link]").parents("div.lexicon_link").hide(1000);
      $("select[name=lexicon_click_option]").parents("div.lexicon_click_option").hide(1000);
      $("input[name=lexicon_disable_indicator]").parents("div.lexicon_disable_indicator").hide(1000);
    }
    else {
      $("select[name=lexicon_link]").parents("div.lexicon_link").show(1000);
      $("select[name=lexicon_click_option]").parents("div.lexicon_click_option").show(1000);
      $("input[name=lexicon_disable_indicator]").parents("div.lexicon_disable_indicator").show(1000);
    }
  }

  // Run the javascript on page load.
  $(document).ready(function () {
    // On page load, determine the current settings.
    lexicon_mark_terms_handler();
    lexicon_replace_handler();
    // Bind the function to click events.
    $("input[name=lexicon_mark_terms]").bind("click", lexicon_mark_terms_handler);
    $("input[name=lexicon_replace]").bind("click", lexicon_replace_handler);
  });
})(jQuery);