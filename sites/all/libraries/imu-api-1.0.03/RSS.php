<?php
/* KE Software Open Source Licence
** 
** Notice: Copyright (c) 2011  KE SOFTWARE PTY LTD (ACN 006 213 298)
** (the "Owner"). All rights reserved.
** 
** Licence: Permission is hereby granted, free of charge, to any person
** obtaining a copy of this software and associated documentation files
** (the "Software"), to deal with the Software without restriction,
** including without limitation the rights to use, copy, modify, merge,
** publish, distribute, sublicense, and/or sell copies of the Software,
** and to permit persons to whom the Software is furnished to do so,
** subject to the following conditions.
** 
** Conditions: The Software is licensed on condition that:
** 
** (1) Redistributions of source code must retain the above Notice,
**     these Conditions and the following Limitations.
** 
** (2) Redistributions in binary form must reproduce the above Notice,
**     these Conditions and the following Limitations in the
**     documentation and/or other materials provided with the distribution.
** 
** (3) Neither the names of the Owner, nor the names of its contributors
**     may be used to endorse or promote products derived from this
**     Software without specific prior written permission.
** 
** Limitations: Any person exercising any of the permissions in the
** relevant licence will be taken to have accepted the following as
** legally binding terms severally with the Owner and any other
** copyright owners (collectively "Participants"):
** 
** TO THE EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS",
** WITHOUT ANY REPRESENTATION, WARRANTY OR CONDITION OF ANY KIND, EXPRESS
** OR IMPLIED, INCLUDING (WITHOUT LIMITATION) AS TO MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. TO THE EXTENT
** PERMITTED BY LAW, IN NO EVENT SHALL ANY PARTICIPANT BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE SOFTWARE.
** 
** WHERE BY LAW A LIABILITY (ON ANY BASIS) OF ANY PARTICIPANT IN RELATION
** TO THE SOFTWARE CANNOT BE EXCLUDED, THEN TO THE EXTENT PERMITTED BY
** LAW THAT LIABILITY IS LIMITED AT THE OPTION OF THE PARTICIPANT TO THE
** REPLACEMENT, REPAIR OR RESUPPLY OF THE RELEVANT GOODS OR SERVICES
** (INCLUDING BUT NOT LIMITED TO SOFTWARE) OR THE PAYMENT OF THE COST OF SAME.
*/
require_once dirname(__FILE__) . '/IMu.php';
require_once IMu::$lib . '/Document.php';

class IMuRSS
{
	public $category;
	public $copyright;
	public $description;
	public $encoding;
	public $language;
	public $link;
	public $title;

	public function
	__construct()
	{
		$this->category = '';
		$this->copyright = '';
		$this->description = '';
		$this->encoding = 'UTF-8';
		$this->language = '';
		$this->link = '';
		$this->title = '';

		$this->items = array();
	}

	public function
	addItem()
	{
		$item = new IMuRSSItem;
		$this->items[] = $item;
		return $item;
	}

	public function
	createXML()
	{
		$date = date('r');

		$xml = new IMuDocument($this->encoding);
		$root = $xml->startElement('rss');
		$root->setAttribute('version', '2.0');
		$xml->startElement('channel');
		$xml->writeElement('category', $this->category);
		$xml->writeElement('copyright', $this->copyright);
		$xml->writeElement('description', $this->description);
		$xml->writeElement('language', $this->language);
		$xml->writeElement('lastBuildDate', $date);
		$xml->writeElement('link', $this->link);
		$xml->writeElement('pubDate', $date);
		$xml->writeElement('title', $this->title);
		foreach ($this->items as $item)
			$item->createXML($xml);
		$xml->endDocument();

		return $xml->saveXML();
	}

	private $items;
}

class IMuRSSItem
{
	public $author;
	public $category;
	public $description;
	public $length;
	public $link;
	public $mimeType;
	public $pubDate;
	public $title;
	public $url;

	public function
	__construct()
	{
		$this->author = '';
		$this->category = '';
		$this->description = '';
		$this->length = '';
		$this->link = '';
		$this->mimeType = '';
		$this->pubDate = '';
		$this->title = '';
		$this->url = '';
	}

	public function
	createXML($xml)
	{
		$xml->startElement('item');

		$xml->writeElement('author', $this->author);
		$xml->writeElement('category', $this->category);
		$xml->writeElement('description', $this->description);

		$enclosure = $xml->startElement('enclosure');
		$enclosure->setAttribute('url', $this->url);
		$enclosure->setAttribute('length', $this->length);
		$enclosure->setAttribute('type', $this->mimeType);
		$xml->endElement();

		$xml->writeElement('guid', $this->link);
		$xml->writeElement('link', $this->link);
		$xml->writeElement('pubDate', $this->pubDate);
		$xml->writeElement('title', $this->title);

		$xml->endElement();
	}
}
?>
