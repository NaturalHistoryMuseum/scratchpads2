<?php

namespace IucnApi\Model;

class Synonym
{
    protected ?int $acceptedId = null;
    protected ?string $acceptedName = null;
    protected ?string $acceptedNameAuthority = null;
    protected ?string $synonym = null;
    protected ?string $synonymAuthority = null;

    public function getAcceptedId(): ?int
    {
        return $this->acceptedId;
    }

    public function getAcceptedName(): ?string
    {
        return $this->acceptedName;
    }

    public function getAcceptedNameAuthority(): ?string
    {
        return $this->acceptedNameAuthority;
    }

    public function getSynonym(): ?string
    {
        return $this->synonym;
    }

    public function getSynonymAuthority(): ?string
    {
        return $this->synonymAuthority;
    }

    /**
     * @param array<string, mixed> $data
     */
    public static function createFromArray(array $data): Synonym
    {
        $synonym = new Synonym();
        $synonym->acceptedId = !is_null($data['accepted_id']) ? intval($data['accepted_id']) : null;
        $synonym->acceptedName = !is_null($data['accepted_name']) ? strval($data['accepted_name']) : null;
        $synonym->acceptedNameAuthority = !is_null($data['authority']) ? strval($data['authority']) : null;
        $synonym->synonym = !is_null($data['synonym']) ? strval($data['synonym']) : null;
        $synonym->synonymAuthority = !is_null($data['syn_authority']) ? strval($data['syn_authority']) : null;

        return $synonym;
    }
}
