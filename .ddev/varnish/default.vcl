vcl 4.0;

backend default {
  .host = "web";
  .port = "80";
  .connect_timeout = 10s;
  .first_byte_timeout = 60s;
  .between_bytes_timeout = 60s;
}

sub vcl_recv {
  if (req.method != "GET" && req.method != "HEAD") {
    return (pass);
  }

  if (req.http.Authorization || req.http.Cookie ~ "S{1,2}ESS") {
    return (pass);
  }
}

sub vcl_backend_response {
  # The test site explicitly supplies Drupal's cache headers. Keep a small
  # fallback TTL so this service also behaves predictably during local checks.
  if (beresp.ttl <= 0s) {
    set beresp.ttl = 30m;
  }
}
