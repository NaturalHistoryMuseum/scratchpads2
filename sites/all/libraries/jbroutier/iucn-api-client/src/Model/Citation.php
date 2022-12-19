<?php

namespace IucnApi\Model;

class Citation
{
    protected ?string $citation = null;

    public function getCitation(): ?string
    {
        return $this->citation;
    }

    /**
     * @param array<string, mixed> $data
     */
    public static function createFromArray(array $data): Citation
    {
        $citation = new Citation();
        $citation->citation = !is_null($data['citation']) ? strval($data['citation']) : null;

        return $citation;
    }
}
