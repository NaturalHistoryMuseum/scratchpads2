<?php

/**
 * @file
 * 
 * Contains the two functions used for sending out mail messages.
 */
function _femail_get_emails($tid, $account){
  // We'll let our SQL do the magic
  $results = db_query("SELECT DISTINCT CONCAT('<', LOWER(mail), '>') as mail FROM {users} u, {femail_user_subscriptions} f WHERE u.uid = f.uid AND (tid = %d OR tid = 0) AND u.uid != %d", $tid, $account->uid);
  $emails = array();
  while($row = db_fetch_array($results)){
    $emails[] = $row['mail'];
  }
  return $emails;
}

function _femail_send_message($tid, $subject, $body, $nid, $files = array(), $cid = 0, $inreplyto = FALSE){
  global $user;
  $emails = _femail_get_emails($tid, $user);
  $forum_term = taxonomy_get_term($tid);
  if(count($emails)){
    $from_emails = variable_get('femail_emails', array());
    $from = ($user->name ? '=?UTF-8?B?' . base64_encode($user->name) . '?= ' : '') . '<' . ($user->mail ? $user->mail : $from_emails[$tid]) . '>';
    global $base_url;
    $parts = parse_url($base_url);
    $msgid = '<' . md5(microtime()) . '@' . $parts['host'] . '>';
    $node = node_load(array(
      'nid' => $nid
    ));
    db_query("INSERT INTO {femail_msgs} (nid, cid, msgid) VALUES (%d, %d, '%s')", $nid, $cid, $msgid);  
    if(count($files)){
      $additional_text = "Files:\n";
      foreach($files as $file){
        $file = (array)$file;
        $additional_text .= url('', array(
          'absolute' => TRUE
        )) . drupal_urlencode($file['filepath']) . "\n";
      }
      $body = $additional_text . "\n" . $body;
    }
    $message = array(
      'id' => 'femail_message',
      'from' => $from,
      'subject' => $subject,
      'body' => drupal_wrap_mail(strip_tags($body)),
      'to' => $from_emails[$tid],
      'headers' => array(
        'Return-path' => $from_emails[$tid],
        'From' => $from,
        'Message-id' => $msgid,
        'Reply-to' => $from_emails[$tid],
        'List-id' => check_plain($forum_term->name) . ' <' . str_replace("@", ".", $from_emails[$tid]) . '>',
        'List-post' => '<' . $from_emails[$tid] . '>',
        'List-archive' => '<' . url('forum/' . $tid, array(
          'absolute' => TRUE
        )) . '>',
        'Thread-topic' => check_plain($node->title),
        'List-subscribe' => '<' . url('user/register', array(
          'absolute' => TRUE
        )) . '>',
        'Bcc' => implode(", ", $emails)
      )
    );
    if($inreplyto){
      $message['headers']['In-reply-to'] = $inreplyto;
    }
    drupal_mail_send($message);
  }
}