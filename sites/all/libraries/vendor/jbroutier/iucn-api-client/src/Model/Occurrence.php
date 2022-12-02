<?php

namespace IucnApi\Model;

class Occurrence
{
    protected ?string $countryCode = null;
    protected ?string $countryName = null;
    protected ?string $distributionCode = null;
    protected ?string $origin = null;
    protected ?string $presence = null;

    public function getCountryCode(): ?string
    {
        return $this->countryCode;
    }

    public function getCountryName(): ?string
    {
        return $this->countryName;
    }

    public function getDistributionCode(): ?string
    {
        return $this->distributionCode;
    }

    public function getOrigin(): ?string
    {
        return $this->origin;
    }

    public function getPresence(): ?string
    {
        return $this->presence;
    }

    /**
     * @param array<string, mixed> $data
     */
    public static function createFromArray(array $data): Occurrence
    {
        $occurrence = new Occurrence();
        $occurrence->countryCode = !is_null($data['code']) ? strval($data['code']) : null;
        $occurrence->countryName = !is_null($data['country']) ? strval($data['country']) : null;
        $occurrence->distributionCode = !is_null($data['distribution_code'])
            ? strval($data['distribution_code']) : null;
        $occurrence->origin = !is_null($data['origin']) ? strval($data['origin']) : null;
        $occurrence->presence = !is_null($data['presence']) ? strval($data['presence']) : null;

        return $occurrence;
    }
}
