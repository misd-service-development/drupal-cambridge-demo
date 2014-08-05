<?php

class SessionProxy_Storage_Database
  extends SessionProxy_Storage_Base
{
  public function open() {
    return TRUE;
  }

  public function close() {
    return TRUE;
  }

  public function read($sessionId) {
    global $user;

    if (!isset($_COOKIE[$this->sessionName])) {
      // Avoid a useless database query.
      return '';
    }

    $data = db_query("SELECT u.uid, u.status, s.session FROM {users} u INNER JOIN {sessions} s ON u.uid = s.uid WHERE s.sid = :sid", array(':sid' => $sessionId))->fetchObject();

    if ($data && $data->uid > 0 && $data->status == 1) {
      $this->uid = $data->uid;
      $serializedData = $data->session;
    } else {
      $serializedData = '';
    }

    $this->sessionDataSetHash($sessionId, $serializedData);
    return $serializedData;
  }

  public function write($sessionId, $serializedData) {
    try {
      // For performance reasons, do not update the sessions table, unless
      // $_SESSION has changed or more than 180 has passed since the last update.
      if ($this->sessionDataHasChanged($sessionId, $serializedData)) {
        // Either ssid or sid or both will be added from $key below.
        $fields = array(
          'uid' => $this->uid,
          'cache' => 0,
          'hostname' => ip_address(),
          'session' => $serializedData,
          'timestamp' => REQUEST_TIME,
        );

        $key = array('sid' => $sessionId, 'ssid' => '');

        db_merge('sessions')
          ->key($key)
          ->fields($fields)
          ->execute();
      }

      return TRUE;
    } catch (Exception $exception) {
      // FIXME: This should never be here, a global try/catch should definitely
      // be done upper in the code.
      require_once DRUPAL_ROOT . '/includes/errors.inc';
      // If we are displaying errors, then do so with no possibility of a further
      // uncaught exception being thrown.
      if (error_displayable()) {
        print '<h1>Uncaught exception thrown in session handler.</h1>';
        print '<p>' . _drupal_render_exception_safe($exception) . '</p><hr />';
      }
      return FALSE;
    }
  }

  public function destroy($sessionId) {
    // Delete session data.
    db_delete('sessions')
      ->condition('sid', $sessionId)
      ->execute();
    $_SESSION = array();
    $this->setSessionUid(NULL);
    return TRUE;
  }

  public function gc($lifetime) {
    // Be sure to adjust 'php_value session.gc_maxlifetime' to a large enough
    // value. For example, if you want user sessions to stay in your database
    // for three weeks before deleting them, you need to set gc_maxlifetime
    // to '1814400'. At that value, only after a user doesn't log in after
    // three weeks (1814400 seconds) will his/her session be removed.
    db_delete('sessions')
      ->condition('timestamp', REQUEST_TIME - $lifetime, '<')
      ->execute();
    return TRUE;
  }

  public function handleHttps() {
    return FALSE;
  }

  public function destroyFor($index, $value) {
    if ('uid' == $index) {
      db_delete('sessions')
        ->condition($index, $value)
        ->execute();
    }
  }
}
