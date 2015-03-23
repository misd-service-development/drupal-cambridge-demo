<?php

namespace Misd\Drupal\RavenModule;

use Behat\Behat\Event\SuiteEvent;
use Behat\Mink\Driver\BrowserKitDriver;
use Behat\Mink\Exception\ElementNotFoundException;
use Behat\Mink\Exception\UnsupportedDriverActionException;
use Behat\MinkExtension\Context\MinkContext;
use Behat\MinkExtension\Context\RawMinkContext;
use Exception;
use PDO;
use Symfony\Component\Filesystem\Filesystem;
use Symfony\Component\Finder\Finder;
use Symfony\Component\Process\Process;

class FeatureContext extends RawMinkContext {
  /**
   * @var FileSystem
   */
  protected static $filesystem;
  protected static $drushPath;
  protected static $drupalPath;
  protected static $dsn;
  protected static $modulePath;

  public function __construct(array $parameters) {
    $this->useContext('mink', new MinkContext());

    self::setup($parameters);
  }

  protected static function setup(array $parameters) {
    self::$filesystem = new Filesystem();
    self::$modulePath = realpath(__DIR__ . '/../../../');
    self::$drushPath = sprintf('php "%s/tests/vendor/drush/drush/drush.php"', self::$modulePath); # bin/drush broken on Windows
    self::$drupalPath = $parameters['drupal_path'];
    self::$dsn = sprintf('sqlite:%s/sites/default/db/site.sqlite', str_replace('\\', '/', $parameters['drupal_path']));
  }

  protected static function getPdo() {
    $pdo = new PDO(self::$dsn);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    return $pdo;
  }

  /**
   * @return MinkContext
   */
  protected function getMinkContext() {
    return $this->getMainContext()->getSubcontext('mink');
  }

  protected static function drushCommand($command, $quiet = TRUE) {
    $command = new Process(sprintf('%s %s --root="%s" --yes %s', self::$drushPath, $command, self::$drupalPath, $quiet ? '--quiet' : ''));
    $command->setTimeout(NULL);
    $command->run();

    if (FALSE === $command->isSuccessful()) {
      throw new Exception('Failed to execute Drush command: ' . $command->getCommandLine());
    }

    return $command->getOutput();
  }

  /**
   * @BeforeSuite
   */
  public static function downloadDrupal(SuiteEvent $event) {
    self::setup($event->getContextParameters());

    self::clearDrupal();

    $path = dirname(self::$drupalPath);
    $folder = basename(self::$drupalPath);

    self::drushCommand(sprintf('dl drupal --drupal-project-rename="%s" --destination="%s"', $folder, $path));

    $finder = Finder::create()->exclude('tests')->in(self::$modulePath);
    self::$filesystem->mirror(self::$modulePath, self::$drupalPath . '/sites/all/modules/raven', $finder, array(
      'override' => TRUE,
      'delete' => TRUE
    ));
  }

  public static function clearDrupal() {
    if (FALSE === is_dir(self::$drupalPath)) {
      return;
    };

    self::$filesystem->chmod(self::$drupalPath, 0777, 0000, TRUE);
    self::$filesystem->remove(self::$drupalPath);
  }

  /**
   * @BeforeScenario
   */
  public function makeSite() {
    $site = self::$drupalPath . '/sites/default';
    $master = self::$drupalPath . '/sites/master';

    if (FALSE === is_dir($master)) {
      self::drushCommand(sprintf('site-install testing --account-name="admin" --account-pass="password" --db-url="%s"', self::$dsn));
      $this->theModuleIsEnabled('raven');
      $this->theVariableIsSetTo('raven_service', 'demo');

      self::$filesystem->mirror($site, $master);
    }
    else {
      self::$filesystem->chmod($site, 0777, 0000, TRUE);
      self::$filesystem->mirror($master, $site, NULL, array('override' => TRUE, 'delete' => TRUE));
    }
  }

  /**
   * @Given /^I am logged in as the admin user$/
   * @When /^I log in as the admin user$/
   */
  public function iAmLoggedInAsTheAdminUser() {
    $minkContext = $this->getMinkContext();

    $minkContext->visit('/');

    if ($this->getSession()->getPage()->hasLink('Log out')) {
      $minkContext->visit('/user/logout');
    }

    if ($this->isVariable('raven_login_override', TRUE) && $this->isVariable('raven_backdoor_login', TRUE)) {
      $minkContext->visit('/user/backdoor/login');
    }
    else {
      $minkContext->visit('/user/login');
    }

    $minkContext->fillField('Username', 'admin');
    $minkContext->fillField('Password', 'password');
    $minkContext->pressButton('Log in');
  }

  /**
   * @Given /^the "([^"]*)" role has the "([^"]*)" "([^"]*)" permission$/
   */
  public function theRoleHasThePermission($role, $module, $permission) {
    $rid = $this->findRidForRole($role);

    $sth = self::getPdo()
      ->prepare('INSERT OR REPLACE INTO role_permission (rid, permission, module) VALUES (:rid, :permission, :module)');
    $sth->execute(array(':rid' => $rid, ':permission' => $permission, ':module' => $module));
  }

  /**
   * @Given /^the "([^"]*)" role does not have the "([^"]*)" "([^"]*)" permission$/
   */
  public function theRoleDoesNotHaveThePermission($role, $module, $permission) {
    $rid = $this->findRidForRole($role);

    $sth = self::getPdo()
      ->prepare('DELETE FROM role_permission WHERE rid = :rid AND permission = :permission AND module = :module');
    $sth->execute(array(':rid' => $rid, ':permission' => $permission, 'module' => $module));
  }

  protected function findRidForRole($role) {
    $sth = self::getPdo()->prepare('SELECT rid FROM role WHERE name = :role LIMIT 1');
    $sth->execute(array(':role' => $role));
    $results = $sth->fetch(PDO::FETCH_ASSOC);

    if (FALSE === $results) {
      throw new Exception('Can\'t find role ' . $role);
    }

    return $results['rid'];
  }

  /**
   * @Given /^the "([^"]*)" module is enabled$/
   */
  public function theModuleIsEnabled($module) {
    self::drushCommand(sprintf('pm-enable "%s" --resolve-dependencies', $module));
  }

  /**
   * @Given /^I have a Raven response with an? "([^"]*)" problem$/
   */
  public function iHaveARavenResponseWithAProblem($problem) {
    $url = rtrim($this->getMinkParameter('base_url'), '/') . '/';

    if (FALSE === in_array($problem, array('kid', 'url', 'auth', 'sso', 'invalid', 'incomplete', 'expired'))) {
      throw new Exception('Unknown problem');
    }

    $this->getSession()->visit(create_raven_response($url, 200, 'test0001', $problem));
  }

  /**
   * @Given /^there is a user called "([^"]*)" with the e-?mail address "([^"]*)"$/
   */
  public function thereIsAUserCalledWithTheEmailAddress($username, $emailAddress) {
    self::drushCommand(sprintf('user-create "%s" --mail="%s"', $username, $emailAddress));
  }

  /**
   * @Given /^the user "([^"]*)" is blocked$/
   */
  public function theUserIsBlocked($username) {
    self::drushCommand(sprintf('user-block "%s"', $username));
  }

  /**
   * @Given /^the "([^"]*)" variable is set to "([^"]*)"$/
   */
  public function theVariableIsSetTo($variable, $value) {
    $value = maybe_serialize($value);

    $sth = self::getPdo()->prepare('INSERT OR REPLACE INTO variable (name, value) VALUES (:variable, :value)');
    $sth->execute(array(':variable' => $variable, ':value' => $value));
  }

  /**
   * @Given /^Spanish is enabled$/
   */
  public function spanishIsEnabled() {
    $minkContext = $this->getMinkContext();

    $this->iAmLoggedInAsTheAdminUser();
    $minkContext->visit('/admin/config/regional/language');
    $minkContext->clickLink('Add language');
    $minkContext->selectOption('Language name', 'Spanish (Español)');
    $minkContext->pressButton('Add language');
    $minkContext->visit('/admin/config/regional/language/configure');
    $minkContext->checkOption('URL language provider');
    $minkContext->pressButton('Save settings');
    $minkContext->clickLink('Log out');
  }

  /**
   * @When /^I log in to Raven as "([^"]*)"$/
   */
  public function iLogInToRavenAs($username) {
    $minkContext = $this->getMinkContext();

    $minkContext->visit('/');

    if ($this->getSession()->getPage()->hasLink('Log out')) {
      $minkContext->visit('/user/logout');
    }

    $minkContext->visit('/raven/login');
    $minkContext->fillField('User-id', $username);
    $minkContext->fillField('Password', 'test');
    $minkContext->pressButton('Submit');
  }

  /**
   * @When /^I check the "([^"]*)" radio button$/
   */
  public function iCheckTheRadioButton($locator) {
    $field = $this->getSession()->getPage()->findField($locator);

    if (NULL === $field) {
      throw new ElementNotFoundException($this->getSession(), 'form field', 'id|name|label', $locator);
    }

    $field->selectOption($field->getAttribute('value'));
  }

  /**
   * @Then /^the "([^"]*)" variable should be "([^"]*)"$/
   */
  public function theVariableShouldBe($variable, $expected) {
    if (FALSE === $this->isVariable($variable, $expected)) {
      throw new Exception(sprintf('The variable is "%s"', $this->getVariable($variable)));
    }
  }

  /**
   * @Given /^the "([^"]*)" "([^"]*)" block is in the "([^"]*)" region$/
   */
  public function theBlockIsInTheRegion($module, $delta, $region) {
    $sth = self::getPdo()
      ->prepare('UPDATE block SET status = 1, region = :region WHERE module = :module AND delta = :delta');
    $sth->execute(array(':region' => $region, ':module' => $module, 'delta' => $delta));
  }

  /**
   * @Given /^the "([^"]*)" path has the alias "([^"]*)"$/
   */
  public function thePathHasTheAlias($path, $alias) {
    $minkContext = $this->getMinkContext();

    $this->iAmLoggedInAsTheAdminUser();
    $minkContext->visit('/admin/config/search/path/add');
    $minkContext->fillField('Existing system path', $path);
    $minkContext->fillField('Path alias', $alias);
    $minkContext->pressButton('Save');

    $minkContext->assertPageContainsText('The alias has been saved.');
  }

  /**
   * @Then /^I should see an? "([^"]*)" "([^"]*)" Watchdog message "([^"]*)"$/
   */
  public function iShouldSeeAWatchdogMessage($severity, $type, $message) {
    $minkContext = $this->getMinkContext();

    $this->iAmLoggedInAsTheAdminUser();
    $minkContext->visit('/admin/reports/dblog');
    $minkContext->selectOption('Type', $type);
    $minkContext->selectOption('Severity', $severity);
    $minkContext->pressButton('Filter');

    foreach ($this->getSession()->getPage()->findAll('css', 'table tbody a') as $event) {
      $event->click();

      if (FALSE !== strpos($this->getSession()->getPage()->getText(), $message)) {
        return;
      }

      $this->getSession()->back();
    }

    throw new Exception('Message not found');
  }

  /**
   * @When /^I restart the browser$/
   */
  public function iRestartTheBrowser() {
    $session = $this->getSession();
    $driver = $session->getDriver();

    if (FALSE === $driver instanceof BrowserKitDriver) {
      throw new UnsupportedDriverActionException('Keeping sessions cookies are not supported by %s', $driver);
    }

    /** @var BrowserKitDriver $driver */
    $client = $driver->getClient();

    $cookies = $client->getCookieJar()->all();

    $session->restart();

    $session->visit('/');

    foreach ($cookies as $cookie) {
      if (FALSE === $cookie->isExpired() && NULL !== $cookie->getExpiresTime()) {
        $client->getCookieJar()->set($cookie);
      }
    }
  }

  /**
   * @Then /^I should see the base URL in the "(?P<element>[^"]*)" element$/
   */
  public function iShouldSeeTheBaseUrlInTheElement($element) {
    $text = rtrim($this->getMinkParameter('base_url'), '/') . '/';

    if ($this->isVariable('clean_url', FALSE)) {
      $text .= '?q=';
    }

    $element = $this->getMinkContext()->assertSession()->elementExists('css', $element);

    if ($element->getText() !== $text) {
      throw new Exception('Element text is "' . $element->getText() . '", but expected "' . $text . '"');
    }
  }

  protected function getVariable($variable) {
    $sth = self::getPdo()->prepare('SELECT * FROM variable WHERE name = :variable LIMIT 1');

    $sth->execute(array(':variable' => $variable));

    $results = $sth->fetch(PDO::FETCH_ASSOC);

    return $results['value'];
  }

  protected function isVariable($variable, $expected) {
    $expected = maybe_serialize($expected);

    $possible = array($expected);

    if ('b:1;' === $expected) {
      $possible[] = 'i:1;';
    }
    elseif ('i:1;' === $expected) {
      $possible[] = 'b:1;';
    }
    elseif ('b:0;' === $expected) {
      $possible[] = 'i:0;';
    }
    elseif ('i:0;' === $expected) {
      $possible[] = 'b:0;';
    }

    return in_array($this->getVariable($variable), $possible, TRUE);
  }
}
