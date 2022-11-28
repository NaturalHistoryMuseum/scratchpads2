<?php

namespace IucnApi\Tests\Unit;

use IucnApi\Model\NarrativeText;
use PHPUnit\Framework\TestCase;

final class NarrativeTextTest extends TestCase
{
    public function testCreateFromArray(): void
    {
        $narrativeText = NarrativeText::createFromArray([
            'conservationmeasures' => 'Nullam ut odio nec',
            'geographicrange' => 'Pellentesque luctus ante',
            'habitat' => 'Mauris sagittis orci',
            'population' => 'In dictum enim eget',
            'populationtrend' => 'Vivamus a sapien a tortor',
            'rationale' => 'In dictum enim eget mauris',
            'taxonomicnotes' => 'Maecenas pretium augue',
            'threats' => 'Donec vitae elit laoreet',
            'usetrade' => 'Cras feugiat diam',
        ]);

        self::assertEquals('Nullam ut odio nec', $narrativeText->getConservationMeasures());
        self::assertEquals('Pellentesque luctus ante', $narrativeText->getGeographicRange());
        self::assertEquals('Mauris sagittis orci', $narrativeText->getHabitat());
        self::assertEquals('In dictum enim eget', $narrativeText->getPopulation());
        self::assertEquals('Vivamus a sapien a tortor', $narrativeText->getPopulationTrend());
        self::assertEquals('In dictum enim eget mauris', $narrativeText->getRationale());
        self::assertEquals('Maecenas pretium augue', $narrativeText->getTaxonomicNotes());
        self::assertEquals('Donec vitae elit laoreet', $narrativeText->getThreats());
        self::assertEquals('Cras feugiat diam', $narrativeText->getUseTrade());
    }
}
