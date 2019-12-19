; ##############################################################################
;
; This is a Drush make file that will automatically download the front-end
; libraries used by Omega. Alternatively, you can use Bower (http://bower.io) to
; accomplish this.
;
; Running Drush make in your sub-theme will cause the libraries to be downloaded
; into your theme. If you want to download them into Omega directly to make them
; available to all of your sub-themes (if you have multiple) then you should
; instead run omega.make from the Omega theme directory.
;
; To run this file with 'drush make' you first have to navigate into your theme.
; Normally, this would be 'sites/all/themes/{{ THEME }}'.
;
; $ cd sites/all/themes/{{ THEME }}
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
