<?php

namespace IucnApi\Model;

class Assessment
{
    protected ?int $assessmentYear = null;
    protected ?string $categoryCode = null;
    protected ?string $categoryName = null;
    protected ?int $publicationYear = null;

    public function getAssessmentYear(): ?int
    {
        return $this->assessmentYear;
    }

    public function getCategoryCode(): ?string
    {
        return $this->categoryCode;
    }

    public function getCategoryName(): ?string
    {
        return $this->categoryName;
    }

    public function getPublicationYear(): ?int
    {
        return $this->publicationYear;
    }

    /**
     * @param array<string, mixed> $data
     */
    public static function createFromArray(array $data): Assessment
    {
        $assessment = new Assessment();
        $assessment->assessmentYear = !is_null($data['assess_year']) ? intval($data['assess_year']) : null;
        $assessment->categoryCode = !is_null($data['code']) ? strval($data['code']) : null;
        $assessment->categoryName = !is_null($data['category']) ? strval($data['category']) : null;
        $assessment->publicationYear = !is_null($data['year']) ? intval($data['year']) : null;

        return $assessment;
    }
}
