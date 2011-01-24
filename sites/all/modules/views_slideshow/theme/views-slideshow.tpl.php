<?php
// $Id: views-slideshow.tpl.php,v 1.1.2.1 2010/12/15 20:23:22 redndahead Exp $

/**
 * @file
 * Default views template for displaying a slideshow.
 *
 * - $view: The View object.
 * - $options: Settings for the active style.
 * - $rows: The rows output from the View.
 * - $title: The title of this group of rows. May be empty.
 *
 * @ingroup views_templates
 */
?>

<div class="skin-<?php print $skin; ?>">
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
