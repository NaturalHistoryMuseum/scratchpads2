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

        // Map to format expected by remote_issues_tab
        return acc.concat({
          link: item.html_url,
          title: item.title,
          body: labels + marked(item.body || '(No details)')
        });
      },
      []
    )
  }
}
