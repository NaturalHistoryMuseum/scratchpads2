<?php

/**
 * @file
 * Please supply a file description.
 */

if (!function_exists('omega_theme_get_setting')) {
  require_once dirname(__FILE__) . '/omega.inc';
}

/**
 * Implements hook_drush_command().
 */
function omega_drush_command() {
  $items['omega-subtheme'] = array(
    'description' => dt('Creates a Omega subtheme.'),
    'arguments' => array(
      'name' => dt('The name of your subtheme.'),
    ),
    'options' => array(
      'destination' => dt('The destination of your subtheme. Defaults to "sites/all/themes".'),
      'machine-name' => dt('The machine-readable name of your subtheme. This will be auto-generated from the human-readable name if omitted.'),
      'starterkit' => dt('The starterkit that your subtheme should use. Defaults to "default".'),
      'basetheme' => dt('Specifies a custom base theme. Defaults to "omega".'),
      'enable' => dt('Automatically enable the subtheme after creation.'),
      'set-default' => dt('Automatically enable the subtheme after creation and make it the default theme.'),
      'no-readme' => dt('Skips readme files when generating the subtheme.'),
      'with-libraries' => dt("Execute the libraries.make file upon completion."),
    ),
    'examples' => array(
      'drush omega-subtheme "My Theme"' => dt('Creates an Omega subtheme called "My Theme".'),
      'drush omega-subtheme "My Theme" --destination=sites/example.com/themes' => dt('Creates an Omega subtheme called "My Theme" in sites/example.com/themes.'),
      'drush omega-subtheme "My Theme" --basetheme=my_custom_basetheme' => dt('Uses the default starterkit from a custom basetheme to create an Omega subtheme called "My Theme" in sites/all/themes.'),
      'drush omega-subtheme "My Theme" --basetheme=my_custom_basetheme --starterkit=my_custom_starterkit' => dt('Uses the my_custom_starterkit from a custom basetheme to create an Omega subtheme called "My Theme" in sites/all/themes.'),
    ),
    'aliases' => array('osub'),
  );

  $items['omega-wizard'] = array(
    'description' => dt('Guides you through a wizard for generating a subtheme.'),
    'aliases' => array('owiz'),
  );

  $items['omega-guard'] = array(
    'description' => dt('Runs guard for the given theme including Compass and LiveReload by default.'),
    'arguments' => array(
      'name' => dt('The name of your subtheme.'),
    ),
    'options' => array(
      'screen' => dt('Run guard watch in a detached screen.'),
      'force-polling' => dt('Polling is required for making guard watch work with remote file systems e.g. in case of virtual environments where guard runs on the guest but the files are modified on the host.'),
      'latency' => dt("Sometimes it seems to be required to set a latency (e.g. 5) if working with --force-polling because otherwise file changes are detected twice. Hence, not setting a latency might affect your system performance."),
    ),
    'aliases' => array('ogrd'),
  );

  $items['omega-export'] = array(
    'description' => dt('Exports the theme settings of a given theme from the database to the .info file.'),
    'arguments' => array(
      'theme' => dt('The machine-readable name of the theme to export the theme settings for.'),
    ),
    'options' => array(
      'revert' => dt('Purges the theme settings from the database after exporting them to the .info file.'),
    ),
    'examples' => array(
      'drush omega-export foo' => dt('Exports the theme settings of the "foo" theme to the "foo.info" file in that theme.'),
      'drush omega-export foo --revert' => dt('Purges the theme settings of the "foo" theme from the database after exporting them to the .info file.'),
    ),
    'aliases' => array('oexp'),
  );

  $items['omega-revert'] = array(
    'description' => dt('Reverts the theme settings of a given theme by deleting them from the database.'),
    'arguments' => array(
      'theme' => dt('The machine-readable name of the theme to revert the theme settings for.'),
    ),
    'options' => array(
      'all' => dt('Reverts the theme settings of all Omega sub-themes.'),
    ),
    'examples' => array(
      'drush omega-revert foo' => dt('Reverts the theme settings of the "foo" theme.'),
    ),
    'aliases' => array('orev'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function omega_drush_help($section) {
  switch ($section) {
    case 'drush:omega-subtheme':
      return dt('Generates a subtheme.');

    case 'drush:omega-wizard':
      return dt('Guides you through a wizard for generating a subtheme.');

    case 'drush:omega-guard':
      return dt('Runs guard for the given theme including Compass and LiveReload by default.');

    case 'drush:omega-export':
      return dt('Exports the theme settings of a given theme.');

    case 'drush:omega-revert':
      return dt('Reverts the theme settings of a given theme.');
  }
}

/**
 * Implements drush_hook_COMMAND_validate().
 */
function drush_omega_subtheme_validate($name = NULL) {
  if (!isset($name)) {
    return drush_set_error('OMEGA_MISSING_ARGUMENT', dt("You didn't specify a name for the subtheme."));
  }

  // Rebuild the theme data so that we can safely check for the existence of
  // themes by using the information provided by list_themes().
  system_rebuild_theme_data();

  if ($machine_name = drush_get_option('machine-name')) {
    // Validate the machine-readable name of the theme.
    if (!is_string($machine_name)) {
      return drush_set_error('OMEGA_THEME_NAME_INVALID', dt('The --machine-name option expects a string value.'));
    }

    if (!preg_match('/^[a-z][a-z0-9_]*$/', $machine_name)) {
      return drush_set_error('OMEGA_THEME_NAME_INVALID', dt('The machine name (@name) is invalid. It may only contain lowercase numbers, letters and underscores and must start with a letter.', array(
        '@name' => $machine_name,
      )));
    }

    $themes = list_themes();
    // Validate that the machine-readable name of the theme is unique.
    if (isset($themes[$machine_name])) {
      return drush_set_error('OMEGA_THEME_ALREADY_EXISTS', dt('A theme with the name @name already exists. The machine-readable name must be unique.', array(
        '@name' => $machine_name,
      )));
    }
  }
  // Check if the provided destination exists.
  $destination = drush_get_option('destination', 'sites/all/themes');
  if (!is_string($destination)) {
    return drush_set_error('OMEGA_DESTINATION_INVALID', dt('The --destination option expects a string value.'));
  }

  if ($basetheme = drush_get_option('basetheme')) {
    if (!is_string($basetheme)) {
      return drush_set_error('OMEGA_BASETHEME_INVALID', dt('The --basetheme option expects a string value.'));
    }

    // Check if the base theme exists.
    if (!array_key_exists($basetheme, list_themes())) {
      return drush_set_error('OMEGA_BASETHEME_DOES_NOT_EXIST', dt('The base theme @basetheme does not exist or is invalid.', array(
        '@basetheme' => $basetheme,
      )));
    }

    // Check if the base theme is an Omega theme.
    if (!array_key_exists('omega', omega_theme_trail($basetheme))) {
      return drush_set_error('OMEGA_BASETHEME_INVALID', dt('The given base theme is invalid (it has to be an Omega sub-theme or Omega itself).', array(
        '@basetheme' => $basetheme,
      )));
    }
  }

  if ($starterkit = drush_get_option('starterkit')) {
    if (!is_string($starterkit)) {
      return drush_set_error('OMEGA_STARTERKIT_INVALID', dt('The --starterkit option expects a string value.'));
    }

    $basetheme = drush_get_option('basetheme', 'omega');

    // Check if the starterkit exists.
    if (!array_key_exists($starterkit, omega_discovery('starterkit', $basetheme))) {
      $themes = list_themes();

      return drush_set_error('OMEGA_STARTERKIT_DOES_NOT_EXIST', dt('There is no valid @basetheme theme starterkit with the name @starterkit. Did you forget to specify the correct basetheme?', array(
        '@basetheme' => $themes[$basetheme]->info['name'],
        '@starterkit' => $starterkit,
      )));
    }
  }
}

/**
 * Implements drush_hook_COMMAND().
 */
function drush_omega_subtheme($name) {
  // Try to generate a machine-readable name. If that fails, prompt for one.
  if (!$machine_name = drush_get_option('machine-name', drush_omega_generate_theme_name($name))) {
    drush_print(dt("Sorry, I couldn't generate a machine-readable name for @name. Please use the '--machine-name' option to specify it manually.", array(
      '@name' => $name,
    )));
  }

  $basetheme = drush_get_option('basetheme', 'omega');
  $starterkits = omega_discovery('starterkit', $basetheme);
  $starterkit = drush_get_option('starterkit', 'default');
  $starterkit = $starterkits[$starterkit];

  // Check whether the destination path does not exist and bail out if it does
  // so we don't delete any important data by accident.
  $destination = drush_get_option('destination', 'sites/all/themes') . '/' . $machine_name;
  if (file_exists($destination)) {
    return drush_set_error('OMEGA_SUBTHEME_PATH', dt('The path @path already exists.', array('@path' => $destination)));
  }

  // Create a temporary directory so we don't leave any stale files if an
  // operation fails.
  $temporary = drush_tempdir() . '/' . $name;

  // Try to copy the starterkit to the destination path of the new subtheme.
  if (!drush_copy_dir($starterkit['path'], $temporary)) {
    return drush_set_error('OMEGA_GENERATE_SUBTHEME', dt('Failed to generate subtheme.'));
  }

  // Delete the .starterkit.inc file.
  drush_delete_dir($temporary . '/' . basename($starterkit['file']));

  // Put the name and description for the new subtheme in place.
  $info = array(
    'name' => $name,
    'description' => 'Please provide a description for your theme.',
    'base theme' => $basetheme,
  ) + $starterkit['info'];

  // If the starterkit does not provide any regions, use those defined in the
  // base theme (if any).
  if (empty($info['regions'])) {
    // We explicitly avoid reading from the generated theme info array for this
    // and get the raw, unprocessed .info file data of the base theme instead to
    // prevent any system- or module-provided regions to leak into our theme's
    // .info file (e.g. via hook_system_info_alter() as used by the dashboard
    // module).
    $basetheme_info = drupal_parse_info_file(drupal_get_path('theme', $basetheme) . "/$basetheme.info");
    if (!empty($basetheme_info['regions'])) {
      $info['regions'] = $basetheme_info['regions'];
    }
  }

  // Write to the new .info file.
  $file = $temporary . '/' . $machine_name . '.info';
  if (!file_put_contents($file, drush_omega_compose_info_file($info))) {
    return drush_set_error('OMEGA_GENERATE_SUBTHEME', dt('Failed to generate subtheme.'));
  }

  // Optionally remove README.txt files.
  if (drush_get_option('no-readme')) {
    foreach (file_scan_directory($temporary, '/^README/') as $file) {
      drush_delete_dir($file->uri);
    }
  }

  // Recursively rewrite the file names and contents of all the files that are
  // now in the subtheme's directory to represent the human- and
  // machine-readable names of the subtheme.
  $camel = strtr(ucwords(strtr($machine_name, array('_' => ' ', '.' => '_ '))), array(' ' => ''));
  $search = array(
    '/{{ THEME }}/' => $machine_name,
    '/{{ THEME NAME }}/' => $name,
    '/{{ THEME SANITIZED }}/' => str_replace('_', '-', $machine_name),
    '/{{ THEME CAMELCASE }}/' => $camel,
    '/{{ THEME CAMELCASE LOWER }}/' => lcfirst($camel),
  );

  if (!drush_omega_rewrite_recursive($temporary, array_keys($search), array_values($search))) {
    return drush_set_error('OMEGA_GENERATE_SUBTHEME', dt('Failed to generate subtheme.'));
  }

  // Move the new subtheme to its destination.
  if (!drush_op('drush_mkdir', dirname($destination)) || !drush_op('drush_move_dir', $temporary, $destination)) {
    return drush_set_error('OMEGA_GENERATE_SUBTHEME', dt('Failed to generate subtheme.'));
  }

  // Rebuild the theme caches so that  we can do some final tasks.
  drupal_theme_rebuild();
  system_rebuild_theme_data();

  if (($default = drush_get_option('set-default')) || !drush_get_option('enable')) {
    // Enable the subtheme.
    drush_op('theme_enable', array($machine_name));

    if ($default) {
      // Make the newly created subtheme the default theme.
      drush_op('variable_set', 'theme_default', $machine_name);
    }
  }

  // Execute the theme's libraries.make file unless disabled.
  if (drush_get_option('with-libraries') && is_file("$destination/libraries.make")) {
    drush_op('chdir', $destination);
    drush_invoke_process('@self', 'make', array('libraries.make'), array(
      'no-core' => TRUE,
      'contrib-destination' => '.',
      'yes' => TRUE,
    ));
  }

  drush_log(dt('You have successfully created the theme @theme (@name) in @path.', array(
    '@theme' => $name,
    '@name' => $machine_name,
    '@path' => dirname($destination),
  )), 'success');
}

/**
 * Implements drush_hook_COMMAND().
 */
function drush_omega_wizard() {
  // Rebuild the theme data so that we can safely check for the existence of
  // themes by using the information provided by list_themes().
  system_rebuild_theme_data();

  // Prompt for a theme name.
  $name = drush_prompt(dt('Please enter the name of the new sub-theme'), 'Omega Subtheme');

  // Try to generate a machine-readable name. If that fails, prompt for one.
  if (!$machine_name = drush_omega_generate_theme_name($name)) {
    drush_print(dt("Sorry, I couldn't generate a machine-readable name for @name", array(
      '@name' => $name,
    )));
  }
  // Prompt for a theme name using the automatically generated default if any.
  drush_set_option('machine-name', drush_omega_require_valid_theme_name(dt('Please enter a machine-readable name for your new theme'), $machine_name));

  // Prompt for a base theme.
  if (!$basetheme = drush_omega_theme_choice(dt('Please choose a base theme for your new theme'))) {
    $basetheme = 'omega';
  }
  drush_set_option('basetheme', $basetheme);

  // Let the user choose a starterkit.
  $starterkits = omega_discovery('starterkit', $basetheme);
  if (!$starterkit = drush_omega_starterkit_choice($starterkits, dt('Please choose a starterkit for your new theme'))) {
    $starterkit = 'default';
  }
  drush_set_option('starterkit', $starterkit);

  // Let the user choose a destination.
  $destination = drush_prompt(dt('Please choose a destination. This is where your sub-theme will be placed'), 'sites/all/themes');
  drush_set_option('destination', $destination);

  // Optionally skip readme files when generating the subtheme.
  drush_set_option('no-readme', !drush_confirm(dt("Do you want to keep the starterkit's readme files?")));

  // Finally, let the user choose to directly enable the subtheme.
  if ($enable = drush_confirm(dt('Do you want to enable your new theme?'))) {
    drush_set_option('set-default', drush_confirm(dt('Do you want to make your new theme the default theme?')));
  }
  drush_set_option('enable', $enable);

  // Finally, let the user decide whether he/she wants to download the libraries directly.
  if (is_file("{$starterkits[$starterkit]['path']}/libraries.make")) {
    drush_set_option('with-libraries', drush_confirm(dt('Do you want to download the libraries defined in libraries.make now?')));
  }

  drush_invoke('omega-subtheme', $name);
}

/**
 * Implements drush_hook_COMMAND_validate().
 */
function drush_omega_export_validate($theme = NULL) {
  return drush_omega_validate_theme($theme);
}

/**
 * Implements drush_hook_COMMAND().
 *
 * Exports the theme settings for the given theme from the database and writes
 * them into the .info file of that theme.
 *
 * @param string $theme
 *   (optional) The machine-readable name of a theme.
 *
 * @return bool
 *   TRUE on success, FALSE on failure.
 */
function drush_omega_export($theme = NULL) {
  if (!isset($theme) && !$theme = drush_omega_theme_choice(dt('Which theme do you want to export the theme settings for?'))) {
    return;
  }

  $themes = list_themes();

  // Insert the theme settings from the database.
  if (!$settings = variable_get('theme_' . $theme . '_settings')) {
    if (!drush_confirm(dt('There are no theme settings for @theme stored in the database. Do you want to purge the theme settings from the .info file too?', array('@theme' => $themes[$theme]->info['name'])))) {
      return;
    }
  }

  // Parse the current content of the .info file so we can append the settings
  // from the database.
  $path = drupal_get_path('theme', $theme) . '/' . $theme . '.info';
  $info = drupal_parse_info_file($path);

  unset($info['settings']);
  if (!empty($settings)) {
    // Append the exported theme settings to the .info file if there are any.
    $info['settings'] = $settings;
  }

  // Write the data to the .info file of the theme.
  if (drush_op('file_put_contents', $path, drush_omega_compose_info_file($info))) {
    drush_log(dt('The theme settings for the @theme theme have been exported to the .info file of the theme.', array('@theme' => $themes[$theme]->info['name'])), 'success');

    if (drush_get_option('revert')) {
      // Revert the theme settings if the 'revert' option is set and they have
      // been exported successfully. In this case, we invoke the API function
      // through the drush command to display the messages.
      drush_invoke_process('@self', 'omega-revert', array($theme));
    }

    return TRUE;
  }
  else {
    // There was an error while exporting the theme settings.
    return drush_set_error('OMEGA_EXPORT_ERROR', dt('An error occurred while trying to export the theme settings for the @theme theme.', array('@theme' => $themes[$theme]->info['name'])));
  }
}

/**
 * Implements drush_hook_COMMAND_validate().
 */
function drush_omega_revert_validate($theme = NULL) {
  return drush_omega_validate_theme($theme);
}

/**
 * Implements drush_hook_COMMAND().
 *
 * Delete the theme settings that have been stored in the database and thereby
 * reverts them to the default settings from the .info file.
 *
 * @param string $theme
 *   (optional) The machine-readable name of a theme.
 */
function drush_omega_revert($theme = NULL) {
  if (drush_get_option('all')) {
    // Get a list of all Omega sub-themes.
    $themes = array();
    foreach (list_themes() as $key => $info) {
      $trail = omega_theme_trail($key);
      if (array_key_exists('omega', $trail)) {
        $themes[$key] = $info->info['name'];
      }
    }

    // Get confirmation from user.
    drush_print(dt('The settings for the following themes will be reverted: @themes', array('@themes' => implode(', ', $themes))));
    if (!drush_confirm(dt('Do you really want to continue?'))) {
      return;
    }
  }
  elseif (!isset($theme) && !$theme = drush_omega_theme_choice(dt('Which theme do you want to revert the theme settings for?'))) {
    return;
  }

  $info = list_themes();
  $themes = isset($themes) ? $themes : array($theme => $info[$theme]->info['name']);
  foreach ($themes as $theme => $name) {
    // Delete the theme settings variable for the given theme.
    drush_op('variable_del', 'theme_' . $theme . '_settings');

    // Clear the theme cache.
    cache_clear_all('omega:' . $theme . ':', 'cache', TRUE);
  }

  // Rebuild the theme data for good measure.
  drupal_theme_rebuild();
  system_rebuild_theme_data();

  drush_log(dt('You have successfully reverted the theme settings for these themes: @themes.', array('@themes' => implode(', ', $themes))), 'success');
}

/**
 * Implements drush_hook_COMMAND_validate().
 */
function drush_omega_guard_validate($theme = NULL) {
  return drush_omega_validate_theme($theme);
}

/**
 * Implements drush_hook_COMMAND().
 *
 * Starts guard for the given theme for compass, theme registry rebuild and
 * livereload.
 *
 * @param string $theme
 *   (optional) The machine-readable name of a theme.
 */
function drush_omega_guard($theme = NULL) {
  if (!isset($theme) && !$theme = drush_omega_theme_choice(dt('Which theme do you want to run Guard for?'))) {
    return;
  }

  // Check if Ruby is installed.
  drush_shell_exec('ruby --version');
  $output = reset(drush_shell_exec_output());
  $matches = array();
  if (!preg_match('/^ruby ([0-9][0-9\.]*)/', $output, $matches)) {
    // Ruby was not found on this machine.
    return drush_set_error(dt('You have to install Ruby version 1.9 or newer.'));
  }
  elseif (!version_compare($matches[1], '1.8', '>=')) {
    // Ruby is outdated.
    return drush_set_error(dt('The installed version of Ruby (@version) is outdated. Please upgrade to version 1.9 or newer.', array(
      '@version' => $matches[1],
    )));
  }

  // Check if Rubygems is installed.
  drush_shell_exec('gem --version');
  $output = reset(drush_shell_exec_output());
  $matches = array();
  if (!preg_match('/^[0-9][0-9\.]*$/', $output, $matches)) {
    // Rubygems was not found on this machine.
    return drush_set_error(dt('You have to install Rubygems version 1.8 or newer.'));
  }
  elseif (!version_compare($matches[0], '1.8', '>=')) {
    // Rubygems is outdated.
    return drush_set_error(dt('The installed version of Rubygems (@version) is outdated. Please upgrade to version 1.8 or newer.', array(
      '@version' => $matches[0],
    )));
  }

  // Check if Bundler is installed.
  drush_shell_exec('bundle --version');
  $output = reset(drush_shell_exec_output());
  $matches = array();
  if (!preg_match('/^Bundler version ([0-9][0-9\.]*)/', $output, $matches)) {
    // Bundler was not found on this machine.
    return drush_set_error(dt('You have to install Bundler version 1.2 or newer.'));
  }
  elseif (!version_compare($matches[1], '1.2', '>=')) {
    // Bundler is outdated.
    return drush_set_error(dt('The installed version of Bundler (@version) is outdated. Please upgrade to version 1.2 or newer.', array(
      '@version' => $matches[1],
    )));
  }

  // Retrieve the path to the theme.
  $path = drupal_get_path('theme', $theme);

  // Output an error message if the gemfiles dependencies are not satisfied.
  drush_shell_cd_and_exec(DRUPAL_ROOT . '/' . $path, 'bundle check --no-color');
  $output = drush_shell_exec_output();
  switch (reset($output)) {
    case "The Gemfile's dependencies are satisfied":
      // All is good, we can proceed.
      break;

    case 'Could not locate Gemfile':
      return drush_set_error(dt('There was no Gemfile at @path.', array(
        '@path' => $path,
      )));

    case "Your Gemfile's dependencies could not be satisfied":
      // @todo Add prompt for running 'bundle install'.
    default:
      return drush_set_error(dt("There was a problem with your setup:\n!error", array(
        '!error' => implode("\n", $output),
      )));
  }

  // This is the command for running guard through bundler.
  $command = 'bundle exec guard';
  if (drush_get_option('screen')) {
    drush_shell_exec('screen --version');
    if (!preg_match('/^Screen version/', reset(drush_shell_exec_output()))) {
      // Screen was not found on this machine.
      return drush_set_error(dt("You have to install 'screen' before you can run 'omega-guard' in a detached screen."));
    }

    // Check if there is already a screen. Running multiple screens at the same
    // time ultimately eats up too much performance. This way we ensure that
    // you don't accidently run the same screen session multiple times.
    drush_shell_exec('screen -list');
    foreach (drush_shell_exec_output() as $output) {
      $output = trim($output);
      $matches = array();
      if (preg_match('/^((\d+)\.omega:' . $theme . ':guard)/', $output, $matches)) {
        $themes = list_themes();

        // Make sure that we only got one screen for each theme at a time.
        if (drush_confirm(dt("There is already a screen running for @theme. Do you want to restart it?", array('@theme' => $themes[$theme]->name)))) {
          drush_shell_exec("screen -S $matches[2] -X quit");
          // We found an existing guard session and killed it.
          break;
        }
        else {
          // Fine, if the user does not want us to kill the session he has to do
          // it manually.
          return drush_set_error(dt("There is already a screen running for the @theme theme (@screen). You have to kill the runing screen with '@command' before you can start a new one.", array(
            '@theme' => $themes[$theme]->name,
            '@screen' => $matches[1],
            '@command' => "screen -S $matches[2] -X quit",
          )));
        }
      }
    };

    // We want to run the command in a detached (-dmS) screen.
    $command = "screen -dmS omega:$theme:guard $command";
  }

  // Polling is required for making guard watch work with remote file systems
  // e.g. in case of virtual environments where guard runs on the guest but
  // the files are modified on the host.
  if (drush_get_option('force-polling')) {
    $command .= ' --force-polling';
  }

  // Sometimes it seems to be required to set a latency (e.g. 5) if working with
  // polling because otherwise the file changes are detected twice. Hence,
  // not setting a latency might affect your system performance. If it doesn't
  // simply don't worry about it.
  if ($latency = drush_get_option('latency')) {
    $command .= ' --latency ' . $latency;
  }

  // Change the active directory to the theme folder and run the command there.
  drush_op('chdir', DRUPAL_ROOT . '/' . drupal_get_path('theme', $theme));
  drush_shell_exec_interactive($command);
}

/**
 * Helper function for printing a list of available Omega themes.
 *
 * @param string $message
 *   The message that should be displayed.
 *
 * @return bool|string
 *   The machine-readable name of the chosen theme or FALSE if the operation was
 *   cancelled.
 */
function drush_omega_theme_choice($message) {
  $options = array();
  foreach (list_themes() as $key => $info) {
    $trail = omega_theme_trail($key);
    if (array_key_exists('omega', $trail)) {
      $parent = count($trail) > 1 ? array_slice($trail, -2, 1) : FALSE;
      $options[$key] = $info->info['name'] . ($parent ? ' (' . dt('Subtheme of @parent', array('@parent' => reset($parent))) . ')' : '') . ' - ' . strip_tags($info->info['description']);
    }
  }
  return drush_choice($options, $message);
}

/**
 * Helper function for printing a list of available starterkits.
 *
 * @param array $starterkits
 *   An array of starterkits.
 * @param string $message
 *   The message that should be displayed.
 *
 * @return bool|string
 *   The machine-readable name of the chosen starterkit or FALSE if the
 *   operation was cancelled.
 */
function drush_omega_starterkit_choice($starterkits, $message) {
  $themes = list_themes();
  $options = array();
  foreach ($starterkits as $key => $info) {
    $options[$key] = dt('@name: !description (Provided by @provider)', array(
      '@name' => $info['info']['name'],
      '!description' => isset($info['info']['description']) ? $info['info']['description'] : dt('No description'),
      '@provider' => $themes[$info['theme']]->info['name'],
    ));
  }
  return drush_choice($options, $message);
}

/**
 * Helper function that continuously prompts for a valid machine-readable name.
 *
 * @param string $message
 *   The message that should be displayed.
 * @param string $default
 *   (Optional) The default theme name. Defaults to NULL.
 *
 * @return string
 *   A valid, unique machine-readable name.
 */
function drush_omega_require_valid_theme_name($message, $default = NULL) {
  while (TRUE) {
    // Keep prompting for a machine-name until we get an acceptable value.
    $prompt = drush_prompt($message, $default);

    if (!preg_match('/^[a-z][a-z0-9_]*$/', $prompt)) {
      drush_print('The machine-readable name is invalid. It may only contain lowercase numbers, letters and underscores and must start with a letter.');
    }
    else {
      $themes = list_themes();
      // Validate that the machine-readable name of the theme is unique.
      if (isset($themes[$prompt])) {
        drush_print(dt('A theme with the name @name already exists. The machine-readable name must be unique.', array(
          '@name' => $prompt,
        )));
      }
      else {
        // The given machine-readable name is valid. Let's proceed.
        return $prompt;
      }
    }
  }
}

/**
 * Recursively rewrites (and renames) all files in a given path.
 *
 * @param string $path
 *   The path to rewrite all files in.
 * @param string|array $search
 *   The string(s) to look for when replacing the file names and contents. Can
 *   be an array or a string.
 * @param string|array $replace
 *   The string(s) to replace $search with. Can be an array or a string.
 *
 * @return bool
 *   TRUE if the operation succeeded, FALSE otherwise.
 *
 * @see omega_drush_replace_contents()
 * @see str_replace()
 */
function drush_omega_rewrite_recursive($path, $search, $replace) {
  if (!is_dir($path)) {
    return drush_set_error('INVALID_PATH', dt('The given path @path is not a directory.', array(
      '!path' => $path,
    )));
  }

  // If the file actually is a directory, proceed with the recursion.
  $directory = new DirectoryIterator($path);
  foreach ($directory as $item) {
    if ($item->isDot()) {
      // Do not process '..' and '.'.
      continue;
    }

    // Retrieve the path of the current item.
    $pathname = $item->getPathname();
    if ($item->isDir() && !drush_omega_rewrite_recursive($pathname, $search, $replace)) {
      return FALSE;
    }
    elseif ($item->isFile()) {
      // If it is a file, try to replace its contents.
      $contents = file_get_contents($pathname);
      if (($changed = preg_replace($search, $replace, $contents)) === NULL) {
        return drush_set_error('REWRITE_FAILURE', dt('There was an error while trying to rewrite !path (!search to !replace)', array(
          '!path' => $pathname,
          '!search' => $search,
          '!replace' => $replace,
        )));
      }

      if ($contents !== $changed) {
        file_put_contents($pathname, $changed);
      }
    }

    // Try to rename (move) the file if the name was changed.
    $original = basename($pathname);
    if (($renamed = preg_replace($search, $replace, $original)) === NULL) {
      return drush_set_error('REWRITE_FAILURE', dt('There was an error while trying to rewrite !path (!search to !replace)', array(
        '!path' => $path,
        '!search' => $search,
        '!replace' => $replace,
      )));
    }

    // Move (rename) if the file or directory name was changed.
    if ($original !== $renamed) {
      $new = dirname($pathname) . "/$renamed";
      if (!drush_move_dir($pathname, $new, TRUE)) {
        return FALSE;
      };
    }
  }

  return TRUE;
}

/**
 * Recursively builds an .info file structure from an array.
 *
 * This function will take the current theme information, and create the export
 * based upon the current info file and settings saved within the database.
 *
 * @param array $array
 *   The array to build the .info file from.
 * @param string|bool $prefix
 *   (Optional) Used internally to forward the current prefix (level of nesting)
 *   for the keys. Defaults to NULL.
 *
 * @return string
 *   The full, prettified .info file.
 */
function drush_omega_compose_info_file($array, $prefix = '') {
  $info = '';
  $incremental = !empty($array) && strval(key($array)) === '0' && array_keys($array) === range(0, count($array) - 1);
  foreach ($array as $key => $value) {
    if (is_array($value)) {
      if (empty($prefix)) {
        $region = ucwords(strtr($key, array('_' => ' ')));
        $info .= "\n";
        $info .= "; ========================================\n";
        $info .= "; $region\n";
        $info .= "; ========================================\n";
      }

      $info .= drush_omega_compose_info_file($value, (empty($prefix) ? $key : "{$prefix}[{$key}]"));
    }
    else {
      $key = $incremental ? '' : $key;
      $value = str_replace("'", "\'", $value);
      $value = strval($value) === '' ? '"' . $value . '"' : $value;
      $info .= $prefix ? ("{$prefix}[$key]") : $key;
      $info .= " = $value\n";
    }
  }

  return $info;
}

/**
 * Retrieve an array of available sites.
 *
 * @return array
 *   An array that contains all the available sites in a multisite environment.
 */
function drush_omega_sites() {
  $sites = array();
  // Look up the available sites by iterating over the contents of the sites
  // directory.
  $files = new DirectoryIterator(DRUPAL_ROOT . '/sites');
  foreach ($files as $file) {
    // The sites/default folder is not a valid destination.
    if ($file->isDir() && !$file->isDot() && $file->getFileName() != 'default') {
      $name = $file->getFileName();
      $sites[$name] = $name;
    }
  }
  return $sites;
}

/**
 * Generates a valid machine-readable name for a theme from any string.
 *
 * @param string $string
 *   The string to generate the machine-readable name from.
 *
 * @return string
 *   The generated machine-readable name.
 */
function drush_omega_generate_theme_name($string) {
  // Machine-readable names have to start with a lowercase letter.
  $string = preg_replace('/^[^a-z]+/', '', strtolower($string));
  // Machine-readable names may only contain alphanumeric characters and
  // underscores.
  $string = preg_replace('/[^a-z0-9_]+/', '_', $string);
  // Trim all trailing and leading underscores.
  $string = trim($string, '_');

  $themes = list_themes();
  if (isset($themes[$string])) {
    $plain = $string;
    $counter = 0;

    while (isset($themes[$string])) {
      // Make sure that the machine-readable name of the theme is unique.
      $string = $plain . '_' . $counter++;
    }
  }

  return $string;
}

/**
 * Helper function for validating a given theme.
 */
function drush_omega_validate_theme($theme) {
  if (!isset($theme)) {
    return;
  }

  // Rebuild the theme data so that we can safely check for the existence of
  // themes by using the information provided by list_themes().
  system_rebuild_theme_data();

  $themes = list_themes();
  // Check if the given theme exists.
  if (!isset($themes[$theme])) {
    return drush_set_error('OMEGA_THEME_DOES_NOT_EXIST', dt('There is no theme with the name @theme.', array(
      '@theme' => $theme,
    )));
  }
}
