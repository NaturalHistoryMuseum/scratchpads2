<?php
// $Id: date_repeat_calc.inc,v 1.24 2011/01/18 00:37:34 karens Exp $
/**
 * @file
 * Code to compute the dates that match an iCal RRULE.
 *
 * Moved to a separate file since it is not used on most pages
 * so the code is not parsed unless needed.
 *
 * Extensive simpletests have been created to test the RRULE calculation
 * results against official examples from RFC 2445.
 *
 * These calculations are expensive and results should be stored or cached
 * so the calculation code is not called more often than necessary.
 *
 * Currently implemented:
 * INTERVAL, UNTIL, COUNT, EXDATE, RDATE, BYDAY, BYMONTHDAY, BYMONTH,
 * YEARLY, MONTHLY, WEEKLY, DAILY
 *
 * Currently not implemented:
 *
 * BYYEARDAY, MINUTELY, HOURLY, SECONDLY, BYMINUTE, BYHOUR, BYSECOND
 *   These could be implemented in the future.
 *
 * BYSETPOS
 *   Seldom used anywhere, so no reason to complicated the code.
 */

/**
 * Private implementation of date_repeat_calc().
 *
 * Compute dates that match the requested rule, within a specified date range.
 */
function _date_repeat_calc($rrule, $start, $end, $exceptions, $timezone, $additions) {
  module_load_include('inc', 'date_api', 'date_api_ical');

  if (empty($timezone)) {
    $timezone = date_default_timezone();
  }

  // Make sure the 'EXCEPTIONS' string isn't appended to the rule.
  $parts = explode("\n", $rrule);
  if (count($parts)) {
    $rrule = $parts[0];
  }
  // Get the parsed array of rule values.
  $rrule = date_ical_parse_rrule('RRULE:', $rrule);

  // Create a date object for the start and end dates.
  $start_date = new DateObject($start, $timezone);
  $end_date = new DateObject($end, $timezone);

  // If the rule has an UNTIL, see if that is earlier than the end date.
  if (!empty($rrule['UNTIL'])) {
    $until_date = date_ical_date($rrule['UNTIL'], $timezone);
    if (date_format($until_date, 'U') < date_format($end_date, 'U')) {
      $end_date = $until_date;
    }
  }

  // Get an integer value for the interval, if none given, '1' is implied.
  $interval = max(1, isset($rrule['INTERVAL']) ? $rrule['INTERVAL'] : 1);
  $count = isset($rrule['COUNT']) ? $rrule['COUNT'] : NULL;

  if (empty($rrule['FREQ'])) {
    $rrule['FREQ'] = 'DAILY';
  }

  // Make sure DAILY frequency isn't used in places it won't work;
  if (!empty($rrule['BYMONTHDAY']) && !in_array($rrule['FREQ'], array('MONTHLY', 'YEARLY'))) {
    $rrule['FREQ'] = 'MONTHLY';
  }
  else if (!empty($rrule['BYDAY']) && !in_array($rrule['FREQ'], array('MONTHLY', 'WEEKLY', 'YEARLY'))) {
    $rrule['FREQ'] = 'WEEKLY';
   }

  // Find the time period to jump forward between dates.
  switch ($rrule['FREQ']) {
   case 'DAILY':
     $jump = $interval . ' days';
     break;
   case 'WEEKLY':
     $jump = $interval . ' weeks';
     break;
   case 'MONTHLY':
     $jump = $interval . ' months';
     break;
   case 'YEARLY':
     $jump = $interval . ' years';
     break;
  }
  $rrule = date_repeat_adjust_rrule($rrule, $start_date);

  // The start date always goes into the results, whether or not it meets
  // the rules. RFC 2445 includes examples where the start date DOES NOT
  // meet the rules, but the expected results always include the start date.
  $days = array(date_format($start_date, DATE_FORMAT_DATETIME));

  // BYMONTHDAY will look for specific days of the month in one or more months.
  // This process is only valid when frequency is monthly or yearly.

  if (!empty($rrule['BYMONTHDAY'])) {
    $finished = FALSE;
    $current_day = clone($start_date);
    $direction_days = array();
    // Deconstruct the day in case it has a negative modifier.
    foreach ($rrule['BYMONTHDAY'] as $day) {
      preg_match("@(-)?([0-9]{1,2})@", $day, $regs);
      if (!empty($regs[2])) {
        // Convert parameters into full day name, count, and direction.
        $direction_days[$day] = array(
          'direction' => !empty($regs[1]) ? $regs[1] : '+',
          'direction_count' => $regs[2],
          );
      }
    }
    while (!$finished) {
      $period_finished = FALSE;
      while (!$period_finished) {
        foreach ($rrule['BYMONTHDAY'] as $monthday) {
          $day = $direction_days[$monthday];
          $current_day = date_repeat_set_month_day($current_day, NULL, $day['direction_count'], $day['direction'], $timezone);
          date_repeat_add_dates($days, $current_day, $start_date, $end_date, $exceptions, $rrule);
          if ($finished = date_repeat_is_finished($current_day, $days, $count, $end_date)) {
            $period_finished = TRUE;
          }
        }
        // If it's monthly, keep looping through months, one INTERVAL at a time.
        if ($rrule['FREQ'] == 'MONTHLY') {
          if ($finished = date_repeat_is_finished($current_day, $days, $count, $end_date)) {
            $period_finished = TRUE;
          }
          // Back up to first of month and jump.
          $current_day = date_repeat_set_month_day($current_day, NULL, 1, '+', $timezone);
          date_modify($current_day, '+'. $jump);
        }
        // If it's yearly, break out of the loop at the
        // end of every year, and jump one INTERVAL in years.
        else {
          if (date_format($current_day, 'n') == 12) {
            $period_finished = TRUE;
          }
          else {
            // Back up to first of month and jump.
            $current_day = date_repeat_set_month_day($current_day, NULL, 1, '+', $timezone);
            date_modify($current_day, '+1 month');
          }
        }
      }
      if ($rrule['FREQ'] == 'YEARLY') {
        // Back up to first of year and jump.
        $current_day = date_repeat_set_year_day($current_day, NULL, 1, '+', $timezone);
        date_modify($current_day, '+'. $jump);
      }
      $finished = date_repeat_is_finished($current_day, $days, $count, $end_date);
    }
  }

  // This is the simple fallback case, not looking for any BYDAY,
  // just repeating the start date. Because of imputed BYDAY above, this
  // will only test TRUE for a DAILY or less frequency (like HOURLY).

  elseif (empty($rrule['BYDAY'])) {
    // $current_day will keep track of where we are in the calculation.
    $current_day = clone($start_date);
    $finished = FALSE;
    $months = !empty($rrule['BYMONTH']) ? $rrule['BYMONTH'] : array();
    while (!$finished) {
      date_repeat_add_dates($days, $current_day, $start_date, $end_date, $exceptions, $rrule);
      $finished = date_repeat_is_finished($current_day, $days, $count, $end_date);
      date_modify($current_day, '+'. $jump);
    }
  }

  else {

    // More complex searches for day names and criteria like '-1SU' or '2TU,2TH',
    // require that we interate through the whole time period checking each BYDAY.

    // Create helper array to pull day names out of iCal day strings.
    $day_names = date_repeat_dow_day_options(FALSE);
    $days_of_week = array_keys($day_names);

    // Parse out information about the BYDAYs and separate them
    // depending on whether they have directional parameters like -1SU or 2TH.
    $month_days = array();
    $week_days = array();

    // Find the right first day of the week to use, iCal rules say Monday
    // should be used if none is specified.
    $week_start_rule = !empty($rrule['WKST']) ? trim($rrule['WKST']) : 'MO';
    $week_start_day = $day_names[$week_start_rule];

    // Make sure the week days array is sorted into week order,
    // we use the $ordered_keys to get the right values into the key
    // and force the array to that order. Needed later when we
    // iterate through each week looking for days so we don't
    // jump to the next week when we hit a day out of order.
    $ordered = date_repeat_days_ordered($week_start_rule);
    $ordered_keys = array_flip($ordered);

    foreach ($rrule['BYDAY'] as $day) {
      preg_match("@(-)?([0-9]+)?([SU|MO|TU|WE|TH|FR|SA]{2})@", trim($day), $regs);
      if (!empty($regs[2])) {
        // Convert parameters into full day name, count, and direction.
        $direction_days[] = array(
          'day' => $day_names[$regs[3]],
          'direction' => !empty($regs[1]) ? $regs[1] : '+',
          'direction_count' => $regs[2],
          );
      }
      else {
        $week_days[$ordered_keys[$regs[3]]] = $day_names[$regs[3]];
      }
    }
    ksort($week_days);

    // BYDAYs with parameters like -1SU (last Sun) or 2TH (second Thur)
    // need to be processed one month or year at a time.
    if (!empty($direction_days) && in_array($rrule['FREQ'], array('MONTHLY', 'YEARLY'))) {
      $finished = FALSE;
      $current_day = clone($start_date);
      while (!$finished) {
        foreach ($direction_days as $day) {
          // Find the BYDAY date in the current month.
          if ($rrule['FREQ'] == 'MONTHLY') {
            $current_day = date_repeat_set_month_day($current_day, $day['day'], $day['direction_count'], $day['direction'], $timezone);
          }
          else {
            $current_day = date_repeat_set_year_day($current_day, $day['day'], $day['direction_count'], $day['direction'], $timezone);
          }
          date_repeat_add_dates($days, $current_day, $start_date, $end_date, $exceptions, $rrule);
        }
        $finished = date_repeat_is_finished($current_day, $days, $count, $end_date);     
        // Reset to beginning of period before jumping to next period.
        // Needed especially when working with values like 'last Saturday'
        // to be sure we don't skip months like February.
        $year = date_format($current_day, 'Y');
        $month = date_format($current_day, 'n');
        if ($rrule['FREQ'] == 'MONTHLY') {
          date_date_set($current_day, $year, $month, 1);
        }
        else {
          date_date_set($current_day, $year, 1, 1);
        }
        // Jump to the next period.
        date_modify($current_day, '+' . $jump);
      }
    }

    // For BYDAYs without parameters,like TU,TH (every Tues and Thur),
    // we look for every one of those days during the frequency period.
    // Iterate through periods of a WEEK, MONTH, or YEAR, checking for
    // the days of the week that match our criteria for each week in the
    // period, then jumping ahead to the next week, month, or year,
    // an INTERVAL at a time.

    if (!empty($week_days) && in_array($rrule['FREQ'], array('MONTHLY', 'WEEKLY', 'YEARLY'))) {
      $finished = FALSE;
      $current_day = clone($start_date);
      $format = $rrule['FREQ'] == 'YEARLY' ? 'Y' : 'n';
      $current_period = date_format($current_day, $format);

      // Back up to the beginning of the week in case we are somewhere in the
      // middle of the possible week days, needed so we don't prematurely
      // jump to the next week. The date_repeat_add_dates() function will
      // keep dates outside the range from getting added.
      if (date_format($current_day, 'l') != $day_names[$day]) {  
        date_modify($current_day, '-1 ' . $week_start_day);
      }
      while (!$finished) {
        $period_finished = FALSE;
        while (!$period_finished) {
          $moved = FALSE;
          foreach ($week_days as $delta => $day) {
            // Find the next occurence of each day in this week, only add it
            // if we are still in the current month or year. The date_repeat_add_dates
            // function is insufficient to test whether to include this date
            // if we are using a rule like 'every other month', so we must
            // explicitly test it here.

            // If we're already on the right day, don't jump or we
            // will prematurely move into the next week.
            if (date_format($current_day, 'l') != $day) {
              date_modify($current_day, '+1 ' . $day);
              $moved = TRUE;
            }
            if ($rrule['FREQ'] == 'WEEKLY' || date_format($current_day, $format) == $current_period) {
              date_repeat_add_dates($days, $current_day, $start_date, $end_date, $exceptions, $rrule);
            }
          }
          $finished = date_repeat_is_finished($current_day, $days, $count, $end_date);

          // Make sure we don't get stuck in endless loop if the current
          // day never got changed above.
          if (!$moved) {
            date_modify($current_day, '+1 day');
          }

          // If this is a WEEKLY frequency, stop after each week,
          // otherwise, stop when we've moved outside the current period.
          // Jump to the end of the week, then test the period.
          if ($finished || $rrule['FREQ'] == 'WEEKLY') {
            $period_finished = TRUE;
          }
          elseif ($rrule['FREQ'] != 'WEEKLY' && date_format($current_day, $format) != $current_period) {
            $period_finished = TRUE;
          }
        }

        if ($finished) {
          continue;
        }

        // We'll be at the end of a week, month, or year when 
        // we get to this point in the code.

        // Go back to the beginning of this period before we jump, to
        // ensure we jump to the first day of the next period.
        switch ($rrule['FREQ']) {
          case 'WEEKLY':
            date_modify($current_day, '+1 ' . $week_start_day);
            date_modify($current_day, '-1 week');
            break;
          case 'MONTHLY':
            date_modify($current_day, '-' . (date_format($current_day, 'j') - 1) . ' days');
            date_modify($current_day, '-1 month');
            break;
          case 'YEARLY':
            date_modify($current_day, '-' . date_format($current_day, 'z') . ' days');
            date_modify($current_day, '-1 year');
            break;
        }
        // Jump ahead to the next period to be evaluated.
        date_modify($current_day, '+' . $jump);
        $current_period = date_format($current_day, $format);
        $finished = date_repeat_is_finished($current_day, $days, $count, $end_date);
      }
    }
  }

  // add additional dates
  foreach($additions as $addition) {
    $days[] = date_format($addition, DATE_FORMAT_DATETIME);
  }

  sort($days);
  return $days;
}

/**
 * See if the RRULE needs some imputed values added to it.
 */
function date_repeat_adjust_rrule($rrule, $start_date) {
  // If this is not a valid value, do nothing;
  if (empty($rrule) || empty($rrule['FREQ'])) {
    return array();
  }

  // RFC 2445 says if no day or monthday is specified when creating repeats for
  // weeks, months, or years, impute the value from the start date.

  if (empty($rrule['BYDAY']) && $rrule['FREQ'] == 'WEEKLY') {
    $rrule['BYDAY'] = array(date_repeat_dow2day(date_format($start_date, 'w')));
  }
  elseif (empty($rrule['BYDAY']) && empty($rrule['BYMONTHDAY']) && $rrule['FREQ'] == 'MONTHLY') {
    $rrule['BYMONTHDAY'] = array(date_format($start_date, 'j'));
  }
  elseif (empty($rrule['BYDAY']) && empty($rrule['BYMONTHDAY']) && empty($rrule['BYYEARDAY']) && $rrule['FREQ'] == 'YEARLY') {
    $rrule['BYMONTHDAY'] = array(date_format($start_date, 'j'));
    if (empty($rrule['BYMONTH'])) {
      $rrule['BYMONTH'] = array(date_format($start_date, 'n'));
    }
  }
  // If we are processing rules for period other than YEARLY or MONTHLY
  // and have BYDAYS like 2SU or -1SA, simplify them to SU or SA since the
  // position rules make no sense in other periods and just add complexity.

  elseif (!empty($rrule['BYDAY']) && !in_array($rrule['FREQ'], array('MONTHLY', 'YEARLY'))) {
    foreach ($rrule['BYDAY'] as $delta => $BYDAY) {
      $rrule['BYDAY'][$delta] = substr($BYDAY, -2);
    }
  }

  return $rrule;
}

/**
 * Helper function to add found date to the $dates array.
 *
 * Check that the date to be added is between the start and end date
 * and that it is not in the $exceptions, nor already in the $days array,
 * and that it meets other criteria in the RRULE.
 */
function date_repeat_add_dates(&$days, $current_day, $start_date, $end_date, $exceptions, $rrule) {
  if (isset($rrule['COUNT']) && sizeof($days) >= $rrule['COUNT']) {
    return FALSE;
  }
  $formatted = date_format($current_day, DATE_FORMAT_DATETIME);
  if ($formatted > date_format($end_date, DATE_FORMAT_DATETIME)) {
    return FALSE;
  }
  if ($formatted < date_format($start_date, DATE_FORMAT_DATETIME)) {
    return FALSE;
  }
  if (in_array(date_format($current_day, 'Y-m-d'), $exceptions)) {
    return FALSE;
  }
  if (!empty($rrule['BYDAY'])) {
    $BYDAYS = $rrule['BYDAY'];
    foreach ($BYDAYS as $delta => $BYDAY) {
      $BYDAYS[$delta] = substr($BYDAY, -2);
    }
    if (!in_array(date_repeat_dow2day(date_format($current_day, 'w')), $BYDAYS)) {
      return FALSE;
    }}
  if (!empty($rrule['BYYEAR']) && !in_array(date_format($current_day, 'Y'), $rrule['BYYEAR'])) {
    return FALSE;
  }
  if (!empty($rrule['BYMONTH']) && !in_array(date_format($current_day, 'n'), $rrule['BYMONTH'])) {
    return FALSE;
  }
  if (!empty($rrule['BYMONTHDAY'])) {
    // Test month days, but only if there are no negative numbers.
    $test = TRUE;
    $BYMONTHDAYS = array();
    foreach ($rrule['BYMONTHDAY'] as $day) {
      if ($day > 0) {
        $BYMONTHDAYS[] = $day;
      }
      else {
        $test = FALSE;
        break;
      }
    }
    if ($test && !empty($BYMONTHDAYS) && !in_array(date_format($current_day, 'j'), $BYMONTHDAYS)) {
      return FALSE;
    }
  }
  // Don't add a day if it is already saved so we don't throw the count off.
  if (in_array($formatted, $days)) {
    return TRUE;
  }
  else {
    $days[] = $formatted;
  }
}

/**
 * Stop when $current_day is greater than $end_date or $count is reached.
 */
function date_repeat_is_finished($current_day, $days, $count, $end_date) {
  if (($count && sizeof($days) >= $count)
  || date_format($current_day, 'U') > date_format($end_date, 'U')) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Set a date object to a specific day of the month.
 *
 * Example,
 *   date_set_month_day($date, 'Sunday', 2, '-')
 *   will reset $date to the second to last Sunday in the month.
 *   If $day is empty, will set to the number of days from the
 *   beginning or end of the month.
 */
function date_repeat_set_month_day($date_in, $day, $count = 1, $direction = '+', $timezone = 'UTC') {
  if (is_object($date_in)) {
    $current_month = date_format($date_in, 'n');

    // Reset to the start of the month.
    // We should be able to do this with date_date_set(), but
    // for some reason the date occasionally gets confused if run
    // through this function multiple times. It seems to work
    // reliably if we create a new object each time.
    $datetime = date_format($date_in, DATE_FORMAT_DATETIME);
    $datetime = substr_replace($datetime, '01', 8, 2);
    $date = new DateObject($datetime, $timezone);
    if ($direction == '-') {
      // For negative search, start from the end of the month.
      date_modify($date, '+1 month');
    }
    else {
      // For positive search, back up one day to get outside the
      // current month, so we can catch the first of the month.
      date_modify($date, '-1 day');
    }

    if (empty($day)) {
      date_modify($date, $direction . $count . ' days');
    }
    else {
      // Use the English text for order, like First Sunday
      // instead of +1 Sunday to overcome PHP5 bug, (see #369020).
      $order = date_order();
      $step = $count <= 5 ? $order[$direction . $count] : $count;
      date_modify($date, $step . ' ' . $day);
    }

    // If that takes us outside the current month, don't go there.
    if (date_format($date, 'n') == $current_month) {
      return $date;
    }
  }
  return $date_in;
}

/**
 * Set a date object to a specific day of the year.
 *
 * Example,
 *   date_set_year_day($date, 'Sunday', 2, '-')
 *   will reset $date to the second to last Sunday in the year.
 *   If $day is empty, will set to the number of days from the
 *   beginning or end of the year.
 */
function date_repeat_set_year_day($date_in, $day, $count = 1, $direction = '+', $timezone = 'UTC') {
  if (is_object($date_in)) {
    $current_year = date_format($date_in, 'Y');

    // Reset to the start of the month.
    // See note above.
    $datetime = date_format($date_in, DATE_FORMAT_DATETIME);
    $datetime = substr_replace($datetime, '01-01', 5, 5);
    $date = new DateObject($datetime, $timezone);
    if ($direction == '-') {
      // For negative search, start from the end of the year.
      date_modify($date, '+1 year');
    }
    else {
      // For positive search, back up one day to get outside the
      // current year, so we can catch the first of the year.
      date_modify($date, '-1 day');
    }
    if (empty($day)) {
      date_modify($date, $direction . $count . ' days');
    }
    else {
      // Use the English text for order, like First Sunday
      // instead of +1 Sunday to overcome PHP5 bug, (see #369020).
      $order = date_order();
      $step = $count <= 5 ? $order[$direction . $count] : $count;
      date_modify($date, $step . ' ' . $day);
    }

    // If that takes us outside the current year, don't go there.
    if (date_format($date, 'Y') == $current_year) {
      return $date;
    }
  }
  return $date_in;
}