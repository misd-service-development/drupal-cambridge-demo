Feature: Raven failure

  Scenario Outline: Can configure Raven failure redirect path
    Given the "clean_url" variable is set to "<clean url>"
    And the "raven_login_fail_redirect" variable is set to "NULL"
    And I am logged in as the admin user
    And I am on "<config page>"
    Then I should see the base URL in the "label:contains('Login failure redirect') + .field-prefix" element
    When I fill in "Login failure redirect" with "node"
    And I press "Save configuration"
    Then the "raven_login_fail_redirect" variable should be "node"

  Examples:
    | clean url | config page                  |
    | TRUE      | /admin/config/people/raven   |
    | FALSE     | ?q=admin/config/people/raven |

  Scenario: Raven failure redirect path has to be valid
    Given I am logged in as the admin user
    And I am on "/admin/config/people/raven"
    When I fill in "Login failure redirect" with "foo"
    And I press "Save configuration"
    Then I should see "The path 'foo' is either invalid or you do not have access to it."

  Scenario: Raven failure redirect path stores unaliased path
    Given the "path" module is enabled
    And the "node" path has the alias "foo"
    And I am logged in as the admin user
    And I am on "/admin/config/people/raven"
    When I fill in "Login failure redirect" with "foo"
    And I press "Save configuration"
    Then the "raven_login_fail_redirect" variable should be "node"
    And the "Login failure redirect" field should contain "node"

  Scenario: Redirects on failure
    Given the "raven_login_fail_redirect" variable is set to "foo"
    When I go to "/?WLS-Response"
    Then I should be on "/foo"

  Scenario: Pressing cancel fails gracefully
    Given I am on "/raven/login"
    When I press "Cancel"
    Then I should see "Raven authentication cancelled"

  Scenario: Pressing cancel fails gracefully when clean URLs are disabled
    Given the "clean_url" variable is set to "FALSE"
    And I am on "/?q=raven/login"
    When I press "Cancel"
    Then I should see "Raven authentication cancelled"

  Scenario: 'kid' problem causes failure
    Given the "dblog" module is enabled
    And I have a Raven response with a "kid" problem
    Then I should see "Suspicious login attempt denied and logged"
    And I should see an "alert" "raven" Watchdog message "Suspicious login attempt claiming to be test0001. 'kid' validation failed: expecting '901', got '999'."

  Scenario: URL problem causes failure
    Given the "dblog" module is enabled
    And I have a Raven response with a "url" problem
    Then I should see "Suspicious login attempt denied and logged"
    And I should see an "alert" "raven" Watchdog message "Suspicious login attempt claiming to be test0001. 'url' validation failed"

  Scenario: 'auth' problem causes failure
    Given the "dblog" module is enabled
    And I have a Raven response with an "auth" problem
    Then I should see "Suspicious login attempt denied and logged"
    And I should see an "alert" "raven" Watchdog message "Suspicious login attempt claiming to be test0001. 'auth' validation failed: expecting 'pwd', got 'test'."

  Scenario: 'sso' problem causes failure
    Given the "dblog" module is enabled
    And I have a Raven response with an "sso" problem
    Then I should see "Suspicious login attempt denied and logged"
    And I should see an "alert" "raven" Watchdog message "Suspicious login attempt claiming to be test0001. 'sso' validation failed: expecting 'pwd', got 'test'."

  Scenario: Invalid response causes failure
    Given the "dblog" module is enabled
    And I have a Raven response with an "invalid" problem
    Then I should see "Raven authentication failure."
    And I should see an "error" "raven" Watchdog message "Authentication failure: Successful authentication."

  Scenario: Incomplete response causes failure
    Given the "dblog" module is enabled
    And I have a Raven response with an "incomplete" problem
    Then I should see "Suspicious login attempt denied and logged"
    And I should see an "alert" "raven" Watchdog message "Suspicious login attempt. Raven response is not acceptable"

  Scenario: Expired response causes failure
    Given the "dblog" module is enabled
    And I have a Raven response with an "expired" problem
    Then I should see "Login attempt timed out."
    And I should see a "warning" "raven" Watchdog message "Timeout on login attempt for test0001"
