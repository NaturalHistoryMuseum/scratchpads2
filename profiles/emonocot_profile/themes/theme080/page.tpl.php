
<div class="bg-top">
<div class="bg">
<div id="main">
<div id="header">
<div class="head-row1">
<div class="col1">
                            <?php
                            $logo = base_path() . drupal_get_path('theme', 'emonocot_sp') . '/logo.png';
                            ?>
                                <p align="center"><img
	src="<?php
print($logo)?>" alt="<?php
print t('Home')?>" border="0"
	class="logo" /><br />
</p>
                            
                            <?php
                            if($site_name):
                              ?>
                                <h1 class='site-name'><a
	href="<?php
                              print $front_page?>" title="<?php
                              print t('Home')?>"><?php
                              print $site_name?></a></h1>
                            
                            <?php endif;
                            ?>
                            
                            <?php
                            if($site_slogan):
                              ?>
                                <div class="slogan"><?php
                              print($site_slogan)?></div>
                            
                            <?php endif;
                            ?>
                        </div>
<div class="col2">
<div class="userlogin">User Login</div>
</div>
</div>
<div class="head-row2">
<div class="col1">
           <?php
          if($main_menu):
            ?>
      <div id="navigation">
<div class="pr-menu">
        <?php
            print theme('links__system_main_menu', array(
              'links' => $main_menu,
              'attributes' => array(
                'id' => 'main-menu',
                'class' => array(
                  'primary-links',
                  'inline',
                  'clearfix'
                )
              )
            ));
            ?>
      </div>
</div>
<!-- /.section, /#navigation -->
    
          <?php endif;
          ?>

                        </div>
<div class="col2">
<div class="search-box"></div>
</div>
</div>


</div>

<div id="cont">
<div class="border-left">
<div class="border-right">
<div class="border-bot">
<div class="corner-bot-left">
<div class="corner-bot-right">
<div class="head-row3">

<div id="header-blue">
                        <?php
                        if($breadcrumb != ""):
                          ?>
                            <?php
                          print $breadcrumb?>
                        
                        <?php endif;
                        ?>
                    </div>
</div>




<div id="left-col">
<div class="ind">
                                                <?php
                                                if($page['sidebar_first']):
                                                  ?>
        <div id="sidebar-first" class="sidebar">
          <?php
                                                  print render($page['sidebar_first']);
                                                  ?>
        </div>
      
                                                <?php endif;
                                                ?>
                                            </div>
</div>

<div id="cont-col">
<div class="ind">
<div class="border-bot2">
<div class="corner-bot-left2">
<div class="corner-bot-right2">
<div class="corner-top-left2">
<div class="corner-top-right2">
<div class="inner">
                                                                    	<?php
                                                                    if($is_front != ""):
                                                                      ?>
                                                                            <div
	id="custom"><?php
                                                                      print render($page['custom']);
                                                                      ?></div>
																		
                                                                    <?php endif;
                                                                    ?>
                                                                        
                                           
                                                                        
                                                                        
                                                                        
                                                                        <div
	class="cent">
                                                                            <?php
                                                                            if(!empty($page['highlighted'])):
                                                                              ?>
                                                                                <div
	id="mission"><?php
                                                                              print render($page['highlighted']);
                                                                              ?></div>
                                                                            
                                                                            <?php endif;
                                                                            ?>
                                                                                        
                                                                            <?php
                                                                            if($tabs):
                                                                              ?><div
	id="tabs-wrapper" class="clearfix">
                                                                            <?php endif;
                                                                            ?>
          <?php
          print render($title_prefix);
          ?>
          <?php
          if($title):
            ?>
            <h1 <?php
            print $tabs ? ' class="title"' : ''?>><?php
            print $title?></h1>
          
          <?php endif;
          ?>
          <?php
          print render($title_suffix);
          ?>
          <?php
          if($tabs):
            ?><?php

            print render($tabs);
            ?></div>
          <?php endif;
          ?>
          <?php
          print render($tabs2);
          ?>
                                                                                             
                                                                            <?php
                                                                            if($show_messages && $messages != ""):
                                                                              ?>
                                                                                <?php
                                                                              print $messages?>
                                                                            
                                                                            <?php endif;
                                                                            ?>
                                                                        
                                                                            <?php
                                                                            if(!empty($help)):
                                                                              ?>
                                                                                <div
	id="help"><?php
                                                                              print $help?></div>
                                                                            
                                                                            <?php endif;
                                                                            ?>
                                                                        
                                                                              <!-- start main content -->
                                                                            <?php
                                                                            print render($page['content']);
                                                                            ?>
                                                                        </div>
</div>
</div>
</div>
</div>
</div>
</div>
</div>
</div>
</div>
</div>
</div>
</div>
</div>

</div>

</div>

<div id="footer">
<div class="foot"></div>
</div>
</div>
</div>
</body>
</html>