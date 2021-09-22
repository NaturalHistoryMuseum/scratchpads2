<?php

/**
 * @file Styles.inc
 * Base class for Styles.
 */

class StylesDefault {
  // The variables passed from our theme.
  public $variables = array();

  // The object passed from our theme.
  public $object = array();

  // Any effects to apply when displaying content matching this style.
  public $effects = array();

  // The final rendered output for this item.
  public $output;
  public $prefix;
  public $suffix;

  public $wrapperType = 'div';
  public $classes = array('styles');

  public $id;
  private $_id = 0;

  public function __construct($object = NULL, $effects = NULL, $variables = NULL) {
    // @TODO: This is not great IMO, the ->object and ->variables props already have everything
    // we shouldn't be duplicating it in different methods / properties.
    if (isset($variables)) {
      $properties = (array) $variables;
      $this->magicSet($properties);
      $this->setVariables($variables);
    }
    // If we are passed an array, then set the object properties from its keys.
    if (isset($object)) {
      $properties = (array) $object;
      $this->magicSet($properties);
      $this->setObject($object);
    }
    if (isset($effects)) {
      $this->setEffects($effects);
    }
  }

  /**
   * Converts a string in the form with_underscores to withUnderscores
   * @param string $str
   * @return string
   */
  protected function _toCamelCase($str) {
    $parts = explode("_", $str);
    $out = array_shift($parts);
    foreach ($parts as $part) {
      $out .= ucfirst($part);
    }
    return $out;
  }

  /**
   * Given an array of k/v pairs calls set$key
   * @param array $properties
   */
  protected function magicSet($properties) {
    $methods = get_class_methods($this);
    foreach ($properties as $key => $value) {
      $propName = "set_{$key}";
      $function = $this->_toCamelCase($propName);
      if (in_array($function, $methods)) {
        $this->$function($value);
      }
    }
  }

  /**
   * Display the rendered output.
   *
   * @param boolean $reset
   *  Optional; if TRUE, the rebuild the output.
   * @return
   *  A fully themed snippet of HTML for output.
   */
  public function display($reset = FALSE) {
    return $this->render($reset);
  }

  /**
   * Build the output for display.
   *
   * @param boolean $reset
   *  Optional; if TRUE, the rebuild the output.
   * @return
   *  A fully themed snippet of HTML for output.
   */
  public function render($reset = FALSE) {
    // If we need to reset, then set the output to NULL.
    if ($reset) {
      $this->setOutput(NULL);
    }

    // Have we already rendered the output?
    $output = $this->getOutput();

    // We need to render the output the first time around, or if reset.
    if (!isset($output)) {
      // First, we get the array of callable class methods for this object.
      // These may each be called by effects called for by the style.
      // @TODO: Should we have a proper array of allowable effects, rather
      // than our current lazy method of allowing all class functions?
      $methods = get_class_methods($this);

      // Loop through and apply each effect.
      foreach ($this->getEffects() as $effect) {
        // What is the effect?
        $effectName = $effect['name'];
        if ($effectName && in_array($effectName, $methods)) {
          // Apply the effect with its settings.
          $this->$effectName($effect['settings']);
        }
        else {
          // Ouch. We have an invalid effect. Flag for bug reporting.
          $variables = $this->getVariables();
          $styleName = $variables['style']['name'];
          watchdog('styles', 'Effect %effect_name not found for %style_name display formatter style of the %class_name class.', array('%effect_name' => $effectName, '%style_name' => $styleName, '%class_name' => $this->getClassName()), WATCHDOG_WARNING);
        }
      }

      // Now the output will have been fully built.
      $output = $this->getOutput();
    }

    return $output;
  }

  public function getClassName() {
    return get_called_class();
  }

  public function _get($variable) {
    if (function_exists('get_' . $variable)) {
      return $this->{'get_' . $variable}();
    }
    else {
      return $this->get($variable);
    }
  }
  public function _set($variable, $value) {
    if (function_exists('set_' . $variable)) {
      return $this->{'set_' . $variable}($value);
    }
    else {
      return $this->set($variable, $value);
    }
  }

  public function arrayPush($variable, $element) {
    $array = (array) $this->_get($variable);
    array_push($array, $element);
    return $this->_set($variable, $array);
  }
  public function arrayPop($variable) {
    $array = (array) $this->_get($variable);
    array_pop($array);
    return $this->_set($variable, $array);
  }
  public function arrayUnshift($variable, $element) {
    $array = (array) $this->_get($variable);
    array_unshift($array, $element);
    return $this->_set($variable, $array);
  }
  public function arrayShift($variable) {
    $array = (array) $this->_get($variable);
    array_shift($array);
    return $this->_set($variable, $array);
  }

  /**
   * Add an effect to the end of the array.
   *
   * An effect is an array with at least a 'name' key, which corresponds to the
   * class function to be called during the rendering process. It will usually
   * also contain an array of 'effects' to apply.
   */
  public function addEffect($effect) {
    return $this->pushEffect($effect);
  }
  public function pushEffect($effect) {
    $effectName = $effect['name'];
    if (method_exists($this, $effectName)) {
      $effects = $this->getEffects();
      array_push($effects, $effect);
      return $this->setEffects($effects);
    }
    else {
      $variables = $this->getVariables();
      $styleName = $variables['style']['label'];
      watchdog('styles', 'Effect %effect_name not found for %style_name display formatter style of the %class_name class.', array('%effect_name' => $effectName, '%style_name' => $styleName, '%class_name' => $this->getClassName()), WATCHDOG_WARNING);
    }
  }
  public function popEffect() {
    $effects = $this->getEffects();
    $effect = array_pop($effects);
    $this->setEffects($effects);
    return $effect;
  }
  public function unshiftEffect($effect) {
    $effectName = $effect['name'];
    if (method_exists($this, $effectName)) {
      $effects = $this->getEffects();
      array_unshift($effects, $effect);
      return $this->setEffects($effects);
    }
    else {
      $variables = $this->getVariables();
      $styleName = $variables['style']['label'];
      watchdog('styles', 'Effect %effect_name not found for %style_name display formatter style of the %class_name class.', array('%effect_name' => $effectName, '%style_name' => $this->getName(), '%class_name' => $this->getClassName()), WATCHDOG_WARNING);
    }
  }
  public function shiftEffect() {
    $effects = $this->getEffects();
    $effect = array_shift($effects);
    $this->setEffects($effects);
    return $effect;
  }

  public function getObject() {
    return $this->get('object');
  }
  public function setObject($value) {
    return $this->set('object', $value);
  }

  public function getVariables() {
    return $this->get('variables');
  }
  public function setVariables($value) {
    return $this->set('variables', $value);
  }

  public function getEffects() {
    return $this->get('effects');
  }
  public function setEffects($value) {
    return $this->set('effects', $value);
  }

  public function getWrapperType() {
    return $this->get('wrapperType');
  }
  public function setWrapperType($value) {
    return $this->set('wrapperType', $value);
  }
  public function getClasses() {
    $classes = $this->get('classes');
    return is_array($classes) ? implode(' ', $classes) : $classes;
  }
  public function setClasses($value) {
    return $this->set('classes', $value);
  }
  public function getId() {
    return $this->get('id');
  }
  public function setId($value) {
    return $this->set('id', $value);
  }
  public function getOutput() {
    return $this->get('output');
  }
  public function setOutput($value) {
    return $this->set('output', $value);
  }
  public function getPrefix() {
    $prefix = $this->get('prefix');
    if (!isset($prefix)) {
      $wrapperType = $this->getWrapperType();
      $id = $this->getId();
      $_id = $this->_id++;
      $classes = $this->getClasses();
      $prefix = "<$wrapperType id=\"styles-$id-$_id\" class=\"$classes\">";
    }

    return $prefix;
  }
  public function setPrefix($value) {
    return $this->set('prefix', $value);
  }
  public function getSuffix() {
    $suffix = $this->get('suffix');
    if (!isset($suffix)) {
      $wrapperType = $this->getWrapperType();
      $suffix = "</$wrapperType>";
    }

    return $suffix;
  }
  public function setSuffix($value) {
    return $this->set('suffix', $value);
  }

  public function get($variable) {
    return isset($this->{$variable}) ? $this->{$variable} : NULL;
  }
  public function set($variable, $value) {
    return $this->{$variable} = $value;
  }
}