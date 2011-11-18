<?php

	/* SVN FILE: $Id$ */

	/**
	 * @author Oliver Lillie (aka buggedcom) <publicmail@buggedcom.co.uk>
	 *
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
	 *
	 * @package PHPVideoToolkit (was called ffmpeg)
	 * @version 0.1.8
	 * @changelog SEE CHANGELOG
	 * @abstract This class can be used in conjunction with several server binary libraries to manipulate video and audio
	 * through PHP. It is not intended to solve any particular problems, however you may find it useful. This php class
	 * is in no way associated with the actual FFmpeg releases. Any mistakes contained in this php class are mine and mine
	 * alone.
	 *
	 * Please Note: There are several prerequisites that are required before this class can be used as an aid to manipulate
	 * video and audio. You must at the very least have FFMPEG compiled on your server. If you wish to use this class for FLV
	 * manipulation you must compile FFMPEG with LAME and Ruby's FLVTOOL2. I cannot answer questions regarding the install of
	 * the server binaries needed by this class. I had too learn the hard way and it isn't easy, however it is a good learning
	 * experience. For those of you who do need help read the install.txt file supplied along side this class. It wasn't written
	 * by me however I found it useful when installing ffmpeg for the first time. The original source for the install.txt file
	 * is located http://www.luar.com.hk/blog/?p=669 and the author is Lunar.
	 *
	 * @see install.txt
	 *
	 * @uses ffmpeg http://ffmpeg.sourceforge.net/
	 * @uses lame http://lame.sourceforge.net/
	 * @uses flvtool2 http://www.inlet-media.de/flvtool2 (and ruby http://www.ruby-lang.org/en/)
	 *
	 * @config examples/example-config.php Please edit this files in order for the examples to work.
	 * @example examples/example01.php Converts video to Flash Video (ie FLV).
	 * @example examples/example02.php Screen grabs video frames.
	 * @example examples/example03.php Compile a movie from multiple jpegs
	 * @example examples/example04.php Watermark a video.
	 * @example examples/example05.php Access media metadata without using the ffmpeg-php library.
	 * @example examples/example06.php Extract audio from video.
	 * @example examples/example07.php Join multiple videos together.
	 * @example examples/example08.php Easy video conversion to common formats using the adapters.
	 * @example examples/example09.php Shows you how to access the information about your ffmpeg installation.
	 * @example examples/example10.php Shows you how to extract a specific frame from a movie.
	 * @example examples/example11.php Shows you how to use the ffmpeg-php adapters to provide a pure php emulation of ffmpeg-php.
	 * @example examples/example12.php Shows you how to manipulate/format timecode strings.
	 * @example examples/example13.php This demonstrates how to simply create a FLV stream script.
	 */

	if(!defined('DS'))
	{
		define('DS', DIRECTORY_SEPARATOR);
	}
	
	/**
	 * Set the ffmpeg binary path
	 */
	if(!defined('PHPVIDEOTOOLKIT_FFMPEG_BINARY'))
	{
		define('PHPVIDEOTOOLKIT_FFMPEG_BINARY', '/usr/local/bin/ffmpeg');
	}
	/**
	 * Set the flvtool2 binary path
	 */
	if(!defined('PHPVIDEOTOOLKIT_FLVTOOLS_BINARY'))
	{
		define('PHPVIDEOTOOLKIT_FLVTOOLS_BINARY', '/usr/bin/flvtool2');
	}
	/**
	 * Set the watermark vhook path
	 */
	if(!defined('PHPVIDEOTOOLKIT_FFMPEG_WATERMARK_VHOOK'))
	{
		define('PHPVIDEOTOOLKIT_FFMPEG_WATERMARK_VHOOK', '/usr/local/lib/vhook/watermark.so');
	}
	/**
	 * Set the memcoder path
	 */
	if(!defined('PHPVIDEOTOOLKIT_MENCODER_BINARY'))
	{
		define('PHPVIDEOTOOLKIT_MENCODER_BINARY', '/usr/local/bin/mencoder');
	}
	
	/**
	* Codec support constants
	*/
   define('PHPVIDEOTOOLKIT_ENCODE', 'encode');
   define('PHPVIDEOTOOLKIT_DECODE', 'decode'); 
		
	/**
	 * Process Results from PHPVideoToolkit::execute
	 */
// 	any return value with this means everything is ok
	define('PHPVIDEOTOOLKIT_RESULT_OK', true);
// 	any return value with this means the file has been processed/converted ok however it was 
// 	not able to be written to the output address. If this occurs you will need to move the
// 	processed file manually from the temp location
	define('PHPVIDEOTOOLKIT_RESULT_OK_BUT_UNWRITABLE', -1);
	
	/**
	 * Overwrite constants used in setOutput
	 */
	define('PHPVIDEOTOOLKIT_OVERWRITE_FAIL', 'fail');
	define('PHPVIDEOTOOLKIT_OVERWRITE_PRESERVE', 'preserve');
	define('PHPVIDEOTOOLKIT_OVERWRITE_EXISTING', 'existing');
	define('PHPVIDEOTOOLKIT_OVERWRITE_UNIQUE', 'unique');
	
	/**
	 * Formats supported
	 * 3g2             3gp2 format
	 * 3gp             3gp format
	 * aac             ADTS AAC
	 * aiff            Audio IFF
	 * amr             3gpp amr file format
	 * asf             asf format
	 * avi             avi format
	 * flv             flv format
	 * gif             GIF Animation
	 * mov             mov format
	 * mov,mp4,m4a,3gp,3g2,mj2 QuickTime/MPEG4/Motion JPEG 2000 format
	 * mp2             MPEG audio layer 2
	 * mp3             MPEG audio layer 3
	 * mp4             mp4 format
	 * mpeg            MPEG1 System format
	 * mpeg1video      MPEG video
	 * mpeg2video      MPEG2 video
	 * mpegvideo       MPEG video
	 * psp             psp mp4 format
	 * rm              rm format
	 * swf             Flash format
	 * vob             MPEG2 PS format (VOB)
	 * wav             wav format
	 * jpeg            mjpeg format
	 * yuv4mpegpipe    yuv4mpegpipe format
	 */
	define('PHPVIDEOTOOLKIT_FORMAT_3GP2', '3g2');
	define('PHPVIDEOTOOLKIT_FORMAT_3GP', '3gp');
	define('PHPVIDEOTOOLKIT_FORMAT_AAC', 'aac');
	define('PHPVIDEOTOOLKIT_FORMAT_AIFF', 'aiff');
	define('PHPVIDEOTOOLKIT_FORMAT_AMR', 'amr');
	define('PHPVIDEOTOOLKIT_FORMAT_ASF', 'asf');
	define('PHPVIDEOTOOLKIT_FORMAT_AVI', 'avi');
	define('PHPVIDEOTOOLKIT_FORMAT_FLV', 'flv');
	define('PHPVIDEOTOOLKIT_FORMAT_GIF', 'gif');
	define('PHPVIDEOTOOLKIT_FORMAT_MJ2', 'mj2');
	define('PHPVIDEOTOOLKIT_FORMAT_MP2', 'mp2');
	define('PHPVIDEOTOOLKIT_FORMAT_MP3', 'mp3');
	define('PHPVIDEOTOOLKIT_FORMAT_MP4', 'mp4');
	define('PHPVIDEOTOOLKIT_FORMAT_MPEG4', 'mpeg4');
	define('PHPVIDEOTOOLKIT_FORMAT_M4A', 'm4a');
	define('PHPVIDEOTOOLKIT_FORMAT_MPEG', 'mpeg');
	define('PHPVIDEOTOOLKIT_FORMAT_MPEG1', 'mpeg1video');
	define('PHPVIDEOTOOLKIT_FORMAT_MPEG2', 'mpeg2video');
	define('PHPVIDEOTOOLKIT_FORMAT_MPEGVIDEO', 'mpegvideo');
	define('PHPVIDEOTOOLKIT_FORMAT_PSP', 'psp');
	define('PHPVIDEOTOOLKIT_FORMAT_RM', 'rm');
	define('PHPVIDEOTOOLKIT_FORMAT_SWF', 'swf');
	define('PHPVIDEOTOOLKIT_FORMAT_VOB', 'vob');
	define('PHPVIDEOTOOLKIT_FORMAT_WAV', 'wav');
	define('PHPVIDEOTOOLKIT_FORMAT_JPG', 'mjpeg');
	define('PHPVIDEOTOOLKIT_FORMAT_Y4MP', 'yuv4mpegpipe');
	
	/**
	 * Size Presets
	 */
	define('PHPVIDEOTOOLKIT_SIZE_SAS', 'SameAsSource');
	define('PHPVIDEOTOOLKIT_SIZE_SQCIF', '128x96');
	define('PHPVIDEOTOOLKIT_SIZE_QCIF', '176x144');
	define('PHPVIDEOTOOLKIT_SIZE_CIF', '352x288');
	define('PHPVIDEOTOOLKIT_SIZE_4CIF', '704x576');
	define('PHPVIDEOTOOLKIT_SIZE_QQVGA', '160x120');
	define('PHPVIDEOTOOLKIT_SIZE_QVGA', '320x240');
	define('PHPVIDEOTOOLKIT_SIZE_VGA', '640x480');
	define('PHPVIDEOTOOLKIT_SIZE_SVGA', '800x600');
	define('PHPVIDEOTOOLKIT_SIZE_XGA', '1024x768');
	define('PHPVIDEOTOOLKIT_SIZE_UXGA', '1600x1200');
	define('PHPVIDEOTOOLKIT_SIZE_QXGA', '2048x1536');
	define('PHPVIDEOTOOLKIT_SIZE_SXGA', '1280x1024');
	define('PHPVIDEOTOOLKIT_SIZE_QSXGA', '2560x2048');
	define('PHPVIDEOTOOLKIT_SIZE_HSXGA', '5120x4096');
	define('PHPVIDEOTOOLKIT_SIZE_WVGA', '852x480');
	define('PHPVIDEOTOOLKIT_SIZE_WXGA', '1366x768');
	define('PHPVIDEOTOOLKIT_SIZE_WSXGA', '1600x1024');
	define('PHPVIDEOTOOLKIT_SIZE_WUXGA', '1920x1200');
	define('PHPVIDEOTOOLKIT_SIZE_WOXGA', '2560x1600');
	define('PHPVIDEOTOOLKIT_SIZE_WQSXGA', '3200x2048');
	define('PHPVIDEOTOOLKIT_SIZE_WQUXGA', '3840x2400');
	define('PHPVIDEOTOOLKIT_SIZE_WHSXGA', '6400x4096');
	define('PHPVIDEOTOOLKIT_SIZE_WHUXGA', '7680x4800');
	define('PHPVIDEOTOOLKIT_SIZE_CGA', '320x200');
	define('PHPVIDEOTOOLKIT_SIZE_EGA', '640x350');
	define('PHPVIDEOTOOLKIT_SIZE_HD480', '852x480');
	define('PHPVIDEOTOOLKIT_SIZE_HD720', '1280x720');
	define('PHPVIDEOTOOLKIT_SIZE_HD1080', '1920x1080');
	
	/**
	 * Ratio Presets
	 */
	define('PHPVIDEOTOOLKIT_RATIO_STANDARD', '4:3');
	define('PHPVIDEOTOOLKIT_RATIO_WIDE', '16:9');
	define('PHPVIDEOTOOLKIT_RATIO_CINEMATIC', '1.85');
	
		
	/**
	 * Audio Channel Presets
	 */
	define('PHPVIDEOTOOLKIT_AUDIO_STEREO', 2);
	define('PHPVIDEOTOOLKIT_AUDIO_MONO', 1);

	/**
	 * @author Oliver Lillie (aka buggedcom) <publicmail@buggedcom.co.uk>
	 * @license BSD
	 * @package PHPVideoToolkit (was called ffmpeg)
	 */
	class PHPVideoToolkit
	{
		
		var $version = '0.1.8';
		
		/**
		 * Error strings
		 */
		var $_messages = array(
			
			'generic_temp_404' 								=> 'The temporary directory does not exist.',
			'generic_temp_writable' 						=> 'The temporary directory is not write-able by the web server.',
			
			'getFileInfo_no_input' 							=> 'Input file does not exist so no information can be retrieved.',
			'inputFileHasVideo_no_input' 					=> 'Input file does not exist so no information can be retrieved.',
			'inputFileHasAudio_no_input' 					=> 'Input file does not exist so no information can be retrieved.',
			'setInputFile_file_existence' 					=> 'Input file "#file" does not exist',
			'extractAudio_valid_format' 					=> 'Value "#format" set from $toolkit->extractAudio, is not a valid audio format. Valid values ffmpeg PHPVIDEOTOOLKIT_FORMAT_AAC, PHPVIDEOTOOLKIT_FORMAT_AIFF, PHPVIDEOTOOLKIT_FORMAT_MP2, PHPVIDEOTOOLKIT_FORMAT_MP3, PHPVIDEOTOOLKIT_FORMAT_MP4, PHPVIDEOTOOLKIT_FORMAT_MPEG4, PHPVIDEOTOOLKIT_FORMAT_M4A or PHPVIDEOTOOLKIT_FORMAT_WAV. If you wish to specifically try to set another format you should use the advanced function $toolkit->addCommand. Set $command to "-f" and $argument to your required value.',
			'extractFrame_video_frame_rate_404' 			=> 'You have attempted to extract a thumbnail from a video while automagically guessing the framerate of the video, but the framerate could not be accessed. You can remove this error by manually setting the frame rate of the video.',
			'extractFrame_video_info_404' 					=> 'You have attempted to extract a thumbnail from a video and check to see if the thumbnail exists, however it was not possible to access the video information. Please check your temporary directory permissions for read/write access by the webserver.',
			'extractFrame_video_frame_count' 				=> 'You have attempted to extract a thumbnail from a video but the thumbnail you are trying to extract does not exist in the video.',
			'extractFrames_video_begin_frame_count' 		=> 'You have attempted to extract thumbnails from a video but the thumbnail you are trying to start the extraction from does not exist in the video.',
			'extractFrames_video_end_frame_count' 			=> 'You have attempted to extract thumbnails from a video but the thumbnail you are trying to end the extraction at does not exist in the video.',
			'setFormat_valid_format' 						=> 'Value "#format" set from $toolkit->setFormat, is not a valid format. Valid values are PHPVIDEOTOOLKIT_FORMAT_3GP2, PHPVIDEOTOOLKIT_FORMAT_3GP, PHPVIDEOTOOLKIT_FORMAT_AAC, PHPVIDEOTOOLKIT_FORMAT_AIFF, PHPVIDEOTOOLKIT_FORMAT_AMR, PHPVIDEOTOOLKIT_FORMAT_ASF, PHPVIDEOTOOLKIT_FORMAT_AVI, PHPVIDEOTOOLKIT_FORMAT_FLV, PHPVIDEOTOOLKIT_FORMAT_GIF, PHPVIDEOTOOLKIT_FORMAT_MJ2, PHPVIDEOTOOLKIT_FORMAT_MP2, PHPVIDEOTOOLKIT_FORMAT_MP3, PHPVIDEOTOOLKIT_FORMAT_MP4, PHPVIDEOTOOLKIT_FORMAT_MPEG4, PHPVIDEOTOOLKIT_FORMAT_M4A, PHPVIDEOTOOLKIT_FORMAT_MPEG, PHPVIDEOTOOLKIT_FORMAT_MPEG1, PHPVIDEOTOOLKIT_FORMAT_MPEG2, PHPVIDEOTOOLKIT_FORMAT_MPEGVIDEO, PHPVIDEOTOOLKIT_FORMAT_PSP, PHPVIDEOTOOLKIT_FORMAT_RM, PHPVIDEOTOOLKIT_FORMAT_SWF, PHPVIDEOTOOLKIT_FORMAT_VOB, PHPVIDEOTOOLKIT_FORMAT_WAV, PHPVIDEOTOOLKIT_FORMAT_JPG. If you wish to specifically try to set another format you should use the advanced function $toolkit->addCommand. Set $command to "-f" and $argument to your required value.',
			'setAudioChannels_valid_channels' 				=> 'Value "#channels" set from $toolkit->setAudioChannels, is not a valid integer. Valid values are 1, or 2. If you wish to specifically try to set another channels value you should use the advanced function $toolkit->addCommand. Set $command to "-ac" and $argument to your required value.',
			'setAudioSampleFrequency_valid_frequency' 		=> 'Value "#frequency" set from $toolkit->setAudioSampleFrequency, is not a valid integer. Valid values are 11025, 22050, 44100. If you wish to specifically try to set another frequency you should use the advanced function $toolkit->addCommand. Set $command to "-ar" and $argument to your required value.',
			'setAudioFormat_valid_format' 					=> 'Value "#format" set from $toolkit->setAudioFormat, is not a valid format. Valid values are PHPVIDEOTOOLKIT_FORMAT_AAC, PHPVIDEOTOOLKIT_FORMAT_AIFF, PHPVIDEOTOOLKIT_FORMAT_AMR, PHPVIDEOTOOLKIT_FORMAT_ASF, PHPVIDEOTOOLKIT_FORMAT_MP2, PHPVIDEOTOOLKIT_FORMAT_MP3, PHPVIDEOTOOLKIT_FORMAT_MP4, PHPVIDEOTOOLKIT_FORMAT_MPEG2, PHPVIDEOTOOLKIT_FORMAT_RM, PHPVIDEOTOOLKIT_FORMAT_WAV. If you wish to specifically try to set another format you should use the advanced function $toolkit->addCommand. Set $command to "-acodec" and $argument to your required value.',
			'setVideoFormat_valid_format' 					=> 'Value "#format" set from $toolkit->setAudioFormat, is not a valid format. Valid values are PHPVIDEOTOOLKIT_FORMAT_3GP2, PHPVIDEOTOOLKIT_FORMAT_3GP, PHPVIDEOTOOLKIT_FORMAT_AVI, PHPVIDEOTOOLKIT_FORMAT_FLV, PHPVIDEOTOOLKIT_FORMAT_GIF, PHPVIDEOTOOLKIT_FORMAT_MJ2, PHPVIDEOTOOLKIT_FORMAT_MP4, PHPVIDEOTOOLKIT_FORMAT_MPEG4, PHPVIDEOTOOLKIT_FORMAT_M4A, PHPVIDEOTOOLKIT_FORMAT_MPEG, PHPVIDEOTOOLKIT_FORMAT_MPEG1, PHPVIDEOTOOLKIT_FORMAT_MPEG2, PHPVIDEOTOOLKIT_FORMAT_MPEGVIDEO. If you wish to specifically try to set another format you should use the advanced function $toolkit->addCommand. Set $command to "-vcodec" and $argument to your required value.',
			'setAudioBitRate_valid_bitrate' 				=> 'Value "#bitrate" set from $toolkit->setAudioBitRate, is not a valid integer. Valid values are 16, 32, 64, 128. If you wish to specifically try to set another bitrate you should use the advanced function $toolkit->addCommand. Set $command to "-ab" and $argument to your required value.',
			'prepareImagesForConversionToVideo_one_img' 	=> 'When compiling a movie from a series of images, you must include at least one image.',
			'prepareImagesForConversionToVideo_img_404' 	=> '"#img" does not exist.',
			'prepareImagesForConversionToVideo_img_copy' 	=> '"#img" can not be copied to "#tmpfile"',
			'prepareImagesForConversionToVideo_img_type' 	=> 'The images can not be prepared for conversion to video. Please make sure all images are of the same type, ie gif, png, jpeg and then try again.',
			'setVideoOutputDimensions_valid_format' 		=> 'Value "#format" set from $toolkit->setVideoOutputDimensions, is not a valid preset dimension. Valid values are PHPVIDEOTOOLKIT_SIZE_SQCIF, PHPVIDEOTOOLKIT_SIZE_SAS, PHPVIDEOTOOLKIT_SIZE_QCIF, PHPVIDEOTOOLKIT_SIZE_CIF, PHPVIDEOTOOLKIT_SIZE_4CIF, PHPVIDEOTOOLKIT_SIZE_QQVGA, PHPVIDEOTOOLKIT_SIZE_QVGA, PHPVIDEOTOOLKIT_SIZE_VGA, PHPVIDEOTOOLKIT_SIZE_SVGA, PHPVIDEOTOOLKIT_SIZE_XGA, PHPVIDEOTOOLKIT_SIZE_UXGA, PHPVIDEOTOOLKIT_SIZE_QXGA, PHPVIDEOTOOLKIT_SIZE_SXGA, PHPVIDEOTOOLKIT_SIZE_QSXGA, PHPVIDEOTOOLKIT_SIZE_HSXGA, PHPVIDEOTOOLKIT_SIZE_WVGA, PHPVIDEOTOOLKIT_SIZE_WXGA, PHPVIDEOTOOLKIT_SIZE_WSXGA, PHPVIDEOTOOLKIT_SIZE_WUXGA, PHPVIDEOTOOLKIT_SIZE_WOXGA, PHPVIDEOTOOLKIT_SIZE_WQSXGA, PHPVIDEOTOOLKIT_SIZE_WQUXGA, PHPVIDEOTOOLKIT_SIZE_WHSXGA, PHPVIDEOTOOLKIT_SIZE_WHUXGA, PHPVIDEOTOOLKIT_SIZE_CGA, PHPVIDEOTOOLKIT_SIZE_EGA, PHPVIDEOTOOLKIT_SIZE_HD480, PHPVIDEOTOOLKIT_SIZE_HD720, PHPVIDEOTOOLKIT_SIZE_HD1080. You can also manually set the width and height.',
			'setVideoOutputDimensions_sas_dim' 				=> 'It was not possible to determine the input video dimensions so it was not possible to continue. If you wish to override this error please change the call to setVideoOutputDimensions and add a true argument to the arguments list... setVideoOutputDimensions(PHPVIDEOTOOLKIT_SIZE_SAS, true);',
			'setVideoOutputDimensions_valid_integer' 		=> 'You tried to set the video output dimensions to an odd number. FFmpeg requires that the video output dimensions are of event value and divisible by 2. ie 2, 4, 6,... etc',
			'setVideoAspectRatio_valid_ratio' 				=> 'Value "#ratio" set from $toolkit->setVideoOutputDimensions, is not a valid preset dimension. Valid values are PHPVIDEOTOOLKIT_RATIO_STANDARD, PHPVIDEOTOOLKIT_RATIO_WIDE, PHPVIDEOTOOLKIT_RATIO_CINEMATIC. If you wish to specifically try to set another video aspect ratio you should use the advanced function $toolkit->addCommand. Set $command to "-aspect" and $argument to your required value.',
			'addWatermark_img_404' 							=> 'Watermark file "#watermark" does not exist.',
			'addWatermark_vhook_disabled' 					=> 'Vhooking is not enabled in your FFmpeg binary. In order to allow video watermarking you must have FFmpeg compiled with --enable-vhook set. You can however watermark any extracted images using GD. To enable frame watermarking, call $toolkit->addGDWatermark($file) before you execute the extraction.',
			'addVideo_file_404' 							=> 'File "#file" does not exist.',
			'setOutput_output_dir_404' 						=> 'Output directory "#dir" does not exist!',
			'setOutput_output_dir_writable' 				=> 'Output directory "#dir" is not writable!',
			'setOutput_%_missing' 							=> 'The output of this command will be images yet you have not included the "%index" or "%timecode" in the $output_name.',
			'setOutput_%d_depreciated' 						=> 'The use of %d in the output file name is now depreciated. Please use %index. Number padding is still supported. You may also use %timecode instead to add a timecode to the filename.',
			'execute_input_404' 							=> 'Execute error. Input file missing.',
			'execute_output_not_set' 						=> 'Execute error. Output not set.',
			'execute_temp_unwritable'		   				=> 'Execute error. The tmp directory supplied is not writable.',
			'execute_overwrite_process' 					=> 'Execute error. A file exists in the temp directory and is of the same name as this process file. It will conflict with this conversion. Conversion stopped.',
			'execute_overwrite_fail' 						=> 'Execute error. Output file exists. Process halted. If you wish to automatically overwrite files set the third argument in "PHPVideoToolkit::setOutput();" to "PHPVIDEOTOOLKIT_OVERWRITE_EXISTING".',
			'execute_partial_error' 						=> 'Execute error. Output for file "#input" encountered a partial error. Files were generated, however one or more of them were empty.',
			'execute_image_error' 							=> 'Execute error. Output for file "#input" was not found. No images were generated.',
			'execute_output_404' 							=> 'Execute error. Output for file "#input" was not found. Please check server write permissions and/or available codecs compiled with FFmpeg. You can check the encode decode availability by inspecting the output array from PHPVideoToolkit::getFFmpegInfo().',
			'execute_output_empty' 							=> 'Execute error. Output for file "#input" was found, but the file contained no data. Please check the available codecs compiled with FFmpeg can support this type of conversion. You can check the encode decode availability by inspecting the output array from PHPVideoToolkit::getFFmpegInfo().',
			'execute_image_file_exists'						=> 'Execute error. There is a file name conflict. The file "#file" already exists in the filesystem. If you wish to automatically overwrite files set the third argument in "PHPVideoToolkit::setOutput();" to "PHPVIDEOTOOLKIT_OVERWRITE_EXISTING".',
			'execute_result_ok_but_unwritable'				=> 'Process Partially Completed. The process successfully completed however it was not possible to output to "#output". The output was left in the temp directory "#process" for a manual file movement.',
			'execute_result_ok'								=> 'Process Completed. The process successfully completed. Output was generated to "#output".',
			
			'ffmpeg_log_ffmpeg_output'						=> 'OUTPUT',
			'ffmpeg_log_ffmpeg_result'						=> 'RESULT',
			'ffmpeg_log_ffmpeg_command'						=> 'COMMAND',
			'ffmpeg_log_ffmpeg_join_gunk'					=> 'FFMPEG JOIN OUTPUT',
			'ffmpeg_log_ffmpeg_gunk'						=> 'FFMPEG OUTPUT',
			'ffmpeg_log_separator'							=> '-------------------------------'
			
		);
		
		/**
		 * A public var that is to the information available about
		 * the current ffmpeg compiled binary.
		 * @var mixed
		 * @access public
		 */
		var $ffmpeg_info		= false;

		/**
		 * A public var that determines if the ffmpeg binary has been found. The default value
		 * is null unless getFFmpegInfo is called whereby depending on the results it is set to
		 * true or false
		 * @var mixed
		 * @access public
		 */
		var $ffmpeg_found		= null;

		/**
		 * A private var that contains the info of any file that is accessed by PHPVideoToolkit::getFileInfo();
		 * @var array
		 * @access private
		 */
		var $_file_info		= array();

		/**
		 * Determines what happens when an error occurs
		 * @var boolean If true then the script will die, if not false is return by the error
		 * @access public
		 */
		var $on_error_die			= false;

		/**
		 * Holds the log file name
		 * @var string
		 * @access private
		 */
		var $_log_file				= null;

		/**
		 * Determines if when outputting image frames if the outputted files should have the %d number
		 * replaced with the frames timecode.
		 * @var boolean If true then the files will be renamed.
		 * @access public
		 */
		var $image_output_timecode 	= true;

		/**
		 * Holds the timecode separator for when using $image_output_timecode = true
		 * Not all systems allow ':' in filenames.
		 * @var string
		 * @access public
		 */
		var $timecode_seperator_output = '-';

		/**
		 * Holds the starting time code when outputting image frames.
		 * @var string The timecode hh(n):mm:ss:ff
		 * @access private
		 */
		var $_image_output_timecode_start = '00:00:00.00';

		/**
		 * The format in which the image %timecode placeholder string is outputted.
		 * 	- %hh (hours) representative of hours
		 * 	- %mm (minutes) representative of minutes
		 * 	- %ss (seconds) representative of seconds
		 * 	- %fn (frame number) representative of frames (of the current second, not total frames)
		 * 	- %ms (milliseconds) representative of milliseconds (of the current second, not total milliseconds) (rounded to 3 decimal places)
		 * 	- %ft (frames total) representative of total frames (ie frame number)
		 * 	- %st (seconds total) representative of total seconds (rounded).
		 * 	- %sf (seconds floored) representative of total seconds (floored).
		 * 	- %mt (milliseconds total) representative of total milliseconds. (rounded to 3 decimal places)
		 * NOTE; there are special characters that will be replace by PHPVideoToolkit::$timecode_seperator_output, these characters are
		 * 	- :
		 *  - .
		 * @var string 
		 * @access public
		 */
		var $image_output_timecode_format = '%hh-%mm-%ss-%fn';

		/**
		 * Holds the fps of image extracts
		 * @var integer
		 * @access private
		 */
		var $_image_output_timecode_fps = 1;

		/**
		 * Holds the current execute commands that will need to be combined
		 * @var array
		 * @access private
		 */
		var $_commands 			= array();

		/**
		 * Holds the commands executed
		 * @var array
		 * @access private
		 */
		var $_processed 		= array();

		/**
		 * Holds the file references to those that have been processed
		 * @var array
		 * @access private
		 */
		var $_files	 			= array();

		/**
		 * Holds the errors encountered
		 * @access private
		 * @var array
		 */
		var $_errors 			= array();

		/**
		 * Holds the input file / input file sequence
		 * @access private
		 * @var string
		 */
		var $_input_file 		= null;

		/**
		 * Holds the output file / output file sequence
		 * @access private
		 * @var string
		 */
		var $_output_address 	= null;

		/**
		 * Holds the process file / process file sequence
		 * @access private
		 * @var string
		 */
		var $_process_address 	= null;

		/**
		 * Temporary filename prefix
		 * @access private
		 * @var string
		 */
		var $_tmp_file_prefix	= 'tmp_';

		/**
		 * Holds the temporary directory name
		 * @access private
		 * @var string
		 */
		var $_tmp_directory 	= null;

		/**
		 * Holds the directory paths that need to be removed by the ___destruct function
		 * @access private
		 * @var array
		 */
		var $_unlink_dirs		= array();

		/**
		 * Holds the file paths that need to be deleted by the ___destruct function
		 * @access private
		 * @var array
		 */
		var $_unlink_files		= array();

		/**
		 * Holds the timer start micro-float.
		 * @access private
		 * @var integer
		 */
		var $_timer_start		= 0;
		
		/**
		 * Holds the times taken to process each file.
		 * @access private
		 * @var array
		 */
		var $_timers			= array();

		/**
		 * Holds the times taken to process each file.
		 * @access private
		 * @var constant
		 */
		var $_overwrite_mode	= null;

		/**
		 * Holds a integer value that flags if the image extraction is just a single frame.
		 * @access private
		 * @var integer
		 */
		var $_single_frame_extraction	= null;

		/**
		 * Holds the watermark file that is used to watermark any outputted images via GD.
		 * @access private
		 * @var string
		 */
		var $_watermark_url	= null;

		/**
		 * Holds the watermark options used to watermark any outputted images via GD.
		 * @access private
		 * @var array
		 */
		var $_watermark_options	= null;

		/**
		 * Holds the number of files processed per run.
		 * @access private
		 * @var integer
		 */
		var $_process_file_count = 0;

		/**
		 * Holds the times taken to process each file.
		 * @access private
		 * @var array
		 */
		var $_post_processes	= array();
		
		/**
		 * Holds commands should be sent added to the exec before the input file, this is by no means a definitive list
		 * of all the ffmpeg commands, as it only utilizes the ones in use by this class. Also only commands that have 
		 * specific required places are entered in the arrays below. Anything not in these arrays will be treated as an 
		 * after-input item.
		 * @access private
		 * @var array
		 */
// 		var $_cmds_before_input		= array();
		var $_cmds_before_input		= array('-inputr');
// 		var $_cmds_before_input		= array('-r', '-f');

		/**
		 * Constructs the class and sets the temporary directory.
		 *
		 * @access private
		 * @param string $tmp_directory A full absolute path to you temporary directory
		 */
		function PHPVideoToolkit($tmp_directory='/tmp/')
		{
			$this->_tmp_directory = $tmp_directory;
// 			emulate php5 destructors
    		register_shutdown_function(array(&$this, '__destruct'));
		}
		
		function microtimeFloat()
		{
		    list($usec, $sec) = explode(" ", microtime());
		    return ((float) $usec + (float) $sec);
		}

		/**
		 * Resets the class
		 *
		 * @access public
		 * @param boolean $keep_input_file Determines whether or not to reset the input file currently set.
		 */
		function reset($keep_input_file=false, $keep_processes=false)
		{
			if($keep_input_file === false)
			{
				$this->_input_file = null;
			}
			if($keep_processes === false)
			{
				$this->_post_processes = array();
			}
			$this->_single_frame_extraction = null;
			$this->_output_address = null;
			$this->_process_address = null;
			$this->_log_file = null;
			$this->_commands = array();
			$this->_timer_start = 0;
			$this->_process_file_count = 0;
			$this->__destruct();
		}

		/**
		 * Returns information about the specified file without having to use ffmpeg-php
		 * as it consults the ffmpeg binary directly. 
		 * 
		 * @access public
		 * @param string $file The absolute path of the file that is required to be manipulated.
		 * @return mixed false on error encountered, true otherwise
		 **/
		function getFFmpegInfo()
		{
// 			check to see if this is a static call
			if(!$this)
			{     
				$toolkit = new PHPVideoToolkit($tmp_directory);
				return $toolkit->getFFmpegInfo();
			}
// 			check to see if the info has already been cached
			if($this->ffmpeg_info !== false)
			{
				return $this->ffmpeg_info;
			}
// 			check to see if this is a static call
			$format = '';
			$data = array();
// 			execute the ffmpeg lookup
			exec(PHPVIDEOTOOLKIT_FFMPEG_BINARY.' -formats 2>&1', $buffer);
			
			$this->ffmpeg_found = $data['ffmpeg-found']  = !(strpos($buffer[0], 'command not found') !== false || strpos($buffer[0], 'No such file or directory') !== false);
			$data['binary'] 	= array();
			$data['compiler'] 	= array();
			$data['ffmpeg-php-support'] = $this->hasFFmpegPHPSupport();
			$data['raw'] 		= implode("\r\n", $buffer);
			
			if(!$this->ffmpeg_found)
			{
				$this->ffmpeg_info = $data;
				return $data;
			} 
			
			$buffer = $data['raw'];
			
// 			start building the info array
			$look_ups = array('configuration'=>'configuration: ', 'formats'=>'File formats:', 'codecs'=>'Codecs:', 'filters'=>'Bitstream filters:', 'protocols'=>'Supported file protocols:', 'abbreviations'=>'Frame size, frame rate abbreviations:', 'Note:');
			$total_lookups = count($look_ups);
			$pregs = array();
			$indexs = array();
			foreach($look_ups as $key=>$reg)
			{
				if(strpos($buffer, $reg) !== false)
				{
					$index = array_push($pregs, $reg);
					$indexs[$key] = $index;
				}
			}
			preg_match('/'.implode('(.*)', $pregs).'(.*)/s', $buffer, $matches);
			$configuration = trim($matches[$indexs['configuration']]);
// 			grab the ffmpeg configuration flags
			preg_match_all('/--[a-zA-Z0-9\-]+/', $configuration, $config_flags);
			$data['binary']['configuration'] = $config_flags[0];
			$data['binary']['vhook-support'] = in_array('--enable-vhook', $config_flags[0]) || !in_array('--disable-vhook', $config_flags[0]);
// 			grab the versions
			$data['binary']['versions'] = array();
			preg_match_all('/([a-zA-Z0-9\-]+) version: ([0-9\.]+)/', $configuration, $versions);
			for($i=0, $a=count($versions[0]); $i<$a; $i++)
			{
				$data['binary']['versions'][strtolower(trim($versions[1][$i]))] = $versions[2][$i];
			}
// 			grab the ffmpeg compile info
			preg_match('/built on (.*), gcc: (.*)/', $configuration, $conf);
			if(count($conf) > 0)
			{
				$data['compiler']['gcc'] = $conf[2];
				$data['compiler']['build_date'] = $conf[1];
				$data['compiler']['build_date_timestamp'] = strtotime($conf[1]);
			}
// 			grab the file formats available to ffmpeg
			preg_match_all('/ (DE|D|E) (.*) {1,} (.*)/', trim($matches[$indexs['formats']]), $formats);
			$data['formats'] = array();
// 			loop and clean
			for($i=0, $a=count($formats[0]); $i<$a; $i++)
			{
				$data['formats'][strtolower(trim($formats[2][$i]))] = array(
					'encode' 	=> $formats[1][$i] == 'DE' || $formats[1][$i] == 'E',
					'decode' 	=> $formats[1][$i] == 'DE' || $formats[1][$i] == 'D',
					'fullname'	=> $formats[3][$i]
				);
			}
// 			grab the bitstream filters available to ffmpeg
			$data['filters'] = array();
			if(isset($indexs['filters']) && isset($matches[$indexs['filters']]))
			{
				$filters = trim($matches[$indexs['filters']]);
				if(empty($filters) === false)
				{
					$data['filters'] = explode(' ', $filters);
				}
			}
// 		    grab the file prototcols available to ffmpeg
			$data['filters'] = array();
			if(isset($indexs['protocols']) && isset($matches[$indexs['protocols']]))
			{
				$protocols = trim($matches[$indexs['protocols']]);
				if(empty($protocols) === false)
				{
					$data['protocols'] = explode(' ', str_replace(':', '', $protocols));
				}
			}
// 			grab the abbreviations available to ffmpeg
			$data['abbreviations'] = array();
			if(isset($indexs['abbreviations']) && isset($matches[$indexs['abbreviations']]))
			{
				$abbreviations = trim($matches[$indexs['abbreviations']]);
				if(empty($abbreviations) === false)
				{
					$data['abbreviations'] = explode(' ', $abbreviations);
				}
			}
			$this->ffmpeg_info = $data;
			return $data;
			
		}
		
		/**
		 * Determines the type of support that exists for the FFmpeg-PHP module.
		 * 
		 * @access public
		 * @return mixed. Boolean false if there is no support, String 'module' if the actuall
		 * 		FFmpeg-PHP module is loaded, or String 'emulated' if the FFmpeg-PHP classes
		 * 		can be emulated through the adapter classes.
		 */
		function hasFFmpegPHPSupport()
		{
			return $this->ffmpeg_found === false ? false : (extension_loaded('ffmpeg') ? 'module' : (is_file(dirname(__FILE__).DS.'adapters'.DS.'ffmpeg-php'.DS.'ffmpeg_movie.php') && is_file(dirname(__FILE__).DS.'adapters'.DS.'ffmpeg-php'.DS.'ffmpeg_frame.php') && is_file(dirname(__FILE__).DS.'adapters'.DS.'ffmpeg-php'.DS.'ffmpeg_animated_gif.php') ? 'emulated' : false));
		}
		
		/**
		 * Determines if the ffmpeg binary has been compiled with vhook support.
		 * 
		 * @access public
		 * @return mixed. Boolean false if there is no support, true there is support.
		 */
		function hasVHookSupport()
		{
			$info = $this->getFFmpegInfo();
			return $info['binary']['vhook-support'];
		}
		
		/**
		 * Returns information about the specified file without having to use ffmpeg-php
		 * as it consults the ffmpeg binary directly. This idea for this function has been borrowed from
		 * a French ffmpeg class located: http://www.phpcs.com/codesource.aspx?ID=45279
		 * 
		 * @access public
		 * @todo Change the search from string explode to a regex based search
		 * @param string $file The absolute path of the file that is required to be manipulated.
		 * @return mixed false on error encountered, true otherwise
		 **/
		function getFileInfo($file=false, $tmp_directory='/tmp/')
		{
// 			check to see if this is a static call
			if($file !== false && !$this)
			{     
				$toolkit = new PHPVideoToolkit($tmp_directory);
				return $toolkit->getFileInfo($file);
			}
// 			if the file has not been specified check to see if an input file has been specified
			if($file === false)
			{
				if(!$this->_input_file)
				{
//					input file not valid
					return $this->_raiseError('getFileInfo_no_input');
//<-				exits
				}
				$file = $this->_input_file;
			}
			$file = escapeshellarg($file);
// 			create a hash of the filename
			$hash = md5($file);
// 			check to see if the info has already been generated
			if(isset($this->_file_info[$hash]))
			{
				return $this->_file_info[$hash];
			}
// 			execute the ffmpeg lookup
			exec(PHPVIDEOTOOLKIT_FFMPEG_BINARY.' -i '.$file.' 2>&1', $buffer);
			$buffer = implode("\r\n", $buffer);
			$data = array();
// 			grab the duration and bitrate data
			preg_match_all('/Duration: (.*)/', $buffer, $matches);
			if(count($matches) > 0)
			{
				$line = trim($matches[0][0]);
// 				capture any data
				preg_match_all('/(Duration|start|bitrate): ([^,]*)/', $line, $matches);
// 				setup the default data
				$data['duration'] = array(
					'timecode' => array(
						'seconds' => array(
							'exact' => -1,
							'excess' => -1
						),
						'rounded' => -1,
					)
				);
// 				get the data
				foreach ($matches[1] as $key => $detail)
				{
					$value = $matches[2][$key];
					switch(strtolower($detail))
					{
						case 'duration' :
// 						print_r($value);
							$data['duration']['timecode']['rounded'] = substr($value, 0, 8);
							$data['duration']['timecode']['frames'] = array();
							$data['duration']['timecode']['frames']['exact'] = $value;
							$data['duration']['timecode']['frames']['excess'] = intval(substr($value, 9));
							break;
						case 'bitrate' :
							$data['bitrate'] = strtoupper($value) === 'N/A' ? -1 : intval($value);
							break;
						case 'start' :
							$data['duration']['start'] = $value;
							break;
					}
				}
			}
			
// 			match the video stream info
			preg_match('/Stream(.*): Video: (.*)/', $buffer, $matches);
			if(count($matches) > 0)
			{
				$data['video'] 						= array();
// 				get the dimension parts
				preg_match('/([0-9]{1,5})x([0-9]{1,5})/', $matches[2], $dimensions_matches);
// 				print_r($dimensions_matches);
				$dimensions_value = $dimensions_matches[0];
				$data['video']['dimensions'] 	= array(
					'width' 					=> floatval($dimensions_matches[1]),
					'height' 					=> floatval($dimensions_matches[2])
				);
// 				get the framerate
				preg_match('/([0-9\.]+) (fps|tb)\(r\)/', $matches[0], $fps_matches);
				$data['duration']['timecode']['frames']['frame_rate'] = $data['video']['frame_rate'] 	= floatval($fps_matches[1]);
				$data['duration']['timecode']['seconds']['total'] = $data['duration']['seconds'] = $this->formatTimecode($data['duration']['timecode']['frames']['exact'], '%hh:%mm:%ss.%fn', '%st.%ms', $data['video']['frame_rate']);
				$fps_value = $fps_matches[0];
// 				get the ratios
				preg_match('/\[PAR ([0-9\:\.]+) DAR ([0-9\:\.]+)\]/', $matches[0], $ratio_matches);
				if(count($ratio_matches))
				{
					$data['video']['pixel_aspect_ratio'] 	= $ratio_matches[1];
					$data['video']['display_aspect_ratio'] 	= $ratio_matches[2];
				}
// 				work out the number of frames
				if(isset($data['duration']) && isset($data['video']))
				{
// 					set the total frame count for the video
					$data['video']['frame_count'] 						= ceil($data['duration']['seconds'] * $data['video']['frame_rate']);
// 					set the framecode
					$data['duration']['timecode']['seconds']['excess'] 	= floatval($data['duration']['seconds']) - floor($data['duration']['seconds']);
					$data['duration']['timecode']['seconds']['exact'] 	= $this->formatSeconds($data['duration']['seconds'], '%hh:%mm:%ss.%ms');
					$data['duration']['timecode']['frames']['total'] 	= $data['video']['frame_count'];
				}
// 				formats should be anything left over, let me know if anything else exists
				$parts 							= explode(',', $matches[2]);
				$other_parts 					= array($dimensions_value, $fps_value);
				$formats = array();
				foreach($parts as $key=>$part)
				{
					$part = trim($part);
					if(!in_array($part, $other_parts))
					{
						array_push($formats, $part);
					}
				}
				$data['video']['pixel_format'] 	= $formats[1];
				$data['video']['codec'] 		= $formats[0];
			}
			
// 			match the audio stream info
			preg_match('/Stream(.*): Audio: (.*)/', $buffer, $matches);
			if(count($matches) > 0)
			{
// 				setup audio values
				$data['audio'] = array(
					'stereo'		=> -1, 
					'sample_rate'	=> -1, 
					'sample_rate'	=> -1
				);
				$other_parts = array();
// 				get the stereo value
				preg_match('/(stereo|mono)/i', $matches[0], $stereo_matches);
				if(count($stereo_matches))
				{
					$data['audio']['stereo'] 		= $stereo_matches[0];
					array_push($other_parts, $stereo_matches[0]);
				}
// 				get the sample_rate
				preg_match('/([0-9]{3,6}) Hz/', $matches[0], $sample_matches);
				if(count($sample_matches))
				{
					$data['audio']['sample_rate'] 	= count($sample_matches) ? floatval($sample_matches[1]) : -1;
					array_push($other_parts, $sample_matches[0]);
				}
// 				get the bit rate
				preg_match('/([0-9]{1,3}) kb\/s/', $matches[0], $bitrate_matches);
				if(count($bitrate_matches))
				{
					$data['audio']['bitrate'] 		= count($bitrate_matches) ? floatval($bitrate_matches[1]) : -1;
					array_push($other_parts, $bitrate_matches[0]);
				}
// 				formats should be anything left over, let me know if anything else exists
				$parts 							= explode(',', $matches[2]);
				$formats = array();
				foreach($parts as $key=>$part)
				{
					$part = trim($part);
					if(!in_array($part, $other_parts))
					{
						array_push($formats, $part);
					}
				}
				$data['audio']['codec'] 		= $formats[0];
// 				if no video is set then no audio frame rate is set 
				if($data['duration']['timecode']['seconds']['exact'] === -1)
				{
					$exact_timecode = $this->formatTimecode($data['duration']['timecode']['frames']['exact'], '%hh:%mm:%ss.%fn', '%hh:%mm:%ss.%ms', 1000);
					$data['duration']['timecode']['seconds'] = array(
						'exact'  => $exact_timecode,
						'excess' => intval(substr($exact_timecode, 8)),
						'total' => $this->formatTimecode($data['duration']['timecode']['frames']['exact'], '%hh:%mm:%ss.%fn', '%ss.%ms', 1000)
					);
					$data['duration']['timecode']['frames']['frame_rate'] = 1000;
					$data['duration']['seconds'] = $data['duration']['timecode']['seconds']['total'];
					//$this->formatTimecode($data['duration']['timecode']['frames']['exact'], '%hh:%mm:%ss.%fn', '%st.%ms', $data['video']['frame_rate']);					
				}
			}

// 			check that some data has been obtained
			if(!count($data))
			{
				$data = false;
			}
			else
			{
				$data['_raw_info'] = $buffer;
			}
			return $this->_file_info[$hash] = $data;
		}		

		/**
		 * Determines if the input media has a video stream.
		 * 
		 * @access public
		 * @param string $file The absolute path of the file that is required to be manipulated.
		 * @return bool
		 **/
		function fileHasVideo($file=false)
		{
// 			check to see if this is a static call
			if($file !== false && !isset($this))
			{     
				$toolkit = new PHPVideoToolkit();
				$data = $toolkit->getFileInfo($file);
			}
// 			if the file has not been specified check to see if an input file has been specified
			else if($file === false)
			{
				if(!$this->_input_file)
				{
//					input file not valid
					return $this->_raiseError('inputFileHasVideo_no_input');
//<-				exits
				}
				$file = $this->_input_file;
				$data = $this->getFileInfo($file);
			}
			return isset($data['video']);
		}		

		/**
		 * Determines if the input media has an audio stream.
		 * 
		 * @access public
		 * @param string $file The absolute path of the file that is required to be manipulated.
		 * @return bool
		 **/
		function fileHasAudio($file=false)
		{
// 			check to see if this is a static call
			if($file !== false && !isset($this))
			{     
				$toolkit = new PHPVideoToolkit();
				$data = $toolkit->getFileInfo($file);
			}
// 			if the file has not been specified check to see if an input file has been specified
			else if($file === false)
			{
				if(!$this->_input_file)
				{
//					input file not valid
					return $this->_raiseError('inputFileHasAudio_no_input');
//<-				exits
				}
				$file = $this->_input_file;
				$data = $this->getFileInfo($file);
			}
			return isset($data['audio']);
		}		

		/**
		 * Determines if your ffmpeg has particular codec support for encode or decode.
		 * 
		 * @access public
		 * @param string $codec The name of the codec you are checking for. 
		 * @param const $support PHPVideoToolkit::ENCODE or PHPVideoToolkit::DECODE, depending on which functionality is desired.
		 * @return mixed. Boolean false if there is no support, true if there is support.
		 */
   		function hasCodecSupport($codec, $support=PHPVIDEOTOOLKIT_ENCODE)
		{      
			$codec = strtolower($codec);
			$data = $this->getFFmpegInfo();     
			return isset($data['formats'][$codec]) ? $data['formats'][$codec][$support] : false;
		}

		/**
		 * Sets the input file that is going to be manipulated.
		 *
		 * @access public
		 * @param string $file The absolute path of the file that is required to be manipulated.
		 * @param mixed $input_frame_rate If 0 (default) then no input frame rate is set, if false it is automatically retreived, otherwise
		 * 		any other integer will be set as the incoming frame rate.
		 * @return boolean false on error encountered, true otherwise
		 */
		function setInputFile($file, $input_frame_rate=0)
		{
			$files_length = count($file);
// 			if the total number of files entered is 1 then only one file is being processed
			if($files_length == 1)
			{
//				check the input file, if there is a %d in there or a similar %03d then the file inputted is a sequence, if neither of those is found
//				then qheck to see if the file exists
				if(!preg_match('/\%([0-9]+)d/', $file) && strpos($file, '%d') === false && !is_file($file))
				{
//					input file not valid
					return $this->_raiseError('setInputFile_file_existence', array('file'=>$file));
//<-				exits
				}
				$escaped_name = $file;
// 				$escaped_name = escapeshellarg($files[0]);
				$this->_input_file = $escaped_name;
				$this->_input_file_id = md5($escaped_name);
				
// 				the -inputr is a hack for -r to come before the input
				if($input_frame_rate !== 0)
				{
					$info = $this->getFileInfo();
					if(isset($info['video']))
					{
						if($input_frame_rate === false)
						{
							$input_frame_rate = $info['video']['frame_rate'];
						}
// 						input frame rate is a command hack
						$this->addCommand('-inputr', $input_frame_rate);
					}
				}
			}
			else
			{
// 				more than one video is being added as input so we must join them all
				call_user_func_array(array(&$this, 'addVideo'), array($file, $input_frame_rate));
			}
			return true;
		}
		
		/**
		 * A shortcut for converting video to FLV.
		 *
		 * @access public
		 * @param integer $audio_sample_frequency
		 * @param integer $audio_bitrate
		 */
		function setFormatToFLV($audio_sample_frequency=44100, $audio_bitrate=64)
		{
			$this->addCommand('-sameq');
			$this->setAudioFormat(PHPVIDEOTOOLKIT_FORMAT_MP3);
//			adjust the audio rates
			$this->setAudioBitRate($audio_bitrate);
			$this->setAudioSampleFrequency($audio_sample_frequency);
//			set the video format
			$this->setFormat(PHPVIDEOTOOLKIT_FORMAT_FLV);
//			flag that the flv has to have meta data added after the excecution of this command
// 			register the post tidy process
			$this->registerPostProcess('_addMetaToFLV', $this);
		}
		
		/**
		 * When converting video to FLV the meta data has to be added by a ruby program called FLVTools2.
		 * This is a second exec call only after the video has been converted to FLV
		 * http://inlet-media.de/flvtool2
		 *
		 * @access private
		 */
		function _addMetaToFLV($files)
		{
			$file = array_pop($files);
//			prepare the command suitable for exec
			$exec_string = $this->_prepareCommand(PHPVIDEOTOOLKIT_FLVTOOLS_BINARY, '-U '.$file);
//			execute the command
			exec($exec_string);
			if(is_array($this->_processed[0]))
			{
				array_push($this->_processed[0], $exec_string);
			}
			else
			{
				$this->_processed[0] = array($this->_processed[0], $exec_string);
			}
			return true;
		}

		/**
		 * Streams a FLV file from a given point. You can control bandwidth, cache and session options.
		 * Inspired by xmoov-php
		 * @see xmoov-php, 
		 * 		- @link http://xmoov.com/
		 * 		- @author Eric Lorenzo Benjamin jr
		 * @access public
		 * @param integer $seek_pos The position in the file to seek to.
		 * @param array|boolean $bandwidth_options If a boolean value, false then no bandwidth limiting will take place. 
		 * 		If true then bandwidth limiting will take place with packet_size = 90 and packet_interval = 0.3.
		 * 		If an array the following values are default packet_size = 90 and packet_interval = 0.3, you will also 
		 * 		have to set active = true, ie array('active'=>true, 'packet_size'=>90, 'packet_interval'=>0.3)
		 * @param boolean $allow_cache If true the file will be allowed to cache in the browser, if false then it won't
		 * @return boolean
		 */
		function flvStreamSeek($seek_pos=0, $bandwidth_options=array(), $allow_cache=true)
		{
// 			check for input file
			if(!$this->_input_file)
			{
//				input file not valid
				return $this->_raiseError('streamFLV_no_input');
//<-			exits
			}
// 			make the pos safe
       		$seek_pos = intval($seek_pos);
// 			absorb the bandwidth options
			$bandwidth_options = is_array($bandwidth_options) ? array_merge(array('active'=>false, 'packet_size'=>90, 'packet_interval'=>0.3), $bandwidth_options) : array('active'=>$bandwidth_options, 'packet_size'=>90, 'packet_interval'=>0.3);
// 			if this file is not allowed to be cached send cache headers for all browsers.
			if(!$allow_cache)
			{
				session_cache_limiter('nocache');
				header('Expires: Thu, 19 Nov 1981 08:52:00 GMT');
				header('Last-Modified: ' . gmdate('D, d M Y H:i:s') . ' GMT');
				header('Cache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0');
				header('Pragma: no-cache');
			}
// 			open file
            $handle = fopen($this->_input_file, 'rb');
            $file_size = filesize($this->_input_file) - (($seek_pos > 0) ? $seek_pos  + 1 : 0);
// 			send the flv headers
			header('Content-Type: video/x-flv');
			header('Content-Disposition: attachment; filename="'.basename($this->_input_file).'"');
			header('Content-Length: '.$file_size);
// 			flv format header
            if($seek_pos != 0) 
            {
                print('FLV');
                print(pack('C', 1));
                print(pack('C', 1));
                print(pack('N', 9));
                print(pack('N', 9));
            }
// 			seek to the required point
            if(fseek($handle, $seek_pos) === -1)
			{
//				input file not valid
				return $this->_raiseError('streamFLV_passed_eof');
//<-			exits
			}
// 			if bandwidth control is active then workout the options
			if($bandwidth_options['active'])
			{
        		$packet_interval = intval($bandwidth_options['packet_interval']);
        		$packet_size = intval($bandwidth_options['packet_size']) * 1042;
			}
// 			output the file
			while(!feof($handle)) 
			{
// 				limit the bandwidth
				if($bandwidth_options['active'] && $packet_interval > 0)
				{
// 					output the required packet
					$time_start = $this->microtimeFloat();
					echo fread($handle, $packet_size);
					$time_stop = $this->microtimeFloat();
// 					delay the output
					$time_difference = $time_stop - $time_start;
					if($time_difference < $packet_interval)
					{
						usleep(($packet_interval * 1000000) - ($time_difference * 1000000));
					}
				}
// 				no bandwidth limiting
				else
				{
					echo fread($handle, $file_size); 
				}
			}
// 			close the file
        	fclose($handle);
			return true;
		}

		/**
		 * This is an alias for setFormat, but restricts it to audio only formats.
		 * 
		 * @access public
		 * @param integer $format A supported audio format.
		 * @param integer $audio_sample_frequency
		 * @param integer $audio_bitrate
		 **/
		function extractAudio($format=PHPVIDEOTOOLKIT_FORMAT_MP3, $audio_sample_frequency=44100, $audio_bitrate=64)
		{
// 			check the format is one of the audio formats
			if(!in_array($format, array(PHPVIDEOTOOLKIT_FORMAT_AAC, PHPVIDEOTOOLKIT_FORMAT_AIFF, PHPVIDEOTOOLKIT_FORMAT_MP2, PHPVIDEOTOOLKIT_FORMAT_MP3, PHPVIDEOTOOLKIT_FORMAT_MP4, PHPVIDEOTOOLKIT_FORMAT_MPEG4, PHPVIDEOTOOLKIT_FORMAT_M4A, PHPVIDEOTOOLKIT_FORMAT_WAV)))
			{
				return $this->_raiseError('extractAudio_valid_format', array('format'=>$format));
//<-			exits
			}
			$this->setFormat($format);
//			adjust the audio rates
			$this->setAudioBitRate($audio_bitrate);
			$this->setAudioSampleFrequency($audio_sample_frequency);
		}

		/**
		 * Sets the new video format.
		 *
		 * @access public
		 * @param defined $format The format should use one of the defined variables stated below.
		 * 		PHPVIDEOTOOLKIT_FORMAT_3GP2 - 3g2
		 * 		PHPVIDEOTOOLKIT_FORMAT_3GP - 3gp
		 * 		PHPVIDEOTOOLKIT_FORMAT_AAC - aac
		 * 		PHPVIDEOTOOLKIT_FORMAT_AIFF - aiff
		 * 		PHPVIDEOTOOLKIT_FORMAT_AMR - amr
		 * 		PHPVIDEOTOOLKIT_FORMAT_ASF - asf
		 * 		PHPVIDEOTOOLKIT_FORMAT_AVI - avi
		 * 		PHPVIDEOTOOLKIT_FORMAT_FLV - flv
		 * 		PHPVIDEOTOOLKIT_FORMAT_GIF - gif
		 * 		PHPVIDEOTOOLKIT_FORMAT_MJ2 - mj2
		 * 		PHPVIDEOTOOLKIT_FORMAT_MP2 - mp2
		 * 		PHPVIDEOTOOLKIT_FORMAT_MP3 - mp3
		 * 		PHPVIDEOTOOLKIT_FORMAT_MP4 - mp4
		 * 		PHPVIDEOTOOLKIT_FORMAT_MPEG4 - mpeg4
		 * 		PHPVIDEOTOOLKIT_FORMAT_M4A - m4a
		 * 		PHPVIDEOTOOLKIT_FORMAT_MPEG - mpeg
		 * 		PHPVIDEOTOOLKIT_FORMAT_MPEG1 - mpeg1video
		 * 		PHPVIDEOTOOLKIT_FORMAT_MPEG2 - mpeg2video
		 * 		PHPVIDEOTOOLKIT_FORMAT_MPEGVIDEO - mpegvideo
		 * 		PHPVIDEOTOOLKIT_FORMAT_PSP - psp
		 * 		PHPVIDEOTOOLKIT_FORMAT_RM - rm
		 * 		PHPVIDEOTOOLKIT_FORMAT_SWF - swf
		 * 		PHPVIDEOTOOLKIT_FORMAT_VOB - vob
		 * 		PHPVIDEOTOOLKIT_FORMAT_WAV - wav
    	 *    	PHPVIDEOTOOLKIT_FORMAT_JPG - jpg
		 * @return boolean false on error encountered, true otherwise
		 */
		function setFormat($format)
		{
//			validate input
			if(!in_array($format, array(PHPVIDEOTOOLKIT_FORMAT_3GP2, PHPVIDEOTOOLKIT_FORMAT_3GP, PHPVIDEOTOOLKIT_FORMAT_AAC, PHPVIDEOTOOLKIT_FORMAT_AIFF, PHPVIDEOTOOLKIT_FORMAT_AMR, PHPVIDEOTOOLKIT_FORMAT_ASF, PHPVIDEOTOOLKIT_FORMAT_AVI, PHPVIDEOTOOLKIT_FORMAT_FLV, PHPVIDEOTOOLKIT_FORMAT_GIF, PHPVIDEOTOOLKIT_FORMAT_MJ2, PHPVIDEOTOOLKIT_FORMAT_MP2, PHPVIDEOTOOLKIT_FORMAT_MP3, PHPVIDEOTOOLKIT_FORMAT_MP4, PHPVIDEOTOOLKIT_FORMAT_MPEG4, PHPVIDEOTOOLKIT_FORMAT_M4A, PHPVIDEOTOOLKIT_FORMAT_MPEG, PHPVIDEOTOOLKIT_FORMAT_MPEG1, PHPVIDEOTOOLKIT_FORMAT_MPEG2, PHPVIDEOTOOLKIT_FORMAT_MPEGVIDEO, PHPVIDEOTOOLKIT_FORMAT_PSP, PHPVIDEOTOOLKIT_FORMAT_RM, PHPVIDEOTOOLKIT_FORMAT_SWF, PHPVIDEOTOOLKIT_FORMAT_VOB, PHPVIDEOTOOLKIT_FORMAT_WAV, PHPVIDEOTOOLKIT_FORMAT_JPG)))
			{
				return $this->_raiseError('setFormat_valid_format', array('format'=>$format));
//<-			exits
			}
			return $this->addCommand('-f', $format);
		}

		/**
		 * Sets the audio sample frequency for audio outputs
		 *
		 * @access public
		 * @param integer $audio_sample_frequency Valid values are 11025, 22050, 44100
		 * @return boolean false on error encountered, true otherwise
		 */
		function setAudioSampleFrequency($audio_sample_frequency)
		{
//			validate input
			if(!in_array(intval($audio_sample_frequency), array(11025, 22050, 44100)))
			{
				return $this->_raiseError('setAudioSampleFrequency_valid_frequency', array('frequency'=>$audio_sample_frequency));
//<-			exits
			}
			return $this->addCommand('-ar', $audio_sample_frequency);
		}

		/**
		 * @access public
		 * @depreciated 
		 * @see PHPVideoToolkit::setAudioCodec()
		 */
		function setAudioFormat($video_format)
		{
			return $this->setAudioCodec($video_format);
		}

		/**
		 * Sets the number of audio channels
		 *
		 * @access public
		 * @param integer $channel_type Valid values are PHPVIDEOTOOLKIT_AUDIO_MONO, PHPVIDEOTOOLKIT_AUDIO_STEREO
		 * @return boolean false on error encountered, true otherwise
		 */
		function setAudioChannels($channel_type=PHPVIDEOTOOLKIT_AUDIO_MONO)
		{
//			validate input
			if(!in_array($channel_type, array(PHPVIDEOTOOLKIT_AUDIO_MONO, PHPVIDEOTOOLKIT_AUDIO_STEREO)))
			{
				return $this->_raiseError('setAudioChannels_valid_channels', array('channels'=>$channel_type));
//<-			exits
			}
			return $this->addCommand('-ac', $channel_type);
		}

		/**
		 * Sets the audio format for audio outputs
		 *
		 * @access public
		 * @param integer $audio_format Valid values are PHPVIDEOTOOLKIT_FORMAT_AAC, PHPVIDEOTOOLKIT_FORMAT_AIFF, PHPVIDEOTOOLKIT_FORMAT_AMR, PHPVIDEOTOOLKIT_FORMAT_ASF, PHPVIDEOTOOLKIT_FORMAT_MP2, PHPVIDEOTOOLKIT_FORMAT_MP3, PHPVIDEOTOOLKIT_FORMAT_MP4, PHPVIDEOTOOLKIT_FORMAT_MPEG2, PHPVIDEOTOOLKIT_FORMAT_RM, PHPVIDEOTOOLKIT_FORMAT_WAV
		 * @return boolean false on error encountered, true otherwise
		 */
		function setAudioCodec($audio_format)
		{
//			validate input
			if(!in_array($audio_format, array(PHPVIDEOTOOLKIT_FORMAT_AAC, PHPVIDEOTOOLKIT_FORMAT_AIFF, PHPVIDEOTOOLKIT_FORMAT_AMR, PHPVIDEOTOOLKIT_FORMAT_ASF, PHPVIDEOTOOLKIT_FORMAT_MP2, PHPVIDEOTOOLKIT_FORMAT_MP3, PHPVIDEOTOOLKIT_FORMAT_MP4, PHPVIDEOTOOLKIT_FORMAT_MPEG2, PHPVIDEOTOOLKIT_FORMAT_RM, PHPVIDEOTOOLKIT_FORMAT_WAV)))
			{
				return $this->_raiseError('setAudioFormat_valid_format', array('format'=>$audio_format));
//<-			exits
			}
// 			run a libmp3lame check as it require different mp3 codec
// 			updated thanks to Varon for providing the research
			if($audio_format == PHPVIDEOTOOLKIT_FORMAT_MP3)
			{
				$info = $this->getFFmpegInfo();
				if(isset($info['formats']['libmp3lame']))
				{
					$audio_format = 'libmp3lame';
				}
			}
			return $this->addCommand('-acodec', $audio_format);
		}

		/**
		 * @access public
		 * @depreciated 
		 * @see PHPVideoToolkit::setVideoCodec()
		 */
		function setVideoFormat($video_format)
		{
			return $this->setVideoCodec($video_format);
		}

		/**
		 * Sets the video format for video outputs. This should not be confused with setFormat. setVideoFormat does not generally need to
		 * be called unless setting a specific video format for a type of media format. It gets a little confusing...
		 *
		 * @access public
		 * @param integer $video_format Valid values are 11025, 22050, 44100
		 * @return boolean false on error encountered, true otherwise
		 */
		function setVideoCodec($video_codec)
		{
//			validate input
			if(!in_array($video_codec, array(PHPVIDEOTOOLKIT_FORMAT_3GP2, PHPVIDEOTOOLKIT_FORMAT_3GP, PHPVIDEOTOOLKIT_FORMAT_AVI, PHPVIDEOTOOLKIT_FORMAT_FLV, PHPVIDEOTOOLKIT_FORMAT_GIF, PHPVIDEOTOOLKIT_FORMAT_MJ2, PHPVIDEOTOOLKIT_FORMAT_MP4, PHPVIDEOTOOLKIT_FORMAT_MPEG4, PHPVIDEOTOOLKIT_FORMAT_M4A, PHPVIDEOTOOLKIT_FORMAT_MPEG, PHPVIDEOTOOLKIT_FORMAT_MPEG1, PHPVIDEOTOOLKIT_FORMAT_MPEG2, PHPVIDEOTOOLKIT_FORMAT_MPEGVIDEO)))
			{
				return $this->_raiseError('setVideoFormat_valid_format', array('format'=>$video_codec));
//<-			exits
			}
			return $this->addCommand('-vcodec', $video_codec);
		}
		
		/**
		 * Disables audio encoding
		 *
		 * @access public
		 * @return boolean false on error encountered, true otherwise
		 */
		function disableAudio()
		{
			return $this->addCommand('-an');
		}

		/**
		 * Disables video encoding
		 *
		 * @access public
		 * @return boolean false on error encountered, true otherwise
		 */
		public function disableVideo()
		{
			return $this->addCommand('-vn');
		}

		/**
		 * Sets the audio bitrate
		 *
		 * @access public
		 * @param integer $audio_bitrate Valid values are 16, 32, 64
		 * @return boolean false on error encountered, true otherwise
		 */
		function setAudioBitRate($bitrate)
		{
//			validate input
			if(!in_array(intval($bitrate), array(16, 32, 64, 128)))
			{
				return $this->_raiseError('setAudioBitRate_valid_bitrate', array('bitrate'=>$bitrate));
//<-			exits
			}
			return $this->addCommand('-ab', $bitrate.'kb');
		}

		/**
		 * Compiles an array of images into a video. This sets the input file (setInputFile) so you do not need to set it.
		 * The images should be a full absolute path to the actual image file.
		 * NOTE 1; This copies and renames all the supplied images into a temporary folder so the images don't have to be specifically named. However, when
		 * creating the ffmpeg instance you will need to set the absolute path to the temporary folder. The default path is '/tmp/'.
		 * NOTE 2; Please make sure all of the images are all of the same type.
		 *
		 * @access public
		 * @param array $images An array of images that are to be joined and converted into a video
		 * @param integer $input_frame_rate An integer that will specify the input frame rate for the images.
		 * @return boolean Returns false on encountering an error
		 */
		function prepareImagesForConversionToVideo($images, $input_frame_rate)
		{
//			http://ffmpeg.mplayerhq.hu/faq.html#TOC3
//			ffmpeg -f image2 -i img%d.jpg /tmp/a.mpg
			if(empty($images))
			{
				return $this->_raiseError('prepareImagesForConversionToVideo_one_img');
//<-			exits
			}
//			loop through and validate existence first before making a temporary copy
			foreach ($images as $key=>$img)
			{
				if(!is_file($img))
				{
					return $this->_raiseError('prepareImagesForConversionToVideo_img_404', array('img'=>$img));
//<-				exits
				}
			}
			if(!is_dir($this->_tmp_directory))
			{
				return $this->_raiseError('generic_temp_404');
//<-			exits
			}
			if(!is_writable($this->_tmp_directory))
			{
				return $this->_raiseError('generic_temp_writable');
//<-			exits
			}
//			get the number of preceding places for the files based on how many files there are to copy
			$total = count($images);
//			create a temp dir in the temp dir
			$uniqid = $this->unique();
			mkdir($this->_tmp_directory.$uniqid, 0777);
//			loop through, copy and rename specified images to the temp dir
			$ext = false;
			foreach ($images as $key=>$img)
			{
				$file_ext = array_pop(explode('.', $img));
				if($ext !== false && $ext !== $file_ext)
				{
					return $this->_raiseError('prepareImagesForConversionToVideo_img_type');
//<-				exits
				}
				$ext = $file_ext;
				$tmp_file = $this->_tmp_directory.$uniqid.DS.$this->_tmp_file_prefix.$key.'.'.$ext;
				if(!@copy($img, $tmp_file))
				{
					return $this->_raiseError('prepareImagesForConversionToVideo_img_copy', array('img'=>$img, 'tmpfile'=>$tmp_file));
//<-				exits
				}
//				push the tmp file name into the unlinks so they can be deleted on class destruction
				array_push($this->_unlink_files, $tmp_file);
			}
// 			the inputr is a hack for -r to come before the input
			$this->addCommand('-inputr', $input_frame_rate);
// 			exit;
//			add the directory to the unlinks
			array_push($this->_unlink_dirs, $this->_tmp_directory.$uniqid);
//			get the input file format
			$file_iteration = $this->_tmp_file_prefix.'%d.'.$ext;
//			set the input filename
			return $this->setInputFile($this->_tmp_directory.$uniqid.DS.$file_iteration);
		}

		/**
		 * Sets the video bitrate
		 *
		 * @access public
		 * @param integer $bitrate 
		 * @return boolean
		 */
		function setVideoBitRate($bitrate)
		{
			$bitrate = intval($bitrate);
			return $this->addCommand('-b', $bitrate.'kb');
		}
		
		/**
		 * Sets the amount of time an animated gif output will loop
		 *
		 * @access public
		 * @param integer $loop_count If false the gif will not loop, if 0 it will loop endlessly, any other number it will loop that amount.
		 */
		function setGifLoops($loop_count)
		{
			if($loop_count !== false)
			{
				$this->addCommand('-loop_output', $loop_count);
			}
		}

		/**
		 * @access public
		 * @depreciated 
		 * @see PHPVideoToolkit::setVideoDimensions()
		 */
		function setVideoOutputDimensions($width, $height=null)
		{
			return $this->setVideoDimensions($width, $height);
		}
		
		/**
		 * Sets the video output dimensions (in pixels)
		 *
		 * @access public
		 * @param mixed $width If an integer height also has to be specified, otherwise you can use one of the class constants
		 * 		PHPVIDEOTOOLKIT_SIZE_SAS		= Same as input source
		 * 		PHPVIDEOTOOLKIT_SIZE_SQCIF 		= 128 x 96
		 * 		PHPVIDEOTOOLKIT_SIZE_QCIF 		= 176 x 144
		 * 		PHPVIDEOTOOLKIT_SIZE_CIF 		= 352 x 288
		 * 		PHPVIDEOTOOLKIT_SIZE_4CIF 		= 704 x 576
		 * 		PHPVIDEOTOOLKIT_SIZE_QQVGA 		= 160 x 120
		 * 		PHPVIDEOTOOLKIT_SIZE_QVGA 		= 320 x 240
		 * 		PHPVIDEOTOOLKIT_SIZE_VGA 		= 640 x 480
		 * 		PHPVIDEOTOOLKIT_SIZE_SVGA 		= 800 x 600
		 * 		PHPVIDEOTOOLKIT_SIZE_XGA 		= 1024 x 768
		 * 		PHPVIDEOTOOLKIT_SIZE_UXGA 		= 1600 x 1200
		 * 		PHPVIDEOTOOLKIT_SIZE_QXGA 		= 2048 x 1536
		 * 		PHPVIDEOTOOLKIT_SIZE_SXGA 		= 1280 x 1024
		 * 		PHPVIDEOTOOLKIT_SIZE_QSXGA 		= 2560 x 2048
		 * 		PHPVIDEOTOOLKIT_SIZE_HSXGA 		= 5120 x 4096
		 * 		PHPVIDEOTOOLKIT_SIZE_WVGA 		= 852 x 480
		 * 		PHPVIDEOTOOLKIT_SIZE_WXGA 		= 1366 x 768
		 * 		PHPVIDEOTOOLKIT_SIZE_WSXGA 		= 1600 x 1024
		 * 		PHPVIDEOTOOLKIT_SIZE_WUXGA 		= 1920 x 1200
		 * 		PHPVIDEOTOOLKIT_SIZE_WOXGA 		= 2560 x 1600
		 * 		PHPVIDEOTOOLKIT_SIZE_WQSXGA		= 3200 x 2048
		 * 		PHPVIDEOTOOLKIT_SIZE_WQUXGA 	= 3840 x 2400
		 * 		PHPVIDEOTOOLKIT_SIZE_WHSXGA 	= 6400 x 4096
		 * 		PHPVIDEOTOOLKIT_SIZE_WHUXGA 	= 7680 x 4800
		 * 		PHPVIDEOTOOLKIT_SIZE_CGA 		= 320 x 200
		 * 		PHPVIDEOTOOLKIT_SIZE_EGA		= 640 x 350
		 * 		PHPVIDEOTOOLKIT_SIZE_HD480 		= 852 x 480
		 * 		PHPVIDEOTOOLKIT_SIZE_HD720 		= 1280 x 720
		 * 		PHPVIDEOTOOLKIT_SIZE_HD1080		= 1920 x 1080
		 * @param integer $height
		 * @return boolean
		 */
		function setVideoDimensions($width, $height=null)
		{
			if($height === null || $height === true)
			{
//				validate input
				if(!in_array($width, array(PHPVIDEOTOOLKIT_SIZE_SAS, PHPVIDEOTOOLKIT_SIZE_SQCIF, PHPVIDEOTOOLKIT_SIZE_QCIF, PHPVIDEOTOOLKIT_SIZE_CIF, PHPVIDEOTOOLKIT_SIZE_4CIF, PHPVIDEOTOOLKIT_SIZE_QQVGA, PHPVIDEOTOOLKIT_SIZE_QVGA, PHPVIDEOTOOLKIT_SIZE_VGA, PHPVIDEOTOOLKIT_SIZE_SVGA, PHPVIDEOTOOLKIT_SIZE_XGA, PHPVIDEOTOOLKIT_SIZE_UXGA, PHPVIDEOTOOLKIT_SIZE_QXGA, PHPVIDEOTOOLKIT_SIZE_SXGA, PHPVIDEOTOOLKIT_SIZE_QSXGA, PHPVIDEOTOOLKIT_SIZE_HSXGA, PHPVIDEOTOOLKIT_SIZE_WVGA, PHPVIDEOTOOLKIT_SIZE_WXGA, PHPVIDEOTOOLKIT_SIZE_WSXGA, PHPVIDEOTOOLKIT_SIZE_WUXGA, PHPVIDEOTOOLKIT_SIZE_WOXGA, PHPVIDEOTOOLKIT_SIZE_WQSXGA, PHPVIDEOTOOLKIT_SIZE_WQUXGA, PHPVIDEOTOOLKIT_SIZE_WHSXGA, PHPVIDEOTOOLKIT_SIZE_WHUXGA, PHPVIDEOTOOLKIT_SIZE_CGA, PHPVIDEOTOOLKIT_SIZE_EGA, PHPVIDEOTOOLKIT_SIZE_HD480, PHPVIDEOTOOLKIT_SIZE_HD720, PHPVIDEOTOOLKIT_SIZE_HD1080)))
				{
					return $this->_raiseError('setVideoOutputDimensions_valid_format', array('format'=>$format));
//<-				exits
				}
				if($width === PHPVIDEOTOOLKIT_SIZE_SAS)
				{
// 					and override is made so no command is added in the hope that ffmpeg will just output the source
					if($height === true)
					{
						return true;
					}
// 					get the file info
					$info = $this->getFileInfo();
					if(!isset($info['video']) || !isset($info['video']['dimensions']))
					{
						return $this->_raiseError('setVideoOutputDimensions_sas_dim');
					}
					else
					{
						$width = $info['video']['dimensions']['width'].'x'.$info['video']['dimensions']['height'];
					}
				}
			}
			else
			{
// 				check that the width and height are even
				if($width % 2 !== 0 || $height % 2 !== 0)
				{
					return $this->_raiseError('setVideoOutputDimensions_valid_integer');
//<-				exits
				}
				$width = $width.'x'.$height;
			}
			$this->addCommand('-s', $width);
			return true;
		}

		/**
		 * Sets the video aspect ratio
		 *
		 * @access public
		 * @param string|integer $ratio Valid values are PHPVIDEOTOOLKIT_RATIO_STANDARD, PHPVIDEOTOOLKIT_RATIO_WIDE, PHPVIDEOTOOLKIT_RATIO_CINEMATIC, or '4:3', '16:9', '1.85' 
		 * @return boolean
		 */
		function setVideoAspectRatio($ratio)
		{
			if(!in_array($ratio, array(PHPVIDEOTOOLKIT_RATIO_STANDARD, PHPVIDEOTOOLKIT_RATIO_WIDE, PHPVIDEOTOOLKIT_RATIO_CINEMATIC)))
			{
				return $this->_raiseError('setVideoAspectRatio_valid_ratio', array('ratio'=>$ratio));
			}
			$this->addCommand('-aspect', $ratio);
			return true;
		}
		
		/**
		 * Sets the frame rate of the video
		 *
		 * @access public
		 * @param string|integer $fps 1 being 1 frame per second, 1:2 being 0.5 frames per second
		 * @return boolean
		 */
		function setVideoFrameRate($fps)
		{
			return $this->addCommand('-r', $fps);
		}
		
		/**
		 * Extracts a segment of video and/or audio
		 * (Note; If set to 1 and the duration set by $extract_begin_timecode and $extract_end_timecode is equal to 1 you get more than one frame.
		 * For example if you set $extract_begin_timecode='00:00:00' and $extract_end_timecode='00:00:01' you might expect because the time span is
		 * 1 second only to get one frame if you set $frames_per_second=1. However this is not correct. The timecode you set in $extract_begin_timecode
		 * acts as the beginning frame. Thus in this example the first frame exported will be from the very beginning of the video, the video will
		 * then move onto the next frame and export a frame there. Therefore if you wish to export just one frame from one position in the video,
		 * say 1 second in you should set $extract_begin_timecode='00:00:01' and set $extract_end_timecode='00:00:01'.)
		 *
		 * @access public
		 * @param string $extract_begin_timecode A timecode (hh:mm:ss.fn - you can change the timecode format by changing the $timecode_format param
		 * 		it obeys the formatting of PHPVideoToolkit::formatTimecode(), see below for more info)
		 * @param string|integer|boolean $extract_end_timecode A timecode (hh:mm:ss.fn - you can change the timecode format by changing the $timecode_format param
		 * 		it obeys the formatting of PHPVideoToolkit::formatTimecode(), see below for more info)
		 * @param integer $timecode_format The format of the $extract_begin_timecode and $extract_end_timecode timecodes are being given in.
		 * 		default '%hh:%mm:%ss'
		 * 			- %hh (hours) representative of hours
		 * 			- %mm (minutes) representative of minutes
		 * 			- %ss (seconds) representative of seconds
		 * 			- %fn (frame number) representative of frames (of the current second, not total frames)
		 * 			- %ms (milliseconds) representative of milliseconds (of the current second, not total milliseconds) (rounded to 3 decimal places)
		 * 			- %ft (frames total) representative of total frames (ie frame number)
		 * 			- %st (seconds total) representative of total seconds (rounded).
		 * 			- %sf (seconds floored) representative of total seconds (floored).
		 * 			- %mt (milliseconds total) representative of total milliseconds. (rounded to 3 decimal places)
		 * 		Thus you could use an alternative, '%hh:%mm:%ss:%ms', or '%hh:%mm:%ss' dependent on your usage.
		 * @param boolean $check_frames_exist Determines if a frame exists check should be made to ensure the timecode given by $extract_end_timecode 
		 * 		actually exists.
		 */
		public function extractSegment($extract_begin_timecode, $extract_end_timecode, $timecode_format='%hh:%mm:%ss.%fn', $frames_per_second=false, $check_frames_exist=true)
		{
// 			check for frames per second, if it's not set auto set it.
			if($frames_per_second === false)
			{
				$info = $this->getFileInfo();
				$frames_per_second = $info['duration']['timecode']['frames']['frame_rate'];
			}
			
// 			check if frame exists
			if($check_frames_exist)
			{
				if($info['duration']['seconds'] < floatval($this->formatTimecode($extract_end_timecode, $timecode_format, '%ss.%ms', $frames_per_second)))
				{
// 					the input has not returned any video data so the frame rate can not be guessed
					return $this->_raiseError('extractSegment_end_timecode');
				}
				else if($extract_end_timecode !== false && $info['duration']['seconds'] < floatval($this->formatTimecode($extract_begin_timecode, $timecode_format, '%ss.%ms', $frames_per_second)))
				{
// 					the input has not returned any video data so the frame rate can not be guessed
					return $this->_raiseError('extractSegment_begin_timecode');
				}
			}
			
// 			format the begin timecode if the timecode format is not already ok.
			$begin_position = (float) $this->formatTimecode($extract_begin_timecode, $timecode_format, '%ss.%ms', $frames_per_second);
			if($timecode_format !== '%hh:%mm:%ss.%ms')
			{
				$extract_begin_timecode = $this->formatTimecode($extract_begin_timecode, $timecode_format, '%hh:%mm:%ss.%ms', $frames_per_second);
			}
			$this->addCommand('-ss', $extract_begin_timecode);
			
//			allows for exporting the entire timeline
			if($extract_end_timecode !== false)
			{
				$end_position = (float) $this->formatTimecode($extract_end_timecode, $timecode_format, '%ss.%ms', $frames_per_second);
// 				format the end timecode if the timecode format is not already ok.
				if($timecode_format !== '%hh:%mm:%ss.%ms')
				{
					$extract_end_timecode = $this->formatTimecode($extract_end_timecode, $timecode_format, '%hh:%mm:%ss.%ms', $frames_per_second);
				}
        		$this->addCommand('-t', $end_position-$begin_position);
			}
			return true;
		}
		
		/**
		 * Extracts frames from a video.
		 * (Note; If set to 1 and the duration set by $extract_begin_timecode and $extract_end_timecode is equal to 1 you get more than one frame.
		 * For example if you set $extract_begin_timecode='00:00:00' and $extract_end_timecode='00:00:01' you might expect because the time span is
		 * 1 second only to get one frame if you set $frames_per_second=1. However this is not correct. The timecode you set in $extract_begin_timecode
		 * acts as the beginning frame. Thus in this example the first frame exported will be from the very beginning of the video, the video will
		 * then move onto the next frame and export a frame there. Therefore if you wish to export just one frame from one position in the video,
		 * say 1 second in you should set $extract_begin_timecode='00:00:01' and set $extract_end_timecode='00:00:01'.)
		 *
		 * @access public
		 * @param string $extract_begin_timecode A timecode (hh:mm:ss.fn - you can change the timecode format by changing the $timecode_format param
		 * 		it obeys the formatting of PHPVideoToolkit::formatTimecode(), see below for more info)
		 * @param string|integer|boolean $extract_end_timecode A timecode (hh:mm:ss.fn - you can change the timecode format by changing the $timecode_format param
		 * 		it obeys the formatting of PHPVideoToolkit::formatTimecode(), see below for more info), or false
		 * 		if all frames from the begin timecode are to be exported. (Boolean added by Matthias. Thanks. 12th March 2007)
		 * @param boolean|integer $frames_per_second The number of frames per second to extract. If left as default false, then the number of frames per second
		 * 		will be automagically gained from PHPVideoToolkit::fileGetInfo();
		 * @param boolean|integer $frame_limit Frame limiter. If set to false then all the frames will be exported from the given time codes, however
		 * 		if you wish to set a export limit to the number of frames that are exported you can set an integer. For example; if you set
		 * 		$extract_begin_timecode='00:00:11.01', $extract_end_timecode='00:01:10.01', $frames_per_second=1, you will get one frame for every second
		 * 		in the video between 00:00:11 and 00:01:10 (ie 60 frames), however if you ant to artificially limit this to exporting only ten frames
		 * 		then you set $frame_limit=10. You could of course alter the timecode to reflect you desired frame number, however there are situations
		 * 		when a shortcut such as this is useful and necessary.
		 * @param integer $timecode_format The format of the $extract_begin_timecode and $extract_end_timecode timecodes are being given in.
		 * 		default '%hh:%mm:%ss'
		 * 			- %hh (hours) representative of hours
		 * 			- %mm (minutes) representative of minutes
		 * 			- %ss (seconds) representative of seconds
		 * 			- %fn (frame number) representative of frames (of the current second, not total frames)
		 * 			- %ms (milliseconds) representative of milliseconds (of the current second, not total milliseconds) (rounded to 3 decimal places)
		 * 			- %ft (frames total) representative of total frames (ie frame number)
		 * 			- %st (seconds total) representative of total seconds (rounded).
		 * 			- %sf (seconds floored) representative of total seconds (floored).
		 * 			- %mt (milliseconds total) representative of total milliseconds. (rounded to 3 decimal places)
		 * 		Thus you could use an alternative, '%hh:%mm:%ss:%ms', or '%hh:%mm:%ss' dependent on your usage.
		 */
		function extractFrames($extract_begin_timecode, $extract_end_timecode, $frames_per_second=false, $frame_limit=false, $timecode_format='%hh:%mm:%ss.%fn', $check_frames_exist=true)
		{
// 			are we autoguessing the frame rate?
			if($frames_per_second === false || $check_frames_exist)
			{
// 				get the file info, will exit if no input has been set
				$info = $this->getFileInfo();
				if($info === false || !isset($info['video']))
				{
// 					the input has not returned any video data so the frame rate can not be guessed
					return $this->_raiseError('extractFrame_video_frame_rate_404');
				}
			}
// 			check to see if we have to get the fps of the input movie
			if($frames_per_second === false)
			{
				$frames_per_second = $info['video']['frame_rate'];
			}
// 			check if frame exists
			if($check_frames_exist)
			{
				if($info['video']['frame_count'] < $this->formatTimecode($extract_end_timecode, $timecode_format, '%ft', $frames_per_second))
				{
// 					the input has not returned any video data so the frame rate can not be guessed
					return $this->_raiseError('extractFrames_video_end_frame_count');
				}
				else if($extract_end_timecode !== false && $info['video']['frame_count'] < $this->formatTimecode($extract_begin_timecode, $timecode_format, '%ft', $frames_per_second))
				{
// 					the input has not returned any video data so the frame rate can not be guessed
					return $this->_raiseError('extractFrames_video_begin_frame_count');
				}
			}
// 			disable audio output
			$this->disableAudio();
// 			format the begin timecode if the timecode format is not already ok.
			if($timecode_format !== '%hh:%mm:%ss.%ms')
			{
				$extract_begin_timecode = $this->formatTimecode($extract_begin_timecode, $timecode_format, '%hh:%mm:%ss.%ms', $frames_per_second);
			}
			$this->addCommand('-ss', $extract_begin_timecode);
//			added by Matthias on 12th March 2007
//			allows for exporting the entire timeline
			if($extract_end_timecode !== false)
			{
// 				format the end timecode if the timecode format is not already ok.
				if($timecode_format !== '%hh:%mm:%ss.%ms')
				{
					$extract_end_timecode = $this->formatTimecode($extract_end_timecode, $timecode_format, '%hh:%mm:%ss.%ms', $frames_per_second);
				}
        		$this->addCommand('-t', $extract_end_timecode);
			}
// 			set the output frame rate
			$this->setVideoFrameRate($frames_per_second);
// 			do we need to limit the number of frames outputted?
			if($frame_limit !== false)
			{
				$this->addCommand('-vframes', $frame_limit);
			}
			$this->_image_output_timecode_start = $extract_begin_timecode;
			$this->_image_output_timecode_fps = $frames_per_second;
		}
		
		/**
		 * Extracts exactly one frame
		 *
		 * @access public
		 * @uses $toolkit->extractFrames
		 * @param string $frame_timecode A timecode (hh:mm:ss.fn) where fn is the frame number of that second
		 * @param integer|boolean $frames_per_second The frame rate of the movie. If left as the default, false. We will use PHPVideoToolkit::getFileInfo() to get
		 * 			the actual frame rate. It is recommended that it is left as false because an incorrect frame rate may produce unexpected results.
		 * @param integer $timecode_format The format of the $extract_begin_timecode and $extract_end_timecode timecodes are being given in.
		 * 		default '%hh:%mm:%ss'
		 * 			- %hh (hours) representative of hours
		 * 			- %mm (minutes) representative of minutes
		 * 			- %ss (seconds) representative of seconds
		 * 			- %fn (frame number) representative of frames (of the current second, not total frames)
		 * 			- %ms (milliseconds) representative of milliseconds (of the current second, not total milliseconds) (rounded to 3 decimal places)
		 * 			- %ft (frames total) representative of total frames (ie frame number)
		 * 			- %st (seconds total) representative of total seconds (rounded).
		 * 			- %sf (seconds floored) representative of total seconds (floored).
		 * 			- %mt (milliseconds total) representative of total milliseconds. (rounded to 3 decimal places)
		 * 		Thus you could use an alternative, '%hh:%mm:%ss:%ms', or '%hh:%mm:%ss' dependent on your usage.
		 * @param boolean $check_frame_exists Makes an explicit check to see if the frame exists, default = true. 
		 * 		Thanks to Istvan Szakacs for suggesting this check. Note, to improve performance disable this check.
		 */
		function extractFrame($frame_timecode, $frames_per_second=false, $frame_timecode_format='%hh:%mm:%ss.%fn', $check_frame_exists=true)
		{
// 			get the file info, will exit if no input has been set
			if($check_frame_exists || $frames_per_second === false)
			{
				$info = $this->getFileInfo();
				if($info === false || !isset($info['video']))
				{
// 					the input has not returned any video data so the frame rate can not be guessed
					return $this->_raiseError('extractFrame_video_info_404');
				}
			}
// 			are we autoguessing the frame rate?
			if($frames_per_second === false)
			{
				if(!isset($info['video']['frame_rate']))
				{
// 					the input has not returned any video data so the frame rate can not be guessed
					return $this->_raiseError('extractFrame_video_frame_rate_404');
				}
				$frames_per_second = $info['video']['frame_rate'];
			}
// 			check if frame exists
			if($check_frame_exists)
			{
				if($info['video']['frame_count'] < $this->formatTimecode($frame_timecode, $frame_timecode_format, '%ft', $frames_per_second))
				{
// 					the input has not returned any video data so the frame rate can not be guessed
					return $this->_raiseError('extractFrame_video_frame_count');
				}
			}
// 			format the frame details if the timecode format is not already ok.
			if($frame_timecode_format !== '%hh:%mm:%ss.%ms')
			{
				$frame_timecode = $this->formatTimecode($frame_timecode, $frame_timecode_format, '%hh:%mm:%ss.%ms', $frames_per_second);
			}
			$this->_single_frame_extraction = 1;
// 			we will limit the number of frames produced so the desired frame is the last image
// 			this way we limit the cpu usage of ffmpeg
// 			Thanks to Istvan Szakacs for pointing out that ffmpeg can export frames using the -ss hh:mm:ss[.xxx]
// 			it has saved a lot of cpu intensive processes.
			$this->extractFrames($frame_timecode, $frame_timecode, $frames_per_second, 1, '%hh:%mm:%ss.%ms', false);
// 			register the post tidy process
// 			$this->registerPostProcess('_extractFrameTidy', $this);
		}
		
// 		/**
// 		 * Tidies up after ffmpeg exports all frames from one second of video.
// 		 *
// 		 * @access public
// 		 * @uses $toolkit->extractFrames
// 		 * @param string $frame_timecode A timecode (hh:mm:ss.fn) where fn is the frame number of that second
// 		 * @param integer|boolean $frames_per_second The frame rate of the movie. If left as the default, false. We will use PHPVideoToolkit::getFileInfo() to get
// 		 * 			the actual frame rate. It is recommended that it is left as false because an incorrect frame rate may produce unexpected results.
// 		 */
// 		function _extractFrameTidy(&$files)
// 		{
// 			$frame_number = 1;
// 			$frame_file = array();
// // 			print_r($files);
// 			foreach($files as $file=>$filename)
// 			{
// // 				print_R(array($this->_single_frame_extraction, $frame_number));
// 				if($this->_single_frame_extraction == $frame_number)
// 				{
// // 					leave this file alone as it is the required frame
// 					$frame_file[$file] = $filename;
// 				}
// 				else
// 				{
// // 					add the frame to the unlink files list
// 					array_push($this->_unlink_files, $file);
// 				}
// 				$frame_number += 1;
// 			}
// // 				print_r($frame_file);
// // 			update the files list
// 			$files = $frame_file;
// 			return true;
// 		}
		
		/**
		 * Adds a watermark to the outputted files. This effects both video and image output.
		 *
		 * @access public
		 * @param string $watermark_url The absolute path to the watermark image.
		 * @param string $vhook The absolute path to the ffmpeg vhook watermark library.
		 * @param string $watermark_options Any additional options to supply to the vhook.
		 */
		function addWatermark($watermark_url, $vhook=PHPVIDEOTOOLKIT_FFMPEG_WATERMARK_VHOOK, $watermark_options=false)
		{
// 			check to see if the ffmpeg binary has support for vhooking
			if(!$this->hasVHookSupport())
			{
				return $this->_raiseError('addWatermark_vhook_disabled');
			}
// 			does the file exist?
			if(!is_file($watermark_url))
			{
				return $this->_raiseError('addWatermark_img_404', array('watermark'=>$watermark_url));
			}
			$this->addCommand('-vhook', $vhook.' -f '.$watermark_url.($watermark_options !== false ? ' '.$watermark_options : ''));
		}
		
		/**
		 * Adds a watermark to the outputted image files using the PHP GD module. 
		 * This effects only image output.
		 *
		 * @access public
		 * @param string $watermark_url The absolute path to the watermark image.
		 */
		function addGDWatermark($watermark_url, $options=array('x-offset'=>0, 'y-offset'=>0, 'position'=>'bottom-right'))
		{
// 			does the file exist?
			if(!is_file($watermark_url))
			{
				return $this->_raiseError('addWatermark_img_404', array('watermark'=>$watermark_url));
			}
// 			save the watermark_url
			$this->_watermark_url = $watermark_url;
			$this->_watermark_options = array_merge(array('x-offset'=>0, 'y-offset'=>0, 'position'=>'bottom-right'), $options);
// 			register the post process
			$this->registerPostProcess('_addGDWatermark', $this);
		}
		
		/**
		 * Adds watermark to any outputted images via GD instead of using vhooking.
		 * 
		 * @access private
		 * @param array $files An array of image files.
		 * @return array
		 */
		function _addGDWatermark($files)
		{
// 			create the watermark resource and give it alpha blending
			$info = pathinfo($this->_watermark_url);
			switch(strtolower($info['extension']))
			{
				case 'jpeg' : 
				case 'jpg' : 
					$watermark = imagecreatefromjpeg($this->_watermark_url);
					break;
				case 'gif' : 
					$watermark = imagecreatefromgif($this->_watermark_url);
					break;
				case 'png' : 
					$watermark = imagecreatefrompng($this->_watermark_url);
					break;
				default :
					return false;
			}
			imagealphablending($watermark, true);
			imagesavealpha($watermark, true);
// 			get the watermark dimensions
			$watermark_width = imagesx($watermark);
			$watermark_height = imagesy($watermark);
// 			$image = imagecreatetruecolor($watermark_width, $watermark_height);
// 			loop and watermark each file
			$blended_files = array();
			foreach($files as $file=>$filename)
			{
// 				detect the file extension and create the resource from them appropriate function
				$info = pathinfo($file);
				$quality = $output_function = null;
				switch(strtolower($info['extension']))
				{
					case 'jpeg' : 
					case 'jpg' : 
						$quality = 80;
						$output_function = 'imagejpeg';
						$image = imagecreatefromjpeg($file);
						break;
					case 'gif' : 
						$output_function = 'imagegif';
						$image = imagecreatefromgif($file);
						break;
					case 'png' : 
						$quality = 9;
						$output_function = 'imagepng';
						$image = imagecreatefrompng($file);
						break;
					default :
						continue 1;
				}
			
// 				the dimensions will/should be the same for each image however still best to check
				$image_width = imagesx($image);
				$image_height = imagesy($image);
// 				calculate where to position the watermark
				$dest_x = 0;
				$dest_y = 0;
				switch($this->_watermark_options['position'])
				{
					case 'top-left' :
						$dest_x = 0;
						$dest_y = 0;
						break;
					case 'top-middle' :
						$dest_x = ($image_width-$watermark_width)/2;
						$dest_y = 0;
						break;
					case 'top-right' :
						$dest_x = $image_width-$watermark_width;
						$dest_y = 0;
						break;
					case 'center-left' :
						$dest_x = 0;
						$dest_y = ($image_height-$watermark_height)/2;
						break;
					case 'center-middle' :
						$dest_x = ($image_width-$watermark_width)/2;
						$dest_y = ($image_height-$watermark_height)/2;
						break;
					case 'center-right' :
						$dest_x = $image_width-$watermark_width;
						$dest_y = ($image_height-$watermark_height)/2;
						break;
					case 'bottom-left' :
						$dest_x = 0;
						$dest_y = $image_height - $watermark_height;
						break;
					case 'bottom-middle' :
						$dest_x = ($image_width-$watermark_width)/2;
						$dest_y = $image_height - $watermark_height;
						break;
					case 'bottom-right' :
					default :
						$dest_x = $image_width-$watermark_width;
						$dest_y = $image_height - $watermark_height;
						break;
				}
				$dest_x += $this->_watermark_options['x-offset'];
				$dest_y += $this->_watermark_options['y-offset'];
// 				copy the watermark to the new image
// 			print_r(array($this->_watermark_url, $image, $watermark, $dest_x, $dest_y, 0, 0, $watermark_width, $watermark_height));
				imagecopy($image, $watermark, $dest_x, $dest_y, 0, 0, $watermark_width, $watermark_height);
// 				delete the old image
				unlink($file);
// 				save the new image in place of the old
				$output_function($image, $file, $quality);
// 				remove the image resouce
				imagedestroy($image);
				array_push($blended_files, $file);
			}
// 			remove the watermark resource
			imagedestroy($watermark);
			return $blended_files;
		}
		
// 		/**
// 		 * This will overlay an audio file over the top of a video file
// 		 **/
// 		function overlayAudio($audio_file)
// 		{
// 			$this->addCommand('-newaudio', '');
// 		}

		/**
		 * This will adjust the audio volume.
		 * 
		 * @access public
		 * @param integer $vol 256 = normal
		 **/
		function adjustVolume($vol=256)
		{
			$this->addCommand('-vol', '');
		}

		/**
		 * This process will combine the original input video with the video specified by this function.
		 * This function accepts more than one video as arguments. They will be added in order of the arguments.
		 * 	ie. input_video -> video1 -> video2 etc
		 * The process of doing this can take a long time as each incoming video has to be first converted
		 * into a format that accepts joining. The default joining codec is "mpg". However for almost lossless
		 * quality you can use the "yuv4mpegpipe" format. This is of course dependent upon your ffmpeg binary.
		 * You can check to see if you server supports yuv4mpegpipe by typing "ffmpeg -formats" into the
		 * command line. If you want to use the yuv4mpegpipe format you can add the flag, FFMPEG_USE_HQ_JOIN to the
		 * end of the video inputs. WARNING: High Quality joins will take longer to process. (well duh!)
		 *
		 * @access public
		 * @param $video1, $video2, $video3... $video(n) Paths of videos to attach to the input video.
		 * @param $flag integer FFMPEG_USE_HQ_JOIN If you wish to use the yuv4mpegpipe format for join add this to the end of the video list.
		 */
		function addVideo()
		{
			$videos = func_get_args();
			$videos_length = count($videos);
// 			is last arg the hq join flag
// 			check to see if a starter file has been added, if not set the input as an array
			if($this->_input_file === null)
			{
				$this->_input_file = array();
			}
// 			if the input file is already set as a string that means as start file has been added so absorb into the input array
			else if(is_string($this->_input_file))
			{
				$this->_input_file = array($this->_input_file);
			}
			foreach($videos as $key=>$file)
			{
				if(!preg_match('/\%([0-9]+)d/', $file) && strpos($file, '%d') === false && !is_file($file))
				{
// 					input file not valid
					return $this->_raiseError('addVideo_file_404', array('file'=>$file));
//<-				exits
				}
				array_push($this->_input_file, $file);
// 				array_push($this->_input_file, escapeshellarg($file));
			}
		}

		/**
		 * @access public
		 * @uses addVideo()
		 */
		function addVideos()
		{
			$videos = func_get_args();
			call_user_func_array(array(&$this, 'addVideo'), $videos);
		}

		/**
		 * Sets the output.
		 *
		 * @access public
		 * @param string $output_directory The directory to output the command output to
		 * @param string $output_name The filename to output to.
		 * 			(Note; if you are outputting frames from a video then you will need to add an extra item to the output_name. The output name you set is required
		 * 			to contain '%d'. '%d' is replaced by the image number. Thus entering setting output_name $output_name='img%d.jpg' will output
		 * 			'img1.jpg', 'img2.jpg', etc... However 'img%03d.jpg' generates `img001.jpg', `img002.jpg', etc...)
		 * @param boolean $overwrite_mode Accepts one of the following class constants
		 * 	- PHPVIDEOTOOLKIT_OVERWRITE_FAIL		- This produces an error if there is a file conflict and the processing is halted.
		 * 	- PHPVIDEOTOOLKIT_OVERWRITE_PRESERVE	- This continues with the processing but no file overwrite takes place. The processed file is left in the temp directory
		 * 									  for you to manually move.
		 * 	- PHPVIDEOTOOLKIT_OVERWRITE_EXISTING	- This will replace any existing files with the freshly processed ones.
		 * 	- PHPVIDEOTOOLKIT_OVERWRITE_UNIQUE		- This will appended every output with a unique hash so that the filesystem is preserved.
		 * @return boolean false on error encountered, true otherwise
		 */
		function setOutput($output_directory, $output_name, $overwrite_mode=PHPVIDEOTOOLKIT_OVERWRITE_FAIL)
		{
//			check if directoy exists
			if(!is_dir($output_directory))
			{
				return $this->_raiseError('setOutput_output_dir_404', array('dir'=>$output_directory));
//<-			exits
			}
//			check if directory is writeable
			if(!is_writable($output_directory))
			{
				return $this->_raiseError('setOutput_output_dir_writable', array('dir'=>$output_directory));
//<-			exits
			}
			$process_name = '';
			
//			check to see if a output delimiter is set
			$has_d = preg_match('/\%([0-9]+)d/', $output_name) || strpos($output_name, '%d') !== false;
			if($has_d)
			{
				return $this->_raiseError('setOutput_%d_depreciated');
//<-			exits
			}
			else
			{
//				determine if the extension is an image. If it is then we will be extracting frames so check for %d
				$output_name_info = pathinfo($output_name);
				$is_image = in_array(strtolower($output_name_info['extension']), array('jpg', 'jpeg', 'png'));
				$is_gif	  = strtolower($output_name_info['extension']) === 'gif';
//				NOTE: for now we'll just stick to the common image formats, SUBNOTE: gif is ignore because ffmpeg can create animated gifs
				if($this->_single_frame_extraction !== null && strpos($output_name, '%timecode') !== false && !(preg_match('/\%index/', $output_name) || strpos($output_name, '%index') !== false) && $is_image)
				{
					return $this->_raiseError('setOutput_%_missing');
//<-				exits
				}
				$process_name = '.'.$output_name_info['extension'];
// 				print_r(array($is_image, ($this->_single_frame_extraction !== null && $is_gif)));
				if($is_image || ($this->_single_frame_extraction !== null && $is_gif))
				{
					$process_name = '-%12d'.$process_name;
				}
			}
//			set the output address
			$this->_output_address = $output_directory.$output_name;
// 			set the processing address in the temp folder so it does not conflict with any other conversions
			$this->_process_address = $this->_tmp_directory.$this->unique().$process_name;
			$this->_overwrite_mode = $overwrite_mode;
			return true;
		}
		
		/**
		 * Sets a constant quality value to the encoding. (but a variable bitrate)
		 * 
		 * @param integer $quality The quality to adhere to. 100 is highest quality, 1 is the lowest quality
		 */
		function setConstantQuality($quality)
		{
// 			interpret quality into ffmpeg value
			$quality = 31 - round(($quality/100) * 31);
			if($quality > 31)
			{
				$quality = 31;
			}
			else if($quality < 1)
			{
				$quality = 1;
			}
			$this->addCommand('-qscale', $quality);
		}

		/**
		 * Translates a number of seconds to a timecode.
		 * NOTE: this is now a depreciated, use formatSeconds() instead.
		 *
		 * @depreciated Use formatSeconds() instead.
		 * @access public
		 * @uses PHPVideoToolkit::formatSeconds()
		 * @param integer $input_seconds The number of seconds you want to calculate the timecode for.
		 */
		function secondsToTimecode($input_seconds=0)
		{
			return $this->formatSeconds($input_seconds, '%hh:%mm:%ss');
		}

		/**
		 * Translates a timecode to the number of seconds.
		 * NOTE: this is now a depreciated, use formatTimecode() instead.
		 *
		 * @depreciated Use formatTimecode() instead.
		 * @access public
		 * @uses PHPVideoToolkit::formatTimecode()
		 * @param integer $input_seconds The number of seconds you want to calculate the timecode for.
		 */
		function timecodeToSeconds($input_timecode='00:00:00')
		{
			return $this->formatTimecode($input_timecode, '%hh:%mm:%ss', '%st');
		}
		
		/**
		 * Translates a number of seconds to a timecode.
		 *
		 * @access public
		 * @param integer $input_seconds The number of seconds you want to calculate the timecode for.
		 * @param integer $return_format The format of the timecode to return. The default is
		 * 		default '%hh:%mm:%ss'
		 * 			- %hh (hours) representative of hours
		 * 			- %mm (minutes) representative of minutes
		 * 			- %ss (seconds) representative of seconds
		 * 			- %fn (frame number) representative of frames (of the current second, not total frames)
		 * 			- %ms (milliseconds) representative of milliseconds (of the current second, not total milliseconds) (rounded to 3 decimal places)
		 * 			- %ft (frames total) representative of total frames (ie frame number)
		 * 			- %st (seconds total) representative of total seconds (rounded).
		 * 			- %sf (seconds floored) representative of total seconds (floored).
		 * 			- %sc (seconds ceiled) representative of total seconds (ceiled).
		 * 			- %mt (milliseconds total) representative of total milliseconds. (rounded to 3 decimal places)
		 * 		Thus you could use an alternative, '%hh:%mm:%ss:%ms', or '%hh:%mm:%ss' dependent on your usage.
		 * @param mixed|boolean|integer $frames_per_second The number of frames per second to translate for. If left false
		 * 		the class automagically gets the fps from PHPVideoToolkit::getFileInfo(), but the input has to be set
		 * 		first for this to work properly.
		 * @param boolean $use_smart_values Default value is true, if a format is found (ie %ss - secs) but no higher format (ie %mm - mins)
		 * 		is found then if $use_smart_values is true the value of of the format will be totaled.
		 * @return string|integer Returns the timecode, but if $frames_per_second is not set and a frame rate lookup is required 
		 * 		but can't be reached then -1 will be returned.
		 */
		function formatSeconds($input_seconds, $return_format='%hh:%mm:%ss', $frames_per_second=false, $use_smart_values=true)
		{
			$timestamp 		= mktime(0, 0, $input_seconds, 0, 0);
			$floored 		= floor($input_seconds);
			$hours  		= $input_seconds > 3600 ? floor($input_seconds/3600) : 0;
			$mins	  		= date('i', $timestamp);
			$searches 		= array();
			$replacements 	= array();
// 			these ones are the simple replacements
// 			replace the hours
			$using_hours = strpos($return_format, '%hh') !== false;
			if($using_hours)
			{
				array_push($searches, '%hh');
				array_push($replacements, $hours);
			}
// 			replace the minutes
			$using_mins = strpos($return_format, '%mm') !== false;
			if($using_mins)
			{
				array_push($searches, '%mm');
// 				check if hours are being used, if not and hours are required enable smart minutes
				if($use_smart_values === true && !$using_hours && $hours > 0)
				{
					$value = ($hours * 60) + $mins;
				}
				else
				{
					$value = $mins;
				}
				array_push($replacements, $value);
			}
// 			replace the seconds
			if(strpos($return_format, '%ss') !== false)
			{
// 				check if hours are being used, if not and hours are required enable smart minutes
				if($use_smart_values === true && !$using_mins && !$using_hours && $hours > 0)
				{
					$mins = ($hours * 60) + $mins;
				}
// 				check if mins are being used, if not and hours are required enable smart minutes
				if($use_smart_values === true && !$using_mins && $mins > 0)
				{
					$value = ($mins * 60) + date('s', $timestamp);
				}
				else
				{
					$value = date('s', $timestamp);
				}
				array_push($searches, '%ss');
				array_push($replacements, $value);
			}
// 			replace the milliseconds
			if(strpos($return_format, '%ms') !== false)
			{
				$milli = round($input_seconds - $floored, 3);
				$milli = substr($milli, 2);
				$milli = empty($milli) ? '0' : $milli;
				array_push($searches, '%ms');
				array_push($replacements, $milli);
			}
// 			replace the total seconds (rounded)
			if(strpos($return_format, '%st') !== false)
			{
				array_push($searches, '%st');
				array_push($replacements, round($input_seconds));
			}
// 			replace the total seconds (floored)
			if(strpos($return_format, '%sf') !== false)
			{
				array_push($searches, '%sf');
				array_push($replacements, floor($input_seconds));
			}
// 			replace the total seconds (ceiled)
			if(strpos($return_format, '%sc') !== false)
			{
				array_push($searches, '%sc');
				array_push($replacements, ceil($input_seconds));
			}
// 			replace the total seconds
			if(strpos($return_format, '%mt') !== false)
			{
				array_push($searches, '%mt');
				array_push($replacements, round($input_seconds, 3));
			}
// 			these are the more complicated as they depend on $frames_per_second / frames per second of the current input
			$has_frames = strpos($return_format, '%fn') !== false;
			$has_total_frames = strpos($return_format, '%ft') !== false;
			if($has_frames || $has_total_frames)
			{
// 				if the fps is false then we must automagically detect it from the input file
				if($frames_per_second === false)
				{
					$info = $this->getFileInfo();
// 					check the information has been received
					if($info === false || (!isset($info['video']) || !isset($info['video']['frame_rate'])))
					{
// 						fps cannot be reached so return -1
						return -1;
					}
					$frames_per_second = $info['video']['frame_rate'];
				}
// 				replace the frames
				$excess_frames = false;
				if($has_frames)
				{
					$excess_frames = ceil(($input_seconds - $floored) * $frames_per_second);
// 			print_r(array($input_seconds, $excess_frames));
					array_push($searches, '%fn');
					array_push($replacements, $excess_frames);
				}
// 				replace the total frames (ie frame number)
				if($has_total_frames)
				{
					$round_frames = $floored * $frames_per_second;
					if(!$excess_frames)
					{
						$excess_frames = ceil(($input_seconds - $floored) * $frames_per_second);
					}
					array_push($searches, '%ft');
					array_push($replacements, $round_frames + $excess_frames);
				}
			}
// 			print_r(array($searches, $replacements, $return_format));
// 			print_r(array($input_seconds, $timestamp, $return_format, str_replace($searches, $replacements, $return_format)));
			return str_replace($searches, $replacements, $return_format);
		}

		/**
		 * Translates a timecode to the number of seconds
		 *
		 * @access public
		 * @param integer $input_seconds The number of seconds you want to calculate the timecode for.
		 * @param integer $input_format The format of the timecode is being given in.
		 * 		default '%hh:%mm:%ss'
		 * 			- %hh (hours) representative of hours
		 * 			- %mm (minutes) representative of minutes
		 * 			- %ss (seconds) representative of seconds
		 * 			- %fn (frame number) representative of frames (of the current second, not total frames)
		 * 			- %ms (milliseconds) representative of milliseconds (of the current second, not total milliseconds) (rounded to 3 decimal places)
		 * 			- %ft (frames total) representative of total frames (ie frame number)
		 * 			- %st (seconds total) representative of total seconds (rounded).
		 * 			- %sf (seconds floored) representative of total seconds (floored).
		 * 			- %mt (milliseconds total) representative of total milliseconds. (rounded to 3 decimal places)
		 * 		Thus you could use an alternative, '%hh:%mm:%ss:%ms', or '%hh:%mm:%ss' dependent on your usage.
		 * @param integer $return_format The format of the timecode to return. The default is
		 * 		default '%ts'
		 * 			- %hh (hours) representative of hours
		 * 			- %mm (minutes) representative of minutes
		 * 			- %ss (seconds) representative of seconds
		 * 			- %fn (frame number) representative of frames (of the current second, not total frames)
		 * 			- %ms (milliseconds) representative of milliseconds (of the current second, not total milliseconds) (rounded to 3 decimal places)
		 * 			- %ft (frames total) representative of total frames (ie frame number)
		 * 			- %st (seconds total) representative of total seconds (rounded).
		 * 			- %sf (seconds floored) representative of total seconds (floored).
		 * 			- %sc (seconds ceiled) representative of total seconds (ceiled).
		 * 			- %mt (milliseconds total) representative of total milliseconds. (rounded to 3 decimal places)
		 * 		Thus you could use an alternative, '%hh:%mm:%ss:%ms', or '%hh:%mm:%ss' dependent on your usage.
		 * @param mixed|boolean|integer $frames_per_second The number of frames per second to translate for. If left false
		 * 		the class automagically gets the fps from PHPVideoToolkit::getFileInfo(), but the input has to be set
		 * 		first for this to work properly.
		 * @param boolean $use_smart_values Default value is true, if a format is found (ie %ss - secs) but no higher format (ie %mm - mins)
		 * 		is found then if $use_smart_values is true the value of of the format will be totaled.
		 * @return float Returns the value of the timecode in seconds.
		 */
		function formatTimecode($input_timecode, $input_format='%hh:%mm:%ss', $return_format='%ts', $frames_per_second=false, $use_smart_values=true)
		{
// 			first we must get the timecode into the current seconds
			$input_quoted 	= preg_quote($input_format);
			$placeholders 	= array('%hh', '%mm', '%ss', '%fn', '%ms', '%ft', '%st', '%sf', '%sc', '%mt');
			$seconds 		= 0;
			$input_regex 	= str_replace($placeholders, '([0-9]+)', preg_quote($input_format));
			preg_match('/'.$input_regex.'/', $input_timecode, $matches);
// 			work out the sort order for the placeholders
			$sort_table = array();
			foreach($placeholders as $key=>$placeholder)
			{
				if(($pos = strpos($input_format, $placeholder)) !== false)
				{
					$sort_table[$pos] = $placeholder;
				}
			}
			ksort($sort_table);
// 			check to see if frame related values are in the input
			$has_frames = strpos($input_format, '%fn') !== false;
			$has_total_frames = strpos($input_format, '%ft') !== false;
			if($has_frames || $has_total_frames)
			{
// 				if the fps is false then we must automagically detect it from the input file
				if($frames_per_second === false)
				{
					$info = $this->getFileInfo();
// 					check the information has been received
					if($info === false || (!isset($info['video']) || !isset($info['video']['frame_rate'])))
					{
// 						fps cannot be reached so return -1
						return -1;
					}
					$frames_per_second = $info['video']['frame_rate'];
				}
			}			
// 			increment the seconds with each placeholder value
			$key = 1;
			foreach($sort_table as $placeholder)
			{
				if(!isset($matches[$key]))
				{
					break;
				}
				$value = $matches[$key];
				switch($placeholder)
				{
// 					time related ones
					case '%hh' : 
						$seconds += $value * 3600;
						break;
					case '%mm' : 
						$seconds += $value * 60;
						break;
					case '%ss' : 
					case '%sf' :
					case '%sc' :
						$seconds += $value;
						break;
					case '%ms' : 
						$seconds += floatval('0.'.$value);
						break;
					case '%st' : 
					case '%mt' :
						$seconds = $value;
						break 1;
						break;
// 					frame related ones
					case '%fn' : 
						$seconds += $value/$frames_per_second;
						break;
					case '%ft' : 
						$seconds = $value/$frames_per_second;
						break 1;
						break;
				}
				$key += 1;
			}
// 			then we just format the seconds
			return $this->formatSeconds($seconds, $return_format, $frames_per_second, $use_smart_values);
		}
		

		/**
		 * This is a function that joins multiple input sources into one source before
		 * the final processing takes place. All videos are temporarily converted into mpg for
		 * joining.
		 * 
		 * PLEASE NOTE. This process is experimental an might not work on all systems.
		 *
		 * @access private
		 * @param boolean $log
		 */
		function _joinInput($log)
		{
			die('INPUT CANNOT YET BE JOINED.');
// ---- ffmpeg works
/*
mkfifo /Users/ollie/Sites/@Projects/ffmpeg/checkout/root/examples/tmp/intermediate1.mpg
mkfifo /Users/ollie/Sites/@Projects/ffmpeg/checkout/root/examples/tmp/intermediate2.mpg
ffmpeg -i /Users/ollie/Sites/@Projects/ffmpeg/checkout/root/examples/tmp/MOV02820.MPG -sameq -y /Users/ollie/Sites/@Projects/ffmpeg/checkout/root/examples/tmp/intermediate1.mpg < /dev/null &
ffmpeg -i /Users/ollie/Sites/@Projects/ffmpeg/checkout/root/examples/tmp/MOV02832.MPG -sameq -y /Users/ollie/Sites/@Projects/ffmpeg/checkout/root/examples/tmp/intermediate2.mpg < /dev/null &
cat /Users/ollie/Sites/@Projects/ffmpeg/checkout/root/examples/tmp/intermediate1.mpg /Users/ollie/Sites/@Projects/ffmpeg/checkout/root/examples/tmp/intermediate2.mpg |\
ffmpeg -f mpeg -i - -sameq -vcodec flv -acodec mp3 -ar 22050 /Users/ollie/Sites/@Projects/ffmpeg/checkout/root/examples/tmp/output.flv
*/
// ---- mencoder works
/*
PHPVIDEOTOOLKIT_MENCODER_BINARY.' -oac copy -ovc copy -idx -o '.$temp_file.' '.implode(' ', $this->_input_file);
*/
// 			run a libmp3lame check as it require different mp3 codec
			$audio_codec = 'mp3';
			$info = $this->getFFmpegInfo();
			if(isset($info['binary']['configuration']) && in_array('--enable-libmp3lame', $info['binary']['configuration']))
			{
				$audio_codec = 'libmp3lame';
			}
			
// 			build commands
			$temp_files = array();
			$mkinfo_commands = array();
			$ffmpeg_commands = array();
			$cat_files = array();
			$unique = $this->unique();
			foreach($this->_input_file as $key=>$file)
			{
				$unique_name = $this->_tmp_directory.$unique.'-'.$key.'-temp.mpg';
				$unique_name_escaped = escapeshellarg($unique_name);
				
				$logfile1 = $this->_tmp_directory.$unique.'-'.$key.'-log1.log';
				$logfile2 = $this->_tmp_directory.$unique.'-'.$key.'-log2.log';
				
				array_push($mkinfo_commands, array('cmd'=> 'mkfifo '.$unique_name_escaped.($log ? ' &> '.$logfile1 : ''), 'logfile'=>$logfile1));
				array_push($ffmpeg_commands, array('cmd'=> PHPVIDEOTOOLKIT_FFMPEG_BINARY.' -i '.escapeshellarg($file).' -acodec '.$audio_codec.' -sameq '.$unique_name_escaped.' < /dev/null '.($log ? '&> '.$logfile2 : '&'), 'logfile'=>$logfile2));
				array_push($cat_files, $unique_name_escaped);
// 				array_push($this->_unlink_files, $unique_name);
				if($log)
				{
// 					array_push($this->_unlink_files, $logfile1);
// 					array_push($this->_unlink_files, $logfile2);
				}
			}
// 			start log
			if($log)
			{
				$log_lines = array();
				array_unshift($log_lines, $this->_getMessage('ffmpeg_log_separator'), $this->_getMessage('ffmpeg_log_ffmpeg_join_gunk'), $this->_getMessage('ffmpeg_log_separator'));
			}
// 			mkinfo for temp files
			foreach($mkinfo_commands as $cmd)
			{
// 				exec($cmd['cmd']);
				echo($cmd['cmd']."\r\n");
				if($log)
				{
					array_push($log_lines, '---------', trim(file_get_contents($cmd['logfile'])));
				}
			}
// 			extract data
			foreach($ffmpeg_commands as $cmd)
			{
// 				exec($cmd['cmd']);
				echo($cmd['cmd']."\r\n");
				if($log)
				{
					array_push($log_lines, trim(file_get_contents($cmd['logfile'])), '---------');
				}
			}
			
// 			join command
			$unique = $this->unique();
			$temp_join_file = $this->_tmp_directory.$unique.'-combined-joined.mpg';
			$temp_join_file_escaped = escapeshellarg($temp_join_file);
			$temp_process_file = $this->_tmp_directory.$unique.'-combined-temp.mpg';
			$temp_process_file_escaped = escapeshellarg($temp_process_file);
			$logfile = $this->_tmp_directory.$unique.'.log';
// 			command for use with cat mkinfo files
// 			exec('cat '.implode(' ', $cat_files).' |\
// '.PHPVIDEOTOOLKIT_FFMPEG_BINARY.' -f mpeg -i - -sameq -vcodec mpeg4 -acodec '.$audio_codec.'  '.escapeshellarg($temp_process_file).($log ? ' &> '.$logfile : ''));
			echo('cat '.implode(' ', $cat_files).' |\
'.PHPVIDEOTOOLKIT_FFMPEG_BINARY.' -f mpeg -i - -sameq -vcodec mpeg4 -acodec '.$audio_codec.' '.escapeshellarg($temp_process_file).($log ? ' &> '.$logfile : '')."\r\n");
// 			echo('cat '.implode(' ', $cat_files).' > '.$temp_join_file_escaped.'
// '.PHPVIDEOTOOLKIT_FFMPEG_BINARY.' -i '.$temp_join_file_escaped.' -sameq -vcodec mpeg4 -acodec '.$audio_codec.' '.$temp_process_file_escaped.($log ? ' &> '.$logfile : ''));
// 			exec('cat '.implode(' ', $cat_files).' > '.$temp_join_file_escaped.'
// '.PHPVIDEOTOOLKIT_FFMPEG_BINARY.' -i '.$temp_join_file_escaped.' -sameq -vcodec mpeg4 -acodec '.$audio_codec.' '.$temp_process_file_escaped.($log ? ' &> '.$logfile : ''));
			if($log)
			{
				array_push($log_lines, trim(file_get_contents($logfile)));
				array_push($this->_unlink_files, $logfile);
				$this->_addToLog($log_lines, 'r+');
				print_r($log_lines);
			}
			
//			create a temp dir in the temp dir
// 			$temp_file = $this->_tmp_directory.$this->unique().'.'.array_pop(explode('.', $this->_process_address));
// 			print_r($temp_file);
			$this->addCommand('-i', $temp_process_file);
		// 	array_push($this->_unlink_files, $temp_process_file);
			
			
			exit;
		}

		/**
		 * Checks to see if a given codec can be encoded by the current ffmpeg binary.
		 * @access public
		 * @param $codec string The shortcode for the codec to check for.
		 * @return boolean True if the codec can be encoded by ffmpeg, otherwise false.
		 */
		function canCodecBeEncoded($codec)
		{
			return $this->validateCodec($codec, 'encode');
		}
		
		/**
		 * Checks to see if a given codec can be decoded by the current ffmpeg binary.
		 * @access public
		 * @param $codec string The shortcode for the codec to check for.
		 * @return boolean True if the codec can be decoded by ffmpeg, otherwise false.
		 */
		function canCodecBeDecoded($codec)
		{
			return $this->validateCodec($codec, 'decode');
		}
		
		/**
		 * Checks to see if a given codec can be decoded by the current ffmpeg binary.
		 * @access public
		 * @param $codec string The shortcode for the codec to check for.
		 * @param $method string 'encode' or 'decode', The method to check against the codec
		 * @return boolean True if the codec can be used with the diven method by ffmpeg, otherwise false.
		 */
		function validateCodec($codec, $method)
		{
			$info = $this->getFFmpegInfo();
			return isset($info['formats'][$codec]) && isset($info['formats'][$codec][$method]) ? $info['formats'][$codec][$method] : false;
		}

		/**
		 * Returns the available codecs.
		 * @access public
		 * @return array An array of codecs available to ffmpeg.
		 */
		function getAvailableCodecs()
		{
			$info = $this->getFFmpegInfo();
			return array_keys($info['formats']);
		}

		/**
		 * Commits all the commands and executes the ffmpeg procedure. This will also attempt to validate any outputted files in order to provide
		 * some level of stop and check system.
		 *
		 * @access public
		 * @param $multi_pass_encode boolean Determines if multi (2) pass encoding should be used.
		 * @param $log boolean Determines if a log file of the results should be generated.
		 * @return mixed 
		 * 		- false 										On error encountered.
		 * 		- PHPVIDEOTOOLKIT_RESULT_OK (bool true)					If the file has successfully been processed and moved ok to the output address
		 * 		- PHPVIDEOTOOLKIT_RESULT_OK_BUT_UNWRITABLE (int -1)		If the file has successfully been processed but was not able to be moved correctly to the output address
		 * 														If this is the case you will manually need to move the processed file from the temp directory. You can
		 * 														get around this by settings the third argument from PHPVideoToolkit::setOutput(), $overwrite to true.
		 * 		- n (int)										A positive integer is only returned when outputting a series of frame grabs from a movie. It dictates
		 * 														the total number of frames grabbed from the input video. You should also not however, that if a conflict exists
		 * 														with one of the filenames then this return value will not be returned, but PHPVIDEOTOOLKIT_RESULT_OK_BUT_UNWRITABLE
		 * 														will be returned instead.
		 * 	Because of the mixed return value you should always go a strict evaluation of the returned value. ie
		 * 
		 * 	$result = $toolkit->excecute();
		 *  if($result === false)
		 *  {
		 * 		// error
		 *  }
		 *  else if($result === PHPVIDEOTOOLKIT_RESULT_OK_BUT_UNWRITABLE)
		 *  {
		 * 		// ok but a manual move is required. The file to move can be it can be retrieved by $toolkit->getLastOutput();
		 *  }
		 *  else if($result === PHPVIDEOTOOLKIT_RESULT_OK)
		 *  {
		 * 		// everything is ok.
		 *  }
		 */
		function execute($multi_pass_encode=false, $log=false)
		{
// 			check for inut and output params
			$has_placeholder = preg_match('/\%([0-9]+)index/', $this->_process_address) || (strpos($this->_process_address, '%index') === false && strpos($this->_process_address, '%timecode') === false);
			if($this->_input_file === null && !$has_placeholder)
			{
				return $this->_raiseError('execute_input_404');
//<-			exits
			}
//			check to see if the output address has been set
			if($this->_process_address === null)
			{
				return $this->_raiseError('execute_output_not_set');
//<-			exits
			}
			
// 			check if temp dir is required and is writable
			if(($multi_pass_encode || $log) && !is_writable($this->_tmp_directory))
			{                                            
				return $this->_raiseError('execute_temp_unwritable');
//<-			exits
			}
			
			if(($this->_overwrite_mode == PHPVIDEOTOOLKIT_OVERWRITE_PRESERVE || $this->_overwrite_mode == PHPVIDEOTOOLKIT_OVERWRITE_FAIL) && is_file($this->_process_address))
			{
				return $this->_raiseError('execute_overwrite_process');
//<-			exits
			}
			
// 			carry out some overwrite checks if required
			$overwrite = '';
			switch($this->_overwrite_mode)
			{
				case PHPVIDEOTOOLKIT_OVERWRITE_UNIQUE :
// 					insert a unique id into the output address (the process address already has one)
					$unique = $this->unique();
					$last_index = strrpos($this->_output_address, DS);
					$this->_output_address = substr($this->_output_address, 0, $last_index+1).$unique.'-'.substr($this->_output_address, $last_index+1);
					break;
					
				case PHPVIDEOTOOLKIT_OVERWRITE_EXISTING :
// 					add an overwrite command to ffmpeg execution call
					$overwrite = '-y ';
					break;
					
				case PHPVIDEOTOOLKIT_OVERWRITE_PRESERVE :
// 					do nothing as the preservation comes later
					break;
					
				case PHPVIDEOTOOLKIT_OVERWRITE_FAIL :
				default :
// 					if the file should fail
					if(!$has_placeholder && is_file($this->_output_address))
					{
						return $this->_raiseError('execute_overwrite_fail');
//<-					exits
					}
					break;
			}
			
			$this->_timer_start = PHPVideoToolkit::microtimeFloat();
			
// 			we have multiple inputs that require joining so convert them to a joinable format and join
			if(is_array($this->_input_file))
			{
				$this->_joinInput($log);
			}
			
//			add the input file command to the mix
			$this->addCommand('-i', $this->_input_file);
			
// 			if multi pass encoding is enabled add the commands and logfile
			if($multi_pass_encode)
			{
				$multi_pass_file = $this->_tmp_directory.$this->unique().'-multipass';
				$this->addCommand('-pass', 1);
				$this->addCommand('-passlogfile', $multi_pass_file);
			}
			
// 			check to see if the format has been set and if it hasn't been set and the extension is a gif 
// 			we need to add an extra argument to set the pix format.
			$format = $this->hasCommand('-f');
			if($format === false)
			{
				$extension = strtolower(array_pop(explode('.', $this->_input_file)));
				if($extension === 'gif')
				{
					$this->addCommand('-pix_fmt', 'rgb24');
				}
			}
			else if($format === PHPVIDEOTOOLKIT_FORMAT_GIF)
			{
				$this->addCommand('-pix_fmt', 'rgb24');
			}

// 			check to see if an aspect ratio is set, if it is correct the width and heights to reflect that aspect ratio.
// 			This isn't strictly needed it is purely for informational purposes that this is done, because if the width is not
// 			inline with what is should be according to the aspect ratio ffmpeg will report the wrong final width and height
// 			when using it to lookup information about the file.
			$ratio = $this->hasCommand('-aspect');
			if($ratio !== false)
			{
				$size = $this->hasCommand('-s');
				if($size === false)
				{
					$info = $this->getFileInfo();
					if(isset($info['video']) && isset($info['video']['dimensions']))
					{
						$size = $info['video']['dimensions']['width'].'x'.$info['video']['dimensions']['height'];
					}					
				}
				if($size !== false)
				{
					$dim = explode('x', substr($size, 1, -1));
					if(($boundry = strpos($ratio, ':')) !== false)
					{
						$ratio = substr($ratio, 1, $boundry-1)/substr($ratio, $boundry+1, -1);
						$new_width = round($dim[1]*$ratio);
// 						make sure new width is an even number
						$ceiled = ceil($new_width);
						$new_width = $ceiled % 2 !== 0 ? floor($new_width) : $ceiled;
						if($new_width != $dim[0])
						{
							$this->setVideoDimensions($new_width, $dim[1]);
						}
					}
					else if(strpos($ratio, '.') !== false)
					{
						$ratio = floatval($ratio);
						$new_width = $dim[1]*$ratio;
// 						make sure new width is an even number
						$ceiled = ceil($new_width);
						$new_width = $ceiled % 2 !== 0 ? floor($new_width) : $ceiled;
						if($new_width != $dim[0])
						{
							$this->setVideoDimensions($new_width, $dim[1]);
						}
					}
				}
			}

//			combine all the output commands
			$command_string = $this->_combineCommands();
//			prepare the command suitable for exec
//			the input and overwrite commands have specific places to be set so they have to be added outside of the combineCommands function
			$exec_string = $this->_prepareCommand(PHPVIDEOTOOLKIT_FFMPEG_BINARY, $command_string, $overwrite.escapeshellcmd($this->_process_address));
// 			$exec_string = $this->_prepareCommand(PHPVIDEOTOOLKIT_FFMPEG_BINARY, '-i '.$this->_commands['-i'].' '.$command_string, $overwrite.escapeshellcmd($this->_process_address));
			if($log)
			{
				$this->_log_file = $this->_tmp_directory.$this->unique().'.info';
				array_push($this->_unlink_files, $this->_log_file);
				$exec_string = $exec_string.' &> '.$this->_log_file;
			}
			
//			execute the command
			exec($exec_string);
			
//			track the processed command by adding it to the class
			array_unshift($this->_processed, $exec_string);
			
// 			create the multiple pass encode
			if($multi_pass_encode)
			{
				$pass2_exc_string = str_replace('-pass '.escapeshellarg(1), '-pass '.escapeshellarg(2), $exec_string);
				exec($pass2_exc_string);
				$this->_processed[0] = array($this->_processed[0], $pass2_exc_string);
// 				remove the multipass log file
				unlink($multi_pass_file.'-0.log');
			}
// 			keep track of the time taken
			$execution_time = PHPVideoToolkit::microtimeFloat() - $this->_timer_start;
			array_unshift($this->_timers, $execution_time);
			
// 			add the exec string to the log file
			if($log)
			{
				$lines = $this->_processed[0];
				if(!is_array($lines))
				{
					$lines = array($lines);
				}
				array_unshift($lines, $this->_getMessage('ffmpeg_log_separator'), $this->_getMessage('ffmpeg_log_ffmpeg_command'), $this->_getMessage('ffmpeg_log_separator'));
				array_unshift($lines, $this->_getMessage('ffmpeg_log_separator'), $this->_getMessage('ffmpeg_log_ffmpeg_gunk'), $this->_getMessage('ffmpeg_log_separator'));
				$this->_addToLog($lines, 'r+');
			}
// 			exit
//			must validate a series of outputed items
//			detect if the output address is a sequence output
			if(preg_match('/\%([0-9]+)d/', $this->_process_address, $d_matches) || strpos($this->_process_address, '%d') !== false)
			{
//				get the path details
				$process_info 	= pathinfo($this->_process_address);
				$output_info 	= pathinfo($this->_output_address);
				$pad_amount 	= intval($d_matches[1]);
// 				print_r(array($process_info, $output_info));
				
// 				get the %index padd amounts
				$has_preg_index = preg_match('/\%([0-9]+)index/', $output_info['basename'], $index_matches);
				$output_index_pad_amount = isset($index_matches[1]) ? intval($index_matches[1], 1) : 0;
// 				var_dump($index_matches);
				
//				init the iteration values
				$num 			= 1;
				$files 			= array();
				$produced	 	= array();
				$error			= false;
				$name_conflict	= false;
				$file_exists	= false;
				
// 				get the first files name
				$filename 		= $process_info['dirname'].DS.str_replace($d_matches[0], str_pad($num, $pad_amount, '0', STR_PAD_LEFT), $process_info['basename']);
				$use_timecode	= strpos($output_info['basename'], '%timecode') !== false;
				$use_index		= $has_preg_index || strpos($output_info['basename'], '%index') !== false;
				
// 				if(!$use_timecode && $use_index)
// 				{
// 					if($log)
// 					{
// 						$this->_logResult('execute_overwrite_fail');
// 					}
// 					return $this->_raiseError('execute_overwrite_fail');
// 				}
				
// 				start the timecode pattern replacement values
				if($use_timecode)
				{
					$secs_start = $this->formatTimecode($this->_image_output_timecode_start, '%hh:%mm:%ss.%ms', '%mt', $this->_image_output_timecode_fps);
					$fps_inc = 1/$this->_image_output_timecode_fps;
					$fps_current_sec = 0;
					$fps_current_frame = 0;
				}
				
//				loop checking for file existence
				while(@is_file($filename))
				{
//					check for empty file
					$size = filesize($filename);
					if($size == 0)
					{
						$error = true;
					}
					array_push($produced, $filename);
// 					create the substitution arrays
					$searches 		= array();
					$replacements 	= array();
					if($use_index)
					{
						array_push($searches, isset($index_matches[0]) ? $index_matches[0] : '%index');
						array_push($replacements, str_pad($num, $output_index_pad_amount, '0', STR_PAD_LEFT));
					}
// 					check if timecode is in the output name, no need to use it if not
					if($use_timecode)
					{
						$fps_current_sec 	+= $fps_inc;
						$fps_current_frame 	+= 1;
						if($fps_current_sec >= 1)
						{
							$fps_current_sec 	 = $fps_inc;
							$secs_start 		+= 1;
							$fps_current_frame 	 = 1;
						}
						$timecode = $this->formatSeconds($secs_start, $this->image_output_timecode_format, $this->_image_output_timecode_fps);
						$timecode 		= str_replace(array(':', '.'), $this->timecode_seperator_output, $timecode);
// 						add to the substitution array
						array_push($searches, '%timecode');
						array_push($replacements, $timecode);
					}
// 					check if the file exists already and if it does check that it can be overriden
					$old_filename = $filename;
// 					print_r(array($searches, $replacements, $output_info['basename']));
					$new_file = str_replace($searches, $replacements, $output_info['basename']);
					$new_filename = $output_info['dirname'].DS.$new_file;
// 					var_dump($filename, $new_filename);
					if(!is_file($new_filename) || $this->_overwrite_mode == PHPVIDEOTOOLKIT_OVERWRITE_EXISTING)
					{
						rename($filename, $new_filename);
						$filename = $new_filename;
					}
// 					the file exists and is not allowed to be overriden so just rename in the temp directory using the timecode
					else if($this->_overwrite_mode == PHPVIDEOTOOLKIT_OVERWRITE_PRESERVE)
					{
						$new_filename = $process_info['dirname'].DS.'tbm-'.$this->unique().'-'.$new_file;
						rename($filename, $new_filename);
						$filename = $new_filename;
// 						add the error to the log file
						if($log)
						{
							$this->_logResult('execute_image_file_exists', array('file'=>$new_filename));
						}
// 						flag the conflict
						$file_exists = true;
					}
// 					the file exists so the process must fail
					else
					{
// 						add the error to the log file
						if($log)
						{
							$this->_logResult('execute_overwrite_fail');
						}
// 						tidy up the produced files
						array_merge($this->_unlink_files, $produced);
						return $this->_raiseError('execute_overwrite_fail');
					}
//					process the name change if the %d is to be replaced with the timecode
					$num += 1;
					$files[$filename] = $size > 0 ? basename($filename) : false;
// 					print_r("\r\n\r\n".is_file($old_filename)." - ".$old_filename.' => '.$new_filename);
// 					print_r($files);
// 					get the next incremented filename to check for existance
					$filename = $process_info['dirname'].DS.str_replace($d_matches[0], str_pad($num, $pad_amount, '0', STR_PAD_LEFT), $process_info['basename']);
				}
//				de-increment the last num as it wasn't found
				$num -= 1;

//				if the file was detected but were empty then display a different error
				if($error === true)
				{
// 					add the error to the log file
					if($log)
					{
						$this->_logResult('execute_partial_error', array('input'=>$this->_input_file));
					}
					return $this->_raiseError('execute_partial_error', array('input'=>$this->_input_file));
//<-				exits
				}
// 				post process any files
// 				print_r($files);
				$post_process_result = $this->_postProcess($log, $files);
// 				print_r($files);
				if(is_array($post_process_result))
				{
// 					post process has occurred and everything is fine
					$num = count($files);
				}
				else if($post_process_result !== false)
				{
// 					the file has encountered an error in the post processing of the files
					return $post_process_result;
				}
// 			var_dump("\r\n\r\n", $files, __LINE__, __FILE__, "\r\n\r\n");
// 			exit;
// 				if the result is false then no post process has taken place

				$this->_process_file_count = $num;

//				no files were generated in this sequence
				if($num == 0)
				{
// 					add the error to the log file
					if($log)
					{
						$this->_logResult('execute_image_error', array('input'=>$this->_input_file));
					}
					return $this->_raiseError('execute_image_error', array('input'=>$this->_input_file));
//<-				exits
				}
				
//				add the files the the class a record of what has been generated
				array_unshift($this->_files, $files);
				
				array_push($lines, $this->_getMessage('ffmpeg_log_separator'), $this->_getMessage('ffmpeg_log_ffmpeg_output'), $this->_getMessage('ffmpeg_log_separator'), implode("\n", $files));
				$this->_addToLog($lines, 'r+');
				
				return $file_exists ? PHPVIDEOTOOLKIT_RESULT_OK_BUT_UNWRITABLE : PHPVIDEOTOOLKIT_RESULT_OK;
			}
//			must validate one file
			else
			{
//				check that it is a file
				if(!is_file($this->_process_address))
				{
// 					add the error to the log file
					if($log)
					{
						$this->_logResult('execute_output_404', array('input'=>$this->_input_file));
					}
					return $this->_raiseError('execute_output_404', array('input'=>$this->_input_file));
//<-				exits
				}
//				the file does exist but is it empty?
				if(filesize($this->_process_address) == 0)
				{
// 					add the error to the log file
					if($log)
					{
						$this->_logResult('execute_output_empty', array('input'=>$this->_input_file));
					}
					return $this->_raiseError('execute_output_empty', array('input'=>$this->_input_file));
//<-				exits
				}
// 				the file is ok so move to output address
				if(!is_file($this->_output_address) || $this->_overwrite_mode == PHPVIDEOTOOLKIT_OVERWRITE_EXISTING)
				{
// 					post process any files
					$post_process_result = $this->_postProcess($log, array($this->_process_address));
					if(is_array($post_process_result) || $post_process_result === true)
					{
// 						post process has occurred and everything is fine
					}
					else if($post_process_result !== false)
					{
						return $post_process_result;
					}
// 					if the result is false then no post process has taken place

// 					rename the file to the final destination and check it went ok
					if(rename($this->_process_address, $this->_output_address))
					{
						array_push($lines, $this->_getMessage('ffmpeg_log_separator'), $this->_getMessage('ffmpeg_log_ffmpeg_output'), $this->_getMessage('ffmpeg_log_separator'), $this->_output_address);
						$this->_addToLog($lines, 'r+');
						
// 						the file has been renamed ok
// 						add the error to the log file
						if($log)
						{
							$this->_logResult('execute_result_ok', array('output'=>$this->_output_address));
						}
						$this->_process_file_count = 1;
//						add the file the the class a record of what has been generated
						array_unshift($this->_files, array($this->_output_address));
						return PHPVIDEOTOOLKIT_RESULT_OK;
					}
// 					renaming failed so return ok but erro
					else
					{
// 						add the error to the log file
						if($log)
						{
							$this->_logResult('execute_result_ok_but_unwritable', array('process'=>$this->_process_address, 'output'=>$this->_output_address));
						}
//						add the file the the class a record of what has been generated
						array_unshift($this->_files, array($this->_process_address));
						array_push($lines, $this->_getMessage('ffmpeg_log_separator'), $this->_getMessage('ffmpeg_log_ffmpeg_output'), $this->_getMessage('ffmpeg_log_separator'), $this->_process_address);
						$this->_addToLog($lines, 'r+');
						return PHPVIDEOTOOLKIT_RESULT_OK_BUT_UNWRITABLE;
					}
				}
// 				if it is not we signal that it has been created but has not been moved.
				else if($this->_overwrite_mode == PHPVIDEOTOOLKIT_OVERWRITE_PRESERVE)
				{
// 					add the error to the log file
					if($log)
					{
						$this->_logResult('execute_result_ok_but_unwritable', array('process'=>$this->_process_address, 'output'=>$this->_output_address));
					}
//					add the file the the class a record of what has been generated
					array_unshift($this->_files, array($this->_process_address));
					return PHPVIDEOTOOLKIT_RESULT_OK_BUT_UNWRITABLE;
				}
// 				the file exists so the process must fail
				else
				{
// 					add the error to the log file
					if($log)
					{
						$this->_logResult('execute_overwrite_fail');
					}
// 					tidy up the produced files
					array_push($this->_unlink_files, $this->_process_address);
					return $this->_raiseError('execute_overwrite_fail');
				}
			}

			return null;

		}
		
		/**
		 * This function registers a post process after the internal handling of the ffmpeg output has been cleaned and checked.
		 * Each function that is set will be called in the order it is set unless an index is specified. All callbacks will be 
		 * supplied with one argument with is an array of the outputted files. 
		 * 
		 * NOTE1: If a post process function is being applied to an outputted video or audio then the process will be applied 
		 * before it has been moved to it's final destination, however if the output is an image sequence the post process 
		 * function will be called after the images have been moved to their final destinations.
		 * 
		 * NOTE2: Also it is important to return a boolean 'true' if the post process has been carried out ok. If the process is not
		 * a true value then the value will be treated/returned as an error and if applicable logged.
		 * 
		 * @access public
		 * @param string $function The name of a function
		 * @param object|boolean $class The name of the callback class. If left as false the callback will be treated as a standalone function.
		 * @param integer|boolean $index The index of the callback array to put the callback into. If left as false it will be pushed to the end of the array.
		 */
		function registerPostProcess($function, $class=false, $index=false)
		{
// 			create the callback
			$callback = $class === false ? $function : array(&$class, $function);
// 			add it to the post process array
			if($index === false)
			{
				array_push($this->_post_processes, $callback);
			}
			else
			{
				$this->_post_processes[$index] = $callback;
			}
		}
		
		/**
		 * Carries out the post processing of the files.
		 * 
		 * @access private
		 * @param boolean $log Determines if logging of errors should be carried out.
		 * @param array $files The array of files that have just been processed.
		 * @return mixed
		 */
		function _postProcess($log, $files)
		{
			if(count($this->_post_processes))
			{
// 				loop through the post processes
				foreach($this->_post_processes as $key=>$process)
				{
// 					call the process
					$return_value = call_user_func_array($process, array($files));
// 					if the return value is not strictly equal to true the result will be treated as an error and exit the process loop
					if(!is_array($return_value) && $return_value !== true)
					{
						if($log)
						{
							$this->_logResult($return_value);
						}
						return $this->_raiseError($return_value);
					}
				}
				return $return_value;
			}
			return false;
		}
		
		/**
		 * Returns the number of files outputted in this run. It will be reset when you call PHPVideoToolkit::reset();
		 * 
		 * @access public
		 * @return integer
		 */
		function getFileOutputCount()
		{
			return $this->_process_file_count;
		}
		
		/**
		 * Adds lines to the current log file.
		 * 
		 * @access private
		 * @param $message
		 * @param $replacements
		 */
		function _logResult($message, $replacements=false)
		{
			$this->_addToLog(array($this->_getMessage('ffmpeg_log_separator'), $this->_getMessage('ffmpeg_log_ffmpeg_result'), $this->_getMessage('ffmpeg_log_separator'), $this->_getMessage($message, $replacements)));
		}
		
		/**
		 * Adds lines to the current log file.
		 * 
		 * @access private
		 * @param $lines array An array of lines to add to the log file.
		 */
		function _addToLog($lines, $where='a')
		{
			$handle = fopen($this->_log_file, $where);
			if(is_array($lines))
			{
				$data = implode("\n", $lines)."\n";
			}
			else
			{
				$data = $lines."\n";
			}
			fwrite($handle, $data);
			fclose($handle);
		}
		
		/**
		 * Moves the current log file to another file.
		 * 
		 * @access public
		 * @param $destination string The absolute path of the new filename for the log.
		 * @return boolean Returns the result of the log file rename.
		 */
		function moveLog($destination)
		{
			$result = false;
			if($this->_log_file !== null && is_file($this->_log_file))
			{
				$result = rename($this->_log_file, $destination);
				$this->_log_file = $destination;
			}
			return $result;
		}

		/**
		 * Reads the current log file
		 * 
		 * @access public
		 * @return string|boolean Returns the current log file content. Returns false on failure.
		 */
		function readLog()
		{
			if($this->_log_file !== null && is_file($this->_log_file))
			{
				$handle = fopen($this->_log_file, 'r');
				$contents = fread($handle, filesize($this->_log_file));
				fclose($handle);
				return $contents;
			}
			return false;
		}

		/**
		 * Returns the last outputted file that was processed by ffmpeg from this class.
		 *
		 * @access public
		 * @return mixed array|string Will return an array if the output was a sequence, or string if it was a single file output
		 */
		function getLastOutput()
		{
			return $this->_files[0];
		}

		/**
		 * Returns all the outputted files that were processed by ffmpeg from this class.
		 *
		 * @access public
		 * @return array
		 */
		function getOutput()
		{
			return $this->_files;
		}

		/**
		 * Returns the amount of time taken of the last file to be processed by ffmpeg.
		 *
		 * @access public
		 * @return mixed integer Will return the time taken in seconds.
		 */
		function getLastProcessTime()
		{
			return $this->_timers[0];
		}

		/**
		 * Returns the amount of time taken of all the files to be processed by ffmpeg.
		 *
		 * @access public
		 * @return array
		 */
		function getProcessTime()
		{
			return $this->_timers;
		}

		/**
		 * Returns the last encountered error message.
		 *
		 * @access public
		 * @return string
		 */
		function getLastError()
		{
			return $this->_errors[0];
		}

		/**
		 * Returns all the encountered errors as an array of strings
		 *
		 * @access public
		 * @return array
		 */
		function getErrors()
		{
			return $this->_errors;
		}

		/**
		 * Returns the last command that ffmpeg was given.
		 * (Note; if setFormatToFLV was used in the last command then an array is returned as a command was also sent to FLVTool2)
		 *
		 * @access public
		 * @return mixed array|string
		 */
		function getLastCommand()
		{
			return $this->_processed[0];
		}

		/**
		 * Returns all the commands sent to ffmpeg from this class
		 *
		 * @access public
		 * @return unknown
		 */
		function getCommands()
		{
			return $this->_processed;
		}

		/**
		 * Raises an error
		 *
		 * @access private
		 * @param string $message
		 * @param array $replacements a list of replacements in search=>replacement format
		 * @return boolean Only returns false if $toolkit->on_error_die is set to false
		 */
		function _raiseError($message, $replacements=false)
		{
			$msg = 'PHPVideoToolkit Error: '.$this->_getMessage($message, $replacements);
//			check what the error is supposed to do
			if($this->on_error_die === true)
			{
				die($msg);
//<-			exits
			}
//			add the error message to the collection
			array_unshift($this->_errors, $msg);
			return false;
		}

		/**
		 * Gets a message.
		 *
		 * @access private
		 * @param string $message
		 * @param array $replacements a list of replacements in search=>replacement format
		 * @return boolean Only returns false if $toolkit->on_error_die is set to false
		 */
		function _getMessage($message, $replacements=false)
		{
			$message = isset($this->_messages[$message]) ? $this->_messages[$message] : 'Unknown!!!';
			if($replacements)
			{
				$searches = $replaces = array();
				foreach($replacements as $search=>$replace)
				{
					array_push($searches, '#'.$search);
					array_push($replaces, $replace);
				}
				$message = str_replace($searches, $replaces, $message);
			}
			return $message;
		}

		/**
		 * Adds a command to be bundled into the ffmpeg command call.
		 * (SPECIAL NOTE; None of the arguments are checked or sanitized by this function. BE CAREFUL if manually using this. The commands and arguments are escaped
		 * however it is still best to check and sanitize any params given to this function)
		 *
		 * @access public
		 * @param string $command
		 * @param mixed $argument
		 * @return boolean
		 */
		function addCommand($command, $argument=false)
		{
			$this->_commands[$command] = $argument === false ? false : escapeshellarg($argument);
			return true;
		}

		/**
		 * Determines if the the command exits.
		 *
		 * @access public
		 * @param string $command
		 * @return mixed boolean if failure or value if exists.
		 */
		function hasCommand($command)
		{
 			return isset($this->_commands[$command]) ? ($this->_commands[$command] === false ? true : $this->_commands[$command]): false;
		}

		/**
		 * Combines the commands stored into a string
		 *
		 * @access private
		 * @return string
		 */
		function _combineCommands()
		{
			$before_input 	= array();
			$after_input 	= array();
			$input 			= null;
			foreach ($this->_commands as $command=>$argument)
			{
				$command_string = trim($command.(!empty($argument) ? ' '.$argument : ''));
//				check for specific none combinable commands as they have specific places they have to go in the string
				switch($command)
				{
					case '-i' :
						$input = $command_string;
						break;
					case '-inputr' :
						$command_string = trim('-r'.($argument ? ' '.$argument : ''));;
					default :
						if(in_array($command, $this->_cmds_before_input))
						{
							array_push($before_input, $command_string);
						}
						else
						{
							array_push($after_input, $command_string);
						}
				}
			}
			
			$before_input = count($before_input) ? implode(' ', $before_input).' ' : '';
			$after_input_string = ' ';
			if(count($after_input))
			{
				$input .= ' ';
				$after_input_string  = implode(' ', $after_input).' ';
			}
			
			return $before_input.$input.$after_input_string;
		}

		/**
		 * Prepares the command for execution
		 *
		 * @access private
		 * @param string $path Path to the binary
		 * @param string $command Command string to execute
		 * @param string $args Any additional arguments
		 * @return string
		 */
		function _prepareCommand($path, $command, $args='')
		{
	        if (strtoupper(substr(PHP_OS, 0, 3)) !== 'WIN' || !preg_match('/\s/', $path))
	        {
	            return $path.' '.$command.' '.$args;
	        }
	        return 'start /D "'.$path.'" /B '.$command.' '.$args;
		}

		/**
		 * Generates a unique id. Primarily used in jpeg to movie production
		 *
		 * @access public
		 * @param string $prefix
		 * @return string
		 */
		function unique($prefix='')
		{
			return uniqid($prefix.time().'-');
		}

		/**
		 * Destructs ffmpeg and removes any temp files/dirs
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
	}

