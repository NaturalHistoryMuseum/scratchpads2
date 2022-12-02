<?php

/**
 * @noinspection SpellCheckingInspection
 */

namespace IucnApi\Tests\Unit;

use IucnApi\Model\SpeciesDetails;
use PHPUnit\Framework\TestCase;

final class SpeciesDetailsTest extends TestCase
{
    public function testCreateFromArray(): void
    {
        $speciesDetails = SpeciesDetails::createFromArray([
            'amended_flag' => true,
            'amended_reason' => '',
            'aoo_km2' => 24000,
            'assessment_date' => '2020-11-13',
            'assessor' => 'John Doe',
            'authority' => '(Blumenbach, 1797)',
            'category' => 'EN',
            'class' => 'MAMMALIA',
            'criteria' => 'A2abd',
            'depth_lower' => 5000,
            'depth_upper' => 0,
            'elevation_lower' => 100,
            'elevation_upper' => 3200,
            'eoo_km2' => 48250,
            'errata_flag' => true,
            'errata_reason' => 'Vestibulum eget augue tempor',
            'family' => 'ELEPHANTIDAE',
            'freshwater_system' => true,
            'genus' => 'Loxodonta',
            'kingdom' => 'ANIMALIA',
            'main_common_name' => 'African Savanna Elephant',
            'marine_system' => true,
            'order' => 'PROBOSCIDEA',
            'phylum' => 'CHORDATA',
            'population_trend' => 'Decreasing',
            'published_year' => 2022,
            'reviewer' => 'Jane Doe',
            'scientific_name' => 'Loxodonta africana',
            'taxonid' => 181008073,
            'terrestrial_system' => true,
        ]);

        self::assertTrue($speciesDetails->isAmended());
        self::assertEquals('', $speciesDetails->getAmendedReason());
        self::assertEquals(24000, $speciesDetails->getAOO());
        self::assertEquals('2020-11-13', $speciesDetails->getAssessmentDate());
        self::assertEquals('John Doe', $speciesDetails->getAssessor());
        self::assertEquals('(Blumenbach, 1797)', $speciesDetails->getAuthority());
        self::assertEquals('EN', $speciesDetails->getCategory());
        self::assertEquals('MAMMALIA', $speciesDetails->getClass());
        self::assertEquals('A2abd', $speciesDetails->getCriteria());
        self::assertEquals(5000, $speciesDetails->getDepthLower());
        self::assertEquals(0, $speciesDetails->getDepthUpper());
        self::assertEquals(100, $speciesDetails->getElevationLower());
        self::assertEquals(3200, $speciesDetails->getElevationUpper());
        self::assertEquals(48250, $speciesDetails->getEOO());
        self::assertTrue($speciesDetails->isErrata());
        self::assertEquals('Vestibulum eget augue tempor', $speciesDetails->getErrataReason());
        self::assertEquals('ELEPHANTIDAE', $speciesDetails->getFamily());
        self::assertTrue($speciesDetails->isFreshwaterSystem());
        self::assertEquals('Loxodonta', $speciesDetails->getGenus());
        self::assertEquals('ANIMALIA', $speciesDetails->getKingdom());
        self::assertEquals('African Savanna Elephant', $speciesDetails->getMainCommonName());
        self::assertTrue($speciesDetails->isMarineSystem());
        self::assertEquals('PROBOSCIDEA', $speciesDetails->getOrder());
        self::assertEquals('CHORDATA', $speciesDetails->getPhylum());
        self::assertEquals('Decreasing', $speciesDetails->getPopulationTrend());
        self::assertEquals(2022, $speciesDetails->getPublicationYear());
        self::assertEquals('Jane Doe', $speciesDetails->getReviewer());
        self::assertEquals('Loxodonta africana', $speciesDetails->getScientificName());
        self::assertTrue($speciesDetails->isTerrestrialSystem());
        self::assertEquals(181008073, $speciesDetails->getTaxonId());
    }
}
