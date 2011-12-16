<?php /* -*- mode: php; indent-tabs-mode: nil; tab-width: 2; -*- */

/**
 * @file
 *   Document content_lock hooks.
 *
 * @ingroup content_lock_hooks
 * @ingroup hooks
 */

/**
 * @defgroup content_lock_hooks Content Lock Hooks
 *
 * Hooks which allow <a href="http://drupal.org/project/content_lock">content_lock</a>
 * to be extended.
 */

/**
 * Determine if locking should be disabled for a given node (e.g. by
 * checking its type or other properties).
 *
 * This hook is called from content_lock_form_alter() before it
 * determines that it is altering a node modification form. Thus, some
 * of this hook's paramesters are the same parameters that would be
 * passed to hook_form_alter().
 *
 * An implementation of this hook can be used to make the ability to
 * lock a node conditional on an arbitrary aspect of the node.
 * @param $node
 *   The node for which a lock might be created. This parameter may be
 *   NULL in the case that the form is for something other than a
 *   node.
 * @param $form_id
 *   The form_id for the node's edit form.
 * @param $form
 *   The form for the node's edit form.
 * @param $form_state
 *   The form_state for the node's edit form.
 * @return
 *   FALSE to indicate that locking is allowed or TRUE to prevent this
 *   node from being locked.
 *
 * @ingroup content_lock_hooks
 * @ingroup hooks
 */
function hook_content_lock_skip_locking($node, $form_id, $form, $form_state) {
  /* Avoid creating warning when $node is NULL */
  if (empty($node)) {
    return FALSE;
  }

  /* 
   * Prevent locking from happening on unpublished nodes since few
   * people can access such nodes anyway.
   */
  if (!empty($node->status)) {
    return TRUE;
  }

  /* By default allo locking. */
  return FALSE;
}

/**
 * Alter the blacklist of form_ids.
 *
 * Locking nodes referenced from certain form_ids, such as comment
 * forms and the like, can supposedly be enabled or disabled here.
 *
 * @param $blacklist
 *   An array of blacklisted form_ids. Set $blacklist[<form_id>] =
 *   TRUE to blacklist <form_id> or unset($blacklist[<form_id>]) to
 *   unblacklist a form. Note that locking is not likely to work for
 *   every type of node form.
 * @param $node
 *   If available, the node being currently checked shalled be passed
 *   in. This may be useful for form_ids based on the nid of a node
 *   which certain modules might do with the help of hook_forms().
 *
 * @ingroup content_lock_hooks
 * @ingroup hooks
 */
function hook_content_lock_form_id_blacklist_alter(&$blacklist, $node = NULL) {
  /*
   * Disable locking an arbitrary form which happens to set
   * $form['nid'] and $form['#node'] to point to a node.
   */
  $blacklist['arbitrary_form'] = TRUE;
}

/**
 * Alter the node type blacklist.
 *
 * Use this hook to disable locking for particular node types.
 *
 * @param $blacklist
 *   An array with keys being node types (such as 'page') and the
 *   values being TRUE if that node type is for a node which shouldn't
 *   ever be locked.
 * @param $node
 *   The node currently being tested for locking eligibility. This
 *   enables the hook to directly test the node's type for eligibility
 *   (and ban it by adding the type to the $blacklist).
 *
 * @ingroup content_lock_hooks
 * @ingroup hooks
 */
function hook_content_lock_node_type_blacklist_alter(&$blacklist, $node) {
  /*
   * Don't lock a custom node type which is only ever editable by its
   * author.
   */
  $blacklist['custom_oneuser_nodetype'] = TRUE;
}

/**
 * Respond to a lock being successfully set.
 *
 * This hook is called from content_lock_node() only after a lock was
 * successfully set on a particular node by a user.
 *
 * @param $nid
 *   The nid of the node which was successfully locked.
 * @param $uid
 *   The uid of the user who initiated the locking.
 *
 * @ingroup content_lock_hooks
 * @ingroup hooks
 */
function hook_content_lock_locked($nid, $uid) {
  /*
   * At the moment, I can't think of what sort of thing one would want
   * to do here. Please file an issue with ideas :-).
   */
}

/**
 * Respond to a node's lock being released.
 *
 * This hook is called from content_lock_release() for every lock
 * which is released. This hook might get called when there wasn't a
 * lock on a node to begin with, but its being called always means
 * that an attempt was made to unlock the given node.
 *
 * @param $nid
 *   The node whose lock was released.
 * @param $uid
 *   The uid of the user who initiated the lock's release or NULL if
 *   the lock release was automated (such as by the
 *   content_lock_timeouts module).
 *
 * @ingroup content_lock_hooks
 * @ingroup hooks
 */
function hook_content_lock_release($nid, $uid = NULL) {
  /*
   * See the body of hook_content_lock_locked() ;-).
   */
}

/**
 * Determine whether or not a node is lockable.
 *
 * Called from _content_lock_is_lockable_node() which is in turn
 * called from any code which is conditional upon a node being
 * lockable or not. If this hook returns an affirmative and allows a
 * node to be locked at one point but later on returns a negative on
 * the same node, any existing locks for the node will be ignored. So
 * this hook can control whether or not content_lock is completely
 * disabled for a node (such that even recorded locks for a node can
 * be ignored with this hook).
 *
 * @param $node
 *   The node whose lockability should be checked.
 * @return
 *   TRUE if the node should be considered lockable (this should be
 *   the default return value) or FALSE if the node may not be
 *   considered lockable.
 *
 * @ingroup content_lock_hooks
 * @ingroup hooks
 */
function hook_content_lock_node_lockable($node) {
  /* Don't bother the superuser with locking */
  if (!$node->status && $node->uid == 1) {
    return FALSE;
  }

  /* By default, let nodes be lockable */
  return TRUE;
}
