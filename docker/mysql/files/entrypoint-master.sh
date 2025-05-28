#!/bin/bash
set -ex
echo "[INFO] Custom entrypoint started"

# Substitute env vars into SQL and move to init directory
envsubst < /tmp/init-master.sql > /docker-entrypoint-initdb.d/init-master.sql

exec /usr/local/bin/docker-entrypoint.sh "$@"