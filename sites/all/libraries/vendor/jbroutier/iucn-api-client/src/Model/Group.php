<?php

namespace IucnApi\Model;

class Group
{
    protected ?string $code = null;

    public function getCode(): ?string
    {
        return $this->code;
    }

    /**
     * @param array<string, mixed> $data
     */
    public static function createFromArray(array $data): Group
    {
        $group = new Group();
        $group->code = !is_null($data['group_name']) ? strval($data['group_name']) : null;

        return $group;
    }
}
