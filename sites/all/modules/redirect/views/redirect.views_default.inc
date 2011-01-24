<?php
// $Id: redirect.views_default.inc,v 1.2 2010/08/26 05:25:34 davereid Exp $

/**
 * Implements hook_views_default_views().
 */
function redirect_views_default_views() {
  $views = array();
  $files = file_scan_directory(drupal_get_path('module', 'redirect') . '/views', '/\.view$/');
  foreach ($files as $path => $file) {
    require $path;
    if (isset($view)) {
      $views[$view->name] = $view;
    }
  }
  return $views;
}
