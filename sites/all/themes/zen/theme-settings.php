<?php
/**
 * Implements hook_form_system_theme_settings_alter() function.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */
function zen_form_system_theme_settings_alter(&$form, $form_state) {

  // Add the form's CSS
  drupal_add_css(drupal_get_path('theme', 'zen') . '/zen-internals/css/theme-settings.css', 'theme');

  // Add javascript to show/hide optional settings
  drupal_add_js(drupal_get_path('theme', 'zen') . '/zen-internals/js/theme-settings.js', 'theme');

  /*
   * Create the form using Forms API
   */
  $form['zen-div-opening'] = array(
    '#value'         => '<div id="zen-settings">',
  );

  $form['breadcrumb'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Breadcrumb settings'),
    '#attributes'    => array('id' => 'zen-breadcrumb'),
  );
  $form['breadcrumb']['zen_breadcrumb'] = array(
    '#type'          => 'select',
    '#title'         => t('Display breadcrumb'),
    '#default_value' => theme_get_setting('zen_breadcrumb'),
    '#options'       => array(
                          'yes'   => t('Yes'),
                          'admin' => t('Only in admin section'),
                          'no'    => t('No'),
                        ),
  );
  $form['breadcrumb']['zen_breadcrumb_separator'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Breadcrumb separator'),
    '#description'   => t('Text only. Don’t forget to include spaces.'),
    '#default_value' => theme_get_setting('zen_breadcrumb_separator'),
    '#size'          => 5,
    '#maxlength'     => 10,
    '#prefix'        => '<div id="div-zen-breadcrumb-collapse">', // jquery hook to show/hide optional widgets
  );
  $form['breadcrumb']['zen_breadcrumb_home'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Show home page link in breadcrumb'),
    '#default_value' => theme_get_setting('zen_breadcrumb_home'),
  );
  $form['breadcrumb']['zen_breadcrumb_trailing'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Append a separator to the end of the breadcrumb'),
    '#default_value' => theme_get_setting('zen_breadcrumb_trailing'),
    '#description'   => t('Useful when the breadcrumb is placed just before the title.'),
  );
  $form['breadcrumb']['zen_breadcrumb_title'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Append the content title to the end of the breadcrumb'),
    '#default_value' => theme_get_setting('zen_breadcrumb_title'),
    '#description'   => t('Useful when the breadcrumb is not placed just before the title.'),
    '#suffix'        => '</div>', // #div-zen-breadcrumb
  );

  $form['themedev'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Theme development settings'),
    '#attributes'    => array('id' => 'zen-themedev'),
  );
  $form['themedev']['zen_rebuild_registry'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Rebuild theme registry on every page.'),
    '#default_value' => theme_get_setting('zen_rebuild_registry'),
    '#description'   => t('During theme development, it can be very useful to continuously <a href="!link">rebuild the theme registry</a>. WARNING: this is a huge performance penalty and must be turned off on production websites.', array('!link' => 'http://drupal.org/node/173880#theme-registry')),
    '#prefix'        => '<div id="div-zen-registry"><strong>' . t('Theme registry:') . '</strong>',
    '#suffix'        => '</div>',
  );
  $form['themedev']['zen_layout'] = array(
    '#type'          => 'radios',
    '#title'         => t('Layout method'),
    '#options'       => array(
                          'zen-columns-liquid' => t('Liquid layout') . ' <small>(layout-liquid.css)</small>',
                          'zen-columns-fixed' => t('Fixed layout') . ' <small>(layout-fixed.css)</small>',
                        ),
    '#default_value' => theme_get_setting('zen_layout'),
  );
  $form['themedev']['zen_wireframes'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Display borders around main layout elements'),
    '#default_value' => theme_get_setting('zen_wireframes'),
    '#description'   => t('<a href="!link">Wireframes</a> are useful when prototyping a website.', array('!link' => 'http://www.boxesandarrows.com/view/html_wireframes_and_prototypes_all_gain_and_no_pain')),
    '#prefix'        => '<div id="div-zen-wireframes"><strong>' . t('Wireframes:') . '</strong>',
    '#suffix'        => '</div>',
  );

  $form['zen-div-closing'] = array(
    '#value'         => '</div>',
  );
}
