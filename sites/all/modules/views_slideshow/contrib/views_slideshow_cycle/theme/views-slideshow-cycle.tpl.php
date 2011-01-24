<?php
// $Id: views-slideshow-cycle.tpl.php,v 1.1.2.2 2010/12/29 00:49:13 redndahead Exp $

/**
 * @file
 * Views Slideshow: Single Frame template file.
 */
?>

<div class="skin-<?php print $settings['views_slideshow_cycle']['skin']; ?>">
  <?php if (isset($top_widget_rendered)): ?>
    <div class="views-slideshow-controls-top clear-block">
      <?php print $top_widget_rendered; ?>
    </div>
  <?php endif; ?>
  
  <?php print $slideshow; ?>
  
  <?php if (isset($bottom_widget_rendered)): ?>
    <div class="views-slideshow-controls-bottom clear-block">
      <?php print $bottom_widget_rendered; ?>
    </div>
  <?php endif; ?>
</div>
