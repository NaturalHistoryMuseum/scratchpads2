
Styles are containers to group similar field display formatters. For example, a
style on a Filefield may contain several formatters based on the mimetype or
stream, while a style on a node reference might display various formatters
based on the referenced node type.

By itself, this module does nothing. Rather, it provides an API available for
use by other modules. The Media Styles module, bundled with the
<a href="/project/media">Media</a> module, is a fully featured module utilizing
the API.

Hooks provided:
<code>
hook_styles_default_containers()
hook_styles_default_styles()
hook_styles_default_presets()
hook_styles_default_containers_alter(&$style)
hook_styles_default_styles_alter(&$style)
hook_styles_default_presets_alter(&$style)
hook_styles_delete(&$style)
</code>

See http://groups.drupal.org/node/35206 for a quick background.
