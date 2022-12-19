<?php

namespace IucnApi\Tests\Unit;

use IucnApi\Model\Country;
use PHPUnit\Framework\TestCase;

final class CountryTest extends TestCase
{
    public function testCreateFromArray(): void
    {
        $country = Country::createFromArray([
            'isocode' => 'DE',
            'country' => 'Germany',
        ]);

        self::assertEquals('DE', $country->getCode());
        self::assertEquals('Germany', $country->getName());
    }
}
