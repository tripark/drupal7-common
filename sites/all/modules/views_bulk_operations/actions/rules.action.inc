<?php

/**
 * Rules 2 actions are not integrated through Drupal actions, so no need
 * for this hook to do anything.
 */
function views_bulk_operations_rules_action_info() {
  return FALSE;
}

function views_bulk_operations_rules_action($entity, $context) {
  // If there was a config form, there's a rules_element.
  // If not, fallback to the component key.
  if (isset($context['rules_element'])) {
    $element = $context['rules_element'];
  }
  else {
   $element = rules_action('component_' . $context['component_key']);
  }
  $result = $element->execute($entity);
}

function views_bulk_operations_rules_action_form($form, &$form_state, $context) {
  require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'rules') . "/ui/ui.forms.inc";

  $element = rules_action('component_' . $context['action']['key']);
  $element->form($form, $form_state);

  // Remove the form element for the "entity" param. It will be passed in manually.
  $entity_key = $context['action']['callback arguments']['entity_key'];
  unset($form['parameter'][$entity_key]);

  return $form;
}

function views_bulk_operations_rules_action_validate($form, $form_state) {
  rules_ui_form_rules_config_validate($form, $form_state);
}

function views_bulk_operations_rules_action_submit($form, $form_state) {
  return array(
    'rules_element' => $form_state['rules_element'],
  );
}
