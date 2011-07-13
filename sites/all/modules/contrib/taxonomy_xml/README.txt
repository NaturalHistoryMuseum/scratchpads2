This module makes it possible to import and export vocabularies and
taxonomy terms via XML. It requires taxonomy.module.

== Exporting vocabularies ==

Once installed and enabled, this module provides a list of downloadable
XML documents for each vocabulary at admin/content/taxonomy/export.

== Importing a vocabulary ==

To import a vocabulary, use admin/content/taxonomy/import.

The file "sample.xml" is a sample XML-formatted vocabulary that can
be used for testing purposes. It contains a vocabulary named "Editorial
sections" that contains four terms: "Analysis," "Feature," "News" and
"Opinion." Using the options available from admin/taxonomy/import, you
can either add those terms to an existing vocabulary, or create a new
"Editorial sections" vocabulary on your site.

Many more examples of different acceptable formats are provided in the
'samples' directory.
Still more are available directly from 'web services' selectable through the
import form.

== About vocabulary syntaxes ==

Please see the contents of the 'help' directory (formats.html) for a complete
explanation on which formats are supported and how to import your own from
other sources.

== Requirements ==

The RDF format functions and recursive remote hierarchy retrieval
requires the download and install of the ARC parser and more.
See INSTALL.txt