CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation


INTRODUCTION
------------

Author:
* Pamela Dingle

This module adds the javascript and user status checker API needed to enable drupal to use Account Choosers

Drupal 7 INSTALLATION
------------

1. Copy the chooser directory to your sites/SITENAME/modules directory or sites/all/modules directory.

2. Enable the module at Administer > Site building > Modules.

3. Visit Administer > Site Configuration > Chooser Settings to optionally configure Federation information

5. When the module is enabled, the following behavior will occur:
	- On login and signup
		- if a user has an account in the chooser, they will be taken to accountchooser.com
		- if the account they choose is registered, the user will be returned to the login page with their identifier filled in and focus set to the password field
		- if the account they choose is not registered, the user will be returned to the signup page with data filled in
		- otherwise the module falls through

For now, federation stuff isn't in.  I will add it shortly.

