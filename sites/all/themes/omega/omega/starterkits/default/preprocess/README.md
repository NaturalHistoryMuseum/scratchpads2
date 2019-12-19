# Defining preprocess hooks
Rather than placing your preprocess hooks directly in the template.php file you
can manage them in automatically discovered and lazy-loaded include files. It is
even possible to organize them in sub-folders. This feature greatly improves the
maintainability of large themes that would otherwise contain hundreds of lines
of unrelated code in your template.php file.

The include files have to follow a certain naming pattern (HOOK.preprocess.inc)
for them to be automatically discovered:

* THEMENAME_preprocess_html() = html.preprocess.inc
* THEMENAME_preprocess_page() = page.preprocess.inc
* THEMENAME_preprocess_node() = node.preprocess.inc
* THEMENAME_preprocess_comment() = comment.preprocess.inc
* THEMENAME_preprocess_region() = region.preprocess.inc

As with template files, you should replace underscores from the hook names with
hyphens:

* THEMENAME_preprocess_comment_wrapper() = comment-wrapper.preprocess.inc
* THEMENAME_preprocess_html_tag() = html-tag.preprocess.inc

Inside of each of these files you define the preprocess hook just as you would
otherwise do in your template.php file:

```
function THEMENAME_preprocess_HOOK(&$variables) {
  // Your code here.
}
```
