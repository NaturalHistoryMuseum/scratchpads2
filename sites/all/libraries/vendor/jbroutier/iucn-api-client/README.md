[![PHP](https://github.com/jbroutier/iucn-api-client/actions/workflows/php.yml/badge.svg)](https://github.com/jbroutier/iucn-api-client/actions/workflows/php.yml)
[![codecov](https://codecov.io/gh/jbroutier/iucn-api-client/branch/develop/graph/badge.svg?token=20HBOP8NB3)](https://codecov.io/gh/jbroutier/iucn-api-client)
[![PHP version](https://img.shields.io/badge/php-7.4+-787cb5?logo=php)](https://github.com/jbroutier/iucn-api-client)
[![License](https://img.shields.io/github/license/jbroutier/iucn-api-client)](https://github.com/jbroutier/iucn-api-client/blob/main/LICENSE)
[![Packagist](https://img.shields.io/packagist/v/jbroutier/iucn-api-client)](https://packagist.org/packages/jbroutier/iucn-api-client)

# IUCN API Client

A PHP client to retrieve data from [The IUCN Red List of Threatened Species™](https://www.iucnredlist.org/).
It currently supports the version 3 of the API as described in
the [API reference](https://apiv3.iucnredlist.org/api/v3/docs), with the IUCN database version 2022-1.

## Disclaimer

:warning: This project is not supported or endorsed in any manner by the IUCN. :warning:

## Installation

```bash
composer require jbroutier/iucn-api-client
```

## Requirements

An API key (token) is required to authenticate yourself and be able to use the IUCN API. To obtain an API key, please
use the [application form](https://apiv3.iucnredlist.org/api/v3/token) and submit your request to the IUCN.

## Getting started

Start by creating an instance of the client.

```php
$client = new IucnApi\Client('<your-api-key>');
```

You can pass an array of options to the underlying HTTP client, if you want to configure the timeout for example. See
the [Symfony HTTP client](https://symfony.com/doc/current/http_client.html) documentation for a list of available
options.

```php
$client = new IucnApi\Client('<your-api-key>', ['timeout' => 5000]);
```

You can then request the details of a species for example.

```php
try {
    $species = $client->getSpeciesByName('Ailurus fulgens');
} catch (\IucnApi\Exception\IucnApiException $exception) {
    // Something doesn't look good…
}

echo $species->getKigdom();         // ANIMALIA
echo $species->getPhylum();         // CHORDATA
echo $species->getClass();          // MAMMALIA
echo $species->getOrder();          // CARNIVORA
echo $species->getFamily();         // AILURIDAE
echo $species->getGenus();          // Ailurus
echo $species->getMainCommonName(); // Red Panda
```

The full list of available methods is described in
the [ClientInterface](https://github.com/jbroutier/iucn-api-client/blob/main/src/ClientInterface.php) interface.

## Documentation

The official API documentation is available [here](https://apiv3.iucnredlist.org/api/v3/docs). The IUCN Red List
database changelog can be found [here](https://apiv3.iucnredlist.org/changelog).
