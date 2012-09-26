<?php

class DrupalFieldFileAttachmentMigration extends DrupalFieldMigration{

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments){
    // Add some extra source fields so they can be set in prepareRow()
    $this->sourceFields['widget_settings'] = t("Widget settings");
    $this->sourceFields['display_settings'] = t("Display settings");
    $this->sourceFields['global_settings'] = t("Global settings");
    $this->sourceFields['widget_type'] = t("Widget type");
    $this->sourceFields['module'] = t("Module");
    $this->sourceFields['widget_module'] = t("Widget module");
    $this->sourceFields["label"] = t("Label");
    $this->sourceFields["description"] = t("Description");
    $this->sourceFields["required"] = t("Required");
    $this->sourceFields["field_name"] = t("Field name");
    $this->sourceFields["type_name"] = t("Type name");
    $this->sourceFields["active"] = t("Active");
    $this->sourceFields["weight"] = t("Weight");
    $this->sourceFields["db_columns"] = t("DB columns");
    $this->sourceFields["db_storage"] = t("DB storage");
    $this->sourceFields["locked"] = t("locked");
    $this->sourceFields["multiple"] = t("Multiple");
    $this->sourceFields["widget_active"] = t("Active");
    parent::__construct($arguments);
    $this->source = new MigrateSourceSQL($this->sourceQuery(), $this->sourceFields, NULL, $this->sourceOptions);
    $this->map = new MigrateSQLMap($this->machineName, array(
      'type_name' => array(
        'type' => 'varchar',
        'length' => 255
      )
    ), MigrateDestinationField::getKeySchema());
    $this->destination = new MigrateDestinationField();
    $this->removeFieldMapping('type');
    $this->addFieldMapping('type', 'type_name');
  }

  /**
   * Query for the basic menu data.
   *
   * @return QueryConditionInterface
   */
  protected function sourceQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('node', 'n');
    $query->addField('n', 'type', 'type_name');
    $query->innerJoin('upload', 'u', 'n.nid = u.nid');
    $query->condition('n.type', DrupalFieldMigration::excludedNodeTypes(), 'NOT IN');
    $query->distinct();
    return $query;
  }

  public function prepareRow($row){
    $row->entity_type = 'node';
    $row->field_name = 'field_file';
    $row->widget_settings = array(
      'type' => 'file_generic',
      'module' => 'file'
    );
    $row->widget_type = 'file_generic';
    $row->widget_module = 'file';
    $row->display_settings = array();
    $row->global_settings = array(
      'file_extensions' => 'txt pdf doc'
    );
    $row->type = 'file';
    $row->label = t('File attachments');
    if(parent::prepareRow($row) === FALSE){return FALSE;}
  }
}
