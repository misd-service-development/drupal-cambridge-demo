Feature: Raven for Life

  Scenario: Can configure Raven for Life
    Given the "raven_allow_raven_for_life" variable is set to "FALSE"
    And I am logged in as the admin user
    And I am on "/admin/config/people/raven"
    When I check "Allow Raven for Life accounts to be authenticated"
    And I press "Save configuration"
    Then the "raven_allow_raven_for_life" variable should be "TRUE"

  Scenario: Rejection when Raven for Life users are not allowed
    Given the "dblog" module is enabled
    And the "user_register" variable is set to "1"
    And the "raven_allow_raven_for_life" variable is set to "FALSE"
    When I log in to Raven as "test0401"
    Then I should see "Raven for Life accounts are not allowed to access the site."
    And I should see an "info" "raven" Watchdog message "Raven for Life account test0401 denied access."

  Scenario: Success when Raven for Life users allowed to authenticate
    Given the "raven_allow_raven_for_life" variable is set to "TRUE"
    And the "user_register" variable is set to "2"
    When I log in to Raven as "test0401"
    Then I should see "Thank you for applying for an account. Your account is currently pending approval by the site administrator."
