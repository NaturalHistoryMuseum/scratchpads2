Import taxon descriptions
=========================

Excel file imports can be used either for creating new nodes or for updating existing. Please see the :doc:`/import/content` page for more general info.

To download the template file (for new data or for updating your data)

1. Go to Import > Nodes > Excel file import
2. Choose taxon description from the second drop-down menu
3. Choose the template file you wish to work on (choose the first for
   creating new data or the second for changing or amending existing
   scratchpad data)
4. Open the downloaded excel file in your computer and edit it

.. only:: training

    5. An example taxon import file *TEMPLATE-import_into_taxon_description.xls* is included in the :ref:`training-material`. 




**Complete the columns as follows:**

-  **GUID**:

Use Global Unique Identifiers for each one of your taxon descriptions.
These identifiers will not be visible to users. Use complex combinations
like TH-SIB-2012-001

-  **Taxonomic name (Name)**:

Use the taxonomic name from your taxonomies the description refers to.
The permitted values of this filed are located in column B of the
PermittedValues tab of your downloaded Excel file. This is a required
field.

-  **Taxonomic name (TID)**:

Instead of typing the taxonomic name the corresponding Term ID (TID) can
be used to identify a term in one of the taxonomies. TIDs are visible
under the taxon name in the edit view of taxonomies.

-  **Taxonomic name (GUID)**:

Instead of typing the taxonomic name the corresponding GUID can be used
to identify a term in one of the taxonomies. Taxonomic names’ GUIDs are
visible under the taxon name in the edit view of taxonomies.

-  **Map**:

You can set the geographic distribution of a taxon using specific values
in this field.

1. To set distribution as predefined map polygon:

   1. Use the LAT/LONG set of coordinates of each of the points that
      constitute your polygon in WGS84. Separate LAT and LONG with a
      space and each set with a comma.
   2. Put your coordinates between the following strings
      *POLYGON:POLYGON((* set of coordinates *))*

For example to set a polygon distribution defined by the following
points *27.42 44.65,17.66 39.36,20.30 36.80,24.25 34.45,27.33
36.31,29.79 37.85,27.42 44.65* you should put in the cell the following
string *POLYGON:POLYGON((27.42 44.65,17.66 39.36,20.30 36.80,24.25
34.45,27.33 36.31,29.79 37.85,27.42 44.65))*

1. To set distribution as geographical or administrative units:

   1. Use the *REGION:* prefix followed by the number of geographical
      unit according to `TDWG controlled vocabulary on geographic
      regions`_

   2. To select a Level 1 region use the *REGION:* prefix followed by
      the number of continent (e.g. REGION:2)

   3. To select a Level 2 region use the *REGION:* prefix followed by
      the number of continent and number of sub-continent
      (e.g. REGION:1:13)

   4. To select a Level 3 region use the *REGION:* prefix followed by
      the three letter alphabetical code of the country or state
      (e.g. for Spain REGION:SPA)

   5. To select a Level 4 region use the *REGION:* prefix followed by
      the hyphenated

   6. To set multiple regions of distribution for a taxon you must separate each *REGION:…* with a line break in the cell. To enter line breaks use Alt+Enter.

-  **Associations**:

Descriptions and lists of taxa that interact with the subject taxon.
Includes explicit reference to the kind of ecological interaction:
Predator/prey; host/parasite, pollinators, symbiosis, mutualism,
commensalism; hybridisation etc.

-  **Behaviour**:

Description of behaviour and behaviour patterns of an organism,
including actions and reactions of organism in relation to its biotic
and abiotic environment. Includes communication, perception, modes and
mechanisms of locomotion, as well as long term strategies (except mating
and reproductive strategies, covered under reproduction).

-  **Biology**:

. An account of the biology of the taxon. E.g. behavior, reproduction,
dispersal

-  **Conservation status**:

A description of the likelihood of the species becoming extinct in the
present day or in the near future. Population size is treated under
Population Biology, and trends in population sizes are treated under
Trends. However, this is the preferred element if an object includes all
of these things and details about conservation listings.

-  **Cyclicity**:

Description of biorhythms, whether on the scale of seconds, hours, days,
or seasons. Those states or conditions characterised by regular
repetition in time. Could also cover phenomena such as chewing rates.
Life cycles are treated in the Life Cycle term. Seasonal migration and
reproduction are usually treated separately.

-  **Cytology**:

Cell biology: formation, structure, organelles, and function of cells.

-  **Diagnostic description**:

Lists the characters that distinguish this taxon from its closest
relatives.

-  **Diseases**:

Description of diseases that the organism is subject to. Disease-causing
organisms can also be listed under associations.

-  **Dispersal**:

Description of the methods, circumstances, and timing of dispersal.

-  **Distribution**:

Covers ranges, e.g., a global range, or a narrower one; may be
biogeographical, political or other (e.g., managed areas like
conservencies); endemism; native or exotic; ref Darwin Core Geospatial
extension. Does not include altitudinal distribution.

-  **Ecology**:

An overview of ecological aspects of the taxon.

-  **Evolution**:

Description of the evolution of the taxon.

-  **General description**:

A comprehensive description of the characteristics of the taxon. To be
used primarily when many of the subject categories are treated together
in one object, but at length. Taxon biology is to be used if a brief
summary.

-  **Genetics**:

Information on the genetics of the taxon, including karyotypes,
barcoding status, whole genome sequencing status, ploidy.

-  **Growth**:

Description of growth rates, allometries, parameters known to be
predictive, morphometrics. Can also include hypotheses of paedomorphy or
neoteny, etc.      

-  **Habitat**:

Includes realm (e.g Terrestrial etc) and climatic information (e.g
Boreal); also includes requirements and tolerances; horizontal and
vertical (altitudinal) distribution.

-  **Legislation**:

Legal regulations or statutes relating to the taxon.

-  **Life cycle**:

Defines and describes obligatory developmental transformations. Includes
metamorphosis, instars, gametophyte/embryophytes, transitions from
sessile to mobile forms. Discusses timing. Morphology usually described
in morphological descriptions.

-  **Life expectancy**:

Any information on longevity, including The average period an organism
can be expected to survive.

-  **Look alikes**:

Other taxa that this taxon may be confused with. Useful for
identification and comparison. Common in invasive species communities.

-  **Management**:

Describes techniques and goals used in management of species. May
include management relative to a piece of legislation, e.g., a CITES
list.

-  **Migration**:

Description of the periodic movement of organisms from one locality to
another (e.g., for breeding). Usually includes locality, timing, and
hypothesised purpose.

-  **Molecular biology**:

Includes proteomic and biochemistry (e.g Toxicity). Genomic information
is usually treated under genetics.

-  **Morphology**:

Description of the appearance of the taxon; e.g body plan, shape and
colour of external features, typical postures. May be referred to as or
include habit, or anatomy.

-  **Phylogeny**:

Description of phylogenetic and systematic treatments of the taxon.

-  **Physiology**:

Description of physiological processes. Includes metabolic rates, and
systems such as circulation, respiration, excretion, immunity,
neurophysiology.

-  **Population biology**:

Includes abundance information (population size, density) and
demographics (e.g. age stratification).

-  **Procedures**:

Deals with how you go about managing this taxon; what are the known
threats to this taxon?

-  **Reproduction**:

Description of reproductive physiology and behavior, including mating
and life history variables. Includes cues, strategies, restraints,
rates.

-  **Risk statement**:

Negative impacts on humans, communities.

-  **Size**:

Average size, max, range; type of size (perimeter, length, volume,
weight …).

-  **Taxon biology**:

Summary or overview of all aspects of an organism’s biology.

-  **Threats**:

The threats to which this taxon is subject.

-  **Trends**:

An indication of whether a population is stable, or increasing or
decreasing.

-  **Trophic strategy**:

Summaries general nature of feeding interactions. For example, basic
mode of nutrient uptake (autotrophy, heterotrophy, coprophagy,
saprophagy), position in food network (top predator, primary producer,
consumer), diet categorization (detritovore, omnivore, carnivore,
herbivore). Specific lists of taxa are treated under associations
(specifying predators or prey).

-  **Uses**:

Benefits for humans (e.g. in the field of Economic Botany). Can include ecosystem services. However, benefits to ecosystems not specific to humans are best treated under Risk statement.

.. _TDWG controlled vocabulary on geographic regions: http://rs.tdwg.org/ontology/voc/GeographicRegion


