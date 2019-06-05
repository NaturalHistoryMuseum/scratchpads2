Lucid Keys
==========

Lucid software is a special type of expert system, specifically designed
for identification and diagnostic purposes. Lucid identification keys
are currently being used by a wide range of end-users, from high school
and university students to taxonomists, quarantine identifiers,
biodiversity scientists and conservation managers. If you have a Lucid
key you can now embed and fully use it through the Scratchpads
environment.

This will give you the opportunity to run the keys in separate
standalone pages or embed them in selected taxon overview pages. You can
have multiple lucid keys running in your Scratchpad, even on the same
page.

To have a lucid key file displayed as a working key in your Scratchpad
you will have to create a separate field in any of existing or in a new
content type in your site.

The following steps will guide you to embed a
Lucid key in a new content type called *Keys*

Enabling the module
~~~~~~~~~~~~~~~~~~~

1. From the :term:`Admin menu` go to *Structure > Tools*
2. Find and enable *Scratchpads Lucid applet* under the *Other* section
   of the page
3. Click *Save*

Creating the new content type
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*You should skip this part if you want to add the lucid key field to an
existing content type*

1. From the :term:`Admin menu` go to *Structure > Content types*
2. Click on *+ Add content type*
3. Provide a Name (e.g. Keys)
4. Provide a description (not required)
5. In the horizontal tabs go to *Publishing options* and select
   *Published*
6. Click *Save*

If you want the keys to display on the taxon overview pages - see :doc:`/advanced-features/custom-content-taxon-pages`.

To add a menu item for the content type, see :doc:`/advanced-features/customise-menus`.


Adding the Lucid key field in a content type
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. From the :term:`Admin menu` go to *Structure > Content types*.

2. Click on *Manage fields* next to content type you wish to add the lucid keys functionality

3. Find the Add new Field section. Provide a *Label* for the field: “Lucid Key”

4. Under *Select a field type* drop-down select *File*

5. Under *Select a widget*  drop-down select *File*

6. Click *Save*

7. In the next screen select both *Enable Display field* and *Files displayed by default*

8.  Click *Save field settings*

9. In the next screen go to *ALLOWED FILE EXTENSIONS* and replace the string 'txt' with 'zip'.

11. Click *Save settings*

12. You will be redirected back to the *Manage fields* page. Click on *Manage Display* on the right top corner of the overlay window

13. Find the Lucid key field you created and select *Lucid3 Key player applet* from the Format column drop-down box

14. Click *Save*


Uploading a Lucid Key file
~~~~~~~~~~~~~~~~~~~~~~~~~~

If you do not have your own lucid key file but want to try out the functionality, you can download `North-Western Palaearctic species of Pristiphora (Hymenoptera, Tenthredinidae): Lucid identification key`__.

__ https://figshare.com/articles/North-Western_Palaearctic_species_of_Pristiphora_Hymenoptera_Tenthredinidae_Lucid_identification_key/5235805

1. Go to Content and click *Add* next to the content type in which you previously added the “Lucid Key” field

2. Provide a title to your page (can be the higher taxon of the key or a short description of the key)

3. Under the field “Lucid Key” click *Browse* to find a valid (lkc3/lkc4 file).

4. Click Upload

5. Once uploaded (might take several minutes) select *Include file in
   display*

7. From the Horizontal tabs menu go to *Publishing options* and click
   *Published*

8. Click *Save*

Viewing your Key
~~~~~~~~~~~~~~~~

Depending on whether you have selected for a link to be added to the
main menu or/and if you have selected for the content type data to be
displayed on the taxon overview pages you should be able to see your
keys working

Browsing the Key
~~~~~~~~~~~~~~~~

You can find more on how to use the Lucid environment by going to
http://www.lucidcentral.com





6. If you want to tag the key with a taxonomic name so that will appear
   on the taxon overview pages then:

   1. Find the *Add existing field* section
   2. Under the *Field to share* drop-down list select *Term reference:
      field_taxonomic_name (Taxonomic name)*
   3. Under the *Form* element to edit the data drop-down list select
      *Autocomplete term widget (tagging)* or any of the other ways to
      select a taxon from an existing classification
