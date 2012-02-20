<?php

/**
 * UI controller.
 */
class OboeUIController extends EntityDefaultUIController{

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu(){
    $items = parent::hook_menu();
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    $items[$this->path]['description'] = t('Manage OBOE Jobs.');
    return $items;
  }

  /**
   * Generates the row for the passed entity and may be overridden in order to
   * customize the rows.
   *
   * @param $additional_cols
   * Additional columns to be added after the entity label column.
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()){
    dpm($entity);
    dpm($id);
    $entity_uri = entity_uri($this->entityType, $entity);
    $row[] = array(
      'data' => array(
        '#theme' => 'entity_ui_overview_item',
        '#label' => entity_label($this->entityType, $entity),
        '#name' => !empty($this->entityInfo['exportable']) ? entity_id($this->entityType, $entity) : FALSE,
        '#url' => $entity_uri ? $entity_uri : FALSE,
        '#entity_type' => $this->entityType
      )
    );
    // Add in any passed additional cols.
    foreach($additional_cols as $col){
      $row[] = $col;
    }
    // Add a row for the exportable status.
    if(!empty($this->entityInfo['exportable'])){
      $row[] = array(
        'data' => array(
          '#theme' => 'entity_status',
          '#status' => $entity->{$this->statusKey}
        )
      );
    }
    // In case this is a bundle, we add links to the field ui tabs.
    $field_ui = !empty($this->entityInfo['bundle of']) && module_exists('field_ui');
    // For exportable entities we add an export link.
    $exportable = !empty($this->entityInfo['exportable']);
    $colspan = 3;
    $colspan = $field_ui ? $colspan + 2 : $colspan;
    $colspan = $exportable ? $colspan + 1 : $colspan;
    // Add operations depending on the status.
    if(entity_has_status($this->entityType, $entity, ENTITY_FIXED)){
      $row[] = array(
        'data' => l(t('clone'), $this->path . '/manage/' . $id . '/clone'),
        'colspan' => $colspan
      );
    }else{
      $row[] = l(t('edit'), $this->path . '/manage/' . $id);
      if($field_ui){
        $row[] = l(t('manage fields'), $this->path . '/manage/' . $id . '/fields');
        $row[] = l(t('manage display'), $this->path . '/manage/' . $id . '/display');
      }
      $row[] = l(t('clone'), $this->path . '/manage/' . $id . '/clone');
      if(empty($this->entityInfo['exportable']) || !entity_has_status($this->entityType, $entity, ENTITY_IN_CODE)){
        $row[] = l(t('delete'), $this->path . '/manage/' . $id . '/delete', array(
          'query' => drupal_get_destination()
        ));
      }elseif(entity_has_status($this->entityType, $entity, ENTITY_OVERRIDDEN)){
        $row[] = l(t('revert'), $this->path . '/manage/' . $id . '/revert', array(
          'query' => drupal_get_destination()
        ));
      }else{
        $row[] = '';
      }
    }
    if($exportable){
      $row[] = l(t('export'), $this->path . '/manage/' . $id . '/export');
    }
    return $row;
  }
}