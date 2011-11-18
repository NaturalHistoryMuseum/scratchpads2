<?php

	/* SVN FILE: $Id$ */
	/**
	 * @author Oliver Lillie (aka buggedcom) <publicmail@buggedcom.co.uk>
	 * @package PHPVideoToolkit
	 * @license BSD
	 * @copyright Copyright (c) 2008 Oliver Lillie <http://www.buggedcom.co.uk>
	 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
	 * files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
	 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
	 * is furnished to do so, subject to the following conditions:  The above copyright notice and this permission notice shall be
	 * included in all copies or substantial portions of the Software.
	 *
	 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
	 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
	 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
	 */
	if(!defined('DS'))
	{
		define('DS', DIRECTORY_SEPARATOR);
	}
	
	class VideoTo
	{
		
		private static $_log_files 			= array();
		private static $_error_messages 	= array();
		private static $_commands 			= array();
		private static $_outputs 			= array();
		
		public static function FLV($file, $options=array(), $target_extension='flv')
		{
// 			merge the options with the defaults
			$options = array_merge(array(
				'temp_dir'					=> '/tmp', 
				'width'						=> 320, 
				'height'					=> 240,
				'frequency'					=> 44100, 
				'audio_bitrate'				=> 64, 
				'video_bitrate'				=> 1200, 
				'ratio'						=> false, // PHPVideoToolkit::RATIO_STANDARD, 
				'frame_rate'				=> 29.7, 
				'output_dir'				=> null,	// this doesn't have to be set it can be automatically retreived from 'output_file'
				'output_file'				=> '#filename.#ext', 	// you can use #filename to automagically hold the filename and #ext to automagically hold the target format extension
				'use_multipass'				=> false, 
				'generate_log'				=> true,
				'log_directory'				=> null,
				'die_on_error'				=> false,
				'overwrite_mode'			=> PHPVideoToolkit::OVERWRITE_FAIL
			), $options);
			
// 			start PHPVideoToolkit class
			require_once dirname(dirname(__FILE__)).DS.'phpvideotoolkit.php5.php';
			$toolkit = new PHPVideoToolkit($options['temp_dir']);
			$toolkit->on_error_die = $options['die_on_error'];
// 			get the output directory
			if($options['output_dir'])
			{
				$output_dir 	= $options['output_dir'];
			}
			else
			{
				$output_dir		= dirname($options['output_file']);
				$output_dir		= $output_dir == '.' ? dirname($file) : $output_dir;
			}
// 			get the filename parts
			$filename 			= basename($file);
			$filename_minus_ext = substr($filename, 0, strrpos($filename, '.'));
// 			get the output filename
			$output_filename	= str_replace(array('#filename', '#ext'), array($filename_minus_ext, $target_extension), basename($options['output_file']));
		
// 			set the input file
			$ok = $toolkit->setInputFile($file);
// 			check the return value in-case of error
			if(!$ok)
			{
				$toolkit->reset();
				array_push(self::$_error_messages, $toolkit->getLastError());
				return false;
			}
		
// 			set the output dimensions
			if($options['ratio'] !== false)
			{
				$toolkit->setVideoAspectRatio($options['ratio']);
			}
			$toolkit->setVideoOutputDimensions($options['width'], $options['height']);
			$toolkit->setVideoBitRate($options['video_bitrate']);
			$toolkit->setVideoFrameRate($options['frame_rate']);
		
// 			set the video to be converted to flv
			$toolkit->setFormatToFLV($options['frequency'], $options['audio_bitrate']);
		
// 			set the output details and overwrite if nessecary
			$ok = $toolkit->setOutput($output_dir, $output_filename, $options['overwrite_mode']);
// 			check the return value in-case of error
			if(!$ok)
			{
				$toolkit->reset();
				array_push(self::$_error_messages, $toolkit->getLastError());
				return false;
			}
		
// 			execute the ffmpeg command using multiple passes and log the calls and PHPVideoToolkit results
			$result = $toolkit->execute($options['use_multipass'], $options['generate_log']);
			array_push(self::$_commands, $toolkit->getLastCommand());
		
// 			check the return value in-case of error
			if($result !== PHPVideoToolkit::RESULT_OK)
			{
// 				move the log file to the log directory as something has gone wrong
				if($options['generate_log'])
				{
					$log_dir = $options['log_directory'] ? $options['log_directory'] : $output_dir;
					$toolkit->moveLog($log_dir.$filename_minus_ext.'.log');
					array_push(self::$_log_files, $log_dir.$filename_minus_ext.'.log');
				}
				$toolkit->reset();
				array_push(self::$_error_messages, $toolkit->getLastError());
				return $result;
			}
			
			array_push(self::$_outputs, $toolkit->getLastOutput());
			
// 			reset 
			$toolkit->reset();
			
			return $result;
		}
		
		public static function PSP($file, $options=array(), $target_extension='mp4')
		{
// 			merge the options with the defaults
			$options = array_merge(array(
				'temp_dir'					=> '/tmp', 
				'width'						=> 368, 
				'height'					=> 192,
				'frequency'					=> 44100, 
				'audio_bitrate'				=> 128, 
				'video_bitrate'				=> 1200, 
				'ratio'						=> false, //PHPVideoToolkit::RATIO_STANDARD, 
				'frame_rate'				=> 29.7, 
				'output_dir'				=> null,	// this doesn't have to be set it can be automatically retreived from 'output_file'
				'output_file'				=> '#filename.#ext', 	// you can use #filename to automagically hold the filename and #ext to automagically hold the target format extension
				'output_title'				=> '#filename', 	// you can use #filename to automagically hold the filename and #ext to automagically hold the target format extension
				'use_multipass'				=> false, 
				'generate_log'				=> true,
				'log_directory'				=> null,
				'die_on_error'				=> false,
				'overwrite_mode'			=> PHPVideoToolkit::OVERWRITE_FAIL
			), $options);
			
// 			start PHPVideoToolkit class
			require_once dirname(dirname(__FILE__)).DS.'phpvideotoolkit.php5.php';
			$toolkit = new PHPVideoToolkit($options['temp_dir']);
			$toolkit->on_error_die = $options['die_on_error'];
// 			get the output directory
			if($options['output_dir'])
			{
				$output_dir 	= $options['output_dir'];
			}
			else
			{
				$output_dir		= dirname($options['output_file']);
				$output_dir		= $output_dir == '.' ? dirname($file) : $output_dir;
			}
// 			get the filename parts
			$filename 			= basename($file);
			$filename_minus_ext = substr($filename, 0, strrpos($filename, '.'));
// 			get the output filename
			$output_filename	= str_replace(array('#filename', '#ext'), array($filename_minus_ext, $target_extension), basename($options['output_file']));
		
// 			set the input file
			$ok = $toolkit->setInputFile($file);
// 			check the return value in-case of error
			if(!$ok)
			{
				$toolkit->reset();
				array_push(self::$_error_messages, $toolkit->getLastError());
				return false;
			}
			$toolkit->setFormat(PHPVideoToolkit::FORMAT_PSP);
			
			$toolkit->setAudioSampleFrequency($options['frequency']);
			$toolkit->setAudioBitRate($options['audio_bitrate']);
// 			$toolkit->addCommand('-acodec', 'libfaac');
// 			$toolkit->addCommand('-acodec', 'mp3');
			
			$toolkit->setVideoFormat(PHPVideoToolkit::FORMAT_MPEG4);
			if($options['ratio'] !== false)
			{
				$toolkit->setVideoAspectRatio($options['ratio']);
			}
			$toolkit->setVideoOutputDimensions($options['width'], $options['height']);
			$toolkit->setVideoBitRate($options['video_bitrate']);
			$toolkit->setVideoFrameRate($options['frame_rate']);
			$toolkit->addCommand('-flags', 'loop');
			$toolkit->addCommand('-trellis', '2');
			$toolkit->addCommand('-partitions', 'parti4x4+parti8x8+partp4x4+partp8x8+partb8x8');
			$toolkit->addCommand('-coder', '1');
			$toolkit->addCommand('-mbd', '2');
			$toolkit->addCommand('-cmp', '2');
			$toolkit->addCommand('-subcmp', '2');
			$toolkit->addCommand('-title', str_replace(array('#filename', '#ext'), array($filename_minus_ext, $target_extension), basename($options['output_title'])));
		
// 			set the output details and overwrite if nessecary
			$ok = $toolkit->setOutput($output_dir, $output_filename, $options['overwrite_mode']);
// 			check the return value in-case of error
			if(!$ok)
			{
				$toolkit->reset();
				array_push(self::$_error_messages, $toolkit->getLastError());
				return false;
			}
			
// 			execute the ffmpeg command using multiple passes and log the calls and PHPVideoToolkit results
			$result = $toolkit->execute($options['use_multipass'], $options['generate_log']);
			array_push(self::$_commands, $toolkit->getLastCommand());
		
// 			check the return value in-case of error
			if($result !== PHPVideoToolkit::RESULT_OK)
			{
// 				move the log file to the log directory as something has gone wrong
				if($options['generate_log'])
				{
					$log_dir = $options['log_directory'] ? $options['log_directory'] : $output_dir;
					$toolkit->moveLog($log_dir.$filename_minus_ext.'.log');
					array_push(self::$_log_files, $log_dir.$filename_minus_ext.'.log');
				}
				$toolkit->reset();
				array_push(self::$_error_messages, $toolkit->getLastError());
				return $result;
			}
			
			array_push(self::$_outputs, $toolkit->getLastOutput());
			
// 			reset 
			$toolkit->reset();
			
			return $result;
		}
		
		public static function iPod($file, $options=array(), $target_extension='mp4')
		{
// 			merge the options with the defaults
			$options = array_merge(array(
				'temp_dir'					=> '/tmp', 
				'width'						=> 320, 
				'height'					=> 240,
				'frequency'					=> 44100, 
				'audio_bitrate'				=> 128, 
				'video_bitrate'				=> 1200, 
				'ratio'						=> false, //PHPVideoToolkit::RATIO_STANDARD, 
				'frame_rate'				=> 29.7, 
				'output_dir'				=> null,	// this doesn't have to be set it can be automatically retreived from 'output_file'
				'output_file'				=> '#filename.#ext', 	// you can use #filename to automagically hold the filename and #ext to automagically hold the target format extension
				'output_title'				=> '#filename', 	// you can use #filename to automagically hold the filename and #ext to automagically hold the target format extension
				'use_multipass'				=> false, 
				'generate_log'				=> true,
				'log_directory'				=> null,
				'die_on_error'				=> false,
				'overwrite_mode'			=> PHPVideoToolkit::OVERWRITE_FAIL
			), $options);
			
// 			start PHPVideoToolkit class
			require_once dirname(dirname(__FILE__)).DS.'phpvideotoolkit.php5.php';
			$toolkit = new PHPVideoToolkit($options['temp_dir']);
			$toolkit->on_error_die = $options['die_on_error'];
// 			get the output directory
			if($options['output_dir'])
			{
				$output_dir 	= $options['output_dir'];
			}
			else
			{
				$output_dir		= dirname($options['output_file']);
				$output_dir		= $output_dir == '.' ? dirname($file) : $output_dir;
			}
// 			get the filename parts
			$filename 			= basename($file);
			$filename_minus_ext = substr($filename, 0, strrpos($filename, '.'));
// 			get the output filename
			$output_filename	= str_replace(array('#filename', '#ext'), array($filename_minus_ext, $target_extension), basename($options['output_file']));
		
// 			set the input file
			$ok = $toolkit->setInputFile($file);
// 			check the return value in-case of error
			if(!$ok)
			{
				$toolkit->reset();
				array_push(self::$_error_messages, $toolkit->getLastError());
				return false;
			}
			$toolkit->setFormat(PHPVideoToolkit::FORMAT_MP4);
			
			$toolkit->setAudioSampleFrequency($options['frequency']);
			$toolkit->setAudioBitRate($options['audio_bitrate']);
// 			$toolkit->addCommand('-acodec', 'libfaac');
// 			$toolkit->addCommand('-acodec', 'mp3');
			
			$toolkit->setVideoFormat(PHPVideoToolkit::FORMAT_MPEG4);
			if($options['ratio'] !== false)
			{
				$toolkit->setVideoAspectRatio($options['ratio']);
			}
			$toolkit->setVideoOutputDimensions($options['width'], $options['height']);
			$toolkit->setVideoFrameRate($options['frame_rate']);
			$toolkit->addCommand('-mbd', '2');
			$toolkit->addCommand('-flags', '+4mv+trell');
			$toolkit->addCommand('-aic', '2');
			$toolkit->addCommand('-cmp', '2');
			$toolkit->addCommand('-subcmp', '2');
			
			$toolkit->addCommand('-title', str_replace(array('#filename', '#ext'), array($filename_minus_ext, $target_extension), basename($options['output_title'])));
		
// 			set the output details and overwrite if nessecary
			$ok = $toolkit->setOutput($output_dir, $output_filename, $options['overwrite_mode']);
// 			check the return value in-case of error
			if(!$ok)
			{
				$toolkit->reset();
				array_push(self::$_error_messages, $toolkit->getLastError());
				return false;
			}
			
// 			execute the ffmpeg command using multiple passes and log the calls and PHPVideoToolkit results
			$result = $toolkit->execute($options['use_multipass'], $options['generate_log']);
			array_push(self::$_commands, $toolkit->getLastCommand());
		
// 			check the return value in-case of error
			if($result !== PHPVideoToolkit::RESULT_OK)
			{
// 				move the log file to the log directory as something has gone wrong
				if($options['generate_log'])
				{
					$log_dir = $options['log_directory'] ? $options['log_directory'] : $output_dir;
					$toolkit->moveLog($log_dir.$filename_minus_ext.'.log');
					array_push(self::$_log_files, $log_dir.$filename_minus_ext.'.log');
				}
				$toolkit->reset();
				array_push(self::$_error_messages, $toolkit->getLastError());
				return $result;
			}
			
			array_push(self::$_outputs, $toolkit->getLastOutput());
			
// 			reset 
			$toolkit->reset();
			
			return $result;
		}
		
		public static function gif($file, $options=array(), $target_extension='gif')
		{
// 			merge the options with the defaults
			$options = array_merge(array(
				'temp_dir'					=> '/tmp', 
				'width'						=> 320, 
				'height'					=> 240,
				'ratio'						=> false, //PHPVideoToolkit::RATIO_STANDARD, 
				'frame_rate'				=> 1, 
				'loop_output'				=> 0,	// 0 will loop endlessly
				'output_dir'				=> null,	// this doesn't have to be set it can be automatically retreived from 'output_file'
				'output_file'				=> '#filename.#ext', 	// you can use #filename to automagically hold the filename and #ext to automagically hold the target format extension
				'output_title'				=> '#filename', 	// you can use #filename to automagically hold the filename and #ext to automagically hold the target format extension
				'use_multipass'				=> false, 
				'generate_log'				=> true,
				'log_directory'				=> null,
				'die_on_error'				=> false,
				'overwrite_mode'			=> PHPVideoToolkit::OVERWRITE_FAIL
			), $options);
			
// 			start PHPVideoToolkit class
			require_once dirname(dirname(__FILE__)).DS.'phpvideotoolkit.php5.php';
			$toolkit = new PHPVideoToolkit($options['temp_dir']);
			$toolkit->on_error_die = $options['die_on_error'];
// 			get the output directory
			if($options['output_dir'])
			{
				$output_dir 	= $options['output_dir'];
			}
			else
			{
				$output_dir		= dirname($options['output_file']);
				$output_dir		= $output_dir == '.' ? dirname($file) : $output_dir;
			}
// 			get the filename parts
			$filename 			= basename($file);
			$filename_minus_ext = substr($filename, 0, strrpos($filename, '.'));
// 			get the output filename
			$output_filename	= str_replace(array('#filename', '#ext'), array($filename_minus_ext, $target_extension), basename($options['output_file']));
		
// 			set the input file
			$ok = $toolkit->setInputFile($file);
// 			check the return value in-case of error
			if(!$ok)
			{
				$toolkit->reset();
				array_push(self::$_error_messages, $toolkit->getLastError());
				return false;
			}
			$toolkit->setFormat(PHPVideoToolkit::FORMAT_GIF);
			
			$toolkit->disableAudio();
			
			if($options['ratio'] !== false)
			{
				$toolkit->setVideoAspectRatio($options['ratio']);
			}
			$toolkit->setVideoOutputDimensions($options['width'], $options['height']);
			$toolkit->setVideoFrameRate($options['frame_rate']);
			$toolkit->addCommand('-loop_output', $options['loop_output']);
			
// 			set the output details and overwrite if nessecary
			$ok = $toolkit->setOutput($output_dir, $output_filename, $options['overwrite_mode']);
// 			check the return value in-case of error
			if(!$ok)
			{
				$toolkit->reset();
				array_push(self::$_error_messages, $toolkit->getLastError());
				return false;
			}
			
// 			execute the ffmpeg command using multiple passes and log the calls and PHPVideoToolkit results
			$result = $toolkit->execute($options['use_multipass'], $options['generate_log']);
			array_push(self::$_commands, $toolkit->getLastCommand());
		
// 			check the return value in-case of error
			if($result !== PHPVideoToolkit::RESULT_OK)
			{
// 				move the log file to the log directory as something has gone wrong
				if($options['generate_log'])
				{
					$log_dir = $options['log_directory'] ? $options['log_directory'] : $output_dir;
					$toolkit->moveLog($log_dir.$filename_minus_ext.'.log');
					array_push(self::$_log_files, $log_dir.$filename_minus_ext.'.log');
				}
				$toolkit->reset();
				array_push(self::$_error_messages, $toolkit->getLastError());
				return $result;
			}
			
			array_push(self::$_outputs, $toolkit->getLastOutput());
			
// 			reset 
			$toolkit->reset();
			
			return $result;
		}
		
		public static function getOutput($all=false)
		{
			return $all ? self::$_outputs : self::$_outputs[count(self::$_outputs)-1];
		}
		
		public static function getCommand($all=false)
		{
			return $all ? self::$_commands : self::$_commands[count(self::$_commands)-1];
		}
		
		public static function getError($all=false)
		{
			return $all ? self::$_error_messages : self::$_error_messages[count(self::$_error_messages)-1];
		}
		
		public static function getLogFile($all=false)
		{
			return $all ? self::$_log_files : self::$_log_files[count(self::$_log_files)-1];
		}
		
	}