; ----------------
; Makefile for fbconnect
; ----------------
;
;
; ------------
; Core version
; ------------

core = 7.x

; ------------
; API version
; ------------

api = 2

; ------------
; Core project
; ------------

projects[drupal][version] = 7

projects[libraries][version] = 2.0-alpha2
projects[libraries][type] = module
projects[libraries][subdir] = contrib

; ------------
; Libraries
; ------------

libraries[facebook-php-sdk][download][type] = "get"
libraries[facebook-php-sdk][download][url] = "http://github.com/facebook/php-sdk/tarball/v3.1.1"
libraries[facebook-php-sdk][destination] = "libraries"

