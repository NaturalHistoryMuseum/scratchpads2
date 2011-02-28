<?php
// $Id: date_token.inc,v 1.3 2011/01/18 00:37:34 karens Exp $
/**
 * @file
 * Token module integration.
 */

function date_token_list($type = 'all') {
  if ($type == 'field' || $type == 'all') {
    $tokens = array();

    $tokens['date']['value']          = t("The raw date value.");
    $tokens['date']['view']           = t("The formatted date.");
    $tokens['date']['timestamp']      = t("The raw date timestamp.");
    $tokens['date']['yyyy']           = t("Date year (four digit)");
    $tokens['date']['yy']             = t("Date year (two digit)");
    $tokens['date']['month']          = t("Date month (full word)");
    $tokens['date']['mon']            = t("Date month (abbreviated)");
    $tokens['date']['mm']             = t("Date month (two digit, zero padded)");
    $tokens['date']['m']              = t("Date month (one or two digit)");
    $tokens['date']['ww']             = t("Date week (two digit)");
    $tokens['date']['date']           = t("Date date (YYYY-MM-DD)");
    $tokens['date']['datetime']       = t("Date datetime (YYYY-MM-DDTHH:MM:SS)");
    $tokens['date']['day']            = t("Date day (full word)");
    $tokens['date']['ddd']            = t("Date day (abbreviation)");
    $tokens['date']['dd']             = t("Date day (two digit, zero-padded)");
    $tokens['date']['d']              = t("Date day (one or two digit)");
    $tokens['date']['dS']             = t("Date day (one or two digit) with ordinal suffix (st, nd, rd or th)");
    $tokens['date']['time']           = t("Time H:i");

    $tokens['date']['to-????']        = t("If the field has a to-date defined, the same tokens exist in the form: [to-????], where ???? is the normal token.");

    return $tokens;
  }
}

function date_token_values($type, $object = NULL) {
  if ($type == 'field') {
    $item = $object[0];
    $item['value'] = trim($item['value']);
    $item_type = isset($item['date_type']) ? $item['date_type'] : (is_numeric($item['value']) ? DATE_UNIX : DATE_ISO);
    $timezone = !empty($item['timezone']) ? $item['timezone'] : date_default_timezone();
    $timezone_db = !empty($item['timezone_db']) ? $item['timezone_db'] : 'UTC';
    $date = new DateObject($item['value'], $timezone_db, date_type_format($item_type));
    if (!empty($date) && $timezone_db != $timezone) {
      date_timezone_set($date, timezone_open($timezone));
    }

    $tokens['value']          = $item['value'];
    $tokens['view']           = $item['view'];
    $tokens['timestamp']      = !empty($date) ? date_format_date($date, 'custom', 'U') : '';
    $tokens['yyyy']           = !empty($date) ? date_format_date($date, 'custom', 'Y') : '';
    $tokens['yy']             = !empty($date) ? date_format_date($date, 'custom', 'y') : '';
    $tokens['month']          = !empty($date) ? date_format_date($date, 'custom', 'F') : '';
    $tokens['mon']            = !empty($date) ? date_format_date($date, 'custom', 'M') : '';
    $tokens['mm']             = !empty($date) ? date_format_date($date, 'custom', 'm') : '';
    $tokens['m']              = !empty($date) ? date_format_date($date, 'custom', 'n') : '';
    $tokens['ww']             = !empty($date) ? date_format_date($date, 'custom', 'W') : '';
    $tokens['date']           = !empty($date) ? date_format_date($date, 'custom', DATE_FORMAT_DATE) : '';
    $tokens['datetime']       = !empty($date) ? date_format_date($date, 'custom', DATE_FORMAT_ISO) : '';
    $tokens['day']            = !empty($date) ? date_format_date($date, 'custom', 'l') : '';
    $tokens['ddd']            = !empty($date) ? date_format_date($date, 'custom', 'D') : '';
    $tokens['dd']             = !empty($date) ? date_format_date($date, 'custom', 'd') : '';
    $tokens['d']              = !empty($date) ? date_format_date($date, 'custom', 'j') : '';
    $tokens['dS']             = !empty($date) ? date_format_date($date, 'custom', 'jS') : '';
    $tokens['time']           = !empty($date) ? date_format_date($date, 'custom', 'H:i') : '';

    if (!empty($item['value2'])) {

      $item['value2'] = trim($item['value2']);
      $date = new DateObject($item['value2'], $timezone_db, date_type_format($item_type));
      if ($timezone_db != $timezone) {
        date_timezone_set($date, timezone_open($timezone));
      }

      $tokens['to-value']          = $item['value2'];
      $tokens['to-view']           = $item['view'];
      $tokens['to-timestamp']      = !empty($date) ? date_format_date($date, 'custom', 'U') : '';
      $tokens['to-yyyy']           = !empty($date) ? date_format_date($date, 'custom', 'Y') : '';
      $tokens['to-yy']             = !empty($date) ? date_format_date($date, 'custom', 'y') : '';
      $tokens['to-month']          = !empty($date) ? date_format_date($date, 'custom', 'F') : '';
      $tokens['to-mon']            = !empty($date) ? date_format_date($date, 'custom', 'M') : '';
      $tokens['to-mm']             = !empty($date) ? date_format_date($date, 'custom', 'm') : '';
      $tokens['to-m']              = !empty($date) ? date_format_date($date, 'custom', 'n') : '';
      $tokens['to-ww']             = !empty($date) ? date_format_date($date, 'custom', 'W') : '';
      $tokens['to-date']           = !empty($date) ? date_format_date($date, 'custom', DATE_FORMAT_DATE) : '';
      $tokens['to-datetime']       = !empty($date) ? date_format_date($date, 'custom', DATE_FORMAT_ISO) : '';
      $tokens['to-day']            = !empty($date) ? date_format_date($date, 'custom', 'l') : '';
      $tokens['to-ddd']            = !empty($date) ? date_format_date($date, 'custom', 'D') : '';
      $tokens['to-dd']             = !empty($date) ? date_format_date($date, 'custom', 'd') : '';
      $tokens['to-d']              = !empty($date) ? date_format_date($date, 'custom', 'j') : '';
      $tokens['to-dS']             = !empty($date) ? date_format_date($date, 'custom', 'jS') : '';
      $tokens['to-time']           = !empty($date) ? date_format_date($date, 'custom', 'H:i') : '';

    }
    return $tokens;
  }
}
