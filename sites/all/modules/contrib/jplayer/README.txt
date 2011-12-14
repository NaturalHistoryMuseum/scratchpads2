CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Player kinds
 * Upgrading
 * Known issues
 * Support
 * Sponsorship


INTRODUCTION
------------

The jPlayer module provides a wrapper around the jPlayer JavaScript library.
This library provides an HTML5-based player, that uses a Flash fallback for
browsers that do not support it. This module provides a default presentation for
the player, as well as integration with file fields. This makes it possible to
easily convert the display of any file field into an audio or video player.

This player will work with the media types jPlayer supports: mp3, m4a, m4v, oga,
ogv, webma, webmv, wav. (Also jpeg, jpg and png for poster artwork.) Please be
sure to restrict the file upload extensions on your file fields to only allow
these extensions, or less depending on your requirements.


INSTALLATION
------------

 1. Drop the 'jplayer' folder into the modules directory '/sites/all/modules/'.

 2. Download jPlayer from http://www.jplayer.org/download/. The downloaded
    directory only contains 2 files, 'jquery.jplayer.min.js' and
    'Jplayer.swf'. Place these two files in 'sites/all/libraries/jplayer/'.

 3. In your Drupal site, enable the module under Administration -> Modules
    '?q=/admin/modules'.

 4. Global admin settings for jPlayer can be found Administration ->
    Configuration -> jPlayer '?q=admin/config/media/jplayer'.

 5. When you manage the display of file fields within your content types 
    '?q=admin/structure/types/manage/{type}/display', choose 'jPlayer - Player'
    as the format. You can then configure the settings available for that
    instance.


PLAYER KINDS
------------

When configuring your formatter you will be given the option of two 'kinds' of 
jPlayer. Below is the difference between them:

 1. Single
    
    Intelligent single instance player which can take multiple formats of the
    same kind e.g. ogv and m4v and display the correct one depending on your
    browser. Also takes image files for poster frames. Works with both audio
    and video.
    
    For use on a multi-value file field, or single value if you only want to
    support one kind of media.
    
    If you happen to upload a video file type then the jPlayer player will
    automatically convert itself into a video player.

 2. Playlist
    
    For use on a multi-value file field. Each file uploaded will be added to a
    playlist. Which users will be able to navigation between. If you upload a
    video file then the player will convert to a video player automatically. If
    audio files are also present then a blank video frame will be displayed.
    
    You can also select your repeat option. If set to 'All' then the player will
    play all files within the playlist one after the other, once it reaches the
    end it will start again at the beginning. If set to 'Single' then it will
    repeat a single file within the playlist until the user chooses to play
    another file.


UPGRADING
---------

Since the jPlayer module is purely a formatter most of the upgrade work required
will be in upgrading from old CCK File Fields into new core File Fields in
Drupal 7. Apart from that you will need to re-select your formatters after
upgrading to this new version.


KNOWN ISSUES
------------

 * Advanced theming methods and documentation needed.


SUPPORT
-------

Many issues you may have may be caused by jPlayer library itself, rather than
the Drupal module. Check with the jPlayer support pages for issues with Flash
warnings or the player behaving oddly:

http://www.jplayer.org/support/

If the problem is with the jPlayer Drupal module, please file a support request
at http://drupal.org/project/issues/jplayer.


SPONSORSHIP
-----------

The D7 branch of this module is currently sponsored by Holy Trinity Brompton
(http://www.htb.org.uk), and Cultivate4 (http://www.cultivatefour.com). Work has
been undertaken by Jordan de Laune.

The original D6 module was written by Nate Haug and was maintained by Blazej
Owczarczyk.