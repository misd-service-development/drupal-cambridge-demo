<?php

/**
 * @file
 * User session proxy to an advanced component-based interface.
 */

// Attempt a last chance autoloader register.
if (!class_exists('SessionProxy_Helper')) {
  define('_SESSION_AUTOLOAD_ROOT', dirname(__FILE__) . '/lib');
  if (class_exists('AutoloadEarly')) {
    // Allow usage of AutoloadEarly module for a PSR-0 autoloader.
    AutoloadEarly::getInstance()->registerNamespace('SessionProxy', _SESSION_AUTOLOAD_ROOT);
  } else {
    // Register a custom PSR-0 autoloader for our namespace.
    function _session_proxy_autoload($className) {
      $parts = explode('_', $className);
      if ('SessionProxy' === $parts[0]) {
        $filename = _SESSION_AUTOLOAD_ROOT . '/' . implode('/', $parts) . '.php';
        if (file_exists($filename)) {
          require_once $filename;
          return TRUE;
        }
      }
      return FALSE;
    }
    spl_autoload_register('_session_proxy_autoload');
  }
}

/**
 * Initializes the session handler.
 */
function drupal_session_initialize() {
  if (!variable_get('session_storage_force_default', FALSE)) {
    $class   = variable_get('session_storage_class', 'SessionProxy_Storage_Database');
    $backend = new SessionProxy_Backend_Default(new $class(variable_get('session_storage_options', array())));
  } else {
    $class   = variable_get('session_backend_class', 'SessionProxy_Backend_Native');
    $backend = new $class;
  }
  SessionProxy_Helper::getInstance()->setBackend($backend);
  // As of PHP 5.0.5 the write and close handlers are called after object
  // destruction and therefore cannot use objects or throw exceptions. The
  // object destructors can however use sessions.
  // See http://www.php.net/manual/en/function.session-set-save-handler.php
  drupal_register_shutdown_function('drupal_session_commit');
}

/**
 * Force session start.
 *
 * Traditionally, this is lazzy done when requesting information from the
 * session or using anyother session handling method.
 *
 * This function has never been used outside of update.php so we need to
 * rewrite it as well.
 */
function drupal_session_start() {
  $session = SessionProxy_Helper::getInstance()->getBackend();
  if (!$session->isStarted()) {
    $session->start();
  }
}

/**
 * Commits the current session, if necessary.
 *
 * If an anonymous user already have an empty session, destroy it.
 */
function drupal_session_commit() {
  return SessionProxy_Helper::getInstance()->getBackend()->commit();
}

/**
 * Returns whether a session has been started.
 */
function drupal_session_started($set = NULL) {
  if ($set) {
    throw new Exception("You cannot manually set the session state.");
  }
  return SessionProxy_Helper::getInstance()->getBackend()->isStarted();
}

/**
 * Called when an anonymous user becomes authenticated or vice-versa.
 *
 * @ingroup php_wrappers
 */
function drupal_session_regenerate() {
  return SessionProxy_Helper::getInstance()->getBackend()->regenerate();
}

/**
 * Ends a specific user's session(s).
 *
 * @param $uid
 *   User ID.
 */
function drupal_session_destroy_uid($uid) {
  SessionProxy_Helper::getInstance()->getBackend()->destroyAllForUser($uid);
}

/**
 * Determines whether to save session data of the current request.
 *
 * This function allows the caller to temporarily disable writing of
 * session data, should the request end while performing potentially
 * dangerous operations, such as manipulating the global $user object.
 * See http://drupal.org/node/218104 for usage.
 *
 * @param $status
 *   Disables writing of session data when FALSE, (re-)enables
 *   writing when TRUE.
 *
 * @return
 *   FALSE if writing session data has been disabled. Otherwise, TRUE.
 */
function drupal_save_session($status = NULL) {
  $helper = SessionProxy_Helper::getInstance()->getBackend();
  if (isset($status)) {
    if ($status) {
      $helper->writeEnable();
    }
    else {
      $helper->writeDisable();
    }
  }
  return $helper->isWriteEnabled();
}
