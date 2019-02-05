FROM java:7-jre

ARG SOLR_UNDERTOW_DIRECTORY='/usr/local/share/solr-undertow'
ARG SOLR_DATA_DIRECTORY='/var/lib/solr-undertow'
ARG SOLR_UNDERTOW_VERSION='1.6.0'
ARG SOLR_VERSION='4.10.4'
ARG SOLR_UNDERTOW_FILE_NAME="solr-undertow-${SOLR_UNDERTOW_VERSION}-with-solr-${SOLR_VERSION}"

RUN mkdir -p $SOLR_UNDERTOW_DIRECTORY
RUN mkdir -p $SOLR_DATA_DIRECTORY
RUN mkdir -p $SOLR_DATA_DIRECTORY/solr-home

ADD "https://github.com/kohesive/solr-undertow/releases/download/v${SOLR_UNDERTOW_VERSION}-release/solr-undertow-${SOLR_UNDERTOW_VERSION}-with-solr-${SOLR_VERSION}.tgz" /tmp/
RUN tar -xzf "/tmp/${SOLR_UNDERTOW_FILE_NAME}.tgz" -C $SOLR_UNDERTOW_DIRECTORY --strip-components=1
RUN ln -s $SOLR_UNDERTOW_DIRECTORY/example/solr-wars $SOLR_DATA_DIRECTORY/solr-wars

COPY files/solr-undertow.conf $SOLR_DATA_DIRECTORY/solr-undertow.conf
COPY files/solr.xml $SOLR_DATA_DIRECTORY/solr-home/solr.xml
COPY files/scratchpads2 $SOLR_DATA_DIRECTORY/solr-home/scratchpads2
# RUN mkdir $SOLR_DATA_DIRECTORY/solr-home/scratchpads2/data

ENV SOLR_UNDERTOW_OPTS="-Xms${SOLR_JVM_MEMORY} -Xmx${SOLR_JVM_MEMORY} -XX:MaxPermSize=512m -XX:PermSize=256m"
WORKDIR "$SOLR_DATA_DIRECTORY"
CMD ["/usr/local/share/solr-undertow/bin/solr-undertow", "/var/lib/solr-undertow/solr-undertow.conf"]