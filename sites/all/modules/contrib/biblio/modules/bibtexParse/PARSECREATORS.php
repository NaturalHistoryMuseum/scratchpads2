<?php
class Creators extends PARSECREATORS
{
  protected   $authors            = array();
  private   $existing_authors   = array();
    protected    $typeMap            = array();
    private   $md5                = array();

  function Creators($init = NULL)
  {
    $this->buildTypeMap();
    if (is_array($init))
    {
      $this->setCreators($init);
    }elseif (is_numeric($init))
    {
      $this->loadCreators($init);
    }

  }

    function buildTypeMap()
    {
      $result = db_query("SELECT * FROM {biblio_contributor_type} ;");
      while ($type = db_fetch_object($result))
      {
        $this->typeMap[$type->type] = $type->ctid;
      }
    }
  function getCreatorByName($name)
  {
    $result = db_query('SELECT *
                    FROM {biblio_contributor_data}
                    WHERE lastname RLIKE "[[:<:]]%s[[:>:]]" ', $name);
  }

  function getCreatorCount()
  {
    return count($this->authors);
  }

  function getCreatorString()
  {
    foreach ($this->authors as $key => $author)
    {
      $author_array[$author['rank']] =  $author['firstname'] .' '. $author['initials'].' '.$author['lastname'];
    }
    ksort($author_array);

    return implode(',  ' , $author_array);
  }


    private
    function loadMD5()
    {
      $result = db_query('SELECT md5,cid  FROM {biblio_contributor_data} ');
      while ($row = db_fetch_array($result))
      {
        $this->md5[$row['cid']] = $row['md5'];
      }
    }

    public
  function loadCreators($vid)
  {
    $query = 'SELECT bcd.lastname, bcd.firstname, bcd.initials,
             bcd.affiliation, bct.type, bc.rank
          FROM    {biblio_contributor} bc,
           {biblio_contributor_data} bcd,
           {biblio_contributor_type} bct
          WHERE bc.vid = %d
             AND bc.cid = bcd.cid
             AND  bc.ctid = bct.ctid
          ORDER BY bc.ctid ASC, bc.rank ASC;';

    $result = db_query($query, array($vid));
    while($creator = db_fetch_array($result))
    {
      $this->authors[] = $creator;
    }

  }

  public
  function saveCreators($nid, $vid)
  {
    if (!empty($this->authors))
    {
      $this->loadMD5();
      db_query('DELETE FROM {biblio_contributor} WHERE nid = %d AND vid = %d', $nid, $vid);
      foreach ($this->authors as $rank => $author)
      {
        if (empty($author['cid']) && !empty($this->md5)) $author['cid'] = array_search($author['md5'], $this->md5);
      if (empty($author['cid']) )
      {
        drupal_write_record('biblio_contributor_data', $author);
        $cid = db_last_insert_id('biblio_contributor_data', 'cid');
      }else
      {
        $cid = $author['cid'];
      }

      $link_array = array('nid' => $nid, 'vid' => $vid,
                        'cid' => $cid, 'rank' => $rank,
                        'ctid' => $author['type']);

      drupal_write_record('biblio_contributor', $link_array );

      }
    }
  }


  function getAuthorArray()
  {
    return $this->authors;
  }

  function getAuthor($rank)
  {
    return $this->authors[$rank];
  }

/**
 * update object with an array of authors
 *
 * @param $authors
 *   an array containing two keys "name" and "type"
 *   the name is the full name of the contributor which will be parsed into
 *   component pieces, and type contains a string indicating the author type
 */
  function setCreators($authors)
  {
    foreach ($authors as $author) {
      if (strlen(trim($author['name'])))
      {
        $this->authors[] = $this->parseAuthor($author['name'], $author['type']);
      }
    }
  }

  function setCreator($author, $type = 'author')
  {
    $this->authors[] = $this->parseAuthor($author, $type);
  }



}

/*
Released through http://bibliophile.sourceforge.net under the GPL licence.
Do whatever you like with this -- some credit to the author(s) would be appreciated.

A collection of PHP classes to manipulate bibtex files.

If you make improvements, please consider contacting the administrators at bibliophile.sourceforge.net so that your improvements can be added to the release package.

Mark Grimshaw 2004/2005
http://bibliophile.sourceforge.net

28/04/2005 - Mark Grimshaw.
  Efficiency improvements.

11/02/2006 - Daniel Reidsma.
  Changes to preg_matching to account for Latex characters in names such as {\"{o}}
*/
// For a quick command-line test (php -f PARSECREATORS.php) after installation, uncomment these lines:

/***********************
  $authors = "Mark \~N. Grimshaw and Bush III, G.W. & M. C. H{\\'a}mmer Jr. and von Frankenstein, Ferdinand Cecil, P.H. & Charles Louis Xavier Joseph de la Vallee P{\\\"{o}}ussin";
  $creator = new PARSECREATORS();
  $creatorArray = $creator->parse($authors);
  print_r($creatorArray);
***********************/

class PARSECREATORS
{
  function PARSECREATORS()
  {
  }

  function parse($input, $type = 'author')
  {
    $input = trim($input);
      // split on ' and '
    $authorArray = preg_split("/\s(and|&)\s/i", $input);
    return $this->parseArray($authorArray, $type);
  }

  function parseArray($authorArray, $type = 'author')
  {
    foreach ($authorArray as $author)
    {
      $this->authors[]  = $this->parseAuthor($author, $type);
    }
  }
/* Create writer arrays from bibtex input.
'author field can be (delimiters between authors are 'and' or '&'):
1. <first-tokens> <von-tokens> <last-tokens>
2. <von-tokens> <last-tokens>, <first-tokens>
3. <von-tokens> <last-tokens>, <jr-tokens>, <first-tokens>
*/
  function parseAuthor($value, $type = 'author')
  {
    $appellation = $prefix = $surname = $firstname = $initials = '';
    $this->prefix = array();
    $author = explode(",", preg_replace("/\s{2,}/", ' ', trim($value)));
    $size = sizeof($author);
// No commas therefore something like Mark Grimshaw, Mark Nicholas Grimshaw, M N Grimshaw, Mark N. Grimshaw
    if ($size == 1)
    {
// Is complete surname enclosed in {...}, unless the string starts with a backslash (\) because then it is
// probably a special latex-sign..
// 2006.02.11 DR: in the last case, any NESTED curly braces should also be taken into account! so second
// clause rules out things such as author="a{\"{o}}"
//
            if (preg_match("/(.*) {([^\\\].*)}/", $value, $matches) &&
         !(preg_match("/(.*) {\\\.{.*}.*}/", $value, $matches2)))
      {
        $author = split(" ", $matches[1]);
        $surname = $matches[2];
      }
      else
      {
        $author = split(" ", $value);
// last of array is surname (no prefix if entered correctly)
        $surname = array_pop($author);
      }
    }
// Something like Grimshaw, Mark or Grimshaw, Mark Nicholas  or Grimshaw, M N or Grimshaw, Mark N.
    else if ($size == 2)
    {
// first of array is surname (perhaps with prefix)
      list($surname, $prefix) = $this->grabSurname(array_shift($author));
    }
// If $size is 3, we're looking at something like Bush, Jr. III, George W
    else
    {
// middle of array is 'Jr.', 'IV' etc.
      $appellation = join(' ', array_splice($author, 1, 1));
// first of array is surname (perhaps with prefix)
      list($surname, $prefix) = $this->grabSurname(array_shift($author));
    }
    $remainder = join(" ", $author);
    list($firstname, $initials) = $this->grabFirstnameInitials($remainder);
    if (!empty($this->prefix))
      $prefix = join(' ', $this->prefix);
    $surname = $surname . ' ' . $appellation;
    $creator = array('firstname' => utf8_encode(trim($firstname)), 'initials' => utf8_encode(trim($initials)), 'lastname' => utf8_encode(trim($surname)), 'prefix' => trim($prefix));
    if (isset($creator))
    {
      $creator['type'] = $this->typeMap[$type];
      $creator['md5']  = $this->md5sum($creator);
      return $creator;
    }
    return FALSE;
  }

    function md5sum($creator)
    {
      $string = $creator['firstname'].$creator['initials'].$creator['lastname'];
    $string = str_replace(' ', '', drupal_strtolower($string));

       return  md5($string);
    }
// grab firstname and initials which may be of form "A.B.C." or "A. B. C. " or " A B C " etc.
  function grabFirstnameInitials($remainder)
  {
    $firstname = $initials = '';
    $array = split(" ", $remainder);
    foreach ($array as $value)
    {
      $firstChar = substr($value, 0, 1);
      if ((ord($firstChar) >= 97) && (ord($firstChar) <= 122))
        $this->prefix[] = $value;
      else if (preg_match("/[a-zA-Z]{2,}/", trim($value)))
        $firstnameArray[] = trim($value);
      else
        $initialsArray[] = str_replace(".", " ", trim($value));
    }
    if (isset($initialsArray))
    {
      foreach ($initialsArray as $initial)
        $initials .= ' ' . trim($initial);
    }
    if (isset($firstnameArray))
      $firstname = join(" ", $firstnameArray);
    return array($firstname, $initials);
  }
// surname may have title such as 'den', 'von', 'de la' etc. - characterised by first character lowercased.  Any
// uppercased part means lowercased parts following are part of the surname (e.g. Van den Bussche)
  function grabSurname($input)
  {
    $surnameArray = split(" ", $input);
    $noPrefix = $surname = FALSE;
    foreach ($surnameArray as $value)
    {
      $firstChar = substr($value, 0, 1);
      if (!$noPrefix && (ord($firstChar) >= 97) && (ord($firstChar) <= 122))
        $prefix[] = $value;
      else
      {
        $surname[] = $value;
        $noPrefix = TRUE;
      }
    }
    if ($surname)
      $surname = join(" ", $surname);
    if (isset($prefix))
    {
      $prefix = join(" ", $prefix);
      return array($surname, $prefix);
    }
    return array($surname, FALSE);
  }
}

