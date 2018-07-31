(function ($) {
  Drupal.behaviors.remote_issues_block = {
    attach: function(context, settings) {
      const loading = document.querySelector('#remote-issue-tab .loading');
      const noJs = document.querySelector('#remote-issue-tab .no-js');
      const items = document.querySelector('#remote-issue-tab .items');
      const empty = document.querySelector('#remote-issue-tab .empty');

      function handleError() {
        noJs.style.display = 'block';
        items.style.display = 'none';
      }

      noJs.style.display = 'none';
      items.style.display = 'block';

      const providerModule = settings[settings.remote_issue_tab.provider_module];

      $.get(settings.remote_issue_tab.fetch_url, null, function (issues) {
        try {
          const list = items.querySelector('ul');
          issues = providerModule.parse_response(issues);

          if (issues.length === 0) {
            empty.style.display = 'block';
          }

          for(item of issues) {
            const li = document.createElement('li');
            li.innerHTML = (providerModule.render_item || render_item)(item);
            list.appendChild(li);
          }

          loading.style.display = 'none';
        } catch(e) {
          handleError();
          throw e;
        }
      }).fail(handleError);
    }
  }
})(jQuery);

function render_item(item) {
  return `<details>
    <summary class="item-summary">
      <h3 class="item-summary-heading">
        <a href="${ item.link }">${ item.title }</a>
      </h3>
    </summary>
    ${ item.body }
  </details>`;
}
