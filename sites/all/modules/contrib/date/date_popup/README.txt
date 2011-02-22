Drupal date_popup.module README.txt
==============================================================================

Javascript popup calendar and timeentry using the 
jquery UI calendar and jquery-timeentry libraries,

==================================================================================
Usage
==================================================================================

To include a popup calendar in a form, use the type 'date_popup':

  $form['date'] = array(
    '#type' => 'date_popup':
    '#title => t('My Date'),
    ....
  );

Set the #type to date_popup and fill the element #default_value with
a date adjusted to the proper local timezone, or leave it blank.

The element will create two textfields, one for the date and one for the
time. The date textfield will include a jQuery popup calendar date picker,
and the time textfield uses a jQuery timepicker.

NOTE - Converting a date stored in the database from UTC to the local zone
and converting it back to UTC before storing it is not handled by this
element and must be done in pre-form and post-form processing!!

==================================================================================
Customization
==================================================================================

To change the default display and functionality of the calendar, set startup
parameters by adding selectors to your element. The configurable options 
are:

#date_type
  The type of date to convert the input value to, DATE_DATETIME, DATE_ISO, or DATE_UNIX

#date_format
  a standard PHP date format string that represents the way the month, day, 
  and year will be displayed in the textfield, like m/d/Y. Months and days 
  must be in the 'm' and 'd' formats that include the zero prefix, the year 
  must be in the 'Y' (four digit) format.

  Any standard separator can be used, '/', '-', '.', or a space.

  The m, d, and Y elements can be in any order and the order will be preserved.

  The time selector will add AM/PM if 'a' is in the format string.

  The default format uses the short site default format.

#date_year_range
  the number of years to go backwards and forwards from current year 
  in year selector, in the format -{years back}:+{years forward},
  like -3:+3

#date_increment
   increment minutes and seconds by this amount, default is 1

==================================================================================
Example:
==================================================================================

$form['date'] = array(
  '#type' => 'date_popup',
  '#default_value' => '2007-01-01 10:30:00,
  '#date_type' => DATE_DATETIME,
  '#date_timezone' => date_default_timezone_name(),
  '#date_format' => 'm/d/Y - H:i',
  '#date_increment' => 1,
  '#date_year_range' => '-3:+3',
);

==================================================================================
Localization
==================================================================================

The module will use the t() function for abbreviated month names, abbreviated 
day names, and the words 'Today', 'Clear', 'Close', 'Prev', and 'Next'. 

The module will use the site default for the first day of the week.