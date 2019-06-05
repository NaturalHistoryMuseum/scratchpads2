Itis standard fields
====================

By default all Scratchpad taxonomies have three basic fields: the term
name, parent and description. This is not really enough when dealing
with biological classifications. We have therefore developed a special
way to deal with biological classifications that follows the standards
used by the :term:`Integrated Taxonomic Information System (ITIS)` and is
compatible with other initiatives.

The fields we use for biological classifications are:

**Standard Scratchpad fields**:

-  **Term name**: A combination of the different unit names and
   indicators.
-  **Parent term name**: An ITIS term denoting the taxon that is the
   next highest level from the subject valid or accepted taxon. The
   parent of a species is usually a genus; the parent of a genus is
   usually a family, and so on. This parent-child linkage between
   records is the basis for the classification hierarchy in ITIS.
-  **Term description**: The term description field is usually not used
   for biological classifications.
-  **GUID**: The global unique identifier for the term name. GUIDs can
   be used to compare/synchronise different databases. Adding a GUID is
   not required, you only need it if your names were generated from an
   established database and you want to be able to update your names
   from this database at a later stage. Note that the GUID really has to
   be **globally** unique, at least across the whole Scratchpad. So it
   is not enough just to start counting from “1”. Better is a
   combination like “Species2000-1”
-  **Parent GUID**: The global unique identifier for the parent term
   name. Adding a Parent GUID is not required.

**ITIS fields**:

-  **Associated accepted_name**: The scientific name of the valid or
   accepted taxon identified as the currently accepted name used for a
   given invalid or not accepted name. Each name that is in synonymy
   (junior synonyms, obsolete combinations, etc.) must be connected to
   one accepted or valid name.
-  **Authors**: The author(s) associated with the name of a taxon. This
   can include the year of publication and brackets, e.g. “(Quoy and
   Gaimard, 1832)”.
-  **Page number**: The page number the taxon was described on in the
   protologue. This field is not part of the ITIS standard.
-  **Rank**: The label associated with the specific level of a taxonomic
   hierarchy, e.g. “Family”, “Genus” or “Species”. For values check ITIS
   or the taxonomic editor for an existing classification on your site.
   Note that the spelling and capitalisation of the ranks in your import
   file must match the ranks in the Scratchpad!
-  **Unacceptability_reason**: An explanation regarding why a given
   scientific name is judged to be invalid or not accepted.
   Unacceptability reasons vary among kingdoms. For values check ITIS or
   the taxonomic editor for an existing classification on your site.
   Note that the spelling and capitalisation of the reasons in your
   import file must match those in the Scratchpad!
-  **Unit indicator 1**: Indicator of an occurrence of a plant hybrid at
   the generic level. The only value allowed is “X”.
-  **Unit name 1**: The singular or first part of a scientifically
   accepted label for an occurrence of Taxonomic Units. For uninomials
   (1-part names like family or genus names) this will be the only name
   field entered. For binomials/polynomials, this field will be used for
   the first part of the name (the genus name). The names in this
   position require the initial letter to be capitalized.
-  **Unit indicator 2**: A category indicator positioned between the
   first and second parts of a binomial/polynomial taxonomic name. The
   only value allowed is “X”.
-  **Unit name 2**: The second part of a scientifically accepted label
   for a binomial/polynomial occurrence of Taxonomic Units (the species
   epithet).
-  **Unit indicator 3**: A category indicator located within a
   polynomial taxonomic name. Allowed values are: “ssp.” (plants and
   fungi), “subsp.” (animals and monerans), “var.”, “subvar.”, “f.”,
   “subf.”, and “X”.
-  Unit name 3: The third portion of a scientifically accepted label for
   a polynomial occurrence of Taxonomic Units (e.g. subspecies name).
-  **Unit indicator 4**: Same as unit indicator3.
-  **Unit name 4**: The fourth part of a scientifically accepted label
   for a polynomial occurrence of Taxonomic Units.
-  **Usage**: Current standing of an occurrence of Taxonomic Units.
   Allowed values are: “accepted” (plants and fungi), “not accepted”
   (plants and fungi), “valid” (animals and monerans), and “invalid”
   (animals and monerans).
-  **Vernacular names**: Enter vernacular names for this taxon,
   comma-separated. This field is not part of the ITIS standard.
