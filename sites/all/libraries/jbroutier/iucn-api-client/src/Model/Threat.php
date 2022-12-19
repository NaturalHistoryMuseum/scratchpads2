<?php

namespace IucnApi\Model;

class Threat
{
    protected ?string $code = null;
    protected ?string $invasive = null;
    protected ?string $scope = null;
    protected ?string $score = null;
    protected ?string $severity = null;
    protected ?string $timing = null;
    protected ?string $title = null;

    public function getCode(): ?string
    {
        return $this->code;
    }

    public function getInvasive(): ?string
    {
        return $this->invasive;
    }

    public function getScope(): ?string
    {
        return $this->scope;
    }

    public function getScore(): ?string
    {
        return $this->score;
    }

    public function getSeverity(): ?string
    {
        return $this->severity;
    }

    public function getTiming(): ?string
    {
        return $this->timing;
    }

    public function getTitle(): ?string
    {
        return $this->title;
    }

    /**
     * @param array<string, mixed> $data
     */
    public static function createFromArray(array $data): Threat
    {
        $threat = new Threat();
        $threat->code = !is_null($data['code']) ? strval($data['code']) : null;
        $threat->invasive = !is_null($data['invasive']) ? strval($data['invasive']) : null;
        $threat->scope = !is_null($data['scope']) ? strval($data['scope']) : null;
        $threat->score = !is_null($data['score']) ? strval($data['score']) : null;
        $threat->severity = !is_null($data['severity']) ? strval($data['severity']) : null;
        $threat->timing = !is_null($data['timing']) ? strval($data['timing']) : null;
        $threat->title = !is_null($data['title']) ? strval($data['title']) : null;

        return $threat;
    }
}
