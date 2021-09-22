<?php

/**
 *
 */
class biblio_handler_field_contributor extends biblio_handler_field {

  /**
   *
   */
  public function init(&$view, &$options) {
    module_load_include('inc', 'biblio', 'includes/biblio.contributors');
    parent::init($view, $options);
    $this->multiple = TRUE;
    $this->additional_fields['vid'] = array('table' => 'biblio', 'field' => 'vid');
  }

  /**
   *
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['auth_category'] = array('default' => 1);
    $options['auth_links'] = array('default' => 0);
    $options['initialize'] = array('default' => TRUE);
    $options['initialize_with'] = array('default' => '.');
    $options['initialize_with_hyphen'] = array('default' => FALSE);
    $options['separator'] = array('default' => '; ');
    $options['sort_separator'] = array('default' => ', ');
    $options['short_form'] = array('default' => 0);
    $options['name_order'] = array('default' => 'first-last');

    return $options;
  }

  /**
   *
   */
  public function options_form(&$form, &$form_state) {
    $form['auth_category'] = array(
      '#type' => 'select',
      '#title' => t('Category of Author'),
      '#default_value' => $this->options['auth_category'],
      '#options' => array(
        0 => t('All categories'),
        1 => t('Primary'),
        2 => t('Secondary'),
        3 => t('Tertiary'),
        4 => t('Subsidiary'),
        5 => t('Corporate/Institutional'),
      ),

    );
    $form['formatting'] = array(
      '#type' => 'fieldset',
      '#title' => t('Author format'),
      '#collapsible' => TRUE,
    );
    $form['name_order'] = array(
      '#type' => 'select',
      '#title' => t('Name order'),
      '#default_value' => $this->options['name_order'],
      '#options' => array(
        'first-last' => t('First name first'),
        'last-first' => t('Last name first'),
      ),
      '#description' => t('The order that first and last names appear for each author.'),
      '#fieldset' => 'formatting',
    );
    $form['initialize'] = array(
      '#type' => 'checkbox',
      '#title' => t('Shorten given names'),
      '#default_value' => $this->options['initialize'],
      '#description' => t('Shorten given names to single initial each.'),
      '#fieldset' => 'formatting',
    );
    $form['short_form'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show only "family" names'),
      '#default_value' => $this->options['short_form'],
      '#description' => t('Show only family name, no first name or initials.'),
      '#fieldset' => 'formatting',
    );
    $form['separators'] = array(
      '#type' => 'fieldset',
      '#title' => t('Separators'),
      '#collapsible' => TRUE,
    );
    $form['initialize_with'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#title' => t('Initial separator'),
      '#default_value' => $this->options['initialize_with'],
      '#description' => t('Enter the character (if any) which will be used to separate the initials.'),
      '#fieldset' => 'separators',
    );
    $form['sort_separator'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#title' => t('Sort separator'),
      '#default_value' => $this->options['sort_separator'],
      '#description' => t('Enter the character which will be used to separate the last name from the first name (or initials) when displayed last name first (Smith, John).'),
      '#fieldset' => 'separators',
    );
    $form['separator'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#title' => t('Author separator'),
      '#default_value' => $this->options['separator'],
      '#required' => TRUE,
      '#description' => t('Enter the character which will be used to separate the authors (Smith, John; Doe, Jane).'),
      '#fieldset' => 'separators',
    );
    parent::options_form($form, $form_state);

  }

  /**
   *
   */
  public function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['vid'];
  }

  /**
   *
   */
  public function post_execute(&$values) {
    $vids = array();
    $this->items = array();
    $filter = '';

    foreach ($values as $result) {
      // Don't add empty results to the array.
      if (isset($this->aliases['vid']) && !empty($result->{$this->aliases['vid']})) {
        $vids[] = $result->{$this->aliases['vid']};
      }
    }

    if (count($vids)) {
      $this->items = biblio_load_contributors_multiple($vids, $this->options['auth_category']);
    }
  }

  /**
   *
   */
  public function render($values) {
    parent::set_label($values);
    $vid = $values->{$this->field_alias};
    if (!isset($this->items[$vid])) {
      return NULL;
    }
    return $this->render_contriubutors($this->items[$vid]);
  }

  /**
   *
   */
  public function render_contriubutors($contributors) {
    $authors = array();
    if (!isset($this->alnum)) {
      list($this->alnum, $this->alpha, $this->cntrl, $this->dash,
          $this->digit, $this->graph, $this->lower, $this->print,
          $this->punct, $this->space, $this->upper, $this->word,
          $this->patternModifiers) = $this->get_regex_patterns();
    }

    foreach ($contributors as $rank => $author) {
      $author = (object) $author;
      if ($author->literal == 1) {
        $authors[] = $author->name;
      }
      else {
        if (!empty($author->firstname) && $this->options['initialize'] == 1) {
          $author->firstname = preg_replace("/([$this->upper])[$this->lower]+/$this->patternModifiers", '\\1', $author->firstname);
          $author->firstname = preg_replace("/(?<=[-$this->upper]) +(?=[-$this->upper])/$this->patternModifiers", "", $author->firstname);
          $author->initials = $author->firstname . $author->initials;
        }
        if (isset($author->initials)) {
          // Within initials, remove any dots:
          $author->initials = preg_replace("/([$this->upper])\.+/$this->patternModifiers", "\\1", $author->initials);
          // Within initials, remove any spaces *between* initials:
          $author->initials = preg_replace("/(?<=[-$this->upper]) +(?=[-$this->upper])/$this->patternModifiers", "", $author->initials);
          if ($this->options['initialize_with_hyphen'] === FALSE) {
            $author->initials = preg_replace("/-/", '', $author->initials);
          }
          // Within initials, add a space after a hyphen, but only if ...
          // ... the delimiter that separates initials ends with a space.
          if (preg_match("/ $/", $this->options['initialize_with'])) {
            $author->initials = preg_replace("/-(?=[$this->upper])/$this->patternModifiers", "- ", $author->initials);
          }
          // then, separate initials with the specified delimiter:
          $init_sep = $this->options['initialize_with'];
          $author->initials = preg_replace("/([$this->upper])(?=[^$this->lower]+|$)/$this->patternModifiers", "\\1$init_sep", $author->initials);

          // $shortenInitials = (isset($options['numberOfInitialsToKeep)) ? $options['numberOfInitialsToKeep : FALSE;
          //      if ($shortenInitials) $given = drupal_substr($given, 0, $shortenInitials);.
          if ($this->options['initialize'] == 1) {
            $author->firstname = $author->initials;
            // If ($shortenInitials) $author->firstname = drupal_substr($author->firstname, 0, $shortenInitials);.
          }
          elseif (!empty($author->firstname)) {
            $author->firstname = $author->firstname . ' ' . $author->initials;
          }
          elseif (empty($author->firstname)) {
            $author->firstname = $author->initials;
          }
        }

        if (isset($author->lastname)) {
          if ($this->options['short_form'] == 1) {
            $authors[] = $author->lastname;
          }
          else {
            switch ($this->options['name_order']) {
              case 'last-first':
                $authors[] = $author->lastname . $this->options['sort_separator'] . $author->firstname;
                break;

              default:
                $authors[] = $author->firstname . ' ' . $author->lastname;
            }
          }
        }
      }
    }

    return implode($this->options['separator'], $authors);
  }

  /**
   *
   */
  public function get_regex_patterns() {
    // Checks if PCRE is compiled with UTF-8 and Unicode support.
    if (!@preg_match('/\pL/u', 'a')) {
      // Probably a broken PCRE library.
      return $this->get_latin1_regex();
    }
    else {
      // Unicode safe filter for the value.
      return $this->get_utf8_regex();
    }
  }

  /**
   *
   */
  public function get_latin1_regex() {
    $alnum = "[:alnum:]ÄÅÁÀÂÃÇÉÈÊËÑÖØÓÒÔÕÜÚÙÛÍÌÎÏÆäåáàâãçéèêëñöøóòôõüúùûíìîïæÿß";
    // Matches ISO-8859-1 letters:
    $alpha = "[:alpha:]ÄÅÁÀÂÃÇÉÈÊËÑÖØÓÒÔÕÜÚÙÛÍÌÎÏÆäåáàâãçéèêëñöøóòôõüúùûíìîïæÿß";
    // Matches ISO-8859-1 control characters:
    $cntrl = "[:cntrl:]";
    // Matches ISO-8859-1 dashes & hyphens:
    $dash = "-–";
    // Matches ISO-8859-1 digits:
    $digit = "[\d]";
    // Matches ISO-8859-1 printing characters (excluding space):
    $graph = "[:graph:]ÄÅÁÀÂÃÇÉÈÊËÑÖØÓÒÔÕÜÚÙÛÍÌÎÏÆäåáàâãçéèêëñöøóòôõüúùûíìîïæÿß";
    // Matches ISO-8859-1 lower case letters:
    $lower = "[:lower:]äåáàâãçéèêëñöøóòôõüúùûíìîïæÿß";
    // Matches ISO-8859-1 printing characters (including space):
    $print = "[:print:]ÄÅÁÀÂÃÇÉÈÊËÑÖØÓÒÔÕÜÚÙÛÍÌÎÏÆäåáàâãçéèêëñöøóòôõüúùûíìîïæÿß";
    // Matches ISO-8859-1 punctuation:
    $punct = "[:punct:]";
    // Matches ISO-8859-1 whitespace (separating characters with no visual representation):
    $space = "[\s]";
    // Matches ISO-8859-1 upper case letters:
    $upper = "[:upper:]ÄÅÁÀÂÃÇÉÈÊËÑÖØÓÒÔÕÜÚÙÛÍÌÎÏÆ";
    // Matches ISO-8859-1 "word" characters:
    $word = "_[:alnum:]ÄÅÁÀÂÃÇÉÈÊËÑÖØÓÒÔÕÜÚÙÛÍÌÎÏÆäåáàâãçéèêëñöøóòôõüúùûíìîïæÿß";
    // Defines the PCRE pattern modifier(s) to be used in conjunction with the above variables:
    // More info: <http://www.php.net/manual/en/reference.pcre.pattern.modifiers.php>
    $patternModifiers = "";

    return array($alnum, $alpha, $cntrl, $dash, $digit, $graph, $lower,
      $print, $punct, $space, $upper, $word, $patternModifiers,
    );

  }

  /**
   *
   */
  public function get_utf8_regex() {
    // Matches Unicode letters & digits:
    // Unicode-aware equivalent of "[:alnum:]".
    $alnum = "\p{Ll}\p{Lu}\p{Lt}\p{Lo}\p{Nd}";
    // Matches Unicode letters:
    // Unicode-aware equivalent of "[:alpha:]".
    $alpha = "\p{Ll}\p{Lu}\p{Lt}\p{Lo}";
    // Matches Unicode control codes & characters not in other categories:
    // Unicode-aware equivalent of "[:cntrl:]".
    $cntrl = "\p{C}";
    // Matches Unicode dashes & hyphens:
    $dash = "\p{Pd}";
    // Matches Unicode digits:
    // Unicode-aware equivalent of "[:digit:]".
    $digit = "\p{Nd}";
    // Matches Unicode printing characters (excluding space):
    // Unicode-aware equivalent of "[:graph:]".
    $graph = "^\p{C}\t\n\f\r\p{Z}";
    // Matches Unicode lower case letters:
    // Unicode-aware equivalent of "[:lower:]".
    $lower = "\p{Ll}\p{M}";
    // Matches Unicode printing characters (including space):
    // same as "^\p{C}", Unicode-aware equivalent of "[:print:]".
    $print = "\P{C}";
    // Matches Unicode punctuation (printing characters excluding letters & digits):
    // Unicode-aware equivalent of "[:punct:]".
    $punct = "\p{P}";
    // Matches Unicode whitespace (separating characters with no visual representation):
    // Unicode-aware equivalent of "[:space:]".
    $space = "\t\n\f\r\p{Z}";
    // Matches Unicode upper case letters:
    // Unicode-aware equivalent of "[:upper:]".
    $upper = "\p{Lu}\p{Lt}";
    // Matches Unicode "word" characters:
    // Unicode-aware equivalent of "[:word:]" (or "[:alnum:]" plus "_")
    $word = "_\p{Ll}\p{Lu}\p{Lt}\p{Lo}\p{Nd}";
    // Defines the PCRE pattern modifier(s) to be used in conjunction with the above variables:
    // More info: <http://www.php.net/manual/en/reference.pcre.pattern.modifiers.php>
    // the "u" (PCRE_UTF8) pattern modifier causes PHP/PCRE to treat pattern strings as UTF-8.
    $patternModifiers = "u";
    return array($alnum, $alpha, $cntrl, $dash, $digit, $graph, $lower,
      $print, $punct, $space, $upper, $word, $patternModifiers,
    );
  }

}
