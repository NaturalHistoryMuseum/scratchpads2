
CONTENTS OF THIS FILE
---------------------

* Introduction 
* Installation 
* API


INTRODUCTION 
------------

Authors: 
* Jeff Miccolis (jmiccolis)
* Jelle Sebreghts (Jelle_S)
* Peter Droogmans (attiks)

Support was provided by Development Seed and the New America Foundation.

Flot is a javascript graphing library written using the jquery that we all know
and love. This module is designed to make it dead-simple to push data into a
flot graph.


INSTALLATION
------------

1. Copy the flot module directory to your sites/SITENAME/modules directory.

2. Download flot from http://code.google.com/p/flot/

3. Extract it as a sub-directory called 'flot' in the flot module folder. For
example:

 /sites/all/modules/flot/flot/

4. Enable the module at Administer >> Site building >> Modules.


API
---

The main API function is a theme function - theme_flot_graph(). It is designed
to mimic the main $.plot() function that flot provides. Think of it as a php
wrapper. It takes the same arguments in the same order as $.plot and returns
the placeholder element that flot will use to make it's graph. Please refer to
API.txt included in the flot package for more details on how to make flot work.

To get started quickly try the following example:

<?php

$d1 = new flotData(array(array(0, 1), array(4, 8), array(8, 5)));
$d2 = new flotData(array(array(0, 8), array(3, 5), array(8, 0.5)));
print theme('flot_graph', array(), array($d1, $d2));

?>
