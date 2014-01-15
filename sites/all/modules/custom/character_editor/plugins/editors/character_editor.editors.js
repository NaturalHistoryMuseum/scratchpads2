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
            this.$input = $('<div></div>')
            .addClass('character-editor-popup')
            .attr('value', '')
            .css({
              position: 'absolute',
              zIndex: '100',
              top: $(editor.container).offset().top,
              left: $(editor.container).offset().left
            });
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
            this.$input.appendTo('body');
            // Click-out overlay
            this.$overlay = $('<div></div>').css({
              position: 'absolute',
              top: '0',
              left: '0',
              width: $(document).width().toString() + "px",
              height: $(document).height().toString() + "px",
              zIndex: '99'
            }).appendTo('body').one('click', $.proxy(function(e){
              editor.cancelChanges();
            }, this));
          }

          /**
           * rowClick
           */
          this.rowClick = function(val){
            if (val == this.getValue()){
              val = '';
            }
            this.setValue(val);
            if (!this.isValueChanged()){
              editor.cancelChanges();
            } else {
              editor.commitChanges();
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
            /* No-op */
          };

          /**
           * getValue
           */
          this.getValue = function() {
            return this.$input.attr('value');
          };

          /**
           * setValue
           */
          this.setValue = function(val) {
            this.$input.attr('value', val);
            $('div.character-editor-popup-row', this.$input)
            .removeClass('character-editor-popup-row-selected')
            .css({
              backgroundImage: 'none'
            });
            $('div.character-editor-popup-row[value=' + val.toString() + ']', this.$input)
            .addClass('character-editor-popup-row-selected')
            .css({
              backgroundImage: 'url("' + Drupal.settings.basePath + Drupal.settings.CharacterEditorPath + '/images/tick.png")',
              backgroundPosition: '0 center',
              backgroundRepeat: 'no-repeat'
            })
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
