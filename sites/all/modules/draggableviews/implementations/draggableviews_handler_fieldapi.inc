<?php

/**
 * @file
 * The default implementation for draggableviews.
 */

/*
 * Default Implementation with FieldAPI fields
 */
class draggableviews_handler_fieldapi extends draggableviews_handler {
  function init($field_name, &$view) {
    parent::init($field_name, $view);
    $this->field           = $view->field[$field_name];
  }

  function save($nid, $value) {
    $node = node_load($nid);

    $field_real_name = $this->field->real_field;
    // Get field.
    if (!isset($node->$field_real_name)) {
      // This note doesn't this field.
      return;
    }
    $field = &$node->$field_real_name;

    if (!isset($value)) {
      // Sometimes there is no value available (e.g. a root node without a parent).
      $value = 0;
    }

    $field_type = $this->field->options['type'];
    $language = $node->language;

    // Differ between certain field types.
    switch ($field_type) {
      case 'nodereference':
        $field[$language][0]['nid'] = $value;
        break;

      default:
      case 'number_integer':
        $field[$language][0]['value'] = $value;
        break;
    }

    node_save($node);
  }
}
