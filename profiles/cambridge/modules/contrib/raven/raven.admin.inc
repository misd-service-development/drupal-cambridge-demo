<?php

/**
 * @file
 * Administration form for the Raven authentication module.
 */

/**
 * Implements hook_form().
 */
function raven_settings_form($form, &$form_state) {
  global $base_url;

  $user1 = user_load(1);

  // Let the user override the standard login pages (user, user/login, ?q=user, ?q=user/login)
  $form['raven_login_override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Raven login override'),
    '#default_value' => variable_get('raven_login_override', FALSE),
    '#description' => t('Override the normal login paths so that users can only log in using Raven.'),
  );

  $form['raven_backdoor_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable non-Raven backdoor login'),
    '#default_value' => variable_get('raven_backdoor_login', TRUE),
    '#states' => array(
      // Hide the settings when the cancel notify checkbox is disabled.
      'invisible' => array(
        ':input[name="raven_login_override"]' => array('checked' => FALSE),
      ),
    ),
    '#description' => t('Open a hidden path (\'@raven_backdoor_login_path\') to still allow normal Drupal logins. This mean that site-created users such as \'@user1\' will still be able to log in.<br /><i>Warning: Disabling this without having an administrator able to log in with Raven will lock you out of your site.</i>', array(
      '@raven_backdoor_login_path' => 'user/backdoor/login',
      // user 1 should always exist, but just in case
      '@user1' => $user1 ? $user1->name : 'admin',
    )),
  );

  // let the user override the administrator approval process for Raven auth accounts
  $form['raven_override_administrator_approval'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable administrator approval override'),
    '#default_value' => variable_get('raven_override_administrator_approval', FALSE),
    '#description' => t('Override the Drupal administrator approval settings for users successfully authenticated by Raven.'),
  );

  // Allow Raven for Life accounts to be authenticated
  $form['raven_allow_raven_for_life'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow <a href="@url">Raven for Life accounts</a> to be authenticated', array('@url' => 'http://www.ucs.cam.ac.uk/accounts/ravenleaving')),
    '#default_value' => variable_get('raven_allow_raven_for_life', FALSE),
  );

  // Set which Raven service to use
  $form['raven_service'] = array(
    '#type' => 'radios',
    '#title' => t('Raven service'),
    '#options' => array('live' => t('Live'), 'demo' => t('Demo')),
    '#default_value' => variable_get('raven_service', 'live'),
    '#description' => t('The <a href="@url">demo Raven service</a> can be useful for development, especially when not on the University network.<br /><i>Warning: the demo service must not be used in production, otherwise your site will be compromised.</i>', array('@url' => 'https://demo.raven.cam.ac.uk/')),
  );

  // Log users out when closing the browser?
  $form['raven_logout_on_browser_close'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log out Raven users when closing the browser'),
    '#default_value' => variable_get('raven_logout_on_browser_close', TRUE),
    '#description' => t('Drupal, by default, does not log the user out when closing the browser (the session is kept active for over 23 days). Raven applications, however, are usually expected to do so. This option logs out users who have logged in through Raven when the browser is closed.<br /><i>Note: Enabling this will not affect existing sessions, users will need to log out manually first.</i>'),
  );

  // Site description for Raven Login page
  $form['raven_website_description'] = array(
    '#type' => 'textfield',
    '#title' => t('Your website description'),
    '#default_value' => variable_get('raven_website_description', NULL),
    // Get custom description, otherwise the site name, otherwise the site url
    '#description' => t('When Raven prompts the user to log in it will display a message with the text <i>\'[...] This resource calls itself \'SITE DESCRIPTION\' and [...]</i>, where SITE DESCRIPTION is specified here. If left blank, the site name will be used (currently \'@sitename\').', array('@sitename' => variable_get('site_name', $base_url))),
  );

  // Site redirect if a login fails
  $form['raven_login_fail_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('Login failure redirect'),
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', FALSE) ? '' : '?q='),
    '#default_value' => variable_get('raven_login_fail_redirect', NULL),
    '#description' => t('If a login fails, they will be redirected to this page.'),
    '#required' => FALSE,
  );

  $form['#validate'][] = 'raven_settings_validate';

  return system_settings_form($form);
}

/**
 * Validator for the raven_settings_form() form.
 */
function raven_settings_validate($form, &$form_state) {
  // Get the normal path of login failure redirect.
  if (!empty($form_state['values']['raven_login_fail_redirect'])) {
    form_set_value($form['raven_login_fail_redirect'], drupal_get_normal_path($form_state['values']['raven_login_fail_redirect']), $form_state);
  }
  // Validate login failure redirect path.
  if (!empty($form_state['values']['raven_login_fail_redirect']) && !drupal_valid_path($form_state['values']['raven_login_fail_redirect'])) {
    form_set_error('raven_login_fail_redirect', t("The path '%path' is either invalid or you do not have access to it.", array('%path' => $form_state['values']['raven_login_fail_redirect'])));
  }
}
