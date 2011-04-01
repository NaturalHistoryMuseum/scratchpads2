<?php

/**
 * @file
 * Views fiter handler for simplenews_category.opt_inout.
 */

/**
 * Provide translatable simplenews_category.opt_inout Options.
 */
class simplenews_handler_filter_category_opt_inout extends views_handler_filter_in_operator {
  function get_value_options() {
    $this->value_options = array(
      'hidden' => t('Hidden'),
      'single' => t('Single opt-in'),
      'double' => t('Double opt-in'),
    );
  }
}
