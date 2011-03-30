; $Id: README.txt,v 1.1.2.2.2.1 2011/01/16 23:52:41 karst Exp $
Module Terms Of Use
Author:
chill35 from http://drupal.org
Caroline Schnapp at <mllegeorgesand@gmail.com>

Co-Maintainer:
Kars-T from http://www.comm-press.de
Karsten Frohwein <karsten.frohwein@comm-press.net>

============
TERMS OF USE MODULE
============
This module adds a Terms of Use text from a node and a [x] I agree check box to
the registration page.

============
INSTALLATION
============

1. Move or copy or extract the 'terms_of_use' folder to sites/all/modules.

2. Enable the module 'Terms of Use' on the page admin/build/modules.

3. Create a Terms of Use page at node/add/page. Do not promote the node.

4. Go to admin/config/people/terms_of_use and type the title of your Terms node
in the autocomplete text field "Title of the post where your Terms of Use are
published".

5. Save your module configuration.

6. Clear your Drupal cache at admin/settings/performance by clicking
'Clear cached data'.

7. Log out and access the registeration page at user/register.

It will now be required for anyone wishing to sign up to check the 'I agree with
these terms.' checkbox.

============
FAQS
============

QUESTION:
I have very long text for term of use of my site, so I don't like to
show all this text in the registration page. Is possible to have this solution:
Put only a link to the term of use page in the registration page containing the
checkbox, like this:

    Terms of use of this page are available here (<- this a link to the terms
    of use page)
    [x] I certify that I read and I agree with these terms

ANSWER:
Certainly. You can enter the token "@link" into the checkbox lable field
at admin/settings/terms_of_use. By this no text will be shown. Or you can edit
the output of terms of use by overriding theme_terms_of_use().


QUESTION:
I installed the module, but I am wondering, how to ask already
registered users to check again "Tem of Use" Box, after an update of the Terms
of Use Node ?

ANSWER:
This will be added to the terms of use module later.

You can also add this text to your Terms:

"We reserve the right to update or revise these Terms of Use at any time without
notice. Please check the Terms of Use periodically for changes. The revised
terms will be effective immediately as soon as they are posted on the WebSite
and by continuing to use the Site you agree to be bound by the revised terms."

If you do so, don't forget to include a link to the Terms node in the footer
area of your site.

QUESTION:
It doesn't look nice when the terms page is long. Why not use a popup
instead? or make the Terms scrollable, or collapsible?

ANSWER:
This will be added to the module in the future. Currently you can override the
terms_of_use.css file with your own version. To use some Javascript you override
CSS and theme_terms_of_use() to achieve this.