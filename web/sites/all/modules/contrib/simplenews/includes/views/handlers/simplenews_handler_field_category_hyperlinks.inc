<?php

/**
 * @file
 * Views handler for simplenews field simplewnews_category.hyperlinks.
 */

/**
 * Provide HTML Mail Hyperlinks position settings.
 */
class simplenews_handler_field_category_hyperlinks extends views_handler_field {
  function render($values) {
    switch ($values->{$this->field_alias}) {
      case 0:
        return t('Bottom');
      case 1:
        return t('Inline');
    }
  }
}
