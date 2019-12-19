# Components
This directory should contain full components (modules in SMACSS), their
sub-components and modifiers.

Components are discrete parts of your page that should sit within the regions
of your layouts. You should try to abstract your components as much as possible
to promote reuse throughout the theme. Components should be flexible enough to
respond to any width and should never rely on context
(e.g. .sidebar-first .component) for styling. This allows modules to be placed
throughout the theme with no risk of them breaking.

If you find you need to change the look of a component depending on it's context
you should avoid using context based classes at all costs. Instead it is better
to add another "modifier" class to the component to alter the styling. Again,
this promotes reuse.

Sub-components are the individual parts that make up a component. As a general
rule, adding a class to target a sub-component is a much better option than
using descendant selectors or element selectors. In many cases sub-components
can be made more reusable by making them components in their own right, so they
can then be used within other components.

Almost everything that doesn't belong in base should be made a component.

## Some common examples throughout Drupal

* Blocks
* Content Types
    For example, you may have a generic node component that contains
    sub-components for the submitted by line and links. Specific components can
    then be created for each content type to style the specifics of each. Finally,
    by using entity view modes you can easily apply modifications based on the
    .node-[view-mode] classes.
* Forms
    For instance the log in form.
* Views
    Each views output style could be made into a component, the content of each
    row should be provided by a view mode styled by it's component CSS. Exposed
    filter forms, views pagers and other views elements are also good candidates
    for components.
