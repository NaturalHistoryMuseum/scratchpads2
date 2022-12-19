<?php

namespace IucnApi\Tests\Unit;

use IucnApi\Model\Occurrence;
use PHPUnit\Framework\TestCase;

final class OccurrenceTest extends TestCase
{
    public function testCreateFromArray(): void
    {
        $occurrence = Occurrence::createFromArray([
            'code' => 'BI',
            'country' => 'Burundi',
            'distribution_code' => 'Regionally Extinct',
            'origin' => 'Native',
            'presence' => 'Regionally Extinct',
        ]);

        self::assertEquals('BI', $occurrence->getCountryCode());
        self::assertEquals('Burundi', $occurrence->getCountryName());
        self::assertEquals('Regionally Extinct', $occurrence->getDistributionCode());
        self::assertEquals('Native', $occurrence->getOrigin());
        self::assertEquals('Regionally Extinct', $occurrence->getPresence());
    }
}
