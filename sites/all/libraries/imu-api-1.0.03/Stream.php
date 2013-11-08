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
require_once IMu::$lib . '/Exception.php';
require_once IMu::$lib . '/Trace.php';

class IMuStream
{
	/* Static Properties */
	public static function
	getBlockSize()
	{
		return self::$_blockSize;
	}

	public static function
	setBlockSize(int $size)
	{
		self::$_blockSize = $size;
	}

	/* Constructor */
	public function
	__construct($socket)
	{
		$this->_socket = $socket;

		$this->_next = '';
		$this->_token = null;
		$this->_string = null;
		$this->_file = null;

		$this->_buffer = '';
		$this->_length = 0;
	}

	/* Methods */
	public function
	get()
	{
		$what = null;
		try
		{
			$this->getNext();
			$this->getToken();
			$what = $this->getValue();
		}
		catch (IMuException $e)
		{
			throw $e;
		}
		catch (Exception $e)
		{
			throw new IMuException("StreamGet", $e->getMessage());
		}
		return $what;
	}

	public function
	put($what)
	{
		try
		{
			$this->putValue($what, 0);
			$this->putLine();
			$this->putFlush();
		}
		catch (IMuException $e)
		{
			throw $e;
		}
		catch (Exception $e)
		{
			throw new IMuException("StreamPut", $e->getMessage());
		}
	}

	private static $_blockSize = 8192;

	private $_socket;

	private $_next;
	private $_token;
	private $_string;
	private $_file;

	private $_buffer;
	private $_length;

	private function
	getValue()
	{
		if ($this->_token == 'end')
			return null;
		if ($this->_token == 'string')
			return $this->_string;
		if ($this->_token == 'number')
			return $this->_string + 0;
		if ($this->_token == '{')
		{
			$array = array();
			$this->getToken();
			while ($this->_token != '}')
			{
				if ($this->_token == 'string')
					$name = $this->_string;
				else if ($this->_token == 'identifier')
					// Extension - allow simple identifiers
					$name = $this->_string;
				else
					throw new IMuException('StreamSyntaxName', $this->_token);

				$this->getToken();
				if ($this->_token != ':')
					throw new IMuException('StreamSyntaxColon', $this->_token);

				$this->getToken();
				$array[$name] = $this->getValue();

				$this->getToken();
				if ($this->_token == ',')
					$this->getToken();
			}
			return $array;
		}
		if ($this->_token == '[')
		{
			$array = array();
			$this->getToken();
			while ($this->_token != ']')
			{
				$array[] = $this->getValue();

				$this->getToken();
				if ($this->_token == ',')
					$this->getToken();
			}
			return $array;
		}
		if ($this->_token == 'true')
			return true;
		if ($this->_token == 'false')
			return false;
		if ($this->_token == 'null')
			return null;
		if ($this->_token == 'binary')
			return $this->_file;

		throw new IMuException('StreamSyntaxToken', $this->_token);
	}

	private function
	getToken()
	{
		while (ctype_space($this->_next))
			$this->getNext();
		$this->_string = '';
		$this->_file = null;
		if ($this->_next == '"')
		{
			$this->_token = 'string';
			$this->getNext();
			while ($this->_next != '"')
			{
				if ($this->_next == '\\')
				{
					$this->getNext();
					if ($this->_next == 'b')
						$this->_next = "\b";
					else if ($this->_next == 'f')
						$this->_next = "\f";
					else if ($this->_next == 'n')
						$this->_next = "\n";
					else if ($this->_next == 'r')
						$this->_next = "\r";
					else if ($this->_next == 't')
						$this->_next = "\t";
					else if ($this->_next == 'u')
					{
						$this->getNext();
						$str = "";
						for ($i = 0; $i < 4; $i++)
						{
							if (! ctype_xdigit($this->_next))
								break;
							$str .= $this->_next;
							$this->getNext();
						}
						if ($str == '')
							throw new IMuException('StreamSyntaxUnicode');
						$this->_next = chr($str);
					}
				}
				$this->_string .= $this->_next;
				$this->getNext();
			}
			$this->_string = utf8_decode($this->_string);
			$this->getNext();
		}
		else if (ctype_digit($this->_next) || $this->_next == '-')
		{
			$this->_token = 'number';
			$this->_string .= $this->_next;
			$this->getNext();
			while (ctype_digit($this->_next))
			{
				$this->_string .= $this->_next;
				$this->getNext();
			}
			if ($this->_next == '.')
			{
				$this->_string .= $this->_next;
				$this->getNext();
				while (ctype_digit($this->_next))
				{
					$this->_string .= $this->_next;
					$this->getNext();
				}
				if ($this->_next == 'e' || $this->_next == 'E')
				{
					$this->_string .= 'e';
					$this->getNext();
					if ($this->_next == '-')
					{
						$this->_string .= '-';
						$this->getNext();
					}
					while (ctype_digit($this->_next))
					{
						$this->_string .= $this->_next;
						$this->getNext();
					}
				}
			}
		}
		else if (ctype_alpha($this->_next) || $this->_next == '_')
		{
			$this->_token = 'identifier';
			while (ctype_alnum($this->_next) || $this->_next == '_')
			{
				$this->_string .= $this->_next;
				$this->getNext();
			}
			$this->_string = utf8_decode($this->_string);
			$lower = strtolower($this->_string);
			if ($lower == 'false')
				$this->_token = 'false';
			else if ($lower == 'null')
				$this->_token = 'null';
			else if ($lower == 'true')
				$this->_token = 'true';
		}
		else if ($this->_next == '*')
		{
			// Extension - allow embedded binary data
			$this->_token = 'binary';
			$this->getNext();
			while (ctype_digit($this->_next))
			{
				$this->_string .= $this->_next;
				$this->getNext();
			}
			if ($this->_string == '')
				throw new IMuException('StreamSyntaxBinary');
			$size = $this->_string + 0;
			while ($this->_next != "\n")
				$this->getNext();

			// Save data into a temporary file
			$temp = tmpfile();
			$left = $size;
			while ($left > 0)
			{
				$read = self::$_blockSize;
				if ($read > $left)
					$read = $left;
				$data = fread($this->_socket, $read);
				if ($data === false)
					throw new IMuException('StreamInput');
				$done = strlen($data);
				if ($done == 0)
					throw new IMuException('StreamEOF', 'binary');
				fwrite($temp, $data);
				$left -= $done;
			}
			fseek($temp, 0, SEEK_SET);
			$this->_file = $temp;

			$this->getNext();
		}
		else
		{
			$this->_token = $this->_next;
			$this->getNext();
		}
	}

	private function
	getNext()
	{
		$c = fgetc($this->_socket);
		if ($c === false)
			throw new IMuException('StreamEOF', 'character');
		$this->_next = $c;
		return $this->_next;
	}

	private function
	putValue($what, $indent)
	{
		$type = gettype($what);
		if ($type == 'NULL')
			$this->putData('null');
		else if ($type == 'string')
			$this->putString($what);
		else if ($type == 'integer')
			$this->putData(sprintf('%d', $what));
		else if ($type == 'double')
			$this->putData(sprintf('%g', $what));
		else if ($type == 'object')
			$this->putObject(get_object_vars($what), $indent);
		else if ($type == 'array')
		{
			/* A bit magical.
			**
			** If the array is empty treat it as an array rather than
			** a JSON object. Also, if the keys of the array are exactly
			** from 0 to count - 1 then put a JSON array otherwise put a
			** JSON object.
			*/
			if (empty($what))
				$this->putArray($what, $indent);
			else if (array_keys($what) === range(0, count($what) - 1))
				$this->putArray($what, $indent);
			else
				$this->putObject($what, $indent);
		}
		else if ($type == 'boolean')
			$this->putData($what ? 'true' : 'false');
		else if ($type == 'resource')
			$this->putResource($what);
		else
			throw new IMuException('StreamType', $type);
	}

	private function
	putString($what)
	{
		$this->putData('"');
		$what = preg_replace('/\\\\/', '\\\\\\\\', $what);
		$what = preg_replace('/"/', '\\"', $what);
		$this->putData($what);
		$this->putData('"');
	}

	private function
	putObject($what, $indent)
	{
		$this->putData('{');
		$this->putLine();
		$count = count($what);
		$i = 0;
		foreach ($what as $name => $what)
		{
			$this->putIndent($indent + 1);
			$this->putString($name);
			$this->putData(' : ');
			$this->putValue($what, $indent + 1);
			if ($i < $count - 1)
				$this->putData(',');
			$this->putLine();
			$i++;
		}
		$this->putIndent($indent);
		$this->putData('}');
	}

	private function
	putArray($what, $indent)
	{
		$this->putData('[');
		$this->putLine();
		$count = count($what);
		$i = 0;
		foreach ($what as $what)
		{
			$this->putIndent($indent + 1);
			$this->putValue($what, $indent + 1);
			if ($i < $count - 1)
				$this->putData(',');
			$this->putLine();
			$i++;
		}
		$this->putIndent($indent);
		$this->putData(']');
	}

	private function
	putResource($what)
	{
		if (fseek($what, 0, SEEK_END) < 0)
			throw new IMuException('StreamFileSeek');
		$size = ftell($what);
		if (fseek($what, 0, SEEK_SET) < 0)
			throw new IMuException('StreamFileSeek');

		$this->putData('*');
		$this->putData($size);
		$this->putLine();

		$left = $size;
		while ($left > 0)
		{
			$need = self::$_blockSize;
			if ($need > $left)
				$need = $left;
			$data = fread($what, $need);
			if ($data === false)
				throw new IMuException('StreamFileRead');
			$done = strlen($data);
			if ($done == 0)
				break;
			$this->putData($data);
			$left -= $done;
		}
		if ($left > 0)
		{
			/* The file did not contain enough bytes
			** so the output is padded with nulls
			*/
			while ($left > 0)
			{
				$need = self::$blockSize;
				if ($need > $left)
					$need = $left;
				$data = str_repeat(chr(0), $need);
				$this->putData($data);
				$left -= $need;
			}
		}
	}

	private function
	putIndent($indent)
	{
		$string = '';
		for ($i = 0; $i < $indent; $i++)
			$string .= "\t";
		$this->putData($string);
	}

	private function
	putLine()
	{
		$this->putData("\r\n");
	}

	private function
	putData($data)
	{
		$this->_buffer .= $data;
		$this->_length += strlen($data);
		if ($this->_length >= self::$_blockSize)
			$this->putFlush();
	}

	private function
	putFlush()
	{
		if ($this->_length > 0)
		{
			while ($this->_length > 0)
			{
				$wrote = fwrite($this->_socket, $this->_buffer);
				if ($wrote === false)
					throw new IMuException('StreamWriteError');
				if ($wrote == 0)
					throw new IMuException('StreamWriteError');
				$this->_buffer = substr($this->_buffer, $wrote);
				$this->_length -= $wrote;
			}
			fflush($this->_socket);
			$this->_buffer = '';
			$this->_length = 0;
		}
	}
}

?>
