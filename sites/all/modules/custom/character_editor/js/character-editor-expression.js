/**
 * This file contains Javascript for the computed character form
 */
(function($){

  /**
   * CharacterExpressionHighlighter
   * 
   * Class to parse an expression and highligth it
   */
  function CharacterExpressionHighlighter(text){
    /**
     * getHighlighted
     * 
     * Return the highlighted text
     */
    this.getHighlighted = function(){
      // Init
      this.text = text;
      this.result = '';
      this.current = 0;
      // Parse and highlight
      var numeric = false;
      while (this.current < this.text.length){
        var char = this.text[this.current];
        if (char == '{'){
          this.parseEnclosed('}', 'character-editor-variable');
        } else if (char == '\''){
          this.parseEnclosed('\'', 'character-editor-symbol');
        } else if (char == '"'){
          this.parseEnclosed('"', 'character-editor-literal');
        } else if (char.match(/[-0-9]/) && (numeric = this.text.slice(this.current).match(/^(-?[0-9]+(\.[0-9]+)?)/))){
          this.result = this.result + '<span class="character-editor-literal">' + numeric[0] + '</span>';
          this.current = this.current + numeric[0].length;
        } else {
          this.result = this.result + char;
          this.current++;
        }
      }
      return this.result;
    }
    
    /**
     * parseEnclosed
     * 
     * Internel method to parse an enclosed expression, eg. '{...}'.
     * This will add the highlighed to this.result, and will increment this.current.
     */
    this.parseEnclosed = function(closing, className){
      var length = this.text.slice(this.current+1).search(closing);
      if (length == -1){
        length = this.text.length - this.current - 1;
      }
      this.result = this.result + '<span class="' + className + '">' + this.text.slice(this.current, this.current + length + 2) + '</span>';
      this.current = this.current + length + 2;
    }
  };

  /**
   * CharacterExpressionTextarea
   * 
   * Class to transform a textarea into an expression editor. This includes
   * - Syntax highligting of the expression ;
   * - List of variables and symbols can be clicked for auto-insertion
   */
  function CharacterExpressionTextarea($textarea){
    /**
     * init
     */
    this.init = function(){
      // Setup
      this.$textarea = $textarea;
      this.$root = this.$textarea.parent();
      this.$widget_root = this.$textarea.closest('.field-widget-text-textarea');
      this.$table = this.$widget_root.find('table.character-editor-variable');
      this.$names = $('.character-editor-variable, .character-editor-symbol', this.$table);
      // Prepare the new editor
      this.$hg = $('<div></div>');
      this.$textarea.css('display', 'none');
      this.$hg.addClass('character-editor-expression-editor')
      .css({
        width: this.$textarea.css('width'),
        height: this.$textarea.css('height')
      })
      .attr('contentEditable', true)
      .appendTo(this.$root);
      // Init the simple form
      this.initSimpleForm();
      // Bind events
      this.$hg.on('keyup', $.proxy(this, 'highlight'));
      this.$names.on('mousedown', $.proxy(this, 'insertName'));
      // And fire
      this.setContent($textarea.val());
      this.highlight();
      this.selectForm();
    }
    
    /**
     * getContent
     * 
     * Return the plain (non-highlighted) content
     * of the editor
     */
    this.getContent = function(){
      var raw = this.$hg.html().replace(/<[^>]+>/ig, '');
      return $('<textarea>').html(raw).text();
    }
    
    /**
     * setContent
     * 
     * Set the content of the editor
     */
    this.setContent = function(content, selection){
      if (typeof selection == 'undefined'){
        selection = characterSaveSelection(this.$hg.get(0));
      }
      this.$hg.html('<pre>' + content + '</pre>')
      characterRestoreSelection(this.$hg.get(0), selection);
    }
    
    /**
     * highlight
     * 
     * Highlight the text in the textarea
     */
    this.highlight = function(){
      var content = this.getContent();
      var hg = new CharacterExpressionHighlighter(content);
      this.setContent(hg.getHighlighted());
      if (content != $textarea.val()){
        // Unselect simple editor options, so as not to confuse user
        $('.condition-form-mode-simple option', this.$widget_root).prop('selected', false);
        // And save the value in the input textarea field
        $textarea.val(content);
      }
    }
    
    /**
     * insertName
     * 
     * Callback when a name to insert is clicked
     */
    this.insertName = function(e){
      var position = characterSaveSelection(this.$hg.get(0));
      var raw = this.getContent();
      var insert = ' ' + $(e.target).text() + ' ';
      var text = [raw.slice(0, position.start), insert, raw.slice(position.start)].join('');
      position.start = position.start + insert.length;
      position.end = position.end + insert.length;
      this.setContent(text, position);
      this.highlight();
      return false;
    }
    
    /**
     * initSimpleForm
     * 
     * Init the simple form mode, if applicable
     */
    this.initSimpleForm = function(){
      $('.condition-form-mode-selector', this.$widget_root).change($.proxy(this, 'selectForm'));
      $('.condition-form-val-selector optgroup', this.$widget_root).each(function(){
        $(this).attr('character_id', $(this).attr('label'));
        $(this).attr('label', '');
      });
      $('.condition-form-var-selector', this.$widget_root).change($.proxy(function(){
        var char_id = $('.condition-form-var-selector', this.$widget_root).val();
        $('.condition-form-val-selector optgroup', this.$widget_root).css('display', 'none');
        $('.condition-form-val-selector optgroup[character_id!="' + char_id.toString() + '"] option', this.$widget_root).prop('selected', false);
        $('.condition-form-val-selector optgroup[character_id="' + char_id.toString() + '"]', this.$widget_root).css('display', 'block');
      }, this)).trigger('change');
      // Empty the advanced editor version when the simple editor is changed to avoid confusing the user.
      $('.condition-form-mode-simple').change($.proxy(function(){
        this.setContent('');
      }, this));
    }
    
    /**
     * selectForm
     * 
     * Switch between simple and advanced form
     */
    this.selectForm = function(){
      if ($('.condition-form-mode-selector', this.$widget_root).length == 0){
        return;
      }
      if ($('.condition-form-mode-selector', this.$widget_root).val() == 'simple'){
        this.$hg.css('display', 'none');
        $('.condition-form-mode-advanced', this.$widget_root).css('display', 'none');
        $('.condition-form-mode-simple', this.$widget_root).css('display', 'block');
      } else {
        this.$hg.css('display', 'block');
        $('.condition-form-mode-advanced', this.$widget_root).css('display', 'block');
        $('.condition-form-mode-simple', this.$widget_root).css('display', 'none');
      }
    }
    
    this.init();
  }

  /**
   * characterSaveSelection
   * 
   * Cross-browser function to save the current cursor position in an editable div
   */
  function characterSaveSelection(containerEl){
    if (window.getSelection && document.createRange) {
      var range = window.getSelection().getRangeAt(0);
      var preSelectionRange = range.cloneRange();
      preSelectionRange.selectNodeContents(containerEl);
      preSelectionRange.setEnd(range.startContainer, range.startOffset);
      var start = preSelectionRange.toString().length;

      return {
        start: start,
        end: start + range.toString().length
      };
    } else if (document.selection) {
      var selectedTextRange = document.selection.createRange();
      var preSelectionTextRange = document.body.createTextRange();
      preSelectionTextRange.moveToElementText(containerEl);
      preSelectionTextRange.setEndPoint("EndToStart", selectedTextRange);
      var start = preSelectionTextRange.text.length;

      return {
        start: start,
        end: start + selectedTextRange.text.length
      };
    }
  }

  /**
   * characterRestoreSelection
   * 
   * Cross-browser function to restore the current cursor position in an editable div
   */
  function characterRestoreSelection(containerEl, savedSel){
    if (window.getSelection && document.createRange) {
      var charIndex = 0, range = document.createRange();
      range.setStart(containerEl, 0);
      range.collapse(true);
      var nodeStack = [containerEl], node, foundStart = false, stop = false;

      while (!stop && (node = nodeStack.pop())) {
          if (node.nodeType == 3) {
              var nextCharIndex = charIndex + node.length;
              if (!foundStart && savedSel.start >= charIndex && savedSel.start <= nextCharIndex) {
                  range.setStart(node, savedSel.start - charIndex);
                  foundStart = true;
              }
              if (foundStart && savedSel.end >= charIndex && savedSel.end <= nextCharIndex) {
                  range.setEnd(node, savedSel.end - charIndex);
                  stop = true;
              }
              charIndex = nextCharIndex;
          } else {
              var i = node.childNodes.length;
              while (i--) {
                  nodeStack.push(node.childNodes[i]);
              }
          }
      }

      var sel = window.getSelection();
      sel.removeAllRanges();
      sel.addRange(range);
    } else if (document.selection) {
      var textRange = document.body.createTextRange();
      textRange.moveToElementText(containerEl);
      textRange.collapse(true);
      textRange.moveEnd("character", savedSel.end);
      textRange.moveStart("character", savedSel.start);
      textRange.select();
    }
  }
  
  /**
   * Drupal.behaviors.characterEditorExpression
   */
  Drupal.behaviors.characterEditorExpression = {
    attach: function(context, settings){
      // Make sure we can provide the functionaly.
      if (((!window.getSelection || !document.createRange) && !document.selection) || (!"contentEditable" in document.body)){
        return;
      }
      if (typeof Drupal.CharacterEditorExpression == 'undefined'){
        Drupal.CharacterEditorExpression = [];
      }
      // Attach the editor class to each textarea
      var $textareas = $('#edit-field-char-expr textarea, #edit-field-char-condition textarea', context);
      for (var i = 0; i < $textareas.length; i++){
        Drupal.CharacterEditorExpression.push(new CharacterExpressionTextarea($($textareas.get(i))));
      }
    }
  };
})(jQuery);