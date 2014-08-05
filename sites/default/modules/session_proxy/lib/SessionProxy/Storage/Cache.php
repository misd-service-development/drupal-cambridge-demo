<?php

/**
 * Storage implementation based upon a specific cache backend.
 */
class SessionProxy_Storage_Cache
  extends SessionProxy_Storage_Base
{
  /**
   * @var DrupalCacheInterface
   */
  protected $cacheBackend;

  /**
   * @var string
   */
  protected $prefix = 'session_';

  /**
   * Get session CID.
   */
  protected function getCid($sessionId) {
    return $this->prefix . $sessionId;
  }

  public function open() {
    return TRUE;
  }

  public function close() {
    return TRUE;
  }

  public function read($sessionId) {
    global $user;

    if (!isset($_COOKIE[$this->sessionName])) {
      $user = drupal_anonymous_user();
      return '';
    }

    $cid = $this->getCid($sessionId);

    if ($cached = $this->cacheBackend->get($cid)) {
      $data = $cached->data;

      if ($data && $data->uid > 0) {
        $this->uid = $data->uid;
        $serializedData = $data->session;
      } else {
        $serializedData = '';
      }

      $this->sessionDataSetHash($sessionId, $serializedData);
      return $serializedData;
    } else {
      return NULL;
    }
  }

  public function write($sessionId, $serializedData) {
    global $user;

    try {
      // For performance reasons, do not update the sessions table, unless
      // $_SESSION has changed or more than 180 has passed since the last update.
      if ($this->sessionDataHasChanged($sessionId, $serializedData)) {

        $cid = $this->getCid($sessionId);

        // Either ssid or sid or both will be added from $key below.
        $data = new stdClass();
        $data->uid = $this->uid;
        $data->session = $serializedData;

        $this->cacheBackend->set($cid, $data);
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
    $cid = $this->getCid($sessionId);
    $this->cacheBackend->clear($cid);
    $_SESSION = array();
    $this->setSessionUid(NULL);
    return TRUE;
  }

  public function gc($lifetime) {
    // FIXME: This is not the valid signature we would really want to have,
    // but this doesn't seems that easy to match the real PHP session lifetime
    // API signature with cache backends.
    $this->cacheBackend->clear();
    return TRUE;
  }

  public function handleHttps() {
    return FALSE;
  }

  public function destroyFor($index, $value) {
    /*
     * FIXME: Do-able?
     * 
    if ('uid' == $index) {
      db_delete('sessions')
        ->condition($index, $value)
        ->execute();
    }
     */
  }

  public function __construct(array $options = array()) {
    parent::__construct($options);
 
    $bin = isset($this->options['cache_bin']) ? $this->options['cache_bin'] : 'cache_sessions';

    if (!isset($this->options['cache_backend']) || !class_exists($this->options['cache_backend'])) {
      $this->cacheBackend = new DrupalDatabaseCache($bin);
    } else {
      $class = $this->options['cache_backend'];
      $this->cacheBackend = new $class($bin);
    }
  }
}
