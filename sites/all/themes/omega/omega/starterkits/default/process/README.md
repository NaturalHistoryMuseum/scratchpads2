# Defining process hooks
Rather than placing your process hooks directly in the template.php file you can
manage them in automatically discovered and lazy-loaded include files. It is
even possible to organize them in sub-folders. This feature greatly improves the
maintainability of large themes that would otherwise contain hundreds of lines
of unrelated code in your template.php file.

The include files have to follow a certain naming pattern (HOOK.process.inc) for
them to be automatically discovered:

* THEMENAME_process_html() = html.process.inc
* THEMENAME_process_page() = page.process.inc
* THEMENAME_process_node() = node.process.inc
* THEMENAME_process_comment() = comment.process.inc
* THEMENAME_process_region() = region.process.inc

As with template files, you should replace underscores from the hook names with
hyphens:

* THEMENAME_process_comment_wrapper() = comment-wrapper.process.inc
* THEMENAME_process_html_tag() = html-tag.process.inc

Inside of each of these files you define the process hook just as you would
otherwise do in your template.php file:

```
function THEMENAME_process_HOOK(&$variables) {
  // Your code here.
}
```
