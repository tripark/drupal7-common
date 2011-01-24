<?php
/**
 * @file
 * Add custom menus to the active trail; required to fix Drupal 7.0 core.
 */

/**
 * Implements hook_menu_insert().
 */
function menu_block_menu_insert($menu) {
  _menu_block_add($menu);
}

/**
 * Implements hook_menu_update().
 */
function menu_block_menu_update($menu) {
  _menu_block_add($menu);
}

/**
 * Implements hook_menu_delete().
 */
function menu_block_menu_delete($menu) {
  $active_menus = variable_get('menu_default_active_menus', array_keys(menu_list_system_menus()));
  if (in_array($menu['menu_name'], $active_menus)) {
    $key = array_search($menu['menu_name'], $active_menus);
    unset($active_menus[$key]);
    variable_set('menu_default_active_menus', $active_menus);
  }
}

/**
 * Add a custom menu to the active trail.
 *
 * @param array $menu
 *   A menu item to add to the active trail.
 */
function _menu_block_add($menu) {
  // Make sure the menu is present in the active menus variable so that its
  // items may appear in the menu active trail.
  // @see menu_set_active_menu_names()
  $active_menus = variable_get('menu_default_active_menus', array_keys(menu_list_system_menus()));
  if (!in_array($menu['menu_name'], $active_menus)) {
    $active_menus[] = $menu['menu_name'];
    variable_set('menu_default_active_menus', $active_menus);
  }
}
