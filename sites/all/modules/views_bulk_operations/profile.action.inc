<?php

function views_bulk_operations_profile_action_info() {
  if (!module_exists('profile')) return array();
  return array('views_bulk_operations_profile_action' => array(
    'type' => 'user',
    'label' => t('Modify profile fields'),
    'configurable' => TRUE,
  ));
}

function views_bulk_operations_profile_action_theme() {
  return array(
    'views_bulk_operations_profile_action_form' => array(
      'arguments' => array('form' => NULL),
    ),
  );
}

function views_bulk_operations_profile_action_form($context) {
  drupal_add_js(drupal_get_path('module', 'views_bulk_operations') . '/profile.action.js');

  $form = array();
  foreach (profile_categories() as $category) {
    $form += profile_form_profile($context, NULL, $category['name']);
    $fields = _profile_get_fields($category['name']);
    foreach ($fields as $field) {
      $form[$category['name']][$field->name]['#required'] = FALSE;
      $form[$field->name . '_check'] = array(
        '#type' => 'checkbox', 
        '#attributes' => array(
          'class' => 'profile-action-toggler', 
          'onchange' => 'Drupal.vbo.profileAction.updateField(this, true)',
        ),
      );
    }
  }
  $form['#theme'] = 'views_bulk_operations_profile_action_form';
  return $form;
}

function theme_views_bulk_operations_profile_action_form(&$form) {
  $header = array(theme('table_select_header_cell'), t('Category'), t('Field'));
  $rows = array();
  foreach (profile_categories() as $category) {
    $fields = _profile_get_fields($category['name']);
    foreach ($fields as $field) {
      $row = array();
      $row[] = drupal_render($form[$field->name . '_check']);
      $row[] = array('data' => $category['name'], 'rowspan' => 1, 'valign' => 'center');
      $row[] = drupal_render($form[$category['name']][$field->name]);
      $rows[] = $row;
    }
    drupal_render($form[$category['name']]); // render the fieldsets but don't add them to the theme
  }

  $output = theme('table', $header, $rows);
  $output .= drupal_render($form);
  return $output;
}

function views_bulk_operations_profile_action_validate($form, $form_state) {
  $at_least_one = FALSE;
  foreach (profile_categories() as $category) {
    $result = _profile_get_fields($category['name']);
    foreach ($fields as $field) {
      if (!$form_state['values'][$field->name . '_check']) continue;
      $at_least_one = TRUE;
      if ($form_state['values'][$field->name]) {
        if ($field->type == 'url') {
          if (!valid_url($form_state['values'][$field->name], TRUE)) {
            form_set_error($field->name, t('The value provided for %field is not a valid URL.', array('%field' => $field->title)));
          }
        }
      }
//      else if ($field->required && !user_access('administer users')) {
//        form_set_error($field->name, t('The field %field is required.', array('%field' => $field->title)));
//      }
    }
  }
  if (!$at_least_one) {
    form_set_error('', t('You must select at least one field to modify.'));
  }
}

function views_bulk_operations_profile_action_submit($form, $form_state) {
  $return = array();
  foreach (profile_categories() as $category) {
    $fields = _profile_get_fields($category['name']);
    foreach ($fields as $field) {
      $return[$field->name .'_check'] = $form_state['values'][$field->name .'_check'];
      $return[$field->name] = $form_state['values'][$field->name];
    }
  }
  return $return;
}

function views_bulk_operations_profile_action(&$user, $context) {
  foreach (profile_categories() as $category) {
    _views_bulk_operations_profile_action_save($context, $user, $category['name']);
  }
}

function _views_bulk_operations_profile_action_save(&$edit, &$user, $category, $register = FALSE) {
  $result = _profile_get_fields($category, $register);
  foreach ($result as $field) {
    if (!$edit[$field->name .'_check']) continue; // skip if the user didn't check that field

    if (_profile_field_serialize($field->type)) {
      $edit[$field->name] = serialize($edit[$field->name]);
    }
    // @todo Use db_merge() here?
    db_delete('profile_values')
      ->condition('fid', $field->fid)
      ->condition('uid', $user->uid)
      ->execute();
    db_insert('profile_values')
      ->fields(array(
        'fid' => $field->fid,
        'uid' => $user->uid,
        'value' => $edit[$field->name],
      ))
      ->execute();
    // Mark field as handled (prevents saving to user->data).
    $edit[$field->name] = NULL;
  }
}

