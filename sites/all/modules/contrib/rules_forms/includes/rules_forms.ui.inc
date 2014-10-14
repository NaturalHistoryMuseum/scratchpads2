<?php

/**
 * @file
 * Defines data type classes for forms and form elements.
 */

abstract class RulesFormsDataUI extends RulesDataUI implements RulesDataDirectInputFormInterface {

  /**
   * Provides the default mode.
   */
  public static function getDefaultMode() {
    return 'input';
  }
  
  /**
   * Provides the default form.
   */
  public static function defaultForm($name, $info, $settings, $title, $type = 'select') {
    $form[$name] = array(
        '#type' => $type,
        '#title' => $title,
        '#default_value' => isset($settings[$name]) ? $settings[$name] : NULL,
        '#required' => empty($info['optional']),
    );
    return $form;
  }

  /**
   * Renders a parameter value.
   */
  public static function render($value) {
    return array(
      'content' => array('#markup' => check_plain($value)),
      '#attributes' => array('class' => array('rules-forms-parameter')),
    );
  }

}

/**
 * Defines the form element data type UI.
 */
class RulesFormsDataElement extends RulesFormsDataUI {

  /**
   * An array of attributes and the form element types to which they apply.
   * This array is used to filter the form element options available during
   * specific attribute setting actions.
   */
  private static $_attributes = array(
    'access' => 'all',
    'changed' => array('textfield', 'textarea', 'checkbox', 'checkboxes', 'radio', 'radios', 'select'),
    'clicked' => array('button', 'submit'), // For Condition: button was clicked.
    'disabled' => array('textfield', 'textarea', 'checkbox', 'checkboxes', 'radio', 'radios', 'select', 'button', 'submit'),
    'default' => array('textfield', 'textarea', 'checkbox', 'checkboxes', 'radio', 'radios', 'select'),
    'description' => array('textfield', 'textarea', 'checkbox', 'checkboxes', 'radio', 'radios', 'select', 'item'),
    'error' => 'all', // Allows an error to be set on any form element.
    'options' => array('select', 'checkboxes'),
    'required' => array('textfield', 'textarea', 'checkbox', 'checkboxes', 'radio', 'radios', 'select'),
    'suffix' => 'all', // Note: This applies to prefix/suffix action.
    'title' => array('textfield', 'textarea', 'checkbox', 'checkboxes', 'radio', 'radios', 'select', 'item'),
    'weight' => 'all',
    'value' => array('textfield', 'textarea', 'checkbox', 'checkboxes', 'radio', 'radios', 'select'),
  );

  /**
   * Returns a select list with options including only available elements.
   */
  public static function inputForm($name, $info, $settings, RulesPlugin $element) {
    $options = array();

    // Get the type of attribute being set from the end of the element name.
    // The action name should directly correspond with the attribute it sets for this reason.
    $element_name = $element->getElementName();
    $attribute = array_pop(explode('_', $element_name));

    // Loop through each variable and retrieve the options that apply to this field type.
    foreach ($element->availableVariables() as $key => $value) {

      // Populate the options list with only form elements.
      if (!in_array($key, array('site', 'form', 'form_state', 'form_id', 'user'))) {
        $type = substr($key, 0, strpos($key, ':'));
        // Filter the elements by determining whether this attribute can be applied to them.
        if (isset(self::$_attributes[$attribute])
            && (self::$_attributes[$attribute] == 'all' || in_array($type, self::$_attributes[$attribute]))) {
          if (!isset($options[$type])) {
            $options[$type] = array();
          }
          $options[$type][$key] = $value['label'];
        }
      }
    }
    ksort($options);
    $form = parent::defaultForm($name, $info, $settings, 'Form element');
    $form[$name]['#options'] = $options;
    return $form;
  }

}

/**
 * Provides a form for the value in Condition: Element has value.
 */
class RulesFormsDataValue extends RulesFormsDataUI {

  /**
   * Sets the input type based on the selected form element type.
   */
  public static function inputForm($name, $info, $settings, RulesPlugin $element) {
    $form = parent::defaultForm($name, $info, $settings, 'Value');
    if (!empty($element->settings['element'])) {
      $type = substr($element->settings['element'], 0, strpos($element->settings['element'], ':'));
      if ($type == 'checkboxes' || $type == 'radios' || $type == 'select') {
        $form[$name]['#type'] = 'textarea';
        $form[$name]['#title'] = 'Value(s)';
      }
      else {
        $form[$name]['#type'] = $type;
      }
    }
    return $form;
  }

}

