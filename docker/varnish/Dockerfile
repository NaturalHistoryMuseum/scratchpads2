# FROM dockerimages/docker-varnish

FROM alpine:3.7

RUN apk update && \
    apk upgrade && \
	apk add varnish


COPY default.vcl /etc/varnish/default.vcl

COPY ./entrypoint.sh /entrypoint.sh

RUN ["chmod", "+x", "/entrypoint.sh"]

USER root

CMD ["/entrypoint.sh"]