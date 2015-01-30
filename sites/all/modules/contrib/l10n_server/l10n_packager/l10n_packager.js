(function ($) {
  Drupal.behaviors.l10n_packager_expand = {
    attach:function (context) {
      l10n_packager_fix_table_classes();
      $('tr.l10n-packager-summary a.expand:not(.l10n-packager-processed)', context)
        .addClass('l10n-packager-processed')
        .click(function () {
          var uri = $(this).parents('tr.l10n-packager-summary').attr('id').replace('l10n-packager-summary-', '');
          $(this).parents('tr.l10n-packager-summary').hide();
          $('tr.l10n-packager-detail-' + uri).fadeIn();
          l10n_packager_fix_table_classes();
          return false;
        });
    }
  }

  function l10n_packager_fix_table_classes() {
    // The fist row is the header, so we need to add even classes to odd rows,
    // and odd classes to even rows so the top row is right in the table.
    $('.l10n-packager-download-dynamic tbody tr:visible:odd').addClass('even').removeClass('odd');
    $('.l10n-packager-download-dynamic tbody tr:visible:even').addClass('odd').removeClass('even');
  }
})(jQuery);
