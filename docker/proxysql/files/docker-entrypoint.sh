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

# Execute LOAD commands using mysql client
mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "SET mysql-monitor_username = '${MYSQL_USER}';"
mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "SET mysql-monitor_password = '${MYSQL_PASSWORD}';"
mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "LOAD MYSQL VARIABLES TO RUNTIME;"
mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "SAVE MYSQL VARIABLES TO DISK;"

# Assign MySQL servers to hostgroups:
#mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "UPDATE mysql_servers SET hostgroup_id = 10 WHERE hostname = 'scratchpads.mysql-master';"
#mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "UPDATE mysql_servers SET hostgroup_id = 20 WHERE hostname = 'scratchpads.mysql-slave';"
#mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "INSERT INTO mysql_replication_hostgroups (writer_hostgroup, reader_hostgroup, comment) VALUES (10, 20, 'master/slave failover');"
#mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "LOAD MYSQL SERVERS TO RUNTIME;"
#mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "SAVE MYSQL SERVERS TO DISK;"

# Add query rules (only for SELECTs)
#mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "INSERT INTO mysql_query_rules (rule_id, active, match_pattern, destination_hostgroup, apply) VALUES (1, 1, '.*', 10, 1);"
#mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "INSERT INTO mysql_query_rules (rule_id, active, match_pattern, destination_hostgroup, apply) VALUES (2, 1, '^SELECT.*', 20, 1);"
#mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "LOAD MYSQL QUERY RULES TO RUNTIME;"
#mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "SAVE MYSQL QUERY RULES TO DISK;"


# Make sure your user connects via hostgroup_id = 10
#mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "UPDATE mysql_users SET default_hostgroup = 10 WHERE username = '${MYSQL_USER}';"
#mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "LOAD MYSQL USERS TO RUNTIME;"
#mysql -h 127.0.0.1 -P 6032 -u admin -p${MYSQL_ROOT_PASSWORD} -e "SAVE MYSQL USERS TO DISK;"




echo "Runtime configuration loaded."

echo "🔧 Starting cron..."
service cron start

echo "📄 Active cron jobs:"
crontab -l

touch /var/log/proxysql_failover.log
tail -F /var/log/proxysql_failover.log &

# Keep ProxySQL running in the foreground (important for Docker)
wait "$PROXYSQL_PID"