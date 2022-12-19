<?php


namespace IucnApi;

use Doctrine\Common\Collections\ArrayCollection;
use Doctrine\Common\Collections\Collection;
use IucnApi\Exception\IucnApiException;
use IucnApi\Model\Assessment;
use IucnApi\Model\Citation;
use IucnApi\Model\CommonName;
use IucnApi\Model\ConservationMeasure;
use IucnApi\Model\Country;
use IucnApi\Model\Group;
use IucnApi\Model\GrowthForm;
use IucnApi\Model\Habitat;
use IucnApi\Model\NarrativeText;
use IucnApi\Model\Occurrence;
use IucnApi\Model\Region;
use IucnApi\Model\Species;
use IucnApi\Model\SpeciesDetails;
use IucnApi\Model\Synonym;
use IucnApi\Model\Threat;
use Symfony\Component\HttpClient\HttpClient;
use Symfony\Contracts\HttpClient\Exception\ExceptionInterface;
use Symfony\Contracts\HttpClient\HttpClientInterface;

class Client implements ClientInterface
{
    // credit: https://stackoverflow.com/questions/65242812/syntax-error-unexpected-request-t-string-expecting-function-t-function-o
    // >= php 7.4: protected HttpClientInterface $httpClient;
    protected $httpClient; 

    /**
     * Create a new instance of the client.
     *
     * @param string $token The API authentication token.
     * @param array<string, mixed>|null $options An optional array of options. These options will be passed directly to the underlying HTTP client.
     */
    public function __construct(string $token, ?array $options = null)
    {
        $options = array_merge_recursive($options ?? [], [
            'headers' => [
                'accept' => 'application/json',
            ],
            'query' => [
                'token' => $token,
            ],
        ]);

        $this->httpClient = HttpClient::createForBaseUri('https://apiv3.iucnredlist.org/api/v3/', $options);
    }

    public function getCountries(): Collection
    {
        try {
            $response = $this->httpClient->request('GET', 'country/list');
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['results'] as $result) {
            $country = Country::createFromArray($result);
            $collection->add($country);
        }

        return $collection;
    }

    public function getComprehensiveGroups(): Collection
    {
        try {
            $response = $this->httpClient->request('GET', 'comp-group/list');
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['result'] as $result) {
            $group = Group::createFromArray($result);
            $collection->add($group);
        }

        return $collection;
    }

    public function getPlantGrowthFormsById(int $id, string $region = null): Collection
    {
        $url = 'growth_forms/species/id/' . $id;

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['result'] as $result) {
            $plantGrowthForm = GrowthForm::createFromArray($result);
            $collection->add($plantGrowthForm);
        }

        return $collection;
    }

    public function getPlantGrowthFormsByName(string $name, string $region = null): Collection
    {
        $url = 'growth_forms/species/name/' . $name;

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['result'] as $result) {
            $plantGrowthForm = GrowthForm::createFromArray($result);
            $collection->add($plantGrowthForm);
        }

        return $collection;
    }

    public function getRegions(): Collection
    {
        try {
            $response = $this->httpClient->request('GET', 'region/list');
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['results'] as $result) {
            $region = Region::createFromArray($result);
            $collection->add($region);
        }

        return $collection;
    }

    public function getSpeciesAssessmentsById(int $id, string $region = null): Collection
    {
        $url = 'species/history/id/' . $id;

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['result'] as $result) {
            $assessment = Assessment::createFromArray($result);
            $collection->add($assessment);
        }

        return $collection;
    }

    public function getSpeciesAssessmentsByName(string $name, string $region = null): Collection
    {
        $url = 'species/history/name/' . $name;

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['result'] as $result) {
            $assessment = Assessment::createFromArray($result);
            $collection->add($assessment);
        }

        return $collection;
    }

    public function getSpeciesByCategory(string $category): Collection
    {
        try {
            $response = $this->httpClient->request('GET', 'species/category/' . $category);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['result'] as $result) {
            $result['category'] = $data['category'];
            $species = Species::createFromArray($result);
            $collection->add($species);
        }

        return $collection;
    }

    public function getSpeciesByComprehensiveGroup(string $group): Collection
    {
        try {
            $response = $this->httpClient->request('GET', 'comp-group/getspecies/' . $group);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['result'] as $result) {
            $species = Species::createFromArray($result);
            $collection->add($species);
        }

        return $collection;
    }

    public function getSpeciesByCountry(string $country): Collection
    {
        try {
            $response = $this->httpClient->request('GET', 'country/getspecies/' . $country);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['result'] as $result) {
            $species = Species::createFromArray($result);
            $collection->add($species);
        }

        return $collection;
    }

    public function getSpeciesById(int $id, string $region = null): ?SpeciesDetails
    {
        $url = 'species/id/' . $id;

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        if (count($data['result']) === 0) {
            return null;
        }

        return SpeciesDetails::createFromArray($data['result'][0]);
    }

    // public function getSpeciesByName(string $name, string $region = null): ?SpeciesDetails
    public function getSpeciesByName(string $name, string $region = null) {
        $url = 'species/' . $name;

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url, ['base_uri' => variable_get('iucn_api_v3_url', FALSE), 'query' => ['token' => variable_get('iucn_api_v3_api_key_token', FALSE)]]);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        if (count($data['result']) === 0) {
            return null;
        }

        return SpeciesDetails::createFromArray($data['result'][0]);
    }

    public function getSpeciesCitationById(int $id, string $region = null): ?Citation
    {
        $url = 'species/citation/id/' . $id;

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        if (count($data['result']) === 0) {
            return null;
        }

        return Citation::createFromArray($data['result'][0]);
    }

    public function getSpeciesCitationByName(string $name, string $region = null): ?Citation
    {
        $url = 'species/citation/' . $name;

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        if (count($data['result']) === 0) {
            return null;
        }

        return Citation::createFromArray($data['result'][0]);
    }

    public function getSpeciesCommonNames(string $name): Collection
    {
        try {
            $response = $this->httpClient->request('GET', 'species/common_names/' . $name);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['result'] as $result) {
            $commonName = CommonName::createFromArray($result);
            $collection->add($commonName);
        }

        return $collection;
    }

    public function getSpeciesConservationMeasuresById(int $id, string $region = null): Collection
    {
        $url = 'measures/species/id/' . $id;

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['result'] as $result) {
            $conservationMeasure = ConservationMeasure::createFromArray($result);
            $collection->add($conservationMeasure);
        }

        return $collection;
    }

    public function getSpeciesConservationMeasuresByName(string $name, string $region = null): Collection
    {
        $url = 'measures/species/name/' . $name;

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['result'] as $result) {
            $conservationMeasure = ConservationMeasure::createFromArray($result);
            $collection->add($conservationMeasure);
        }

        return $collection;
    }

    public function getSpeciesCount(string $region = null, bool $includeSubspecies = true): int
    {
        $url = 'speciescount';

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        return intval($includeSubspecies ? $data['count'] : $data['speciescount']);
    }

    public function getSpeciesHabitatsById(int $id, string $region = null): Collection
    {
        $url = 'habitats/species/id/' . $id;

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['result'] as $result) {
            $habitat = Habitat::createFromArray($result);
            $collection->add($habitat);
        }

        return $collection;
    }

    public function getSpeciesHabitatsByName(string $name, string $region = null): Collection
    {
        $url = 'habitats/species/name/' . $name;

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['result'] as $result) {
            $habitat = Habitat::createFromArray($result);
            $collection->add($habitat);
        }

        return $collection;
    }

    public function getSpeciesNarrativeTextById(int $id, string $region = null): ?NarrativeText
    {
        $url = 'species/narrative/id/' . $id;

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        if (count($data['result']) === 0) {
            return null;
        }

        return NarrativeText::createFromArray($data['result'][0]);
    }

    public function getSpeciesNarrativeTextByName(string $name, string $region = null): ?NarrativeText
    {
        $url = 'species/narrative/' . $name;

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        if (count($data['result']) === 0) {
            return null;
        }

        return NarrativeText::createFromArray($data['result'][0]);
    }

    public function getSpeciesOccurrencesById(int $id, string $region = null): Collection
    {
        $url = 'species/countries/id/' . $id;

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['result'] as $result) {
            $occurrence = Occurrence::createFromArray($result);
            $collection->add($occurrence);
        }

        return $collection;
    }

    public function getSpeciesOccurrencesByName(string $name, string $region = null): Collection
    {
        $url = 'species/countries/name/' . $name;

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['result'] as $result) {
            $occurrence = Occurrence::createFromArray($result);
            $collection->add($occurrence);
        }

        return $collection;
    }

    public function getSpeciesSynonyms(string $name): Collection
    {
        try {
            $response = $this->httpClient->request('GET', 'species/synonym/' . $name);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['result'] as $result) {
            $synonym = Synonym::createFromArray($result);
            $collection->add($synonym);
        }

        return $collection;
    }

    public function getSpeciesThreatsById(int $id, string $region = null): Collection
    {
        $url = 'threats/species/id/' . $id;

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['result'] as $result) {
            $threat = Threat::createFromArray($result);
            $collection->add($threat);
        }

        return $collection;
    }

    public function getSpeciesThreatsByName(string $name, string $region = null): Collection
    {
        $url = 'threats/species/name/' . $name;

        if (!is_null($region)) {
            $url .= '/region/' . $region;
        }

        try {
            $response = $this->httpClient->request('GET', $url);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        $collection = new ArrayCollection();

        foreach ($data['result'] as $result) {
            $threat = Threat::createFromArray($result);
            $collection->add($threat);
        }

        return $collection;
    }

    public function getVersion(): string
    {
        try {
            $response = $this->httpClient->request('GET', 'version');
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        return $data['version'];
    }

    public function getWebsiteLink(string $name): ?string
    {
        try {
            $response = $this->httpClient->request('GET', 'weblink/' . $name);
            $data = $response->toArray();
        } catch (ExceptionInterface $exception) {
            throw new IucnApiException($exception->getMessage(), $exception->getCode(), $exception);
        }

        if (!array_key_exists('rlurl', $data)) {
            return null;
        }

        return $data['rlurl'];
    }
}
