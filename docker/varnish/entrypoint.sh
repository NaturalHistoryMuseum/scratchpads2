#!/bin/sh

init_varnish_secret() {
    echo -e "${VARNISH_SECRET}" > /etc/varnish/secret
}

init_varnish_secret

# Start varnish and log
varnishd -f /etc/varnish/default.vcl -s malloc,${VARNISH_MEMORY} -a 0.0.0.0:${VARNISH_PORT} -T 0.0.0.0:6082 -S /etc/varnish/secret
varnishlog