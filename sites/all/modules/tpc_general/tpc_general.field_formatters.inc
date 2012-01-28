<?php
/**
 * Implements hook_field_formatter_info().
 *
 * Add a text formatter similar to plain text, but with HTML line breaks.
 */
function tpc_general_field_formatter_info() {
  return array(
    'text_plain_linebreaks' => array(
      'label' => t('Plain text with linebreaks'),
      'field types' => array('text_long', 'text_with_summary'),
    ),
    'text_email' => array(
      'label' => t('Email address as "mailto:" link'),
      'field types' => array('text'),
    ),
  );
}

function tpc_general_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  switch ($display['type']) {
    case 'text_plain_linebreaks':
      foreach ($items as $delta => $item) {
        $element[$delta] = array('#markup' => nl2br(strip_tags($item['value'])));
      }
      break;
    case 'text_email':
      foreach ($items as $delta => $item) {
        $item['value'] = trim($item['value']);
        $markup = (valid_email_address($item['value'])) ? l($item['value'], 'mailto:' . $item['value']) : $item['value'];
        $element[$delta] = array('#markup' => $markup);
      }
      break;
  }
  return $element;
}