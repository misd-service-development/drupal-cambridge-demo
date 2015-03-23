Feature: Raven rules

  Scenario: Is Raven user condition
    Given the "rules" module is enabled
    And the "rules_admin" module is enabled
    And the "raven_override_administrator_approval" variable is set to "TRUE"
    And I am logged in as the admin user
    And I am on "/admin/config/workflow/rules/reaction/add"
    When I fill in "Name" with "Test Rule"
    And I fill in "Machine-readable name" with "test_rule"
    And I select "User has logged in" from "React on event"
    And I press "Save"
    And I follow "Add condition"
    And I select "User is a Raven user" from "Select the condition to add"
    And I press "Continue"
    And I press "Save"
    And I follow "Add action"
    And I select "Show a message on the site" from "Select the action to add"
    And I press "Continue"
    And I fill in "Value" with "Hello Raven user"
    And I press "Save"
    When I log in to Raven as "test0001"
    Then I should see "Hello Raven user"
    When I log in as the admin user
    Then I should not see "Hello Raven user"
