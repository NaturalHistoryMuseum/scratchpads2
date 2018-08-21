#!/usr/bin/env bash

VERSION=$(curl -s https://api.github.com/repos/NaturalHistoryMuseum/scratchpads2/tags | grep -o -m1 '2\(\.[0-9]\+\)\+')
DATE=$(date +%Y-%m-%d)
docker build -t naturalhistorymuseum/scratchpad -t naturalhistorymuseum/scratchpad:$VERSION  -t naturalhistorymuseum/scratchpad:$VERSION-$DATE --build-arg VERSION=$VERSION ./apache
docker build -t naturalhistorymuseum/scratchpad-solr -t naturalhistorymuseum/scratchpad-solr:$DATE ./solr
docker build -t naturalhistorymuseum/scratchpad-mysql -t naturalhistorymuseum/scratchpad-mysql:$DATE ./mysql