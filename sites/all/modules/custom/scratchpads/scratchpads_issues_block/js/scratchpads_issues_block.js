Drupal.settings.scratchpads_issues_block = {
  // Hook used by remote_issues_tab module
  // Take the JSON body from the issue feed, filter and map
  parse_response: function(body) {
    return body.reduce(
      (acc, item) => {
        // Take first 10 issues that aren't pull requests
        if (acc.length >= 10 || item.pull_request) {
          return acc;
        }

        const labels = item.labels.map(
          label => `<span class="label" style="border-color:#${label.color};">${ label.name }</span>`
        ).join('');

        const bodyMaxLength = 500;
        const body = !item.body ? '(No details)' :
                     // Truncate the message if it's too long, to stop firefox crashing
                     item.body.length > bodyMaxLength ? item.body.substring(0, bodyMaxLength) + '…' :
                     item.body;

        // Map to format expected by remote_issues_tab
        return acc.concat({
          link: '/issues/' + item.html_url.match(/[0-9]+(?=\/?$)/)[0],
          title: item.title,
          body: labels + marked(body)
        });
      },
      []
    )
  }
}
