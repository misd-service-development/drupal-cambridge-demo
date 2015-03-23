<?php

/**
 * Implements hook_rules_condition_info().
 */
function raven_rules_condition_info() {
  return array(
    'is_raven_user' => array(
      'label' => t('User is a Raven user'),
      'parameter' => array(
        'account' => array('type' => 'user', 'label' => t('User')),
      ),
      'group' => t('User'),
      'access callback' => 'rules_user_integration_access',
      'base' => 'is_raven_user',
    ),
  );
}
