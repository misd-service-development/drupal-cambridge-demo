Feature: Login override

  Scenario: Can configure login override
    Given the "raven_login_override" variable is set to "FALSE"
    And the "raven_backdoor_login" variable is set to "FALSE"
    And I am logged in as the admin user
    And I am on "/admin/config/people/raven"
    When I check "Enable Raven login override"
    And I press "Save configuration"
    Then the "raven_login_override" variable should be "TRUE"
    And the "raven_backdoor_login" variable should be "FALSE"

  Scenario: Can configure backdoor login
    Given the "raven_login_override" variable is set to "FALSE"
    And the "raven_backdoor_login" variable is set to "FALSE"
    And I am logged in as the admin user
    And I am on "/admin/config/people/raven"
    When I check "Enable Raven login override"
    And I check "Enable non-Raven backdoor login"
    And I press "Save configuration"
    Then the "raven_login_override" variable should be "TRUE"
    And the "raven_backdoor_login" variable should be "TRUE"

  Scenario Outline: Overrides user pages when set
    Given the "raven_login_override" variable is set to "TRUE"
    When I go to "<path>"
    Then I should be on "https://demo.raven.cam.ac.uk/auth/authenticate.html"

  Examples:
    | path           |
    | /user          |
    | /user/register |
    | /user/login    |
    | /usEr          |
    | /usEr/register |
    | /usEr/login    |

  Scenario Outline: Overrides user pages with language prefix when set
    Given the "raven_login_override" variable is set to "TRUE"
    And the "i18n" module is enabled
    And Spanish is enabled
    When I go to "/es<path>"
    Then I should be on "https://demo.raven.cam.ac.uk/auth/authenticate.html"

  Examples:
    | path           |
    | /user          |
    | /user/register |
    | /user/login    |
    | /usEr          |
    | /usEr/register |
    | /usEr/login    |

  Scenario Outline: Overrides user pages when in maintenance mode
    Given the "maintenance_mode" variable is set to "TRUE"
    And the "raven_login_override" variable is set to "TRUE"
    When I go to "<path>"
    Then I should be on "https://demo.raven.cam.ac.uk/auth/authenticate.html"

  Examples:
    | path           |
    | /user          |
    | /user/register |
    | /user/login    |
    | /usEr          |
    | /usEr/register |
    | /usEr/login    |

  Scenario Outline: Blocks password page when set
    Given the "raven_login_override" variable is set to "TRUE"
    When I go to "<path>"
    Then I should see "Access Denied"

  Examples:
    | path           |
    | /user/password |
    | /usEr/password |

  Scenario Outline: Blocks password page with language prefix when set
    Given the "raven_login_override" variable is set to "TRUE"
    And the "i18n" module is enabled
    And Spanish is enabled
    When I go to "/es<path>"
    Then I should see "Access Denied"

  Examples:
    | path           |
    | /user/password |
    | /usEr/password |

  Scenario Outline: Blocks password page when in maintenance mode
    Given the "maintenance_mode" variable is set to "TRUE"
    And the "raven_login_override" variable is set to "TRUE"
    When I go to "<path>"
    Then I should see "Access Denied"

  Examples:
    | path           |
    | /user/password |
    | /usEr/password |

  Scenario: Can't access backdoor when not enabled
    Given the "raven_login_override" variable is set to "TRUE"
    And the "raven_backdoor_login" variable is set to "FALSE"
    When I go to "/user/backdoor/login"
    Then I should see "Access denied"

  Scenario: Can't access backdoor when login isn't overridden
    Given the "raven_login_override" variable is set to "FALSE"
    And the "raven_backdoor_login" variable is set to "TRUE"
    When I go to "/user/backdoor/login"
    Then I should see "Access denied"

  Scenario: Provides a backdoor when set
    Given the "raven_login_override" variable is set to "TRUE"
    And the "raven_backdoor_login" variable is set to "TRUE"
    When I go to "/user/backdoor/login"
    Then I should see "Non-Raven backdoor login"
    When I fill in "Username" with "admin"
    And I fill in "Password" with "password"
    And I press "Log in"
    Then I should be on "/user/1"
    And I should see "admin" in the "#page-title" element
    And I should see "Log out"

  Scenario: Provides a backdoor when set and in maintenance mode
    Given the "maintenance_mode" variable is set to "TRUE"
    And the "raven_login_override" variable is set to "TRUE"
    And the "raven_backdoor_login" variable is set to "TRUE"
    When I go to "/user/backdoor/login"
    Then I should see "Non-Raven backdoor login"

  Scenario: User login block changes when set
    Given the "block" module is enabled
    And the "raven_login_override" variable is set to "TRUE"
    And the "user" "login" block is in the "sidebar_first" region
    And I am on "/"
    Then I should see "Log in with Raven" in the "#block-user-login" element
    And I should not see "Username" in the "#block-user-login" element
    And I should not see "Password" in the "#block-user-login" element
    When I follow "Log in with Raven"
    Then I should be on "https://demo.raven.cam.ac.uk/auth/authenticate.html"
