-- SUMMARY --

Charts and Graphs is a API for developers. It can easily be extended by
third-party modules that want to add their own charting implementations. It does
nothing by itself. It should only be installed if some other module requires it.

See Views Charts <http://drupal.org/project/views_charts> for usage of a
user-centric application as the Charts and Graphs module is a framework.

There are several modules that provide the rendering of charts for this module:

# http://drupal.org/project/charts_graphs_flot
# more to follow when they are promoted to projects

Please read the README/INSTALL files of the implementations since their installation 
may involve downloading third-party libraries. Drupal guidelines does
not always allow packaging those with Drupal code.

For a full description of the module, visit the project page:
  http://drupal.org/project/charts_graphs

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/charts_graphs


-- REQUIREMENTS --

None


-- SUPPORT CHARTING LIBRARIES --

* Bluff <http://bluff.jcoglan.com/> - Beautiful Graphics in Javascript - MIT and
  GPL license

* Google Charts <http://code.google.com/apis/charttools/index.html> - Is free to
  use. Google ask you to contact them if you plan on producing more than 250,000
  API calls per day, see Google Chart Usage Policy at
  <http://code.google.com/apis/chart/docs/making_charts.html>.

* Flot http://code.google.com/p/flot/ 


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.

* Choose one of the supported submodules and install that 

* There is additional information available at <http://drupal.org/node/681660>.
  Google Charts can be used right away. Bluff needs a additional download to 
  work in IE. 


-- UPGRADE --

* Upgrading Charts Graphs from Drupal 6 to Drupal 7 is entirely untested. If there 
are issues please post them in the issue queue (or even better, fix them and post
patches there)


-- CONFIGURATION --

* The configuration options are available at admin/config/charts_graphs. They
  deal only with the appearance or not of a few warnings at the "Status reports"
  page.


-- TROUBLESHOOTING --

Empty.


-- FAQ --

Q: Empty

A: Empty.
