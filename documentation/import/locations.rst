Import locations
================

Excel file imports can be used either for creating new nodes or for
updating existing ones. Please see the :doc:`/import/content` page for more general info.

To download the template file (for new data or for updating your data)

-  Go to Import > Nodes > Excel file import
-  Choose locations from the second drop-down menu
-  Choose the template file you wish to work on (choose the first for
   creating new data or the second for changing or amending existing
   scratchpad data)
-  Open the downloaded excel file in your computer and edit it

.. figure:: /_static/Import_locations_p1.png

**Complete the columns as follows:**

-  **GUID**:

Use Global Unique Identifiers for each one of your localities. These
identifiers will not show up to users. Use complex combinations like
EU-GB-S-DK-000-001

-  **Title**:

The title is the location identifier and will be used to refer to each
specific location in your scratchpad. Make it human readable.

-  **Continent/Ocean**:

Select the continent or Ocean of your locality. Choose one from the
following: *Africa, Antarctic, Asia-Temperate, Asia-Tropical, Atlantic
Ocean, Australasia, Europe, Indian Ocean, North-Central Pacific,
Northern America, Northwestern Pacific, Pacific, South-Central Pacific,
Southern America, Southwestern Pacific*

-  **Verbatim Coordinate System**:

Provide the name of the system the coordinates of the locality were
recorded. Choose one from the following: *decimal degrees, degrees
decimal minutes, degrees minutes seconds, UTM, CRTM*.

-  **Coordinate uncertainty in meters**:

The upper limit of the distance (in meters) from the given latitude and
longitude describing a circle within which the whole of the described
locality lies. Leave the value empty if the uncertainty is unknown,
cannot be estimated, or is not applicable (because there are no
coordinates). The value should be greater than zero.

-  **Country (ISO alpha-2)**:

Choose the administrative region (Country) the locality is in. Please
fill in the the official ISO 3166-1-alpha-2 country code. An exhaustive
list can be found `here`_.

.. figure:: /_static/Import_locations_p2.png

-  **County**:

Provide the the name of a second-level administrative subdivision of a
country.

-  **Geodetic datum**:

Provide the geodetic datum of the locality coordinates. e.g. WGS84,
NAD83, NAD27 etc.

-  **Georeference protocol**:

Give a citation (publication or URL) to the resource describing the
methods used to determine the georeference (coordinates AND uncertainty,
or footprint)

-  **Georeference remarks**:

Comments about the spatial description determination, explaining
assumptions made in addition or opposition to the those formalized in
the method referred to in Georeference Protocol.

.. _here: http://www.iso.org/iso/english_country_names_and_code_elements