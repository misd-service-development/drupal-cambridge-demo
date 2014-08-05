<?php

/**
 * Native implementation of session backend: use PHP native session handling.
 * Using it allows to use built-in extensions session handling, such as Redis
 * or Memcache extensions features, which are known to be really fast.
 */
class SessionProxy_Backend_Native
  extends SessionProxy_Backend_Base
{
  protected function getSessionUid() {
    return isset($_SESSION['uid']) ? $_SESSION['uid'] : NULL;
  }

  public function writeDisable() {}

  public function writeEnable() {}

  public function isWriteEnabled() {
    return TRUE;
  }

  public function regenerate() {
    global $user;

    if (!$this->sessionIsEmpty()) {
      $currentData = $_SESSION;
    }

    if ($this->started) {
      $this->started = FALSE;
      session_destroy();
      // Remove potential remaining cookie.
      setcookie($this->sessionName, FALSE);
    }

    $this->generateSessionIdentifier();

    if (isset($currentData) && !empty($currentData)) {
      $_SESSION = $currentData;
      $this->start();
    } else if ($user->uid) {
      $this->start();
      $_SESSION['uid'] = $user->uid;
    }

    if ($this->started) {
      // Some PHP versions won't reset correctly the cookie.
      $params = session_get_cookie_params();
      $expire = $params['lifetime'] ? REQUEST_TIME + $params['lifetime'] : 0;
      setcookie($this->sessionName, $this->sessionIdentifier, $expire, $params['path'], $params['domain'], $params['secure'], $params['httponly']);
    }

    $this->refreshAfterSessionChange();
  }

  public function destroyAllForUser($uid) {
    return;
  }
}
