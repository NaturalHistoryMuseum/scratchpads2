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
require_once IMu::$lib . '/Handler.php';

class IMuModules extends IMuHandler
{
	/* Constructor */
	public function
	__construct($session = null)
	{
		parent::__construct($session);

		$this->_name = 'Modules';
	}

	/* Methods */
	public function
	addFetchSet($name, $set)
	{
		$args = array();
		$args['name'] = $name;
		$args['set'] = $set;
		return $this->call('addFetchSet', $args) + 0;
	}

	public function
	addFetchSets($sets)
	{
		return $this->call('addFetchSets', $sets) + 0;
	}

	public function
	addSearchAlias($name, $set)
	{
		$args = array();
		$args['name'] = $name;
		$args['set'] = $set;
		return $this->call('addSearchAlias', $args) + 0;
	}

	public function
	addSearchAliases($aliases)
	{
		return $this->call('addSearchAliases', $aliases) + 0;
	}

	public function
	addSortSet($name, $set)
	{
		$args = array();
		$args['name'] = $name;
		$args['set'] = $set;
		return $this->call('addSortSet', $args) + 0;
	}

	public function
	addSortSets($sets)
	{
		return $this->call('addSortSets', $sets) + 0;
	}

	public function
	fetch($flag, $offset, $count, $columns = null)
	{
		$params = array();
		$params['flag'] = $flag;
		$params['offset'] = $offset;
		$params['count'] = $count;
		if ($columns != null)
			$params['columns'] = $columns;
		$data = $this->call('fetch', $params);

		$result = new IMuModulesFetchResult;
		$result->count = $data['count'] + 0;
		$result->modules = array();
		foreach ($data['modules'] as $item)
		{
			$module = new IMuModulesFetchModule;
			$module->hits = $item['hits'] + 0;
			$module->index = $item['index'] + 0;
			$module->name = $item['name'];
			$module->rows = $item['rows'];

			$result->modules[] = $module;
		}
		if (array_key_exists('current', $data))
			$result->current = $this->makePosition($data['current']);
		if (array_key_exists('next', $data))
			$result->next = $this->makePosition($data['next']);
		if (array_key_exists('prev', $data))
			$result->prev = $this->makePosition($data['prev']);

		return $result;
	}

	public function
	findAttachments($table, $column, $key)
	{
		$args = array();
		$args['table'] = $table;
		$args['column'] = $column;
		$args['key'] = $key;
		return $this->call('findAttachments', $args) + 0;
	}

	public function
	findKeys($keys, $include = null)
	{
		$args = array();
		$args['keys'] = $keys;
		if ($include != null)
			$args['include'] = $include;
		return $this->call('findKeys', $args);
	}

	public function
	findTerms($terms, $include = null)
	{
		$args = array();
		$args['terms'] = $terms;
		if ($include != null)
			$args['include'] = $include;
		return $this->call('findTerms', $args);
	}

	public function
	getHits($module = null)
	{
		return $this->call('getHits', $module) + 0;
	}

	public function
	restoreFromFile($file, $module = null)
	{
		$args = array();
		$args['file'] = $file;
		if ($module != null)
			$args['module'] = $module;
		return $this->call('restoreFromFile', $args);
	}

	public function
	restoreFromTemp($file, $module = null)
	{
		$args = array();
		$args['file'] = $file;
		if ($module != null)
			$args['module'] = $module;
		return $this->call('restoreFromTemp', $args);
	}

	public function
	setModules($list)
	{
		return $this->call('setModules', $list) + 0;
	}

	public function
	sort($set, $flags = null)
	{
		$args = array();
		$args['set'] = $set;
		if ($flags !== null)
			$args['flags'] = $flags;
		return $this->call('sort', $args);
	}

	protected function
	makePosition($array)
	{
		$flag = $array['flag'];
		$offset = $array['offset'] + 0;
		return new IMuModulesFetchPosition($flag, $offset);
	}
}

class IMuModulesFetchResult
{
	public $count;
	public $current;
	public $modules;
	public $next;
	public $prev;

	public function
	__construct()
	{
		$this->count = 0;
		$this->current = null;
		$this->modules = null;
		$this->next = null;
		$this->prev = null;
	}
}

class IMuModulesFetchModule
{
	public $hits;
	public $index;
	public $name;
	public $rows;

	public function
	__construct()
	{
		$this->hits = 0;
		$this->index = -1;
		$this->name = null;
		$this->rows = null;
	}
}

class IMuModulesFetchPosition
{
	public $flag;
	public $offset;

	public function
	__construct($flag, $offset)
	{
		$this->flag = $flag;
		$this->offset = $offset;;
	}
}
?>
