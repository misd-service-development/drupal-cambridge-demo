<?php

abstract class SessionProxy_Storage_Base implements SessionProxy_Storage_Interface {
  /**
   * @var array
   */
  protected $options;

  /**
   * @var bool
   */
  protected $isHttps;

  /**
   * @var string
   */
  protected $sessionName;

  /**
   * @var string
   */
  protected $sessionNameUnsecure;

  /**
   * @var string
   */
  protected $lastReadSessionId = NULL;

  /**
   * @var string
   */
  protected $lastReadValueHash = NULL;

  /**
   * @var int
   */
  protected $uid = NULL;

  /**
   * @var int
   */
  protected $uidHasChanged = FALSE;

  /**
   * Call this function at read time, it will allow you to check if the session
   * has changed or not and write it accordingly.
   * 
   * @param string $sessionId
   * @param string $serializedData
   */
  protected function sessionDataSetHash($sessionId, $serializedData) {
    $this->lastReadSessionId = $sessionId;
    $this->lastReadValueHash = md5($serializedData);
  }

  /**
   * Does the session data has changed.
   * 
   * @param string $sessionId
   * @param string $serializedData
   */
  protected function sessionDataHasChanged($sessionId, $serializedData) {
    global $user;
    return $this->uidHasChanged || $this->lastReadSessionId != $sessionId || md5($serializedData) != $this->lastReadValueHash || (REQUEST_TIME - $user->timestamp > variable_get('session_write_interval', 180));
  }

  public function getSessionUid() {
    return $this->uid;
  }

  public function setSessionUid($uid) {
    $this->uidHasChanged = $this->uid != $uid;
    $this->uid = $uid;
  }

  public function __construct(array $options = array()) {
    global $is_https;
    $this->sessionName = session_name();
    $this->isHttps = $is_https;
    $this->options = $options;
  }
}
