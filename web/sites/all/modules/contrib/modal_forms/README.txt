Drupal modal_forms module:
------------------------
Maintainers:
  Fredrik Jonsson (http://drupal.org/user/5546)
Requires - Drupal 7
License - GPL (see LICENSE)


Overview:
--------
Modal forms make use of the modal feature in the ctools module to open
some common forms in a modal window.


Supported forms:
---------------
Modal forms can automatically rewrite links to the following forms and place them in a modal.

* Log in (modal_forms/nojs/login)
* Request new password (modal_forms/nojs/password)
* Create new account (modal_forms/nojs/register)
* Comment (modal_forms/nojs/modal_forms/nojs/comment/reply/[nid])
* Contact (modal_forms/nojs/contact)


Webforms:
--------
There is support to open webforms in a modal by constructing special links.

A webform link should look like this one:

<a class="ctools-use-modal ctools-modal-modal-popup-large" href="/modal_forms/nojs/webform/[nid]">Link to click</a>

Replace [nid] with the node id of your webform.

Second class is optional, you can use one of this:

ctools-modal-modal-popup-small (300x300);
ctools-modal-modal-popup-medium (550x450);
ctools-modal-modal-popup-large (80%x80%).


Installation:
------------
1. Download and unpack the Modal forms module directory in your modules folder
   (this will usually be "sites/all/modules/").
2. Go to "Administer" -> "Modules" and enable the module.


Configuration:
-------------
Go to "Configuration" -> "Development" -> "Modal forms" to find
all the configuration options.
