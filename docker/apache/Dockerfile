FROM php:8.3.36-apache-bookworm

ARG DRUSH_VERSION=8.4.12
ARG DRUSH_SHA256=62d0fd943eb6d4b24900cfe278be0a5af638a3852aa37f855a08c660675041ac
ARG IMAGICK_VERSION=3.8.1
ARG MCRYPT_VERSION=1.0.9
ARG UPLOADPROGRESS_VERSION=2.0.2

# Install the libraries needed by Drupal and the PHP extensions used by
# Scratchpads. mbstring and OPcache are already built into the official PHP
# image, so they must not be compiled a second time here.
RUN set -eux; \
    apt-get update; \
    apt-get install -y --no-install-recommends \
        cron \
        default-mysql-client \
        git \
        imagemagick \
        libfreetype6-dev \
        libgmp-dev \
        libjpeg62-turbo-dev \
        libmagickwand-dev \
        libmcrypt-dev \
        libpng-dev \
        libxml2-dev \
        libzip-dev \
        msmtp \
        nano \
        unzip \
        zlib1g-dev; \
    docker-php-ext-configure gd --with-freetype --with-jpeg; \
    docker-php-ext-install -j"$(nproc)" \
        exif \
        gd \
        gmp \
        pdo_mysql \
        soap \
        sockets \
        zip; \
    pecl install \
        "imagick-${IMAGICK_VERSION}" \
        "mcrypt-${MCRYPT_VERSION}" \
        "uploadprogress-${UPLOADPROGRESS_VERSION}"; \
    docker-php-ext-enable imagick mcrypt uploadprogress; \
    rm -rf /tmp/pear /var/lib/apt/lists/*

# Enable Drupal's clean URLs.
RUN a2enmod rewrite

# Install the final Drush 8 release used by the PHP 8.3 DDEV environment.
RUN set -eux; \
    curl -fsSL \
        "https://github.com/drush-ops/drush/releases/download/${DRUSH_VERSION}/drush.phar" \
        -o /usr/local/bin/drush; \
    echo "${DRUSH_SHA256}  /usr/local/bin/drush" | sha256sum -c -; \
    chmod +x /usr/local/bin/drush; \
    drush --version

# Configure Drupal, PHP, mail delivery, and Apache.
COPY files/settings.php /var/www/html/sites/default/settings.php
COPY files/php.ini /usr/local/etc/php/conf.d/scratchpads.ini
COPY files/apache2.conf /etc/apache2/apache2.conf
COPY files/ports.conf /etc/apache2/ports.conf
COPY files/000-default.conf /etc/apache2/sites-available/000-default.conf

RUN usermod -u 10001 www-data \
    && groupmod -g 10001 www-data

WORKDIR /var/www/html
