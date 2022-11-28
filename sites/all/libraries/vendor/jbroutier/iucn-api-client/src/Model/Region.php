<?php

namespace IucnApi\Model;

class Region
{
    protected ?string $identifier = null;
    protected ?string $name = null;

    public function getIdentifier(): ?string
    {
        return $this->identifier;
    }

    public function getName(): ?string
    {
        return $this->name;
    }

    /**
     * @param array<string, mixed> $data
     */
    public static function createFromArray(array $data): Region
    {
        $region = new Region();
        $region->identifier = !is_null($data['identifier']) ? strval($data['identifier']) : null;
        $region->name = !is_null($data['name']) ? strval($data['name']) : null;

        return $region;
    }
}
