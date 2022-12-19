<?php

namespace IucnApi\Tests\Unit;

use IucnApi\Model\GrowthForm;
use PHPUnit\Framework\TestCase;

final class GrowthFormTest extends TestCase
{
    public function testCreateFromArray(): void
    {
        $growthForm = GrowthForm::createFromArray([
            'name' => 'Tree - large',
        ]);

        self::assertEquals('Tree - large', $growthForm->getName());
    }
}
