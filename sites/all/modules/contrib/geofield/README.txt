                       __ _      _     _ 
                      / _(_)    | |   | |
      __ _  ___  ___ | |_ _  ___| | __| |
     / _` |/ _ \/ _ \|  _| |/ _ \ |/ _` |
    | (_| |  __/ (_) | | | |  __/ | (_| |
     \__, |\___|\___/|_| |_|\___|_|\__,_|
      __/ |                              
     |___/                               
     
     Geofield is a module for storing geographic data in drupal 7.
     It supports all geo-types (points, lines, polygons, multitypes etc.)

     http://drupal.org/project/geofield

     
     Dependancies:

     libraries    provides API for handling libraries     http://drupal.org/project/libraries
     geoPHP       provides geometry transformations       https://github.com/phayes/geoPHP
     

     Related Modules:

     openlayers   provides mapping for geofield           http://drupal.org/project/openlayers
     geocode      provides geocode widget for geofield    https://github.com/phayes/geocode


     Credit:
     
     Original author:  Tristan O'Neil
     Contributors:     Alex Barth, Jeff Miccolis, Young Hahn, Tom MacWright, Patrick Hayes,
                       Dave Tarc, Nikhil Trivedi, Marek Sotak, Khalid Jebbari

     
     API Notes:
     
     Geofield fields contain 9 columns of information about the geographic data that is stores.
     At it's heart is the 'wkt' column where it stores the full geometry in the 'Well Known Text'
     (WkT) format. All other columns are metadata derived from the WKT column. Columns are as follws:
       'wkt'          WKT
       'geo_type'     Type of geometry (point, linestring, polygon etc.)
       'lat'          Centroid (Latitude or Y)
       'lon'          Centroid (Longitude or X)
       'top'          Bouding Box Top (Latitude or Max Y)
       'bottom'       Bouding Box Bottom (Lattidue or Min Y)
       'left'         Bouding Box Left (Longitude or Min X)
       'right'        Bouding Box Right (Longitude or Max X)

     When a geofield is saved using the provided widgets, these values are passed through the
     geofield_compute_values function in order to compute dependant values. By default dependant 
     values are computed based on WKT, but this may be overriden to compute values based on other
     columns. For example, geofield_compute_values may be called like so:
     
       geofield_compute_values($values, 'latlon');
     
     This will compute the wkt field (and all other fields) based on the lat/lon columns, resulting
     in a point. As a developer this is imporant to remember if you modifying geofield information
     using node_load and node_save. Make sure to run any modified geofield instances through
     geofield_compute_values in order to make all columns consistant.

