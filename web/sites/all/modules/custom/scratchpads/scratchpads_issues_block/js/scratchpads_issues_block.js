Drupal.settings.scratchpads_issues_block = {
  // Hook used by remote_issues_tab module
  // Take the JSON body from the issue feed, filter and map
  parse_response: function (body) {
    let issues = body.filter((item) => !item.pull_request).sort((a, b) => {
      let dateA = new Date(a.state === 'closed' ? a.closed_at : a.created_at);
      let dateB = new Date(b.state === 'closed' ? b.closed_at : b.created_at);
      return dateB.getTime() - dateA.getTime();
    });
    issues = issues.slice(0, Math.min(10, issues.length));

    // Map to format expected by remote_issues_tab
    return issues.map((item) => {
      const labels = item.labels.map(
        label => `<span class="label" style="border-color:#${label.color};">${label.name}</span>`
      ).join('');

      const bodyMaxLength = 500;
      let body = '<em>' + item.title + '</em><br>' + (!item.body ? '(No details)' : item.body);
      // Truncate the message if it's too long, to stop firefox crashing
      body = body.length > bodyMaxLength ? body.substring(0, bodyMaxLength) + '…' : body;

      const titleMaxLength = 45;
      const title = item.title.length > titleMaxLength ? item.title.substring(0, titleMaxLength) + '…' : item.title;

      const when = item.state === 'closed' ? new Date(item.closed_at) : new Date(item.created_at);
      const timeDiff = (Date.now() - when.getTime()) / (1000 * 60 * 60);
      const timeAgo = timeDiff > 24 ? Math.floor(timeDiff / 24).toString() + ' days' : Math.floor(timeDiff).toString() + 'h';

      return {
        link: '/issues/' + item.html_url.match(/[0-9]+(?=\/?$)/)[0],
        title: title,
        body: labels + marked(body),
        state: item.state,
        timeAgo: timeAgo
      };
    });
  }
};
