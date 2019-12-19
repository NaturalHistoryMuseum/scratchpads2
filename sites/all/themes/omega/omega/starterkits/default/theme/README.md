# Overriding theme function
Instead of defining theme function overrides in your template.php file Omega allows you to split them up into separate include files. These follow the same naming pattern as (pre-)process include files and are automatically lazy-loaded whenever the theme hook is invoked.

This feature greatly improves the maintainability of large themes that would otherwise contain hundreds of lines of unrelated code in your template.php file.

The include files have to follow a certain naming pattern (HOOK.theme.inc) for them to be automatically discovered:

* THEMENAME_breadcrumb() = breadcrumb.theme.inc
* THEMENAME_button() = button.theme.inc

As with template files, you should replace underscores from the hook names with hyphens:

* THEMENAME_status_messages() = status-messages.theme.inc
* THEMENAME_menu_link() = menu-link.theme.inc

Inside of each of these files you define the theme function override just as you would otherwise do in your template.php file:

```
function THEMENAME_HOOK(&$variables) {
  // Your code here.
}
```

Example:

```
function THEMENAME_menu_link(&$variables) {
  // Your code here.
}
```

You can also provide theme function include files for theme hook suggestions.
