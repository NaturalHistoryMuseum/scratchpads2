
Current for 6.x-2.0-alpha2

# Description

OpenLayers is a suite of module to help integrate the
OpenLayers Javascript library within Drupal.

# Requirements

OpenLayers Requires

* CTools: http://drupal.org/project/ctools

Sub-modules require

* OpenLayers Views requires Views: http://drupal.org/project/views
* OpenLayers CCK requires CCK: http://drupal.org/project/cck

OpenLayers can use Features to package maps into modules

* Features: http://drupal.org/project/features

# Sub-Modules

You'll most likely want to enable OpenLayers UI and OpenLayers Views modules for typical uses of OpenLayers.

* OpenLayers UI - Creates a user interface to make map
  arrays which can be used in various places
* CCK - Allows integration with CCK so that geo data
  can be inputted via a map
* Views - Allows data to be displayed on a map
  via Views
* Filters - Creates a filter to replace
  [openlayers map_name] with a map.

# Basic Concepts

## Maps

An OpenLayers map in Drupal is a combination of decisions about which 
layers to show, what styles to apply, the minimum and maximum zoom levels, 
and how the user can interact with the map. A map object is where 
the options for these settings are combined and stored. Maps can be 
rendered through OpenLayers views of the type "OpenLayers Map," through 
filters provided by the OpenLayers Filters module, or directly in your 
modules using the api function openlayers_render_map()

## Layers

OpenLayers allows for the display of a variety of raster and vector data on 
maps. These are divided into two categories: basemaps and overlays. 
Basemaps, like those from OpenStreetMap or Google Maps, provide a background
for your data. Overlays can be data from OpenLayers data views, KML layers, 
or other types of data from external sources.

For more information on layer types, see docs/LAYER_TYPES.txt

## Styles

The OpenLayers module currently allows users to define custom styles for 
vector overlays. The primary use-case here will be data from OpenLayers 
data views but this could also include KML files. Styles define things like 
stroke width, background and outline colors, and point size and allow the 
use of icons if desired. They must be associated with layers on a per-
map basis.

## Behaviors

Behaviors can be enabled on maps to control how users interact with 
the map. Examples of behaviors include navigation, which, when enabled, 
allows users to zoom and pan, and Layer Switcher, which provides a UI that 
allows users to toggle on and off the layers available on the map.

## OpenLayers Views

OpenLayers views come in two varieties. The first is a display type called 
OpenLayers Data. This is what allows users to control which pieces of 
content show up on the map and in what manner. Each OpenLayers data view 
provides a layer that can be added to maps when configuring a map. The 
second is a style called OpenLayers Map that can be applied to pages, 
blocks, attachments, and so on and give users an easy way of rendering 
maps.

# Installation

1. Normal Drupal module installation

2. Set OpenLayers Source at: admin/structure/openlayers
   It is suggested to download the library and use it locally for performance.

3. A tutorial 


# Documentation

More documentation is available in the docs/ directory of this module:

* CUSTOMIZATION.txt: a cookbook for common customizations users might want 
  to make for performance and tweaking

* API.txt: a technical description of the main map-generating API

* LAYER_TYPES.txt: a technical description of how layer types work and 
  guide for implementation of new layer types

* KML.txt: an explanation of how KML support works in OpenLayers

* JAVASCRIPT.txt: a technical explanation of how the Javascript included 
  in the OpenLayers module (not the code in the actual OpenLayers library) 
  operates

A [tutorial is available on Drupal.org](http://drupal.org/node/627816)
  
# Authors/Credits

* [zzolo](http://drupal.org/user/147331)
* [phayes](http://drupal.org/user/47098)
* [tmcw](http://drupal.org/user/12664)
* [brynbellomy](http://drupal.org/user/537416)
* [bdragon](http://drupal.org/user/53081)
* [OpenLayers monster by Saman Bemel Benrud](http://www.flickr.com/photos/samanpwbb/)
* [strk](http://drupal.org/user/781486)
