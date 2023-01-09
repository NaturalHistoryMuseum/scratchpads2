<?php

namespace IucnApi\Model;

class NarrativeText
{
    protected ?string $conservationMeasures = null;
    protected ?string $geographicRange = null;
    protected ?string $habitat = null;
    protected ?string $population = null;
    protected ?string $populationTrend = null;
    protected ?string $rationale = null;
    protected ?string $taxonomicNotes = null;
    protected ?string $threats = null;
    protected ?string $useTrade = null;

    public function getConservationMeasures(): ?string
    {
        return $this->conservationMeasures;
    }

    public function getGeographicRange(): ?string
    {
        return $this->geographicRange;
    }

    public function getHabitat(): ?string
    {
        return $this->habitat;
    }

    public function getPopulation(): ?string
    {
        return $this->population;
    }

    public function getPopulationTrend(): ?string
    {
        return $this->populationTrend;
    }

    public function getRationale(): ?string
    {
        return $this->rationale;
    }

    public function getTaxonomicNotes(): ?string
    {
        return $this->taxonomicNotes;
    }

    public function getThreats(): ?string
    {
        return $this->threats;
    }

    public function getUseTrade(): ?string
    {
        return $this->useTrade;
    }

    /**
     * @param array<string, mixed> $data
     */
    public static function createFromArray(array $data): NarrativeText
    {
        $narrativeText = new NarrativeText();
        $narrativeText->conservationMeasures = !is_null($data['conservationmeasures'])
            ? strval($data['conservationmeasures']) : null;
        $narrativeText->geographicRange = !is_null($data['geographicrange']) ? strval($data['geographicrange']) : null;
        $narrativeText->habitat = !is_null($data['habitat']) ? strval($data['habitat']) : null;
        $narrativeText->population = !is_null($data['population']) ? strval($data['population']) : null;
        $narrativeText->populationTrend = !is_null($data['populationtrend']) ? strval($data['populationtrend']) : null;
        $narrativeText->rationale = !is_null($data['rationale']) ? strval($data['rationale']) : null;
        $narrativeText->taxonomicNotes = !is_null($data['taxonomicnotes']) ? strval($data['taxonomicnotes']) : null;
        $narrativeText->threats = !is_null($data['threats']) ? strval($data['threats']) : null;
        $narrativeText->useTrade = !is_null($data['usetrade']) ? strval($data['usetrade']) : null;

        return $narrativeText;
    }
}
