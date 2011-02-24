Content Template (contemplate) Module for Drupal
by Jeff Robbins | Lullabot | www.lullabot.com
------------------------------------------------

What is the difference between ConTemplate and theming?

In Drupal, the presentational layer (the theme) is a walled-off and separate
entity. Since it is possible for users to choose their own themes or to switch
themes programmatically, Drupal does not make any assumptions about presentation
in the way that it "thinks" about content. This can become a problem when
modules like CCK are adding fields that you may want to show up in specific
ways. Drupal's RSS feeds and search indexing does not run through the theme
layer. This is as it should be because neither of these use presentational
information. However, there are certainly times when you want to rearrange this
content, remove parts of it, or display things in special ways. You can affect
most of the site display using your theme's node templates. But if you would
like to affect the content across multiple themes, multiple sites, or simply
affect the RSS feed and search indexing, you'll want to use ConTemplate.

ConTemplate alters the way that Drupal "thinks" of the content. It alters the
node teaser and/or body fields before the theme is invoked, so it will affect
RSS feed and search index in addition to the $content variable that gets sent to
your theme's node templates.


What does ConTemplate do?

This module allows modification of Drupal's teaser, body, and RSS feed using
administrator defined templates. These templates use PHP code in a manner very
similar to the PHPTemplate theme engine and all of the node object variables are
available for use in each template. An example node object is displayed and it
is as simple as clicking on its properties to add them to the current template.

This module was written to solve a need with the Content Construction Kit (CCK),
where it had a tendency toward outputting content in a not-very-pretty way. And
as such, it dovetails nicely with CCK, adding a "template" tab to CCK
content-type editing pages and pre-populating the templates with CCK's default
layout. This makes it easy to rearrange fields, output different fields for
teaser and body, remove the field title headers, output fields wrapped for use
with tabs.module (part of JSTools), or anything you need.


Disk-based templates

It is also possible to create disk-based template files. To do this, copy the
contents of a contemplate textarea and paste it into a file called
"node-{nodetype}-{field}.tpl.php" where {nodetype} is the content type and
{field} is either "body", "teaser", or "rss". It is also possible to create a
template called "node-{nodetype}.tpl.php" which will affect all cases, and
"node.tpl.php" which will affect all node types.

Place these files into a directory called "contemplates" inside of either your
"sites/all" directory or "sites/{yoursite}" depending on your setup. It is also
possible to have multiple "contemplate" directories with
"sites/all/contemplates" being the fallback for templates the contemplate does
not find in the more specific site directory.

When adding or removing template files, you will need to visit the Content
Templates admin page to refresh the template list cache. You do not need to do
this again when making changes to the content of the templates.