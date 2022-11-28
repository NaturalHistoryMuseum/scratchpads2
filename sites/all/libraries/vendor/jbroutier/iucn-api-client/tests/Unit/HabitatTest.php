<?php

namespace IucnApi\Tests\Unit;

use IucnApi\Model\Habitat;
use PHPUnit\Framework\TestCase;

final class HabitatTest extends TestCase
{
    public function testCreateFromArray(): void
    {
        $habitat = Habitat::createFromArray([
            'code' => '12.1',
            'habitat' => 'Marine Intertidal - Rocky Shoreline',
            'majorimportance' => 'Yes',
            'season' => 'Summer',
            'suitability' => 'Suitable',
        ]);

        self::assertEquals('12.1', $habitat->getCode());
        self::assertEquals('Yes', $habitat->getMajorImportance());
        self::assertEquals('Marine Intertidal - Rocky Shoreline', $habitat->getName());
        self::assertEquals('Summer', $habitat->getSeason());
        self::assertEquals('Suitable', $habitat->getSuitability());
    }
}
