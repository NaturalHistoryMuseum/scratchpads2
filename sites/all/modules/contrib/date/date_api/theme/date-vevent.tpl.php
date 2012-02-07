<?php
// $Id$
/**
 * $event
 *   An array with the following information about each event:
 *
 *   $event['uid'] - a unique id for the event (usually the url).
 *   $event['summary'] - the name of the event.
 *   $event['start'] - the formatted start date of the event.
 *   $event['end'] - the formatted end date of the event.
 *   $event['rrule'] - the RRULE of the event, if any.
 *   $event['timezone'] - the formatted timezone name of the event, if any.
 *   $event['url'] - the url for the event.
 *   $event['location'] - the name of the event location, or a vvenue location id.
 *   $event['description'] - a description of the event.
 *   $event['alarm'] - an optional array of alarm values.
 *    @see date-valarm.tpl.php.
 */
?>
BEGIN:VEVENT
UID:<?php print($event['uid'] . "\r\n") ?>
SUMMARY:<?php print($event['summary'] . "\r\n") ?>
DTSTAMP:<?php print($site_timezone_utc . "Z\r\n") ?>
DTSTART;<?php print $event['timezone'] ?><?php print($event['start'] . "\r\n") ?>
<?php if (!empty($event['end'])): ?>
DTEND;<?php print $event['timezone'] ?><?php print($event['end'] . "\r\n") ?>
<?php endif; ?>
<?php if (!empty($event['rrule'])) : ?>
<?php print($event['rrule'] . "\r\n") ?>
<?php endif; ?>
<?php if (!empty($event['url'])): ?>
URL;VALUE=URI:<?php print($event['url'] . "\r\n") ?>
<?php endif; ?>
<?php if (!empty($event['location'])): ?>
LOCATION:<?php print($event['location'] . "\r\n") ?>
<?php endif; ?>
<?php if (!empty($event['description'])) : ?>
DESCRIPTION:<?php print($event['description'] . "\r\n") ?>
<?php endif; ?>
END:VEVENT
