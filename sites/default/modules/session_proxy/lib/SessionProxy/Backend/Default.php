<?php

/**
 * Default implementation relies on a custom storage engine.
 */
class SessionProxy_Backend_Default extends SessionProxy_Backend_Base {
  /**
   * @var SessionProxy_Storage_Interface
   */
  protected $storage;

  /**
   * Session write proxy that will allow us to disable session writing if
   * we are master of the storage.
   */
  public function writeProxy($sessionId, $serializedData) {
    if ($this->isWriteEnabled()) {
      $this->storage->write($sessionId, $serializedData);
    }
  }

  /**
   * Session destroy will require us to update the current logged in user.
   */
  /*
  protected function destroyProxy($sessionId) {
    $this->storage->destroy($sessionId);

    // Unset the session cookies.
    SessionProxy_Helper::deleteSessionCookie($this->sessionName);

    $this->refreshAfterSessionChange();
  }
   */

  /**
   * @var int
   */
  protected $uid;
  
  protected function getSessionUid() {
    return $this->storage->getSessionUid();
  }

  /**
   * @var bool
   */
  protected $doWrite = TRUE;

  public function writeDisable() {
    $this->doWrite = FALSE;
  }

  public function writeEnable() {
    $this->doWrite = TRUE;
  }

  public function isWriteEnabled() {
    return $this->doWrite;
  }

  public function handleHttps() {
    return variable_get('https', FALSE) && $storage->handleHttps();
  }

  protected function sessionSetHandlers() {
    if (FALSE === session_set_save_handler(
      array($this->storage, 'open'),
      array($this->storage, 'close'),
      array($this->storage, 'read'),
      array($this, 'writeProxy'),
      array($this->storage, 'destroy'),
      array($this->storage, 'gc')
    )) {
      throw new Exception(__METHOD__ . ': unable to register the session handler');
    }
  }

  public function regenerate() {
    global $user;

    // FIXME: Default backend will erase current user at session read time.
    // We need to get it out of there for good and avoid this ugly hack.
    if ($user->uid) {
      $account = $user;
    }

    if (!$this->sessionIsEmpty()) {
      $currentData = $_SESSION;
    }

    if ($this->started) {
      $this->started = FALSE;
      session_destroy();
      // PHP 5.2 bug: When session_destroy() is called, you need to reset
      // session handlers, else PHP loose them. See
      // http://php.net/manual/en/function.session-set-save-handler.php#22194
      $this->sessionSetHandlers();
      // Remove potential remaining cookie.
      setcookie($this->sessionName, FALSE);
    }

    $this->generateSessionIdentifier();

    if (isset($currentData) && !empty($currentData)) {
      $_SESSION = $currentData;
    }

    $this->start();

    // See comment above.
    if (isset($account)) {
      $user = $account;
    }

    if ($this->started) {
      // Some PHP versions won't reset correctly the cookie.
      $params = session_get_cookie_params();
      $expire = $params['lifetime'] ? REQUEST_TIME + $params['lifetime'] : 0;
      setcookie($this->sessionName, $this->sessionIdentifier, $expire, $params['path'], $params['domain'], $params['secure'], $params['httponly']);
    }

    // On session regenerate, the storage UID is set to the one read during
    // the first session read attempt: we need to advertise the backend that
    // future session write will be linked to a new UID.
    $this->storage->setSessionUid($user->uid);

    $this->refreshAfterSessionChange();
  }

  public function destroyAllForUser($uid) {
    $this->storage->destroyFor(array('uid', $uid));
  }

  public function __construct(SessionProxy_Storage_Interface $storage) {
    $this->storage = $storage;
    $this->sessionSetHandlers();
    parent::__construct();
  }
}
