FROM alpine:3.7

RUN apk update && \
    apk upgrade && \
    apk add varnish


COPY files/default.vcl /etc/varnish/default.vcl

COPY files/entrypoint.sh /entrypoint.sh

RUN ["chmod", "+x", "/entrypoint.sh"]

USER root

CMD ["/entrypoint.sh"]