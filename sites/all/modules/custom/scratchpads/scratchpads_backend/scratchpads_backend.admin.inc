<?php

/**
 * Menu callback
 * New admin content
 */
function scratchpads_backend_admin_content(){
  $entities_info = entity_get_info();
  $build = array();
  $header = array(
    t('Type'),
    array(
      'data' => t('Published'),
      'class' => array(
        'scratchpads-admin-content-stats',
        'scratchpads-admin-content-published'
      )
    ),
    array(
      'data' => t('Unpublished'),
      'class' => array(
        'scratchpads-admin-content-stats',
        'scratchpads-admin-content-unpublished'
      )
    ),
    array(
      'data' => '',
      'class' => array(
        'scratchpads-admin-content-operations'
      )
    )
  );
  foreach($entities_info as $entity_type => $entity_info){
    $attributes = array(
      'class' => array(
        'scratchpads-admin-content',
        'scratchpads-admin-content-' . $entity_type
      )
    );
    $rows = array();
    switch($entity_type){
      case 'node':
        $node_types = node_type_get_types();
        $statistics = scratchpads_backend_statistics($entity_info);
        foreach(array_keys($entity_info['bundles']) as $bundle){
          if(!$node_types[$bundle]->disabled){
            $view_url = 'admin/content/' . $bundle;
            $operations = array(
              l(t('View'), $view_url),
              l(t('Add'), 'node/add/' . str_replace('_', '-', $bundle)),
              $bundle == 'media_gallery' ? '<strike style="color:white">' . t('Import') . '</strike>' : l(t('Import'), ($bundle == 'biblio' ? 'biblio/import' : 'import/node_importer_' . $bundle . '_xls'))
            );
            // If there's a grid editor for this view, add a link
            if(views_get_view('slickgrid_admin_' . $bundle)){
              $operations[] = l(t('Grid'), 'admin/content/slickgrid/' . $bundle);
            }
            $operations = theme('item_list', array(
              'items' => $operations
            ));
            $rows[] = array(
              'data' => array(
                l($node_types[$bundle]->name, $view_url) . (!empty($node_types[$bundle]->description) ? '<p class="description">' . $node_types[$bundle]->description . '</p>' : ''),
                (isset($statistics[$bundle]['published']) ? $statistics[$bundle]['published'] : 0),
                (isset($statistics[$bundle]['unpublished']) ? $statistics[$bundle]['unpublished'] : 0),
                $operations
              ),
              'class' => array(
                'node-' . $bundle
              )
            );
          }
        }
        // Manually add character projects if it's enabled
        if(array_key_exists('character_editor_project', $entities_info)){
          $statistics = scratchpads_backend_statistics($entities_info['character_editor_project'], false);
          $view_url = 'admin/content/character-editor/projects';
          $operations = theme('item_list', array(
            'items' => array(
              l(t('View'), $view_url),
              l(t('Add'), 'admin/content/character-editor/projects/add/default_character_project')
            )
          ));
          $rows[] = array(
            'data' => array(
              l(t('Character projects'), $view_url) . '<p class="description">' . t('Character projects') . '</p>',
              (isset($statistics['published']) ? $statistics['published'] : 0),
              (isset($statistics['unpublished']) ? $statistics['unpublished'] : 0),
              $operations
            ),
            'class' => array(
              'character-project'
            )
          );
        }
        $build[$entity_type] = array(
          '#theme' => 'table',
          '#weight' => 1,
          '#header' => $header,
          '#rows' => $rows,
          '#attributes' => $attributes
        );
        break;
      case 'comment':
        $statistics = scratchpads_backend_statistics($entity_info, false);
        $view_url = 'admin/content/comment';
        $operations = theme('item_list', array(
          'items' => array(
            l(t('View'), $view_url)
          )
        ));
        $rows = array(
          array(
            l(t('Comments'), $view_url) . '<p class="description">' . t("View comments on your site") . '</p>',
            (isset($statistics['published']) ? $statistics['published'] : 0),
            (isset($statistics['unpublished']) ? $statistics['unpublished'] : 0),
            $operations
          )
        );
        $comments_header = $header;
        $comments_header[2]['data'] = t('Unapproved');
        $build[$entity_type] = array(
          '#theme' => 'table',
          '#caption' => t("Comments"),
          '#header' => $comments_header,
          '#weight' => 2,
          '#rows' => $rows,
          '#attributes' => $attributes
        );
        break;
      case 'file':
        $statistics = scratchpads_backend_statistics($entity_info);
        foreach(array_keys($entity_info['bundles']) as $bundle){
          $view_url = 'admin/file/' . $bundle;
          $operations = array(
            l(t('View'), $view_url),
            l(t('Add'), 'media/browser', array(
              'query' => _scratchpads_tweaks_get_media_browser_query()
            ))
          );
          // If there's a grid editor for this view, add a link
          if(views_get_view('slickgrid_admin_' . $bundle)){
            $operations[] = l(t('Grid'), 'admin/file/slickgrid/' . $bundle);
          }
          $operations = theme('item_list', array(
            'items' => $operations
          ));
          $rows[] = array(
            'data' => array(
              l($entity_info['bundles'][$bundle]['label'], $view_url),
              (isset($statistics[$bundle]['published']) ? $statistics[$bundle]['published'] : 0),
              (isset($statistics[$bundle]['unpublished']) ? $statistics[$bundle]['unpublished'] : 0),
              $operations
            ),
            'class' => array(
              'file-' . $bundle
            )
          );
          $file_header = $header;
          $file_header[0] = t('File type');
          $build[$entity_type] = array(
            '#theme' => 'table',
            '#caption' => t("Files"),
            '#weight' => 3,
            '#header' => $file_header,
            '#rows' => $rows,
            '#attributes' => $attributes
          );
        }
        break;
      case 'publication':
        $statistics = scratchpads_backend_statistics($entity_info);
        foreach(array_keys($entity_info['bundles']) as $bundle){
          $view_url = 'admin/content/publications';
          $operations = array(
            l(t('View'), $view_url),
            l(t('Add'), 'publication/add/' . $bundle)
          );
          $operations = theme('item_list', array(
            'items' => $operations
          ));
          $rows[] = array(
            'data' => array(
              l($entity_info['bundles'][$bundle]['label'], $view_url),
              (isset($statistics[$bundle]['published']) ? $statistics[$bundle]['published'] : 0),
              (isset($statistics[$bundle]['unpublished']) ? $statistics[$bundle]['unpublished'] : 0),
              $operations
            ),
            'class' => array(
              'publication-' . $bundle
            )
          );
          $file_header = $header;
          $file_header[0] = t('Publications');
          $build[$entity_type] = array(
            '#theme' => 'table',
            '#caption' => t("Publications"),
            '#weight' => 3,
            '#header' => $file_header,
            '#rows' => $rows,
            '#attributes' => $attributes
          );
        }
        break;
    }
  }
  return $build;
}

function scratchpads_backend_statistics($entity_info, $bundle = true){
  $statistics = array();
  $query = db_select($entity_info['base table'], 't');
  $id_key = $entity_info['entity keys']['id'];
  $bundle_key = $entity_info['entity keys']['bundle'];
  $query->addExpression('COUNT(DISTINCT (t.' . $id_key . '))', 'count');
  // If this is a file type, hide the eol files
  if($entity_info['base table'] == 'file_managed'){
    $query->leftJoin('file_usage', 'f', 't.fid = f.fid');
    $query->where('f.type != :type OR f.type IS NULL', array(
      ':type' => 'eolapi'
    ));
  }
  // Do we want to break these stats down by bundle?
  if($bundle){
    $query->fields('t', array(
      $bundle_key,
      'status'
    ));
    $query->groupBy($bundle_key);
    $query->groupBy('status');
    foreach($query->execute() as $result){
      if($result->status == 0){
        $key = 'unpublished';
      }else{
        $key = 'published';
      }
      $statistics[$result->{$bundle_key}][$key] = $result->count;
    }
  }else{ //Just get all the stats for this entity type
    $query->fields('t', array(
      'status'
    ));
    $query->groupBy('status');
    foreach($query->execute() as $result){
      if($result->status == 0){
        $key = 'unpublished';
      }else{
        $key = 'published';
      }
      $statistics[$key] = $result->count;
    }
  }
  return $statistics;
}

