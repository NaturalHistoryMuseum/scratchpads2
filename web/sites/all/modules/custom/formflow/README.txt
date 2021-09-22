
  ------------------------------------------------------------------------------------
                                ABOUT FORM FLOW
  ------------------------------------------------------------------------------------

Form flow provides a simple way for users to chain forms together into multi-step forms. It is built on top of ctool's form wizard functionality.

It comprises 2 main modules:

FORMFLOW - Provides the main funtionality of the module.

FORMFLOW UI - An admin interface for building flows using existing forms on the site.


  ------------------------------------------------------------------------------------
                                      USAGE
  ------------------------------------------------------------------------------------

HOOKS

This module calls hook_default_flows() to retrieve definitions of formflows, which contain:

* Title
* Name
* Description
* Path
* Menu (menu settings)
* Show trail (if a breadcrumb trail should be shown at the tob of the form flow)
* Show back (Show back button)
* Show cancel (Show cancel button)
* Steps (An array of form ids, titles & paths, defining each of the forms in the flow) 

See formflow_example module for an example.


FIELD GROUPS

If the field groups module (http://drupal.org/project/field_group) is available, a new field group "formflow" will be created. 
This allows you to break up fieldable entities across multiple steps, by creating formflow groups for each step.


UI

If you enable the formflow UI module, you can create formflows via the admin interface. 
You can customise the form flow in the settings (show trail options etc.,) & select forms from your site.
The UI is avaiable at admin/structure/formflow


  ------------------------------------------------------------------------------------
                                  INSTALLATION
  ------------------------------------------------------------------------------------
  
Install as usual - no installation requirements.
  

  ------------------------------------------------------------------------------------
                                 RECOMMENDATIONS
  ------------------------------------------------------------------------------------
 
 * Field group module
 
 
  ------------------------------------------------------------------------------------
                                  REQUIREMENTS
  ------------------------------------------------------------------------------------
  
The following modules are required:

  * ctools

The formflow_example module requires the ctools_ajax_sample module.


  

