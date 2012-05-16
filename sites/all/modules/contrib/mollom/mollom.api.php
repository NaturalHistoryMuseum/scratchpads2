<?php

/**
 * @file
 * API documentation for Mollom module.
 */

/**
 * @defgroup mollom_api Mollom API
 * @{
 * Functions to integrate with Mollom form protection.
 *
 * In general, there are two different kinds of form submissions:
 * - Entities created out of form submissions, which can be edited or deleted
 *   afterwards; whereas "entity" just refers to a uniquely identifiable data
 *   record.
 * - Form submissions that do not store any data, such as contact form mail
 *   messages and similar forms. While there may be an entity type (e.g.,
 *   "contact_mail"), there is no unique id for the post, which could be
 *   referred to later on.
 *
 * The Mollom API therefore supports two types of integration:
 * - Entity form integration: Mollom integrates with the add/edit form for an
 *   entity, and additionally with the delete confirmation form of the entity
 *   to send feedback to Mollom. Almost everything happens in an automated way,
 *   solely based on the information provided via Mollom's info hooks, as
 *   explained below.
 * - Free integration: Mollom integrates with a given form_id without 'entity'.
 *   Allowing users to send feedback requires to manually add "report to Mollom"
 *   links. Additionally requires to specify a 'report access [callback]' and
 *   'report delete callback' to correctly handle access to report and delete
 *   a posted piece of content. An example for this kind of integration can be
 *   found in contact_mollom_form_list(), mollom_mail_alter(), and related
 *   functions. This kind of integration is discouraged; it is recommended to
 *   implement and integrate with entity forms.
 *
 * Considering a very simple Instant Messaging module ("IM") that implements a
 * "im_message_form" allowing to send or edit an instant message, which should
 * be possible to be protected by Mollom:
 * @code
 * function im_message_form(&$form_state, $im) {
 *   // To allow other modules to extend this form more easily and simplify our
 *   // own form submission handling, we use the dedicated parent key "im" for
 *   // all message properties (allows for easy casting from array to object).
 *   // Also helps us to explain handling of hierarchical sub-keys. :)
 *   $form['#tree'] = TRUE;
 *
 *   // This is the stored message id (or 'post_id'), if any:
 *   // @see im_message_form_submit()
 *   $form['im']['id'] = array(
 *     '#type' => 'value',
 *     '#value' => isset($im->id) ? $im->id : NULL,
 *   );
 *   $form['im']['subject'] = array(
 *     '#type' => 'textfield',
 *     '#title' => t('Subject'),
 *     '#default_value' => isset($im->subject) ? $im->subject : '',
 *   );
 *   $form['im']['body'] = array(
 *     '#type' => 'textfield',
 *     '#title' => t('Message'),
 *     '#default_value' => isset($im->body) ? $im->body : '',
 *   );
 *   $form['actions']['submit'] = array(
 *     '#type' => 'submit',
 *     '#value' => t('Send'),
 *   );
 *   return $form;
 * }
 * @endcode
 *
 * "entity" refers to an entity type. For example, "node", "user", "comment",
 * but also "webform_submission". It is not necessarily the name of a database
 * table, but most often it actually is. The string is only used internally to
 * identify to which module a form submission belongs. Once in use, it should
 * not be changed.
 *
 * Our form accepts an argument $im, which we assume is the entity being created
 * or edited, so we can also assume the following submit handler:
 * @code
 * function im_message_form_submit($form, &$form_state) {
 *   // Do whatever we need to do to insert or update the message.
 *   $im = (object) $form_state['values']['im'];
 *   im_save($im);
 *   // Ensure subsequent submit handlers have an entity id to work with, as
 *   // newly created messages will not have an id in the form values.
 *   $form_state['values']['im']['id'] = $im->id;
 * }
 * @endcode
 *
 * The form values will not contain an entity id for a newly created message,
 * which is usually an auto_increment column value returned from the database.
 * Whenever a form submission is related to the entity (e.g., leads to a stored
 * entity being created, updated, or deleted) the form should *always* contain
 * the entity id in the same location of the submitted form values.
 * Above example therefore purposively assigns the new id after inserting it.
 *
 * @code
 * function im_message_delete_confirm_form(&$form_state, $im) {
 *   $form['#im'] = $im;
 *
 *   // Always provide entity id in the same form key as in the entity edit form.
 *   $form['im']['id'] = array('#type' => 'value', '#value' => $im->id);
 *
 *   // In our case, we also need to enable #tree, so that above value ends up
 *   // in 'im][id' where we expect it.
 *   $form['#tree'] = TRUE;
 *
 *   return confirm_form($form,
 *     t('Are you sure you want to delete %title?', array('%title' => $im->subject)),
 *     'im/' . $im->id,
 *     NULL,
 *     t('Delete')
 *   );
 * }
 * @endcode
 *
 * The same applies to the delete confirmation form for the entity: it also
 * provides the entity id for form submit handlers.
 *
 * After ensuring these basics, the first step is to register the basic form_id
 * along with its title, entity type, as well as the form_id of the
 * corresponding delete confirmation form via hook_mollom_form_list():
 *
 * @code
 * function im_mollom_form_list() {
 *   $forms['im_message_form'] = array(
 *     'title' => t('Instant messaging form'),
 *     'entity' => 'im',
 *     // Specify the $form_id of the delete confirmation form that allows
 *     // privileged users to delete a stored message. Mollom will automatically
 *     // add form elements to send feedback to Mollom to this form.
 *     'delete form' => 'im_message_delete_confirm_form',
 *   );
 *   return $forms;
 * }
 * @endcode
 *
 * Since modules can provide many forms, only minimal information is returned
 * via hook_mollom_form_list(). All details about the form are only required and
 * asked for, if the site administrator actually enables Mollom's protection for
 * the form. Therefore, everything else is registered via
 * hook_mollom_form_info():
 *
 * @code
 * function im_mollom_form_info($form_id) {
 *   switch ($form_id) {
 *     case 'im_message_form':
 *       $form_info = array(
 *         // Optional: User permission list to skip Mollom's protection for.
 *         'bypass access' => array('administer instant messages'),
 *         // Optional: Function to invoke to put a bad form submission into a
 *         // moderation queue instead of discarding it.
 *         'moderation callback' => 'im_mollom_form_moderation',
 *         // Optional: To allow textual analysis of the form values, the form
 *         // elements needs to be registered individually. The keys are the
 *         // field keys in $form_state['values']. Sub-keys are noted using "]["
 *         // as delimiter.
 *         'elements' => array(
 *           'im][subject' => t('Subject'),
 *           'im][body' => t('Message body'),
 *         ),
 *         // Required when either specifying 'entity' or 'elements': the keys
 *         // are predefined data properties sent to Mollom (see full list in
 *         // hook_mollom_form_info()), the values refer to field keys in
 *         // $form_state['values']. Sub-keys are noted using "][" as delimiter.
 *         'mapping' => array(
 *           // Required when specifying 'entity' above: Where to find the id of
 *           // the entity being posted, edited, or deleted.
 *           // Important: The following assignment means that Mollom is able to
 *           // find the message id of the created, edited, or deleted message
 *           // in $form_state['values']['im']['id'].
 *           'post_id' => 'im][id',
 *           // Required if the form or entity contains a title-alike field:
 *           'post_title' => 'im][subject',
 *           // Optional: If our instant message form was accessible for
 *           // anonymous users and would contain form elements to enter the
 *           // sender's name, e-mail address, and web site, then those fields
 *           // should be additionally specified. Otherwise, information from
 *           // the global user session would be automatically taken over.
 *           'author_name' => 'im][sender][name',
 *           'author_mail' => 'im][sender][mail',
 *           'author_url' => 'im][sender][homepage',
 *         ),
 *       );
 *       break;
 *   }
 *   return $form_info;
 * }
 * @endcode
 *
 * "elements" is a list of form elements, in which users can freely type text.
 * The elements should not contain numeric or otherwise predefined option
 * values, only text actually coming from user input. Only by registering
 * "elements", Mollom is able to perform textual analysis. Without registered
 * form elements, Mollom can only provide a CAPTCHA.
 *
 * "mapping" is a mapping of form elements to predefined XML-RPC data properties
 * of the Mollom web service. For example, "post_title", "author_name",
 * "author_id", "author_mail", etc. Normally, all form elements specified in
 * "elements" would be merged into the "post_body" data property. By specifying
 * a "mapping", certain form element values are sent for the specified data
 * property instead. In our case, the form submission contains something along
 * the lines of a title in the "subject" field, so we map the "post_title" data
 * property to the "subject" field.
 *
 * Additionally, the "post_id" data property always needs to be mapped to a form
 * element that holds the entity id.
 *
 * When registering a 'moderation callback', then the registered function needs
 * to be available when the form is validated, and it is responsible for
 * changing the submitted form values in a way that results in an unpublished
 * post ending up in a moderation queue:
 * @code
 * function im_mollom_form_moderation(&$form, &$form_state) {
 *   $form_state['values']['status'] = 0;
 * }
 * @endcode
 *
 * @see mollom_node
 * @see mollom_comment
 * @see mollom_user
 * @see mollom_contact
 */

/**
 * Return information about forms that can be protected by Mollom.
 *
 * Mollom invokes this hook for all modules to gather information about forms
 * that can be protected. Only forms that have been registered via this hook are
 * configurable in Mollom's administration interface.
 *
 * @return
 *   An associative array containing information about the forms that can be
 *   protected, keyed by $form_id:
 *   - title: The human-readable name of the form.
 *   - entity: (optional) The internal name of the entity type the form is for,
 *     e.g. 'node' or 'comment'. This is required for all forms that will store
 *     the submitted content persistently. It is only optional for forms that do
 *     not permanently store the submitted form values, such as contact forms
 *     that only send an e-mail, but do not store it in the database.
 *     Note that forms that specify 'entity' also need to specify 'post_id' in
 *     the 'mapping' (see below).
 *   - report access callback: (optional) A function name to invoke to check
 *     access to Mollom's dedicated "report to Mollom" form, which should return
 *     either TRUE or FALSE (similar to menu access callbacks).
 *   - report access: (optional) A list containing user permission strings, from
 *     which the current user needs to have at least one. Should only be used if
 *     no "report access callback" was defined.
 *   - report delete callback: (optional) A function name to invoke to delete an
 *     entity after reporting it to Mollom.
 *
 * @see hook_mollom_form_info()
 */
function hook_mollom_form_list() {
  // Mymodule's comment form.
  $forms['mymodule_comment_form'] = array(
    'title' => t('Comment form'),
    'entity' => 'mymodule_comment',
    'report access callback' => 'mymodule_comment_report_access',
    'report delete callback' => 'mymodule_comment_report_delete',
  );
  // Mymodule's user registration form.
  $forms['mymodule_user_register'] = array(
    'title' => t('User registration form'),
    'entity' => 'user',
    'report access' => array('administer comments', 'bypass node access'),
    // Make it private, so it's not a hook_user_delete() implementation.
    'report delete callback' => '_mymodule_user_delete',
  );
  return $forms;
}

/**
 * Alter the list of forms that can be protected by Mollom.
 *
 * @param &$form_list
 *   An associative array containing information about the forms that can be
 *   protected, keyed by $form_id. See hook_mollom_form_list() for details.
 */
function hook_mollom_form_list_alter(&$form_list) {
  if (isset($form_list['mymodule_user_register'])) {
    $form_list['mymodule_user_register']['report delete callback'] = '_mymodule_user_register_delete';
  }
}

/**
 * Return information about a form that can be protected by Mollom.
 *
 * @param $form_id
 *   The form id to return information for.
 *
 * @return
 *   An associative array describing the form identified by $form_id:
 *   - mode: (optional) The default protection mode for the form, which can be
 *     one of:
 *     - MOLLOM_MODE_ANALYSIS: Text analysis of submitted form values with
 *       fallback to CAPTCHA.
 *     - MOLLOM_MODE_CAPTCHA: CAPTCHA-only protection.
 *   - bypass access: (optional) A list of user permissions to check for the
 *     current user to determine whether to protect the form with Mollom or do
 *     not validate submitted form values. If the current user has at least one
 *     of the listed permissions, the form will not be protected.
 *   - moderation callback: (optional) A function name to invoke when a form
 *     submission would normally be discarded. This allows modules to put such
 *     posts into a moderation queue (i.e., to accept but not publish them) by
 *     altering the $form or $form_state that are passed by reference.
 *   - mail ids: (optional) An array of mail IDs that will be sent as a result
 *     of this form being submitted. When these mails are sent, a 'report to
 *     Mollom' link will be included at the bottom of the mail body. Be sure to
 *     include only user-submitted mails and not any mails sent by Drupal since
 *     they should never be reported as spam.
 *   - elements: (optional) An associative array of elements in the form that
 *     can be configured for Mollom's text analysis. The site administrator can
 *     only select the form elements to process (and exclude certain elements)
 *     when a form registers elements. Each key is a form API element #parents
 *     string representation of the location of an element in the form. For
 *     example, a key of "myelement" denotes a form element value on the
 *     top-level of submitted form values. For nested elements, a key of
 *     "parent][child" denotes that the value of 'child' is found below 'parent'
 *     in the submitted form values. Each value contains the form element label.
 *     If omitted, Mollom can only provide a CAPTCHA protection for the form.
 *   - mapping: (optional) An associative array to explicitly map form elements
 *     (that have been specified in 'elements') to the data structure that is
 *     sent to Mollom for validation. The submitted form values of all mapped
 *     elements are not used for the post's body, so Mollom can validate certain
 *     values individually (such as the author's e-mail address). None of the
 *     mappings are required, but most implementations most likely want to at
 *     least denote the form element that contains the title of a post.
 *     The following mappings are possible:
 *     - post_id: The form element value that denotes the ID of the content
 *       stored in the database.
 *     - post_title: The form element value that should be used as title.
 *     - post_body: Mollom automatically assigns this property based on all
 *       elements that have been selected for textual analysis in Mollom's
 *       administrative form configuration.
 *     - author_name: The form element value that should be used as author name.
 *     - author_mail: The form element value that should be used as the author's
 *       e-mail address.
 *     - author_url: The form element value that should be used as the author's
 *       homepage.
 *     - author_id: The form element value that should be used as the author's
 *       user uid.
 *     - author_openid: Mollom automatically assigns this property based on
 *       'author_id', if no explicit form element value mapping was specified.
 *     - author_ip: Mollom automatically assigns the user's IP address if no
 *       explicit form element value mapping was specified.
 */
function hook_mollom_form_info($form_id) {
  switch ($form_id) {
    // Mymodule's comment form.
    case 'mymodule_comment_form':
      $form_info = array(
        'mode' => MOLLOM_MODE_ANALYSIS,
        'bypass access' => array('administer comments'),
        'mail ids' => array('mymodule_comment_mail'),
        'elements' => array(
          'subject' => t('Subject'),
          'body' => t('Body'),
        ),
        'mapping' => array(
          'post_id' => 'cid',
          'post_title' => 'subject',
          'author_name' => 'name',
          'author_mail' => 'mail',
          'author_url' => 'homepage',
        ),
      );
      return $form_info;

    // Mymodule's user registration form.
    case 'mymodule_user_register':
      $form_info = array(
        'mode' => MOLLOM_MODE_CAPTCHA,
        'mapping' => array(
          'post_id' => 'uid',
          'author_name' => 'name',
          'author_mail' => 'mail',
        ),
      );
      return $form_info;
  }
}

/**
 * Alter registered information about a form that can be protected by Mollom.
 *
 * @param &$form_info
 *   An associative array describing the protectable form. See
 *   hook_mollom_form_info() for details.
 * @param $form_id
 *   The $form_id of the form.
 */
function hook_mollom_form_info_alter(&$form_info, $form_id) {
  if ($form_id == 'comment_form') {
    $form_info['elements']['mymodule_field'] = t('My additional field');
  }
}

/**
 * @} End of "defgroup module_group".
 */
