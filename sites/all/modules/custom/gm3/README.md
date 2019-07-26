# Geo Mapping Management Module (gm3)

Scratchpads needs a way of displaying geo content on maps; points, lines, polygons, regions, etc. Aditionally users may want to use maps as form inputs, or add a map as part of a body of content. These are all reasonable requests, though can be complex to implement in Drupal. GM3 and its children comprise an ambitious attempt to implement these features. The following document is a guideline to how.

## Modules

Gm3 is split into the following:

  - **gm3**: Defines theme hooks for displaying maps
  - **gm3_field**: Defines fields, widgets and form elements allowing the use of maps as input elements
  - **gm3_region**: An extension to the core gm3/gm3_field functions that adds TDWG region support
  - **gm3_filter**: Lets users add maps to custom content areas

See also: eol_gbif_maps_module

## Theme hooks

### gm3.module

gm3_views_api (hook)
gm3_library (hook)
gm3_theme hook
gm3_form_views_ui_add_form_alter h
gm3_load_geophp
gm3_permission h
gm3_menu h
gm3_settings

Define themes:
  gm3_map
  gm3_view_gm3
  gm3_point_button
  gm3_polygon_button
  gm3_rectangle_button
  gm3_polyline_button
  gm3_beautytip

#### gm3_map

Variables / defaults:
    map => [
        'id' => 'gm3-map',
        'libraries' => [], // Javascript libraries to include & their settings
        'tools' => [], // Buttons to add to the sidebar
        'settings' => [] // General map JS settings
    ]

Renders a basic map, which is essentially just a `<div id>` which is hooked into by the Leaflet.js library. We pass the id of the element into the javascript through drupal_add_js.

Maps need to define what tools to include and points to render on them. The apropriate js library for each can then get included and the values passed to the script, along with the settings.

#### gm3_view_gm3

This is for viewing multiple geo records as one map. It's used in conjunction with the views module - see gm3.views.inc for config.

Does a bunch of extra work but ultimately calls gm3_map.

## gm3_field

### Themes

Defines the gm3_field theme, which expects input elements as children, finds the map values based on that, renders the map,
and then renders the children. See theme_gm3_field.

### Elements

Defines the gm3_field element, which expects the actual input fields as its children, and uses the gm3_field theme to render.
gm3_field_element_info

### Fields

The install file defines the various geo field types made available by this module.

### Widgets

Defines the available widgets using gm3_field_field_widget_info, and what field types they're available for - gm3_field_field_widget_info.

Also defines gm3_field_field_widget_form, the hook that defines how various gm3 widgets are generated; ultimately returns an instance of the gm3_field element.

### Other?

## Refactor

Should there be a different theme and element fro each field/widget? Or should they share the same?

E.g. the points field should have:
    - a text widget
    - a map widget
    - a combined widget

    points widget should:
      - return a fieldset element containing
      + map with points libraries and tools
      + points field element

I don't think any extra elements or themes are needed,
just a bunch of widgets with functions for converting
values from text to database (for both text and map widgets).