<?php

/**
 * Return the form which allows us to set the Registry to use.
 */
use GBIF\Registry;

define('GBIF_REGISTRY_OBJECTS_PER_PAGE', 20);

function gbif_registry_admin_form()
{
    return system_settings_form(array(
        'gbif_registry_web_service_url' => array(
            '#type' => 'textfield',
            '#title' => t('GBIF Registry web service URL'),
            '#default_value' => variable_get('gbif_registry_web_service_url', 'http://api.gbif-uat.org/v1/')
        )
    ));
}

/**
 * Validate the data sent from the settings form.
 */
function gbif_registry_admin_form_validate(&$form, &$form_state)
{
  // We check that the Registry URL ends with a slash.
    if (substr($form_state['values']['gbif_registry_web_service_url'], -1) != '/') {
        form_set_error('gbif_registry_web_service_url', t('The web service URL must end with a "/".'));
    }
}

/**
 * Display a single GBIF Registry object
 */
function gbif_registry_view_object($type = false, $id = false)
{
    if (!$type || !$id) {
        return t('An example object is shown below.');
    } else {
    // Create the registry object first to ensure we're using our cache one!
        $registry = GBIFRegistry::singleton();
        if (class_exists('GBIF\\' . $type)) {
            $class = gbif_registry_create_instance('GBIF\\' . $type, array(
                $id
            ));
            $rows = array(
                array(
                    'Type',
                    check_plain(implode(" ", preg_split('/(?<=\\w)(?=[A-Z])/', $type)))
                )
            );
            $content = array(
                'title' => array(
                    '#markup' => '<h2>' . check_plain(implode(" ", preg_split('/(?<=\\w)(?=[A-Z])/', $type))) . '</h2>'
                ),
                'table' => array(
                    '#theme' => 'table',
                    '#rows' => gbif_registry_get_as_formatted_array($class),
                    'contacts' => array()
                )
            );
            foreach ($class->getContacts() as $contact) {
                $content['contacts'][] = array(
                    '#theme' => 'table',
                    '#prefix' => '<div style="margin:3px;">',
                    '#suffix' => '</div>',
                    '#header' => array(
                        array(
                            'data' => 'Contact',
                            'colspan' => 2
                        )
                    ),
                    '#rows' => gbif_registry_get_as_formatted_array($contact)
                );
            }
            if (isset($content['contacts'])) {
                $content['contacts']['#type'] = 'fieldset';
                $content['contacts']['#title'] = t('Contacts');
            }
            if (method_exists($class, 'getEndpoints')) {
                foreach ($class->getEndpoints() as $endpoint) {
                    $content['endpoints'][] = array(
                        '#theme' => 'table',
                        '#prefix' => '<div style="margin:3px;">',
                        '#suffix' => '</div>',
                        '#header' => array(
                            array(
                                'data' => 'Endpoint',
                                'colspan' => 2
                            )
                        ),
                        '#rows' => gbif_registry_get_as_formatted_array($endpoint)
                    );
                }
                if (isset($content['endpoints'])) {
                    $content['endpoints']['#type'] = 'fieldset';
                    $content['endpoints']['#title'] = t('Endpoints');
                }
            }
            if (method_exists($class, 'getInstallations')) {
                foreach ($class->getInstallations() as $ti) {
                    $content['installations'][] = array(
                        '#theme' => 'table',
                        '#prefix' => '<div style="margin:3px;">',
                        '#suffix' => '</div>',
                        '#header' => array(
                            array(
                                'data' => 'Installation',
                                'colspan' => 2
                            )
                        ),
                        '#rows' => gbif_registry_get_as_formatted_array($ti)
                    );
                }
                if (isset($content['installations'])) {
                    $content['installations']['#type'] = 'fieldset';
                    $content['installations']['#title'] = t('Installations');
                }
            }
            return $content;
        } else {
            return t('There has been an error, @class class not found.', array(
                '@class' => $type
            ));
        }
    }
}

/**
 * Get a GBIF Object as an array that can be put in theme_html().
 */
function gbif_registry_get_as_formatted_array($object)
{
    $rows = array();
    foreach ($object->getAsArray() as $key => $value) {
        switch ($key) {
            default:
                if (is_string($value) && trim($value)) {
                    $key = implode(" ", preg_split('/(?<=\\w)(?=[A-Z])/', ucfirst($key)));
                    $rows[] = array(
                        array(
                            'data' => check_plain($key),
                            'style' => 'width:40%;'
                        ),
                        array(
                            'data' => filter_xss_admin($value),
                            'style' => 'width:60%;'
                        )
                    );
                }
        }
    }
    return $rows;
}

/**
 * Dynamically create an instance of a GBIF class.
 */
function gbif_registry_create_instance($class, $params)
{
    $reflection_class = new ReflectionClass($class);
    return $reflection_class->newInstanceArgs($params);
}

/**
 * Display the browse page.
 *
 * FIXME - Need to add the "Next" and "Previous" links.
 */
function gbif_registry_browse_page($class = 'Nodes')
{
    $registry = GBIFRegistry::singleton();
    if (method_exists($registry, "get$class")) {
        $objects = call_user_func(array(
            $registry,
            "get$class"
        ));
        $rows = array();
        if (isset($_GET['offset'])) {
            if (is_array($objects)) {
        // We have all the items, simply knock the first x off the start.
                for ($i = 0; $i < $_GET['offset']; $i++) {
                    array_shift($objects);
                }
            } else {
                $objects->setOffset($_GET['offset']);
            }
        }
        $i = 0;
        foreach ($objects as $object) {
            $rows[] = array(
                'title' => check_plain($object->getTitle()),
                'description' => l(check_plain($object->getHomepage()), $object->getHomepage()),
                'key' => l(check_plain($object->getKey()), "admin/config/services/gbif_registry/view/" . substr($class, 0, -1) . "/{$object->getKey()}")
            );
            $i++;
            if ($i == GBIF_REGISTRY_OBJECTS_PER_PAGE) {
                break;
            }
        }
        $content = array(
            'objects' => array(
                '#type' => 'fieldset',
                '#title' => check_plain(implode(" ", preg_split('/(?<=\\w)(?=[A-Z])/', $class))),
                '#collapsed' => true,
                '#collapsible' => true,
                'table' => array(
                    '#theme' => 'table',
                    '#header' => array(
                        t('Title'),
                        t('Homepage'),
                        t('Key')
                    ),
                    '#rows' => $rows
                )
            )
        );
        if (isset($_GET['offset']) && $_GET['offset'] >= GBIF_REGISTRY_OBJECTS_PER_PAGE) {
            $content['objects']['previous_lot'] = array(
                '#theme' => 'link',
                '#text' => t('Previous'),
                '#path' => $_GET['q'],
                '#options' => array(
                    'attributes' => array(),
                    'html' => false,
                    'query' => array(
                        'offset' => $_GET['offset'] - GBIF_REGISTRY_OBJECTS_PER_PAGE
                    )
                )
            );
        }
        if (count($rows) == GBIF_REGISTRY_OBJECTS_PER_PAGE) {
            $content['objects']['next_lot'] = array(
                '#prefix' => isset($content['objects']['previous_lot']) ? ' | ' : '',
                '#theme' => 'link',
                '#text' => t('Next'),
                '#path' => $_GET['q'],
                '#options' => array(
                    'attributes' => array(),
                    'html' => false,
                    'query' => array(
                        'offset' => isset($_GET['offset']) ? $_GET['offset'] + GBIF_REGISTRY_OBJECTS_PER_PAGE : GBIF_REGISTRY_OBJECTS_PER_PAGE
                    )
                )
            );
        }
        return $content;
    }
}