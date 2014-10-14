 -------------------------------------------------------------------------
 |||||||||||||||||||||||||||||| RULES FORMS ||||||||||||||||||||||||||||||
 -------------------------------------------------------------------------

 maintained by Jordan Halterman <jordan.halterman@gmail.com>
 http://drupal.org/project/rules_forms

 Thanks to klausi and fago for original development and maintainance of
 the earlier version of Rules Forms, which was included with Rules 1.

 Installation
 ------------
 This module requires Rules 2 (http://drupal.org/project/rules).
 Navigate to administer >> modules and enable the Rules Forms module.


 Getting started
 ---------------
 This is a short usage guide to build Rules on you forms:

 * Go to the "Form events" page in the Rules administration menu
   (admin/config/workflow/rules/forms).
 * Select the checkbox "Enable event activation messages on forms" and hit the "Save
   settings" button.
 * Go to the form on your site that you would like to customize with Rules, e.g.
   go to 'node/add/article' to enable events on the "Create Article" form.
 * On the top of the page you see a drupal message with a link to activate events
   for the form, click it.
 * Enter a human-readable label for identifying the form.
 * In some cases you may want to enable events for individual buttons on the form.
   To do so, check the checkbox labeled "Enable button level validate/submit events".
 * Confirm the activation by clicking the "Activate" button.
 * Go to the "Triggered rules" admin page (admin/config/workflow/rules) and click
   the "Add a new rule" link.
 * Fill out the label, choose a form event by selecting one in the "Rules Forms"
   group and confirm with "Save changes".
 * Now you can add conditions and actions to react on the form event.


 Activating button level validate and submit events
 --------------------------------------------------

 In some cases, form level validate and submit events may not appropriately
 trigger your rules because forms may implement button level validate or submit
 handlers or you may just want to target a specific form button for validate
 or submit events. To activate button level validate and submit events:

 * When activating events for a form, select the option labeled "Enable button
   level validate/submit events".
 * When creating new rules, you will find events for individual form buttons.
 * Note: In Rules 2.x you can create rules that react on multiple events. Rules
   forms rules can be invoked on form and button level validate/submit events.


 Creating form element conditions and actions
 --------------------------------------------

 The Rules forms module allows you to manipulate single form elements, where you
 need the ID of the element. This guide shows you you how to find them.

 * Create a new reaction rule using the Rules interface.
 * Select the desired event for the target form.
 * On the following screen, you will be displayed a select list of form elements.
   Select the target form element from the select list and click "Continue".
 * Note: Form elements are categorized by type to make them more easily identifiable.
 * In the event that the form element select list is not populated, try rebuilding
   form element data as described below.

 #1429622: *The Rules data selector cannot currently be used to select form elements*


 Using the form element inspection tool
 --------------------------------------

 Rules forms support provides a tool for inspecting form elements on active forms.
 This tool can be used to determine the current attributes of form elements and to
 inspect elements following the execution of rules.

 * Navigate to the "Form events" page in the Rules administration menu
   (admin/config/workflow/rules/forms)
 * Select the checkbox labeled "Enable form element inspection tool".
 * Click the button label "Save settings".
 * To use the inspection tool, navigate to a form whose events are activated.
 * Mouse over the question mark (?) near any form element to display a list of that
   element's current attributes and their values.


 Rebuilding form element data
 ----------------------------

 In the event that form elements are not being displayed within the element select list
 in the rules interface, you can rebuild Rules forms' internal storage of form element
 data.

 * Navigate to the "Form events" page in the Rules administration menu
   (admin/config/workflow/rules/forms)
 * Expand the fieldset labeled "Form elements".
 * Select the form whose elements you want to rebuild from the select list.
 * Click the "Rebuild" button.
 * Finally, navigate to the form whose elements need to be rebuilt. When the form is
   displayed for the first time, Rules forms will evaluate it and display a message
   notifying the user that its internal memory has been updated.


 Deactivate a form
 -----------------

 To deactivate form events for a form:

 * Navigate to the "Form events" page in the Rules administration menu
   (admin/config/workflow/rules/forms)
 * Expand the fieldset labeled "Active forms".
 * Select the forms you would like to disable from the checkboxes.
 * Click the button labeled "Deactivate events".
 * Note: deactivating form events will break existing rules that implement events
   provided by those forms.

