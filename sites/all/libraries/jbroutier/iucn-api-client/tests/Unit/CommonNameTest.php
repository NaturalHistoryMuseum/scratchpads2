<?php

namespace IucnApi\Tests\Unit;

use IucnApi\Model\CommonName;
use PHPUnit\Framework\TestCase;

final class CommonNameTest extends TestCase
{
    public function testCreateFromArray(): void
    {
        $commonName = CommonName::createFromArray([
            'language' => 'eng',
            'taxonname' => 'Red panda',
            'primary' => true,
        ]);

        self::assertEquals('eng', $commonName->getLanguage());
        self::assertEquals('Red panda', $commonName->getName());
        self::assertTrue($commonName->isPrimary());
    }
}
