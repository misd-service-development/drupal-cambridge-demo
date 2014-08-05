Feature: Browser closing causes logout

  Scenario: Can configure browser closing causes logout
    Given the "raven_logout_on_browser_close" variable is set to "FALSE"
    And I am logged in as the admin user
    And I am on "/admin/config/people/raven"
    When I check "Log out Raven users when closing the browser"
    And I press "Save configuration"
    Then the "raven_logout_on_browser_close" variable should be "TRUE"

  Scenario: Closing the browser logs out a Raven user when set
    Given the "raven_logout_on_browser_close" variable is set to "TRUE"
    And there is a user called "test0001" with the email address "test0001@example.com"
    When I log in to Raven as "test0001"
    And I restart the browser
    And I go to "/"
    Then I should not see "Log out"

  Scenario: Closing the browser doesn't log out a Raven user when not set
    Given the "raven_logout_on_browser_close" variable is set to "FALSE"
    And there is a user called "test0001" with the email address "test0001@example.com"
    When I log in to Raven as "test0001"
    And I restart the browser
    And I go to "/"
    Then I should see "Log out"

  Scenario: Closing the browser doesn't log out a normal user when set
    Given the "raven_logout_on_browser_close" variable is set to "TRUE"
    And I am logged in as the admin user
    When I restart the browser
    And I go to "/"
    Then I should see "Log out"
