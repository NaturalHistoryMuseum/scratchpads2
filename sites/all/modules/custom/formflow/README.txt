  ------------------------------------------------------------------------------------
                         ABOUT FORM FLOW
  ------------------------------------------------------------------------------------

Form flow provides a simple way for users to chain forms together into multi-step forms. It is built on top of ctool's form wizard functionality.

It comprises 2 main modules:

formflow - provides the main funtionality of the module.

formflow ui - an interface for building flows using existing forms on the site - these will be stored in the DB. Alternatively, flows can be defined as ctools plugins. 
The formflow_example module has a demonstration flow plugin.

 
  ------------------------------------------------------------------------------------
                         REQUIREMENTS
  ------------------------------------------------------------------------------------
  
The following modules are required:

  * ctools

The formflow_example module requires the ctools_ajax_sample module.

There is a bug - requires patching.

  

