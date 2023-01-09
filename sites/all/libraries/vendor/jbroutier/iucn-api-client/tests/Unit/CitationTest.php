<?php

namespace IucnApi\Tests\Unit;

use IucnApi\Model\Citation;
use PHPUnit\Framework\TestCase;

final class CitationTest extends TestCase
{
    public function testCreateFromArray(): void
    {
        $citation = Citation::createFromArray([
            'citation' => 'Lorem ipsum dolor sit amet'
        ]);

        self::assertEquals('Lorem ipsum dolor sit amet', $citation->getCitation());
    }
}
