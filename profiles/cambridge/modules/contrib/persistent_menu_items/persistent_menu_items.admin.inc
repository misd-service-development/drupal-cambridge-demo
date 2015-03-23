<?php

/**
 * Implements hook _settings_form().
 * Admin settings form for the persistent_menu_items module.
 */
function persistent_menu_items_settings_form($form, &$form_state) {

  // Let the user override the standard login pages (user, user/login, ?q=user, ?q=user/login)
  $form['persistent_menu_items_visible_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Default settings for Persistent Menu Items Option'),
    '#default_value' => variable_get('persistent_menu_items_visible_default', FALSE),
    '#description' => t('Use this field to set whether the Persistent Menu Items option for menu items is applied by default or not.'),
  );

  $nodes = node_type_get_names();
  $selected_content_types = variable_get('persistent_menu_items_content_type_settings', array());
  $form['persistent_menu_items_content_type_settings'] = array(
    '#type' => 'checkboxes',
    '#options' => $nodes,
    '#title' => 'Content types',
    '#default_value' => variable_get('persistent_menu_items_content_type_settings', array()),
    '#description' => 'Set the available of always available menu items on a per content type basis',
  );

  return system_settings_form($form);
}
