<?php

/**
 * @noinspection PhpUnhandledExceptionInspection
 * @noinspection SpellCheckingInspection
 */

namespace IucnApi\Tests\Functional;

use IucnApi\Client;
use IucnApi\ClientInterface;
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
use PHPUnit\Framework\TestCase;
use Symfony\Component\HttpClient\MockHttpClient;
use Symfony\Component\HttpClient\Response\MockResponse;

final class MockClient extends Client
{
    public function __construct(callable $responses)
    {
        parent::__construct('');

        $this->httpClient = new MockHttpClient($responses, 'https://apiv3.iucnredlist.org/api/v3/');
    }
}

final class ClientTest extends TestCase
{
    protected ClientInterface $client;

    public function setUp(): void
    {
        if (($json = file_get_contents(dirname(__FILE__) . '/responses.json')) === false) {
            throw new \Exception('Could not read "responses.json" file.');
        }

        if (($responses = json_decode($json, true)) === false) {
            throw new \Exception('Could not parse "responses.json" file.');
        }

        $responses = (array)$responses;

        $this->client = new MockClient(function ($method, $url) use ($responses) {
            if (array_key_exists($url, $responses)) {
                return new MockResponse((string)json_encode($responses[$url]));
            }

            return new MockResponse();
        });
    }

    public function testGetCountries(): void
    {
        $countries = $this->client->getCountries();

        self::assertCount(251, $countries);

        $country = $countries->first();

        self::assertInstanceOf(Country::class, $country);
        self::assertEquals('UZ', $country->getCode());
        self::assertEquals('Uzbekistan', $country->getName());
    }

    public function testGetComprehensiveGroups(): void
    {
        $groups = $this->client->getComprehensiveGroups();

        self::assertCount(31, $groups);

        $group = $groups->first();

        self::assertInstanceOf(Group::class, $group);
        self::assertEquals('reef_building_corals', $group->getCode());
    }

    public function testGetPlantGrowthFormsById(): void
    {
        $growthForms = $this->client->getPlantGrowthFormsById(19891625);

        self::assertCount(4, $growthForms);

        $growthForm = $growthForms->first();

        self::assertInstanceOf(GrowthForm::class, $growthForm);
        self::assertEquals('Annual', $growthForm->getName());

        $growthForms = $this->client->getPlantGrowthFormsById(63532, 'europe');

        self::assertCount(1, $growthForms);

        $growthForm = $growthForms->first();

        self::assertInstanceOf(GrowthForm::class, $growthForm);
        self::assertEquals('Tree - large', $growthForm->getName());
    }

    public function testGetPlantGrowthFormsByName(): void
    {
        $growthForms = $this->client->getPlantGrowthFormsByName('Mucuna bracteata');

        self::assertCount(4, $growthForms);

        $growthForm = $growthForms->first();

        self::assertInstanceOf(GrowthForm::class, $growthForm);
        self::assertEquals('Annual', $growthForm->getName());

        $growthForms = $this->client->getPlantGrowthFormsByName('Quercus robur', 'europe');

        self::assertCount(1, $growthForms);

        $growthForm = $growthForms->first();

        self::assertInstanceOf(GrowthForm::class, $growthForm);
        self::assertEquals('Tree - large', $growthForm->getName());
    }

    public function testGetRegions(): void
    {
        $regions = $this->client->getRegions();

        self::assertCount(10, $regions);

        $region = $regions->first();

        self::assertInstanceOf(Region::class, $region);
        self::assertEquals('northeastern_africa', $region->getIdentifier());
        self::assertEquals('Northeastern Africa', $region->getName());
    }

    public function testGetSpeciesAssessmentsById(): void
    {
        $assessments = $this->client->getSpeciesAssessmentsById(181008073);

        self::assertCount(1, $assessments);

        $assessment = $assessments->first();

        self::assertInstanceOf(Assessment::class, $assessment);
        self::assertEquals('2020', $assessment->getAssessmentYear());
        self::assertEquals('EN', $assessment->getCategoryCode());
        self::assertEquals('Endangered', $assessment->getCategoryName());
        self::assertEquals('2021', $assessment->getPublicationYear());

        $assessments = $this->client->getSpeciesAssessmentsById(22823, 'europe');

        self::assertCount(1, $assessments);

        $assessment = $assessments->first();

        self::assertInstanceOf(Assessment::class, $assessment);
        self::assertEquals('2006', $assessment->getAssessmentYear());
        self::assertEquals('VU', $assessment->getCategoryCode());
        self::assertEquals('Vulnerable', $assessment->getCategoryName());
        self::assertEquals('2007', $assessment->getPublicationYear());
    }

    public function testGetSpeciesAssessmentsByName(): void
    {
        $assessments = $this->client->getSpeciesAssessmentsByName('Loxodonta africana');

        self::assertCount(1, $assessments);

        $assessment = $assessments->first();

        self::assertInstanceOf(Assessment::class, $assessment);
        self::assertEquals('2020', $assessment->getAssessmentYear());
        self::assertEquals('EN', $assessment->getCategoryCode());
        self::assertEquals('Endangered', $assessment->getCategoryName());
        self::assertEquals('2021', $assessment->getPublicationYear());

        $assessments = $this->client->getSpeciesAssessmentsByName('Ursus maritimus', 'europe');

        self::assertCount(1, $assessments);

        $assessment = $assessments->first();

        self::assertInstanceOf(Assessment::class, $assessment);
        self::assertEquals('2006', $assessment->getAssessmentYear());
        self::assertEquals('VU', $assessment->getCategoryCode());
        self::assertEquals('Vulnerable', $assessment->getCategoryName());
        self::assertEquals('2007', $assessment->getPublicationYear());
    }

    public function testGetSpeciesByCategory(): void
    {
        $species = $this->client->getSpeciesByCategory('LRlc');

        self::assertCount(512, $species);

        $species = $species->first();

        self::assertInstanceOf(Species::class, $species);
        self::assertEquals('LR/lc', $species->getCategory());
        self::assertEquals('Abarema commutata', $species->getScientificName());
        self::assertNull($species->getSubpopulation());
        self::assertNull($species->getSubspeciesName());
        self::assertNull($species->getSubspeciesRank());
        self::assertEquals(36549, $species->getTaxonId());
    }

    public function testGetSpeciesByComprehensiveGroup(): void
    {
        $species = $this->client->getSpeciesByComprehensiveGroup('mammals');

        self::assertCount(6423, $species);

        $species = $species->first();

        self::assertInstanceOf(Species::class, $species);
        self::assertEquals('DD', $species->getCategory());
        self::assertEquals('Abditomys latidens', $species->getScientificName());
        self::assertNull($species->getSubpopulation());
        self::assertNull($species->getSubspeciesName());
        self::assertNull($species->getSubspeciesRank());
        self::assertEquals(42641, $species->getTaxonId());
    }

    public function testGetSpeciesByCountry(): void
    {
        $species = $this->client->getSpeciesByCountry('AZ');

        self::assertCount(1162, $species);

        $species = $species->first();

        self::assertInstanceOf(Species::class, $species);
        self::assertEquals('LC', $species->getCategory());
        self::assertEquals('Abies nordmanniana', $species->getScientificName());
        self::assertNull($species->getSubpopulation());
        self::assertNull($species->getSubspeciesName());
        self::assertNull($species->getSubspeciesRank());
        self::assertEquals(42293, $species->getTaxonId());
    }

    public function testGetSpeciesById(): void
    {
        $species = $this->client->getSpeciesById(181008073);

        self::assertInstanceOf(SpeciesDetails::class, $species);
        self::assertEquals('EN', $species->getCategory());
        self::assertEquals('MAMMALIA', $species->getClass());
        self::assertEquals('ELEPHANTIDAE', $species->getFamily());
        self::assertEquals('Loxodonta', $species->getGenus());
        self::assertEquals('ANIMALIA', $species->getKingdom());
        self::assertEquals('African Savanna Elephant', $species->getMainCommonName());
        self::assertEquals('PROBOSCIDEA', $species->getOrder());
        self::assertEquals('CHORDATA', $species->getPhylum());
        self::assertEquals('Loxodonta africana', $species->getScientificName());
        self::assertEquals(181008073, $species->getTaxonId());

        $species = $this->client->getSpeciesById(22694927, 'europe');

        self::assertInstanceOf(SpeciesDetails::class, $species);
        self::assertEquals('EN', $species->getCategory());
        self::assertEquals('AVES', $species->getClass());
        self::assertEquals('ALCIDAE', $species->getFamily());
        self::assertEquals('Fratercula', $species->getGenus());
        self::assertEquals('ANIMALIA', $species->getKingdom());
        self::assertEquals('Atlantic Puffin', $species->getMainCommonName());
        self::assertEquals('CHARADRIIFORMES', $species->getOrder());
        self::assertEquals('CHORDATA', $species->getPhylum());
        self::assertEquals('Fratercula arctica', $species->getScientificName());
        self::assertEquals(22694927, $species->getTaxonId());
    }

    public function testGetSpeciesByName(): void
    {
        $species = $this->client->getSpeciesByName('Loxodonta africana');

        self::assertInstanceOf(SpeciesDetails::class, $species);
        self::assertEquals('EN', $species->getCategory());
        self::assertEquals('MAMMALIA', $species->getClass());
        self::assertEquals('ELEPHANTIDAE', $species->getFamily());
        self::assertEquals('Loxodonta', $species->getGenus());
        self::assertEquals('ANIMALIA', $species->getKingdom());
        self::assertEquals('African Savanna Elephant', $species->getMainCommonName());
        self::assertEquals('PROBOSCIDEA', $species->getOrder());
        self::assertEquals('CHORDATA', $species->getPhylum());
        self::assertEquals('Loxodonta africana', $species->getScientificName());
        self::assertEquals(181008073, $species->getTaxonId());

        $species = $this->client->getSpeciesByName('Fratercula arctica', 'europe');

        self::assertInstanceOf(SpeciesDetails::class, $species);
        self::assertEquals('EN', $species->getCategory());
        self::assertEquals('AVES', $species->getClass());
        self::assertEquals('ALCIDAE', $species->getFamily());
        self::assertEquals('Fratercula', $species->getGenus());
        self::assertEquals('ANIMALIA', $species->getKingdom());
        self::assertEquals('Atlantic Puffin', $species->getMainCommonName());
        self::assertEquals('CHARADRIIFORMES', $species->getOrder());
        self::assertEquals('CHORDATA', $species->getPhylum());
        self::assertEquals('Fratercula arctica', $species->getScientificName());
        self::assertEquals(22694927, $species->getTaxonId());
    }

    public function testGetSpeciesCitationById(): void
    {
        $citation = $this->client->getSpeciesCitationById(181008073);

        self::assertInstanceOf(Citation::class, $citation);
        self::assertStringStartsWith('Gobush, K.S., Edwards, C.T.T', (string)$citation->getCitation());

        $citation = $this->client->getSpeciesCitationById(2467, 'europe');

        self::assertInstanceOf(Citation::class, $citation);
        self::assertStringStartsWith('Species account by IUCN', (string)$citation->getCitation());
    }

    public function testGetSpeciesCitationByName(): void
    {
        $citation = $this->client->getSpeciesCitationByName('Loxodonta africana');

        self::assertInstanceOf(Citation::class, $citation);
        self::assertStringStartsWith('Gobush, K.S., Edwards, C.T.T', (string)$citation->getCitation());

        $citation = $this->client->getSpeciesCitationByName('Balaena mysticetus', 'europe');

        self::assertInstanceOf(Citation::class, $citation);
        self::assertStringStartsWith('Species account by IUCN', (string)$citation->getCitation());
    }

    public function testGetSpeciesCommonNames(): void
    {
        $commonNames = $this->client->getSpeciesCommonNames('Loxodonta africana');

        self::assertCount(7, $commonNames);

        $commonName = $commonNames->first();

        self::assertInstanceOf(CommonName::class, $commonName);
        self::assertEquals('eng', $commonName->getLanguage());
        self::assertEquals('African Savanna Elephant', $commonName->getName());
        self::assertTrue($commonName->isPrimary());
    }

    public function testGetSpeciesConservationMeasuresById(): void
    {
        $conservationMeasures = $this->client->getSpeciesConservationMeasuresById(181008073);

        self::assertCount(20, $conservationMeasures);

        $conservationMeasure = $conservationMeasures->first();

        self::assertInstanceOf(ConservationMeasure::class, $conservationMeasure);
        self::assertEquals('1.1', $conservationMeasure->getCode());
        self::assertEquals('Site/area protection', $conservationMeasure->getTitle());

        $conservationMeasures = $this->client->getSpeciesConservationMeasuresById(22823, 'europe');

        self::assertCount(3, $conservationMeasures);

        $conservationMeasure = $conservationMeasures->first();

        self::assertInstanceOf(ConservationMeasure::class, $conservationMeasure);
        self::assertEquals('1.1', $conservationMeasure->getCode());
        self::assertEquals('Site/area protection', $conservationMeasure->getTitle());
    }

    public function testGetSpeciesConservationMeasuresByName(): void
    {
        $conservationMeasures = $this->client->getSpeciesConservationMeasuresByName('Loxodonta africana');

        self::assertCount(20, $conservationMeasures);

        $conservationMeasure = $conservationMeasures->first();

        self::assertInstanceOf(ConservationMeasure::class, $conservationMeasure);
        self::assertEquals('1.1', $conservationMeasure->getCode());
        self::assertEquals('Site/area protection', $conservationMeasure->getTitle());

        $conservationMeasures = $this->client->getSpeciesConservationMeasuresByName('Ursus maritimus', 'europe');

        self::assertCount(3, $conservationMeasures);

        $conservationMeasure = $conservationMeasures->first();

        self::assertInstanceOf(ConservationMeasure::class, $conservationMeasure);
        self::assertEquals('1.1', $conservationMeasure->getCode());
        self::assertEquals('Site/area protection', $conservationMeasure->getTitle());
    }

    public function testGetSpeciesCount(): void
    {
        self::assertEquals(150754, $this->client->getSpeciesCount());
        self::assertEquals(147517, $this->client->getSpeciesCount(null, false));
        self::assertEquals(16232, $this->client->getSpeciesCount('europe'));
        self::assertEquals(16132, $this->client->getSpeciesCount('europe', false));
    }

    public function testGetSpeciesHabitatsById(): void
    {
        $habitats = $this->client->getSpeciesHabitatsById(181008073);

        self::assertCount(17, $habitats);

        $habitat = $habitats->first();

        self::assertInstanceOf(Habitat::class, $habitat);
        self::assertEquals('1.5', $habitat->getCode());
        self::assertEquals('Yes', $habitat->getMajorImportance());
        self::assertEquals('Forest - Subtropical/Tropical Dry', $habitat->getName());
        self::assertNull($habitat->getSeason());
        self::assertEquals('Suitable', $habitat->getSuitability());

        $habitats = $this->client->getSpeciesHabitatsById(22823, 'europe');

        self::assertCount(6, $habitats);

        $habitat = $habitats->first();

        self::assertInstanceOf(Habitat::class, $habitat);
        self::assertEquals('12.1', $habitat->getCode());
        self::assertNull($habitat->getMajorImportance());
        self::assertEquals('Marine Intertidal - Rocky Shoreline', $habitat->getName());
        self::assertNull($habitat->getSeason());
        self::assertEquals('Suitable', $habitat->getSuitability());
    }

    public function testGetSpeciesHabitatsByName(): void
    {
        $habitats = $this->client->getSpeciesHabitatsByName('Loxodonta africana');

        self::assertCount(17, $habitats);

        $habitat = $habitats->first();

        self::assertInstanceOf(Habitat::class, $habitat);
        self::assertEquals('1.5', $habitat->getCode());
        self::assertEquals('Yes', $habitat->getMajorImportance());
        self::assertEquals('Forest - Subtropical/Tropical Dry', $habitat->getName());
        self::assertNull($habitat->getSeason());
        self::assertEquals('Suitable', $habitat->getSuitability());

        $habitats = $this->client->getSpeciesHabitatsByName('Ursus maritimus', 'europe');

        self::assertCount(6, $habitats);

        $habitat = $habitats->first();

        self::assertInstanceOf(Habitat::class, $habitat);
        self::assertEquals('12.1', $habitat->getCode());
        self::assertNull($habitat->getMajorImportance());
        self::assertEquals('Marine Intertidal - Rocky Shoreline', $habitat->getName());
        self::assertNull($habitat->getSeason());
        self::assertEquals('Suitable', $habitat->getSuitability());
    }

    public function testGetSpeciesNarrativeTextById(): void
    {
        $narrativeText = $this->client->getSpeciesNarrativeTextById(181008073);

        self::assertInstanceOf(NarrativeText::class, $narrativeText);
        self::assertStringStartsWith('<p>The African Savanna', (string)$narrativeText->getConservationMeasures());
        self::assertStringStartsWith('<p>African Savanna Elephants', (string)$narrativeText->getGeographicRange());
        self::assertStringStartsWith('<p>African Savanna Elephants', (string)$narrativeText->getHabitat());
        self::assertStringStartsWith('<p>Over the past century', (string)$narrativeText->getPopulation());
        self::assertEquals('decreasing', $narrativeText->getPopulationTrend());
        self::assertStringStartsWith('<p>The African Savanna Elephant', (string)$narrativeText->getRationale());
        self::assertStringStartsWith('Three elephant taxa remain', (string)$narrativeText->getTaxonomicNotes());
        self::assertStringStartsWith('Poaching of African Savanna Elephants', (string)$narrativeText->getThreats());
        self::assertStringStartsWith('<strong>Ivory:</strong>', (string)$narrativeText->getUseTrade());

        $narrativeText = $this->client->getSpeciesNarrativeTextById(2467, 'europe');

        self::assertInstanceOf(NarrativeText::class, $narrativeText);
        self::assertStringStartsWith('The International', (string)$narrativeText->getConservationMeasures());
        self::assertStringStartsWith('Bowhead whales are found', (string)$narrativeText->getGeographicRange());
        self::assertStringStartsWith('The seasonal distribution', (string)$narrativeText->getHabitat());
        self::assertStringStartsWith('Current population size', (string)$narrativeText->getPopulation());
        self::assertEquals('unknown', $narrativeText->getPopulationTrend());
        self::assertStringStartsWith('This species is assessed', (string)$narrativeText->getRationale());
        self::assertStringStartsWith('The taxonomy is not in doubt', (string)$narrativeText->getTaxonomicNotes());
        self::assertStringStartsWith('Heavy commercial hunting', (string)$narrativeText->getThreats());
        self::assertNull($narrativeText->getUseTrade());
    }

    public function testGetSpeciesNarrativeTextByName(): void
    {
        $narrativeText = $this->client->getSpeciesNarrativeTextByName('Loxodonta africana');

        self::assertInstanceOf(NarrativeText::class, $narrativeText);
        self::assertStringStartsWith('<p>The African Savanna', (string)$narrativeText->getConservationMeasures());
        self::assertStringStartsWith('<p>African Savanna Elephants', (string)$narrativeText->getGeographicRange());
        self::assertStringStartsWith('<p>African Savanna Elephants', (string)$narrativeText->getHabitat());
        self::assertStringStartsWith('<p>Over the past century', (string)$narrativeText->getPopulation());
        self::assertEquals('decreasing', $narrativeText->getPopulationTrend());
        self::assertStringStartsWith('<p>The African Savanna Elephant', (string)$narrativeText->getRationale());
        self::assertStringStartsWith('Three elephant taxa remain', (string)$narrativeText->getTaxonomicNotes());
        self::assertStringStartsWith('Poaching of African Savanna Elephants', (string)$narrativeText->getThreats());
        self::assertStringStartsWith('<strong>Ivory:</strong>', (string)$narrativeText->getUseTrade());

        $narrativeText = $this->client->getSpeciesNarrativeTextByName('Fratercula arctica', 'europe');

        self::assertInstanceOf(NarrativeText::class, $narrativeText);
        self::assertStringStartsWith('<strong>Conservation Actions', (string)$narrativeText->getConservationMeasures());
        self::assertStringStartsWith('The species can be found', (string)$narrativeText->getGeographicRange());
        self::assertStringStartsWith('The breeding range', (string)$narrativeText->getHabitat());
        self::assertStringStartsWith('The European breeding', (string)$narrativeText->getPopulation());
        self::assertEquals('decreasing', $narrativeText->getPopulationTrend());
        self::assertStringStartsWith('<strong>European regional assessment', (string)$narrativeText->getRationale());
        self::assertNull($narrativeText->getTaxonomicNotes());
        self::assertStringStartsWith('This species is highly', (string)$narrativeText->getThreats());
        self::assertNull($narrativeText->getUseTrade());
    }

    public function testGetSpeciesOccurrencesById(): void
    {
        $occurrences = $this->client->getSpeciesOccurrencesById(181008073);

        self::assertCount(26, $occurrences);

        $occurrence = $occurrences->first();

        self::assertInstanceOf(Occurrence::class, $occurrence);
        self::assertEquals('AO', $occurrence->getCountryCode());
        self::assertEquals('Angola', $occurrence->getCountryName());
        self::assertEquals('Native', $occurrence->getDistributionCode());
        self::assertEquals('Native', $occurrence->getOrigin());
        self::assertEquals('Extant', $occurrence->getPresence());

        $occurrences = $this->client->getSpeciesOccurrencesById(22823, 'europe');

        self::assertCount(3, $occurrences);

        $occurrence = $occurrences->first();

        self::assertInstanceOf(Occurrence::class, $occurrence);
        self::assertEquals('NO', $occurrence->getCountryCode());
        self::assertEquals('Norway', $occurrence->getCountryName());
        self::assertEquals('Native', $occurrence->getDistributionCode());
        self::assertEquals('Native', $occurrence->getOrigin());
        self::assertEquals('Extant', $occurrence->getPresence());
    }

    public function testGetSpeciesOccurrencesByName(): void
    {
        $occurrences = $this->client->getSpeciesOccurrencesByName('Loxodonta africana');

        self::assertCount(26, $occurrences);

        $occurrence = $occurrences->first();

        self::assertInstanceOf(Occurrence::class, $occurrence);
        self::assertEquals('AO', $occurrence->getCountryCode());
        self::assertEquals('Angola', $occurrence->getCountryName());
        self::assertEquals('Native', $occurrence->getDistributionCode());
        self::assertEquals('Native', $occurrence->getOrigin());
        self::assertEquals('Extant', $occurrence->getPresence());

        $occurrences = $this->client->getSpeciesOccurrencesByName('Ursus maritimus', 'europe');

        self::assertCount(3, $occurrences);

        $occurrence = $occurrences->first();

        self::assertInstanceOf(Occurrence::class, $occurrence);
        self::assertEquals('NO', $occurrence->getCountryCode());
        self::assertEquals('Norway', $occurrence->getCountryName());
        self::assertEquals('Native', $occurrence->getDistributionCode());
        self::assertEquals('Native', $occurrence->getOrigin());
        self::assertEquals('Extant', $occurrence->getPresence());
    }

    public function testGetSpeciesSynonyms(): void
    {
        $synonyms = $this->client->getSpeciesSynonyms('Loxodonta africana');

        self::assertCount(3, $synonyms);

        $synonym = $synonyms->first();

        self::assertInstanceOf(Synonym::class, $synonym);
        self::assertEquals(181007989, $synonym->getAcceptedId());
        self::assertEquals('Loxodonta cyclotis', $synonym->getAcceptedName());
        self::assertEquals('Matschie, 1900', $synonym->getAcceptedNameAuthority());
        self::assertEquals('Loxodonta africana', $synonym->getSynonym());
        self::assertNull($synonym->getSynonymAuthority());
    }

    public function testGetSpeciesThreatsById(): void
    {
        $threats = $this->client->getSpeciesThreatsById(181008073);

        self::assertCount(40, $threats);

        $threat = $threats->first();

        self::assertInstanceOf(Threat::class, $threat);
        self::assertEquals('1.1', $threat->getCode());
        self::assertNull($threat->getInvasive());
        self::assertEquals('Minority (<50%)', $threat->getScope());
        self::assertEquals('Unknown', $threat->getScore());
        self::assertEquals('Unknown', $threat->getSeverity());
        self::assertEquals('Ongoing', $threat->getTiming());
        self::assertEquals('Housing & urban areas', $threat->getTitle());

        $threats = $this->client->getSpeciesThreatsById(2467, 'europe');

        self::assertCount(2, $threats);

        $threat = $threats->first();

        self::assertInstanceOf(Threat::class, $threat);
        self::assertEquals('9.2', $threat->getCode());
        self::assertNull($threat->getInvasive());
        self::assertNull($threat->getScope());
        self::assertNull($threat->getScore());
        self::assertNull($threat->getSeverity());
        self::assertEquals('Ongoing', $threat->getTiming());
        self::assertEquals('Industrial & military effluents', $threat->getTitle());
    }

    public function testGetSpeciesThreatsByName(): void
    {
        $threats = $this->client->getSpeciesThreatsByName('Loxodonta africana');

        self::assertCount(40, $threats);

        $threat = $threats->first();

        self::assertInstanceOf(Threat::class, $threat);
        self::assertEquals('1.1', $threat->getCode());
        self::assertNull($threat->getInvasive());
        self::assertEquals('Minority (<50%)', $threat->getScope());
        self::assertEquals('Unknown', $threat->getScore());
        self::assertEquals('Unknown', $threat->getSeverity());
        self::assertEquals('Ongoing', $threat->getTiming());
        self::assertEquals('Housing & urban areas', $threat->getTitle());

        $threats = $this->client->getSpeciesThreatsByName('Fratercula arctica', 'europe');

        self::assertCount(17, $threats);

        $threat = $threats->first();

        self::assertInstanceOf(Threat::class, $threat);
        self::assertEquals('11.1', $threat->getCode());
        self::assertNull($threat->getInvasive());
        self::assertEquals('Unknown', $threat->getScope());
        self::assertEquals('Unknown', $threat->getScore());
        self::assertEquals('Unknown', $threat->getSeverity());
        self::assertEquals('Ongoing', $threat->getTiming());
        self::assertEquals('Habitat shifting & alteration', $threat->getTitle());
    }

    public function testGetVersion(): void
    {
        self::assertEquals('2022-1', $this->client->getVersion());
    }

    public function testGetWebsiteLink(): void
    {
        self::assertEquals(
            'https://www.iucnredlist.org/species/181008073/204401095',
            $this->client->getWebsiteLink('Loxodonta africana')
        );
    }
}
