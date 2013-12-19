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
require_once IMu::$lib . '/Session.php';

class IMuHandler
{
	/* Constructor */
	public function
	__construct($session = null)
	{
		if ($session == null)
			$this->_session = new IMuSession;
		else
			$this->_session = $session;

		$this->_create = null;
		$this->_destroy = null;
		$this->_id = null;
		$this->_language = null;
		$this->_name = null;
	}

	/* Properties */
	public function
	getCreate()
	{
		return $this->_create;
	}

	public function
	setCreate($create)
	{
		$this->_create = $create;
	}

	public function
	getDestroy()
	{
		if ($this->_destroy == null)
			return false;
		return $this->_destroy;
	}

	public function
	setDestroy($destroy)
	{
		$this->_destroy = $destroy;
	}

	public function
	getID()
	{
		return $this->_id;
	}

	public function
	setID($id)
	{
		$this->_id = $id;
	}

	public function
	getLanguage()
	{
		return $this->_language;
	}

	public function
	setLanguage($language)
	{
		$this->_language = $language;
	}

	public function
	getName()
	{
		return $this->_name;
	}

	public function
	setName($name)
	{
		$this->_name = $name;
	}

	public function
	getSession()
	{
		return $this->_session;
	}

	public function
	__get($name)
	{
		switch ($name)
		{
		  case 'create':
		  	return $this->getCreate();
		  case 'destroy':
		  	return $this->getDestroy();
		  case 'id':
		  	return $this->getID();
		  case 'language':
		  	return $this->getLanguage();
		  case 'name':
		  	return $this->getName();
		  case 'session':
		  	return $this->getSession();
		  default:
		  	throw new IMuException('HandlerProperty', $name);
		}
	}

	public function
	__set($name, $value)
	{
		switch ($name)
		{
		  case 'create':
		  	return $this->setCreate($value);
		  case 'destroy':
		  	return $this->setDestroy($value);
		  case 'id':
		  	return $this->setID($value);
		  case 'language':
		  	return $this->setLanguage($value);
		  case 'name':
		  	return $this->setName($value);
		  case 'session':
		  	throw new IMuException('HandlerSessionReadOnly');
		  default:
		  	throw new IMuException('HandlerProperty', $name);
		}
	}

	/* Methods */
	public function
	call($method, $params = null)
	{
		$request = array();
		$request['method'] = $method;
		if ($params != null)
			$request['params'] = $params;
		$response = $this->request($request);
		return $response['result'];
	}

	public function
	request($request)
	{
		if ($this->_id != null)
			$request['id'] = $this->_id;
		else if ($this->_name != null)
		{
			$request['name'] = $this->_name;
			if ($this->_create != null)
				$request['create'] = $this->_create;
		}
		if ($this->_destroy != null)
			$request['destroy'] = $this->_destroy;
		if ($this->_language != null)
			$request['language'] = $this->_language;

		$response = $this->_session->request($request);

		if (array_key_exists('id', $response))
			$this->_id = $response['id'];

		return $response;
	}

	protected $_session;

	protected $_create;
	protected $_destroy;
	protected $_id;
	protected $_language;
	protected $_name;
}
?>
