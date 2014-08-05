<?php

/**
 * Static tool container.
 *
 * Also acts as a container for the current backend being used, implemented
 * using the singleton pattern. This provides a working wrapper for actual
 * procedural core code.
 */
class SessionProxy_Helper {
  /**
   * @var SessionProxy_Helper
   */
  private static $instance;

  /**
   * @return SessionProxy_Helper
   */
  public static function getInstance() {
    if (!isset(self::$instance)) {
      self::$instance = new self;
    }
    return self::$instance;
  }

  /**
   * Deletes the session cookie.
   *
   * @param string $name
   *   Name of session cookie to delete.
   * @param bool $force_insecure
   *   Force cookie to be insecure.
   */
  public static function deleteSessionCookie($name, $force_insecure = FALSE) {
    if (isset($_COOKIE[$name])) {
      $params = session_get_cookie_params();
      setcookie($name, '', REQUEST_TIME - 3600, $params['path'], $params['domain'], !$force_insecure && $params['secure'], $params['httponly']);
      unset($_COOKIE[$name]);
    }
  }

  /**
   * @var SessionProxy_Backend_Interface
   */
  protected $backend;

  /**
   * Tell if backend has been set.
   * 
   * @return bool
   */
  public static function hasBackend() {
    return isset($this->backend);
  }

  /**
   * Set backend.
   * 
   * @param SessionProxy_Backend_Interface $backend
   * 
   * @throws Exception
   *   If already set.
   */
  public function setBackend(SessionProxy_Backend_Interface $backend) {
    if (isset($this->backend) && $this->backend->isStarted()) {
      throw new Exception("Cannot replace session backend at runtime.");
    }
    $this->backend = $backend;
  }

  /**
   * Get backend.
   * 
   * @return SessionProxy_Backend_Interface
   */
  public function getBackend() {
    if (!isset($this->backend)) {
      throw new Exception("No default implementation exists.");
    }
    return $this->backend;
  }
}
