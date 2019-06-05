Autotagging
===========

Once you have added terms to a biological classification, new content
will be automatically tagged to the classification by default. This is
called autotagging. The system will look for the presence of terms in
the text and if it finds a term it will add a link to this term to the
`node`_. Autotagging can be problematic because if names of other taxa
are mentioned in the text, for example while comparing characters, then
these will be tagged, too. For some content types like taxon
descriptions or images, this can be undesirable. The other problem is
that species names will automatically be tagged to the genus name as
well as the species name because both are part of the name. But for
example a species description should only be linked to the respective
species.

Disable autotagging for a certain content type
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. figure:: /_static/AdminStructure.jpg
   :alt: Structure administration page

   Structure administration page

-  Click on *Structure* in the :term:`Admin menu` and click on Content types.

   .. figure:: /_static/ContentTypes.jpg
      :alt: Content types administration page

      Content types administration page


-  Click on the edit link for the content type, e.g. for taxon description.
   
   .. figure:: /_static/EditContentType.jpg
      :alt: Edit content type administration page

      Edit content type administration page

-  Click on the vertical Tag options tab near the bottom.

   .. figure:: /_static/EditContentTypeVoc.jpg
      :alt: Tag options on the edit content type administration page

      Tag options on the edit content type administration page

-  Unselect (Control+Click on PC) all classifications/vocabularies in
   the ‘Vocabularies’ box close to the bottom of the page. Save.
