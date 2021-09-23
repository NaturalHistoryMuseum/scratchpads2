<?php
/**
 * @file
 * Class that extends the regular ctools export ui.
 */

/**
 * Base class for export UI.
 */
class linkit_profiles_ui extends ctools_export_ui {

  public function list_table_header() {
    $header = array();
    $header[] = array('data' => t('Title'), 'class' => array('ctools-export-ui-title'));
    $header[] = array('data' => t('Name'), 'class' => array('ctools-export-ui-name'));
    $header[] = array('data' => t('Storage'), 'class' => array('ctools-export-ui-storage'));
    $header[] = array('data' => t('Roles'), 'class' => array('ctools-export-ui-roles'));
    $header[] = array('data' => t('Weight'), 'class' => array('ctools-export-ui-weight'));
    $header[] = array('data' => t('Operations'), 'class' => array('ctools-export-ui-operations'));
    return $header;
  }

  public function list_build_row($item, &$form_state, $operations) {
    // Set up sorting
    $name = $item->name;
    $schema = ctools_export_get_schema($this->plugin['schema']);

    switch ($form_state['values']['order']) {
      case 'disabled':
        $this->sorts[$name] = $item->weight;
        break;
      case 'disabled_title':
        $this->sorts[$name] = empty($item->disabled) . $item->admin_title;
        break;
      case 'title':
        $this->sorts[$name] = $item->admin_title;
        break;
      case 'name':
        $this->sorts[$name] = $name;
        break;
      case 'storage':
        $this->sorts[$name] = $item->{$schema['export']['export type string']} . $name;
        break;
    }

    $ops = theme('links__ctools_dropbutton', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline'))));
    $role_list = $this->buildRoleList($item->role_rids);

    $this->rows[$name] = array(
      'data' => array(
        array('data' => check_plain($item->admin_title), 'class' => array('ctools-export-ui-title')),
        array('data' => check_plain($name), 'class' => array('ctools-export-ui-name')),
        array('data' => check_plain($item->{$schema['export']['export type string']}), 'class' => array('ctools-export-ui-storage')),
        array('data' => check_plain($role_list), 'class' => array('ctools-export-ui-roles')),
        array('data' => check_plain($item->weight), 'class' => array('ctools-export-ui-weight')),
        array('data' => $ops, 'class' => array('ctools-export-ui-operations')),
      ),
      'class' => array(!empty($item->disabled) ? 'ctools-export-ui-disabled' : 'ctools-export-ui-enabled'),
    );
  }

  /**
   * Build a list of all roles that is asssigned to a profile.
   */
  public static function buildRoleList($rids) {
    $list = array();
    foreach ($rids as $rid => $value) {
      $role = user_role_load($rid);
      $list[] = $role->name;
    }
    return implode(', ', $list);
  }

  public function list_sort_options() {
    $options = array(
      'disabled' => t('Weight'),
      'disabled_title' => t('Enabled, title'),
      'admin_title' => t('Title'),
      'name' => t('Name'),
      'storage' => t('Storage'),
    );
    return $options;
  }
}





