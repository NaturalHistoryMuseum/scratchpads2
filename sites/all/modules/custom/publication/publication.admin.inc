<?php

/**
 * UI controller.
 */
class PublicationUIController extends EntityDefaultUIController{

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu(){
    $items = array();
    $items[$this->path] = array(
      'title' => 'Publications',
      'description' => 'Add, edit and update publications.',
      'page callback' => 'drupal_get_form',
      'page arguments' => array(
        'publication_admin_overview_form'
      ),
      'access arguments' => array(
        'administer publications'
      ),
      'file' => 'publication.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
      'type' => MENU_LOCAL_TASK
    );
    // Change the add page menu to multiple types of entities
    $items['publication/add'] = array(
      'title' => 'Add a publication',
      'description' => 'Add a new publication',
      'page callback' => 'publication_add_page',
      'access callback' => 'publication_access',
      'access arguments' => array(
        'edit'
      ),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'publication.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    // Add menu items to add each different type of entity.
    foreach(publication_get_types() as $type){
      $items['publication/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'publication_form_wrapper',
        'page arguments' => array(
          'publication_edit_form',
          publication_create(array(
            'type' => $type->type
          ))
        ),
        'access callback' => 'publication_access',
        'access arguments' => array(
          'edit',
          $type->type
        ),
        'file' => 'publication.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }
    $items['publication/%publication'] = array(
      'title callback' => 'publication_page_title',
      'title arguments' => array(
        1
      ),
      'page callback' => 'publication_page_view',
      'page arguments' => array(
        1
      ),
      'access callback' => 'publication_access',
      'access arguments' => array(
        'view',
        1
      )
    );
    $items['publication/%publication/view'] = array(
      'title' => 'View',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10
    );
    $items['publication/%publication/edit'] = array(
      'title' => 'Edit',
      'page callback' => 'publication_form_wrapper',
      'page arguments' => array(
        'publication_edit_form',
        1
      ),
      'access callback' => 'publication_access',
      'access arguments' => array(
        'edit',
        1
      ),
      'weight' => 0,
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'publication.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items['publication/%publication/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'publication_form_wrapper',
      'page arguments' => array(
        'publication_delete_form',
        1
      ),
      'access callback' => 'publication_access',
      'access arguments' => array(
        'edit',
        1
      ),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'publication.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    return $items;
  }
}

/**
 * Menu callback: publication content administration.
 */
function publication_admin_overview_form(){
  $form = array();
  // Build the sortable table header.
  $header = array(
    'title' => array(
      'data' => t('Title'),
      'field' => 'p.title'
    ),
    'type' => array(
      'data' => t('Type'),
      'field' => 'p.type'
    ),
    'author' => t('Author'),
    'changed' => array(
      'data' => t('Updated'),
      'field' => 'p.changed',
      'sort' => 'desc'
    )
  );
  $header['operations'] = array(
    'data' => t('Operations')
  );
  $query = db_select('publication', 'p')->extend('PagerDefault')->extend('TableSort');
  $pids = $query->fields('p', array(
    'pid'
  ))->limit(50)->orderByHeader($header)->execute()->fetchCol();
  $publications = publication_load_multiple($pids);
  $rows = array();
  if(count($publications)){
    $destination = drupal_get_destination();
    foreach($publications as $publication){
      $rows[$publication->pid] = array(
        'title' => array(
          'data' => array(
            '#type' => 'link',
            '#title' => $publication->title,
            '#href' => 'publication/' . $publication->pid,
            '#suffix' => ' ' . theme('mark', array(
              'type' => node_mark($publication->pid, $publication->changed)
            ))
          )
        ),
        'type' => check_plain($publication->type),
        'author' => theme('username', array(
          'account' => user_load($publication->uid)
        )),
        'changed' => format_date($publication->changed, 'short')
      );
      // Build a list of all the accessible operations for the current publication.
      $operations = array();
      if(publication_access('update', $publication)){
        $operations['edit'] = array(
          'title' => t('Edit'),
          'href' => 'publication/' . $publication->pid . '/edit',
          'query' => $destination
        );
      }
      if(publication_access('delete', $publication)){
        $operations['delete'] = array(
          'title' => t('Delete'),
          'href' => 'publication/' . $publication->pid . '/delete',
          'query' => $destination
        );
      }
      $rows[$publication->pid]['operations'] = array();
      if(count($operations) > 1){
        // Render an unordered list of operations links.
        $rows[$publication->pid]['operations'] = array(
          'data' => array(
            '#theme' => 'links__node_operations',
            '#links' => $operations,
            '#attributes' => array(
              'class' => array(
                'links',
                'inline'
              )
            )
          )
        );
      }elseif(!empty($operations)){
        // Render the first and only operation as a link.
        $link = reset($operations);
        $rows[$publication->pid]['operations'] = array(
          'data' => array(
            '#type' => 'link',
            '#title' => $link['title'],
            '#href' => $link['href'],
            '#options' => array(
              'query' => $link['query']
            )
          )
        );
      }
    } // end of foreach publications
  }
  $form['publications'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No content available.')
  );
  $form['pager'] = array(
    '#markup' => theme('pager')
  );
  return $form;
}

/**
 * Menu callback: list of publication types that can be added
 */
function publication_add_page(){
  $item = menu_get_item();
  $content = system_admin_menu_block($item);
  return theme('publication_add_list', array(
    'content' => $content
  ));
}

/**
 * Form callback wrapper: create or edit a publication.
 *
 * @param $publication
 * The publication object being edited by this form.
 *
 * @see publication_edit_form()
 */
function publication_form_wrapper($form_id, $publication = null){
  return drupal_get_form($form_id, $publication);
}

/**
 * Form callback: create or edit a publication.
 *
 * @param $publication
 * The publication object to edit or for a create form an empty publication object
 * with only a publication type defined.
 */
function publication_edit_form($form, &$form_state, $publication){
  // Jump to a particular step
  if(isset($_GET['step']) & !$form_state['submitted']){
    $form_state['current_group'] = $_GET['step'];
  }
  // Add the default field elements.
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Short title'),
    '#default_value' => isset($publication->title) ? $publication->title : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5
  );
  $form['data']['#tree'] = TRUE;
  // Add the field related form elements.
  $form_state['publication'] = $publication;
  field_attach_form('publication', $publication, $form, $form_state);
  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'form-actions'
      )
    ),
    '#weight' => 400
  );
  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();
  if(!empty($form['#submit'])){
    $submit += $form['#submit'];
  }
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => $submit + array(
      'publication_edit_form_submit'
    )
  );
  if(!empty($publication->pid)){
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => $submit + array(
        'publication_form_submit_delete'
      ),
      '#weight' => 45
    );
  }
  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'publication_edit_form_validate';
  return $form;
}

/**
 * Button submit function: handle the 'Delete' button on the node form.
 */
function publication_form_submit_delete($form, &$form_state){
  $destination = array();
  if(isset($_GET['destination'])){
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $publication = $form_state['publication'];
  $form_state['redirect'] = array(
    'publication/' . $publication->pid . '/delete',
    array(
      'query' => $destination
    )
  );
}

/**
 * Form API validate callback for the publication form
 */
function publication_edit_form_validate(&$form, &$form_state){
  $publication = $form_state['publication'];
  // Notify field widgets to validate their data.
  field_attach_form_validate('publication', $publication, $form, $form_state);
}

/**
 * Form API submit callback for the publication form.
 */
function publication_edit_form_submit(&$form, &$form_state){
  global $user;
  $publication = entity_ui_controller('publication')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the publication and go back to the list of publications
  // Add in created and changed times.
  $publication->is_new = isset($publication->is_new);
  if($publication->is_new){
    $publication->created = time();
    $publication->uid = $user->uid;
  }
  $publication->changed = time();
  $publication->save();
  $form_state['pid'] = $publication->pid;
  $form_state['redirect'] = 'publication/' . $publication->pid;
}

/**
 * Form callback: confirmation form for deleting a publication.
 *
 * @param $publication
 * The publication to delete
 *
 * @see confirm_form()
 */
function publication_delete_form($form, &$form_state, $publication){
  $form_state['publication'] = $publication;
  $form['#submit'][] = 'publication_delete_form_submit';
  $form = confirm_form($form, t('Are you sure you want to delete publication %title?', array(
    '%title' => $publication->title
  )), 'admin/content/publications/publication', '<p>' . t('This action cannot be undone.') . '</p>', t('Delete'), t('Cancel'), 'confirm');
  return $form;
}

/**
 * Submit callback for publication_delete_form
 */
function publication_delete_form_submit($form, &$form_state){
  $publication = $form_state['publication'];
  publication_delete($publication);
  drupal_set_message(t('The publication %title has been deleted.', array(
    '%title' => $publication->title
  )));
  watchdog('Publication', 'Deleted publication %title.', array(
    '%title' => $publication->title
  ));
  $form_state['redirect'] = 'admin/content/publications';
}

