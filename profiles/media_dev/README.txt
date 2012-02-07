Media Development profile
------------------------------

== What does it do ==

Gets your D7 install configured to insert images into a text area via ckeditor.

== What do you need to do ==

Get the modules

git clone --branch 7.x-1.x http://git.drupal.org:project/media.git
git clone --branch 7.x-1.x http://git.drupal.org:project/media_youtube.git
git clone --branch 7.x-1.x http://git.drupal.org:project/plupload.git
git clone --branch 7.x-1.x http://git.drupal.org:project/wysiwyg.git

Get Ckeditor and install it in sites/all/libraries

Here's a little script to help you:

cd sites/all
mkdir libraries
cd libraries
wget http://download.cksource.com/CKEditor/CKEditor/CKEditor%203.0.1/ckeditor_3.0.1.tar.gz
tar -zxvf ckeditor_3.0.1.tar.gz
