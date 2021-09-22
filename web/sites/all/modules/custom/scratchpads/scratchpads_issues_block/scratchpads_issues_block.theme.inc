<?php

/**
 * Theme function for rendering issue thread
 */
function theme_scratchpads_issues_block_thread($variables)
{
    $title = $variables['thread']['#title'];
    $author = $variables['thread']['#author'];
    $scratchpad = $variables['thread']['#scratchpad'];
    $body = $variables['thread']['#body'];
    $url = $variables['thread']['#github_url'];
    $date = $variables['thread']['#date'];

    if ($url) {
        $title = l($title, $url);
    }

    drupal_add_css(
        drupal_get_path('module', 'scratchpads_issues_block') . '/css/thread.css'
    );

    $a = [
      '#theme' => 'scratchpads_issues_block_comment',
      '#author' => $author,
      '#scratchpad' => $scratchpad,
      '#body' => $body,
      '#date' => $date
    ];
    $issueBody = drupal_render($a);

    $comments = drupal_render_children($variables['thread']);

    // Finally add the form for posting a reply
    $form = drupal_render(drupal_get_form('scratchpads_issues_block_reply_form'));

    // Print issue first comment
    return <<<EOD
<article class="sp2-issue">
  <div class="sp2-issue__title">
    <h1>$title</h1>
  </div>
  <section class="sp2-issue__comments">
    $issueBody
    $comments
  </section>
  $form
</article>
EOD;
}

/**
 * Theme function for rendering individual comment
 */
function theme_scratchpads_issues_block_comment($variables)
{
    require_once libraries_get_path('vendor') . '/autoload.php';

    $author = $variables['author'];
    $scratchpad = $variables['scratchpad'];
    $date = format_date(strtotime($variables['date']));
    $body = \Michelf\Markdown::defaultTransform($variables['body']);

    return <<<EOD
<article class="sp2-issue-comment">
  <header class="sp2-issue-comment__author">
    <b>$author</b><i>$scratchpad</i><em class="sp2-issue-comment__date">$date</em>
  </header>
  <div class="sp2-issue-comment__body">
    $body
  </div>
</article>
EOD;
}
