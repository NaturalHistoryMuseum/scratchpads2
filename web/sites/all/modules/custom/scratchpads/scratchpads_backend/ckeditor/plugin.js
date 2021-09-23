(function($) {
  /**
   * Add our plugin to CKEDitor plugins, so that styles are transformed to attributes
   * when modified.
   *
   */
  CKEDITOR.plugins.add("scratchpads_backend_tweaks", {
    init : function(editor) {
      if (editor.dataProcessor && editor.dataProcessor.htmlFilter) {
        editor.dataProcessor.htmlFilter.addRules({
          elements: {
            img: function(element) {
              if (element.attributes.style) {
                scratchpads_backend_style_to_attr(element, 'width', /(?:^|\s)width\s*:\s*(\d+)px;?/i);
                scratchpads_backend_style_to_attr(element, 'height', /(?:^|\s)height\s*:\s*(\d+)px;?/i);
                scratchpads_backend_style_to_attr(element, 'align', /(?:^|\s)float\s*:\s*(left|right);?/i);
                scratchpads_backend_style_to_attr(element, 'border', /(?:^|\s)border-width\s*:\s*(\d+)px;?/i);
                scratchpads_backend_style_to_attr(element, ['vspace', 'hspace'], /(?:^|\s)margin\s*:\s*(\d+)px(?:\s+(\d+)px)?;?/i);
                if (typeof element.attributes.hspace == 'undefined' && typeof element.attributes.vspace != 'undefined') {
                  element.attributes.hspace = element.attributes.vspace;
                }
                element.attributes.style = element.attributes.style.replace(/(?:^|\s)border-style\s*:\s*(solid);?/i, '');
                element.attributes.style = $.trim(element.attributes.style);
                if (element.attributes.style == '') {
                  delete element.attributes.style;
                }
              }
              return element;
            }
          }
        });
      }
    }
  });

  /**
   * Alter the image plugin to not allow for border/vspace/hspace settings
   */
  CKEDITOR.on('dialogDefinition', function(context) {
    var dialogName = context.data.name;
    var dialogDefinition = context.data.definition;

    if (context.data.name == 'image') {
      var infoTab = context.data.definition.getContents('info');
      infoTab.remove('txtHSpace');
      infoTab.remove('txtVSpace');
      infoTab.remove('txtBorder');
    }
  });

  /**
   * Helper function to replace styles with attributes
   */
  function scratchpads_backend_style_to_attr(elem, attr, reg) {
    if (typeof attr != 'object') {
      attr = [attr];
    }
    var match = reg.exec(elem.attributes.style);
    if (match) {
      for(var i=0; i < attr.length; i++) {
        if (match[i+1]) {
          elem.attributes[attr[i]] = match[i+1];
        }
      }
      elem.attributes.style = elem.attributes.style.replace(reg, '');
    }
  }
})(jQuery);