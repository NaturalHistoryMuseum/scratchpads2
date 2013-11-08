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

class IMuDocument extends DOMDocument
{
	public function
	__construct($encoding = false)
	{
		parent::__construct('1.0');
		if ($encoding !== false)
			$this->encoding = $encoding;

		$this->xpath = null;

		$this->formatOutput = true;
		$this->stack = array($this);
		$this->options = array();
	}

	public function
	endDocument()
	{
		while (count($this->stack) > 1)
			$this->endElement();
	}

	public function
	endElement()
	{
		array_shift($this->stack);
	}

	public function
	getTagOption($tag, $name, $default = false)
	{
		if (! array_key_exists($tag, $this->options))
			return $default;
		if (! array_key_exists($name, $this->options[$tag]))
			return $default;
		return $this->options[$tag][$name];
	}

	public function
	hasTagOption($tag, $name)
	{
		if (! array_key_exists($tag, $this->options))
			return false;
		return array_key_exists($name, $this->options[$tag]);
	}

	public function
	setTagOption($tag, $name, $value)
	{
		if (! array_key_exists($tag, $this->options))
			$this->options[$tag] = array();
		$this->options[$tag][$name] = $value;
	}

	public function
	startElement($name)
	{
		$child = $this->createElement($name);
		$parent = $this->stack[0];
		$parent->appendChild($child);
		array_unshift($this->stack, $child);
		return $child;
	}

	public function
	writeElement($name, $value)
	{
		if (is_array($value))
		{
			if (array_keys($value) === range(0, count($value) - 1))
				$elem = $this->writeList($name, $value);
			else
				$elem = $this->writeHash($name, $value);
		}
		else if (is_object($value))
			$elem = $this->writeObject($name, $value);
		else
			$elem = $this->writeText($name, $value);
		return $elem;
	}

	private $stack;
	private $options;

	private function
	writeList($tag, $list)
	{
		/* This is an ugly hack */
		if ($this->hasTagOption($tag, 'child'))
			$child = $this->getTagOption($tag, 'child');
		else if (preg_match('/(.*)s$/', $tag, $match))
			$child = $match[1];
		else if (preg_match('/(.*)_tab$/', $tag, $match))
			$child = $match[1];
		else if (preg_match('/(.*)0$/', $tag, $match))
			$child = $match[1];
		else if (preg_match('/(.*)_nesttab$/', $tag, $match))
			$child = $match[1] . '_tab';
		else
			$child = 'item';

		$elem = $this->startElement($tag);
		foreach ($list as $item)
			$this->writeElement($child, $item);
		$this->endElement();
		return $elem;
	}

	private function
	writeHash($tag, $hash)
	{
		$elem = $this->startElement($tag);
		foreach ($hash as $name => $value)
			$this->writeElement($name, $value);
		$this->endElement();
		return $elem;
	}

	private function
	writeObject($tag, $object)
	{
		$elem = $this->startElement($tag);
		foreach (get_object_vars($object) as $name => $value)
			$this->writeElement($name, $value);
		$this->endElement();
		return $elem;
	}

	private function
	writeText($tag, $text)
	{
		$parent = $this->startElement($tag);
		if ($text !== '')
		{
			$type = gettype($text);
			if ($type == 'boolean')
				$text = $text ? 'true' : 'false';
			else
				$text = utf8_encode($text);

			/* Check if special processing is required
			*/
			if ($this->getTagOption($tag, 'html', false))
			{
				$child = $this->writeHTML($text);
			}
			else if ($this->getTagOption($tag, 'xml', false))
			{
				$child = $this->writeXML($text);
			}
			/* Deprecated: use 'xml' option instead */
			else if ($this->getTagOption($tag, 'raw', false))
			{
				$child = $this->writeXML($text);
			}
			else
				$child = $this->createTextNode($text);

			@$parent->appendChild($child);
		}
		$this->endElement();
		return $parent;
	}

	private function
	writeHTML($text)
	{
		/* Transform entities as these break the XML processing
		*/
		$text = preg_replace('/&nbsp;/', '&#160;', $text);
		// TODO other transformations

		return $this->writeXML($text);
	}

	private function
	writeXML($text)
	{
		$node = $this->createDocumentFragment();
		@$node->appendXML($text);
		return $node;
	}
}
?>
