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
 * - Symbols, which should be enclosed in single quotes. If a symbol is build as
 *   'label:id' then only the 'id' part is taken into consideration in tests. So 
 *   for instance ('hello:1' is 'world:1') is true, while ('hello:1' is 'hello:2') 
 *   is not. If symbols are constructed without an id, then they are treated as
 *   text.
 *     
 * - Variables, which should be enclosed in curly brackets, eg. {height}
 *   If a variable is build as {label:id} then when evaluating the expression
 *   only the id part is looked up in the variable list ;
 * 
 * - Lists of expressions, enclosed in square brackets and with expressions separated
 *   by commas, eg. ["green", "blue", "yellow"] ;
 *   
 * - Ranges, enclosed in square brackets and using the '..' operator, eg.
 *   [1 .. 5]. Ranges of strings or arrays are not permitted - however it is
 *   possible to build ranges of symbols such as ['yellow' .. 'green'] provided
 *   the ordering of symbols is passed in when evaluating the expression ;
 *   
 * - Mathematical operators which act on numbers: +,-,*,/
 * 
 * - Comparaison operators: <, <=, >, >=, is, isn't. Comparaison operators
 *   return '1' for true and '0' for false ;
 *   
 * - List and range test operators: in, not-in (eg. ("blue" in ["blue", "red"])
 *   or (2 in [1 .. 5]) ;
 *   
 * - Boolean operators: and, or, not. These will cast their operands such that
 *   non-zero numbers are true, and everything else is false.
 *   
 * - Parenthesized expressions
 * 
 * Some notes:
 *   
 * - Operations between numbers and ranges apply to both end of the
 *   range - so adding a number and a range returns the range with
 *   the value added to both min and max.
 *   
 *   However testing for equality between a value and a range is always FALSE.
 *   
 * - Lists are not ordered, and the + operator can be used to add items to a list ;
 * 
 * - The fact boolean return 0/1 can be used to construct simple tests:
 *   {var1} * ({var3} is 'state1:1') + {var2} * ({var3} is 'state2:2')
 *   Though this only works for numeric values.
 *   
 */
class CharacterExpressionParser{
  /* Exception Error codes */
  const MISSING_VARIABLE = 1;
  /* Formatting style for output */
  const FORMAT_EXPRESSION = 1;

  const FORMAT_EDITOR = 2;
  // Operators in order of precedence, with associated arity. Unariy "-" is allowed
  // but replaced at parse time with "neg". "," and ".." are parsed as operators,
  // but internally used to build lists and ranges.
  private $operators = array(
    'neg' => 1,
    'not' => 1,
    '*' => 2,
    '/' => 2,
    '+' => 2,
    '-' => 2,
    '<' => 2,
    '<=' => 2,
    '>' => 2,
    '>=' => 2,
    'isn\'t' => 2,
    'is' => 2,
    'in' => 2,
    'not-in' => 2,
    'and' => 2,
    'or' => 2,
    ',' => 2,
    '..' => 2
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
   * Evaluate the value of the expression with the given variables
   */
  public function evaluate($variable_map = array(), $symbol_order = array(), $format = CharacterExpressionParser::FORMAT_EXPRESSION){
    $val = $this->evaluateRecursive($this->tree, array(
      'variable_map' => $variable_map,
      'symbol_order' => $symbol_order
    ));
    return $val->toString($format);
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
      if($this->operators[$tree['operator']] == 1){
        return $ops[0]->applyUnaryOperator($tree['operator']);
      }else if($tree['operator'] == ','){
        if($ops[0]->type == 'list'){
          return $ops[0]->applyBinaryOperator('+', $ops[1]);
        }else if($ops[1]->type == 'list'){
          return $ops[1]->applyBinaryOperator('+', $ops[0]);
        }else{
          return new CharacterExpressionList(array(
            $ops[0],
            $ops[1]
          ), $this->context);
        }
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
   * getExpression
   * 
   * Generate a string represenation of the expression, mapping variables
   * and symbols using the given maps.
   */
  public function getExpression($var_map = array(), $sym_map = array()){
    $out = '';
    $tokens = $this->tokens;
    foreach($this->expression as $item){
      if($item == 'o'){
        $out .= array_shift($tokens);
      }else if($item == 'v'){
        $v = array_shift($tokens);
        $out .= $v->toString();
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
      }
      // Replace '-' with 'neg' if required
      if($current == 'o' && $current_token == '-'){
        if(!$prev || $this->isSymbol($prev, 'start') || $prev == 'o'){
          $current_token = 'neg';
        }
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
      if($next == 'o' && $next_token != "-" && $this->operators[$next_token] == 2){return t('Symbol "%sym" cannot be followed by binary operator %op', array(
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
    }else if($current == 'o' && $this->operators[$current_token] == 2){
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
      foreach($this->operators as $operator => $arity){
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
            if($arity == 1){
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
   * toString
   * 
   * Generate the result to be returned to the caller for this item
   */
  public function toString($format = CharacterExpressionParser::FORMAT_EXPRESSION){
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
    parent::__construct($v, 'number', $context);
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
    if($v->type() == 'number'){
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
          return new CharacterExpressionNumber((int)($this->val == $v->value()), $this->context);
        case 'isn\'t':
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
    if($v->type() == 'text'){
      switch($o){
        case '+':
          return new CharacterExpressionText($this->val . $v->value(), $this->context);
        case 'is':
          return new CharacterExpressionNumber((int)($this->val == $v->value()), $this->context);
        case 'isn\'t':
          return new CharacterExpressionNumber((int)($this->val != $v->value()), $this->context);
        default:
          throw new Exception(t("Operator %op cannot be used between text strings", array(
            '%op' => $o
          )));
      }
    }else if($v->type() == 'symbol'){
      switch($o){
        case 'is':
          return new CharacterExpressionNumber((int)($this->val == $v->stringValue()), $this->context);
        case 'isn\'t':
          return new CharacterExpressionNumber((int)($this->val != $v->stringValue()), $this->context);
        default:
          throw new Exception("Operator $o cannot be used between a text string and a symbol");
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
    $this->type = 'symbol';
    $this->context = $context;
    if(preg_match('/^(.*):([^:]+)$/', $val, $matches)){
      $this->val = $matches[2];
      $this->string = $matches[1];
    }else{
      $this->val = $val;
      $this->string = $val;
    }
  }

  /**
   * stringValue
   */
  public function stringValue(){
    return $this->string;
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
          return new CharacterExpressionNumber((int)($this->val == $v->value()), $this->context);
        case 'isn\'t':
          return new CharacterExpressionNumber((int)($this->val != $v->value()), $this->context);
        default:
          throw new Exception(t("Operator %op cannot be used between symbols", array(
            '%op' => $o
          )));
      }
    }else if($v->type() == 'text'){
      switch($o){
        case 'is':
          return new CharacterExpressionNumber((int)($this->string == $v->value()), $this->context);
        case 'isn\'t':
          return new CharacterExpressionNumber((int)($this->string != $v->value()), $this->context);
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
  public function __construct($val, $context){
    parent::__construct($val, 'list', $context);
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
   * toString
   */
  public function toString($format = CharacterExpressionParser::FORMAT_EXPRESSION){
    $a = array();
    foreach($this->val as $item){
      $a[] = $item->evaluate();
    }
    if($format == CharacterExpressionParser::FORMAT_EXPRESSION){
      return '[' . implode(', ', $a) . ']';
    }else{
      return implode(',', $a);
    }
  }

  /**
   * applyUnaryOperator
   */
  public function applyUnaryOperator($o){
    if($o == 'neg'){
      $o = '-';
    }
    throw new Exception(t("Operator %op cannot be applied to lists", array(
      '%op' => $o
    )));
  }

  /**
   * applyBinaryOperator
   *
   * $reverse may be true for numbers, texts and symbols which may call this directly.
   */
  public function applyBinaryOperator($o, $v, $reverse = FALSE){
    if($v->type() == 'list'){
      $a1 = array();
      $a2 = array();
      foreach($this->val as $item){
        $a1[] = $item->value();
      }
      foreach($v->value() as $item){
        $a2[] = $item->value();
      }
      switch($o){
        case '+':
          return new CharacterExpressionList(array_merge($this->val, $v->value()), $this->context);
        case 'is':
          return new CharacterExpressionNumber((int)(count(array_diff($a1, $a2)) == 0 && count(array_diff($a2, $a1)) == 0), $this->context);
          ;
        case 'isn\'t':
          return new CharacterExpressionNumber((int)(count(array_diff($a1, $a2)) > 0), $this->context);
        case 'in':
          return new CharacterExpressionNumber((int)(count(array_diff($a1, $a2)) == 0), $this->context);
        case 'not-in':
          return new CharacterExpressionNumber((int)(count(array_diff($a1, $a2)) > 0), $this->context);
        default:
          throw new Exception(t("Operator %op cannot be used between lists", array(
            '%op' => $o
          )));
      }
    }else if($v->type() == 'range'){
      $range = $v->value();
      $in = FALSE;
      foreach($this->val as $item){
        $ismin = $item->applyBinaryOperator('>=', $range['min']);
        $ismax = $item->applyBinaryOperator('<=', $range['max']);
        $in = $in && $ismin->value() && $ismax->value();
        if(!$in){
          break;
        }
      }
      switch($o){
        case 'in':
          return new CharacterExpressionNumber((int)$in, $this->context);
        case 'not-in':
          return new CharacterExpressionNumber((int)!$in, $this->context);
        default:
          throw new Exception(t("Operator %op cannot be used between a list and a range", array(
            '%op' => $o
          )));
      }
    }else{
      if($o == 'in' || $o == 'not-in'){
        if(!$reverse && $op != '+'){throw new Exception(t("Operator %op cannot be used between a list and a %type", array(
            '%op' => $o,
            '%type' => $v->type()
          )));}
        $a1 = array();
        foreach($this->val as $item){
          $a1[] = $item->value();
        }
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
      switch($o){
        case '+':
          return new CharacterExpressionList(array_merge($this->val, array(
            $v
          )), $this->context);
        case 'in':
          return new CharacterExpressionNumber((int)in_array($v->value(), $a1), $this->context);
        case 'not-in':
          return new CharacterExpressionNumber((int)!in_array($v->value(), $a1), $this->context);
        case '>':
        case '>=':
        case '<':
        case '<=':
          foreach($this->val as $item){
            $c = $item->applyBinaryOperator($o, $v);
            if(!$c->value()){return new CharacterExpressionNumber(0, $this->context);}
          }
          return new CharacterExpressionNumber(1, $this->context);
        default:
          throw new Exception(t("Operator %op cannot be used between an array and a %type", array(
            '%op' => $o,
            '%type' => $v->type()
          )));
      }
    }
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
   * toString
   */
  public function toString($format = CharacterExpressionParser::FORMAT_EXPRESSION){
    if($format == CharacterExpressionParser::FORMAT_EXPRESSION){
      return '[' . $this->val['min']->toString($format) . ' .. ' . $this->val['max']->toString($format) . ']';
    }else{
      return $this->val['min']->toString($format) . ' - ' . $this->val['max']->toString($format);
    }
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
    if($v->type == 'range'){
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
        case 'isn\'t':
          $min = $this->val['min']->applyBinaryOperator('isn\'t', $v->value());
          $max = $this->val['max']->applyBinaryOperator('isn\'t', $v->value());
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
        case 'isn\'t':
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
   * toString
   */
  public function toString(){
    $this->fetchItem();
    return $this->item->toString();
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
    if(preg_match('/:([^:]+)$/', $this->name, $matches)){
      $name = $matches[1];
    }else{
      $name = $this->name;
    }
    if(empty($this->context['variable_map']) || !isset($this->context['variable_map'][$name])){throw new Exception(t("Value for variable %name is not defined.", array(
        '%name' => $this->name
      )), CharacterExpressionParser::MISSING_VARIABLE);}
    $this->item = $this->getValueObject($this->context['variable_map'][$name]);
  }

  /**
   * getValueObject
   */
  private function getValueObject($val){
    $type = FALSE;
    if(is_array($val) && count($val) == 2 && isset($val['type']) && isset($val['value'])){
      $type = $val['type'];
      $val = $val['value'];
    }
    if(is_numeric($val) && (!$type || $type == 'number')){
      return new CharacterExpressionNumber($val, $this->context);
    }else if(is_string($val) && (!$type || $type == 'text')){
      return new CharacterExpressionText($val, $this->context);
    }else if(is_string($val) && $type == 'symbol'){
      return new CharacterExpressionSymbol($val, $this->context);
    }else if(isset($val['min']) && isset($val['max']) && count($val) == 2 && (!$type || $type == 'range')){
      $min = $this->getValueObject($val['min']);
      $max = $this->getValueObject($val['max']);
      return new CharacterExpressionRange($min, $max, $this->context);
    }else if(is_array($val) && (!$type || $type == 'list')){
      $a = array();
      foreach($val as $v){
        $a[] = $this->getValueObject($v);
      }
      return new CharacterExpressionList($a, $this->context);
    }else{
      throw new Exception(t("Could not work out variable type for %name", array(
        '%name' => $this->name
      )));
    }
  }
}