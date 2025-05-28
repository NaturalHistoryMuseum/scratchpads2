#!/bin/bash
set -ex
echo "[INFO] Custom entrypoint started"

# Substitute env vars into SQL and move to init directory
envsubst < /tmp/init-slave.sql > /docker-entrypoint-initdb.d/init-slave.sql

exec /usr/local/bin/docker-entrypoint.sh "$@"