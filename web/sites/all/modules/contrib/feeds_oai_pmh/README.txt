
Feeds OAI-PMH Fetcher and Parser README

SUMMARY
==================================
This is a Drupal module that fetches and parses OAI_DC (Dublin core) metadata 
records from OAI-PMH services. It depends on the Feeds module.

REQUIREMENTS
==================================
You need the Feeds module and its dependencies (Ctools, etc.) 

Recommended additional modules: CCK, Link (to hold the resource URLs)

CONFIGURATION AND USAGE
==================================
* Enable all modules: Feeds (and its dependencies) and the "Feeds OAI-PMH 
    Fetcher and Parser" module.

* Create a new node type for the importer. For instance "OAI repository". Each 
    node of this type will hold each Feed importer configured later on. You could
    have a different set and/or repository per node, for instance.
    
* Create a new node type for the imported records. Here you should add a field
    for each of the Dublin Core fields that will be imported. For instance:
    description, publisher, type, format, subject, date, etc. You could also
    add taxonomy vocabularies for this node type.
    
* Create a new Feeds importer at admin/build/feeds/create

* Configure your importer as you would any other, except for these settings:
    On Basic Settings, in "Attach to content type" choose the importer node type 
      created earlier. I recommend you uncheck "Import on submission".
    For Fetcher, choose "HTTP OAI-PMH Fetcher". 
    For Parser, choose "OAI parser". 
    For Processor, choose "Node processor".
    Under the node processor settings, on "Content type" select the node type
      created earlier for the records.    
    
* Add a new node of the type configued.
   On the "Feed" fieldgroup, on the "URL" field, you should enter the URL for the
     OAI-PMH endpoint. For instance:
       http://www.dlese.org/oai/provider

   The "Set to fetch" options box will be populated if you have Javascript
     active. If not, just save and re-edit the node to see the available sets.

   When you're set, save the node and then click on the "Import" tab above the 
     node. This will create a node for each record from the repository's selected 
     set. Note that large repositories will take a long time.

MORE INFORMATION
==================================
For a listing of available repositories, here's a list of some registries:
   
   The Directory of Open Access Repositories - OpenDOAR
   http://www.opendoar.org/
   
   The University of Illinois OAI-PMH Data Provider Registry
   http://gita.grainger.uiuc.edu/registry/
   
   Open Archives Initiative - Repository Explorer
   http://re.cs.uct.ac.za/
   
   Registry of Open Access Repositories (ROAR)
   http://roar.eprints.org/
   
CONTACT
==================================
Module mantainer:
  Alejandro Garza (janusman)
  Drupal.org profile: http://drupal.org/user/153120
   
This project has been sponsored by:
  Center for Innovation in Technology and Education, Tecnológico de Monterrey
  http://www.itesm.mx/innovate
  

