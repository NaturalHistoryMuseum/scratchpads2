<?php

namespace IucnApi\Tests\Unit;

use IucnApi\Model\ConservationMeasure;
use PHPUnit\Framework\TestCase;

final class ConservationMeasureTest extends TestCase
{
    public function testCreateFromArray(): void
    {
        $conservationMeasure = ConservationMeasure::createFromArray([
            'code' => '4.2',
            'title' => 'Lorem ipsum dolor sit amet',
        ]);

        self::assertEquals('4.2', $conservationMeasure->getCode());
        self::assertEquals('Lorem ipsum dolor sit amet', $conservationMeasure->getTitle());
    }
}
