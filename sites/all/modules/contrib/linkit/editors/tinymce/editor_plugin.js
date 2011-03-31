
(function ($) {
	tinymce.create('tinymce.plugins.linkit', {
		init : function(ed, url) {
			// Register commands
			ed.addCommand('mceLinkit', function() {
        ed.windowManager.open({
					file : Drupal.settings.linkit.url.wysiwyg_tinymce,
					width : 580,
					height : 480,
					inline : true,
          scrollbars : 1,
          popup_css : false
				}, {
					plugin_url : url
				});
			});

			// Register buttons
			ed.addButton('linkit', {
				title : 'Linkit',
				cmd : 'mceLinkit',
        image : url + '/images/linkit.png'
			});

      ed.onNodeChange.add(function(ed, cm, n, co) {
				var p;
        p = tinymce.DOM.getParent(n, 'A');
        if (!p || !p.name) {
          cm.setActive('linkit', !!p);
        }
			});
		},

		getInfo : function() {
			return {
				longname : 'Linkit',
				author : 'Emil Stjerneman',
				authorurl : 'http://www.stjerneman.com',
				infourl : 'http://drupal.org/project/linkit',
				version : tinymce.majorVersion + "." + tinymce.minorVersion
			};
		}
	});

	// Register plugin
	tinymce.PluginManager.add('linkit', tinymce.plugins.linkit);
})(jQuery);