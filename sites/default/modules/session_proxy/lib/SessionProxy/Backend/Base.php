<?php

abstract class SessionProxy_Backend_Base
  implements SessionProxy_Backend_Interface
{
  /**
   * @var bool
   */
  protected $httpsEnabled = FALSE;

  /**
   * @var bool
   */
  protected $started = FALSE;

  /**
   * @var string
   */
  protected $sessionIdentifier;

  /**
   * @var string
   */
  protected $sessionName;

  /**
   * @var string
   */
  protected $sessionNameUnsecure;

  /**
   * @var bool
   */
  protected $userAccessUpdated = FALSE;

  /**
   * Generate new session identifier, set it as PHP session identifier and
   * return it.
   * 
   * @return string
   */
  protected function generateSessionIdentifier() {
    $this->sessionIdentifier = drupal_hash_base64(uniqid(mt_rand(), TRUE));
    // Depending on the underlaying implementation, some hash may fail to
    // achieve the session_start() especially if you are using the PHP native
    // one to handle session storage.
    $this->sessionIdentifier = str_replace('_', '-', $this->sessionIdentifier);
    session_id($this->sessionIdentifier);
    return $this->sessionIdentifier;
  }

  /**
   * Get the actual logged in user user identifier if any.
   * 
   * @return int
   *   Valid user identifier or NULL.
   */
  protected abstract function getSessionUid();

  /**
   * Refresh global user data following the actual session state.
   */
  protected function updateUser() {
    global $user;

    $uid = $this->getSessionUid();

    if (!empty($uid)) {
      $user = db_query("SELECT u.* FROM {users} u WHERE u.uid = :uid", array(':uid' => $uid))->fetchObject();
      if (1 == $user->status) {
        $user->data = unserialize($user->data);
        $user->roles = array();
        $user->roles[DRUPAL_AUTHENTICATED_RID] = 'authenticated user';
        $user->roles += db_query("SELECT r.rid, r.name FROM {role} r INNER JOIN {users_roles} ur ON ur.rid = r.rid WHERE ur.uid = :uid", array(':uid' => $user->uid))->fetchAllKeyed(0, 1);
      } else {
        $user = drupal_anonymous_user();
      }
    } else {
      $user = drupal_anonymous_user();
    }

    // The 'session' attribute is an insanity and should be removed.
    $user->session = '';
    $user->timestamp = REQUEST_TIME;

    // Avoid some PHP warnings with backends using it (mongodb module does
    // check it). Some backends may set this variable, if they keep it, some
    // other won't.
    if (!isset($user->cache)) {
      $user->cache = 0;
    }

    // Do not update access time more than once per 180 seconds. Also check
    // for an active database connection: actual core will have one, but in
    // the late future we may have session handling without database at all.
    if (!$this->userAccessUpdated && Database::isActiveConnection() && $user->uid && (REQUEST_TIME - $user->access > variable_get('session_write_interval', 180))) {
      db_update('users')
        ->fields(array('access' => REQUEST_TIME))
        ->condition('uid', $user->uid)
        ->execute();
      $this->userAccessUpdated = TRUE;
    }
  }

  /**
   * Refresh various information of the object right after session state
   * change.
   */
  protected function refreshAfterSessionChange() {
    $this->updateUser();

    // FIXME: This should not live in user session, but as a reaction of
    // session creation or regeneration in user module.
    date_default_timezone_set(drupal_get_user_timezone());
  }

  /**
   * Is the current session is empty.
   * 
   * @return bool
   */
  protected function sessionIsEmpty() {
    return !isset($_SESSION) || empty($_SESSION) || (isset($_SESSION['uid']) && 1 == count($_SESSION));
  }

  public function isStarted() {
    return $this->started;
  }

  public function start() {
    // Command line clients do not support cookies nor sessions.
    if (!$this->started && !drupal_is_cli()) {
      if (!$this->sessionIsEmpty()) {
        // Keep data from already set data, even if the session has not been
        // started yet, some pieces of software may have set $_SESSION super
        // global data before us: this is an artifact of the lazzy session
        // creation feature.
        $currentData = $_SESSION;
        session_start();
        $_SESSION += $currentData;
      } else {
        session_start();
      }
      $this->started = TRUE;
    }
  }

  public function handleHttps() {
    return FALSE;
  }

  public function commit() {
    global $user;

    if (!$this->isWriteEnabled()) {
      return;
    }

    if ($user->uid) {
      // Always save logged in user sessions: we will let the underlaying
      // storage engine decide weither or not data should really be saved.
      session_write_close();
    } else if (!empty($_SESSION)) {
      // Save session for anonymous only if session data has been se: this
      // is another lazzy session creation feature artifact.
      if (!$this->started) {
        $this->start();
      }
      session_write_close();
    }
  }

  /**
   * Native implementation is opaque, and cannot allow us to index session: it
   * is impossible to proceed with this cleaning.
   * 
   * @see SessionProxy_Backend_Interface::destroyAllForUser()
   */
  public function destroyAllForUser($uid) {}

  /**
   * Default constructor.
   */
  public function __construct() {
    global $user, $is_https;

    $this->httpsEnabled = $this->handleHttps() && $is_https;
    $this->sessionName = session_name();

    if ($this->httpsEnabled) {
      $this->sessionNameUnsecure = substr(session_name(), 1);
    }

    if (!empty($_COOKIE[$this->sessionName]) || ($this->httpsEnabled && !empty($_COOKIE[$this->sessionNameUnsecure]))) {
      // If a session cookie exists, initialize the session. Otherwise the
      // session is only started on demand in drupal_session_commit(), making
      // anonymous users not use a session cookie unless something is stored in
      // $_SESSION. This allows HTTP proxies to cache anonymous page views.
      $this->start();
      $this->sessionIdentifier = session_id();
      $this->refreshAfterSessionChange();

      if ($user->uid || !$this->sessionIsEmpty()) {
        drupal_page_is_cacheable(FALSE);
      }
    } else {
      // Set a session identifier for this request. This is necessary because
      // we lazily start sessions at the end of this request, and some
      // processes (like drupal_get_token()) needs to know the future
      // session ID in advance.
      $user = drupal_anonymous_user();
      $this->generateSessionIdentifier();
      $this->refreshAfterSessionChange();
    }
  }
}
