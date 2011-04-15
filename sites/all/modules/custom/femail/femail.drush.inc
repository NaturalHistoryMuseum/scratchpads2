<?php

function femail_drush_command(){
  return array(
    'femail' => array(
      'callback' => 'femail_drush_callback',
      'description' => 'Drush command for importing mail messages into a forum'
    )
  );
}

function femail_drush_help($section){
  switch($section){
    case 'drush:femail':
      return dt("Imports a mail message into a site's Forum.  Usually called by Procmail or similar");
  }
}

function _femail_decode_text($text){
  if(strtolower(substr($text, 0, 8)) == '=?utf-8?'){
    if(strtolower(substr($text, 0, 9)) == '=?utf-8?q'){
      $text = quoted_printable_decode(substr($text, 10, strpos($text, '?=') - 10));
    }elseif(strtolower(substr($text, 0, 9)) == '=?utf-8?b'){
      $text = base64_decode(substr($text, 10, strpos($text, '?=') - 10));
    }
  }
  return $text;
}

function _femail_get_msg(){
  $filename = fread(STDIN, 200);
  $original_message = file_get_contents($filename);
  $msg = array(
    'body' => $original_message
  );
  $mailparse = mailparse_msg_create();
  mailparse_msg_parse($mailparse, $msg['body']);
  $structure = mailparse_msg_get_structure($mailparse);
  $body_is_set = FALSE;
  foreach($structure as $s){
    // Get the subject to use as the node/comment title
    $part = mailparse_msg_get_part($mailparse, $s);
    $part_data = mailparse_msg_get_part_data($part);
    if(isset($part_data['headers']['subject'])){
      $msg['title'] = _femail_decode_text($part_data['headers']['subject']);
    }
    // Get the From: header, which will be used to work out which user to
    // attribute this node/comment to
    if(isset($part_data['headers']['from'])){
      $from = mailparse_rfc822_parse_addresses($part_data['headers']['from']);
      $msg['from'] = $from[0]['address'];
    }
    // Get the reply-to header, this will tell us whether or not we should
    // continue parsing this message
    if(isset($part_data['headers']['reply-to'])){
      $msg['reply-to'] = $part_data['headers']['reply-to'];
    }
    // Get the To: header, which will be used to work out which forum this 
    // message wants to go into
    if(isset($part_data['headers']['envelope-to'])){
      $to = mailparse_rfc822_parse_addresses($part_data['headers']['envelope-to']);
      $msg['to'] = $to[0]['address'];
    }
    // Get the body for the node/comment body
    if(!$body_is_set){
      if(isset($part_data['headers']['content-type'])){
        if(strpos(strtolower($part_data['headers']['content-type']), 'text/plain') !== FALSE){
          if(isset($part_data['headers']['content-transfer-encoding']) && $part_data['headers']['content-transfer-encoding'] == 'base64'){
            $msg['body'] = base64_decode(substr($msg['body'], $part_data['starting-pos-body'], $part_data['ending-pos-body'] - $part_data['starting-pos-body']));
          }else{
            $msg['body'] = quoted_printable_decode(substr($msg['body'], $part_data['starting-pos-body'], $part_data['ending-pos-body'] - $part_data['starting-pos-body']));
          }
          // Lets get the charset, for those shite bloody email clients that
          // don't use UTF-8.  WTF I hear you cry, yes, that's right, why isn't 
          // everybody just using UTF-8 nowadays? GRRRRR!
          $charset_array = preg_split('/charset=/i', $part_data['headers']['content-type']);
          if(count($charset_array)){
            // We've got a specific charset, we'll need to convert the encoding
            // to UTF-8            
            $charset = array_pop($charset_array);
            // We also need to ensure that we haven't got other spazzy rubbish on
            // the end of this charset string
            if(strpos($charset, ';')){
              $charset = substr($charset, 0, strpos($charset, ';'));
            }
            $msg['body'] = iconv($charset, 'utf-8', $msg['body']);
          }
          $body_is_set = TRUE;
        }
      }
    }
    // We'll attempt to save the attachments
    if(isset($part_data['content-disposition']) && ($part_data['content-disposition'] == 'attachment' || $part_data['content-disposition'] == 'inline')){
      // First we check the file extension, and only continue if we're allowed 
      // to
      $allowed_extensions = explode(" ", variable_get('upload_extensions_default', 'jpg jpeg gif png txt doc xls pdf ppt pps odt ods odp'));
      $part_data['disposition-filename'] = _femail_decode_text($part_data['disposition-filename']);
      $pathinfo = pathinfo($part_data['disposition-filename']);
      if(substr(strtolower($pathinfo['extension']), 0, 3) == 'php'){ // Covers php, php3, phps, php4, php5, etc
        // We NEVER allow the upload of PHP files, as potentially messages could
        // have their "From" address spoofed, resulting in a potential route of
        // attack.
        watchdog('femail', 'Femail refuses to allow the upload of PHP files. Somebody just posted "@file"', array(
          '@file' => $pathinfo['basename']
        ), WATCHDOG_NOTICE);
      }else{
        if(in_array(strtolower($pathinfo['extension']), $allowed_extensions)){
          // Write the data to a tmp file, which will be moved later if the 
          // message was from a legitimate user.
          // As we're using Drush, we need the server root.
          $temp_name = tempnam(file_directory_temp(), 'fem');
          if($part_data['transfer-encoding'] == 'base64'){
            file_put_contents($temp_name, base64_decode(substr($original_message, $part_data['starting-pos-body'], $part_data['ending-pos-body'] - $part_data['starting-pos-body'])));
          }else{
            // Default to no decoding.
            file_put_contents($temp_name, substr($original_message, $part_data['starting-pos-body'], $part_data['ending-pos-body'] - $part_data['starting-pos-body']));
          }
          $msg['files'][] = array(
            'temp_name' => $temp_name,
            'mime' => $part_data['content-type'],
            'original_name' => $part_data['disposition-filename']
          );
        }else{
          // Not allowed to upload files of this type.
          watchdog('femail', 'Posting of files with extension "@extension" is not allowed.', array(
            '@extension' => $pathinfo['extension']
          ));
        }
      }
    }
    // Get the msgid
    if(isset($part_data['headers']['message-id'])){
      $msg['msgid'] = $part_data['headers']['message-id'];
    }
    // Get the InReplyTo header for possibly putting this message as a reply to 
    // another forum post
    if(isset($part_data['headers']['in-reply-to'])){
      $msg['in-reply-to'] = $part_data['headers']['in-reply-to'];
    }
  }
  if(!$body_is_set){
    // Looks like we have a none mime encoded message.  Here we need to loop
    // through each line, throwing them away until we reach a blank line, the
    // rest of the lines become the body.
    $lines = preg_split('/[\n\r]/', $msg['body']);
    while(trim(array_shift($lines))){
      ; // Nothing to do here, all done in the while statement.
    }
    $msg['body'] = implode("\n", $lines);
  }
  return $msg;
}

function _femail_post_comment($msg, $user, $status){
  // First we'll make sure the in-reply-to is in the msgid table, if not, we
  // will have to add this as a new thread.
  if(isset($msg['in-reply-to'])){
    //$result = db_query("SELECT cid, nid FROM {femail_msgs} WHERE msgid = '%s'", $msg['in-reply-to']);
    $row = db_select('femail_msgs', 'f')->fields('f', array(
      'cid',
      'nid'
    ))->condition('msgid', $msg['in-reply-to'])->execute()->fetch();
  }else{
    // We need to select the nid and 0 as cid from node where title is the same
    // as the subject line (less "re:")
    //$result = db_query("SELECT 0 AS cid, nid FROM node WHERE title = '%s' AND type = 'forum' LIMIT 1", trim(substr($msg['title'], 3)));
    $row = db_select('node', 'n')->fields('n', array(
      '0' => 'cid',
      'nid'
    ))->condition('title', trim(substr($msg['title'], 3)))->condition('type', 'forum')->execute()->fetch();
  }
  if($row){
    // Check we have permission to post a forum post.
    if(user_access('post comments')){
      // FFS, the comment_upload module doesn't accept files as objects. Ah well
      // we'll convert them here.
      $ffs_fussy_cu = array();
      foreach($msg['files'] as $fid => $file_tmp){
        $ffs_fussy_cu[$fid] = (array)$file_tmp;
      }
      $edit = array(
        'author' => $user->name,
        'subject' => $msg['title'],
        'comment' => $msg['body'],
        'pid' => $row['cid'],
        'nid' => $row['nid'],
        'uid' => $user->uid,
        'op' => 'Save',
        'date' => 'now',
        'name' => $user->name,
        'status' => $status,
        'format' => variable_get('femail_input_format', NULL),
        'files' => $ffs_fussy_cu
      );
      $cid = comment_save($edit);
      watchdog('femail', 'Posted forum comment: ' . l($msg['title'], 'node/' . $row['nid'], array(
        'fragment',
        'comment-' . $cid
      )));
    }else{
      watchdog('femail', 'Failed to post forum comment: ' . $msg['title']);
    }
    return TRUE;
  }
  watchdog('femail', 'Failed to create comment for "@subject" FROM "@email"', array(
    '@subject' => $msg['title'],
    '@email' => $msg['from']
  ), WATCHDOG_INFO);
  return FALSE;
}

function _femail_post_forum_topic($msg, $user, $status){
  // Get the tid of the forum that this post will go into
  $emails = variable_get('femail_emails', array());
  $tid = array_search($msg['to'], $emails);
  $term = taxonomy_term_load($tid);
  $node = new stdClass();
  $node->uid = $user->uid;
  $node->body = array(
    'und' => array(
      array(
        'value' => $msg['body']
      )
    )
  );
  $node->comment = variable_get('comment_forum', COMMENT_NODE_OPEN);
  $node->title = $msg['title'];
  $node->type = 'forum';
  $node->taxonomy = array(
    $term->vid => $tid
  );
  $node->taxonomy_forums = array(
    'und' => array(
      array(
        'tid' => $tid
      )
    )
  );
  $node->status = $status;
  $node->format = variable_get('femail_input_format', NULL);
  if(isset($msg['files'])){
    $node->files = $msg['files'];
  }
  node_save($node);
  watchdog('femail', 'Posted forum topic: ' . l($node->title, 'node/' . $node->nid), array(), WATCHDOG_INFO);
}

function femail_drush_callback(){
  // Femail must be installed, so lets make sure it is, else we exit.
  if(!module_exists('femail')){return;}
  $msg = _femail_get_msg();
  // Check to see this isn't a message that we've just sent out
  if(isset($msg['reply-to']) && $msg['reply-to'] == $msg['to']){return;}
  watchdog('femail', "Following message was received:\n\n" . print_r($msg, TRUE), array(), WATCHDOG_INFO);
  // User needed for both comments, and nodes, so load first.
  global $user;
  $account = user_load_by_mail($msg['from']);
  if($account){
    $user = $account;
  }else{
    $row = db_select('femail_user_emails', 'f')->fields('f', array(
      'uid'
    ))->condition('email', $msg['from'])->condition('status', '0', '!=')->execute()->fetch();
    //$row = db_fetch_array(db_query("SELECT uid FROM {femail_user_emails} WHERE email = '%s' AND status !=0", $msg['from']));
    if(is_array($row)){
      $user = user_load($row);
    }else{
      $user = drupal_anonymous_user();
    }
  }
  // Now that we have a UID, we can upload the files.  We'll check for the 
  // upload permissions too (we need both)
  if(isset($msg['files']) && count($msg['files'])){
    $temp_files = $msg['files'];
    unset($msg['files']);
    if(user_access('upload files') && user_access('upload files to comments')){
      foreach($temp_files as $tmpfile){
        $filename = $tmpfile['temp_name'];
        if(file_move($filename, drush_get_context('DRUSH_DRUPAL_ROOT') . '/' . file_directory_path() . '/' . $tmpfile['original_name'])){
          // The file is now saved, we should now add an entry to the files table.
          $pathinfo = pathinfo($filename);
          $file = new stdClass();
          $file->filename = $pathinfo['basename'];
          $file->filepath = file_directory_path() . '/' . $pathinfo['basename'];
          $file->filemime = $tmpfile['mime'];
          $file->source = 'femail';
          $file->filesize = filesize($filename);
          $file->uid = $user->uid;
          $file->status = 0;
          $file->new = 1;
          $file->list = 1;
          $file->description = $pathinfo['filename'];
          $file->weight = 0;
          $file->timestamp = time();
          drupal_write_record('files', $file);
          $msg['files'][$file->fid] = $file;
        }
      }
    }
  }
  if(!user_access('post by femail')){
    // Add a message to the logs so that we know we received a message and that
    // it hasn't been published
    watchdog('femail', 'Message "@subject" FROM "@email" has been posted, but unpublished', array(
      '@subject' => $msg['title'],
      '@email' => $msg['from']
    ), WATCHDOG_INFO);
    $comment_status = 1;
    $node_status = 0;
  }else{
    $comment_status = 0;
    $node_status = 1;
  }
  // If the in-reply-to is set, then we need to create a comment else we need to
  // create a node, or creating a comment has failed as the msgid could not be
  // found.
  if(isset($msg['in-reply-to']) || strtolower(substr($msg['title'], 0, 3)) == 're:'){
    if(_femail_post_comment($msg, $user, $comment_status)){return;}
  }
  // else we need to create a node, or creating a comment has failed as the 
  // msgid could not be found.
  _femail_post_forum_topic($msg, $user, $node_status);
}
