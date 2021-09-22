<?php
/**
 * @file
 * scratchpads_events.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function scratchpads_events_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'events';
  $context->description = '';
  $context->tag = 'Events';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'event' => 'event',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'event' => 'event',
        'event/upcoming' => 'event/upcoming',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-event_calendar-block_1' => array(
          'module' => 'views',
          'delta' => 'event_calendar-block_1',
          'region' => 'sidebar',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Events');
  $export['events'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'events_calendar_pages';
  $context->description = '';
  $context->tag = 'Events';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'event/month' => 'event/month',
        'event/year' => 'event/year',
        'event/week' => 'event/week',
        'event/day' => 'event/day',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-event_calendar-block_2' => array(
          'module' => 'views',
          'delta' => 'event_calendar-block_2',
          'region' => 'sidebar',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Events');
  $export['events_calendar_pages'] = $context;

  return $export;
}
