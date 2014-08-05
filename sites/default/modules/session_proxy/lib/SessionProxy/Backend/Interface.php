<?php

/**
 * Drupal specific session handling additions.
 */
interface SessionProxy_Backend_Interface {
  /**
   * Enable session writing explicitely.
   * 
   * This is not fail-safe, if no storage is provided by a PHP extension you
   * cannot disable session writing.
   */
  public function writeDisable();

  /**
   * Disable session writing explicitely.
   */
  public function writeEnable();

  /**
   * Is session write enabled.
   * 
   * @return bool
   */
  public function isWriteEnabled();

  /**
   * Does this instance handle HTTPS diverging session name.
   * 
   * @return bool
   */
  public function handleHttps();

  /**
   * Start session.
   */
  public function start();

  /**
   * Is session started.
   * 
   * @return bool
   */
  public function isStarted();

  /**
   * Called during shutdown hook time, this allows you to perform additional
   * operations outside of the core PHP session handling at the end of request.
   */
  public function commit();

  /**
   * Regenerate the current session.
   */
  public function regenerate();

  /**
   * Destroy all session for given user identifier.
   * 
   * This might be silent if storage is handled by a PHP extension or if the
   * storage backend does not implement conditional cleaning.
   * 
   * @param int $uid
   */
  public function destroyAllForUser($uid);
}
