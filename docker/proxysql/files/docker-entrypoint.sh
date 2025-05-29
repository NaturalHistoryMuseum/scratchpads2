#!/bin/bash
set -e

# Render the config using envsubst
envsubst < /tmp/proxysql.cnf.template > /etc/proxysql.cnf

envsubst < /tmp/env.sh.template > /env.sh

# Launch ProxySQL with the rendered config in the background
/usr/bin/proxysql --initial -f -c /etc/proxysql.cnf &
PROXYSQL_PID=$!

# Wait for ProxySQL to start listening on the admin port (adjust timeout if needed)
TIMEOUT=10
START_TIME=$(date +%s)
until nc -z 127.0.0.1 6032; do
  CURRENT_TIME=$(date +%s)
  ELAPSED=$((CURRENT_TIME - START_TIME))
  if [ "$ELAPSED" -gt "$TIMEOUT" ]; then
    echo "Error: Timeout waiting for ProxySQL admin interface."
    kill -9 "$PROXYSQL_PID"
    exit 1
  fi
  echo "Waiting for ProxySQL admin interface..."
  sleep 2
done

echo "ProxySQL admin interface is up. Loading runtime configuration..."

# Bug fix - explicitly set username and password
mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "SET mysql-monitor_username = '${MYSQL_USER}';"
mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "SET mysql-monitor_password = '${MYSQL_PASSWORD}';"
mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "LOAD MYSQL VARIABLES TO RUNTIME;"
mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "SAVE MYSQL VARIABLES TO DISK;"

echo "Runtime configuration loaded."

echo "🔧 Starting cron..."
service cron start

echo "📄 Active cron jobs:"
crontab -l

touch /var/log/proxysql_failover.log
tail -F /var/log/proxysql_failover.log &

# Keep ProxySQL running in the foreground (important for Docker)
wait "$PROXYSQL_PID"