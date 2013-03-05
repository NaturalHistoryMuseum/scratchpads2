<?php
class biblio_handler_citation extends views_handler_field {
  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->additional_fields['nid'] = array('table' => 'node', 'field' => 'nid');
  }

  function query() {
    $this->field_alias = 'nid';
    $this->add_additional_fields();
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['style_name'] = array('default' => biblio_get_style());
    $options['export_links'] = array('default' => 1);
    $options['file_attachments'] = array('default' => 1);
    $options['open_url_link'] = array('default' => 1);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['style_name'] = array(
      '#type' => 'select',
      '#title' => t('Style'),
      '#default_value' => $this->options['style_name'],
      '#options' => biblio_get_styles(),
      '#description' => t('Define the layout of citation.')
    );
    $form['export_links'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show export links'),
      '#default_value' => $this->options['export_links'],
      '#description' => t('This will add a set of links to export the entry in various file formats such as Bibtex or RIS.')
    );
    $form['file_attachments'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show download links for file attachments'),
      '#default_value' => $this->options['file_attachments'],
      '#description' => t('If there are files attached to the entry, this will add a download link for each file attached.')
    );
    $form['open_url_link'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show OpenURL links'),
      '#default_value' => $this->options['open_url_link'],
      '#description' => t('This will add an !openurl link to the entry, assuming you have competed the OpenURL configuration on the Biblio !settings page.', array('!openurl' => l('OpenURL', "http://en.wikipedia.org/wiki/OpenURL"), '!settings' => l('settings', 'admin/config/content/biblio') )),
    );
  }

  function pre_render(&$values) {
    $nids  = array();
    $nodes = array();

    foreach ($values as $result) {
      if (!empty($result->{$this->aliases['nid']})) {
        $nids[] = $result->{$this->aliases['nid']};
      }
    }

    if ($nids) {
      $langcode = $GLOBALS['language_content']->language;
      $nodes = node_load_multiple($nids);
      if (!empty($nodes)) {
        field_attach_prepare_view('node', $nodes, 'full', $langcode);
        entity_prepare_view('node',  $nodes, $langcode);

        foreach ($values as $key => $result) {
          if (isset($result->{$this->aliases['nid']})) {
            $values[$key]->node = $nodes[$result->{$this->aliases['nid']}];
          }
        }
      }
    }
  }

  function render($values) {
    $output = '';

    if (!isset($values->node) || $values->node->type != 'biblio') {
      return;
    }

    if (empty($this->biblio_base)) {
      $this->biblio_base = variable_get('biblio_base', 'biblio');
    }

    $item = $values->node;

    if (isset($item->biblio_year)) $item->biblio_year = _biblio_text_year($item->biblio_year);
    if (variable_get('biblio_hide_bibtex_braces', 0)) {
      $item->title = biblio_remove_brace($item->title);
    }

    if (!$item->status) {
      $output .= '<div id="node-' . $item->nid . '" class="node node-unpublished">';
    }

    // first add the styled entry...
    $output .= theme('biblio_style', array('node' => $item, 'style_name' => $this->options['style_name']));

    $annotation_field = variable_get('biblio_annotations', 'none');
    if ($annotation_field != 'none' && $item-> $annotation_field) {
      $output .= '<div class="biblio-annotation">';
      $output .= filter_xss($item->$annotation_field, biblio_get_allowed_tags());
      $output .= '</div>';
    }

    $openurl_base = variable_get('biblio_baseopenurl', '');
    if (!empty($openurl_base) && $this->options['open_url_link']) {
      $output .= theme('biblio_openurl', array('openURL' => biblio_openurl($item)));
    }

    if (biblio_access('export') && $this->options['export_links']) {
      $output .= theme('biblio_export_links', array('node' => $item));
    }

    if (biblio_access('download', $item) && $this->options['file_attachments']) {
      $output .= theme('biblio_download_links', array('node' => $item));
    }
    if (!$item->status) {
      $output .= '</div>';
    }

    return $output;
   }

}