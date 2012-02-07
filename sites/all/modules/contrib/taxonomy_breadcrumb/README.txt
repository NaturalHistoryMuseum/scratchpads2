
Taxonomy Breadcrumb
-------------------

 The taxonomy_breadcrumb module generates taxonomy based breadcrumbs on node
 pages and taxonomy/term pages.  The breadcrumb trail takes on the form:
   [HOME] >> [VOCABULARY] >> TERM >> [TERM] ...

   - The HOME breadcrumb (if present) links to the homepage.  The text
     displayed for HOME is administrator configurable.  If the HOME
     breadcrumb is not defined by the administrator, it will not appear
     in the breadcrumb trail.
   - The VOCABULARY breadcrumb (if present) will link to an administrator
     defined page.  If the VOCABULARY does not have an administrator
     defined page, it will not appear in the breadcrumb trail.
   - Each TERM breadcrumb will link to either
     (1) taxonomy/term/tid by default, or
     (2) an administrator defined page if one is defined for the term.
   - These administrator defined "breadcrumb links" for VOCABULARIES and TERMS
     are controlled from the add/edit vocabulary and add/edit term
     administrator pages.

 Examples:
   home >> term >> term
   mysite >> term >> term
   home >> vocabulary >> term >> term
   vocabulary >> term >> term

* Enable the module and any option submodules (see below for details)
* Configure taxonomy breadcrumb settngs at admin/config/user-interface/taxonomy-breadcrumb
  - set the home breadcrumb text, if desired.
  - under the advanced settings fieldset, select the node types to either include or
    exclude when applying taxonomy-based breadcrumbs.
  - Edit your taxonomy vocabularies and set the taxonomy breadcrumb paths, if desired.
  - Edit your taxonomy terms and set the taxonomy breadcrumb paths, if desired.
* Visit node pages and taxonomy term pages and your taxonomy breadcrumbs should appear.
