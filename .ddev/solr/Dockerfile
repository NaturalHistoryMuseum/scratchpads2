# Solr 4.10.4 is distributed as Java bytecode, but the historical image also
# bundles an amd64-only JVM. Copy the same Solr release onto a native,
# multi-architecture Java 8 runtime so it runs reliably on Apple Silicon.
FROM geerlingguy/solr:4.10.4 AS legacy_solr

FROM eclipse-temurin:8-jre-jammy

RUN apt-get update \
  && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    curl \
    lsof \
    procps \
    unzip \
  && rm -rf /var/lib/apt/lists/*

COPY --from=legacy_solr /opt/solr /opt/solr

RUN mkdir -p /var/solr \
  && chmod 0777 /var/solr

EXPOSE 8983

CMD ["/opt/solr/bin/solr", "start", "-p", "8983", "-f", "-s", "/var/solr"]
