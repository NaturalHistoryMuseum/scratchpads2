<?php

/**
 * The class used for publication entities
 */
class EntityUI extends Entity{

  protected function defaultLabel(){
    return $this->title;
  }

  protected function defaultUri(){
    return array(
      'path' => str_replace(' ', '-', strtolower($this->entityInfo['label'])) . (isset($this->id) ? '/' . $this->id : '')
    );
  }
}