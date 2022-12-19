<?php

namespace IucnApi\Model;

class Species
{
    protected ?string $category = null;
    protected ?string $scientificName = null;
    protected ?string $subpopulation = null;
    protected ?string $subspeciesName = null;
    protected ?string $subspeciesRank = null;
    protected ?int $taxonId = null;

    public function getCategory(): ?string
    {
        return $this->category;
    }

    public function getScientificName(): ?string
    {
        return $this->scientificName;
    }

    public function getSubpopulation(): ?string
    {
        return $this->subpopulation;
    }

    public function getSubspeciesName(): ?string
    {
        return $this->subspeciesName;
    }

    public function getSubspeciesRank(): ?string
    {
        return $this->subspeciesRank;
    }

    public function getTaxonId(): ?int
    {
        return $this->taxonId;
    }

    /**
     * @param array<string, mixed> $data
     */
    public static function createFromArray(array $data): Species
    {
        $species = new Species();
        $species->category = !is_null($data['category']) ? strval($data['category']) : null;
        $species->scientificName = !is_null($data['scientific_name']) ? strval($data['scientific_name']) : null;
        $species->subpopulation = !is_null($data['subpopulation']) ? strval($data['subpopulation']) : null;
        $species->subspeciesName = !is_null($data['subspecies']) ? strval($data['subspecies']) : null;
        $species->subspeciesRank = !is_null($data['rank']) ? strval($data['rank']) : null;
        $species->taxonId = !is_null($data['taxonid']) ? intval($data['taxonid']) : null;

        return $species;
    }
}
