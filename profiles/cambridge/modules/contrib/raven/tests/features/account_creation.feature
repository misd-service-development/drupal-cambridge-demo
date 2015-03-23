Feature: Account creation

  Scenario: Can create accounts when approval required
    Given the "user_register" variable is set to "2"
    When I log in to Raven as "test0001"
    Then I should see "Thank you for applying for an account. Your account is currently pending approval by the site administrator."
    And I should not see "The username test0001 is blocked"
    And I should not see "Log out"

  Scenario: Can create accounts when allowed
    Given the "dblog" module is enabled
    And the "user_register" variable is set to "1"
    When I log in to Raven as "test0001"
    Then I should see "Log out"
    And I should see an "notice" "raven" Watchdog message "New user: test0001 (test0001@cam.ac.uk)."
    And I should not see "Thank you for applying for an account. Your account is currently pending approval by the site administrator."
    And I should not see "The username test0001 is blocked"

  Scenario: Can't create accounts when administrators only
    Given the "user_register" variable is set to "0"
    When I log in to Raven as "test0001"
    Then I should see "Only site administrators can create accounts."
    And I should not see "Thank you for applying for an account. Your account is currently pending approval by the site administrator."
    And I should not see "The username test0001 is blocked"
    And I should not see "Log out"

  Scenario: Takes over existing accounts
    Given the "dblog" module is enabled
    And there is a user called "test0001" with the email address "test0001@example.com"
    When I log in to Raven as "test0001"
    Then I should see "Log out"
    And I should see an "notice" "raven" Watchdog message "Migrated user: test0001 (test0001@example.com)."
    And I should not see "Thank you for applying for an account. Your account is currently pending approval by the site administrator."
    And I should not see "The username test0001 is blocked"

  Scenario: Can't log in to existing blocked account
    Given the "dblog" module is enabled
    And there is a user called "test0001" with the email address "test0001@example.com"
    And the user "test0001" is blocked
    When I log in to Raven as "test0001"
    Then I should see "The username test0001 is blocked"
    And I should see an "notice" "raven" Watchdog message "Migrated user: test0001 (test0001@example.com)."
    And I should not see "Thank you for applying for an account. Your account is currently pending approval by the site administrator."
