<?php
// $Id$
/**
 * $alarm
 *   An array with the following information about each alarm:
 *
 *   $alarm['action'] - the action to take, either 'DISPLAY' or 'EMAIL'
 *   $alarm['trigger'] - the time period for the trigger, like -P2D.
 *   $alarm['repeat'] - the number of times to repeat the alarm.
 *   $alarm['duration'] - the time period between repeated alarms, like P1D.
 *   $alarm['description'] - the description of the alarm.
 *
 *   An email alarm should have two additional parts:
 *   $alarm['email'] - a comma-separated list of email recipients.
 *   $alarm['summary'] - the subject of the alarm email.
 */
?>
BEGIN:VALARM
ACTION:<?php print $alarm['action']  . "\n";?>
<?php if (!empty($alarm['trigger'])): ?>
TRIGGER:<?php print $alarm['trigger']  . "\n"; ?>
<?php endif; ?>
<?php if (!empty($alarm['repeat'])): ?>
REPEAT:<?php print $alarm['repeat']  . "\n"; ?>
<?php endif; ?>
<?php if (!empty($alarm['duration'])): ?>
DURATION:<?php print $alarm['duration']  . "\n"; ?>
<?php endif; ?>
<?php if ($alarm['action'] == 'EMAIL'): ?>
ATTENDEE:MAILTO:<?php print $alarm['email']  . "\n" ?>
SUMMARY:<?php print $alarm['summary']  . "\n" ?>
<?php endif; ?>
DESCRIPTION:<?php print $alarm['description']  . "\n" ?>
END:VALARM