<?php

namespace IucnApi\Tests\Unit;

use IucnApi\Model\Region;
use PHPUnit\Framework\TestCase;

final class RegionTest extends TestCase
{
    public function testCreateFromArray(): void
    {
        $region = Region::createFromArray([
            'identifier' => 'europe',
            'name' => 'Europe',
        ]);

        self::assertEquals('europe', $region->getIdentifier());
        self::assertEquals('Europe', $region->getName());
    }
}
