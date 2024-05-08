include .env
export

CURRENT_DATE := $(shell date +%Y-%m-%d)

up:
	@echo "Starting containers"
	@docker compose up

down:
	@echo "Stopping & removing containers"
	@docker compose down

stop:
	@echo "Stopping containers"
	@docker compose stop

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

push:
	@docker push naturalhistorymuseum/scratchpad-apache
	@docker push naturalhistorymuseum/scratchpad-apache:$(CURRENT_DATE)
	@docker push naturalhistorymuseum/scratchpad-solr
	@docker push naturalhistorymuseum/scratchpad-solr:$(CURRENT_DATE)
	@docker push naturalhistorymuseum/scratchpad-mysql
	@docker push naturalhistorymuseum/scratchpad-mysql:$(CURRENT_DATE)		
	@docker push naturalhistorymuseum/scratchpad-varnish
	@docker push naturalhistorymuseum/scratchpad-varnish:$(CURRENT_DATE)

site-from-archive:
	@test -f $(archive) && echo Creating site from archive $(archive) || echo Archive not found
	@tar -xf $(archive) database.sql
	@docker exec -i -t scratchpads.apache drush sql-drop
	@docker exec -i scratchpads.apache drush sql-cli < database.sql
	@echo Database created from archive
	@rm database.sql
	@tar -xf $(archive) files
	@docker cp files scratchpads.apache:/var/www/html/sites/default/
	@docker exec -i scratchpads.apache chown www-data:www-data -R /var/www/html/sites/default/files
	@docker exec -i scratchpads.apache chmod 755 -R /var/www/html/sites/default/files	
	@echo Site files copied across
	@docker exec -i scratchpads.apache drush solr-set-env-url 'http://scratchpads.solr:8983/solr/scratchpads2'
	@docker exec -i scratchpads.apache drush solr-delete-index
	@docker exec -i scratchpads.apache drush solr-mark-all
	@echo Scratchpad created from archive

# https://stackoverflow.com/a/6273809/1826109
%:
	@: