<?php
  /**
   * Theme a publication group for the pensoft_publication module
   *
   */
?>
<div class="publication-group clearfix">
  <label><?php echo $label; ?></label>
  <?php
    if ($element_id == 'publication_pensoft_full_group_publication_auth_contrib') :
  ?>
    <div class='field field-publication-background'>
      <div class='field-items'>
        <?php echo implode(', ', $variables['authors']); ?>
      </div>
    </div>
  
    <div class='field field-publication-background field-publication-secondary'>
      <div class='field-items'>
        <?php echo implode('', $variables['institutions']); ?>
      </div>
    </div>
  
    <?php  
      $other = array('field_publication_copy_editor', 'field_publication_linguistic_editor', 'field_publication_mentor', 'field_publication_contributors');
      foreach ($other as $field) :
        if (!empty($element[$field]['#children'])) :
     ?>
          <div class='field field-publication-background field-publication-secondary'>
            <div class='field-items'>
              <?php echo $element[$field]['#children']; ?>
            </div>
          </div>
     <?php
        endif;
      endforeach;
    ?>
  <?php elseif ($element_id == 'publication_pensoft_full_group_pensoft_files'): ?>
    <?php /* References */ ?>
    <div class="field">
      <div class="field-label">
        References:
      </div>
      <?php echo $element['field_publication_references'][0]['#markup']; ?>
    </div>
    <?php /* Figures */ ?>
    <?php if (count(element_children($element['field_publication_figures']))) : ?>
      <div class="field">
        <div class="field-label">Figures:</div>
        <div class="field-items">
          <?php
            $count = 0;
            foreach (element_children($element['field_publication_figures']) as $key) :
              $count++;
              $fig = $element['field_publication_figures'][$key];
          ?>
              <div class="clearfix">
                <?php echo str_replace('Figure', 'Figure ' . $count, $fig['#markup']); ?>
              </div>
          <?php endforeach; ?>          
        </div>
      </div>
    <?php endif; ?>
    <?php /* Tables */ ?>
    <?php if (count(element_children($element['field_publication_tables']))) : ?>
      <div class="field">
        <div class="field-label">Tables:</div>
        <div class="field-items">
          <?php
            foreach (element_children($element['field_publication_tables']) as $key) :
              echo render($element['field_publication_tables'][$key]);
             endforeach;
           ?>
        </div>
      </div>
    <?php endif; ?>
  <?php else: ?>
    <?php echo $element['#children']; ?>
  <?php endif; ?>
</div>