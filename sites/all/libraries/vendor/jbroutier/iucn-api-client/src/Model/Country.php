<?php

namespace IucnApi\Model;

class Country
{
    protected ?string $code = null;
    protected ?string $name = null;

    public function getCode(): ?string
    {
        return $this->code;
    }

    public function getName(): ?string
    {
        return $this->name;
    }

    /**
     * @param array<string, mixed> $data
     */
    public static function createFromArray(array $data): Country
    {
        $country = new Country();
        $country->code = !is_null($data['isocode']) ? strval($data['isocode']) : null;
        $country->name = !is_null($data['country']) ? strval($data['country']) : null;

        return $country;
    }
}
