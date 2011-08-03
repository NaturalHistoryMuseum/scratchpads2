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
	
	/**
	 * This is similar in terms to FFmpeg-PHP's ffmpeg_movie, however it is just for video's and not just audio.
	 */
	class PHPVideoToolkit_Video
	{
		private $_frame_index 	= 1;
		private $_toolkit 		= null;
		private $_media_data 	= null;
		private $_path_to_media = null;
		private $_tmp_directory = null;
		
		/**
		 * Class Constructor
		 * @param string $path_to_media The path to the media file you want to use.
		 * @param string $tmp_directory The temp directory to which to work from. (remember the trailing slash), default set by PHPVIDEOTOOLKIT_TEMP_DIRECTORY
		 */
		function __construct($path_to_media, $tmp_directory=PHPVIDEOTOOLKIT_TEMP_DIRECTORY)
		{
// 			init PHPVideoToolkit class
			require_once dirname(dirname(dirname(__FILE__))).DS.'phpvideotoolkit.php5.php';
// 			store the media path
			$this->_path_to_media = $path_to_media;
			$this->_tmp_directory = $tmp_directory;
// 			if the path is infact a class of PHPVideoToolkit then just copy the class
			if(get_class($path_to_media) === false)
			{
				$this->_toolkit = $path_to_media;
			}
			else
			{
// 				create the toolkit item
				$this->_toolkit = new PHPVideoToolkit($tmp_directory);
				$this->_toolkit->on_error_die = false;
// 				set the input
				$this->_toolkit->setInputFile($path_to_media);
			}
		}
		
		/**
		 * Destructs any internal processes.
		 * @access private
		 */
		function __destruct()
		{
//			loop through the temp files to remove first as they have to be removed before the dir can be removed
			if(!empty($this->_unlink_files))
			{
				foreach ($this->_unlink_files as $key=>$file)
				{
					if(is_file($file))
					{
						@unlink($file);
					}
				}
				$this->_unlink_files = array();
			}
//			loop through the dirs to remove
			if(!empty($this->_unlink_dirs))
			{
				foreach ($this->_unlink_dirs as $key=>$dir)
				{
					if(is_dir($dir))
					{
						@rmdir($dir);
					}
				}
				$this->_unlink_dirs = array();
			}
		}
		
		/**
		 * Loads the meta data for the media file
		 * @access private
		 */
		private function _getMediaData()
		{
			if($this->_media_data === null)
			{
				$this->_media_data = $this->_toolkit->getFileInfo();
			}
		}
		
		/**
		 * Return the duration of a movie or audio file in seconds.
		 * @access public
		 * @return integer
		 */
		public function getDuration()
		{
			$this->_getMediaData();
			return $this->_media_data['duration']['seconds'];
		}
		
		/**
		 * Return the number of frames in a movie or audio file.
		 * @access public
		 * @return integer
		 */
		public function getFrameCount()
		{
			return $this->hasVideo() ? $this->_media_data['video']['frame_count'] : -1;
		}
		
		/**
		 * Return the frame rate of a movie in fps.
		 * @access public
		 * @return integer
		 */
		public function getFrameRate()
		{
			return $this->hasVideo() ? $this->_media_data['video']['frame_rate'] : -1;
		}
		
		/**
		 * Return the path and name of the movie file or audio file.
		 * @access public
		 * @return string
		 */
		public function getFilename()
		{
			return basename($this->_path_to_media);
		}
		
		/**
		 * Return the height of the movie in pixels.
		 * @access public
		 * @return integer
		 */
		public function getFrameHeight()
		{
			return $this->hasVideo() && isset($this->_media_data['video']['dimensions']) ? $this->_media_data['video']['dimensions']['height'] : -1;
		}
		
		/**
		 * Return the width of the movie in pixels.
		 * @access public
		 * @return integer
		 */
		public function getFrameWidth()
		{
			return $this->hasVideo() && isset($this->_media_data['video']['dimensions']) ? $this->_media_data['video']['dimensions']['width'] : -1;
		}
		
		/**
		 * Return the pixel format of the movie.
		 * @access public
		 * @return mixed string | -1
		 */
		public function getPixelFormat()
		{
			return $this->hasVideo() ? $this->_media_data['video']['pixel_format'] : -1;
		}
		
		/**
		 * Return the pixel aspect ratio of the movie
		 * @access public
		 * @return integer
		 */
		public function getPixelAspectRatio()
		{
			return -1; 
		}
		
		/**
		 * Return the bit rate of the movie or audio file in bits per second.
		 * @access public
		 * @return integer
		 */
		public function getBitRate()
		{
			$this->_getMediaData();
			return isset($this->_media_data['bitrate']) ? $this->_media_data['bitrate'] : -1;
		}
		
		/**
		 * Return the bit rate of the video in bits per second.
		 * NOTE: This only works for files with constant bit rate.
		 * @access public
		 * @return integer
		 */
		public function getVideoBitRate()
		{
			return $this->hasVideo() && isset($this->_media_data['video']['bitrate']) ? $this->_media_data['video']['bitrate'] : -1;
		}
		
		/**
		 * Return the audio bit rate of the media file in bits per second.
		 * @access public
		 * @return integer
		 */
		public function getAudioBitRate()
		{
			return $this->hasAudio() && isset($this->_media_data['audio']['bitrate']) ? $this->_media_data['audio']['bitrate'] : -1;
		}
		
		/**
		 * Return the audio sample rate of the media file in bits per second.
		 * @access public
		 * @return integer
		 */
		public function getAudioSampleRate()
		{
			return $this->hasAudio() && isset($this->_media_data['audio']['sample_rate']) ? $this->_media_data['audio']['sample_rate'] : -1;
		}
		
		/**
		 * Return the name of the video codec used to encode this movie as a string.
		 * @access public
		 * @param boolean $return_all If true it will return all audio codecs found.
		 * @return mixed string | array
		 */
		public function getVideoCodec($return_all=false)
		{
			return $this->hasVideo() ? $this->_media_data['video']['codec'] : -1;
		}
		
		/**
		 * Return the name of the audio codec used to encode this movie as a string.
		 * @access public
		 * @param boolean $return_all If true it will return all audio codecs found.
		 * @return mixed string | array
		 */
		public function getAudioCodec()
		{
			return $this->hasAudio() ? $this->_media_data['audio']['codec'] : -1;
		}
		
		/**
		 * Return the number of audio channels in this movie as an integer.
		 * @access public
		 * @return integer
		 */
		public function getAudioChannels()
		{
			return $this->hasAudio();
		}
		
		/**
		 * Return boolean value indicating whether the movie has an audio stream.
		 * @access public
		 * @return boolean
		 */
		public function hasAudio()
		{
			$this->_getMediaData();
			return isset($this->_media_data['audio']);
		}
		
		/**
		 * Return boolean value indicating whether the movie has a video stream.
		 * @access public
		 * @return boolean
		 */
		public function hasVideo()
		{
			$this->_getMediaData();
			return isset($this->_media_data['video']);
		}
		
		public function convert()
		{
		}
		
		/**
		 * This will resize and return an new instance of PHPVideoToolkit_Video.
		 * @access public
		 * @return PHPVideoToolkit_Video
		 */
		public function resize($width, $height=null, $multi_pass_encode=false)
		{
// 			set the instance dimensions
			$this->_toolkit->setVideoDimensions($width, $height);
// 			create a new video toolkit object to return
			return new PHPVideoToolkit_Video($this->_toolkit, $this->_tmp_directory);
		}
		
		public function watermark()
		{
		}
		
		public function getAudio()
		{
		}
		
		public function save($filename, $multi_pass_encode=false, $overwrite_mode=PHPVideoToolkit::OVERWRITE_FAIL)
		{
			$output_directory, $output_name, 
			$path_info = pathinfo($filename);
// 			set the output
			$this->_toolkit->setOutput($path_info['dirname'], $path_parts['basename'], $overwrite_mode);
			return $this->_toolkit->execute($multi_pass_encode, false);
		}
		
		/**
		 * Returns a frame from the movie as an PHPVideoToolkit_Frame object. 
		 * Returns false if the frame was not found.
		 * @access public
		 * @return mixed boolean | PHPVideoToolkit_Frame
		 */
		public function getFrame($frame_number=false)
		{
			if(!$this->hasVideo())
			{
				return false;
			}
			$this->_toolkit->reset(true);
			require_once dirname(__FILE__).DS.'frame.php';
			if(!$frame_number)
			{
				$frame_number = $this->_frame_index;
				$this->_frame_index += 1;
			}
			else
			{
				$this->_frame_index = $frame_number;
			}
// 			check the frame required exists in the video
			if($frame_number > $this->getFrameCount())
			{
				return false;
			}
// 			work out the exact frame to take
			$frame_rate = $this->getFrameRate();
// 			generate a unique name
			$this->_toolkit->setOutput($this->_tmp_directory, $this->_toolkit->unique().'-%timecode.jpg', PHPVideoToolkit::OVERWRITE_EXISTING);
// 			extract the frame and check the extract is ok
			if(!($result = $this->_toolkit->extractFrame($frame_number, $frame_rate, '%ft'))
			{
				return $result;
			}
// 			return the PHPVideoToolkit_Frame instance
			return new PHPVideoToolkit_Frame($this->_toolkit);
		}
		
		/**
		 * Returns the next key frame from the movie as an PHPVideoToolkit_Frame object. 
		 * Returns false if the frame was not found.
		 * @uses PHPVideoToolkit_Video::getFrame();
		 * @access public
		 * @return mixed boolean | PHPVideoToolkit_Frame
		 */
		public function getNextKeyFrame()
		{
			$frame_rate 	= $this->getFrameRate();
// 			work out the next frame
			$current_second = floor($frame_number/$frame_rate);
			$excess			= $frame_number-($seconds * $frame_rate);
			$frames_to_next = $frame_rate-$excess;
			$this->_frame_index += $frames_to_next;
// 			get the frame
			return $this->getFrame();
		}
		
		/**
		 * Return the current frame index.
		 * @access public
		 * @return integer
		 */
		public function getFrameNumber()
		{
			return $this->_frame_index;
		}
	}
