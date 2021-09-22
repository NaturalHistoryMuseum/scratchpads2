<?php

/**
 *
 */
class biblio_handler_sort_contributor_lastname extends views_handler_sort {

  /**
   *
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['rank'] = array('default' => 0);
    return $options;
  }

  /**
   *
   */
  public function admin_summary() {
    $order = parent::admin_summary();
    $rank = $this->rank_options();
    return $rank[$this->options['rank']] . ' ' . t('Author') . ', ' . $order;
  }

  /**
   *
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['op_val_start'] = array('#value' => '<div class="clearfix">');
    $form['rank'] = array(
      '#title' => t('Sort by which author?'),
      '#type' => 'select',
      '#options' => $this->rank_options(),
      '#default_value' => $this->options['rank'],
    );
    $form['op_val_end'] = array('#value' => '</div>');

  }

  /**
   *
   */
  public function rank_options() {
    return array('1st', '2nd', '3rd', '4th', '5th');
  }

  /**
   *
   */
  public function query() {
    parent::query();
    $this->query->add_where(0, "biblio_contributor.rank", $this->options['rank'], '=');
  }

}
