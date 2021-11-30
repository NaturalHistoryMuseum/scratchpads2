********************************************************************
D R U P A L    M O D U L E
********************************************************************
Name: Legal Module
Author: Robert Castelo
Sponsor: Code Positive [www.codepositive.com]
Drupal: 7.0.x
********************************************************************
DESCRIPTION:

    A module which displays your Terms & Conditions to users who want to
    register, and makes sure they accept the T&C before their registration
    is accepted.

    Note: No T&C will be displayed until the T&C text has been input by
              the administrator.

    Each time a new version of the T&C is created all users will be required to
    accept the new version.

    Note: T&C text should only be entered by administrators or other highly trusted users.
              filter_xss_admin() is used to filter content for display, this is a very permissive
              XSS/HTML filter intended for admin-only use.

   Integrates with Views, and ships with 2 default views that display T&C history, and user
   acceptance of T&Cs.

   Tokens can be used in T&C text.


 ********************************************************************
INSTALLATION:

    Note: It is assumed that you have Drupal up and running.  Be sure to
    check the Drupal web site if you need assistance.  If you run into
    problems, you should always read the INSTALL.txt that comes with the
    Drupal package and read the online documentation.

      Dependencies: none

    Place the entire legal directory into your Drupal modules/directory.
    Enable the Legal module by navigating to: Administer > Modules > Other > Legal

    Click the 'Save configuration' button at the bottom to commit your changes.

********************************************************************
CONFIGURATION:

    There are two ways to set permissions:

        1. Go to Administer > Modules > Other > Legal, under the "Operations" column click the "Permissions" link
            Set which roles can "View Terms and Conditions".
            Set which roles can "Administer Terms and Conditions".
        2. Go to Administer > People > Permissions
            Set which roles can "View Terms and Conditions".
            Set which roles can "Administer Terms and Conditions".

        Click the 'Save permissions' button at the bottom to commit your changes.

    There are two ways to configure the Legal module:

        1. Go to Administer > Modules > Other > Legal, under the "Operations" column click the "Configure" link
            Input your terms & conditions text.
        2. Go to Administer > People > Legal
            Input your terms & conditions text.


    Set how you would like it displayed:

    - Scroll Box - Standard form text box (read only). Text is entered and displayed as text only.
    - Scroll Box (CSS) - Scrollable text box created in CSS.
        Text should be entered with HTML formatting. (less accessible than a standard scroll box)
    - HTML Text - Terms & conditions displayed as HTML formatted text.
        Text should be entered with HTML formatting.
    - Page Link

    Note: When displayed on the page at /legal, your T&Cs will be automatically reformatted to HTML Text
        if entered as a Scroll Box or Scroll Box (CSS).

    Click the 'Save' button at the bottom to commit your changes.


********************************************************************
ADDITIONAL CONFIGURATION

ADDITIONAL CHECKBOXES
Each field that contains text will be shown as a checkbox which the user must tick to register.
For example, if you enter "I am at least 18 years of age" in the text area, this will display as an additional checkbox,
which must be ticked in order to proceed.

EXPLAIN CHANGES
Explain what changes were made to the T&C since the last version.
This will only be shown to users who accepted a previous version (authenticated users).
Each line will automatically be shown as a bullet point.

FACEBOOK CONNECT

In facebook applications, click edit, click on Facebook User Settings.
Click on "Do not create accounts Automatically". Then, when user DOES
create account, it runs them through the Legal agreement.

VARNISH CACHE

Some users have reported that when a user who hasn't accepted the
latest version of the legal terms attempts to login, they are logged
out with the message 'Operation timed out. Please try to log in again.'

To fix this please add the following to your default.vcl file:

sub vcl_recv {
  if (req.url ~ "^/legal_accept") {
    return (pipe);
  }
}


********************************************************************
ACKNOWLEDGEMENTS

* Drupal 5 update sponsorship
Lullabot (http://www.lullabot.com)

* User data variables clean up
Steven Wittens (Steven)

* T&C Page formatting
Bryant Mairs (Susurrus)




