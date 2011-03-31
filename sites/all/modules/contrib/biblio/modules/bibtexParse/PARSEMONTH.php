<?php
/*
Released through http://bibliophile.sourceforge.net under the GPL licence.
Do whatever you like with this -- some credit to the author(s) would be appreciated.

A collection of PHP classes to manipulate bibtex files.

If you make improvements, please consider contacting the administrators at bibliophile.sourceforge.net so 
that your improvements can be added to the release package.

Mark Grimshaw 2005
http://bibliophile.sourceforge.net
*/
/*****
*  PARSEMONTH: BibTeX MONTH import class
*
* BibTeX month field can come in as:
* jan
* "8~" # jan
* jan#"~8"
* etc.
* where # is concatenation and '~' can be any non-numeric character.
*****/

// 17/June/2005 - Mark Grimshaw:  month fields that have multiple dates (e.g. dec # " 5--9," or nov # " 29" # "--" # dec # " 2") are correctly parsed.
class PARSEMONTH
{
// Constructor
  function PARSEMONTH()
  {
  }
  function init($monthField)
  {
    $startMonth = $this->startDay = $endMonth = $this->endDay = FALSE;
    $date = split("#", $monthField);
    foreach ($date as $field)
    {
      $field = ucfirst(strtolower(trim($field)));
      if ($month = array_search($field, $this->monthToLongName()))
      {
        if (!$startMonth)
          $startMonth = $month;
        else
          $endMonth = $month;
        continue;
      }
      else if ($month = array_search($field, $this->monthToShortName()))
      {
        if (!$startMonth)
          $startMonth = $month;
        else
          $endMonth = $month;
        continue;
      }
      $this->parseDay($field);
    }
    if ($this->endDay && !$endMonth)
      $endMonth = $startMonth;
    return array($startMonth, $this->startDay, $endMonth, $this->endDay);
  }
// extract day of month from field
  function parseDay($dayField)
  {
    preg_match("/([0-9]+).*([0-9]+)|([0-9]+)/", $dayField, $array);
    if (array_key_exists(3, $array))
    {
      if (!$this->startDay)
        $this->startDay = $array[3];
      else if (!$this->endDay)
        $this->endDay = $array[3];
    }
    else
    {
      if (array_key_exists(1, $array))
        $this->startDay = $array[1];
      if (array_key_exists(2, $array))
        $this->endDay = $array[2];
    }
  }
// Convert month to long name
  function monthToLongName()
  {
    return array(
        1  =>  'January',
        2  =>  'February',
        3  =>  'March',
        4  =>  'April',
        5  =>  'May',
        6  =>  'June',
        7  =>  'July',
        8  =>  'August',
        9  =>  'September',
        10  =>  'October',
        11  =>  'November',
        12  =>  'December',
      );
  }
// Convert month to short name
  function monthToShortName()
  {
    return array(
        1  =>  'Jan',
        2  =>  'Feb',
        3  =>  'Mar',
        4  =>  'Apr',
        5  =>  'May',
        6  =>  'Jun',
        7  =>  'Jul',
        8  =>  'Aug',
        9  =>  'Sep',
        10  =>  'Oct',
        11  =>  'Nov',
        12  =>  'Dec',
      );
  }
}
?>
