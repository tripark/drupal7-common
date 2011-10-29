<?php
/**
 * Theme override functions for Drupal core's Seven theme.
 *
 * These should go in Seven's template.php file, but that would override core.
 * Instead, we'll make those theme functions available here. This is probably
 * not going to cause any problems, he said confidently.
 */

/**
 * Theme function override for theme_form_element().
 *
 * This should really go in template.php for the Seven theme,
 * but that's a core file and I don't want to touch it.
 * Putting the function here is a clever and terrible idea.
 */
function seven_form_element($variables) {
  $element = &$variables['element'];
  // This is also used in the installer, pre-database setup.
  $t = get_t();

  // This function is invoked as theme wrapper, but the rendered form element
  // may not necessarily have been processed by form_builder().
  $element += array(
    '#title_display' => 'before',
  );

  // Add element #id for #type 'item'.
  if (isset($element['#markup']) && !empty($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  // Add element's #type and #name as class to aid with JS/CSS selectors.
  $attributes['class'] = array('form-item');
  if (!empty($element['#type'])) {
    $attributes['class'][] = 'form-type-' . strtr($element['#type'], '_', '-');
  }
  if (!empty($element['#name'])) {
    $attributes['class'][] = 'form-item-' . strtr($element['#name'], array(' ' => '-', '_' => '-', '[' => '-', ']' => ''));
  }
  // Add a class for disabled elements to facilitate cross-browser styling.
  if (!empty($element['#attributes']['disabled'])) {
    $attributes['class'][] = 'form-disabled';
  }
  $output = '<div' . drupal_attributes($attributes) . '>' . "\n";

  // If #title is not set, we don't display any label or required marker.
  if (!isset($element['#title'])) {
    $element['#title_display'] = 'none';
  }
  $prefix = isset($element['#field_prefix']) ? '<span class="field-prefix">' . $element['#field_prefix'] . '</span> ' : '';
  $suffix = isset($element['#field_suffix']) ? ' <span class="field-suffix">' . $element['#field_suffix'] . '</span>' : '';

  // Process the description here instead so that we can place it between the
  // label and the element.
  if (!empty($element['#description'])) {
    $description = '<div class="description">' . $element['#description'] . "</div>\n";
  }
  else {
    $description = '';
  }

  switch ($element['#title_display']) {
    case 'before':
    case 'invisible':
      $output .= ' ' . theme('form_element_label', $variables);
      $output .= $description;
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;

    case 'after':
      $output .= ' ' . $prefix . $element['#children'] . $suffix;
      $output .= ' ' . theme('form_element_label', $variables) . "\n";
      $output .= $description;
      break;

    case 'none':
    case 'attribute':
      // Output no label and no required marker, only the children.
      $output .= $description;
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;
  }

  // We used to render the description here.
  $output .= "</div>\n";

  return $output;
}

function seven_text_format_wrapper($variables) {
  $element = $variables['element'];
  // The description normally renders at this level; we have to push it into the
  // 'value' child element and then re-render the whole wrapper. Total pain, but
  // I'm not sure how else to do this easily.
  if (!empty($element['#description'])) {
    $element['value']['#description'] = $element['#description'];
    unset($element['#description']);
    unset($element['value']['#printed']);
    unset($element['format']['#printed']);
    $element['#children'] = drupal_render_children($element);
  }
  $output = '<div class="text-format-wrapper">';
  $output .= $element['#children'];
  // The description used to be output here.
  $output .= "</div>\n";

  return $output;
}
