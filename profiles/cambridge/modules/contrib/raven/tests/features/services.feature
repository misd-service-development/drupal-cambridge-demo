Feature: Raven services

  Scenario: Can configure Raven service
    Given the "raven_service" variable is set to "demo"
    And I am logged in as the admin user
    And I am on "/admin/config/people/raven"
    When I check the "Live" radio button
    And I press "Save configuration"
    Then the "raven_service" variable should be "live"

  Scenario: Uses demo service when set
    Given the "raven_service" variable is set to "demo"
    When I go to "/raven/login"
    Then I should be on "https://demo.raven.cam.ac.uk/auth/authenticate.html"

  Scenario: Uses live service when set
    Given the "raven_service" variable is set to "live"
    When I go to "/raven/login"
    Then I should be on "https://raven.cam.ac.uk/auth/authenticate.html"
