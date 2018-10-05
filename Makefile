include .env
export

CURRENT_DATE := $(shell date +%Y-%m-%d)

up:
	@echo "Starting containers"
	@docker-compose up

down: stop

stop:
	@echo "Stopping containers"
	@docker-compose stop

sync-up:
	@echo "Starting containers and syncing files"
	@docker-sync-stack start

sync-down: sync-stop

sync-clean:
	@echo "Stopping containers and removing volumes"
	@docker-sync-stack clean

drush:
	@docker exec -i -t scratchpads.apache drush $(filter-out $@,$(MAKECMDGOALS))


bash:
	@docker exec -i -t scratchpads.apache bash

build:
	@docker build -t naturalhistorymuseum/scratchpad-apache -t naturalhistorymuseum/scratchpad-apache:$(CURRENT_DATE) docker/apache
	@docker build -t naturalhistorymuseum/scratchpad-solr -t naturalhistorymuseum/scratchpad-solr:$(CURRENT_DATE) docker/solr
	@docker build -t naturalhistorymuseum/scratchpad-mysql -t naturalhistorymuseum/scratchpad-mysql:$(CURRENT_DATE) docker/mysql
	@docker build -t naturalhistorymuseum/scratchpad-varnish -t naturalhistorymuseum/scratchpad-varnish:$(CURRENT_DATE) docker/varnish

# https://stackoverflow.com/a/6273809/1826109
%:
	@: