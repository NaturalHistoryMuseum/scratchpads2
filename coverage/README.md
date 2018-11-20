# Coverage tool

This tool runs [PHP code sniffer](https://github.com/squizlabs/PHP_CodeSniffer) on lines that have changed since master.

By default it scans all code in sites/all/modules/custom, but you can provide a specific path to check.

Run:

`./coverage/test.sh sites/all/modules/custom/module_name`

Currently it only checks the files, it doesn't repair them.
