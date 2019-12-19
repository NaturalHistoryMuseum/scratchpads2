; ##############################################################################
;
; This is a Drush make file that will automatically download the front-end
; libraries used by Omega. Alternatively, you can use Bower (http://bower.io) to
; accomplish this.
;
; To run this file with 'drush make' you first have to navigate into the Omega
; theme directory.
;
; $ cd sites/all/themes/omega/omega
;
; Now you can invoke 'drush make' using the following command:
;
; $ drush make libraries.make --no-core --contrib-destination=.
;
; ##############################################################################

core = 7.x
api = 2

libraries[selectivizr][download][type] = "file"
libraries[selectivizr][download][url] = "https://github.com/fubhy/selectivizr/archive/master.zip"

libraries[html5shiv][download][type] = "file"
libraries[html5shiv][download][url] = "https://github.com/fubhy/html5shiv/archive/master.zip"

libraries[respond][download][type] = "file"
libraries[respond][download][url] = "https://github.com/fubhy/respond/archive/master.zip"

libraries[matchmedia][download][type] = "file"
libraries[matchmedia][download][url] = "https://github.com/fubhy/matchmedia/archive/master.zip"

libraries[pie][download][type] = "file"
libraries[pie][download][url] = "https://github.com/fubhy/pie/archive/master.zip"
