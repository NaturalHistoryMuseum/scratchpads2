<?php

/**
 * Client for importing biblio data form crossref json api
 */
class BiblioCrossRefClient {
  const BASE_URL = 'https://api.crossref.org/works/';
  private $doi;

  /**
   *
   */
  public function __construct($doi = '') {
    $this->doi = $doi;
  }

	/**
	 * Get the array of biblio contributor data for a given
	 * $category - The biblio contributor category ID [1:author, 2:editor, 3:chair, 4:translator]
	 * $list - The list of crossref contributors for this category
	 * $type - The biblio type ID for the current work
	 */
	private function getContributors($category, $list, $type) {
		return array_map(
			function($author) use($category, $type){
				return [
					'auth_type' => _biblio_get_auth_type($category, $type),
					'auth_category' => $category,
					'name' => $author['family'] . (isset($author['given']) ? ', ' . $author['given'] : '')
				];
			},
			$list
		);
	}

	/**
	 * Get the biblio type ID for a given crossref type
	 * See list here https://api.crossref.org/types
	 */
	private function get_biblio_type($type){
		switch($type) {
			case 'book':
			case 'book-set':
			case 'reference-book':
			case 'book-series':
			case 'edited-book':
				return 100; // book
			case 'book-section':
			case 'book-part':
			case 'book-chapter':
				return 101; // book chapter
			case 'journal-article':
				return 102; // journal article
			case 'proceedings-article':
			case 'proceedings-series':
			case 'proceedings':
				return 104; // conference proceedings
			case 'dissertation':
				return 108; // thesis
			case 'report':
			case 'report-series':
				return 109; // report
			case 'journal-volume':
			case 'journal':
			case 'journal-issue':
				return 131; // journal
			default /* everything else */:
				return 129; // misc
		}
	}

	/**
	 * Given a title, cleans it by removing any non <i> or <em> tags and ensuring that all remaining
	 * tags are lowercase.
	 *
	 * @param $title string the title
	 * @return string
	 */
	private function cleanTitle($title) {
		// remove any tags that aren't <i> or <em> (this is a case-insensitive strip)
		$title = strip_tags($title, '<i><em>');
		// then ensure any uppsercase <i> or <em> tags are lowercased
		$search = array('<I>', '</I>', '<EM>', '</EM>', '<eM>', '</eM>', '<Em>', '</Em>');
		// the replacements of each are just the lowercase versions
		$replace = array_map('strtolower', $search);
		// replace and return
		return str_replace($search, $replace, $title);
	}

  /**
   * Fetch data from the API and return the node properties
   */
  public function fetch() {
		$url = self::BASE_URL . urlencode($this->doi);

		$json = file_get_contents($url);
		if($json === false) {
			drupal_set_message(t('Failed to retrieve data for doi %doi', array('%doi' => $this->doi)), 'error');
			return [];
		}
		$work = json_decode($json, true);
		if($work['status'] != 'ok') {
			drupal_set_message(t('CrossRef Error: @error', array('@error' => $json)), 'error');
			return [];
		}
		$message = $work['message'];
		list($year, $month, $day) = $message['issued']['date-parts'][0];
		$type = $this->get_biblio_type($message['type']);

		$biblio = [
			'title' => $this->cleanTitle($message['title'][0]),
			'biblio_type' => $type,
			'biblio_contributors' => array_merge(
				$this->getContributors(1, $message['author'] ?? [], $type),
				$this->getContributors(2, $message['editor'] ?? [], $type),
				$this->getContributors(3, $message['chair'] ?? [], $type),
				$this->getContributors(4, $message['translator'] ?? [], $type)
			),
			'biblio_crossref_id' => $message['DOI'],
			'biblio_abst_e' => $message['abstract'],
			'biblio_pages' => $message['page'] ?? '',
			'month' => $month,
			'day' => $day,
			'year' => $year,
			'biblio_date' => date('M-d-Y', mktime(0, 0, 0, $month ?? 1, $day ?? 1, $year ?? 0)),
			'doi' => $message['DOI'],
			'biblio_pages' => $message['page'] ?? '',
			//'biblio_first_page'
			//'biblio_last_page'
			'biblio_url' => $message['URL'],
			'biblio_year' => $year,
			//'biblio_place_published',	// publisher_place'
			'biblio_publisher' => $message['publisher'],	// 'publisher_name'
			'biblio_volume' => $message['volume'] ?? '',	// 'volume'
			//'biblio_number' => ,	// 'number'
			//'biblio_issue',	// 'issue'
			// 'biblio_edition',	// 'edition_number'
			// 'biblio_section',	// 'section'
			'biblio_doi' => $message['DOI'],	// 'doi'
			'biblio_isbn' => implode(', ', $message['ISBN'] ?? []),	// 'isbn'
			'biblio_issn' => implode(', ', $message['ISSN'] ?? []),	// 'issn'

			// - Journal metadata.
			'biblio_secondary_title' => $message['subtitle'][0]??'',	// 'full_title'
			'biblio_short_title' => $message['short-title'][0]??'',	// 'abbrev_title'

			// - Conference metadata.
			//'biblio_place_published',	// 'conference_location
			//'biblio_secondary_title',	// 'conference_name'
			//'biblio_short_title',	// 'conference_acronym'

			// - Proceedings metadata.
			// 'biblio_secondary_title',	// 'proceedings_title'
			// 'year',	// 'year'
			// 'month',	// 'month'
			// 'day',	// 'day'
			// 'biblio_type_of_work',	// 'degree'
			// 'error',	// 'error'
			// 'biblio_lang',	// 'language'
		];

		$biblio['biblio_crossref_md5'] = md5(serialize($biblio));

		return $biblio;
  }
}
