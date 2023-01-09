<?php

namespace IucnApi\Tests\Unit;

use IucnApi\Model\Group;
use PHPUnit\Framework\TestCase;

final class GroupTest extends TestCase
{
    public function testCreateFromArray(): void
    {
        $group = Group::createFromArray([
            'group_name' => 'mammals',
        ]);

        self::assertEquals('mammals', $group->getCode());
    }
}
