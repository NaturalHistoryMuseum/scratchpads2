<?php

/**
 * @file
 * Views handler to display links to a submission.
 */

/**
 * Field handler to present a link to the user.
 */
class webform_handler_field_submission_link extends views_handler_field {
  var $link_type;

  function construct() {
    // We need to set this property before calling the construct() chain
    // as we use it in the option_definintion() call.
    $this->link_type = $this->definition['link_type'];

    parent::construct();
    $this->additional_fields['sid'] = 'sid';
    $this->additional_fields['nid'] = 'nid';
    $this->additional_fields['serial'] = 'serial';
    $this->additional_fields['is_draft'] = 'is_draft';
  }

  function allow_advanced_render() {
    return FALSE;
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['label'] = array('default' => '', 'translatable' => TRUE);
    $options['text'] = array('default' => $this->link_type, 'translatable' => TRUE);
    $options['access_check'] = array('default' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
      '#description' => t('The token [serial] will be replaced with the serial number and draft indication.'),
    );
    $form['access_check'] = array(
      '#type' => 'checkbox',
      '#title' => t('Verify submission access for each link'),
      '#default_value' => $this->options['access_check'],
    );
  }

  function render($values) {
    $sid = $values->{$this->aliases['sid']};
    $nid = $values->{$this->aliases['nid']};
    $serial = $values->{$this->aliases['serial']};
    $is_draft = $values->{$this->aliases['is_draft']};

    $text = str_ireplace('[serial]',
                          $serial . ($is_draft ? ' (' . t('draft') . ')' : ''),
                          $this->options['text']);
    switch ($this->link_type) {
      case 'view':
        $text = !empty($text) ? $text : t('view');
        $link = l($text, "node/$nid/submission/$sid");
        break;
      case 'edit':
        $text = !empty($text) ? $text : t('edit');
        $link = l($text, "node/$nid/submission/$sid/edit");
        break;
      case 'delete':
        $text = !empty($text) ? $text : t('delete');
        $path = drupal_get_path_alias($_GET['q']);
        $link = l($text, "node/$nid/submission/$sid/delete", array('query' => array('destination' => $path)));
        break;
      default:
        return;
    }

    if ($this->options['access_check']) {
      module_load_include('inc', 'webform', 'includes/webform.submissions');
      $node = node_load($nid);
      $submission = webform_get_submission($nid, $sid);
      if (!webform_submission_access($node, $submission, $this->link_type)) {
        return;
      }
    }

    return $link;
  }
}
