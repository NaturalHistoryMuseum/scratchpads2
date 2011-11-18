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
	
	/**
	 * This is similar in terms to FFmpeg-PHP's ffmpeg_frame, however the constructor can accept any number of items such as 
	 * a GD image, an image file, or an instance of PHPVideoToolkit, note if using an instance of PHPVideoToolkit you must
	 * have already specified the frame you wish to extract with PHPVideoToolkit::extractFrame() or set 
	 */
	class PHPVideoToolkit_Frame
	{
		
		private $_resource 			= null;
		private $_gd_resource		= null;
		private $_resource_type		= null;
		private $_width 			= null;
		private $_height 			= null;
		private $_timecode			= null;
		
		/**
		 * Class Constructor
		 * @param resource $gd_resource A GD image resource.
		 * @param integer $timecode The time in seconds of the frame. If false and the $resource is a PHPVideoToolkit instance then the timecode will be retrieved from
		 * 	the instance. However if $timecode is set and the $resource is a PHPVideoToolkit the $timecode will override the time retrieved from the instance.
		 */
		function __construct($resource, $timecode=false)
		{
// 			is this a gd resource
			if(is_resource($resource) && get_resource_type($resource) == 'gd')
			{
				$this->_gd_resource		= $resource;
				$this->_resource_type 	= 'gd';
				$this->_width  			= imagesx($resource);
				$this->_height 			= imagesy($resource);
				$this->_timecode		= $timecode;
			}
// 			is this a gd resource
			else if(is_string($resource) && is_file($resource))
			{
				$this->_resource_type 	= 'file';
				$dimensions				= getimagesize($resource);
				$this->_width  			= $dimensions[0];
				$this->_height 			= $dimensions[1];
				$this->_timecode		= $timecode;
			}
// 			check for PHPVideoToolkit class instance
			else if(get_class($resource) == 'PHPVideoToolkit')
			{
				$info = $resource->getFileInfo();
				if(!$info)
				{
// 					throw error
				}
				if(!isset($info['video']))
				{
// 					throw error
				}
				$this->_resource 		= $resource;
				$this->_resource_type 	= 'toolkit';
				$this->_width  			= $info['video']['dimensions']['width'];
				$this->_height 			= $info['video']['dimensions']['height'];
// 				set the timecode, the $timecode value will override
				$set_timecode			= $resource->hasCommand('-ss');
				$this->_timecode		= $timecode === false && $set_timecode !== false ? $set_timecode : $timecode;
			}
// 			isn't a valid resource type so throw error
			else
			{
// 				throw error
			}
		}
		
		/**
		 * Destroys any gd resource if made
		 */
		function __destruct()
		{
			if($this->_resource_type == 'gd' && is_resource($this->_resource))
			{
				imagedestroy($this->_resource);
			}
		}
		
		/**
		 * Determines if the resource supplied to the frame is valid.
		 * @access public
		 * @return integer
		 */
		public function hasValidResource()
		{
			return $this->_resource_type !== null;
		}
		
		/**
		 * Return the width of the frame.
		 * @access public
		 * @return integer
		 */
		public function getWidth()
		{
			return $this->_width;
		}
		
		/**
		 * Return the height of the frame.
		 * @access public
		 * @return integer
		 */
		public function getHeight()
		{
			return $this->_height;
		}
		
		/**
		 * Return the presentation time stamp of the frame.
		 * @access public
		 * @uses ffmpeg_frame::getPTS()
		 * @return integer
		 */
		public function getPresentationTimestamp()
		{
			return $this->getPTS();
		}
		
		/**
		 * Return the presentation time stamp of the frame.
		 * @access public
		 * @return integer
		 */
		public function getPTS()
		{
			return $this->_timecode;
		}
		
		/**
		 * Determines if the current frame is a keyframe.
		 * @access public
		 * @return integer
		 */
		public function isKeyFrame()
		{
			return false;
		}
		
		/**
		 * Resize and optionally crop the frame.
		 * NOTE 1: Cropping is always applied to the frame before it is resized.
		 * NOTE 2: Crop values must be even numbers.
		 * @access public
		 * @param integer $width New width of the frame (must be an even number).
		 * @param integer $height New height of the frame (must be an even number).
		 * @param integer $crop_top Remove [croptop] rows of pixels from the top of the frame.
		 * @param integer $crop_bottom Remove [cropbottom] rows of pixels from the bottom of the frame.
		 * @param integer $crop_left Remove [cropleft] rows of pixels from the left of the frame.
		 * @param integer $crop_right Remove [cropright] rows of pixels from the right of the frame. 
		 * @return boolean
		 */
		public function resize($width, $height, $crop_top=false, $crop_bottom=false, $crop_left=false, $crop_right=false)
		{
// 			generate a GD resource
			$this->_generateGDImageFromResource();
// 			are we cropping?
			if($crop_top !== false || $crop_bottom !== false || $crop_left !== false || $crop_right !== false)
			{
// 				crop and check it went ok
				if(!$this->crop($crop_top, $crop_bottom, $crop_left, $crop_right))
				{
					return false;
				}
			}
// 			check the width and height
			if($width <= 0 || $height <= 0)
			{
				return false;
			}
// 			now resize what we have
			$resize_resource = imagecreatetruecolor($width, $height);
// 			copy the portion we want
			imagecopyresampled($resize_resource, $this->_gd_resource, 0, 0, 0, 0, $width, $height, $this->_width, $this->_height);
// 			destroy the old crop resource to free up memory
			imagedestroy($this->_gd_resource);
// 			save the new resource
			$this->_gd_resource = $resize_resource;
// 			update the saved width and height
			$this->_width  	= $width;
			$this->_height 	= $height;
			return true;
		}
		
		/**
		 * Crop the frame.
		 * @access public
		 * @param integer $crop_top Remove [croptop] rows of pixels from the top of the frame.
		 * @param integer $crop_bottom Remove [cropbottom] rows of pixels from the bottom of the frame.
		 * @param integer $crop_left Remove [cropleft] rows of pixels from the left of the frame.
		 * @param integer $crop_right Remove [cropright] rows of pixels from the right of the frame. 
		 * @return boolean
		 */
		public function crop($crop_top=false, $crop_bottom=false, $crop_left=false, $crop_right=false)
		{
// 			generate a GD resource
			$this->_generateGDImageFromResource();
// 			work out the newwidth and height and positions
			$w = $this->_width;
			$h = $this->_height;
			$x = 0;
			$y = 0;
			$x_bottom_chord = 0;
			if($crop_top !== false)
			{
				$x = $crop_top;
				$h -= $crop_top;
			}
			if($crop_bottom !== false)
			{
				$h -= $crop_bottom;
			}
			if($crop_left !== false)
			{
				$y = $crop_left;
				$w -= $crop_left;
			}
			if($crop_right !== false)
			{
				$w -= $crop_left;
			}
// 			is the width and height greater than 0
			if($w < 0 || $h < 0)
			{
				return false;
			}
// 			create the new image resource
			$crop_resource = imagecreatetruecolor($w, $h);
// 			copy the portion we want
			imagecopyresampled($crop_resource, $this->_gd_resource, 0, 0, $x, $y, $w, $h, $w, $h);
// 			destroy the old resource to free up memory
			imagedestroy($this->_gd_resource);
// 			save the new resource
			$this->_gd_resource = $crop_resource;
// 			update the saved width and height
			$this->_width  	= $w;
			$this->_height 	= $h;
			return true;
		}
		
		/**
		 * Returns a truecolor GD image of the frame.
		 * @access public
		 * @return resource Returns a GD resource.
		 */
		public function toGDImage()
		{
			$this->_generateGDImageFromResource()
			return $this->_gd_resource;
		}
		
		/**
		 * Returns a GD resource from the current resource type.
		 * @access private
		 */
		private function _generateGDImageFromResource()
		{
// 			don't do this if the gd resource is already defined
			if($this->_gd_resource === null)
			{
				switch($this->_resource_type)
				{
					case 'toolkit' :
						$result = $this->_resource->execute(false, false);
// 						check the return value in-case of error
						if($result !== PHPVideoToolkit::RESULT_OK)
						{
// 							throw error
						}
						$img = array_shift($this->_resource->getLastOutput());
						if(!is_file($img))
						{
// 							throw error
						}
						$this->_gd_resource = imagecreatefromjpeg($img);
						break;
						
					case 'file' :
						$path_info = pathinfo($this->_resource);
						switch(strtolower($path_info['extension']))
						{
							case 'jpeg' :
							case 'jpg' :
								$this->_gd_resource = imagecreatefromjpeg($this->_resource);
								break;
							case 'gif' :
								$this->_gd_resource = imagecreatefromgif($this->_resource);
								break;
							case 'png' :
								$this->_gd_resource = imagecreatefrompng($this->_resource);
								break;
							default :
// 								throw error
						}
						break;
						
					case 'gd' :
// 						resource is already gd
						break;
				}
			}
		}
		
	}
