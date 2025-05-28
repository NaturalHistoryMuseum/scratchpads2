#!/bin/bash

echo "[$(date)] Running slave check cron job"

WRITER_HOSTGROUP=1
READER_HOSTGROUP=2

check_slave_health() {

local status=$(mysql -u root -p$MYSQL_ROOT_PASSWORD -h $MYSQL_SLAVE_HOST -e "SHOW SLAVE STATUS\G" 2>/dev/null)

local io=$(echo "$status" | grep -w "Slave_IO_Running" | awk '{print $2}')
local sql=$(echo "$status" | grep -w "Slave_SQL_Running" | awk '{print $2}')
local lag=$(echo "$status" | grep -w "Seconds_Behind_Master" | awk '{print $2}')

  if [[ "$io" == "Yes" && "$sql" == "Yes" && "$lag" =~ ^[0-9]+$ && "$lag" -lt 30 ]]; then
    return 0  # Healthy
  else
    return 1  # Unhealthy
  fi

}

# Function to check if readers are online
readers_online=$(mysql -u$PROXYSQL_USER -p$PROXYSQL_PASSWORD -h 127.0.0.1 -P 6032 -N -e "
  SELECT COUNT(*) FROM runtime_mysql_servers
  WHERE hostgroup_id=$READER_HOSTGROUP AND status='ONLINE';
")

slave_is_healthy=0
if [ -z "$readers_online" ]; then
  echo "   ^}^l No readers online."
else
  if check_slave_health; then
    slave_is_healthy=1
  else
    echo "  ^}^l Replication check failed on slave"
  fi
fi

echo "Master"
echo $MYSQL_MASTER_HOST

# Step 3: Take action based on checks
if [ "$slave_is_healthy" -eq 1 ]; then
  echo "   ^|^e Slave healthy. Enabling writes on master."
  mysql -u root -p$MYSQL_ROOT_PASSWORD -h $MYSQL_MASTER_HOST -e "SET GLOBAL read_only = OFF;"
else
  echo "   ^=^z Slave unavailable or unhealthy. Enabling read_only on master."
  mysql -u root -p$MYSQL_ROOT_PASSWORD -h $MYSQL_MASTER_HOST -e "SET GLOBAL read_only = ON;"
fi