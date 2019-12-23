<div class="l-page">
    <header class="l-header" role="banner">
        <div class="l-branding sp-bg flex-row flex-stretch-first flex-boxes-center">
            <div class="flex-row flex-stretch-last pad-children-h">
              <?php if ($logo): ?>
                  <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"
                     class="site-logo"><img
                              src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>"/></a>
              <?php endif; ?>

                <div class="flex-column">
                  <?php if ($site_name || $site_slogan): ?>
                    <?php if ($site_name): ?>
                          <h1 class="site-name">
                              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"
                                 rel="home"><?php print $site_name; ?></a>
                          </h1>
                    <?php endif; ?>

                    <?php if ($site_slogan): ?>
                          <h2 class="site-slogan"><?php print $site_slogan; ?></h2>
                    <?php endif; ?>
                  <?php endif; ?>
                </div>
            </div>

          <?php print render($page['header']); ?>
        </div>
      <?php print render($page['menu']); ?>
    </header>

    <div class="l-main">
        <div class="l-content" role="main">
          <?php print render($page['highlighted']); ?>
          <?php print $breadcrumb; ?>
            <a id="main-content"></a>
          <?php print render($title_prefix); ?>
          <?php if ($title): ?>
              <h1><?php print $title; ?></h1>
          <?php endif; ?>
          <?php print render($title_suffix); ?>
          <?php print $messages; ?>
          <?php print render($tabs); ?>
          <?php print render($page['help']); ?>
          <?php if ($action_links): ?>
              <ul class="action-links"><?php print render($action_links); ?></ul>
          <?php endif; ?>
          <?php print render($page['content']); ?>
          <?php print $feed_icons; ?>
        </div>

      <?php print render($page['sidebar']); ?>
    </div>

    <footer class="l-footer" role="contentinfo">
      <?php print render($page['footer']); ?>
    </footer>
</div>
