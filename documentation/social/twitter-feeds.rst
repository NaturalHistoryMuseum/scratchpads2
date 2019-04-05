:orphan:

.. Twitter integration is broken - see https://github.com/NaturalHistoryMuseum/scratchpads2/issues/5877

Twitter feeds
=============

*You can easily embed twitter feeds in your Scratchpad pages. In order
for the twitter module to work you will have first to connect your
twitter account with the module.*


Link your account
~~~~~~~~~~~~~~~~~

1. From the :term:`Admin menu` go to *Configuration*
2. Under *Web services* select *Twitter*
3. Click *Go to Twitter and add an authenticated account*
4. Follow instructions from the twitter service and click *Authorise app*

Embed a hashtag search
~~~~~~~~~~~~~~~~~~~~~~

1. Edit your page or block.

2. Make sure that you edit the content of a page having the text format
   set to Filtered HTML

3. Use the following string where you want the feed to appear:
   [TWITTER:{hashtags}]

4. Replace {hashtags} with the hashtags you want the feed to search for.

5. Separate hashtags with a space. For example: [TWITTER:
   #naturalhistorymuseum #scratchpads]

.. figure:: /_static/Add_tweets_in_a_page.png

