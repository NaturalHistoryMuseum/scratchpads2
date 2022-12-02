<?php

namespace IucnApi;

use Doctrine\Common\Collections\Collection;
use IucnApi\Exception\IucnApiException;
use IucnApi\Model\Assessment;
use IucnApi\Model\Citation;
use IucnApi\Model\CommonName;
use IucnApi\Model\ConservationMeasure;
use IucnApi\Model\Country;
use IucnApi\Model\Group;
use IucnApi\Model\Habitat;
use IucnApi\Model\NarrativeText;
use IucnApi\Model\Occurrence;
use IucnApi\Model\GrowthForm;
use IucnApi\Model\Region;
use IucnApi\Model\Species;
use IucnApi\Model\SpeciesDetails;
use IucnApi\Model\Synonym;
use IucnApi\Model\Threat;

interface ClientInterface
{
    /**
     * Get a list of countries.
     *
     * @see https://apiv3.iucnredlist.org/api/v3/docs#countries
     *
     * @return Collection<int, Country>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getCountries(): Collection;

    /**
     * Get a list of comprehensive groups.
     *
     * @see https://apiv3.iucnredlist.org/api/v3/docs#comp-groups
     *
     * @return Collection<int, Group>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getComprehensiveGroups(): Collection;

    /**
     * Get a list of growth forms for a plant species, by species ID.
     *
     * @see https://apiv3.iucnredlist.org/api/v3/docs#growth-forms-id
     *
     * @param int $id The species ID.
     * @param string|null $region An optional region identifier.
     *
     * @return Collection<int, GrowthForm>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getPlantGrowthFormsById(int $id, string $region = null): Collection;

    /**
     * Get a list of growth forms for a plant species, by species name.
     *
     * @see https://apiv3.iucnredlist.org/api/v3/docs#growth-forms-name
     *
     * @param string $name The species name.
     * @param string|null $region An optional region identifier.
     *
     * @return Collection<int, GrowthForm>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getPlantGrowthFormsByName(string $name, string $region = null): Collection;

    /**
     * Get a list of regions.
     *
     * @see https://apiv3.iucnredlist.org/api/v3/docs#regions
     *
     * @return Collection<int, Region>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getRegions(): Collection;

    /**
     * Get a list of historical assessments for a species, by species ID.
     *
     * @see http://apiv3.iucnredlist.org/api/v3/docs#species-history-id
     *
     * @param int $id The species ID.
     * @param string|null $region An optional region identifier.
     *
     * @return Collection<int, Assessment>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesAssessmentsById(int $id, string $region = null): Collection;

    /**
     * Get a list of historical assessments for a species, by species name.
     *
     * @see http://apiv3.iucnredlist.org/api/v3/docs#species-history-name
     *
     * @param string $name The species name.
     * @param string|null $region An optional region identifier.
     *
     * @return Collection<int, Assessment>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesAssessmentsByName(string $name, string $region = null): Collection;

    /**
     * Get a list of species by category.
     *
     * @see http://apiv3.iucnredlist.org/api/v3/docs#species-category
     *
     * @param string $category The category code.
     *
     * @return Collection<int, Species>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesByCategory(string $category): Collection;

    /**
     * Get a list of species by comprehensive group.
     *
     * @see https://apiv3.iucnredlist.org/api/v3/docs#comp-groups-species
     *
     * @param string $group The group code.
     *
     * @return Collection<int, Species>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesByComprehensiveGroup(string $group): Collection;

    /**
     * Get a list of species by country.
     *
     * @see http://apiv3.iucnredlist.org/api/v3/docs#countries-species
     *
     * @param string $country The country, as an ISO 3166-1 alpha 2 country code.
     *
     * @return Collection<int, Species>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesByCountry(string $country): Collection;

    /**
     * Get a species, by species ID.
     *
     * @see http://apiv3.iucnredlist.org/api/v3/docs#species-individual-id
     *
     * @param int $id The species ID.
     * @param string|null $region An optional region identifier.
     *
     * @return SpeciesDetails|null
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesById(int $id, string $region = null): ?SpeciesDetails;

    /**
     * Get a species, by species name.
     *
     * @see http://apiv3.iucnredlist.org/api/v3/docs#species-individual-name
     *
     * @param string $name The species name.
     * @param string|null $region An optional region identifier.
     *
     * @return SpeciesDetails|null
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesByName(string $name, string $region = null);

    /**
     * Get the citation for a species, by species ID.
     *
     * @see http://apiv3.iucnredlist.org/api/v3/docs#species-citation-id
     *
     * @param int $id The species ID.
     * @param string|null $region An optional region identifier.
     *
     * @return Citation|null
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesCitationById(int $id, string $region = null): ?Citation;

    /**
     * Get the citation for a species, by species name.
     *
     * @see http://apiv3.iucnredlist.org/api/v3/docs#species-citation-name
     *
     * @param string $name The species name.
     * @param string|null $region An optional region identifier.
     *
     * @return Citation|null
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesCitationByName(string $name, string $region = null): ?Citation;

    /**
     * Get a list of common names for a species.
     *
     * @see http://apiv3.iucnredlist.org/api/v3/docs#species-common
     *
     * @param string $name The species name.
     *
     * @return Collection<int, CommonName>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesCommonNames(string $name): Collection;

    /**
     * Get a list of conservation measures for a species, by species ID.
     *
     * @see https://apiv3.iucnredlist.org/api/v3/docs#measures-id
     *
     * @param int $id The species ID.
     * @param string|null $region An optional region identifier.
     *
     * @return Collection<int, ConservationMeasure>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesConservationMeasuresById(int $id, string $region = null): Collection;

    /**
     * Get a list of conservation measures for a species, by species name.
     *
     * @see https://apiv3.iucnredlist.org/api/v3/docs#measures-name
     *
     * @param string $name The species name.
     * @param string|null $region An optional region identifier.
     *
     * @return Collection<int, ConservationMeasure>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesConservationMeasuresByName(string $name, string $region = null): Collection;

    /**
     * Get the total species count.
     *
     * @see http://apiv3.iucnredlist.org/api/v3/docs#species-count
     *
     * @param string|null $region An optional region identifier.
     * @param bool $includeSubspecies Whether subspecies should be included or not.
     *
     * @return int
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesCount(string $region = null, bool $includeSubspecies = true): int;

    /**
     * Get a list of habitats for a species, by species ID.
     *
     * @see https://apiv3.iucnredlist.org/api/v3/docs#habitat-id
     *
     * @param int $id The species ID.
     * @param string|null $region An optional region identifier.
     *
     * @return Collection<int, Habitat>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesHabitatsById(int $id, string $region = null): Collection;

    /**
     * Get a list of habitats for a species, by species name.
     *
     * @see https://apiv3.iucnredlist.org/api/v3/docs#habitat-name
     *
     * @param string $name The species name.
     * @param string|null $region An optional region identifier.
     *
     * @return Collection<int, Habitat>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesHabitatsByName(string $name, string $region = null): Collection;

    /**
     * Get narrative information for a species, by species ID.
     *
     * @see http://apiv3.iucnredlist.org/api/v3/docs#species-narrative-id
     *
     * @param int $id The species ID.
     * @param string|null $region An optional region identifier.
     *
     * @return NarrativeText|null
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesNarrativeTextById(int $id, string $region = null): ?NarrativeText;

    /**
     * Get narrative information for a species, by species name.
     *
     * @see http://apiv3.iucnredlist.org/api/v3/docs#species-narrative-name
     *
     * @param string $name The species name.
     * @param string|null $region An optional region identifier.
     *
     * @return NarrativeText|null
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesNarrativeTextByName(string $name, string $region = null): ?NarrativeText;

    /**
     * Get a list of countries of occurrence for a species, by species ID.
     *
     * @see http://apiv3.iucnredlist.org/api/v3/docs#species-occurrence-id
     *
     * @param int $id The species ID.
     * @param string|null $region An optional region identifier.
     *
     * @return Collection<int, Occurrence>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesOccurrencesById(int $id, string $region = null): Collection;

    /**
     * Get a list of countries of occurrence for a species, by species name.
     *
     * @see http://apiv3.iucnredlist.org/api/v3/docs#species-occurrence-name
     *
     * @param string $name The species name.
     * @param string|null $region An optional region identifier.
     *
     * @return Collection<int, Occurrence>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesOccurrencesByName(string $name, string $region = null): Collection;

    /**
     * Get a list of synonyms for a species.
     *
     * @see http://apiv3.iucnredlist.org/api/v3/docs#species-synonym
     *
     * @param string $name The species name.
     *
     * @return Collection<int, Synonym>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesSynonyms(string $name): Collection;

    /**
     * Get a list of threats to a species, by species ID.
     *
     * @see http://apiv3.iucnredlist.org/api/v3/docs#threat-id
     *
     * @param int $id The species ID.
     * @param string|null $region An optional region identifier.
     *
     * @return Collection<int, Threat>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesThreatsById(int $id, string $region = null): Collection;

    /**
     * Get a list of threats to a species, by species name.
     *
     * @see http://apiv3.iucnredlist.org/api/v3/docs#threat-name
     *
     * @param string $name The species name.
     * @param string|null $region An optional region identifier.
     *
     * @return Collection<int, Threat>
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getSpeciesThreatsByName(string $name, string $region = null): Collection;

    /**
     * Get the version of the IUCN Red List database.
     *
     * @see http://apiv3.iucnredlist.org/api/v3/docs#version
     *
     * @return string
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getVersion(): string;

    /**
     * Get the IUCN website link for a species.
     *
     * @see https://apiv3.iucnredlist.org/api/v3/docs#weblink
     *
     * @param string $name The species name.
     *
     * @return string|null
     *
     * @throws IucnApiException If any error occurs.
     */
    public function getWebsiteLink(string $name): ?string;
}
