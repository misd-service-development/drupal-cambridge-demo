<?php

/**
 * Storage implementation must be able to index all session using the owner
 * user identifier: this complexify the session handling but allow aggresive
 * optimizations.
 */
interface SessionProxy_Storage_Interface {

  public function open();

  public function close();

  public function read($sessionId);

  public function write($sessionId, $serializedData);

  public function destroy($sessionId);

  public function gc($lifetime);

  /**
   * Get the logged in user identifier, if any. In all cases, this will be
   * called after read();
   * 
   * @return int
   *   Valid user identifier or NULL.
   */
  public function getSessionUid();

  /**
   * When session has been regenerated, inform the storage backend that further
   * session writing will be done for a new user identenfier.
   * 
   * @param int $uid
   */
  public function setSessionUid($uid);

  /**
   * Destroy all known sessions using the given conditions.
   * 
   * This function implementation is optional, garbage collector should be
   * enough for session destruction in most cases.
   * 
   * Implement this function will only improve performances, it is important
   * that it should remain silent if you cannot handle the given parameters.
   * 
   * @param string $index
   *   Potential session index key, such as 'uid'.
   * @param mixed $value
   *   Potential session index value, such as an integer for 'uid'.
   */
  public function destroyFor($index, $value);
}
