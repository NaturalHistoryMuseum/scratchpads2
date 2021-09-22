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
require_once IMu::$lib . '/Stream.php';
require_once IMu::$lib . '/Trace.php';

class IMuSession
{
	/* Static Properties */
	public static function
	getDefaultHost()
	{
		return self::$_defaultHost;
	}

	public static function
	setDefaultHost($host)
	{
		self::$_defaultHost = $host;
	}

	public static function
	getDefaultPort()
	{
		return self::$_defaultPort;
	}

	public static function
	setDefaultPort($port)
	{
		self::$_defaultPort = $port;
	}

	/* Constructor */
	public function
	__construct($host = null, $port = null)
	{
		$this->initialise();
		if ($host != null)
			$this->_host = $host;
		if ($port != null)
			$this->_port = $port;
	}

	/* Properties */
	public function
	getClose()
	{
		if ($this->_close == null)
			return false;
		return $this->_close;
	}

	public function
	setClose($close)
	{
		$this->_close = $close;
	}

	public function
	getContext()
	{
		return $this->_context;
	}

	public function
	setContext($context)
	{
		$this->_context = $context;
	}

	public function
	getHost()
	{
		return $this->_host;
	}

	public function
	setHost($host)
	{
		$this->_host = $host;
	}

	public function
	getPort()
	{
		return $this->_port;
	}

	public function
	setPort($port)
	{
		$this->_port = $port;
	}

	public function
	getSuspend()
	{
		if ($this->_suspend == null)
			return false;
		return $this->_suspend;
	}

	public function
	setSuspend($suspend)
	{
		$this->_suspend = $suspend;
	}

	public function
	__get($name)
	{
		switch ($name)
		{
		  case 'close':
		  	return $this->getClose();
			break;
		  case 'context':
		  	return $this->getContext();
			break;
		  case 'host':
		  	return $this->getHost();
			break;
		  case 'port':
		  	return $this->getPort();
			break;
		  case 'suspend':
		  	return $this->getSuspend();
			break;
		  default:
		  	throw new IMuException('SessionProperty', $name);
		}
	}

	public function
	__set($name, $value)
	{
		switch ($name)
		{
		  case 'close':
		  	return $this->setClose($value);
			break;
		  case 'context':
		  	return $this->setContext($value);
			break;
		  case 'host':
		  	return $this->setHost($value);
			break;
		  case 'port':
		  	return $this->setPort($value);
			break;
		  case 'suspend':
		  	return $this->setSuspend($value);
			break;
		  default:
		  	throw new IMuException('SessionProperty', $name);
		}
	}

	/* Methods */
	public function
	connect()
	{
		if ($this->_socket != null)
			return;

		IMuTrace::write(2, 'connecting to %s:%d', $this->_host, $this->_port);
		$socket = @fsockopen($this->_host, $this->_port, $errno, $errstr);
		if ($socket === false)
			throw new IMuException('SessionConnect', $this->_host, $this->_port,
				$errstr);
		IMuTrace::write(2, 'connected ok');
		$this->_socket = $socket;
		$this->_stream = new IMuStream($this->_socket);
	}

	public function
	disconnect()
	{
		if ($this->_socket == null)
			return;

		IMuTrace::write(2, 'closing connection');
		@fclose($this->_socket);
		$this->initialise();
	}

	public function
	login($login, $password = null, $spawn = true)
	{
		$request = array();
		$request['login'] = $login;
		$request['password'] = $password;
		$request['spawn'] = $spawn;
		return $this->request($request);
	}

	public function
	request($request)
	{
		$this->connect();

		if ($this->_close != null)
			$request['close'] = $this->_close;
		if ($this->_context != null)
			$request['context'] = $this->_context;
		if ($this->suspend != null)
			$request['suspend'] = $this->_suspend;

		$this->_stream->put($request);
		$response = $this->_stream->get();
		$type = gettype($response);
		if ($type != 'array')
			throw new IMuException('SessionResponse', $type);

		if (array_key_exists('context', $response))
			$this->_context = $response['context'];
		if (array_key_exists('reconnect', $response))
			$this->_port = $response['reconnect'];

		$disconnect = false;
		if ($this->_close != null)
			$disconnect = $this->_close;
		if ($disconnect)
			$this->disconnect();

		$status = $response['status'];
		if ($status == 'error')
		{
			IMuTrace::write(2, 'server error');

			$id = 'SessionServerError';
			if (array_key_exists('error', $response))
				$id = $response['error'];
			else if (array_key_exists('id', $response))
				$id = $response['id'];

			$e = new IMuException($id);

			if (array_key_exists('args', $response))
				$e->setArgs($response['args']);

			if (array_key_exists('code', $response))
				$e->setCode($response['code']);

			IMuTrace::write(2, 'throwing exception %s', $e->__toString());

			throw $e;
		}

		return $response;
	}

	private static $_defaultHost = '127.0.0.1';
	private static $_defaultPort = 40000;

	private $_close;
	private $_context;
	private $_host;
	private $_port;
	private $_socket;
	private $_stream;
	private $_suspend;

	private function
	initialise()
	{
		$this->_close = null;
		$this->_context = null;
		$this->_host = self::$_defaultHost;
		$this->_port = self::$_defaultPort;
		$this->_socket = null;
		$this->_stream = null;
		$this->_suspend = null;
	}
}
?>
