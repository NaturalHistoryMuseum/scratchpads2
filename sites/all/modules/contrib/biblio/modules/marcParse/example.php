<?php

/**
 * @package PHP-MARC
 */

//-----------------------------------------------------------------------------
//
// Copyright (C) 2003-2005 Oy Realnode Ab
//
//-----------------------------------------------------------------------------
//
// example.php
//     Part of the Emilda Project (http://www.emilda.org/)
//
// Description
//     Examples how to use PHP-MARC
//
// Authors
//     Christoffer Landtman <landtman (at) realnode com>
//
//-----------------------------------------------------------------------------
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
//-----------------------------------------------------------------------------
//
// $Revision$
//
//-----------------------------------------------------------------------------

require_once "../php-marc/php-marc.php";

// Other way to access file
/*$string = file("example.mrc");
$file = new USMARC($string[0]);*/

// Open file
$file = new File("example.mrc");

// Read next record
$record = $file->next();

// Create new field
$field = new Field("245", "", "", array("a" => "Mumin"));
// Add subfield
$field->add_subfields(array("b" => "Det Osynliga Barnet"));
// Other ways to update field
$field->update(array("ind2" => "1", "b" => "Vinter i Mumindalen", "c" => "Tove Jansson"));

// Replace existing field
$existing =& $record->field("245");
$existing->replace_with($field);

$clone = $field->make_clone();
// Change some more
$clone->update(array("a" => "Muminsagor", "b" => "Muminpappans memoarer"));

// And append to record
$record->append_fields($clone);

// Some output
print "<pre>";
print $record->formatted();
print "\n\n";
print $file->raw[0];
print "\n";
print $record->raw();
print "\n\n";
print $record->ffield("245", "Formatted output: Title: <b>%a</b>, Remainder of title: <b>%b</b>, Responsibility: <b>%c</b>\n");
print "</pre>";

?>