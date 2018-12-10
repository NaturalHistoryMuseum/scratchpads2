
                           biblio.module

Author:  Ron Jerome 
Released under the GPL


Description:
============
This module extends the node data type with additional fields to manage lists 
of scholarly publications.

It closely follows the EndNote model, thus both importing from and exporting 
to Endote are supported. Other formats such as bibtex and RIS are also supported.

Bibliographic information is displayed in lists with links to detailed 
information on each publication.

The lists can be sorted, filtered and ordered in many different ways.




Requirements:
=============
Drupal 7.x, Upgrades supported from any Biblio version after 6.x-1.9

Installation:
=============
Create a directory called biblio in the sites/all/modules directory, then place all of the
files packaged with this module in that directory.

This module will auto-install the required database tables the first time you 
enable it on the admin/modules page.   This will also setup a number of pre-defined 
publication types.  These types can be changed or deleted on the 
admin/config/content/biblio/types page.

Robots.txt
==========
In order to limit recursive searches by web bots, it is recommended that you add the following
to your robots.txt file.  Note: if you change the base url for biblio you will have to make the 
corresponding change here.  i.e. if you base url for biblio is "publications" then replace "biblio"
with "publications" in the directives below.

# Biblio
Disallow: /biblio/export/
Disallow: /biblio?*
Disallow: /biblio?page=*&*
Allow: /biblio?page=*


Settings:
=========
A number of settings are available at admin/config/content/biblio.  They control how 
the author names are displayed, whether export links are added to pages and the
number of entries per page to display.

The 'admin/config/content/biblio/fields' page allows the the site administrator to set
the default field titles and set which fields are common to all publication 
types.  When a new publication type is added, it will contain all the common 
fields and any that are specifically activated (custom is checked).  This also
allows the admin to over ride any of the default settings for any given type.

Access Control:
===============
Three permissions are controlable on the admin/access page.  I think they are fairly
self evident, they control who can create biblio entries, edit entries and who can
import from file.

Adding/importing records:
=========================
Bibliographic entries can be added to the database in one of two ways, individualy
from the node/add/biblio link, or by importing records from one of the supported file 
formats.  Administrators can go to 'admin/config/content/biblio/import' and fill in 
the form to upload and import publication data from files.


Features:
=========
By default, the /biblio page will list all of the entries in the database sorted
by Year in descending order. If you wish to sort by "Title" or "Type", you may 
do so by clicking on the appropriate links at the top of the page. To reverse 
the sort order, simply click the link a second time.


Filtering Search Results:
=========================
If you wish to filter the results, click on the "Filter" tab at the top of the 
page. To add a filter, click the radio button to the left of the filter type 
you wish to apply, then select the filter criteria from the drop down list 
on the right, then click the filter button.

It is possible to create complex filters by returning to the "Filter" tab and 
adding additional filters. Simply follow the steps outlined above and press 
the "Refine" button.

All filters can be removed by clicking the Clear All Filters link at the top 
of the result page, or on the "Filter" tab they can be removed one at a time 
using the "Undo" button, or you can remove them all using the "Clear All" button

You may also construct URLs which filter. For example, /biblio/year/2005 will 
show all of the entries for 2005. /biblio/year/2005/author/smith will show all 
of entries from 2005 for smith.


Exporting Search Results:
=========================
Assuming this option has been enabled by the administrator, you can export 
search results directly into EndNote. The link at the top of the result page 
will export all of the search results, and the links on individual entries will 
export the information related to that single entry.

Clicking on one of the export links should cause your browser to ask you 
whether you want to Open, or Save To Disk, the file endnote.enw. If you choose 
to open it, Endnote should start and ask you which library you would like 
store the results in. Alternatively, you can save the file to disk and manually 
import it into EndNote.


The information is exported in either EndNote "Tagged" format similar to this...

              %0  Book
              %A  John Smith 
              %D  1959
              %T  The Works of John Smith
              ...
              
Or Endnote 7 XML format which is similar to this...

              <XML>
                <RECORDS>
                  <RECORD>
                    <REFERENCE_TYPE>10</REFERENCE_TYPE>
                    <YEAR>1959</YEAR>
                    <TITLE>The Works of John Smith</TITLE>
                    <AUTHORS>
                      <AUTHOR>John Smith </AUTHOR>
                    </AUTHORS>
                  </RECORD>
                </RECORDS>
              </XML>
              
              
