********************************************************************
D R U P A L    M O D U L E
********************************************************************
Name: Legal Module 
Author: Robert Castelo
Sponsor: Code Positive [www.codepositive.com]
Drupal: 6.0.x
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

********************************************************************
INSTALLATION:

    Note: It is assumed that you have Drupal up and running.  Be sure to
    check the Drupal web site if you need assistance.  If you run into
    problems, you should always read the INSTALL.txt that comes with the
    Drupal package and read the online documentation.

      Dependencies: checkbox_validate Module  

	1. Place the entire legal directory into your Drupal
        modules/directory.

	2. Enable the legal module by navigating to:

	   Administer > Site building > Modules

	Click the 'Save configuration' button at the bottom to commit your
    changes.
    


********************************************************************
CONFIGURATION

	1. Go to Administer > User management > Access control
	    
	    Set which roles can "view Terms and Conditions"
	    Set which roles can "administer Terms and Conditions"
	
	2. Go to Administer > Site configuration > Legal

	   Input your terms & conditions text, set how you would like it
     displayed:

	- Scroll Box - Standard form text box (read only) Text is entered
    and displayed as text only

	- Scroll Box (CSS) - Scrollable text box created in CSS Text should
    be entered with HTML formatting. 
       (less accessible than a standard scroll box)

	- HTML Text - Terms & conditions displayed as HTML formatted text
       Text should be entered with HTML formatting

	Note: When displayed on the page /legal your T&Cs will be automatically 
                reformatted to HTML Text if entered as a Scroll Box or Scroll Box (CSS)

FACEBOOK CONNECT

In facebook applications, click edit, click on Facebook User Settings. Click on "Do not create accounts Automatically". Then, when user DOES create account, it runs them through the Legal agreement.      
       
       
********************************************************************
ACKNOWLEDGEMENTS

* Drupal 5 update sponsorship
Lullabot (http://www.lullabot.com)

* User data variables clean up
Steven Wittens (Steven)

* T&C Page formatting
Bryant Mairs (Susurrus) 




