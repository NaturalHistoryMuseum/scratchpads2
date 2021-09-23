/* $Id: potx_test.js,v 1.1.4.3 2009/11/17 12:35:09 goba Exp $ */

/**
 * Test Drupal.t and Drupal.formatPlural() invocation.
 */
 
Drupal.t('Test string in JS');
Drupal.formatPlural(count, '1 test string in JS', '@count test strings in JS');
Drupal.t('Another test string in JS', {'test': Drupal.t('Embedded test string in JS')});
Drupal.t('');
