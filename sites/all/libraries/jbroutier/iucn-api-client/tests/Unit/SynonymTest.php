<?php

namespace IucnApi\Tests\Unit;

use IucnApi\Model\Synonym;
use PHPUnit\Framework\TestCase;

final class SynonymTest extends TestCase
{
    public function testCreateFromArray(): void
    {
        $synonym = Synonym::createFromArray([
            'accepted_id' => 181008073,
            'accepted_name' => 'Loxodonta africana',
            'authority' => '(Blumenbach, 1797)',
            'synonym' => 'Elephas africana',
            'syn_authority' => 'Blumenbach, 1797',
        ]);

        self::assertEquals(181008073, $synonym->getAcceptedId());
        self::assertEquals('Loxodonta africana', $synonym->getAcceptedName());
        self::assertEquals('(Blumenbach, 1797)', $synonym->getAcceptedNameAuthority());
        self::assertEquals('Elephas africana', $synonym->getSynonym());
        self::assertEquals('Blumenbach, 1797', $synonym->getSynonymAuthority());
    }
}
