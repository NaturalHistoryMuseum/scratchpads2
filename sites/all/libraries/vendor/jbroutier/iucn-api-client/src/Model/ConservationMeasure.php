<?php

namespace IucnApi\Model;

class ConservationMeasure
{
    protected ?string $code = null;
    protected ?string $title = null;

    public function getCode(): ?string
    {
        return $this->code;
    }

    public function getTitle(): ?string
    {
        return $this->title;
    }

    /**
     * @param array<string, mixed> $data
     */
    public static function createFromArray(array $data): ConservationMeasure
    {
        $conservationMeasure = new ConservationMeasure();
        $conservationMeasure->code = !is_null($data['code']) ? strval($data['code']) : null;
        $conservationMeasure->title = !is_null($data['title']) ? strval($data['title']) : null;

        return $conservationMeasure;
    }
}
