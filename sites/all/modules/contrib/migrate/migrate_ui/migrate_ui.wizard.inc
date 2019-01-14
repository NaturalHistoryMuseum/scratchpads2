<?php

/**
 * @file
 * Migration wizard framework.
 */

/**
 * The primary formbuilder function for the wizard form.
 *
 * This form has two defined submit handlers to process the different steps:
 *  - Previous: handles the way to get back one step in the wizard.
 *  - Next:     handles each step form submission,
 *
 * The third handler, the finish button handler, is the default form _submit
 * handler used to process the information.
 *
 * @param string $class_name
 *  Name of the MigrateUIWizard clsas for this wizard.
 */
function migrate_ui_wizard($form, &$form_state, $class_name = '') {
  // Rather than track state in $form_state, we simply keep our wizard
  // instance there, and it encapsulates all the state. We just need
  // to create the instance the first time in, and it will be serialized
  // between steps.
  /** @var MigrateUIWizard $wizard */
  if (empty($form_state['wizard'])) {
    $wizard = new $class_name();

    // Add any extenders.
    $module_apis = migrate_get_module_apis();
    // Need a second pass at this to add wizard extenders.
    foreach ($module_apis as $module => $info) {
      // Add any extenders.
      // @todo: consider allowing extender classes to declare dependencies on
      // other extender classes, to ensure they work in the correct order?
      if (isset($info['wizard extenders'])) {
        foreach ($info['wizard extenders'] as $wizard_class => $extender_classes) {
          // Note that $class_name is in lower case, so we can't just use isset()
          // to find our wizard.
          if (strtolower($wizard_class) == $class_name) {
            foreach ($extender_classes as $extender_class) {
              $wizard->addExtender($extender_class);
            }
          }
        }
      }
    }

    $form_state['wizard'] = $wizard;
  }
  else {
    $wizard = $form_state['wizard'];
  }

  // Fetch the form for the wizard's current step.
  $form = $wizard->form($form_state);

  return $form;
}

/**
 * Submit handler for the "previous" button. Moves the wizard back to the
 * previous step, and retrieves the values that were submitted on that step.
 *
 * @todo: Can we remove steps that were dynamically added?
 */
function migrate_ui_wizard_previous_submit($form, &$form_state) {
  /** @var MigrateUIWizard $wizard */
  $wizard = $form_state['wizard'];
  $wizard->gotoPreviousStep($form_state);
}

/**
 * Validate handler for the 'next' button. Dispatches to the wizard's current
 * step for validation.
 */
function migrate_ui_wizard_next_validate($form, &$form_state) {
  /** @var MigrateUIWizard $wizard */
  $wizard = $form_state['wizard'];
  $wizard->formValidate($form_state);
}

/**
 * Submit handler for the 'next' button. Saves the form values for the step
 * we're leaving, so Previous can pick them up, and moves the wizard to the
 * next step.
 */
function migrate_ui_wizard_next_submit($form, &$form_state) {
  /** @var MigrateUIWizard $wizard */
  $wizard = $form_state['wizard'];
  $wizard->gotoNextStep($form_state);
}

/**
 * Submit handler for the Save settings button. Register the migrations that
 * were
 * (implicitly) defined along the way and redirect to the Migrate dashboard.
 */
function migrate_ui_wizard_submit($form, &$form_state) {
  /** @var MigrateUIWizard $wizard */
  $wizard = $form_state['wizard'];
  $wizard->formSaveSettings();
  $form_state['redirect'] = 'admin/content/migrate/groups/' .
    $wizard->getGroupName();
}

/**
 * Submit handler for the "Save settings and import" button. Register the
 * migrations that were (implicitly) defined along the way, run the import, and
 * redirect to the Migrate dashboard.
 */
function migrate_ui_wizard_migrate_submit($form, &$form_state) {
  /** @var MigrateUIWizard $wizard */
  $wizard = $form_state['wizard'];
  $wizard->formSaveSettings();
  $wizard->formPerformImport();
  $form_state['redirect'] = 'admin/content/migrate/groups/' .
    $wizard->getGroupName();
}

/**
 * The base class for migration wizards. Extend this class to implement a
 * wizard UI for importing into Drupal from a given source format (Drupal,
 * WordPress, etc.).
 */
abstract class MigrateUIWizard {

  /**
   * We maintain a doubly-linked list of wizard steps, both to support
   * previous/next, and to easily insert steps dynamically.
   *
   * The first step of the wizard, which has no predecessor. Will generally be
   * an overview/introductory page.
   *
   * @var MigrateUIStep
   */
  protected $firstStep;

  /**
   * The last step of the wizard, which has no successor. Will generally be a
   * review page.
   *
   * @var MigrateUIStep
   */
  protected $lastStep;

  /**
   * Get the list of steps currently defined.
   *
   * @return
   *  An array of MigrateUIStep objects, in the order defined, keyed by the step
   *  name.
   */
  protected function getSteps() {
    $steps = array();

    $steps[$this->firstStep->getName()] = $this->firstStep;
    $next_step = $this->firstStep->nextStep;
    while (!is_null($next_step)) {
      $steps[$next_step->getName()] = $next_step;

      $next_step = $next_step->nextStep;
    }

    return $steps;
  }

  /**
   * The current step of the wizard (the one being shown in the UI, and the one
   * whose button is being clicked on).
   *
   * @var MigrateUIStep
   */
  protected $currentStep;

  /**
   * The step number, used in the page title.
   *
   * @var int
   */
  protected $stepNumber = 1;

  /**
   * The group name to assign to any Migration instances created.
   *
   * @var string
   */
  protected $groupName = 'default';

  public function getGroupName() {
    return $this->groupName;
  }

  /**
   * The user-visible title of the group.
   *
   * @var string
   */
  protected $groupTitle = 'default';

  /**
   * Any arguments that apply to all migrations in the group.
   *
   * @var array
   */
  protected $groupArguments = array();

  /**
   * Array of Migration argument arrays, keyed by machine name. On Finish, used
   * to register Migrations.
   *
   * @var array
   */
  protected $migrations = array();

  /**
   * Array of MigrateUIWizardExtender objects that extend this wizard.
   *
   * @var array
   */
  protected $extenders = array();

  public function getExtender($extender_class) {
    if (isset($this->extenders[$extender_class])) {
      return $this->extenders[$extender_class];
    }
    else {
      return NULL;
    }
  }

  /**
   * Returns the translatable name representing the source of the data (e.g.,
   * "Drupal", "WordPress", etc.).
   *
   * @return string
   */
  abstract public function getSourceName();

  public function __construct() {
  }

  /**
   * Add a wizard extender.
   *
   * This initializes the new extender and adds it to our internal list.
   *
   * @param $extender_class
   *  The name of an extender class.
   */
  public function addExtender($extender_class) {
    $steps = $this->getSteps();

    $extender = new $extender_class($this, $steps);
    $this->extenders[$extender_class] = $extender;
  }

  /**
   * Add a step to the wizard, using a step name and method.
   *
   * @param string $name
   *  Translatable name for the step, to be used in the page title.
   * @param callable $form_method
   *  Callable returning the form array for the step. This can be either the
   *  name of a MigrateUIWizard method, or a callable array specifying a method
   *  on a wizard extender. The validation method is formed from the method's
   *  name with the suffix 'Validate' added.
   * @param MigrateUIStep $after
   *  Optional step after which to insert the new step. If omitted, add it at
   *  the end.
   * @param mixed $context
   *  Optional data to be used by this step's form.
   *
   * @return MigrateUIStep
   */
  public function addStep($name, $form_method, MigrateUIStep $after = NULL, $context = NULL) {
    if (!is_array($form_method)) {
      $form_method = array($this, $form_method);
    }

    $new_step = new MigrateUIStep($name, $form_method, $context);

    // There were no steps, so this is the only one.
    if (is_null($this->firstStep)) {
      $this->firstStep = $this->lastStep = $this->currentStep = $new_step;
    }
    else {
      // If no insertion point is specified, append to the end.
      if (is_null($after)) {
        $after = $this->lastStep;
      }
      // Do the insert, rewriting the links appropriately.
      $new_step->nextStep = $after->nextStep;

      if (is_null($new_step->nextStep)) {
        $this->lastStep = $new_step;
      }
      else {
        $new_step->nextStep->previousStep = $new_step;
      }
      $new_step->previousStep = $after;
      $after->nextStep = $new_step;
    }
    return $new_step;
  }

  /**
   * Remove the named step from the wizard.
   *
   * @param $name
   */
  protected function removeStep($name) {
    for ($current_step = $this->firstStep; !is_null($current_step); $current_step = $current_step->nextStep) {
      if ($current_step->getName() == $name) {
        if (is_null($current_step->previousStep)) {
          $this->firstStep = $current_step->nextStep;
        }
        else {
          $current_step->previousStep->nextStep = $current_step->nextStep;
        }
        if (is_null($current_step->nextStep)) {
          $this->lastStep = $current_step->previousStep;
        }
        else {
          $current_step->nextStep->previousStep = $current_step->previousStep;
        }
        break;
      }
    }
  }

  /**
   * Move the wizard to the next step in line (if any), first squirreling away
   * the current step's form values.
   */
  public function gotoNextStep(&$form_state) {
    if ($this->currentStep && $this->currentStep->nextStep) {
      $this->currentStep->setFormValues($form_state['values']);
      $form_state['rebuild'] = TRUE;
      $this->currentStep = $this->currentStep->nextStep;
      $this->stepNumber++;

      // Ensure a page reload remains on the current step.
      $current_step_form_values = $this->currentStep->getFormValues();
      if (!empty($current_step_form_values)) {
        $form_state['values'] = $current_step_form_values;
      }
      else {
        $form_state['values'] = array();
      }
    }
  }

  /**
   * Move the wizard to the previous step in line (if any), restoring its
   * form values.
   */
  public function gotoPreviousStep(&$form_state) {
    if ($this->currentStep && $this->currentStep->previousStep) {
      $this->currentStep = $this->currentStep->previousStep;
      $this->stepNumber--;
      $form_state['values'] = $this->currentStep->getFormValues();
      $form_state['rebuild'] = TRUE;
    }
  }

  /**
   * Build the form for the current step.
   *
   * @return array
   */
  public function form(&$form_state) {
    drupal_set_title(t('Import from @source_title',
      array('@source_title' => $this->getSourceName())));

    $form_method = $this->currentStep->getFormMethod();
    $form['title'] = array(
      '#prefix' => '<h2>',
      '#markup' => t('Step @step: @step_name',
        array(
          '@step' => $this->stepNumber,
          '@step_name' => $this->currentStep->getName(),
        )),
      '#suffix' => '</h2>',
    );

    $form += call_user_func_array($form_method, array(&$form_state));

    $form['actions'] = array('#type' => 'actions');

    // Show the 'previous' button if appropriate. Note that #submit is set to
    // a special submit handler, and that we use #limit_validation_errors to
    // skip all complaints about validation when using the back button. The
    // values entered will be discarded, but they will not be validated, which
    // would be annoying in a "back" button.
    if ($this->currentStep != $this->firstStep) {
      $form['actions']['prev'] = array(
        '#type' => 'submit',
        '#value' => t('Previous'),
        '#name' => 'prev',
        '#submit' => array('migrate_ui_wizard_previous_submit'),
        '#limit_validation_errors' => array(),
      );
    }

    // Show the Next button only if there are more steps defined.
    if ($this->currentStep == $this->lastStep) {
      $form['actions']['finish'] = array(
        '#type' => 'submit',
        '#value' => t('Save import settings'),
      );

      $form['actions']['migrate'] = array(
        '#type' => 'submit',
        '#value' => t('Save import settings and run import'),
        '#submit' => array('migrate_ui_wizard_migrate_submit'),
      );
    }
    else {
      $form['actions']['next'] = array(
        '#type' => 'submit',
        '#value' => t('Next'),
        '#name' => 'next',
        '#submit' => array('migrate_ui_wizard_next_submit'),
        '#validate' => array('migrate_ui_wizard_next_validate'),
      );
    }
    return $form;
  }

  /**
   * Call the validation function for the current form (which has the same
   * name of the form function with 'Validate' appended).
   *
   * @param array $form_state
   */
  public function formValidate(&$form_state) {
    $validate_method = $this->currentStep->getFormMethod();

    // This is an array for a method, or a function name.
    if (is_array($validate_method)) {
      $validate_method[1] .= 'Validate';
    }
    else {
      $validate_method .= 'Validate';
    }

    if (is_callable($validate_method)) {
      call_user_func_array($validate_method, array(&$form_state));
    }
  }

  /**
   * Take the information we've accumulated throughout the wizard, and create
   * the Migrations to perform the import.
   */
  public function formSaveSettings() {
    MigrateGroup::register($this->groupName, $this->groupTitle, $this->groupArguments);
    $info['arguments']['group_name'] = $this->groupName;
    foreach ($this->migrations as $machine_name => $info) {
      // Call the right registerMigration implementation. Note that this means
      // that classes that override registerMigration() must handle registration
      // themselves, they cannot leave it to us and expect their extension to be
      // called.
      if (is_subclass_of($info['class_name'], 'Migration')) {
        Migration::registerMigration($info['class_name'], $machine_name,
          $info['arguments']);
      }
      else {
        MigrationBase::registerMigration($info['class_name'], $machine_name,
          $info['arguments']);
      }
    };
    menu_rebuild();
  }

  /**
   * Run the import process for the migration group we've defined.
   */
  public function formPerformImport() {
    $migrations = migrate_migrations();
    $operations = array();
    /** @var Migration $migration */
    foreach ($migrations as $migration) {
      $group_name = $migration->getGroup()->getName();
      if ($group_name == $this->groupName) {
        $operations[] = array(
          'migrate_ui_batch',
          array('import', $migration->getMachineName(), NULL, 0),
        );
      }
    }

    if (count($operations) > 0) {
      $batch = array(
        'operations' => $operations,
        'title' => t('Import processing'),
        'file' => drupal_get_path('module', 'migrate_ui') . '/migrate_ui.pages.inc',
        'init_message' => t('Starting import process'),
        'progress_message' => t(''),
        'error_message' => t('An error occurred. Some or all of the import processing has failed.'),
        'finished' => 'migrate_ui_batch_finish',
      );
      batch_set($batch);
    }
  }

  /**
   * Record all the information necessary to register a migration when this is
   * all over.
   *
   * @param string $machine_name
   *  Machine name for the migration class.
   * @param string $class_name
   *  Name of the Migration class to instantiate.
   * @param array $arguments
   *  Further information configuring the migration.
   */
  public function addMigration($machine_name, $class_name, $arguments) {
    // Give extenders an opportunity to modify or reject this migration.
    foreach ($this->extenders as $extender) {
      if (!$extender->addMigrationAlter($machine_name, $class_name, $arguments, $this)) {
        return FALSE;
      }
    }

    $machine_name = $this->groupName . $machine_name;
    if (isset($arguments['dependencies'])) {
      foreach ($arguments['dependencies'] as $index => $dependency) {
        $arguments['dependencies'][$index] = $this->groupName . $dependency;
      }
    }
    if (isset($arguments['soft_dependencies'])) {
      foreach ($arguments['soft_dependencies'] as $index => $dependency) {
        $arguments['soft_dependencies'][$index] = $this->groupName . $dependency;
      }
    }
    $arguments += array(
      'group_name' => $this->groupName,
      'machine_name' => $machine_name,
    );
    $this->migrations[$machine_name] = array(
      'class_name' => $class_name,
      'arguments' => $arguments,
    );

    return TRUE;
  }
}

/**
 * Class representing one step of a wizard.
 */
class MigrateUIStep {

  /**
   * A translatable string briefly describing this step, to be used in the page
   * title for the step form.
   *
   * @var string
   */
  protected $name;

  public function getName() {
    return $this->name;
  }

  /**
   * Callable that returns the form array for this step.
   *
   * @var string
   */
  protected $formMethod;

  public function getFormMethod() {
    return $this->formMethod;
  }

  /**
   * The form values ($form_state['values']) submitted for this step, saved in
   * case we need to restore them on a Previous action.
   *
   * @var array
   */
  protected $formValues;

  public function getFormValues() {
    return $this->formValues;
  }

  public function setFormValues($form_values) {
    $this->formValues = $form_values;
  }

  /**
   * Any contextual data needed by the form for this step. For example, a
   * field mapping form would need to know the source and destination content
   * types so it can determine what fields to expose.
   *
   * @var mixed
   */
  protected $context;

  public function getContext() {
    return $this->context;
  }

  /**
   * The step object is a node in a doubly-linked list - it links to its
   * predecessor and successor steps.
   *
   * @var MigrateUIStep
   */
  public $nextStep;

  /**
   * @var MigrateUIStep
   */
  public $previousStep;

  /**
   * Class constructor.
   *
   * @param $name
   *  The machine name of the wizard step.
   * @param $form_method
   *  A callable for the form array for this step. The validation method is
   *  formed from the method name with the suffix 'Validate' added, regardless
   *  of which object it is on.
   * @param $context = NULL
   *  Contextual data needed by the form for this step.
   */
  public function __construct($name, $form_method, $context = NULL) {
    $this->name = $name;
    $this->formMethod = $form_method;
    $this->context = $context;
  }
}

/**
 *
 */
abstract class MigrateUIWizardExtender {

  /**
   * Reference to the wizard object that this extender applies to.
   */
  protected $wizard;

  /**
   * Class constructor.
   *
   * Wizard extenders should override this to add their steps to the wizard.
   */
  public function __construct(MigrateUIWizard $wizard, array $wizard_steps) {
    $this->wizard = $wizard;
  }

  /**
   * Alter the arguments to a migration before it is registered, or potentially
   * reject it.
   *
   * @param string $machine_name
   *  Machine name for the migration class.
   * @param string $class_name
   *  Name of the Migration class to instantiate.
   * @param array $arguments
   *  Further information configuring the migration.
   * @param MigrateUIWizard $wizard
   *  The wizard class performing the registration.
   *
   * @return bool
   *  Return FALSE to prevent registration of this migration.
   */
  public function addMigrationAlter($machine_name, $class_name, &$arguments, $wizard) {
    return TRUE;
  }
}
