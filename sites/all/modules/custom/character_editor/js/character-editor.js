/**
 * Charcter project JS
 */
(function($){
  /**
   * Drupal Behaviour.
   *
   * UI elements are created here. Note that it is assumed that there is only
   * one CharacterEditor on a page.
   */
  Drupal.behaviors.characterEditor = {
    attach: function(context, settings){
      var $slick = $('#slickgrid', context);
      // Overlay width fix
      if(typeof settings.overlay === 'undefined') {
        $slick.parent().width($('#overlay-content').find('#content').width());
      }
      // Bind to slickgrid init
      if (typeof Drupal.settings.CharacterEditorInit === 'undefined' || !Drupal.settings.CharacterEditorInit){
        Drupal.settings.CharacterEditorInit = true;
        $slick.bind('onSlickgridInit', function(event, slickgrid){
          // Create the tree
          Drupal.characterTreeUI = new Drupal.CharacterTreeUI(Drupal.settings.CharacterTreeUI.mode, Drupal.settings.CharacterTreeUI.tree, context, slickgrid);
          Drupal.settings.CharacterTreeUI.tree = {};
          // Create the cell hover
          Drupal.characterHoverUI = new Drupal.ColumnHoverUI(slickgrid);
          // Create the context menu
          Drupal.characterContextMenu = new Drupal.CharacterContextMenu(slickgrid);
          // Create the metadata manager
          Drupal.characterMetadataManager = new Drupal.CharacterMetadataManager(slickgrid);
          // Alter Slickgrid ajax call to specifiy which character project this is (slickgrid typically
          // only specifies the view/display)
          $.ajaxSetup({
            beforeSend: function(jqXHR, settings){
              if (settings.url.match(/slickgrid\/get\/form\/slickgrid_settings_form/)){
                if (typeof settings.data == 'string' && settings.data.length > 0){
                  var project_id = encodeURIComponent(Drupal.settings.CharacterTreeUI.project);
                  settings.data = settings.data.replace(/display_id=[^&]+(&|$)/, 'display_id=' + project_id + '$1');
                }
              }
            }
          });
        });
      } else {
        Drupal.characterTreeUI.updateTree(Drupal.settings.CharacterTreeUI.tree);
        Drupal.settings.CharacterTreeUI.tree = {};
      }
    }
  }
})(jQuery);