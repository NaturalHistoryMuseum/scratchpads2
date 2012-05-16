Countries module - http://drupal.org/project/countries
======================================================

DESCRIPTION
------------
This module provides country related tasks. It replaces the Countries API and
CCK Country modules from Drupal 6.

The region data parts can be obtained using one of

Location Taxonomize: http://drupal.org/project/location_taxonomize
Countries regions (Sandbox project): http://drupal.org/sandbox/aland/1311114

Features include:
 * A countries database with an administrative interface.
 * To alter Drupals core country list.
 * A countries field.
 * Ability to add any additional Fields to a country.
 * Integration with Views, Token, Apache solr search and Feeds modules.
 * Numerious methods to handle and filter the data.
 * A country FAPI element.

Countries 7.x-2.x only
 * Entity API integration.
 * A countries field with continent filter.
 * New continent and continent code formatters
 * Integration with CountryIcons v2 with more features for less loc.

New hooks for listening to country changes.
* hook_country_insert()
* hook_country_update()
* hook_country_delete()

REQUIREMENTS
------------
Drupal 7.x

For Countries 7.x-2.x and above

 * Entity API
   http://drupal.org/project/entity

INSTALLATION
------------
1.  Place both the Entity API and Countries modules into your modules directory.
    This is normally the "sites/all/modules" directory.

2.  Go to admin/build/modules. Enable both modules.
    The Countries modules is found in the Fields section.

Read more about installing modules at http://drupal.org/node/70151

3.  Updating the core list
    The module does not override the standard name that is defined by the core
    Drupal country list during installation. If you want to bulk update all
    standard names to those of ISO 3166-1, visit the bulk update page and select
    the countries to update. Selecting all updates should bring the database in
    sync with the ISO standard.
    
    The bulk update page is found here:
    http://www.example.com/admin/config/regional/countries/import

UPGRADING
---------
Any updates should be automatic. Just remember to run update.php!

To reset your countries database with the ISO defined countries list, visit
http://www.example.com/admin/config/regional/countries/import to manually
select which country properties to update.

FEATURES
--------

1 - Countries database

This is a simple table based on the ISO 3166-1 alpha-2 codes [1]. It covers the
countries standard name, official name, ISO 3166-1 alpha-3 code, UN numeric code
(ISO 3166-1 numeric-3) and continent (Africa, Antarctica, Asia, Europe, North
America, Oceania, South America). An enabled flag defines a countries status.
 
For example, Taiwan has the following values:

 * Name           - Taiwan
 * Offical name   - Taiwan, Republic of China
 * ISO alpha-2    - TW
 * ISO alpha-3    - TWN
 * ISO numeric-3  - 158
 * Continent      - Asia
 * Enabled        - Yes

The official names were originally taken from WikiPedia [2] and the majority of
the continent information was imported from Country codes API project [3]. This
have been since standardised with the ISO 3166-1 standard. 

Country updates are added when the ISO officially releases these. This process
may be up to 2 - 6 months. South Sudans inclusion took around a month. Kosovo
is taking many months, but this should be added in the near future as Kosovo is
a member both the IMF and World Bank.

Please report any omissions / errors.

2 - Alter Drupals core country list

The module implement hook_countries_alter() which updates any list generated
using country_get_list() to filter out any disabled countries and adds the
potential to rename these based on your personal or political preferences.

## Developers note: ##

There is no need to make this module a dependency unless you use the API or
Field element. See the countries_example module for examples.

3 - A country FAPI element

After programming yet another select list with a country drop down, I
encapsulated the logic into a simple FAPI element. By default it uses
country_get_list(), so filters based on the countries status.

Custom filters are available to bypass the default country_get_list(), to filter
based on status and continent.

--------------------------------------------------------------------------------
<?php
  $element = array(
    '#type' => 'country',
    '#default_value' => 'AU',
    '#multiple' => TRUE, // multiple select
    '#cardinality' => 4, // max. selection allowed is 4 values
    '#filters' => array(
      // enabled options should be one of these constants:
      // COUNTRIES_ALL, COUNTRIES_ENABLED, or COUNTRIES_DISABLED
      'enabled' => COUNTRIES_ENABLED,
      // The restrict by continent filter accepts an array of continent codes.
      // The default continents that are defined are [code - name]:
      // AF - Africa, AN - Antarctica, AS - Asia, EU - Europe,
      // NA - North America, OC - Oceania, SA - South America, UN - Unknown
      'continents' => array('EU', 'OC'),
    ),
  );
?>
--------------------------------------------------------------------------------

For Countries 7.x-2.x and latter, we recommend using a select element instead.

However, there are no plans to drop this, especially now with the new continents
country widget that uses it (it is easier and cleaner).

--------------------------------------------------------------------------------
<?php
  $element = array(
    '#type' => 'select',
    '#title' => t('Country'),
    '#default_value' => 'AU',
    '#options' => countries_get_countries('name', array('enabled' => COUNTRIES_ENABLED)),
  );

  $filters = array(
    // enabled options should be one of these constants:
    // COUNTRIES_ALL, COUNTRIES_ENABLED, or COUNTRIES_DISABLED
    'enabled' => COUNTRIES_ENABLED,
    // The restrict by continent filter accepts an array of continent codes.
    // The default continents that are defined are [code - name]:
    // AF - Africa, AN - Antarctica, AS - Asia, EU - Europe,
    // NA - North America, OC - Oceania, SA - South America, UN - Unknown
    'continents' => array('EU', 'OC'),
    // If you want a very granular control of the available countries.
    'countries' => array('AU', 'CA', 'CN', 'MX', 'NZ', 'US'),
  );
  $element = array(
    '#type' => 'select',
    '#title' => t('Country'),
    '#default_value' => 'AU',
    '#options' => countries_get_countries('name', $filters),
    '#multiple' => TRUE, // multiple select
    '#size' => 6,
  );
?>
--------------------------------------------------------------------------------

4 - A country field

Provides a standard field called "Country", with a widget "Country select list".
This expands the core Drupal Options list provide the functionality of either
a select list, radios or checkboxes.

The default display options are:

Default (The country name)
Official name
ISO alpha-2 code
ISO alpha-3 code
ISO numeric-3 code
Continent
Continent code

HOWTO / FAQ
-----------

1 - Revert the database to the original values.

To reset your countries database with the ISO defined countries list, visit
http://www.example.com/admin/config/regional/countries/import to manually
select which countries to update. Replace www.example.com with your sites URL.

2 - Change the continent list.

These are generated using a variable_get() like this:

--------------------------------------------------------------------------------
<?php
  $continents = variable_get('countries_continents',
      countries_get_default_continents());
?>
--------------------------------------------------------------------------------

To update these, you need to set the system variable 'countries_continents'. The
easiest way to do this is to cut and paste the following into your themes
template.php, changing the array values to suit your requirements. Load one page
on your site that uses the theme, then delete the code.

--------------------------------------------------------------------------------
<?php
  variable_set('countries_continents', array(
    'AF' => t('Africa'),
    'EA' => t('Asia & Europe'),
    'AM' => t('America'),
    'OC' => t('Oceania'),
    'AN' => t('Antarctica'),
    'UN' => t('Unknown'),
  ));
?>
--------------------------------------------------------------------------------

Any invalid continent keys that are found are converted to t('Unknown'), so
update all respective countries before deleting any existing values.

For I18n sites, to ensure that the new continents are translated correctly, use
codes from the following list.

* Default
  'AF' => t('Africa'),
  'AS' => t('Asia'),
  'EU' => t('Europe'),
  'NA' => t('North America'),
  'SA' => t('South America'),
  'OC' => t('Oceania'),
  'AN' => t('Antarctica'),
  'UN' => t('Unknown', array(), array('context' => 'countries')),

* Additionally defined  
  'AE' => t('Afro-Eurasia'),
  'AM' => t('Americas'),
  'AU' => t('Australasia'),
  'CA' => t('Caribbean'),
  'CE' => t('Continental Europe'),
  'ER' => t('Eurasia'),
  'IC' => t('Indian subcontinent'),

If you need another continent listed, please lodge an issue and we will consider
it for inclusion.

3 - Hiding columns in the administrative country overview page.

Like the continents, these are dynamically generated from the system variables.
They can also be changed in a similar variable_set, like 'countries_continents'.

The name, ISO alpha-2 and enabled columns can not be removed.

--------------------------------------------------------------------------------
<?php
  // Remove the columns that you want to hide.
  variable_set('countries_admin_overview_columns', array(
    'iso3' => t('ISO3'),
    'numcode' => t('Number code'),
    'continent' => t('Continent'),
    'official_name' => t('Official name'),
  ));
?>
--------------------------------------------------------------------------------

4 - I18n support (Countries 7.x-2.x only)

This is in the early implemenation stages using the Entity API integration.

5 - Why is the delete link hidden on some countries?
  - Why is the edit ISO alpha-2 code disabled on some countries?

These are the countries that Drupal defines. To disable a country in the list of
countries that Drupal generates, these must be present in the database. Also
done to ensure that existing references to these countries still exist, even if
you can no longer select them when they are disabled.

6 - How does this differ from countries_api?

The countries_api is a just that, an API locked into a back-end country and
regions database that has no configurable options. It main purpose is converting
country code data from one format to another.

From the Country codes API modules project page:

    "Typical usage would be converting a country name to its ISO2
    (or ISO3) country code."

The Countries module is based on the philosophy that only the ISO2 code can be
trusted. All other data can be modified by the sites administrator, and the ISO2
is the primary key. Then the most common country requirements are built on top
of this base, providing the input elements, etc.

Function                           Drupal 6                Drupal 7
Provide a list of countries        Countries API           Drupal
Update the countries list          N/A                     Countries
Provide a field element            CCK Country             Countries
Country getter API                 Countries API           Countries


Here is an approximate mapping of the Country API functions in the Countries
module. Note that the Country API module generally returns an array, while the
Countries module returns an object.

Countries API module
--------------------------------------------------------------------------------
<?php

# 1 - All countries (an array of country arrays)
$countries = countries_api_get_list();

# 2 - Load a country (an array)
$country = countries_api_get_country($iso2_or_iso3);
$country = countries_api_iso2_get_country($iso2);
$country = countries_api_iso3_get_country($iso3);
$country = _countries_api_iso_get_country($property, $value);

# 3 - Get a countries name
$name = countries_api_get_name($iso2_or_iso3);
$name = countries_api_iso2_get_name($iso2);
$name = countries_api_iso3_get_name($iso3);

# 4 - Toggle between ISO character codes
$iso3 = countries_api_iso2_get_iso3($iso2);
$iso2 = countries_api_iso3_get_iso2($code);

# 5 - Option lists
$list = countries_api_get_array($list_key_property, $list_option_property);
$standard_list = countries_api_get_options_array();
?>
--------------------------------------------------------------------------------

Countries module
--------------------------------------------------------------------------------
<?php
# 1 - All countries (an array of country objects)
$countries = countries_get_countries();

# 2 - Load a country (an object)
$country = country_load($iso2);
$country = countries_country_lookup($value);
$country = countries_country_lookup($value, $property);

# 3 - Get a countries name

// The recommended method for an existing country using ISO 2 code
$name = country_load($iso2)->name;

// If the ISO 2 code can not be trusted:
$name = ($country = country_load($iso2) ? $country->name : '');

// Any property (iso2, iso3, num code or name) supplied by an end user
$name = $country = countries_country_lookup($value) ? $country->name : '';

# 4 - Toggle between ISO character codes
$iso3 = country_load($iso2)->iso3;
$iso2 = $country = countries_country_lookup($iso3, 'iso3') ? $country->iso2 : '';

# 5 - Option lists
// If keyed by iso2 value.
$list = countries_get_countries($list_option_property);

// Other lists that are keyed differently would need to be generated manually.
$list = array();
foreach (countries_get_countries() as $country) {
  $list[$country->numcode] = $country->name;
}

$standard_list = array('' => t('Please Choose')) + countries_get_countries('name');

# Please note that the following are equivalent.

  // Core Drupal iso2/name listing.
  // Returns a list of countries passed through hook_countries_alter().
  include_once DRUPAL_ROOT . '/includes/locale.inc';
  $list = country_get_list();

  // Countries module.
  // Get a list of enabled countries and then allow other modules to update this
  // list via hook_countries_alter(). This avoids loading 'include/locale.inc'.
  $list = countries_get_countries('name', array('enabled' => COUNTRIES_ENABLED));
  countries_invoke_additional_countries_alter($list);

?>
--------------------------------------------------------------------------------

7 - Related modules (as of early 2010) see http://drupal.org/node/1412962


CHANGE LOG
----------

Countries 7.x-1.x to 7.x-2.x
1) Entity API integration

   This is now an dependency.  

2) countries_get_country() is been depreciated.

   Use country_load() instead.

3) countries_get_countries() will throw an Exception if you attempt to
   use it to lookup an invalid property.

4) CRUD functions have been completely refactored.

AUTHORS
-------
Alan D. - http://drupal.org/user/198838.
Florian Weber (webflo) - http://drupal.org/user/254778.

Thanks to everybody else who have helped test and contribute patches!

REFERENCES
----------
[1] http://www.iso.org/iso/country_codes/iso_3166_code_lists.htm
[2] http://en.wikipedia.org/wiki/List_of_countries
[3] http://drupal.org/project/countries_api
