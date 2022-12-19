<?php

namespace IucnApi\Model;

class GrowthForm
{
    protected ?string $name = null;

    public function getName(): ?string
    {
        return $this->name;
    }

    /**
     * @param array<string, mixed> $data
     */
    public static function createFromArray(array $data): GrowthForm
    {
        $plantGrowthForm = new GrowthForm();
        $plantGrowthForm->name = !is_null($data['name']) ? strval($data['name']) : null;

        return $plantGrowthForm;
    }
}
