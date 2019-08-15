workflow "✨Add new issues to projects" {
  resolves = ["alex-page/add-new-issue-project"]
  on = "issues"
}

action "alex-page/add-new-issue-project" {
  uses = "alex-page/add-new-issue-project@master"
  args = [ "16th August - 26th September", "To do"]
  secrets = ["GITHUB_TOKEN"]
}
