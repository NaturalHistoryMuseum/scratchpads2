<?php
/**
 * A handler to display dates as just the time if today, otherwise as time and date.
 *
 * @ingroup views_field_handlers
 */
class views_handler_get_list_from_file_in_operator extends views_handler_filter_in_operator {

  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    if (isset($this->definition['options callback']) && is_callable($this->definition['options callback'])) {
      return $this->value_options =array('option_1' => t('Option 1'), 'option_2' => t('Option 2'));
    }
    else {
      $this->value_options = array(t('Yes'), t('No'));
    }

    return $this->value_options;
  }
}
	//return $this->value_options =array('option_1' => t('Option 1'), 'option_2' => t('Option 2'));
   //return $this->value_options= _get_list_from_file_get_codes($field['settings']['file']);
  
