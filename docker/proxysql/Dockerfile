FROM proxysql/proxysql:2.5.5

# Install bash, envsubst, mail
RUN apt-get update
RUN apt-get install -y bash gettext-base mailutils
RUN apt-get install -y netcat
RUN apt-get install -y nano

# Copy entrypoint script
COPY files/docker-entrypoint.sh /usr/local/bin/docker-entrypoint.sh
COPY files/proxysql.cnf.template /tmp/proxysql.cnf.template
COPY files/env.sh.template /tmp/env.sh.template
COPY files/proxysql_block_writes_if_no_slave.sh /usr/local/bin/proxysql_block_writes_if_no_slave.sh

RUN chmod +x /usr/local/bin/docker-entrypoint.sh
RUN chmod +x /usr/local/bin/proxysql_block_writes_if_no_slave.sh

# Add crontab entry
RUN echo "* * * * * root . /env.sh && /usr/local/bin/proxysql_block_writes_if_no_slave.sh >> /var/log/proxysql_failover.log 2>&1" > /etc/cron.d/proxysql_check && \
    chmod 0644 /etc/cron.d/proxysql_check && \
    crontab /etc/cron.d/proxysql_check

ENTRYPOINT ["/usr/local/bin/docker-entrypoint.sh"]