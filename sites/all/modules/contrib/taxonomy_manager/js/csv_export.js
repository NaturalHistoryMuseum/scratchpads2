/** 
 * CSV Export
 *
 * adds click event to export button and makes AJAX call to get the CSV data
 */
(function ($) {

Drupal.behaviors.TaxonomyManagerCSVExport = {
  attach: function(context, settings) {    
    if (!$('#edit-export-show.csv-processed').length) {
      $('#edit-export-show').addClass('csv-processed');
      var url = settings.exportCSV['url'];
      var vid = settings.taxonomytree[0].vid;
  
      $("#edit-export-submit").click(function() {
        var area = $("#edit-export-csv");
        var param = new Object();
        param['delimiter'] = $("#edit-export-delimiter").val();
        param['depth'] = $("#edit-export-depth").val();
        param['option'] = $("#taxonomy_manager_export_options").find("input:checked").val();
        param['vid'] = vid;
        var tid = 0;
        $('.treeview').find("input:checked").each(function() {
         tid = Drupal.getTermId($(this).parents("li").eq(0));
        });
        param['tid'] = tid;
    
        $.post(url, param, function(data) {
          $(area).val(data['csv']);
        });
        return false;
      });
    }
  }
}

})(jQuery);
