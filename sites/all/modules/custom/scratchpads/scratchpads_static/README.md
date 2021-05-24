# Scratchpads Static

This module gets a site ready to be converted to a static site, in the following way:

	- Removes the login form, search form, contact forms for the anonymous user
	- Sets any remaining forms to disabled
	- Add a message saying the site has been archived

Enable the module via drush and then use a site cralwer:

```bash
wget -P . -mpck --user-agent="" -e robots=off --wait 1 -E http://extinction.myspecies.info/
```
