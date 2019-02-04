; ----------------
; Generated makefile from http://drushmake.me
; Permanent URL: http://drushmake.me/file.php?token=109c2ce30afa
; ----------------
;
; This is a working makefile - try it! Any line starting with a `;` is a comment.

; Core version
; ------------
; Each makefile should begin by declaring the core version of Drupal that all
; projects should be compatible with.

core = 7.x

; API version
; ------------
; Every makefile needs to declare its Drush Make API version. This version of
; drush make uses API version `2`.

api = 2

; Libraries
; ---------
libraries[jquery.cycle][type] = "libraries"
libraries[jquery.cycle][download][type] = "file"
libraries[jquery.cycle][download][url] = "https://raw.githubusercontent.com/malsup/cycle/3.0.3-a/jquery.cycle.all.js"
libraries[json2][type] = "libraries"
libraries[json2][download][type] = "file"
libraries[json2][download][url] = "https://raw.githubusercontent.com/douglascrockford/JSON-js/107fc93c94aa3a9c7b48548631593ecf3aac60d2/json2.js"
