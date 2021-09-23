(function ($) {
  Drupal.behaviors.remote_issues_block = {
    attach: function (context, settings) {

      // setup jquery ui tabs
      $('#issuesTabs').tabs({});

      // define the provider
      const providerModule = settings[settings.remote_issue_tab.provider_module];

      // no-js block (outside of tabs)
      const noJs = $('#remote-issue-tab .no-js');

      // tabs
      const issuesTabs = {
        'open': {
          'query': {'state': 'open'},
          'ref': $('#issuesTabOpen')
        },
        'closed': {
          'query': {'state': 'closed', 'sort': 'updated'},
          'ref': $('#issuesTabClosed')
        }
      };

      function handleError() {
        noJs.css('display', 'block');
        $.each(issuesTabs, (tabName, o) => {
          o.ref.css('display', 'none');
        });
      }

      noJs.css('display', 'none');


      $.each(issuesTabs, (tabName, o) => {
        $.get(settings.remote_issue_tab.fetch_url, o.query, function (issues) {
          try {
            issues = providerModule.parse_response(issues);
            const list = o.ref.find('ul.issues');
            list.empty();
            if (issues.length === 0) {
              o.ref.find('.empty').css('display', 'block');
            }

            for (item of issues) {
              const li = document.createElement('li');
              li.innerHTML = (providerModule.render_item || render_item)(item);
              list.append(li);
            }

            o.ref.find('.loading').css('display', 'none');

          } catch (e) {
            handleError();
            throw e;
          }
        }).fail(handleError);
      });
    }
  };
})(jQuery);

function render_item(item) {
  return `<details>
    <summary class="item-summary">
      <h3 class="item-summary-heading">
        <a href="${item.link}">${item.title}</a>
        <span class="item-time-ago">${item.timeAgo}</span>
      </h3>
    </summary>
    ${item.body}
  </details>`;
}
