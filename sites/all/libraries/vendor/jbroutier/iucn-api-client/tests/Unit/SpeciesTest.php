<?php

namespace IucnApi\Tests\Unit;

use IucnApi\Model\Species;
use PHPUnit\Framework\TestCase;

final class SpeciesTest extends TestCase
{
    public function testCreateFromArray(): void
    {
        $species = Species::createFromArray([
            'category' => 'LR/lc',
            'scientific_name' => 'Agarista mexicana var. pinetorum',
            'subpopulation' => 'nothingii',
            'subspecies' => 'pinetorum',
            'rank' => 'var.',
            'taxonid' => 37432,
        ]);

        self::assertEquals('LR/lc', $species->getCategory());
        self::assertEquals('Agarista mexicana var. pinetorum', $species->getScientificName());
        self::assertEquals('nothingii', $species->getSubpopulation());
        self::assertEquals('pinetorum', $species->getSubspeciesName());
        self::assertEquals('var.', $species->getSubspeciesRank());
        self::assertEquals(37432, $species->getTaxonId());
    }
}
