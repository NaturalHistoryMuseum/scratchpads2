<?php

/**
 * Primary value passed to this field must be the two letter ISO country code of
 * the address.
 *
 * Arguments are used to specify all the other values:
 *   'administrative_area' - The administrative area of this address. (i.e. State/Province)
 *   'sub_administrative_area' - The sub administrative area of this address.
 *   'locality' - The locality of this address. (i.e. City)
 *   'dependent_locality' - The dependent locality of this address.
 *   'postal_code' - The postal code of this address.
 *   'thoroughfare' - The thoroughfare of this address. (i.e. Street address)
 *   'premise' - The premise of this address. (i.e. Apartment / Suite number)
 *   'sub_premise' - The sub_premise of this address.
 *   'organisation_name' - Contents of a primary OrganisationName element in the xNL XML.
 *   'name_line' - Contents of a primary NameLine element in the xNL XML.
 *   'first_name' - Contents of the FirstName element of a primary PersonName element in the xNL XML.
 *   'last_name' - Contents of the LastName element of a primary PersonName element in the xNL XML.
 *   'data' - Additional data for this address.
 *
 * Add the source field mappings to the argument array then add null mappings to
 * avoid having fields flagged as as unmapped:
 * @code
 *   $arguments = array(
 *     'thoroughfare' => array('source_field' => 'profile_address'),
 *     'locality' => array('source_field' => 'profile_city'),
 *     'administrative_area' => array('source_field' => 'profile_state'),
 *   );
 *   // The country should be passed in as the primary value.
 *   $this->addFieldMapping('field_user_address', 'profile_country')
 *        ->arguments($arguments);
 *   // Since the excerpt is mapped via an argument, add a null mapping so it's
 *   // not flagged as unmapped.
 *   $this->addFieldMapping(NULL, 'profile_address');
 *   $this->addFieldMapping(NULL, 'profile_city');
 *   $this->addFieldMapping(NULL, 'profile_state');
 * @endcode
 */
class MigrateAddressFieldHandler extends MigrateFieldHandler {
  public function __construct() {
    $this->registerTypes(array('addressfield'));
  }

  public function prepare($entity, array $field_info, array $instance, array $values) {
    $arguments = array();
    if (isset($values['arguments'])) {
      $arguments = array_filter($values['arguments']);
      unset($values['arguments']);
    }
    $language = $this->getFieldLanguage($entity, $field_info, $arguments);

    // Setup the standard Field API array for saving.
    $delta = 0;
    foreach ($values as $value) {
      $return[$language][$delta] = array('country' => $value) + array_intersect_key($arguments, $field_info['columns']);
      $delta++;
    }

    return isset($return) ? $return : NULL;
  }
}