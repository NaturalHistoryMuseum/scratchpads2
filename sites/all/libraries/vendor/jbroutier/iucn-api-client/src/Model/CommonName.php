<?php

namespace IucnApi\Model;

class CommonName
{
    protected ?string $language = null;
    protected ?string $name = null;
    protected ?bool $primary = null;

    public function getLanguage(): ?string
    {
        return $this->language;
    }

    public function getName(): ?string
    {
        return $this->name;
    }

    public function isPrimary(): ?bool
    {
        return $this->primary;
    }

    /**
     * @param array<string, mixed> $data
     */
    public static function createFromArray(array $data): CommonName
    {
        $commonName = new CommonName();
        $commonName->language = !is_null($data['language']) ? strval($data['language']) : null;
        $commonName->name = !is_null($data['taxonname']) ? strval($data['taxonname']) : null;
        $commonName->primary = !is_null($data['primary']) ? boolval($data['primary']) : null;

        return $commonName;
    }
}
