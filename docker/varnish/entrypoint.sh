#!/bin/sh

# Start varnish and log
varnishd -f /etc/varnish/default.vcl -s malloc,${VARNISH_MEMORY} -a 0.0.0.0:${VARNISH_PORT}
varnishlog