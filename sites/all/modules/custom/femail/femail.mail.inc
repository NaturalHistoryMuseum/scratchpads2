<?php

/**
 * @file
 * 
 * Contains the two functions used for sending out mail messages.
 */
function _femail_get_emails($tid, $account, $nid){
  $node = node_load($nid);
  $query = db_select('users', 'u');
  $query->join('femail_user_subscriptions', 'f', 'u.uid = f.uid');
  $query->where('(tid = :tid OR tid = 0)', array(':tid' => $tid));
  $query->condition('u.uid', $account->uid, '!=');
  $query->fields('u', array('uid'));
  $query->distinct(TRUE);
  $results = $query->execute();
  $uids = array();
  foreach($results as $result){
    $uids[] = $result->uid;
  }
  $users = user_load_multiple($uids);
  $emails = array();
  foreach($users as $account){
    if(node_access('view', $node, $account)){
      $emails[] = "<{$account->mail}>";
    }
  }
  return $emails;
}

function _femail_send_message($tid, $subject, $body, $nid, $files = array(), $cid = 0, $inreplyto = FALSE){
  global $user;
  $emails = _femail_get_emails($tid, $user, $nid);
  $forum_term = taxonomy_term_load($tid);
  if(count($emails)){
    $from_emails = variable_get('femail_emails', array());
    $from = ($user->name ? '=?UTF-8?B?' . base64_encode($user->name) . '?= ' : '') . '<' . ($user->mail ? $user->mail : $from_emails[$tid]) . '>';
    global $base_url;
    $parts = parse_url($base_url);
    $msgid = '<' . md5(microtime()) . '@' . $parts['host'] . '>';
    $node = node_load($nid);
    db_insert('femail_msgs')->fields(array(
      'nid',
      'cid',
      'msgid'
    ), array(
      $nid,
      $cid,
      $msgid
    ))->execute();
    if(count($files)){
      $additional_text = "Files:\n";
      foreach($files as $file){
        $file = (array)$file;
        $additional_text .= file_create_url($file['uri']) . "\n";
      }
      $body = $additional_text . "\n" . $body;
    }
    $message = array(
      'id' => 'femail_message',
      'from' => $from,
      'subject' => $subject,
      // How fucking ridiculous is this that we can no longer send a string as 
      // the body for a mail message!
      'body' => drupal_wrap_mail(strip_tags($body)),
      'to' => $from_emails[$tid],
      'headers' => array(
        'Return-path' => $from_emails[$tid],
        'From' => $from,
        'Message-id' => $msgid,
        'Reply-to' => $from_emails[$tid],
        'List-id' => check_plain($forum_term->name) . ' <' . str_replace("@", ".", $from_emails[$tid]) . '>',
        'List-post' => '<mailto:' . $from_emails[$tid] . '>',
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
    $system = drupal_mail_system('femail', $msgid);
    $system->mail($message);
  }
}