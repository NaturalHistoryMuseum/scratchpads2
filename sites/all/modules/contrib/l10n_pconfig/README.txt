
Plural formula configurator
--------------------------------------------------------------------------------
Project page: http://drupal.org/project/l10n_pconfig
Issue queue:  http://drupal.org/project/issues/l10n_pconfig

ABOUT
--------------------------------------------------------------------------------

The plural formula configurator sets sensible defaults for plural forms when
adding languages and lets you edit the plural formula for all languages on the
web interface.

Drupal does not expose these fields for editing due to the complexity of plural
forms. You should make sure to only give permissions to edit language details
to those, who will likely not screw up your plural formulas.

INSTALLATION
--------------------------------------------------------------------------------

1. Enable l10n_pconfig at Administer > Site building > Modules.

2. The "administer languages" permission applies to functionality of this
   module. You probably already has this granted to all parties involved.
   
3. Configure plural formulas on Administer > Site configuration > Languages.
   Plural formulas are set when known for predefined languages. You can set
   custom plural formulas when adding a custom language or editing an existing
   one. 

CONTRIBUTORS
--------------------------------------------------------------------------------

Gábor Hojtsy http://drupal.org/user/4166 (current maintainer)
