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
class IMuTrace
{
	/* Static Properties */
	public static function
	getFile()
	{
		return self::$_file;
	}

	public static function
	setFile($file = null)
	{
		self::$_file = $file;

		if (self::$_handle != null && self::$_handle != STDOUT)
			fclose(self::$_handle);

		if (self::$_file == null || self::$_file == '')
		{
			self::$_file = '';
			self::$_handle = null;
		}
		else if (self::$_file == 'STDOUT')
			self::$_handle = STDOUT;
		else
		{
			self::$_handle = @fopen(self::$_file, 'a');
			if (self::$_handle === false)
			{
				self::$_file = '';
				self::$_handle = null;
			}
		}
	}

	public static function
	getLevel()
	{
		return self::$_level;
	}

	public static function
	setLevel($level)
	{
		self::$_level = $level;
	}

	public static function
	getPrefix()
	{
		return self::$_prefix;
	}

	public static function
	setPrefix($prefix)
	{
		self::$_prefix = $prefix;
	}

	public static function
	write($level, $format)
	{
		$args = func_get_args();
		array_shift($args);
		array_shift($args);
		self::writeArgs($level, $format, $args);
	}

	public static function
	writeArgs($level, $format, $args)
	{
		if (self::$_handle == null)
			return;
		if ($level > self::$_level)
			return;

		/* time */
		$y = date('Y');
		$m = date('m');
		$d = date('d');
		$D = "$y-$m-$d";

		$H = date('H');
		$M = date('i');
		$S = date('s');
		$T = "$H:$M:$S";

		/* process id */
		$p = getmypid();

		/* function information */
		$F = '(unknown)';
		$L = '(unknown)';
		$f = '(none)';
		$g = '(none)';
		$trace = debug_backtrace();
		$count = count($trace);
		for ($i = 0; $i < $count; $i++)
		{
			$frame = $trace[$i];
			if ($frame['file'] != __FILE__)
			{
				$F = $frame['file'];
				$L = $frame['line'];
				if ($i < $count - 1)
				{
					$frame = $trace[$i + 1];
					if (array_key_exists('class', $frame))
						$f = $frame['class'] . '::' . $frame['function'];
					else
						$f = $frame['function'];
					$g = preg_replace('/^IMu/', '', $f);
				}
				break;
			}
		}

		/* Build the prefix */
		$prefix = self::$_prefix;

		$prefix = preg_replace('/%y/', $y, $prefix);
		$prefix = preg_replace('/%m/', $m, $prefix);
		$prefix = preg_replace('/%d/', $d, $prefix);
		$prefix = preg_replace('/%D/', $D, $prefix);

		$prefix = preg_replace('/%H/', $H, $prefix);
		$prefix = preg_replace('/%M/', $M, $prefix);
		$prefix = preg_replace('/%S/', $S, $prefix);
		$prefix = preg_replace('/%T/', $T, $prefix);

		$prefix = preg_replace('/%p/', $p, $prefix);

		$prefix = preg_replace('/%F/', $F, $prefix);
		$prefix = preg_replace('/%L/', $L, $prefix);
		$prefix = preg_replace('/%f/', $f, $prefix);
		$prefix = preg_replace('/%g/', $g, $prefix);

		/* Build the string */
		$strs = array();
		foreach ($args as $arg)
			$strs[] = print_r($arg, true);
		$format = "$format";
		if (count($args) > 0)
			$format = vsprintf($format, $strs);
		$text = $prefix . $format;
		$text = preg_replace('/\r?\n/', PHP_EOL, $text);
		$text = preg_replace('/\s+$/', '', $text);
		$text .= PHP_EOL;

		/* Write it out */
		if (self::$_handle != STDOUT)
		{
			/* Lock */
			if (! flock(self::$_handle, LOCK_EX))
				return;

			/* Append */
			if (fseek(self::$_handle, 0, SEEK_END) != 0)
			{
				flock(self::$_handle, LOCK_UN);
				return;
			}
		}
		fwrite(self::$_handle, $text);
		fflush(self::$_handle);
		if (self::$_handle != STDOUT)
			flock(self::$_handle, LOCK_UN);
	}

	private static $_file = 'STDOUT';
	private static $_handle = STDOUT;
	private static $_level = 1;
	private static $_prefix = '%D %T: ';
}
?>
