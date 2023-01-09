<?php

namespace IucnApi\Tests\Unit;

use IucnApi\Model\Threat;
use PHPUnit\Framework\TestCase;

final class ThreatTest extends TestCase
{
    public function testCreateFromArray(): void
    {
        $threat = Threat::createFromArray([
            'code' => '5.1',
            'invasive' => 'Homo sapiens',
            'scope' => 'Majority (50-90%)',
            'score' => 'Low Impact: 5',
            'severity' => 'Negligible declines',
            'timing' => 'Ongoing',
            'title' => 'Hunting & trapping terrestrial animals',
        ]);

        self::assertEquals('5.1', $threat->getCode());
        self::assertEquals('Homo sapiens', $threat->getInvasive());
        self::assertEquals('Majority (50-90%)', $threat->getScope());
        self::assertEquals('Low Impact: 5', $threat->getScore());
        self::assertEquals('Negligible declines', $threat->getSeverity());
        self::assertEquals('Ongoing', $threat->getTiming());
        self::assertEquals('Hunting & trapping terrestrial animals', $threat->getTitle());
    }
}
