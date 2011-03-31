
##############################################
## ONLY if you use ckeditor WITHOUT wysiwyg ##
##############################################

Installation:

Do the following steps to add Linkit button to the CKEditor toolbar:

   1. Open /drupal/modules/ckeditor/ckeditor.config.js

   2. Scroll down to the end of the file, right before "};" insert:
      
       // Linkit plugin.
      config.extraPlugins += (config.extraPlugins ? ',Linkit' : 'Linkit' );
      CKEDITOR.plugins.addExternal('Linkit', Drupal.settings.basePath + Drupal.settings.linkit.modulepath + '/editors/ckeditor/');

   3. Add button to the toolbar. 
      
      3.1 Go to Configuration -> CKEditor (admin/config/content/ckeditor)
          Click "Edit" on the profile you what to use with Linkit.

      3.2 Expand "Editor appearance" and go to "Toolbar".
      
          The button name is: Linkit
          For example if you have a toolbar with an array of buttons defined as follows:

          ['Link','Unlink','Anchor']

          simply add the button somewhere in the array:

          ['Linkit','Link','Unlink','Anchor']

          (remember about single quotes).