<?php

/**
 * class PublicationHtmlFormatXMLModifier
 *
 * Implement a PublicationXMLModifierInterface that is used to
 * track citations (figures/tables/references) in
 * the generated content, rename and reorder them
 * appropriately (extending PublicationHtmlFormatChangeCitations for
 * this functionality)
 */
class PublicationHtmlFormatXMLModifier extends PublicationHtmlFormatChangeCitations implements PublicationXMLModifierInterface{

  /**
   * Implement PublicationXMLModifierInterface::start_building
   */
  public function start_building($schema, $entity_type, $entity, $dom){}

  /**
   * Implement PublicationXMLModifierInterface::insert_value
   */
  public function insert_value($schema, $tag, $value){
    if(is_string($value)){
      // Replace and track table citations
      $value = preg_replace_callback('/(<tbls-citation.*?citation_id=["\'](.*?)["\'].*?>).*?(<\/tbls-citation>)/', array(
        $this,
        '_replace_table_citation'
      ), $value);
      // Replace and track figure citations
      $value = preg_replace_callback('/(<fig-citation.*?citation_id=["\'](.*?)["\'].*?>).*?(<\/fig-citation>)/', array(
        $this,
        '_replace_figure_citation'
      ), $value);
      // Replace reference citations
      $value = preg_replace_callback('/(\[bib\]\s*(\d+)\s*\[\/bib\])/', array(
        $this,
        '_replace_reference_citation'
      ), $value);
    }
    return $value;
  }

  /**
   * Implement PublicationXMLModifierInterface::insert_value_array
   * to re-order the figures and tables.
   * XXX This only works if tables/figures are added after the
   * fields that cite them (which is fine with the current schema, but
   * could be a problem if it changes)
   *
   */
  public function insert_value_array($schema, $tag, $values){
    if(isset($schema['#field'])){
      if($schema['#field'] == 'field_publication_tables'){
        // Re-order tables
        $new_values = array();
        $max_order = count($this->table_ids);
        foreach($values as $collection){
          $lang = field_language('field_collection_item', $collection, 'field_publication_table_id');
          $table_id = $collection->field_publication_table_id[$lang][0]['value'];
          $order = array_search($table_id, $this->table_ids);
          if($order === FALSE){
            $order = $max_order++;
          }
          $new_values[$order] = $collection;
        }
        ksort($new_values);
        return $new_values;
      }else if($schema['#field'] == 'field_publication_figures'){
        // Re-order figures
        $new_values = array();
        $max_order = count($this->figure_ids);
        foreach($values as $collection){
          $lang = field_language('field_collection_item', $collection, 'field_publication_figure_figure');
          $fid = $collection->field_publication_figure_figure[$lang][0]['fid'];
          $order = array_search($fid, $this->figure_ids);
          if($order === FALSE){
            $order = $max_order++;
          }
          $new_values[$order] = $collection;
        }
        ksort($new_values);
        return $new_values;
      }
    }
    return $values;
  }
}