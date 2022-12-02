<?php

namespace IucnApi\Model;

class SpeciesDetails
{
    protected $amended = null;
    protected $amendedReason = null;
    protected $aoo = null;
    protected $assessmentDate = null;
    protected $assessor = null;
    protected $authority = null;
    protected $category = null;
    protected $class = null;
    protected $criteria = null;
    protected $depthLower = null;
    protected $depthUpper = null;
    protected $elevationLower = null;
    protected $elevationUpper = null;
    protected $eoo = null;
    protected $errata = null;
    protected $errataReason = null;
    protected $family = null;
    protected $freshwaterSystem = null;
    protected $genus = null;
    protected $kingdom = null;
    protected $mainCommonName = null;
    protected $marineSystem = null;
    protected $order = null;
    protected $phylum = null;
    protected $populationTrend = null;
    protected $publicationYear = null;
    protected $reviewer = null;
    protected $scientificName = null;
    protected $taxonId = null;
    protected $terrestrialSystem = null;

    public function isAmended(): ?bool
    {
        return $this->amended;
    }

    public function getAmendedReason(): ?string
    {
        return $this->amendedReason;
    }

    public function getAOO(): ?int
    {
        return $this->aoo;
    }

    public function getAssessmentDate(): ?string
    {
        return $this->assessmentDate;
    }

    public function getAssessor(): ?string
    {
        return $this->assessor;
    }

    public function getAuthority(): ?string
    {
        return $this->authority;
    }

    public function getCategory(): ?string
    {
        return $this->category;
    }

    public function getClass(): ?string
    {
        return $this->class;
    }

    public function getCriteria(): ?string
    {
        return $this->criteria;
    }

    public function getDepthLower(): ?int
    {
        return $this->depthLower;
    }

    public function getDepthUpper(): ?int
    {
        return $this->depthUpper;
    }

    public function getElevationLower(): ?int
    {
        return $this->elevationLower;
    }

    public function getElevationUpper(): ?int
    {
        return $this->elevationUpper;
    }

    public function getEOO(): ?int
    {
        return $this->eoo;
    }

    public function isErrata(): ?bool
    {
        return $this->errata;
    }

    public function getErrataReason(): ?string
    {
        return $this->errataReason;
    }

    public function getFamily(): ?string
    {
        return $this->family;
    }

    public function isFreshwaterSystem(): ?bool
    {
        return $this->freshwaterSystem;
    }

    public function getGenus(): ?string
    {
        return $this->genus;
    }

    public function getKingdom(): ?string
    {
        return $this->kingdom;
    }

    public function getMainCommonName(): ?string
    {
        return $this->mainCommonName;
    }

    public function isMarineSystem(): ?bool
    {
        return $this->marineSystem;
    }

    public function getOrder(): ?string
    {
        return $this->order;
    }

    public function getPhylum(): ?string
    {
        return $this->phylum;
    }

    public function getPopulationTrend(): ?string
    {
        return $this->populationTrend;
    }

    public function getPublicationYear(): ?int
    {
        return $this->publicationYear;
    }

    public function getReviewer(): ?string
    {
        return $this->reviewer;
    }

    public function getScientificName(): ?string
    {
        return $this->scientificName;
    }

    public function getTaxonId(): ?int
    {
        return $this->taxonId;
    }

    public function isTerrestrialSystem(): ?bool
    {
        return $this->terrestrialSystem;
    }

    /**
     * @param array<string, mixed> $data
     */
    public static function createFromArray(array $data): SpeciesDetails
    {
        $speciesDetails = new SpeciesDetails();
        $speciesDetails->amended = !is_null($data['amended_flag']) ? boolval($data['amended_flag']) : null;
        $speciesDetails->amendedReason = !is_null($data['amended_reason']) ? strval($data['amended_reason']) : null;
        $speciesDetails->aoo = !is_null($data['aoo_km2']) ? intval($data['aoo_km2']) : null;
        $speciesDetails->assessmentDate = !is_null($data['assessment_date']) ? strval($data['assessment_date']) : null;
        $speciesDetails->assessor = !is_null($data['assessor']) ? strval($data['assessor']) : null;
        $speciesDetails->authority = !is_null($data['authority']) ? strval($data['authority']) : null;
        $speciesDetails->category = !is_null($data['category']) ? strval($data['category']) : null;
        $speciesDetails->class = !is_null($data['class']) ? strval($data['class']) : null;
        $speciesDetails->criteria = !is_null($data['criteria']) ? strval($data['criteria']) : null;
        $speciesDetails->depthLower = !is_null($data['depth_lower']) ? intval($data['depth_lower']) : null;
        $speciesDetails->depthUpper = !is_null($data['depth_upper']) ? intval($data['depth_upper']) : null;
        $speciesDetails->elevationLower = !is_null($data['elevation_lower']) ? intval($data['elevation_lower']) : null;
        $speciesDetails->elevationUpper = !is_null($data['elevation_upper']) ? intval($data['elevation_upper']) : null;
        $speciesDetails->eoo = !is_null($data['eoo_km2']) ? intval($data['eoo_km2']) : null;
        $speciesDetails->errata = !is_null($data['errata_flag']) ? boolval($data['errata_flag']) : null;
        $speciesDetails->errataReason = !is_null($data['errata_reason']) ? strval($data['errata_reason']) : null;
        $speciesDetails->family = !is_null($data['family']) ? strval($data['family']) : null;
        $speciesDetails->freshwaterSystem = !is_null($data['freshwater_system'])
            ? boolval($data['freshwater_system']) : null;
        $speciesDetails->genus = !is_null($data['genus']) ? strval($data['genus']) : null;
        $speciesDetails->kingdom = !is_null($data['kingdom']) ? strval($data['kingdom']) : null;
        $speciesDetails->mainCommonName = !is_null($data['main_common_name'])
            ? strval($data['main_common_name']) : null;
        $speciesDetails->marineSystem = !is_null($data['marine_system']) ? boolval($data['marine_system']) : null;
        $speciesDetails->order = !is_null($data['order']) ? strval($data['order']) : null;
        $speciesDetails->phylum = !is_null($data['phylum']) ? strval($data['phylum']) : null;
        $speciesDetails->populationTrend = !is_null($data['population_trend'])
            ? strval($data['population_trend']) : null;
        $speciesDetails->publicationYear = !is_null($data['published_year']) ? intval($data['published_year']) : null;
        $speciesDetails->reviewer = !is_null($data['reviewer']) ? strval($data['reviewer']) : null;
        $speciesDetails->scientificName = !is_null($data['scientific_name']) ? strval($data['scientific_name']) : null;
        $speciesDetails->taxonId = !is_null($data['taxonid']) ? intval($data['taxonid']) : null;
        $speciesDetails->terrestrialSystem = !is_null($data['terrestrial_system'])
            ? boolval($data['terrestrial_system']) : null;

        return $speciesDetails;
    }
}
