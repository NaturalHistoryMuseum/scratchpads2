<?php

class DrupalUser7Migration extends DrupalUserMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->destination = new MigrateDestinationUser();
  }
}
