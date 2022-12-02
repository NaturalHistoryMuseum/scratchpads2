<?php

namespace IucnApi\Model;

class Habitat
{
    protected ?string $code = null;
    protected ?string $majorImportance = null;
    protected ?string $name = null;
    protected ?string $season = null;
    protected ?string $suitability = null;

    public function getCode(): ?string
    {
        return $this->code;
    }

    public function getMajorImportance(): ?string
    {
        return $this->majorImportance;
    }

    public function getName(): ?string
    {
        return $this->name;
    }

    public function getSeason(): ?string
    {
        return $this->season;
    }

    public function getSuitability(): ?string
    {
        return $this->suitability;
    }

    /**
     * @param array<string, mixed> $data
     */
    public static function createFromArray(array $data): Habitat
    {
        $habitat = new Habitat();
        $habitat->code = !is_null($data['code']) ? strval($data['code']) : null;
        $habitat->majorImportance = !is_null($data['majorimportance']) ? strval($data['majorimportance']) : null;
        $habitat->name = !is_null($data['habitat']) ? strval($data['habitat']) : null;
        $habitat->season = !is_null($data['season']) ? strval($data['season']) : null;
        $habitat->suitability = !is_null($data['suitability']) ? strval($data['suitability']) : null;

        return $habitat;
    }
}
