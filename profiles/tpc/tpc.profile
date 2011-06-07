<?php

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Allows the profile to alter the site configuration form.
 */
function tpc_form_install_configure_form_alter(&$form, $form_state) {
  $tpc_email = 'webmaster@triangleparkcreative.com';
  // Pre-populate the site name with the server name.
  $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
  $form['site_information']['site_mail']['#default_value'] = $tpc_email;
  $form['admin_account']['account']['name']['#default_value'] = 'admin';
  $form['admin_account']['account']['mail']['#default_value'] = $tpc_email;
  $form['server_settings']['site_default_country']['#default_value'] = 'US';
}
