<?php
// $Id: page.tpl.php,v 1.17.2.4 2010/11/19 14:42:44 danprobo Exp $
?>
<div <?php print danland_page_class($page['sidebar_first'], $page['sidebar_second']); ?>>
<div id="header">
<div id="header-wrapper">
	<?php if ($logo): ?> 
		<div id="logo-wrapper">
			<div class="logo">
				<a href="<?php print $base_path ?>" title="<?php print t('Home') ?>"><img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" /></a>
			</div>
		</div><!-- end logo wrapper -->
	<?php endif; ?>
	<?php if ($site_name || $site_slogan) : ?>
		<div id="branding-wrapper">
			<?php if ($site_name) : ?>
				<?php if ($is_front) : ?>
					<h1 class="site-name"><a href="<?php print $base_path ?>" title="<?php print $site_name ?>"><?php print $site_name ?></a></h1>
				<?php endif; ?>
				<?php if (!$is_front) : ?>
					<h2 class="site-name"><a href="<?php print $base_path ?>" title="<?php print $site_name ?>"><?php print $site_name ?></a></h2>
				<?php endif; ?>
			<?php endif; ?>
			<?php if ($site_slogan) : ?>
				<div class='site-slogan'><?php print $site_slogan; ?></div>
			<?php endif; ?>
        	</div><!-- end branding wrapper -->
	<?php endif; ?>
	
	<?php if ($page['search_box']): ?>
		<div id="search-box">
			<?php print render ($page['search_box']); ?>
		</div><!-- /search-box -->
	<?php endif; ?>

	<?php if ($feed_icons): ?>
		<div class="feed-wrapper">
			<?php print $feed_icons; ?>
		</div>
	<?php endif; ?>

	<?php if (!$is_admin): ?>
		<div id="authorize">
      		      <ul><?php global $user; if ($user->uid != 0) { print '<li class="first">' .t('Logged in as '). '<a href="' .url('user/'.$user->uid). '">' .$user->name. '</a></li>'; print '<li><a href="' .url('user/logout'). '">' .t('Logout'). '</a></li>'; } else { print '<li class="first"><a href="' .url('user'). '">' .t('Login'). '</a></li>'; print '<li><a href="' .url('user/register'). '">' .t('Register'). '</a></li>'; } ?></ul>
		</div>
	<?php endif; ?>

      </div><!-- end header-wrapper -->
</div> <!-- /header -->
<div style="clear:both"></div>

<div id="menu">
<div id="rounded-menu-left"></div>
 <?php if ($main_menu || $page['superfish_menu']): ?>
      <div id="<?php print $main_menu ? 'nav' : 'superfish' ; ?>">
        <?php 
					     if ($main_menu) {
		          print theme('links__system_main_menu', array('links' => $main_menu));  
				      }
				      elseif (!empty($page['superfish_menu'])) {
				        print render ($page['superfish_menu']);
				      }
        ?>
      </div> <!-- end primary -->
    <?php endif; ?>
<div id="rounded-menu-right"></div>
</div> <!-- end menu -->
<div style="clear:both"></div>

<?php if($is_front): ?>
<div id="slideshow-wrapper">
<div class="slideshow-inner">
<div id="slideshow-preface">
 <?php if ($page['preface']): ?>
          <div id="preface">
            <?php print render ($page['preface']); ?>
          </div><!-- end preface -->
 <?php endif; ?>
</div>
<?php if ($page['highlighted']) : ?><div id="slideshow-bottom">
<div id="mission"><?php print render ($page['highlighted']); ?></div></div><?php endif; ?>
<div class="slideshow">
<img src="<?php print $base_path . $directory; ?>/images/slideshows/sea.jpg" width="950" height="355" alt="slideshow 1"/>
<img src="<?php print $base_path . $directory; ?>/images/slideshows/noon.jpg" width="950" height="355" alt="slideshow 2"/>
<img src="<?php print $base_path . $directory; ?>/images/slideshows/snow.jpg" width="950" height="355" alt="slideshow 3"/>
</div>
</div>
</div>
<?php endif; ?>

 <?php if($page['preface_first'] || $page['preface_middle'] || $page['preface_last']) : ?>
    <div style="clear:both"></div>
    <div id="preface-wrapper" class="in<?php print (bool) $page['preface_first'] + (bool) $page['preface_middle'] + (bool) $page['preface_last']; ?>">
          <?php if($page['preface_first']) : ?>
          <div class="column A">
            <?php print render ($page['preface_first']); ?>
          </div>
          <?php endif; ?>
          <?php if($page['preface_middle']) : ?>
          <div class="column B">
            <?php print render ($page['preface_middle']); ?>
          </div>
          <?php endif; ?>
          <?php if($page['preface_last']) : ?>
          <div class="column C">
            <?php print render ($page['preface_last']); ?>
          </div>
          <?php endif; ?>
      <div style="clear:both"></div>
    </div>
    <?php endif; ?>

<div style="clear:both"></div>
<div id="wrapper">

    <?php if ($page['sidebar_first']): ?>
      <div id="sidebar-left" class="column sidebar"><div class="section">
        <?php print render($page['sidebar_first']); ?>
      </div></div> <!-- end sidebar-first -->
    <?php endif; ?>
<div id="content">
			<a id="main-content"></a>
			<?php if ($page['content_top']) : ?><div class="content-top"><?php print render ($page['content_top']); ?></div>
			<?php endif; ?>
			<?php if (!$is_front) print $breadcrumb; ?>
			<?php if ($show_messages) { print $messages; }; ?>
      		<?php print render($title_prefix); ?>
      			<?php if ($title): ?>
        				<h1 class="title" id="page-title">
         			 		<?php print $title; ?>
        				</h1>
     				 <?php endif; ?>
      		<?php print render($title_suffix); ?>
      		<?php if ($tabs): ?>
        			<div class="tabs">
          				<?php print render($tabs); ?>
        			</div>
      		<?php endif; ?>
      		<?php print render($page['help']); ?>
      		<?php if ($action_links): ?>
        			<ul class="action-links">
          				<?php print render($action_links); ?>
        			</ul>
      		<?php endif; ?>
		      <?php if ($page['content']) : ?><div class="content-middle"><?php print render ($page['content']); ?></div>
			<?php endif; ?>
			<?php if ($page['content_bottom']) : ?><div class="content-bottom"><?php print render ($page['content_bottom']); ?></div>
			<?php endif; ?>

</div> <!-- end content -->

    <?php if ($page['sidebar_second']): ?>
      <div id="sidebar-right" class="column sidebar"><div class="section">
        <?php print render($page['sidebar_second']); ?>
      </div></div> <!-- end sidebar-second -->
    <?php endif; ?>
<div style="clear:both"></div>
</div> <!-- end wrapper -->


<?php if($page['bottom_first'] || $page['bottom_middle'] || $page['bottom_last']) : ?>
    <div style="clear:both"></div>
    <div id="bottom-teaser" class="in<?php print (bool) $page['bottom_first'] + (bool) $page['bottom_middle'] + (bool) $page['bottom_last']; ?>">
          <?php if($page['bottom_first']) : ?>
          <div class="column A">
            <?php print render ($page['bottom_first']); ?>
          </div>
          <?php endif; ?>
          <?php if($page['bottom_middle']) : ?>
          <div class="column B">
            <?php print render ($page['bottom_middle']); ?>
          </div>
          <?php endif; ?>
          <?php if($page['bottom_last']) : ?>
          <div class="column C">
            <?php print render ($page['bottom_last']); ?>
          </div>
          <?php endif; ?>
      <div style="clear:both"></div>
    </div> <!-- end bottom first etc. -->
    <?php endif; ?>


 <?php if($page['bottom_1'] || $page['bottom_2'] || $page['bottom_3'] || $page['bottom_4']) : ?>
    <div style="clear:both"></div><!-- Do not touch -->
    <div id="bottom-wrapper" class="in<?php print (bool) $page['bottom_1'] + (bool) $page['bottom_2'] + (bool) $page['bottom_3'] + (bool) $page['bottom_4']; ?>">
          <?php if($page['bottom_1']) : ?>
          <div class="column A">
            <?php print render ($page['bottom_1']); ?>
          </div>
          <?php endif; ?>
          <?php if($page['bottom_2']) : ?>
          <div class="column B">
            <?php print render ($page['bottom_2']); ?>
          </div>
          <?php endif; ?>
          <?php if($page['bottom_3']) : ?>
          <div class="column C">
            <?php print render ($page['bottom_3']); ?>
          </div>
          <?php endif; ?>
          <?php if($page['bottom_4']) : ?>
          <div class="column D">
            <?php print render ($page['bottom_4']); ?>
          </div>
          <?php endif; ?>
      <div style="clear:both"></div>
    </div><!-- end bottom -->
    <?php endif; ?>

<div style="clear:both"></div>
<div id="footer-wrapper">
<?php if($page['footer']): ?>
<div id="footer">
 <?php print render ($page['footer']); ?>
</div>
<?php endif; ?>
<?php if($secondary_menu) : ?>
<div id="subnav-wrapper">
<?php print theme('links__system_secondary_menu', array('links' => $secondary_menu, 'attributes' => array('id' => 'subnav', 'class' => array('links', 'clearfix')))); ?>
</div>
<?php endif; ?>
</div> <!-- end footer wrapper -->

<div style="clear:both"></div>
<div id="notice"><p>Theme by <a href="http://www.danetsoft.com">Danetsoft</a> and <a href="http://www.danpros.com">Danang Probo Sayekti</a> inspired by <a href="http://www.maksimer.no">Maksimer</a></p></div>
</div>