<?php
/**
 * @file
 * Contains infrequently used theme registry build functions.
 */

/**
 * Implements HOOK_theme().
 *
 * We are simply using this hook as a convenient time to do some related work.
 */
function _zen_theme(&$existing, $type, $theme, $path) {
  // Force the region hook to search for region__ suggestions.
  if (empty($exisiting['region']['pattern'])) {
    $exisiting['region']['pattern'] = 'region__';
  }

  // Compute the conditional stylesheets.
  if (!module_exists('conditional_styles')) {
    include_once './' . drupal_get_path('theme', 'zen') . '/zen-internals/template.conditional-styles.inc';
    // _conditional_styles_theme() only needs to be run once.
    if ($theme == 'zen') {
      _conditional_styles_theme($existing, $type, $theme, $path);
    }
  }

  // If we are auto-rebuilding the theme registry, warn about the feature.
  // Always display the warning in the admin section, otherwise limit to three
  // warnings per hour.
  if (function_exists('user_access') && user_access('administer site configuration') && theme_get_setting('zen_rebuild_registry') && $theme == $GLOBALS['theme'] && (arg(0) == 'admin' || flood_is_allowed($GLOBALS['theme'] . '_rebuild_registry_warning', 3))) {
    flood_register_event($GLOBALS['theme'] . '_rebuild_registry_warning');
    drupal_set_message(t('For easier theme development, the theme registry is being rebuilt on every page request. It is <em>extremely</em> important to <a href="!link">turn off this feature</a> on production websites.', array('!link' => url('admin/appearance/settings/' . $GLOBALS['theme']))), 'warning', FALSE);
  }

  // Return nothing.
  return array();
}
