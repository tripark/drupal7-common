<?php
/**
 * @file
 * Add stylesheets that are only needed when Zen is the enabled theme.
 *
 * Don't do something this dumb in your sub-theme. Stylesheets should be added
 * using your sub-theme's .info file. If you desire styles that apply
 * conditionally, you can conditionally add a "body class" in the
 * preprocess_page function. For example, see how wireframes.css is handled in
 * zen_preprocess_page() and wireframes.css.
 */

/**
 * If the user is silly and enables Zen as the theme, manually add some stylesheets.
 */
function _zen_preprocess_html(&$vars, $hook) {
  $directory = drupal_get_path('theme', 'zen') . '/zen-internals/css/';

  // Add default styles.
  if (theme_get_setting('zen_layout') == 'zen-columns-fixed') {
    drupal_add_css($directory . 'zen-fixed.css', array('media' => 'all', 'weight' => 1));
  }
  else {
    drupal_add_css($directory . 'zen-liquid.css', array('media' => 'all', 'weight' => 1));
  }
  // Add print styles.
  drupal_add_css($directory . 'print.css', array('media' => 'print', 'weight' => 1));
  // Add conditional stylesheets.
  drupal_add_css($directory . 'ie.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE', '!IE' => FALSE), 'every_page' => TRUE));
  drupal_add_css($directory . 'ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 6', '!IE' => FALSE), 'every_page' => TRUE));
}
