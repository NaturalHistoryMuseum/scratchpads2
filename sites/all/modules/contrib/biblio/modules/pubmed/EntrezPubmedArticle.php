<?php
/**
 * @file EntrezPubmedArticle.php
 * Provides a class for handling PubMed articles retrieved with EFetch.
 * Orginally writen by Stefan Freudenberg
 */

class BiblioEntrezPubmedArticle
{
  private $article;

  private $id;

  private $md5;

  private $biblio = array();

  /**
   * Stores the given SimpleXMLElement the PubMed ID and the md5 hash of the
   * serialized XML.
   *
   * @param $pubmedArticle
   *   a PubmedArticle element
   */
  public function __construct(SimpleXMLElement $pubmedArticle = NULL)
  {
    if ($pubmedArticle) {
      $this->setArticle($pubmedArticle);
    }
  }

  /**
   * Returns the PubMed ID of the article.
   *
   * @return int
   */
  public function setArticle(SimpleXMLElement $pubmedArticle)
  {
    $this->biblio = array();
    $this->article = $pubmedArticle->MedlineCitation;
    $this->pubmeddata = $pubmedArticle->PubmedData;
    $this->id = (int)$pubmedArticle->MedlineCitation->PMID;
    $this->md5 = md5($pubmedArticle->asXML());
    return $this;
  }
  public function getId()
  {
    return $this->id;
  }

  /**
   * Returns the md5 hash of the serialized XML.
   *
   * @return string
   */
  public function getMd5()
  {
    return $this->md5;
  }

  public function getBiblioAsObject() {
    return (object)$this->getBiblio();
  }

  /**
   * Returns article elements as an associative array suitable for import into
   * a biblio node.
   *
   * @return array
   */
  public function getBiblio()
  {
    if (empty($this->biblio)) {
        if (variable_get('biblio_auto_citekey', 1) ) {
          $citekey = '';
        }
        else {
          $citekey = $this->id;
        }

        // Attempts to extract the name of the journal from MedlineTA if
        // available.
        if (!empty($this->article->MedlineJournalInfo->MedlineTA)) {
          $journal = (string)$this->article->MedlineJournalInfo->MedlineTA;
        }
        elseif (!empty($this->article->Article->Journal->ISOAbbreviation)) {
          $journal = (string)$this->article->Article->Journal->ISOAbbreviation;
        }
        else {
          $journal = (string)$this->article->Article->Journal->Title;
        }

        $this->biblio = array(
        'title'           => (string)$this->article->Article->ArticleTitle,
        'biblio_citekey'  => $citekey,
        'biblio_pubmed_id' => $this->id,
        'biblio_pubmed_md5' => $this->md5,
        'biblio_contributors' => $this->contributors(),
        // MedlineCitations are always articles from journals or books
        'biblio_type'     => 102,
        'biblio_date'     => $this->date(),
        'biblio_year'     => substr($this->date(), 0, 4),
        'biblio_secondary_title' => $journal,
        'biblio_alternate_title' => (string)$this->article->Article->Journal->ISOAbbreviation,
        'biblio_volume'   => (string)$this->article->Article->Journal->JournalIssue->Volume,
        'biblio_issue'    => (string)$this->article->Article->Journal->JournalIssue->Issue,
        'biblio_issn'     => (string)$this->article->Article->Journal->ISSN,
        'biblio_pages'    => (string)$this->article->Article->Pagination->MedlinePgn,
        'biblio_abst_e'   => $this->abst(),
        'biblio_custom1'  => "http://www.ncbi.nlm.nih.gov/pubmed/{$this->id}?dopt=Abstract",
        'biblio_keywords' => $this->keywords(),
        'biblio_lang'     => $this->lang(),
      );

      $doi = $this->article->xpath('.//ELocationID[@EIdType="doi"]/text()');
      if (empty($doi)) {
        $doi = $this->pubmeddata->xpath('.//ArticleId[@IdType="doi"]/text()');
      }
      if (!empty($doi)) {
        $this->biblio['biblio_doi'] = (string)$doi[0];
      }

      $pmcid = $this->pubmeddata->xpath('.//ArticleId[@IdType="pmc"]/text()');
      if (!empty($pmcid)) {
        $this->biblio['biblio_pmcid'] = (string)$pmcid[0];
      }

      $grants = $this->grants();
      if (!empty($grants)) {
        $this->biblio['biblio_pubmed_grants'] = $grants;
      }
    }

    return $this->biblio;
  }

  /**
   * Returns the list of contributors for import obtained from the given
   * MedlineCitation element.
   *
   * @return array
   *   the contributors of the article
   */
  private function contributors()
  {
    $contributors = array();

    if (isset($this->article->Article->AuthorList->Author)) {
      foreach ($this->article->Article->AuthorList->Author as $author) {
        $name = '';
        if (isset($author->CollectiveName)) {
          $category = 5; // corporate author
          $name = (string)$author->CollectiveName;
        } else {
          $category = 1; //primary (human) author
          $lastname = (string)$author->LastName;
          if (isset($author->ForeName)) {
            $name = $lastname . ', ' . (string)$author->ForeName;
          } elseif (isset($author->FirstName)) {
            $name = $lastname . ', ' . (string)$author->FirstName;
          } elseif (isset($author->Initials)) {
            $name = $lastname . ', ' . (string)$author->Initials;
          }
        }
        if (!empty($name)) {
          $contributors[] = array('name' => $name, 'auth_category' => $category);
        }
      }
    }

    return $contributors;
  }

  private function grants() {
    $grants = array();
    if (isset($this->article->Article->GrantList->Grant)) {
      foreach ($this->article->Article->GrantList->Grant as $grant) {
         $grants[] = array('grantid' => (string)$grant->GrantID,
                           'acronym' => (string)$grant->Acronym,
                           'agency'  => (string)$grant->Agency,
                           'country' => (string)$grant->Country
                     );
      }
    }
    return $grants;
  }

  /**
   * Returns the publication date obtained from the given MedlineCitation's
   * PubDate element. See the reference documentation for possible values:
   * http://www.nlm.nih.gov/bsd/licensee/elements_descriptions.html#pubdate
   * According to the above source it always begins with a four digit year.
   *
   * @return string
   *   the publication date of the article
   */
  private function date()
  {
    $pubDate = $this->article->Article->Journal->JournalIssue->PubDate;

    if (isset($pubDate->MedlineDate)) {
      $date = (string)$pubDate->MedlineDate;
    } else {
      $date = implode(' ', (array)$pubDate);
    }

    return $date;
  }

  private function keywords() {
    $keywords = array();
    if (isset($this->article->MeshHeadingList->MeshHeading)) {
      foreach ($this->article->MeshHeadingList->MeshHeading as $heading) {
        $keywords[] = (string)$heading->DescriptorName;
      }
    }
    return $keywords;
  }

  private function lang() {
    if (isset($this->article->Article->Language)) {
      return (string)$this->article->Article->Language;
    }

  }

  private function abst() {
    if (isset($this->article->Article->Abstract)) {
      $abst = '';
      foreach ($this->article->Article->Abstract->AbstractText  as $text) {
        $abst .= "<p>";
        $attrs = $text->attributes();
        if (isset($attrs['Label'])) {
          $abst .= '<b>' . $attrs['Label'] . ': </b>';
        }
        $abst .=  (string)$text . '</p>';
      }
      return $abst;
    }
  }
}
