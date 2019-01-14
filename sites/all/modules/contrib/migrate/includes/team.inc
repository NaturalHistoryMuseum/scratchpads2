<?php

/**
 * @file
 * Info on migration team members. Display-only at the moment, but eventually
 * there should be notification features.
 */

class MigrateTeamMember {

  protected $name;

  public function getName() {
    return $this->name;
  }

  protected $emailAddress;

  public function getEmailAddress() {
    return $this->emailAddress;
  }

  protected $group;

  public function getGroup() {
    return $this->group;
  }

  public function __construct($name, $email_address, $group) {
    $this->name = $name;
    $this->emailAddress = $email_address;
    $this->group = $group;
  }

  public function contact($subject, $text) {
    // TODO
  }
}
