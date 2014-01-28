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
   * Drupal.behaviors.characterEditorExpression
   */
  Drupal.behaviors.characterEditorExpression = {
    attach: function(context, settings){
      // Make sure we can provide the functionaly.
      if (((!window.getSelection || !document.createRange) && !document.selection) || (!"contentEditable" in document.body)){
        return;
      }
      // Replace the textarea with an editable div, and provide text highligting
      var $textarea = $('#edit-field-char-expr textarea', context);
      var $root = $textarea.parent();
      var $hg = $('<div></div>')
      .addClass('character-editor-expression-editor').css({
        width: $textarea.css('width'),
        height: $textarea.css('height'),
      }).attr('contentEditable', true)
      .html('<pre>' + $textarea.val() + '</pre>')
      .appendTo($root)
      .on('keyup', function(){
        var selection = characterSaveSelection(this);
        var raw = $(this).html().replace(/<[^>]+>/ig, '');
        raw = $('<textarea>').html(raw).text();
        var out = (new CharacterExpressionHighlighter(raw)).getHighlighted();
        $(this).html('<pre>' + out + '</pre>');
        $textarea.val(raw);
        characterRestoreSelection(this, selection);
      })
      .trigger('keyup');
      $textarea.css('display', 'none');
      // Insert variables/symbols when clicked
      var $table = $('#edit-field-char-expr table', context)
      var $select = $('.character-editor-variable, .character-editor-symbol', $table);
      $select.on('mousedown', function(e){
        var position = characterSaveSelection($hg.get(0));
        var raw = $hg.html().replace(/<[^>]+>/ig, '');
        raw = $('<textarea>').html(raw).text();
        var insert = ' ' + $(this).text() + ' ';
        var text = [raw.slice(0, position.start), insert, raw.slice(position.start)].join('');
        $hg.html('<pre>' + text + '</pre>');
        position.start = position.start + insert.length;
        position.end = position.end + insert.length;
        characterRestoreSelection($hg.get(0), position);
        $hg.trigger('keyup');
        return false;
      });
    }
  };
})(jQuery);

/**
 * characterSaveSelection
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