<?php

/**
 * class CharacterExpressionParser
 * 
 * This class is used to parse mathematical and logical expressions
 * with variables. It is used for computed characters as well as
 * character dependencies.
 * 
 * Expressions are formed of:
 * 
 * - Numbers, eg. 4.3 ;
 * 
 * - Text, which should be enclosed in double quotes, eg. "hello" ;
 * 
 * - Symbols, which should be enclosed in single quotes, eg. 'blue'.
 *   Symbols differ from text in that they can be ordered to create
 *   ranges (see 'Ranges') and renamed via the 'rename' method ;
 *     
 * - Variables, which should be enclosed in curly brackets, eg. {height}
 * 
 * - Ranges, enclosed in square brackets and using the '..' operator, eg.
 *   [1 .. 5]. Ranges of strings or lists are not permitted - however it is
 *   possible to build ranges of symbols such as ['yellow' .. 'green'] provided
 *   the ordering of symbols is passed in when evaluating the expression ;
 *   
 * - Lists of alternatives, enclosed in square brackets, built with & (for and) 
 *   and | (for or) operators, eg. ["blue" | "green"], [('tall' & 'thin') | 'small'] ;
 *   
 * - Mathematical operators which act on numbers: +,-,*,/
 * 
 * - Comparaison operators: <, <=, >, >=, is, is-not. Comparaison operators
 *   return '1' for true and '0' for false ;
 *   
 * - List and range test operators: in, not-in (eg. ("blue" in ["blue" | "red"])
 *   or (2 in [1 .. 5]) ;
 *   
 * - Boolean operators: and, or, not. These will cast their operands such that
 *   non-zero numbers are true, and everything else is false.
 *   
 * - Parenthesized expressions
 * 
 * When an expression is evaluated, a list of variables and on order for possible symbols
 * can be provided. The variables is map from name to value. The value can directly
 * be an instance of CharacterExpressionValue, however for simplicity of storage
 * simpler types can also be provided directly:
 * 
 * - Numbers (strict, 12 is a number but "12" isn't) ;
 * - Text ;
 * - Ranges, represented by an array defining 'min' and 'max' (and nothing else) ;
 * - Arrays of conjunctions, represented by an array with numeric keys ;
 * - An array defining 'type' and 'value', where 'type' may be:
 *   'number', 'text', 'symbol', 'range', 'list-and', 'list-or', 'expr' and
 *   'undefined'.
 *
 *     . 'symbol' is needed to differentiate from 'text'.
 *     . Lists can be build using 'list-and' and 'list-or' trees, though it's simpler
 *      to use 'expr' type which will parse it's value as af it was an expression
 *     . 'undefined' is used to represent undefined values (see notes).
 * 
 * Some notes:
 * - Testing for equality between a value and a range is always FALSE.
 *   
 * - For convenience, when 'in' and 'not-in' are used such that neither operands are
 *   lists or ranges, then it is assumed the second operand is a list with one item, eg.
 *   'a' in 'a' is equivalent to 'a' in ['a'] (or 'a' is 'a').
 *   
 * - Mathematical operators and the 'and' and 'or' operators apply on all elements of a list:
 *   so 10 * [1 & 2 | 3] is [10 & 20 | 30] ;
 *   
 * - Equality and comparaison operators apply as expected on the elements of a list:
 *   10 > [1 & 11] is FALSE while 10 > [1 | 11] is TRUE. The 'in' (or 'not-in') operators
 *   will test for one of the alternatives ; eg. 'blue' in ['blue' | 'green'] is TRUE.
 *   Note that only ['blue' & 'green'] is equal to ['blue' & 'green'], though
 *   ['blue' & 'green'] in [('blue' & 'green') | 'yellow'] is TRUE, and
 *   ['thin' & 'blue'] is [('thin' | 'thick') & ('blue' | 'green')] is TRUE.
 *
 * - Undefined variables evaluate to <undefined> ;
 *
 * - Operators which are not comparaison operators return <undefined> when
 *   applied to <undefined>. Inequality operators (not-in, is-not) return TRUE
 *   when applied to <undefined> and other comparaison operators return FALSE
 *   when applied to <undefined>.
 *
 */
class CharacterExpressionParser{
  /* Exception Error codes */
  const MISSING_VARIABLE = 1;
  // Operators in order of precedence, with associated arity. Unariy "-" is allowed
  // but replaced at parse time with "neg". "," and ".." are parsed as operators,
  // but internally used to build lists and ranges.
  private $operators = array(
    'neg' => array(
      'arity' => 1
    ),
    'not' => array(
      'arity' => 1
    ),
    '*' => array(
      'arity' => 2
    ),
    '/' => array(
      'arity' => 2
    ),
    '+' => array(
      'arity' => 2
    ),
    '-' => array(
      'arity' => 2
    ),
    '<' => array(
      'arity' => 2,
      'boolean' => TRUE
    ),
    '<=' => array(
      'arity' => 2,
      'boolean' => TRUE
    ),
    '>' => array(
      'arity' => 2,
      'boolean' => TRUE
    ),
    '>=' => array(
      'arity' => 2,
      'boolean' => TRUE
    ),
    'is' => array(
      'arity' => 2,
      'boolean' => TRUE
    ),
    'is-not' => array(
      'arity' => 2,
      'boolean' => TRUE
    ),
    'in' => array(
      'arity' => 2,
      'boolean' => TRUE
    ),
    'not-in' => array(
      'arity' => 2,
      'boolean' => TRUE
    ),
    'and' => array(
      'arity' => 2,
      'boolean' => TRUE
    ),
    'or' => array(
      'arity' => 2,
      'boolean' => TRUE
    ),
    '&' => array(
      'arity' => 2,
      'constructor' => TRUE
    ),
    '|' => array(
      'arity' => 2,
      'constructor' => TRUE
    ),
    ',' => array(
      'arity' => 2,
      'constructor' => TRUE
    ),
    '..' => array(
      'arity' => 2,
      'constructor' => TRUE
    )
  );

  /**
   * __construct
   * 
   * Create a new expression parser from an expression
   */
  public function __construct($expression){
    $this->raw = $expression;
    $this->tokenize();
    $this->parse();
    $this->buildTree();
  }

  /**
   * evaluate
   * 
   * Evaluate the value of the expression with the given variables.
   * 
   * This will return an object which inherits from CharacterExpressionValue.
   */
  public function evaluate($variable_map = array(), $symbol_order = array()){
    if(empty($this->tree)){throw new Exception("No expression defined");}
    $val = $this->evaluateRecursive($this->tree, array(
      'variable_map' => $variable_map,
      'symbol_order' => $symbol_order
    ));
    return $val;
  }

  /**
   * evaluateRecursive
   */
  private function evaluateRecursive($tree, $context){
    if($tree['type'] == 'application'){
      $ops = array();
      foreach($tree['operands'] as $operand){
        $ops[] = $this->evaluateRecursive($operand, $context);
      }
      if($this->operators[$tree['operator']]['arity'] == 1){
        return $ops[0]->applyUnaryOperator($tree['operator']);
      }else if($tree['operator'] == '&' || $tree['operator'] == '|'){
        return new CharacterExpressionList(array(
          $ops[0],
          $ops[1]
        ), ($tree['operator'] == '&'), $context);
      }else if($tree['operator'] == '..'){
        return new CharacterExpressionRange($ops[0], $ops[1]);
      }else{
        return $ops[0]->applyBinaryOperator($tree['operator'], $ops[1]);
      }
    }else if($tree['type'] == 'v'){
      $tree['value']->setContext($context);
      return $tree['value'];
    }
  }

  /**
   * rename
   * 
   * Rename the variables and symbol in this expression
   */
  public function rename($var_map = array(), $sym_map = array()){
    foreach($this->tokens as $t){
      if(is_object($t) && $t instanceof CharacterExpressionVariable){
        if(isset($var_map[$t->name()])){
          $t->rename($var_map[$t->name()]);
        }
      }else if(is_object($t) && $t instanceof CharacterExpressionSymbol){
        if(isset($sym_map[$t->value()])){
          $t->set($sym_map[$t->value()]);
        }
      }
    }
    return $this;
  }

  /**
   * toString
   * 
   * Returns the parsed expression as a string
   */
  public function getExpression(){
    $out = '';
    $tokens = $this->tokens;
    foreach($this->expression as $item){
      if($item == 'o'){
        $v = array_shift($tokens);
        if($this->operators[$v]['arity'] == 2){
          $out .= ' ' . $v . ' ';
        }else{
          $out .= $v;
        }
      }else if($item == 'v'){
        $v = array_shift($tokens);
        $out .= $v->asExpression();
      }else{
        $out .= $item;
      }
    }
    return $out;
  }

  /**
   * tokenize
   * 
   * Tokenize the expression by replacing literals by the letter 'l', variables by 'v' and
   * operators by 'o' and pushing the corresponding values on $this->tokens
   */
  private function tokenize(){
    $this->tokens = array();
    $this->expression = array();
    $pos = 0;
    while($pos < strlen($this->raw)){
      // Skip spaces
      if(preg_match('/\s/', $this->raw[$pos])){
        $pos++;
        continue;
      }
      // Match token
      $end_pos = -1;
      $closing_symbol = '';
      $no_token = FALSE;
      $type = '';
      if($this->raw[$pos] == "'"){
        $end_pos = strpos($this->raw, "'", $pos + 1);
        $this->expression[] = 'v';
        $closing_symbol = "'";
        $type = 'symbol';
      }else if($this->raw[$pos] == '"'){
        $end_pos = strpos($this->raw, '"', $pos + 1);
        $this->expression[] = 'v';
        $closing_symbol = '"';
        $type = 'text';
      }else if($this->raw[$pos] == '{'){
        $end_pos = strpos($this->raw, '}', $pos + 1);
        $this->expression[] = 'v';
        $closing_symbol = '}';
        $type = 'variable';
      }else if(preg_match('/^([0-9]+(\.[0-9]+)?)/', substr($this->raw, $pos), $matches)){
        $this->expression[] = 'v';
        $end_pos = $pos + strlen($matches[1]) - 1;
        $type = 'number';
      }else if(substr($this->raw, $pos, strlen('<undefined>')) == '<undefined>'){
        $this->expression[] = 'v';
        $end_pos = $pos + strlen('<undefined>');
        $type = 'undefined';
      }else if($this->isSymbol($this->raw[$pos])){
        $end_pos = $pos;
        $this->expression[] = $this->raw[$pos];
        $no_token = TRUE;
      }else{
        // Find the longest matching operator
        foreach($this->operators as $op => $info){
          if(substr($this->raw, $pos, strlen($op)) == $op){
            if($pos + strlen($op) - 1 > $end_pos){
              $end_pos = $pos + strlen($op) - 1;
            }
          }
        }
        $this->expression[] = 'o';
      }
      // Report errors
      if($end_pos == -1){
        if($closing_symbol){
          throw new Exception(t("Could not find closing '%sym' from position %pos.", array(
            '%expr' => $this->raw,
            '%sym' => $closing_symbol,
            '%pos' => $pos
          )));
        }else{
          throw new Exception(t("Item at position %pos and begining with '%start...' is neither an operator, literal or accepted symbol. Maybe it should be enclosed in quotes?", array(
            '%pos' => $pos,
            '%start' => substr($this->raw, $pos, 5)
          )));
        }
      }
      // Store token
      if(!$no_token){
        if($closing_symbol){
          $str = substr($this->raw, $pos + 1, $end_pos - $pos - 1);
        }else{
          $str = substr($this->raw, $pos, $end_pos - $pos + 1);
        }
        if($type == 'symbol'){
          $this->tokens[] = new CharacterExpressionSymbol($str);
        }else if($type == 'text'){
          $this->tokens[] = new CharacterExpressionText($str);
        }else if($type == 'variable'){
          $this->tokens[] = new CharacterExpressionVariable($str);
        }else if($type == 'number'){
          $this->tokens[] = new CharacterExpressionNumber($str);
        }else if($type == 'undefined'){
          $this->tokens[] = new CharacterExpressionUndefined();
        }else{
          $this->tokens[] = $str;
        }
      }
      $pos = $end_pos + 1;
    }
  }

  /**
   * parse
   * 
   * Parse the tokenized expression into nodes
   */
  private function parse(){
    $expr = $this->expression;
    $tokens = $this->tokens;
    $this->nodes = array(
      'type' => 'group',
      'nodes' => $this->parseRecursive($expr, $tokens)
    );
  }

  /**
   * isGroupSymbol
   * 
   * Return TRUE if the given character is a grouping symbol.
   * 
   * If $type is start/end only return true for start/end symbols.
   * 
   */
  private function isSymbol($char, $type = 'all'){
    if(($type == 'all' || $type == 'start') && ($char == '(' || $char == '[')){return TRUE;}
    if(($type == 'all' || $type == 'end') && ($char == ')' || $char == ']')){return TRUE;}
    return FALSE;
  }

  /**
   * parseRecursive
   * 
   * Recursive parser
   */
  private function parseRecursive(&$expr, &$tokens, $initial = NULL){
    $nodes = array();
    $current = $initial;
    $current_token = NULL;
    while(count($expr)){
      // Read prev/current/next token
      $prev = $current;
      $prev_token = $current_token;
      $current = array_shift($expr);
      if(!$this->isSymbol($current)){
        $current_token = array_shift($tokens);
      }else{
        $current_token = NULL;
      }
      if(count($expr)){
        $next = reset($expr);
        if(!$this->isSymbol($next)){
          $next_token = reset($tokens);
        }else{
          $next_token = NULL;
        }
      }else{
        $next = NULL;
        $next_token = NULL;
      }
      // Replace '-' with 'neg' if required
      if($current == 'o' && $current_token == '-'){
        if(!$prev || $this->isSymbol($prev, 'start') || $prev == 'o'){
          $current_token = 'neg';
        }
      }
      // Replace ',' with '|'
      if($current == 'o' && $current_token == ','){
        $current_token = '|';
      }
      // Validate
      $val = $this->validateParseElement($prev, $prev_token, $current, $current_token, $next, $next_token);
      if($val !== TRUE){throw new Exception($val);}
      // Create node
      if($current == '('){
        $children = $this->parseRecursive($expr, $tokens, $current);
        if(count($children) == 0){
          throw new Exception("Cannot not have empty groups");
        }else if(count($children) == 1){
          $node = reset($children);
          $current = $node['type'];
          $current_token = $node['value'];
          $nodes[] = $current;
        }else{
          $nodes[] = array(
            'type' => 'group',
            'nodes' => $children
          );
          $current = ')';
        }
      }else if($current == ')'){
        if($initial != '('){throw new Exception(t("Found a closing bracket without a matching opening bracket", array(
            '%expr' => $this->raw
          )));}
        return $nodes;
      }else if($current == '['){
        $children = $this->parseRecursive($expr, $tokens, $current);
        $nodes[] = array(
          'type' => 'group',
          'nodes' => $children
        );
        $current = ']';
      }else if($current == ']'){
        if($initial != '['){throw new Exception(t("Found a closing square bracket without a matching opening bracket", array(
            '%expr' => $this->raw
          )));}
        return $nodes;
      }else{
        $nodes[] = array(
          'type' => $current,
          'value' => $current_token
        );
      }
    }
    if($initial){throw new Exception(t("Unclosed bracket '%ini'", array(
        '%expr' => $this->raw,
        '%ini' => $initial
      )));}
    return $nodes;
  }

  /**
   * validateParseElement
   * 
   * Validate that an element is ok given it's context
   */
  private function validateParseElement($prev, $prev_token, $current, $current_token, $next, $next_token){
    if($this->isSymbol($current, 'start')){
      if($prev == 'v'){return t('Symbol "%sym" cannot be preceded by value %value', array(
          '%sym' => $current,
          '%value' => $prev_token
        ));}
      if($next === NULL){return t('Symbol "%sym" cannot be at the end of the expression', array(
          '%sym' => $current
        ));}
      if($next == 'o' && $next_token != "-" && $this->operators[$next_token]['arity'] == 2){return t('Symbol "%sym" cannot be followed by binary operator %op', array(
          '%sym' => $current,
          '%op' => $next_token
        ));}
    }else if($this->isSymbol($current, 'end')){
      if($prev == NULL){return t('Symbol "%sym" cannot be at the begining of the expression', array(
          '%sym' => $current
        ));}
      if($prev == 'o'){return t('Symbol "%sym" cannot be preceded by operator %op', array(
          '%sym' => $current,
          '%op' => $prev_token
        ));}
      if($next == 'v'){return t('Symbol ")" cannot be followed by value %value', array(
          '%value' => $next_token
        ));}
    }else if($current == 'o' && $this->operators[$current_token]['arity'] == 2){
      if($prev === NULL || $this->isSymbol($prev, 'start')){return t('Binary operator %op cannot be at start of expression or group', array(
          '%op' => $current_token
        ));}
      if($next === NULL || $this->isSymbol($next, 'end')){return t('Binary operator %op cannot be at end of expression or group', array(
          '%op' => $current_token
        ));}
    }else if($current == 'o'){
      if($prev == 'v'){return t('Unary operator %op cannot be preceded by value %value', array(
          '%value' => $prev_token,
          '%op' => $current_token
        ));}
      if($next === NULL || $this->isSymbol($next, 'end')){return t('Unary operator %op cannot be at end of expression or group', array(
          '%op' => $current_token
        ));}
    }else if($current == 'v'){
      if($prev == 'v'){return t("Value %value cannot be preceded by value %p_value", array(
          '%value' => $current_token,
          '%p_value' => $prev_token
        ));}
      if($next == 'v'){return t("Value %value cannot be followed by value %p_value", array(
          '%value' => $current_token,
          '%p_type' => $translate[$next],
          '%p_value' => $next_token
        ));}
      if($this->isSymbol($next, 'start')){return t("Value %value cannot be followed by symbol \"%sym\"", array(
          '%sym' => $next,
          '%value' => $current_token
        ));}
    }
    return TRUE;
  }

  /**
   * buildTree
   * 
   * From the parsed expression build an execution tree.
   */
  private function buildTree(){
    $this->tree = $this->buildTreeRecursive($this->nodes);
  }

  /**
   * buildTreeRecursive
   */
  private function buildTreeRecursive($node){
    if($node['type'] == 'group'){
      $tree = array();
      foreach($node['nodes'] as $child_node){
        $tree[] = $this->buildTreeRecursive($child_node);
      }
      foreach($this->operators as $operator => $info){
        do{
          $index = -1;
          foreach($tree as $i => $elem){
            if($elem['type'] == 'o' && $elem['value'] == $operator){
              $index = $i;
              break;
            }
          }
          if($index >= 0){
            $new_tree = array();
            if($info['arity'] == 1){
              $new_tree = array_slice($tree, 0, $index);
              $operands = array(
                $tree[$index + 1]
              );
            }else{
              $new_tree = array_slice($tree, 0, $index - 1);
              $operands = array(
                $tree[$index - 1],
                $tree[$index + 1]
              );
            }
            $new_tree[] = array(
              'type' => 'application',
              'operator' => $operator,
              'operands' => $operands
            );
            $new_tree = array_merge($new_tree, array_slice($tree, $index + 2));
            $tree = $new_tree;
          }
        }
        while($index >= 0);
      }
      return reset($tree);
    }
    return $node;
  }
}

/**
 * CharacterExpressionValue
 * 
 * This is a parent class to be used by the possible
 * value types in the expressions.
 * 
 */
abstract class CharacterExpressionValue{

  /**
   * __construct
   */
  public function __construct($val, $type, $context){
    $this->type = $type;
    $this->val = $val;
    $this->context = $context;
  }

  /**
   * value
   */
  public function value(){
    return $this->val;
  }

  /**
   * type
   */
  public function type(){
    return $this->type;
  }

  /**
   * setContext
   */
  public function setContext($context){
    $this->context = $context;
  }

  /**
   * asExpression
   * 
   * Return a string representing this value as a parsable expression
   */
  public function asExpression(){
    return $this->val;
  }

  /**
   * asVariable
   * 
   * Return this value of this object as a type which can be used as an
   * input variable
   */
  public function asVariable(){
    return $this->val;
  }

  /**
   * applyUnaryOperator
   * 
   * Called when the given unrary operator is applied to this value.
   * Should return a new value or raise an exception.
   */
  public abstract function applyUnaryOperator($o);

  /**
   * applyBinaryOperator
   * 
   * Called when the given binary operator is applied to this value,
   * such that this value is on the left of the expression.
   * Should return a new value or raise an exception.
   */
  public abstract function applyBinaryOperator($o, $v);
}

/**
 * CharacterExpressionNumber
 * 
 * This class represents a number
 */
class CharacterExpressionNumber extends CharacterExpressionValue{

  /**
   * __construct
   */
  public function __construct($v, $context = array()){
    parent::__construct($v + 0, 'number', $context);
  }

  /**
   * applyUnaryOperator
   */
  public function applyUnaryOperator($o){
    if($o == 'not'){
      return new CharacterExpressionNumber((int)(!$this->val), $this->context);
    }else if($o == 'neg'){
      return new CharacterExpressionNumber(-$this->val, $this->context);
    }else{
      throw new Exception(t("Operator %op cannot be applied to numbers", array(
        '%op' => $o
      )));
    }
  }

  /**
   * applyBinaryOperator
   */
  public function applyBinaryOperator($o, $v){
    if($v->type() == 'undefined'){
      return $v->applyBinaryOperator($o, $this);
    }else if($v->type() == 'number'){
      switch($o){
        case '+':
          return new CharacterExpressionNumber($this->val + $v->value(), $this->context);
        case '-':
          return new CharacterExpressionNumber($this->val - $v->value(), $this->context);
        case '*':
          return new CharacterExpressionNumber($this->val * $v->value(), $this->context);
        case '/':
          return new CharacterExpressionNumber($this->val / $v->value(), $this->context);
        case '>':
          return new CharacterExpressionNumber((int)($this->val > $v->value()), $this->context);
        case '>=':
          return new CharacterExpressionNumber((int)($this->val >= $v->value()), $this->context);
        case '<':
          return new CharacterExpressionNumber((int)($this->val < $v->value()), $this->context);
        case '<=':
          return new CharacterExpressionNumber((int)($this->val <= $v->value()), $this->context);
        case 'is':
        case 'in':
          return new CharacterExpressionNumber((int)($this->val == $v->value()), $this->context);
        case 'is-not':
        case 'not-in':
          return new CharacterExpressionNumber((int)($this->val != $v->value()), $this->context);
        case 'and':
          return new CharacterExpressionNumber((int)($this->val && $v->value()), $this->context);
        case 'or':
          return new CharacterExpressionNumber((int)($this->val || $v->value()), $this->context);
        default:
          throw new Exception(t("Operator %op cannot be used between numbers", array(
            '%op' => $o
          )));
      }
    }else if($v->type() == 'range' || $v->type() == 'list'){
      return $v->applyBinaryOperator($o, $this, TRUE);
    }else{
      throw new Exception(t("Operator %op cannot be applied between a number and a %type", array(
        '%op' => $o,
        '%type' => $v->type()
      )));
    }
  }
}

/**
 * CharacterExpressionText
 *
 * This class represents a string of text
 */
class CharacterExpressionText extends CharacterExpressionValue{

  /**
   * __construct
   */
  public function __construct($v, $context = array()){
    parent::__construct($v, 'text', $context);
  }

  /**
   * asExpression
   */
  public function asExpression(){
    return '"' . $this->val . '"';
  }

  /**
   * applyUnaryOperator
   */
  public function applyUnaryOperator($o){
    if($o == 'neg'){
      $o = '-';
    }
    throw new Exception(t("Operator %op cannot be applied to text", array(
      '%op' => $o
    )));
  }

  /**
   * applyBinaryOperator
  */
  public function applyBinaryOperator($o, $v){
    if($v->type() == 'undefined'){
      return $v->applyBinaryOperator($o, $this);
    }else if($v->type() == 'text' || $v->type() == 'symbol'){
      switch($o){
        case '+':
          return new CharacterExpressionText($this->val . $v->value(), $this->context);
        case 'is':
        case 'in':
          return new CharacterExpressionNumber((int)($this->val == $v->value()), $this->context);
        case 'is-not':
        case 'not-in':
          return new CharacterExpressionNumber((int)($this->val != $v->value()), $this->context);
        default:
          throw new Exception(t("Operator %op cannot be used between text strings", array(
            '%op' => $o
          )));
      }
    }else if($v->type() == 'list' || $v->type() == 'range'){
      return $v->applyBinaryOperator($o, $this, TRUE);
    }else{
      throw new Exception(t("Operator %op cannot be applied between text and a %type", array(
        '%op' => $o,
        '%type' => $v->type()
      )));
    }
  }
}

/**
 * CharacterExpressionSymbol
 *
 * This class represents a symbol
 */
class CharacterExpressionSymbol extends CharacterExpressionValue{

  /**
   * __construct
   */
  public function __construct($val, $context = array()){
    parent::__construct($val, 'symbol', $context);
  }

  /**
   * set
   */
  public function set($val){
    $this->val = $val;
  }

  /**
   * asExpression
   */
  public function asExpression(){
    return '\'' . $this->val . '\'';
  }

  /**
   * asVariable
   */
  public function asVariable(){
    return array(
      'type' => 'symbol',
      'value' => $this->val
    );
  }

  /**
   * applyUnaryOperator
   */
  public function applyUnaryOperator($o){
    if($o == 'neg'){
      $o = '-';
    }
    throw new Exception(t("Operator %op cannot be applied to symbols", array(
      '%op' => $o
    )));
  }

  /**
   * applyBinaryOperator
   */
  public function applyBinaryOperator($o, $v){
    if($v->type() == 'undefined'){return $v->applyBinaryOperator($o, $this);}
    if(isset($this->context['symbol_order'])){
      $symbol_order = $this->context['symbol_order'];
    }else{
      $symbol_order = array();
    }
    if($v->type() == 'symbol'){
      if($o == '>' || $o == '>=' || $o == '<' || $o == '<='){
        if(!in_array($this->val, $symbol_order) || !in_array($v->value(), $symbol_order)){return new CharacterExpressionNumber(0, $this->context);}
      }
      switch($o){
        case '>':
          return new CharacterExpressionNumber((int)(array_search($this->val, $symbol_order) > array_search($v->value(), $symbol_order)), $this->context);
        case '>=':
          return new CharacterExpressionNumber((int)(array_search($this->val, $symbol_order) >= array_search($v->value(), $symbol_order)), $this->context);
        case '<':
          return new CharacterExpressionNumber((int)(array_search($this->val, $symbol_order) < array_search($v->value(), $symbol_order)), $this->context);
        case '<=':
          return new CharacterExpressionNumber((int)(array_search($this->val, $symbol_order) <= array_search($v->value(), $symbol_order)), $this->context);
        case 'is':
        case 'in':
          return new CharacterExpressionNumber((int)($this->val == $v->value()), $this->context);
        case 'is-not':
        case 'not-in':
          return new CharacterExpressionNumber((int)($this->val != $v->value()), $this->context);
        default:
          throw new Exception(t("Operator %op cannot be used between symbols", array(
            '%op' => $o
          )));
      }
    }else if($v->type() == 'text'){
      switch($o){
        case 'is':
        case 'in':
          return new CharacterExpressionNumber((int)($this->val == $v->value()), $this->context);
        case 'is-not':
        case 'not-in':
          return new CharacterExpressionNumber((int)($this->val != $v->value()), $this->context);
        default:
          throw new Exception(t("Operator %op cannot be used between a symbol and a text string", array(
            '%op' => $o
          )));
      }
    }else if($v->type() == 'list' || $v->type() == 'range'){
      return $v->applyBinaryOperator($o, $this, TRUE);
    }else{
      throw new Exception(t("Operator %op cannot be applied between a symbol and a %type", array(
        '%op' => $o,
        '%type' => $v->type()
      )));
    }
  }
}

/**
 * CharacterExpressionList
 *
 */
class CharacterExpressionList extends CharacterExpressionValue{

  /**
   * __construct
   */
  public function __construct($val, $conjoined, $context){
    parent::__construct($val, 'list', $context);
    $this->conjoined = $conjoined;
    $new_values = array();
    // Merge lists
    foreach($this->val as $v){
      if(($v instanceof CharacterExpressionList) && $v->isConjoined() == $this->conjoined){
        $new_values = array_merge($new_values, $v->value());
      }else{
        $new_values[] = $v;
      }
    }
    // Remove duplicate terms
    $this->val = array();
    for($i = 0; $i < count($new_values); $i++){
      $dup = FALSE;
      if($i < count($new_values) - 1){
        for($j = $i + 1; $j < count($new_values); $j++){
          if(get_class($new_values[$i]) == get_class($new_values[$j]) && $new_values[$i]->asExpression() == $new_values[$j]->asExpression()){
            $dup = TRUE;
            break;
          }
        }
      }
      if(!$dup){
        $this->val[] = $new_values[$i];
      }
    }
  }

  /**
   * isConjoined
   */
  public function isConjoined(){
    return $this->conjoined;
  }

  /**
   * setContext
   */
  public function setContext($context){
    $this->context = $context;
    foreach($this->val as $item){
      $item->setConcext($context);
    }
  }

  /**
   * asExpression
   */
  public function asExpression(){
    $a = array();
    foreach($this->val as $item){
      if($item instanceof CharacterExpressionList){
        $item_str = $item->asExpression();
        $a[] = '(' . substr($item_str, 1, strlen($item_str) - 2) . ')';
      }else{
        $a[] = $item->asExpression();
      }
    }
    $op = $this->conjoined ? ' & ' : ' | ';
    return '[' . implode($op, $a) . ']';
  }

  /**
   * asString
   */
  public function asString(){
    $a = array();
    foreach($this->val as $item){
      if($item instanceof CharacterExpressionList){
        $item_str = $item->asString();
        $a[] = '(' . $item_str . ')';
      }else{
        $a[] = (string)$item->value();
      }
    }
    $op = $this->conjoined ? ' <em>and</em> ' : ' <em>or</em> ';
    return implode($op, $a);
  }

  /**
   * asVariable
   */
  public function asVariable(){
    $a = array();
    foreach($this->val as $item){
      $a[] = $item->asVariable();
    }
    return array(
      'list-' . ($this->conjoined ? 'and' : 'or'),
      'value' => $a
    );
  }

  /**
   * getFlattenedValue
   *
   * Return an array containing the elements in this list and sublists ;
   * note that this does not indiciate the type of operators used between
   * the elements.
   */
  public function getFlattenedValue(){
    $out = array();
    foreach($this->val as $item){
      if($item instanceof CharacterExpressionList){
        $out = array_merge($out, $item->getFlattenedValue());
      }else{
        $out[] = $item->value();
      }
    }
    return $out;
  }

  /**
   * applyUnaryOperator
   */
  public function applyUnaryOperator($o){
    foreach($this->val as $item){
      $item->applyUnaryOperator($o);
    }
  }

  /**
   * applyBinaryOperator
   *
   * $reverse may be true for numbers, texts and symbols which may call this directly.
   */
  public function applyBinaryOperator($o, $v, $reverse = FALSE){
    if($v->type() == 'undefined'){return $v->applyBinaryOperator($o, $this);}
    if($reverse){
      if($o == '>'){
        $o = '<=';
      }else if($o == '>='){
        $o = '<';
      }else if($o == '<'){
        $o = '>=';
      }else if($o == '<='){
        $o = '>';
      }
    }
    if($v->type() != 'list'){
      switch($o){
        case 'in':
        case 'is':
        case 'not-in':
        case 'is-not':
        case '>':
        case '>=':
        case '<':
        case '<=':
          foreach($this->val as $item){
            $test = $item->applyBinaryOperator($o, $v)->value();
            if($test && !$this->conjoined){
              return new CharacterExpressionNumber(1, $this->context);
            }else if(!$test && $this->conjoined){return new CharacterExpressionNumber(0, $this->context);}
          }
          return new CharacterExpressionNumber((int)($this->conjoined), $this->context);
        case '+':
        case '-':
        case '*':
        case '/':
        case 'and':
        case 'or':
          $new_array = array();
          foreach($this->val as $index => $item){
            if($reverse){
              $new_array[] = $v->applyBinaryOperator($o, $item);
            }else{
              $new_array[] = $item->applyBinaryOperator($o, $v);
            }
          }
          return new CharacterExpressionList($new_array, $this->conjoined, $this->context);
        default:
          throw new Exception(t("Operator %op cannot be used between an array and a %type", array(
            '%op' => $o,
            '%type' => $v->type()
          )));
      }
    }else{
      switch($o){
        case 'is':
          $r1 = $this->applyBinaryOperator('in', $v);
          if($r1->value() == 0){return $r1;}
          return $v->applyBinaryOperator('in', $this);
        case 'is-not':
          $r1 = $this->applyBinaryOperator('not-in', $v);
          if($v1->value() != 0){return $r1;}
          return $v->applyBinaryOperator('not-in', $this);
        case 'not-in':
          return new CharacterExpressionNumber((int)(!$this->applyBinaryOperator('in', $v)->value()), $this->context);
        case 'in':
          if(!$this->conjoined){
            foreach($this->val as $item){
              if($item->applyBinaryOperator($o, $v)->value()){return new CharacterExpressionNumber(1, $this->context);}
            }
            return new CharacterExpressionNumber(0, $this->context);
          }else{
            if($v->isConjoined()){
              // We must ensure that for every element in this list,
              // there is a matching element in the other list ; but
              // the same element in the other list cannot match two
              // elements in this list.
              $matches = array();
              foreach($this->val as $this_index => $this_val){
                foreach($v->value() as $v_index => $v_item){
                  if($this_val->applyBinaryOperator('in', $v_item)->value()){
                    $matches[$this_index][] = $v_index;
                  }
                }
                if(empty($matches[$this_index])){return new CharacterExpressionNumber(0, $this->context);}
              }
              // Ensure we have a partitioning
              if($this->partitioningExists($matches)){
                return new CharacterExpressionNumber(1, $this->context);
              }else{
                return new CharacterExpressionNumber(0, $this->context);
              }
            }else{
              return $v->applyBinaryOperator('in', $this);
            }
          }
      }
    }
  }

  /**
   * partitioningExists
   * 
   * Given a two dimensional values where the first dimension
   * represents a partition and the second dimension the values
   * that are allowed in that partition, ensure it is possible to
   * reduce the available values such that no parition is empty
   * and no value is used in more than one partition.
   * 
   */
  private function partitioningExists($values, $exclude = array()){
    $start = array_diff(array_shift($values), $exclude);
    if(empty($values)){return count($start) > 0;}
    foreach($start as $v){
      if($this->partitioningExists($values, array_merge($exclude, array(
        $v
      )))){return TRUE;}
    }
    return FALSE;
  }
}

/**
 * CharacterExpressionRange
 */
class CharacterExpressionRange extends CharacterExpressionValue{

  /**
   * __construct
   */
  public function __construct($min, $max, $context = array()){
    $this->val = array(
      'min' => $min,
      'max' => $max
    );
    $this->type = 'range';
    if(!empty($context)){
      $this->setContext($context);
    }else{
      $this->context = $context;
    }
  }

  /**
   * order
   * 
   * Ensure the range is ordered. Only do this once we have
   * a context.
   */
  private function order(){
    // We test min > max so that ranges for types that always return false for '<='
    // will not switch over every time.
    $c = $this->val['min']->applyBinaryOperator('>', $this->val['max']);
    if($c->value()){
      $this->val = array(
        'min' => $this->val['max'],
        'max' => $this->val['min']
      );
    }
  }

  /**
   * setContext
   */
  public function setContext($context){
    $this->context = $context;
    $this->val['min']->setContext($context);
    $this->val['max']->setContext($context);
    $this->order();
  }

  /**
   * asExpression
   */
  public function asExpression(){
    return '[' . $this->val['min']->toString($format) . ' .. ' . $this->val['max']->toString($format) . ']';
  }

  /**
   * asVariable
   */
  public function asVariable(){
    return array(
      'min' => $this->val['min']->asVariable(),
      'max' => $this->val['max']->asVariable()
    );
  }

  /**
   * applyUnaryOperator
   */
  public function applyUnaryOperator($o){
    if($o == 'neg'){
      $min = $this->val['min']->applyUnaryOperator('neg');
      $max = $this->val['max']->applyUnaryOperator('neg');
      return new CharacterExpressionRange($max, $min, $this->context);
    }else{
      throw new Exception(t("Operator %op cannot be applied to ranges", array(
        '%op' => $o
      )));
    }
  }

  /**
   * applyBinaryOperator
   *
   * $reverse may be true for numbers, texts and symbols which may call this directly.
   */
  public function applyBinaryOperator($o, $v, $reverse = FALSE){
    if($v->type() == 'undefined'){
      return $v->applyBinaryOperator($o, $this);
    }else if($v->type() == 'range'){
      switch($o){
        case 'in':
          $min = $this->val['min']->applyBinaryOperator('>=', $v->value());
          $max = $this->val['max']->applyBinaryOperator('<=', $v->value());
          return new CharacterExpressionNumber((int)($min->value() && $max->value()), $this->context);
        case 'not-in':
          $min = $this->val['min']->applyBinaryOperator('<', $v->value());
          $max = $this->val['max']->applyBinaryOperator('>', $v->value());
          return new CharacterExpressionNumber((int)($min->value() || $max->value()), $this->context);
        case 'is':
          $min = $this->val['min']->applyBinaryOperator('is', $v->value());
          $max = $this->val['max']->applyBinaryOperator('is', $v->value());
          return new CharacterExpressionNumber((int)($min->value() && $max->value()), $this->context);
        case 'is-not':
          $min = $this->val['min']->applyBinaryOperator('is-not', $v->value());
          $max = $this->val['max']->applyBinaryOperator('is-not', $v->value());
          return new CharacterExpressionNumber((int)($min->value() || $max->value()), $this->context);
        default:
          throw new Exception(t("Operator %op cannot be used between ranges", array(
            '%op' => $o
          )));
      }
    }else if($v->type == 'list'){
      throw new Exception(t("Operator %op cannot be used between a range and a list", array(
        '%op' => $o
      )));
    }else{
      if(($o == 'in' || $o == 'not-in') && !$reverse){
        throw new Exception(t("Operator %op cannot be used between a range and a %type", array(
          '%op' => $op,
          '%type' => $v->type()
        )));
      }else if($reverse){
        if($o == '>'){
          $o = '<=';
        }else if($o == '>='){
          $o = '<';
        }else if($o == '<'){
          $o = '>=';
        }else if($o == '<='){
          $o = '>';
        }
      }
      if($o == '+' || $o == '-' || $o == '*' || $o == '/'){
        if($reverse){
          if($v->type() == 'range'){
            $other_r = $v->value();
            $min_val = $other_r['min']->applyBinaryOperator($o, $this->val['min']);
            $max_val = $other_r['max']->applyBinaryOperator($o, $this->val['max']);
          }else{
            $min_val = $v->applyBinaryOperator($o, $this->val['min']);
            $max_val = $v->applyBinaryOperator($o, $this->val['max']);
          }
        }else{
          if($v->type() == 'range'){
            $other_r = $v->value();
            $min_val = $this->val['min']->applyBinaryOperator($o, $other_r['min']);
            $max_val = $this->val['max']->applyBinaryOperator($o, $other_r['max']);
          }else{
            $min_val = $this->val['min']->applyBinaryOperator($o, $v);
            $max_val = $this->val['max']->applyBinaryOperator($o, $v);
          }
        }
        return new CharacterExpressionRange($min_val, $max_val, $this->context);
      }
      switch($o){
        case 'is':
        case 'is-not':
          return new CharacterExpressionNumber(0, $this->context);
        case 'in':
          $min = $v->applyBinaryOperator('>=', $this->val['min']);
          $max = $v->applyBinaryOperator('<=', $this->val['max']);
          return new CharacterExpressionNumber((int)($min->value() && $max->value()), $this->context);
        case 'not-in':
          $min = $v->applyBinaryOperator('<', $this->val['min']);
          $max = $v->applyBinaryOperator('>', $this->val['max']);
          return new CharacterExpressionNumber((int)($min->value() || $max->value()), $this->context);
        case '>':
        case '>=':
        case '<':
        case '<=':
          $min = $this->val['min']->applyBinaryOperator($o, $v);
          $max = $this->val['max']->applyBinaryOperator($o, $v);
          return new CharacterExpressionNumber((int)($min->value() && $max->value()), $this->context);
        default:
          throw new Exception(t("Operator %op cannot be used between a range and a %type", array(
            '%op' => $op,
            '%type' => $v->type()
          )));
      }
    }
  }
}

/**
 * CharacterExpressionVariable
 */
class CharacterExpressionVariable extends CharacterExpressionValue{

  /**
   * __construct
   */
  public function __construct($name, $context = array()){
    $this->name = $name;
    $this->context = $context;
    $this->item = NULL;
  }

  /**
   * setContext
   */
  public function setContext($context){
    $this->context = $context;
    $this->item = NULL;
  }

  /**
   * value
   */
  public function value(){
    $this->fetchItem();
    return $this->item->value();
  }

  /**
   * type
   */
  public function type(){
    $this->fetchItem();
    return $this->item->type();
  }

  /**
   * name
   */
  public function name(){
    return $this->name;
  }

  /**
   * rename
   */
  public function rename($name){
    $this->name = $name;
  }

  /**
   * asExpression
   * 
   * Note that 'asExpression' returns the name of the variable, while
   * 'asVariable' evaluates it.
   */
  public function asExpression(){
    return '{' . $this->name . '}';
  }

  /**
   * asVariable
   */
  public function asVariable(){
    $this->fetchItem();
    return $this->item->asVariable();
  }

  /**
   * applyUnaryOperator
   */
  public function applyUnaryOperator($o){
    $this->fetchItem();
    return $this->item->applyUnaryOperator($o);
  }

  /**
   * applyBinaryOperator
  */
  public function applyBinaryOperator($o, $v, $reverse = FALSE){
    $this->fetchItem();
    return $this->item->applyBinaryOperator($o, $v, $reverse);
  }

  /**
   * fetchItem
   */
  private function fetchItem(){
    if(!empty($this->item)){return $this->item;}
    if(empty($this->context['variable_map']) || !isset($this->context['variable_map'][$this->name])){
      $this->item = new CharacterExpressionUndefined($this->context);
    }else{
      $this->item = $this->getValueObject($this->context['variable_map'][$this->name]);
    }
  }

  /**
   * getValueObject
   */
  private function getValueObject($val){
    $type = FALSE;
    if(is_array($val) && count($val) == 1 && isset($val['type'])){
      $type = $val['type'];
      $val = '';
    }else if(is_array($val) && count($val) == 2 && isset($val['type']) && isset($val['value'])){
      $type = $val['type'];
      $val = $val['value'];
    }
    if($type == 'undefined'){
      return new CharacterExpressionUndefined();
    }else if(is_object($val) && ($val instanceof CharacterExpressionValue)){
      return $val;
    }else if((is_int($val) || is_float($val)) && (!$type || $type == 'number')){
      return new CharacterExpressionNumber($val, $this->context);
    }else if(is_string($val) && (!$type || $type == 'text')){
      return new CharacterExpressionText($val, $this->context);
    }else if(is_string($val) && $type == 'symbol'){
      return new CharacterExpressionSymbol($val, $this->context);
    }else if(isset($val['min']) && isset($val['max']) && count($val) == 2 && (!$type || $type == 'range')){
      $min = $this->getValueObject($val['min']);
      $max = $this->getValueObject($val['max']);
      return new CharacterExpressionRange($min, $max, $this->context);
    }else if(is_array($val) && (!$type || $type == 'list-and' || $type = 'list-or')){
      $a = array();
      foreach($val as $v){
        $a[] = $this->getValueObject($v);
      }
      return new CharacterExpressionList($a, !$type || $type == 'list-and', $this->context);
    }else if(is_string($val) && $type == 'expr'){
      $expr = new CharacterExpressionParser($val);
      $new_val = $expr->evaluate($this->context['variable_map'], $this->context['symbol_order']);
      return $this->getValueObject($new_val);
    }else{
      throw new Exception(t("Could not work out variable type for %name", array(
        '%name' => $this->name
      )));
    }
  }
}

/**
 * CharacterExpressionUndefined
 *
 * Represent an undefined value
 *
 */
class CharacterExpressionUndefined extends CharacterExpressionValue{

  /**
   * __construct
   */
  public function __construct($context){
    parent::__construct(NULL, 'undefined', $context);
  }

  /**
   * asExpression
   *
   * Return a string representing this value as a parsable expression
   */
  public function asExpression(){
    return '<undefined>';
  }

  /**
   * asVariable
   *
   * Return this value of this object as a type which can be used as an
   * input variable
   */
  public function asVariable(){
    return array(
      'type' => 'undefined'
    );
  }

  /**
   * applyUnaryOperator
   *
   * Called when the given unrary operator is applied to this value.
   * Should return a new value or raise an exception.
   */
  public function applyUnaryOperator($o){
    return new CharacterExpressionNumber(0, $this->context);
  }

  /**
   * applyBinaryOperator
   *
   * Called when the given binary operator is applied to this value,
   * such that this value is on the left of the expression.
   * Should return a new value or raise an exception.
  */
  public function applyBinaryOperator($o, $v){
    if($o == 'is-not' || $o == 'not-in'){
      return new CharacterExpressionNumber(1, $this->context);
    }else if($o == 'is' || $o == 'in' || $o == '<' || $o == '<=' || $o == '>' || $o == '>='){
      return new CharacterExpressionNumber(0, $this->context);
    }else{
      return new CharacterExpressionUndefined();
    }
  }
}
