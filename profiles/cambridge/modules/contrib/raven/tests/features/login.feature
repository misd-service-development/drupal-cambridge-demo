Feature: Login message

  Scenario Outline: Login page message
    When I go to "<path>"
    Then I should see "Have a Raven account? You can log in with Raven instead." in the "form .messages" element
    When I follow "log in with Raven"
    Then I should be on "https://demo.raven.cam.ac.uk/auth/authenticate.html"

  Examples:
    | path           |
    | /user          |
    | /user/register |
    | /user/login    |

  Scenario: User login block message
    Given the "block" module is enabled
    And the "user" "login" block is in the "sidebar_first" region
    And I am on "/"
    Then I should see "Have a Raven account? You can log in with Raven instead." in the "#block-user-login" element
    When I follow "log in with Raven"
    Then I should be on "https://demo.raven.cam.ac.uk/auth/authenticate.html"

  Scenario: After Raven login redirects to current page
    Given the "raven_override_administrator_approval" variable is set to "TRUE"
    And I am on "/user"
    And I follow "log in with Raven"
    And I fill in "User-id" with "test0001"
    And I fill in "Password" with "test"
    And I press "Submit"
    Then I should be on "/user"

  Scenario: After Raven login redirects to destination page
    Given the "raven_override_administrator_approval" variable is set to "TRUE"
    When I go to "/raven/login?destination=foo"
    And I fill in "User-id" with "test0001"
    And I fill in "Password" with "test"
    And I press "Submit"
    Then I should be on "/foo"

  Scenario: After Raven login redirects to the homepage when the destination page is not a relative URL
    Given the "raven_override_administrator_approval" variable is set to "TRUE"
    When I go to "/raven/login?destination=http://www.cam.ac.uk/"
    And I fill in "User-id" with "test0001"
    And I fill in "Password" with "test"
    And I press "Submit"
    Then I should be on the homepage

  Scenario: Raven login available when in maintenance mode
    Given the "maintenance_mode" variable is set to "TRUE"
    And the "authenticated user" role has the "system" "access site in maintenance mode" permission
    And there is a user called "test0001" with the email address "test0001@example.com"
    When I go to "/raven/login"
    And I fill in "User-id" with "test0001"
    And I fill in "Password" with "test"
    And I press "Submit"
    Then I should see "Log out"
    And I should see "Operating in maintenance mode"
