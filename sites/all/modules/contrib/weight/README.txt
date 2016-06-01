Overview
--------
This module provides a weight field for enabled content types. The default
frontpage is overridden to sort nodes first by sticky, then weight, then created
date. Nodes with a lower weight will be positioned before those with higher
weights.

Installation
------------
This module can be installed by following the instructions at
http://drupal.org/node/70151.

Configuration
-------------
To enable Weight for a content type, go to Administration > Structure > Content
types and select the content type you want to enable Weight for. Select the Weight
vertical tab, choose your desired settings, and click the Save button.

Set a node weight
-----------------
To set the weight for a node, go to that node's edit page, select the Weight
vertical tab, and choose the desired weight. If the content type is set to use
menu weight, the weight selector will be disabled.

Views
-----
Weight includes a default view that provides a drag-and-drop interface for
reordering nodes. Go to Administration > Structure > Views, locate the disabled
Weight view, and click enable. This view can now be edited as normal to fit your
needs. Note that the Weight field can’t be the first field in the view.

Node weights can also be used in new or existing views as Filters, Sorts, or
Arguments.
