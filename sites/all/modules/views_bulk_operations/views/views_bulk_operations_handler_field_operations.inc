<?php

/**
* @file
* Views field handler. Contains all relevant VBO options and related logic.
* Implements the Views Form API.
*/

class views_bulk_operations_handler_field_operations extends views_handler_field {
  var $all_operations = array();
  var $revision = FALSE;

  function option_definition() {
    $options = parent::option_definition();

    $options['vbo'] = array(
      'contains' => array(
        'selected_operations' => array('default' => array()),
        'execution_type' => array('default' => VBO_EXECUTION_DIRECT),
        'display_type' => array('default' => 0),
        'skip_confirmation' => array('default' => FALSE),
        'display_result' => array('default' => TRUE),
        'merge_single_action' => array('default' => TRUE),
        'force_single' => array('default' => FALSE),
      ),
    );

    // Per-action settings.
    $this->populate_operations();
    foreach ($this->all_operations as $key => $operation) {
      $form_function = $operation['callback'] . '_settings_form_options';
      if (function_exists($form_function)) {
        $options['vbo']['contains'][$key] = array('default' => call_user_func($form_function));
      }
    }

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['vbo'] = array(
      '#type' => 'fieldset',
      '#title' => t('Views Bulk Operations'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $execution = array(
      VBO_EXECUTION_DIRECT => t('Invoke them directly'),
      VBO_EXECUTION_BATCH => t('Use Batch API'),
      VBO_EXECUTION_QUEUE => t('Use Drupal Queue'),
    );
    $form['vbo']['execution_type'] = array(
      '#type' => 'radios',
      '#title' => t('To execute operations'),
      '#default_value' => $this->options['vbo']['execution_type'],
      '#options' => $execution,
    );
    $form['vbo']['display_type'] = array(
      '#type' => 'radios',
      '#title' => t('Display operations as'),
      '#default_value' => $this->options['vbo']['display_type'],
      '#options' => array(
        t('Dropdown selectbox with Submit button'),
        t('Each action as a separate button'),
      ),
    );
    $form['vbo']['force_single'] = array(
      '#type' => 'checkbox',
      '#title' => t('Force single'),
      '#default_value' => $this->options['vbo']['force_single'],
      '#description' => t('Check this box to restrict selection to a single value.'),
    );
    $form['vbo']['skip_confirmation'] = array(
      '#type' => 'checkbox',
      '#title' => t('Skip confirmation step'),
      '#description' => t('Check this box to skip the confirmation page on this view and directly execute the operation.'),
      '#default_value' => $this->options['vbo']['skip_confirmation'],
    );
    $form['vbo']['display_result'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display processing result'),
      '#description' => t('Check this box to let Drupal display a message with the result of processing the selected objects.'),
      '#default_value' => $this->options['vbo']['display_result'],
    );
    $form['vbo']['merge_single_action'] = array(
      '#type' => 'checkbox',
      '#title' => t('Merge single action\'s form with the row selection view'),
      '#description' => t('In case only one action is selected *and* this action is configurable, display its action form along with the row selection view.'),
      '#default_value' => $this->options['vbo']['merge_single_action'],
    );
    $form['vbo']['selected_operations'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Selected operations'),
      '#options' => $this->get_operations_options(),
      '#default_value' => $this->options['vbo']['selected_operations'],
    );

    // Per-action settings form.
    $entity_type = $this->get_entity_type();
    foreach ($this->all_operations as $key => $operation) {
      if ($operation['type'] != 'entity' && $operation['type'] != $entity_type) {
        continue;
      }

      $form_function = $operation['callback'] . '_settings_form';
      if (function_exists($form_function)) {
        $form['vbo'][$key] = array(
          '#tree' => TRUE,
          '#type' => 'fieldset',
          '#title' => $operation['label'],
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
        );
        $form['vbo'][$key] += call_user_func($form_function, $this->options['vbo'][$key]);
      }
    }
  }

  function options_validate(&$form, &$form_state) {
    // Filter out the empty values.
    $form_state['values']['options']['vbo']['selected_operations'] = array_filter($form_state['values']['options']['vbo']['selected_operations']);

    foreach ($form_state['values']['options']['vbo']['selected_operations'] as $selected_operation) {
      if (!isset($form_state['values']['options']['vbo'][$selected_operation])) {
        continue;
      }

      $operation = $this->all_operations[$selected_operation];
      $form_function = $operation['callback'] . '_settings_form_validate';
      if (function_exists($form_function)) {
        $form_state['values']['options']['vbo'][$selected_operation]['_error_element_base'] = 'vbo][' . $selected_operation . '][';
        call_user_func($form_function, $form, array('values' => $form_state['values']['options']['vbo'][$selected_operation]));
      }
    }
  }

  function options_submit(&$form, &$form_state) {
    foreach ($form_state['values']['options']['vbo']['selected_operations'] as $selected_operation) {
      if (!isset($form_state['values']['options']['vbo'][$selected_operation])) {
        continue;
      }

      $operation = $this->all_operations[$selected_operation];
      $form_function = $operation['callback'] . '_settings_form_submit';
      if (function_exists($form_function)) {
        call_user_func($form_function, $form, array('values' => $form_state['values']['options']['vbo'][$selected_operation]));
      }
    }
  }

  /**
   * If the view is using a table style, provide a
   * placeholder for a "select all" checkbox.
   */
  function label() {
    if ($this->view->style_plugin instanceof views_plugin_style_table && !$this->options['vbo']['force_single']) {
      return '<!--views-bulk-operations-select-all-->';
    }
    else {
      return parent::label();
    }
  }

  function render($values) {
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  /**
   * Returns the form which replaces the placeholder from render().
   */
  function views_form(&$form, &$form_state) {
    // The view is empty, abort.
    if (empty($this->view->result)) {
      return;
    }

    // If there's a session variable on this view, pre-load the old values.
    if (isset($_SESSION['vbo_values'][$_GET['q']])) {
      $session_rows = $_SESSION['vbo_values'][$_GET['q']]['rows'];
      $default_rows = array(
        'selection' => $session_rows['selection'], 'select_all' => $session_rows['select_all']
      );
    }
    else {
      $default_rows = array('selection' => array(), 'select_all' => FALSE);
    }

    $form[$this->options['id']] = array(
      '#tree' => TRUE,
    );
    // At this point, the query has already been run, so we can access the results
    // in order to get the base key value (for example, nid for nodes).
    foreach ($this->view->result as $row_id => $row) {
      $entity_id = $this->get_value($row);

      if ($default_rows['select_all'] || isset($default_rows['selection'][$entity_id])) {
        $default_value = TRUE;
      }
      else {
        $default_value = FALSE;
      }
      if ($this->options['vbo']['force_single']) {
        $form[$this->options['id']][$row_id] = array(
          '#type' => 'radio',
          '#parents' => array($this->options['id']),
          '#return_value' => $entity_id,
        );
      }
      else {
        $form[$this->options['id']][$row_id] = array(
          '#type' => 'checkbox',
          '#return_value' => $entity_id,
          '#default_value' => $default_value,
        );
      }
    }
  }

  function get_selected_operations() {
    $selected = array();
    foreach (array_filter($this->options['vbo']['selected_operations']) as $key) {
      if (!isset($this->all_operations[$key])) {
        continue;
      }

      if (module_exists('actions_permissions')) {
        $perm = actions_permissions_get_perm($this->all_operations[$key]['label'], $this->all_operations[$key]['callback']);
        if (!user_access($perm)) {
          continue;
        }
      }
      if (!empty($this->all_operations[$key]['permissions'])) {
        foreach ($this->all_operations[$key]['permissions'] as $perm) {
          if (!user_access($perm)) {
            continue 2;
          }
        }
      }
      if ($this->all_operations[$key]['source'] == 'rules_action') {
        if (!rules_action('component_' . $key)->access()) {
          continue 2;
        }
      }
      $selected[$key] = $this->all_operations[$key]['label'];
    }
    return $selected;
  }

  function get_operation_info($key) {
    $operation = $this->all_operations[$key];
    if ($operation['type'] == 'entity') { // Actions that accept any entity type.
      $entity_type = $this->get_entity_type();
      $operation['type'] = $entity_type;
    }
    return $operation;
  }

  private function get_operations_options() {
    $options = array();
    $entity_type = $this->get_entity_type();
    foreach ($this->all_operations as $key => $operation) {
      if ($operation['type'] == 'entity') { // Actions that accept any entity type.
        $operation['type'] = $entity_type;
      }
      if ($operation['type'] == $entity_type) {
        $options[$key] = $operation['label'] .' ('. $key .')';
      }
    }
    return $options;
  }

  private function populate_operations() {
    // The operations have already been populated.
    if (!empty($this->all_operations)) {
      return;
    }

    $operations = array();

    // Merge in Drupal actions, including those declared by VBO itself.
    $action_operations = actions_list();
    foreach ($action_operations as $callback => $operation) {
      $key = isset($operation['key']) ? $operation['key'] : $callback;
      $operations[$key] = array(
        'key' => $key,
        'label' => isset($operation['label']) ? $operation['label'] : '',
        'callback' => $callback,
        'callback arguments' => isset($operation['parameters']) ? $operation['parameters'] : array(),
        'configurable' => isset($operation['configurable']) ? $operation['configurable'] : FALSE,
        'source' => 'action',
        'type' => $operation['type'],
        'aggregate' => isset($operation['aggregate']) ? (int)$operation['aggregate'] : VBO_AGGREGATE_FORBIDDEN,
        'behavior' => isset($operation['behavior']) ? $operation['behavior'] : array(),
        'access operation' => $this->get_access_op($operation),
        'permissions' => isset($operation['permissions']) ? $operation['permissions'] : NULL,
      );
    }

    // Merge in actions provided by Rules 2.
    if (module_exists('rules')) {
      $entity_info = entity_get_info();
      $entity_types = array_keys($entity_info);
      $supported_types = array('entity');
      foreach ($entity_types as $type) {
        $supported_types[] = $type;
      }
      foreach (rules_get_components(FALSE, 'action') as $component_key => $component) {
        $parameter_info = $component->parameterInfo();
        $first_parameter = reset($parameter_info);
        $parameter_keys = array_keys($parameter_info);
        $entity_key = reset($parameter_keys);
        // If the first param is not an entity type, skip the component.
        if (!in_array($first_parameter['type'], $supported_types)) {
          continue;
        }

        $operations[$component_key] = array(
          'key' => $component_key,
          'label' => $component->label,
          'callback' => 'views_bulk_operations_rules_action',
          'callback arguments' => array('component_key' => $component_key, 'entity_key' => $entity_key),
          'configurable' => count($parameter_info) > 1,
          'source' => 'rules_action',
          'type' => $first_parameter['type'],
          'aggregate' => VBO_AGGREGATE_FORBIDDEN,
          'access operation' => VBO_ACCESS_OP_UPDATE, // assume edit by default.
          'permissions' => NULL,
        );
      }
    }

    uasort($operations, create_function('$a, $b', 'return strcasecmp($a["label"], $b["label"]);'));
    $this->all_operations = $operations;
  }

  private function get_access_op($operation) {
    // Assume edit by default.
    if (!isset($operation['behavior'])) {
      $operation['behavior'] = array('changes_property');
    }

    $access_op = 0;
    if (in_array('views_property', $operation['behavior'])) {
      $access_op |= VBO_ACCESS_OP_VIEW;
    }
    if (in_array('changes_property', $operation['behavior'])) {
      $access_op |= VBO_ACCESS_OP_UPDATE;
    }
    if (in_array('creates_property', $operation['behavior'])) {
      $access_op |= VBO_ACCESS_OP_CREATE;
    }
    if (in_array('deletes_property', $operation['behavior'])) {
      $access_op |= VBO_ACCESS_OP_DELETE;
    }
    return $access_op;
  }

  function get_operation_settings($operation) {
    return (isset($this->options[$operation['key']]) ? $this->options[$operation['key']] : NULL);
  }

  /**
   * Determine the base table of the VBO field, and then use it to determine
   * the entity type that VBO is operating on.
   */
  function get_entity_type() {
    $base_table = $this->view->base_table;

    // If the current field is under a relationship you can't be sure that the
    // base table of the view is the base table of the current field.
    // For example a field from a node author on a node view does have users as base table.
    if (!empty($this->options['relationship']) && $this->options['relationship'] != 'none') {
      $relationships = $this->view->display_handler->get_option('relationships');
      $options = $relationships[$this->options['relationship']];
      $data = views_fetch_data($options['table']);
      $base_table = $data[$options['field']]['relationship']['base'];
    }
    // The base table is now known, use it to determine the entity type.
    foreach (entity_get_info() as $entity_type => $info) {
      if ($info['base table'] == $base_table) {
        return $entity_type;
      }
      elseif (isset($info['revision table']) && $info['revision table'] == $base_table) {
        $this->revision = TRUE;
        return $entity_type;
      }
    }
    // This should never happen.
    _views_bulk_operations_report_error("Could not determine the entity type for VBO field on views base table %table", array('%table' => $base_table));
    return FALSE;
  }
}
