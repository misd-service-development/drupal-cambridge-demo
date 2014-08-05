Feature: Config

  Scenario: See config page when user 1
    Given I am logged in as the admin user
    And I am on "/admin/config"
    Then I should see a "h3:contains('People') + div dt:contains('Raven authentication')" element
    When I follow "Raven authentication"
    Then I should be on "/admin/config/people/raven"
    And I should see "Raven authentication"

  Scenario: See config page when permitted
    Given the "authenticated user" role has the "raven" "administer raven authentication" permission
    And the "authenticated user" role has the "system" "access administration pages" permission
    And there is a user called "test0001" with the email address "test0001@example.com"
    When I log in to Raven as "test0001"
    And I go to "/admin/config"
    Then I should see a "h3:contains('People') + div dt:contains('Raven authentication')" element
    When I follow "Raven authentication"
    Then I should be on "/admin/config/people/raven"
    And I should see "Raven authentication"

  Scenario: Can't see config page when not permitted
    Given the "authenticated user" role does not have the "raven" "administer raven authentication" permission
    And the "authenticated user" role has the "system" "access administration pages" permission
    And there is a user called "test0001" with the email address "test0001@example.com"
    When I log in to Raven as "test0001"
    And I go to "/admin/config"
    Then I should not see "Raven authentication"
    When I go to "/admin/config/people/raven"
    And I should see "Access denied"
