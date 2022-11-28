<?php

namespace IucnApi\Tests\Unit;

use IucnApi\Model\Assessment;
use PHPUnit\Framework\TestCase;

final class AssessmentTest extends TestCase
{
    public function testCreateFromArray(): void
    {
        $assessment = Assessment::createFromArray([
            'assess_year' => 2021,
            'code' => 'VU',
            'category' => 'Vulnerable',
            'year' => 2022,
        ]);

        self::assertEquals(2021, $assessment->getAssessmentYear());
        self::assertEquals('VU', $assessment->getCategoryCode());
        self::assertEquals('Vulnerable', $assessment->getCategoryName());
        self::assertEquals(2022, $assessment->getPublicationYear());
    }
}
