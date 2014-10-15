<?php
/**
 * @file
 * agridrupal_content_types.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function agridrupal_content_types_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_agconference';
  $strongarm->value = '0';
  $export['ant_agconference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_agcorporate_body';
  $strongarm->value = '0';
  $export['ant_agcorporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_agdlios';
  $strongarm->value = '0';
  $export['ant_agdlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_agevent';
  $strongarm->value = '0';
  $export['ant_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_aginstitution';
  $strongarm->value = '0';
  $export['ant_aginstitution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_agnews';
  $strongarm->value = '0';
  $export['ant_agnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_agperiodical';
  $strongarm->value = '0';
  $export['ant_agperiodical'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_agprofile';
  $strongarm->value = '1';
  $export['ant_agprofile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_agrssevents';
  $strongarm->value = '0';
  $export['ant_agrssevents'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_agrssjob';
  $strongarm->value = 0;
  $export['ant_agrssjob'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_agrssnews';
  $strongarm->value = 0;
  $export['ant_agrssnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_agvacancy';
  $strongarm->value = '0';
  $export['ant_agvacancy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_agconference';
  $strongarm->value = '';
  $export['ant_pattern_agconference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_agcorporate_body';
  $strongarm->value = '';
  $export['ant_pattern_agcorporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_agdlios';
  $strongarm->value = '';
  $export['ant_pattern_agdlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_agevent';
  $strongarm->value = '';
  $export['ant_pattern_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_aginstitution';
  $strongarm->value = '';
  $export['ant_pattern_aginstitution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_agnews';
  $strongarm->value = '';
  $export['ant_pattern_agnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_agperiodical';
  $strongarm->value = '';
  $export['ant_pattern_agperiodical'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_agprofile';
  $strongarm->value = '[node:field-ag-lastname], [node:field-ag-name]';
  $export['ant_pattern_agprofile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_agrssevents';
  $strongarm->value = '';
  $export['ant_pattern_agrssevents'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_agrssjob';
  $strongarm->value = '';
  $export['ant_pattern_agrssjob'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_agrssnews';
  $strongarm->value = '';
  $export['ant_pattern_agrssnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_pattern_agvacancy';
  $strongarm->value = '';
  $export['ant_pattern_agvacancy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_agconference';
  $strongarm->value = 0;
  $export['ant_php_agconference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_agcorporate_body';
  $strongarm->value = 0;
  $export['ant_php_agcorporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_agdlios';
  $strongarm->value = 0;
  $export['ant_php_agdlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_agevent';
  $strongarm->value = 0;
  $export['ant_php_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_aginstitution';
  $strongarm->value = 0;
  $export['ant_php_aginstitution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_agnews';
  $strongarm->value = 0;
  $export['ant_php_agnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_agperiodical';
  $strongarm->value = 0;
  $export['ant_php_agperiodical'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_agprofile';
  $strongarm->value = 0;
  $export['ant_php_agprofile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_agrssevents';
  $strongarm->value = 0;
  $export['ant_php_agrssevents'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_agrssjob';
  $strongarm->value = 0;
  $export['ant_php_agrssjob'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_agrssnews';
  $strongarm->value = 0;
  $export['ant_php_agrssnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ant_php_agvacancy';
  $strongarm->value = 0;
  $export['ant_php_agvacancy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agauthor';
  $strongarm->value = '1';
  $export['comment_agauthor'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agconference';
  $strongarm->value = '0';
  $export['comment_agconference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agcorporate_body';
  $strongarm->value = '0';
  $export['comment_agcorporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agdlios';
  $strongarm->value = '0';
  $export['comment_agdlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agevent';
  $strongarm->value = '0';
  $export['comment_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_aginstitution';
  $strongarm->value = '2';
  $export['comment_aginstitution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agnews';
  $strongarm->value = '0';
  $export['comment_agnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agperiodical';
  $strongarm->value = '1';
  $export['comment_agperiodical'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agprofile';
  $strongarm->value = '2';
  $export['comment_agprofile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agrssevents';
  $strongarm->value = '0';
  $export['comment_agrssevents'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agrssjob';
  $strongarm->value = '2';
  $export['comment_agrssjob'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agrssnews';
  $strongarm->value = '0';
  $export['comment_agrssnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_agvacancy';
  $strongarm->value = '2';
  $export['comment_agvacancy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agconference';
  $strongarm->value = 0;
  $export['comment_anonymous_agconference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agcorporate_body';
  $strongarm->value = 0;
  $export['comment_anonymous_agcorporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agdlios';
  $strongarm->value = 0;
  $export['comment_anonymous_agdlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agevent';
  $strongarm->value = 0;
  $export['comment_anonymous_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_aginstitution';
  $strongarm->value = 0;
  $export['comment_anonymous_aginstitution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agnews';
  $strongarm->value = 0;
  $export['comment_anonymous_agnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agperiodical';
  $strongarm->value = 0;
  $export['comment_anonymous_agperiodical'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agprofile';
  $strongarm->value = 0;
  $export['comment_anonymous_agprofile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agrssevents';
  $strongarm->value = 0;
  $export['comment_anonymous_agrssevents'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agrssjob';
  $strongarm->value = 0;
  $export['comment_anonymous_agrssjob'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agrssnews';
  $strongarm->value = 0;
  $export['comment_anonymous_agrssnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_agvacancy';
  $strongarm->value = 0;
  $export['comment_anonymous_agvacancy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agauthor';
  $strongarm->value = '3';
  $export['comment_controls_agauthor'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agconference';
  $strongarm->value = '3';
  $export['comment_controls_agconference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agcorporate_body';
  $strongarm->value = '3';
  $export['comment_controls_agcorporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agdlios';
  $strongarm->value = '3';
  $export['comment_controls_agdlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agevent';
  $strongarm->value = '3';
  $export['comment_controls_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_aginstitution';
  $strongarm->value = '3';
  $export['comment_controls_aginstitution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agnews';
  $strongarm->value = '3';
  $export['comment_controls_agnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agprofile';
  $strongarm->value = '3';
  $export['comment_controls_agprofile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agrssevents';
  $strongarm->value = '3';
  $export['comment_controls_agrssevents'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agrssjob';
  $strongarm->value = '3';
  $export['comment_controls_agrssjob'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agrssnews';
  $strongarm->value = '3';
  $export['comment_controls_agrssnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_agvacancy';
  $strongarm->value = '3';
  $export['comment_controls_agvacancy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agconference';
  $strongarm->value = 1;
  $export['comment_default_mode_agconference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agcorporate_body';
  $strongarm->value = 1;
  $export['comment_default_mode_agcorporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agdlios';
  $strongarm->value = 1;
  $export['comment_default_mode_agdlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agevent';
  $strongarm->value = 1;
  $export['comment_default_mode_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_aginstitution';
  $strongarm->value = 1;
  $export['comment_default_mode_aginstitution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agnews';
  $strongarm->value = 1;
  $export['comment_default_mode_agnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agperiodical';
  $strongarm->value = 0;
  $export['comment_default_mode_agperiodical'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agprofile';
  $strongarm->value = 1;
  $export['comment_default_mode_agprofile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agrssevents';
  $strongarm->value = 1;
  $export['comment_default_mode_agrssevents'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agrssjob';
  $strongarm->value = 1;
  $export['comment_default_mode_agrssjob'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agrssnews';
  $strongarm->value = 1;
  $export['comment_default_mode_agrssnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_agvacancy';
  $strongarm->value = 1;
  $export['comment_default_mode_agvacancy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agauthor';
  $strongarm->value = '1';
  $export['comment_default_order_agauthor'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agconference';
  $strongarm->value = '1';
  $export['comment_default_order_agconference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agcorporate_body';
  $strongarm->value = '1';
  $export['comment_default_order_agcorporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agdlios';
  $strongarm->value = '1';
  $export['comment_default_order_agdlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agevent';
  $strongarm->value = '1';
  $export['comment_default_order_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_aginstitution';
  $strongarm->value = '1';
  $export['comment_default_order_aginstitution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agnews';
  $strongarm->value = '1';
  $export['comment_default_order_agnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agprofile';
  $strongarm->value = '1';
  $export['comment_default_order_agprofile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agrssevents';
  $strongarm->value = '1';
  $export['comment_default_order_agrssevents'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agrssjob';
  $strongarm->value = '1';
  $export['comment_default_order_agrssjob'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agrssnews';
  $strongarm->value = '1';
  $export['comment_default_order_agrssnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_agvacancy';
  $strongarm->value = '1';
  $export['comment_default_order_agvacancy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_agauthor';
  $strongarm->value = '50';
  $export['comment_default_per_page_agauthor'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_agconference';
  $strongarm->value = '50';
  $export['comment_default_per_page_agconference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_agcorporate_body';
  $strongarm->value = '50';
  $export['comment_default_per_page_agcorporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_agdlios';
  $strongarm->value = '50';
  $export['comment_default_per_page_agdlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_agevent';
  $strongarm->value = '50';
  $export['comment_default_per_page_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_aginstitution';
  $strongarm->value = '50';
  $export['comment_default_per_page_aginstitution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_agnews';
  $strongarm->value = '50';
  $export['comment_default_per_page_agnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_agprofile';
  $strongarm->value = '50';
  $export['comment_default_per_page_agprofile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_agrssjob';
  $strongarm->value = '50';
  $export['comment_default_per_page_agrssjob'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_agrssnews';
  $strongarm->value = '50';
  $export['comment_default_per_page_agrssnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_agvacancy';
  $strongarm->value = '50';
  $export['comment_default_per_page_agvacancy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agauthor';
  $strongarm->value = 0;
  $export['comment_form_location_agauthor'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agconference';
  $strongarm->value = 0;
  $export['comment_form_location_agconference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agcorporate_body';
  $strongarm->value = 0;
  $export['comment_form_location_agcorporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agdlios';
  $strongarm->value = 0;
  $export['comment_form_location_agdlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agevent';
  $strongarm->value = 0;
  $export['comment_form_location_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_aginstitution';
  $strongarm->value = '0';
  $export['comment_form_location_aginstitution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agnews';
  $strongarm->value = '0';
  $export['comment_form_location_agnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agperiodical';
  $strongarm->value = 1;
  $export['comment_form_location_agperiodical'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agprofile';
  $strongarm->value = 0;
  $export['comment_form_location_agprofile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agrssevents';
  $strongarm->value = '0';
  $export['comment_form_location_agrssevents'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agrssjob';
  $strongarm->value = '0';
  $export['comment_form_location_agrssjob'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agrssnews';
  $strongarm->value = '0';
  $export['comment_form_location_agrssnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_agvacancy';
  $strongarm->value = '0';
  $export['comment_form_location_agvacancy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agauthor';
  $strongarm->value = '0';
  $export['comment_preview_agauthor'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agconference';
  $strongarm->value = '2';
  $export['comment_preview_agconference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agcorporate_body';
  $strongarm->value = '2';
  $export['comment_preview_agcorporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agdlios';
  $strongarm->value = '2';
  $export['comment_preview_agdlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agevent';
  $strongarm->value = '2';
  $export['comment_preview_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_aginstitution';
  $strongarm->value = 2;
  $export['comment_preview_aginstitution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agnews';
  $strongarm->value = 2;
  $export['comment_preview_agnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agperiodical';
  $strongarm->value = '1';
  $export['comment_preview_agperiodical'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agprofile';
  $strongarm->value = '2';
  $export['comment_preview_agprofile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agrssevents';
  $strongarm->value = 2;
  $export['comment_preview_agrssevents'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agrssjob';
  $strongarm->value = 2;
  $export['comment_preview_agrssjob'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agrssnews';
  $strongarm->value = 2;
  $export['comment_preview_agrssnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_agvacancy';
  $strongarm->value = 2;
  $export['comment_preview_agvacancy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agconference';
  $strongarm->value = 0;
  $export['comment_subject_field_agconference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agcorporate_body';
  $strongarm->value = 1;
  $export['comment_subject_field_agcorporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agdlios';
  $strongarm->value = 1;
  $export['comment_subject_field_agdlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agevent';
  $strongarm->value = 1;
  $export['comment_subject_field_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_aginstitution';
  $strongarm->value = '1';
  $export['comment_subject_field_aginstitution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agnews';
  $strongarm->value = '1';
  $export['comment_subject_field_agnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agperiodical';
  $strongarm->value = 0;
  $export['comment_subject_field_agperiodical'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agprofile';
  $strongarm->value = 1;
  $export['comment_subject_field_agprofile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agrssevents';
  $strongarm->value = '1';
  $export['comment_subject_field_agrssevents'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agrssjob';
  $strongarm->value = '1';
  $export['comment_subject_field_agrssjob'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agrssnews';
  $strongarm->value = '1';
  $export['comment_subject_field_agrssnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_agvacancy';
  $strongarm->value = '1';
  $export['comment_subject_field_agvacancy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_agauthor';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '1',
    'revision_information' => '2',
    'author' => '3',
    'options' => '4',
    'comment_settings' => '5',
    'menu' => '0',
    'path' => '7',
    'attachments' => '6',
  );
  $export['content_extra_weights_agauthor'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_agconference';
  $strongarm->value = array(
    'title' => '-5',
    'revision_information' => '0',
    'author' => '2',
    'options' => '3',
    'comment_settings' => '1',
    'menu' => '-1',
    'path' => '4',
  );
  $export['content_extra_weights_agconference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_agcorporate_body';
  $strongarm->value = array(
    'title' => '-5',
    'revision_information' => '-1',
    'author' => '2',
    'options' => '3',
    'comment_settings' => '0',
    'menu' => '-2',
    'path' => '1',
  );
  $export['content_extra_weights_agcorporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_agdlios';
  $strongarm->value = array(
    'title' => '2',
    'revision_information' => '28',
    'author' => '22',
    'options' => '27',
    'comment_settings' => '29',
    'menu' => '30',
    'path' => '31',
    'attachments' => '25',
  );
  $export['content_extra_weights_agdlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_agevent';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '-4',
    'revision_information' => '1',
    'author' => '5',
    'options' => '6',
    'comment_settings' => '2',
    'menu' => '3',
    'path' => '4',
  );
  $export['content_extra_weights_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_ageventsfeed';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '-4',
    'revision_information' => '-1',
    'comment_settings' => '1',
    'menu' => '-2',
    'taxonomy' => '-3',
    'path' => '0',
  );
  $export['content_extra_weights_ageventsfeed'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_aginstitution';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '-2',
    'revision_information' => '3',
    'author' => '20',
    'options' => '25',
    'comment_settings' => '6',
    'menu' => '7',
    'path' => '4',
    'attachments' => '5',
  );
  $export['content_extra_weights_aginstitution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_agnews';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '-4',
    'revision_information' => '0',
    'author' => '3',
    'options' => '4',
    'comment_settings' => '1',
    'menu' => '-1',
    'path' => '2',
  );
  $export['content_extra_weights_agnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_agprofile';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '1',
    'revision_information' => '7',
    'author' => '12',
    'options' => '13',
    'comment_settings' => '11',
    'menu' => '10',
    'path' => '8',
    'attachments' => '9',
  );
  $export['content_extra_weights_agprofile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_agrssevents';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '-4',
    'revision_information' => '3',
    'author' => '6',
    'options' => '7',
    'comment_settings' => '5',
    'menu' => '2',
    'taxonomy' => '1',
    'path' => '4',
  );
  $export['content_extra_weights_agrssevents'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_agrssjob';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '-4',
    'revision_information' => '0',
    'author' => '20',
    'options' => '25',
    'comment_settings' => '2',
    'menu' => '3',
    'path' => '1',
    'attachments' => '-1',
  );
  $export['content_extra_weights_agrssjob'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_agrssnews';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '-3',
    'revision_information' => '0',
    'author' => '4',
    'options' => '5',
    'comment_settings' => '2',
    'menu' => '1',
    'taxonomy' => '-4',
    'path' => '3',
  );
  $export['content_extra_weights_agrssnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_agvacancy';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '-4',
    'revision_information' => '-1',
    'author' => '20',
    'options' => '25',
    'comment_settings' => '1',
    'menu' => '2',
    'path' => '0',
    'attachments' => '-2',
  );
  $export['content_extra_weights_agvacancy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_ag_author';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '1',
    'revision_information' => '3',
    'ag_author' => '6',
    'options' => '7',
    'comment_settings' => '4',
    'menu' => '2',
    'path' => '5',
  );
  $export['content_extra_weights_ag_author'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_author';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '1',
    'revision_information' => '3',
    'author' => '6',
    'options' => '7',
    'comment_settings' => '4',
    'menu' => '2',
    'path' => '5',
  );
  $export['content_extra_weights_author'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_conference';
  $strongarm->value = array(
    'title' => '-5',
    'revision_information' => '0',
    'author' => '2',
    'options' => '3',
    'comment_settings' => '1',
    'menu' => '-1',
    'path' => '4',
  );
  $export['content_extra_weights_conference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_corporate_body';
  $strongarm->value = array(
    'title' => '-5',
    'revision_information' => '0',
    'author' => '3',
    'options' => '4',
    'comment_settings' => '1',
    'menu' => '-1',
    'path' => '2',
  );
  $export['content_extra_weights_corporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_event';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '-4',
    'revision_information' => '2',
    'comment_settings' => '3',
    'menu' => '4',
    'path' => '5',
  );
  $export['content_extra_weights_event'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_institution';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '-2',
    'revision_information' => '3',
    'author' => '20',
    'options' => '25',
    'comment_settings' => '6',
    'menu' => '7',
    'path' => '4',
    'attachments' => '5',
  );
  $export['content_extra_weights_institution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_rssjob';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '-4',
    'revision_information' => '0',
    'author' => '20',
    'options' => '25',
    'comment_settings' => '2',
    'menu' => '3',
    'path' => '1',
    'attachments' => '-1',
  );
  $export['content_extra_weights_rssjob'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_rssnews';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '-3',
    'revision_information' => '0',
    'author' => '4',
    'options' => '5',
    'comment_settings' => '2',
    'menu' => '1',
    'taxonomy' => '-4',
    'path' => '3',
  );
  $export['content_extra_weights_rssnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_schema_version';
  $strongarm->value = 6009;
  $export['content_schema_version'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agconference';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'body' => array(
          'weight' => 0,
        ),
        'title' => array(
          'weight' => '-5',
        ),
        'revision_information' => array(
          'weight' => '0',
        ),
        'author' => array(
          'weight' => '2',
        ),
        'options' => array(
          'weight' => '3',
        ),
        'comment_settings' => array(
          'weight' => '1',
        ),
        'menu' => array(
          'weight' => '-1',
        ),
        'path' => array(
          'weight' => '4',
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(),
  );
  $export['field_bundle_settings_node__agconference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agcorporate_body';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'body' => array(
          'weight' => 0,
        ),
        'title' => array(
          'weight' => '0',
        ),
        'revision_information' => array(
          'weight' => '-1',
        ),
        'author' => array(
          'weight' => '2',
        ),
        'options' => array(
          'weight' => '3',
        ),
        'comment_settings' => array(
          'weight' => '0',
        ),
        'menu' => array(
          'weight' => '-2',
        ),
        'path' => array(
          'weight' => '3',
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(),
  );
  $export['field_bundle_settings_node__agcorporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agdlios';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'body' => array(
          'weight' => 0,
        ),
        'title' => array(
          'weight' => '1',
        ),
        'revision_information' => array(
          'weight' => '28',
        ),
        'author' => array(
          'weight' => '22',
        ),
        'options' => array(
          'weight' => '27',
        ),
        'comment_settings' => array(
          'weight' => '29',
        ),
        'menu' => array(
          'weight' => '30',
        ),
        'path' => array(
          'weight' => '27',
        ),
        'attachments' => array(
          'weight' => '25',
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => TRUE,
      ),
      'rss' => array(
        'custom_settings' => TRUE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => TRUE,
      ),
    ),
  );
  $export['field_bundle_settings_node__agdlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agevent';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
        'body' => array(
          'weight' => '-4',
        ),
        'revision_information' => array(
          'weight' => '1',
        ),
        'author' => array(
          'weight' => '5',
        ),
        'options' => array(
          'weight' => '6',
        ),
        'comment_settings' => array(
          'weight' => '2',
        ),
        'menu' => array(
          'weight' => '3',
        ),
        'path' => array(
          'weight' => '6',
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(),
  );
  $export['field_bundle_settings_node__agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__aginstitution';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
        'body' => array(
          'weight' => '-2',
        ),
        'revision_information' => array(
          'weight' => '3',
        ),
        'author' => array(
          'weight' => '20',
        ),
        'options' => array(
          'weight' => '25',
        ),
        'comment_settings' => array(
          'weight' => '6',
        ),
        'menu' => array(
          'weight' => '7',
        ),
        'path' => array(
          'weight' => '9',
        ),
        'attachments' => array(
          'weight' => '5',
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => TRUE,
      ),
      'rss' => array(
        'custom_settings' => TRUE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
    ),
  );
  $export['field_bundle_settings_node__aginstitution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agnews';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '-5',
        ),
        'body' => array(
          'weight' => '-4',
        ),
        'revision_information' => array(
          'weight' => '0',
        ),
        'author' => array(
          'weight' => '3',
        ),
        'options' => array(
          'weight' => '4',
        ),
        'comment_settings' => array(
          'weight' => '1',
        ),
        'menu' => array(
          'weight' => '-1',
        ),
        'path' => array(
          'weight' => '2',
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(),
  );
  $export['field_bundle_settings_node__agnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agperiodical';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
        'path' => array(
          'weight' => '13',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__agperiodical'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agprofile';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
        'body' => array(
          'weight' => '1',
        ),
        'revision_information' => array(
          'weight' => '7',
        ),
        'author' => array(
          'weight' => '12',
        ),
        'options' => array(
          'weight' => '13',
        ),
        'comment_settings' => array(
          'weight' => '11',
        ),
        'menu' => array(
          'weight' => '10',
        ),
        'path' => array(
          'weight' => '14',
        ),
        'attachments' => array(
          'weight' => '9',
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'search_index' => array(
        'custom_settings' => FALSE,
      ),
      'search_result' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
    ),
  );
  $export['field_bundle_settings_node__agprofile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agrssevents';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
        'body' => array(
          'weight' => '-4',
        ),
        'revision_information' => array(
          'weight' => '3',
        ),
        'author' => array(
          'weight' => '6',
        ),
        'options' => array(
          'weight' => '7',
        ),
        'comment_settings' => array(
          'weight' => '5',
        ),
        'menu' => array(
          'weight' => '2',
        ),
        'taxonomy' => array(
          'weight' => '1',
        ),
        'path' => array(
          'weight' => '8',
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(),
  );
  $export['field_bundle_settings_node__agrssevents'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agrssjob';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '-5',
        ),
        'body' => array(
          'weight' => '-4',
        ),
        'revision_information' => array(
          'weight' => '0',
        ),
        'author' => array(
          'weight' => '20',
        ),
        'options' => array(
          'weight' => '25',
        ),
        'comment_settings' => array(
          'weight' => '2',
        ),
        'menu' => array(
          'weight' => '3',
        ),
        'path' => array(
          'weight' => '1',
        ),
        'attachments' => array(
          'weight' => '-1',
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(),
  );
  $export['field_bundle_settings_node__agrssjob'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agrssnews';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '-5',
        ),
        'body' => array(
          'weight' => '-3',
        ),
        'revision_information' => array(
          'weight' => '0',
        ),
        'author' => array(
          'weight' => '4',
        ),
        'options' => array(
          'weight' => '5',
        ),
        'comment_settings' => array(
          'weight' => '2',
        ),
        'menu' => array(
          'weight' => '1',
        ),
        'taxonomy' => array(
          'weight' => '-4',
        ),
        'path' => array(
          'weight' => '3',
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(),
  );
  $export['field_bundle_settings_node__agrssnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__agvacancy';
  $strongarm->value = array(
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
        'body' => array(
          'weight' => '-4',
        ),
        'revision_information' => array(
          'weight' => '-1',
        ),
        'author' => array(
          'weight' => '20',
        ),
        'options' => array(
          'weight' => '25',
        ),
        'comment_settings' => array(
          'weight' => '1',
        ),
        'menu' => array(
          'weight' => '2',
        ),
        'path' => array(
          'weight' => '6',
        ),
        'attachments' => array(
          'weight' => '-2',
        ),
      ),
      'display' => array(),
    ),
    'view_modes' => array(),
  );
  $export['field_bundle_settings_node__agvacancy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agconference';
  $strongarm->value = '0';
  $export['language_content_type_agconference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agcorporate_body';
  $strongarm->value = '0';
  $export['language_content_type_agcorporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agdlios';
  $strongarm->value = '0';
  $export['language_content_type_agdlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agevent';
  $strongarm->value = '0';
  $export['language_content_type_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_aginstitution';
  $strongarm->value = '0';
  $export['language_content_type_aginstitution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agnews';
  $strongarm->value = '0';
  $export['language_content_type_agnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agperiodical';
  $strongarm->value = '0';
  $export['language_content_type_agperiodical'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agprofile';
  $strongarm->value = '0';
  $export['language_content_type_agprofile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agrssevents';
  $strongarm->value = '0';
  $export['language_content_type_agrssevents'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agrssjob';
  $strongarm->value = '0';
  $export['language_content_type_agrssjob'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agrssnews';
  $strongarm->value = '0';
  $export['language_content_type_agrssnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_agvacancy';
  $strongarm->value = '0';
  $export['language_content_type_agvacancy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_agevent';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_agevent';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agconference';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_agconference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agcorporate_body';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_agcorporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agdlios';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_agdlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agevent';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_aginstitution';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_aginstitution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agnews';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_agnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agperiodical';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_agperiodical'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agprofile';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_agprofile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agrssevents';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_agrssevents'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agrssjob';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_agrssjob'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agrssnews';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_agrssnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_agvacancy';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_agvacancy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agconference';
  $strongarm->value = '1';
  $export['node_preview_agconference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agcorporate_body';
  $strongarm->value = '1';
  $export['node_preview_agcorporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agdlios';
  $strongarm->value = '1';
  $export['node_preview_agdlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agevent';
  $strongarm->value = '1';
  $export['node_preview_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_aginstitution';
  $strongarm->value = 1;
  $export['node_preview_aginstitution'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agnews';
  $strongarm->value = 1;
  $export['node_preview_agnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agperiodical';
  $strongarm->value = '1';
  $export['node_preview_agperiodical'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agprofile';
  $strongarm->value = '1';
  $export['node_preview_agprofile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agrssevents';
  $strongarm->value = 1;
  $export['node_preview_agrssevents'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agrssjob';
  $strongarm->value = 1;
  $export['node_preview_agrssjob'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agrssnews';
  $strongarm->value = 1;
  $export['node_preview_agrssnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_agvacancy';
  $strongarm->value = 1;
  $export['node_preview_agvacancy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_agconference';
  $strongarm->value = 0;
  $export['node_submitted_agconference'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_agcorporate_body';
  $strongarm->value = 0;
  $export['node_submitted_agcorporate_body'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_agdlios';
  $strongarm->value = 0;
  $export['node_submitted_agdlios'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_agevent';
  $strongarm->value = 1;
  $export['node_submitted_agevent'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_agnews';
  $strongarm->value = 0;
  $export['node_submitted_agnews'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_agperiodical';
  $strongarm->value = 1;
  $export['node_submitted_agperiodical'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_agprofile';
  $strongarm->value = 1;
  $export['node_submitted_agprofile'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_agrssevents';
  $strongarm->value = 0;
  $export['node_submitted_agrssevents'] = $strongarm;

  return $export;
}
