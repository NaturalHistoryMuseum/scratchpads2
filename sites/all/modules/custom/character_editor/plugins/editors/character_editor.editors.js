(function($) {

    var SlickEditor = {
        
        /**
         *  ControlledCharacter
         */
        ControlledCharacter: function(editor){
          /**
           * init
           */
          this.init = function(){
            // We can have multiple values if this is a taxon, or if this is an 'and' character
            this.multiple = editor.column.data.type == 'AND' || editor.item.id.match(/^taxonomy_term:\d+$/);
            // Create the popup body
            this.$input = $('<div></div>')
            .addClass('character-editor-popup')
            .attr('value', '')
            .css({
              position: 'absolute',
              zIndex: '100',
              top: $(editor.container).offset().top,
              left: $(editor.container).offset().left
            });
            // Set up the text input
            this.$textinputwrapper = $('<div></div>')
            .addClass('character-editor-popup-header')
            .css('position', 'relative')
            .appendTo(this.$input);
            this.$textinput = $('<input type="text" />')
            .css({
              position: 'absolute',
              zIndex: '11',
              background: 'rgba(0,0,0,0)'
            })
            .on('input', $.proxy(this, 'textInputChange'))
            .appendTo(this.$textinputwrapper);
            this.$textinputshadow = $('<input type="text" />')
            .attr('disabled', 'disabled')
            .css({
            })
            .appendTo(this.$textinputwrapper);
            this.$textinput.on('keyup', $.proxy(this, 'textInputKeyUp'));
            // Add the selectable rows
            for (var i in editor.column.data.options){
              var $row = $('<div></div>')
              .addClass('character-editor-popup-row')
              .html(editor.column.data.options[i])
              .attr('value', i);
              $row.appendTo(this.$input);
              $row.click((function(context, val){
                return $.proxy(context, 'rowClick', i)
              })(this, i));
            }
            if (this.multiple){
              // Add a 'done' button
              $('<div></div>')
              .addClass('character-editor-popup-button')
              .html('done')
              .click($.proxy(this, 'commit'))
              .appendTo(this.$input);
            }
            this.$input.appendTo('body');
            // Click-out overlay
            this.$overlay = $('<div></div>').css({
              position: 'absolute',
              top: '0',
              left: '0',
              width: $(document).width().toString() + "px",
              height: $(document).height().toString() + "px",
              zIndex: '99'
            }).appendTo('body').one('click', $.proxy(this, 'commit'));
            this.focus();
          }

          /**
           * rowClick
           */
          this.rowClick = function(val){
            this.toggleValue(val);
            if (!this.multiple){
              this.commit();
            } else {
              // Return focus to the text input
              this.focus();
            }
          }

          /**
           * commit
           */
          this.commit = function(){
            if (!this.isValueChanged()){
              editor.cancelChanges();
            } else {
              editor.commitChanges();
            }
          }

          /**
           * textInputChange
           */
          this.textInputChange = function(){
            // Remove any arrow selected item
            this.current = null;
            $('div.character-editor-popup-row', this.$input)
            .removeClass('character-editor-popup-row-highlight')
            .css('display', 'block');
            if (this.$textinput.val().length > 0){
              var textInputAuto = '';
              var textInputVal = '';
              var start = this.$textinput.val();
              $('div.character-editor-popup-row', this.$input).filter(function(){
                if ($(this).html().indexOf(start) != 0){
                  return true;
                } else if (textInputAuto.length == 0) {
                  textInputAuto = $(this).html();
                  textInputVal = $(this).attr('value');
                }
              }).css('display', 'none');
            }
            this.textInputVal = textInputVal;
            this.$textinputshadow.val(textInputAuto);
          }

          /**
           * textInputSubmit
           */
          this.textInputSubmit = function(e){
            var value = null;
            if (typeof this.current !== 'undefined' && this.current !== null){
              value = $('div.character-editor-popup-row-highlight', this.$input).attr('value');
            } else if (this.textInputVal && this.textInputVal.length > 0){
              value = this.textInputVal;
            }
            if (typeof value !== 'undefined' && value !== null){
              this.toggleValue(value);
              if (typeof e.nocommit == 'undefined' || !e.nocommit){
                this.commit();
              }
            } else if (typeof e.nocommit == 'undefined' || !e.nocommit){
              editor.cancelChanges();
            }
          }

          /**
           * activeDoneButton
           */
          this.activeDoneButton = function(m){
            this.done_active = m;
            if (this.done_active){
              $('div.character-editor-popup-button', this.$input)
              .addClass('character-editor-popup-button-highlight');
            } else {
              $('div.character-editor-popup-button', this.$input)
              .removeClass('character-editor-popup-button-highlight');
            }
          }

          /**
           * textInputKeyUp
           */
          this.textInputKeyUp = function(e){
            if (e.keyCode == 13) { // Enter
              if (typeof this.done_active !== 'undefined' && this.done_active){
                this.commit();
              } else {
                this.textInputSubmit({nocommit: this.multiple});
              }
            }  else if (e.keyCode == 27){ // Esc key
              this.commit();
            } else if (e.keyCode == 40 || e.keyCode == 38) { // Down/Up key
              var $items = $('div.character-editor-popup-row', this.$input).filter(':visible');
              var delta = e.keyCode == 40 ? 1 : -1;
              if (delta < 0 && this.done_active){
                this.current = $items.length;
              }
              this.activeDoneButton(false);
              if (typeof this.current == 'undefined' || this.current === null){
                this.current = delta == 1 ? 0 : -1;
              } else {
                this.current = this.current + delta;
              }
              if (this.current >= $items.length){
                if (this.multiple){
                  this.current = null;
                  this.activeDoneButton(true);
                } else {
                  this.current = 0;
                }
              } else if (this.current < 0){
                if (this.multiple){
                  this.current = null;
                  this.activeDoneButton(true);
                } else {
                  this.current = $items.length - 1;
                }
              }
              $items.removeClass('character-editor-popup-row-highlight');
              $($items[this.current]).addClass('character-editor-popup-row-highlight');
            }
          }

          /**
           * destroy
           */
          this.destroy = function(){
            this.$overlay.remove();
            this.$input.remove();
          }

          /**
           * focus
           */
          this.focus = function() {
            this.$textinput.focus();
          };

          /**
           * getValueArray
           */
          this.getValueArray = function(){
            return this.getValue().split(',');
          }

          /**
           * getValue
           */
          this.getValue = function() {
            return this.$input.attr('value');
          };

          /**
           * toggleValue
           */
          this.toggleValue = function(val){
            var a = this.getValueArray();
            var p = $.inArray(val, a);
            if (p >= 0){
              if (this.multiple){
                a.splice(p, 1);
              } else {
                a = [];
              }
            } else {
              if (this.multiple){
                a.push(val);
              } else {
                a = [val];
              }
            }
            this.setValue(a);
          }

          /**
           * setValue
           */
          this.setValue = function(val) {
            if (typeof val !== 'undefined'){
              if (!$.isArray(val)){
                val = val.split(',');
              }
              val.sort();
              if (!this.multiple && val.length > 0){
                val = [val[0]];
              }
              this.$input.attr('value', val.join(','));
            }
            $('div.character-editor-popup-row', this.$input)
            .removeClass('character-editor-popup-row-selected')
            .css({
              backgroundImage: 'none'
            });
            for (var i = 0; i < val.length; i++){
              $('div.character-editor-popup-row[value=' + val[i].toString() + ']', this.$input)
              .addClass('character-editor-popup-row-selected')
              .css({
                backgroundImage: 'url("' + Drupal.settings.basePath + Drupal.settings.CharacterEditorPath + '/images/tick.png")',
                backgroundPosition: '0 center',
                backgroundRepeat: 'no-repeat'
              });
            }
          };

          /**
           * loadValue
           */
          this.loadValue = function(item) {
            var m = Drupal.characterMetadataManager.getMetadata(item.index, editor.column.id);
            if (m && typeof m.value !== 'undefined'){
              this.defaultValue = m.value;
            } else {
              this.defaultValue = '';
            }
            this.$input[0].defaultValue = this.defaultValue;
            this.setValue(this.defaultValue);
          };

          /**
           * serializeValue
           */
          this.serializeValue = function() {
            return this.$input.attr('value');
          };

          /**
           * applyValue
           */
          this.applyValue = function(item, value) {
            var data = {
              // Data to be passed to the backend
              display_id: slickgrid.getViewDisplayID(),
              view: slickgrid.getViewName(),
              entity_ids: slickgrid.getEntityIDs(item),
              field_id: editor.column.id,
              field_name: editor.column.fieldName,
              entity_type: options['entity_type'],
              revision: options['undo'],
              value: value,
              plugin: 'ControlledCharacter',
              character_type: editor.column.data.charType,
              id: editor.item.id
            };
            // Perform the update
            slickgrid.callback('update', data);
          }

          /**
           * isValueChanged
           */
          this.isValueChanged = function() {
            return (this.$input.attr('value') != "" || this.defaultValue != null) &&
                   (this.$input.attr('value') != this.defaultValue);
          };

          /**
           * validate
           */
          this.validate = function() {
            if (editor.column.validator) {
              var validationResults = editor.column.validator(this.$input.attr('value'), this.$input);
              if (!validationResults.valid)
                return validationResults;
            }

            return {
              valid: true,
              msg: null
            }
          }

          this.init();
        },

        /**
         * InlineCharacter
         */
        InlineCharacter: function(args) {
            var $input;
            var defaultValue;
            var scope = this;
            var bt;

            this.init = function() {

                $input = $("<INPUT type=text class='editor-text' />")
                .appendTo(args.container)
                .bind("keydown.nav",
                function(e) {
                    if (e.keyCode === $.ui.keyCode.LEFT || e.keyCode === $.ui.keyCode.RIGHT) {
                        e.stopImmediatePropagation();
                    }
                })
                .focus()
                .select();                
                scope.bt();                
            };

            this.destroy = function() {
                $input.btOff();
                $input.remove();
            };
            
            this.bt = function(){
              if(typeof(args.column.data.bt) !== 'undefined'){
                var ops = {
                  trigger: "none",
                  cssClass: "character-editor-inline controlled-char",
                  fill: 'rgba(0, 0, 0, .7)',
                  cssStyles: {color: 'white', 'font-size': '10px'},
                  spikeLength: 8,
                  shrinkToFit: true,
                  offsetParent: '#slickgrid',
                }
              $input.bt(args.column.data.bt, ops);
              $input.btOn();
              }
            };

            this.focus = function() {
                $input.focus();
            };

            this.getValue = function() {
                return $input.val();
            };

            this.setValue = function(val) {
                $input.val(val);
            };

            this.loadValue = function(item) {
                defaultValue = item[args.column.field] || "";
                $input.val(defaultValue);
                $input[0].defaultValue = defaultValue;
                $input.select();
            };

            this.serializeValue = function() {
                return $input.val();
            };

            this.applyValue = function(item, value) {
              
              var data = {
                  // Data to be passed to the backend
                  display_id: slickgrid.getViewDisplayID(),
                  view: slickgrid.getViewName(),
                  entity_ids: slickgrid.getEntityIDs(item),
                  field_id: args.column.id,
                  field_name: args.column.fieldName,
                  entity_type: options['entity_type'],
                  revision: options['undo'],
                  value: value,
                  plugin: 'InlineCharacter',
                  character_type: args.column.data.charType,
                  id: args.item.id
              };              
              // Perform the update
              slickgrid.callback('update', data);        
              
            }
            
            this.isValueChanged = function() {
                return (! ($input.val() == "" && defaultValue == null)) && ($input.val() != defaultValue);
            };

            this.validate = function() {
                if (args.column.validator) {
                    var validationResults = args.column.validator($input.val(), $input);
                    if (!validationResults.valid)
                    return validationResults;
                }

                return {
                    valid: true,
                    msg: null
                };
            };

            this.init();
        },

        /**
         * ModalCharacter
         */
        ModalCharacter: function(args) {

            var scope = this;
            var defaultValue;
            var state;

            // Attach Drupal.ajax to the slickgrid element
            var element = $('#slickgrid');

            this.init = function() {

                // Open a CTools modal dialog
                Drupal.CTools.Modal.show('ctools-modal-slickgrid-fixed');
                
                // Replace the default close event
                $('a.close').unbind('click').bind('click', scope.cancel);
                
                // Set up the ajax functionality
                var base = 'ctools-modal-slickgrid';
                // Ajax settings
                var element_settings = {
                    event: 'modal',
                    // Drupal.ajax needs an event to fire it - use modal as it should be harmless
                    url: Drupal.settings.slickgrid.slickgrid_callback_url + 'update',
                    submit: {
                        // Data to be passed to the backend
                        js: true,
                        display_id: slickgrid.getViewDisplayID(),
                        view: slickgrid.getViewName(),
                        entity_ids: slickgrid.getEntityIDs(args.item),
                        field_id: args.column.id,
                        field_name: args.column.fieldName,
                        entity_type: options['entity_type'],
                        revision: options['undo'],
                        plugin: 'ModalCharacter',
                        character_type: args.column.data.charType,
                        id: args.item.id
                    }
                };
                Drupal.ajax[base] = new Drupal.ajax(base, element[0], element_settings);
                // Trigger the ready event so start the callback
                element.trigger('modal');
            };

            this.loadValue = function(item) {
                defaultValue = item[args.column.field] || "";
            };

            this.serializeValue = function() {
                return defaultValue;
            };

            this.isValueChanged = function() {
                
                return (state !== defaultValue);
            };

            this.applyValue = function(item) {
                item[args.column.field] = state;
            };
            
            this.cancel = function(){
              args.cancelChanges();  
            }
            
            this.stop = function(){
              
            }

            this.destroy = function() {                       
              element.unbind('modal');
              Drupal.CTools.Modal.dismiss();
            }

            // Validation happens server side
            this.validate = function() {
                return {
                    valid: true,
                    msg: null
                };
            }

            this.init();
        }

    };

    $.extend(window, SlickEditor);

})(jQuery);
