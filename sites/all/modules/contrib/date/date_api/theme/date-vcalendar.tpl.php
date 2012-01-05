<?php
// $Id$
/**
 * $calname
 *   The name of the calendar.
 * $events
 *   @see date-vevent.tpl.php.
 *   @see date-valarm.tpl.php.
 */
if (empty($method)) {
  $method = 'PUBLISH';
}
?>
BEGIN:VCALENDAR
VERSION:2.0
METHOD:<?php print $method; ?>
<?php if (!empty($calname)): ?>
X-WR-CALNAME;VALUE=TEXT:<?php print $calname . "\r\n"; ?>
<?php endif; ?>
PRODID:-//Drupal iCal API//EN
<?php foreach($events as $event): ?>
<?php print theme('date_vevent', $event); ?>
<?php endforeach; ?>
END:VCALENDAR
