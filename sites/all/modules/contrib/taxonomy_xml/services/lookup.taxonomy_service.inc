<?php
/* 
 */


/**
 * @file declare how to import a taxonomy from file upload.
 */

/**
 * hook_taxonomy_service_info()
 */ 
function lookup_taxonomy_service_info() {
  $services = array();
  $services['lookup'] = array(
    'provider' => 'Remote Lookup',
    'name' => 'Simple remote lookup from known online services',
    'id' => 'lookup',
    'description' => '
      Choose from a set of simple known online resources.
    ',
    // Define the name of the form function that returns service-specific UI
    'import_form_callback' => 'lookup_taxonomy_service_form',
  );
  return $services;
}

/**
 * A sub-form that provides UI additions to the taxonomy import form
 */
function lookup_taxonomy_service_form() {
  $available_services = lookup_taxonomy_service_services();
  $options = array();
  foreach ($available_services as $id => $service) {
    $options[$id] = $service['provider'] . " - " . $service['name'];
  }
  $formats = taxonomy_xml_formats();

  $form = array(
    '#type' => 'fieldset',
    '#attributes' => array('id' => 'edit-service-wrapper'),
    'service_id' => array(
      '#type' => 'select',
      '#title' => t('Taxonomy Server'),
      '#description' => t('
        Choose one of the available taxonomy server services.
        These preset services are defined in the taxonomy_xml module
        and may be extended by other contributed modules.
      '),
      '#default_value' => variable_get('taxonomy_xml_service_id', ''),
      '#options' => $options,
    ),
    'identifier' => array(
      '#type' => 'textfield',
      '#title' => t('Unique Identifier for this service'),
      '#description' => t('Optional ID, GIUD, LSID, URI or other UID as (if) required by this service.'),
      '#default_value' => variable_get('taxonomy_xml_identifier', ''),
    ),
    'information' => array(
      '#type' => 'markup',
      '#value' => l('More info about remote taxonomy services', TAXONOMY_XML_ADMIN . '/import/services'),
    )

  );

  $form['format'] = array(
    '#type' => 'select',
    '#title' => t('Format of file'),
    '#default_value' => variable_get('taxonomy_xml_format', 'xml_format'),
    '#options' => $formats,
  );

  return $form;
}



/**
 * @file
 *   A list of known lookup services and a brief description of their syntax
 *
 * Remote lookup servers may be web services invoked directly through URL
 * arguments or more advanced XML/RPC type interfaces.
 *
 * Currently only simple URL patterns are supported
 */

/**
 * Return a list of known lookup and search services, and some instructions on
 * how they may be invoked.
 *
 * Implementation pf HOOK_taxonomy_servers
 *
 * @ingroup taxonomy_xml_services
 */
function lookup_taxonomy_service_services() {
  $services = array(

    'freebase' => array(
      'provider' => 'Freebase by Metaweb Technologies, ',
      'name' => 'Freebase lookup',
      'servicetype' => 'lookup',
      'description' => 'Freebase lookup service can return RDF data about all sorts of topics. It requires an id, often prefixed with a concept type, eg <code>event.disaster</code>',
      'about' => 'http://www.freebase.com/',
      'identifier' => 'identifier',
      'protocol' => 'URI',
      'pattern' => 'http://rdf.freebase.com/ns/!identifier',
      'format' => 'rdf',
    ),

    'twdg' => array(
      'provider' => 'Biodiversity Information Standards (TDWG)',
      'name' => 'LSID Web Resolver',
      'servicetype' => 'lookup',
      'description' => 'This currently requires you to know an LSID in advance - there are no machine-readable LSID search services that I can find yet',
      'identifier' => 'identifier',
      'protocol' => 'URI',
      'pattern' => 'http://lsid.tdwg.org/!identifier',
      'format' => 'rdf',
    ),
    'gcl' => array(
      'provider' => 'UK GovTalk ',
      'name' => 'GCL (Government Category List)',
      'servicetype' => 'lookup',
      'description' => 'an encoding scheme for the Subject element of the current version of the e-Government Metadata Standard (e-GMS).',
      'about' => 'http://isegserv.itd.rl.ac.uk/skos/gcl/',
      'identifier' => 'identifier',
      'protocol' => 'URI',
      'pattern' => 'http://isegserv.itd.rl.ac.uk/skos/gcl/gcl2.1.rdf',
      'format' => 'rdf',
    ),
    'WCA-terms' => array(
      'provider' => 'W3C ',
      'name' => 'Web Characterization Terminology & Definitions Sheet',
      'servicetype' => 'lookup',
      'description' => 'Glossary of web technology terms used by the W3C. (WCA-terms)',
      'about' => 'http://www.w3.org/QA/2003/01/Glossary',
      #'identifier' => 'identifier',
      'protocol' => 'URI',
      'pattern' => 'http://www.w3.org/2003/03/glossary-project/data/glossaries/WCA-terms.rdf',
      'format' => 'rdf',
    ),

    'LOC-lookup' => array(
      'provider' => 'Library of Congress',
      'name' => 'Library of Congress Subject Headings',
      'servicetype' => 'lookup',
      'description' => "
        LCSH in this service includes all Library of Congress Subject Headings, free-floating subdivisions (topical and form), Genre/Form headings, Children's (AC) headings, and validation strings* for which authority records have been created. The content includes a few name headings (personal and corporate), such as William Shakespeare, Jesus Christ, and Harvard University; and geographic headings that are added to LCSH as they are needed to establish subdivisions, provide a pattern for subdivision practice, or provide reference structure for other terms. This content is expanded beyond 'the big red books' (the paper issue of LCSH) with inclusion of validation strings.
      ",
      'about' => 'http://id.loc.gov/authorities/about.html',
      'identifier' => 'identifier',
      'protocol' => 'URI',
      'pattern' => 'http://id.loc.gov/authorities/!identifier',
      'format' => 'rdf',
    ),

    'LOC-music' => array(
      'provider' => 'Library of Congress',
      'name' => 'Library of Congress Subject Headings - Popular music - SAMPLE',
      'servicetype' => 'lookup',
      'description' => "
        Example LoC subject headings under the topic 'Popular Music'.
      ",
      'about' => 'http://id.loc.gov/authorities/about.html',
      'protocol' => 'URI',
      'pattern' => 'http://id.loc.gov/authorities/sh85088865',
      'format' => 'rdf',
    ),

    'sonz' => array(
      'provider' => 'New Zealand Government Locator Service (NZGLS)',
      'name' => 'Services Of New Zealand (SONZ)',
      'description' => 'A list of topics and classifications used to describe subjects of interest to government and governemnt agencies. Subjects include terms relating to administration, national affairs, education, infrastructure, law, economics and industry.',
      'about' => 'http://www.e.govt.nz/standards/nzgls/thesauri',
      'servicetype' => 'lookup',
      'protocol' => 'URI',
      'pattern' => 'http://www.e.govt.nz/standards/nzgls/thesauri/sonz150.txt/download',
      'format' => 'csv',
    ),
    'fonz' => array(
      'provider' => 'New Zealand Government Locator Service (NZGLS)',
      'name' => 'Functions Of New Zealand (FONZ)',
      'description' => 'A list of topics and classifications used to describe government departments and administrative tasks. Subjects include terms relating to taxation, healthcare, immigration, defense and law. This version contains 477 terms and is provided as an all-in-one CSV (triple) file.',
      'about' => 'http://www.e.govt.nz/standards/nzgls/thesauri',
      'servicetype' => 'lookup',
      'protocol' => 'URI',
      'pattern' => 'http://www.e.govt.nz/standards/nzgls/thesauri/fonz2.30.txt/download',
      'format' => 'csv',
    ),

    'AVM' => array(
      'provider' => 'Virtual Astronomy Multimedia Project (VAMP) ',
      'name' => 'Astronomy Visualization Metadata Standard (AVM) RDF+SKOS',
      'description' => 'A structured glossary for use for tagging celestial bodies and related items. Terms like  Pulsar planet, Satellite, Dwarf Star, Dark Matter.',
      'about' => 'http://www.virtualastronomy.org/avm_metadata.php',
      'servicetype' => 'lookup',
      'protocol' => 'URI',
      'pattern' => 'http://www.ivoa.net/rdf/Vocabularies/vocabularies-20091007/AVM/AVM.rdf',
      'format' => 'rdf',
    ),
    
    'sioc-types' => array(
      'provider' => 'SIOC Core Ontology Specification',
      'name' => 'SIOC Types Module',
      'description' => 'Types of posts used within SIOC, Standard terms for functionally classifying services and resources.
Container: AddressBook, AnnotationSet, AudioChannel, BookmarkFolder, Briefcase, EventCalendar, ImageGallery, ProjectDirectory, ResumeBank, ReviewArea, SubscriptionList, SurveyCollection, VideoChannel, Wiki.
Item: Poll.
Forum: ArgumentativeDiscussion, ChatChannel, MailingList, MessageBoard, Weblog.
Post: BlogPost, BoardPost, Comment, InstantMessage, MailMessage, WikiArticle',
      'servicetype' => 'lookup',
      'protocol' => 'URI',
      'about' => 'http://sioc-project.org/ontology',
      'pattern' => 'http://rdfs.org/sioc/types',
      'format' => 'rdf',
    ),
  );
  return $services;
}