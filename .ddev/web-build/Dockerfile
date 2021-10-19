
# You can copy this Dockerfile.example to Dockerfile to add configuration
# or packages or anything else to your webimage
ARG BASE_IMAGE
FROM $BASE_IMAGE

# matched drush with pre-ddev custom docker drush version in docker/apache/Dockerfile
#
# credit: https://github.com/drud/ddev/issues/1999#issue-537120147
ENV DRUSH_VERSION=8.1.7
RUN curl -sSL "https://github.com/drush-ops/drush/releases/download/${DRUSH_VERSION}/drush.phar" -o /usr/local/bin/drush8 && chmod +x /usr/local/bin/drush8
