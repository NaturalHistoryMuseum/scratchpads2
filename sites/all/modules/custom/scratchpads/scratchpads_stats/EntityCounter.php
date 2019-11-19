<?php
namespace Scratchpads\Stats;

/**
 * A class for counting numbers of entities
 */
class EntityCounter {
	/**
	 * Create an instance of EntityCounter. Unlike EntityFieldQuery, an instance of this class
	 * can be used multiple times. Internally it creates a new instance of EntityFieldQuery each
	 * time it is executed.
	 *
	 * @param String $entity	The name of the entity to get counts for
	 * @param String $statusField	The field that designates whether an entity is eligable for counting, or null
	 */
	function __construct($entity, $statusField){
			$this->entity = $entity;
			$this->statusField = $statusField;
			$this->bundle = null;
			$this->counts = [];
	}

	/**
	 * Create the basic EntityFieldQuery instance
	 *
	 * @return \EntityFieldQuery
	 */
	private function createQuery(){
			$query = new \EntityFieldQuery();
			$query->entityCondition('entity_type', $this->entity);
			if($this->bundle) {
					$query->entityCondition('bundle', $this->bundle);
			}
			if($this->statusField) {
					$query->propertyCondition($this->statusField, 1);
			}
			$query->count();
			return $query;
	}

	/**
	 * Set the bundle to count
	 *
	 * @param String $bundle The name of the bundle to count, or null to count everything
	 * @return void
	 */
	function setBundle($bundle) {
			$this->bundle = $bundle;
	}

	/**
	 * Return a count of all entities or bundles 
	 *
	 * @return Number
	 */
	function count(){
			return $this->createQuery()->execute();
	}

	/**
	 * Count all entities or bundles that have a datestamp since a given date
	 *
	 * @param string $dateField The namme of the timestamp field
	 * @param integer $since The timestamp to include from
	 * @return integer
	 */
	function countRecent($dateField, $since) {
			return $this->createQuery()->propertyCondition($dateField, $since, '>')->execute();
	}

	/**
	 * Add a timestamp count to be included in the result of `execute`
	 *
	 * @param string $key The key to assign to in the result array
	 * @param string $field The name of the timestamp field to include a count for
	 * @return void
	 */
	function addCount($key, $field) {
			$this->counts[$key] = $field;
	}

	/**
	 * Return an array containing the total count of bundles and any counts
	 * specified with the addCount function
	 *
	 * @param integer $since	Timestamp to count from for since-timestamp queries
	 * @param string $bundle	The bundle name to count for, or null to count all
	 * @return Array
	 */
	function execute($since, $bundle = null){
			$this->setBundle($bundle);
			$self = $this;

			return [
					'total' => $this->count()
			]
			+ array_map( 
					function($field) use ($self, $since) {
							return $self->countRecent($field, $since);
					},
					$this->counts
			);
	}
}