vcl 4.0;

import directors;
import std;

backend default {
  .host = "scratchpads.apache";
  .port = "80";
  .max_connections = 100;
  .connect_timeout = 60s;
  .first_byte_timeout = 60s;
  .between_bytes_timeout = 60s;
}

acl trusted_network {
  "157.140.0.0"/16;
  "127.0.0.0"/16;
}

sub vcl_init {
  new dir = directors.round_robin();
  dir.add_backend(default);
 }


sub vcl_recv {

	if (req.restarts == 0) {
	  if (req.http.x-forwarded-for) {
	    set req.http.X-Forwarded-For = req.http.X-Forwarded-For + ", " + client.ip;
	  } else {
	    set req.http.X-Forwarded-For = client.ip;
	  }
	}

	# Set the grace header which is overridden later.
	set req.http.grace = "none";

	# Allow PURGE from the acl purge (defined at the top of this file)
	if (req.method == "PURGE") {
	  if (!client.ip ~ trusted_network) {
	    return (synth(405, "Not allowed."));
	  }
	  return (hash);
	}

	# Get rid of progress.js query params
	if (req.url ~ "^/misc/progress\.js\?[0-9]+$") {
	  set req.url = "/misc/progress.js";
	}

	# We only deal with GET and HEAD
	if (req.method != "GET" && req.method != "HEAD") {
	    return (pass);
	}

	# Handle compression correctly. Different browsers send different
	# "Accept-Encoding" headers, even though they mostly all support the same
	# compression mechanisms. By consolidating these compression headers into
	# a consistent format, we can reduce the size of the cache and get more hits.=
	# @see: http:// varnish.projects.linpro.no/wiki/FAQ/Compression
	if (req.http.Accept-Encoding) {
	  if (req.http.Accept-Encoding ~ "gzip") {
	    # If the browser supports it, we will use gzip.
	    set req.http.Accept-Encoding = "gzip";
	  }
	  else if (req.http.Accept-Encoding ~ "deflate") {
	    # Next, try deflate if it is supported.
	    set req.http.Accept-Encoding = "deflate";
	  }
	  else {
	    # Unknown algorithm. Remove it and send unencoded.
	    unset req.http.Accept-Encoding;
	  }
	}

	# Always cache the following file types for all users.
	if (  req.url ~ "(?i)\.(png|gif|jpeg|jpg|ico|swf|css|js|html|htm)(\?[a-z0-9]+)?$" &&
	  req.url !~ "system" &&
	  req.url !~ "/journals/edit/[0-9]+.js") {
	  unset req.http.Cookie;
	}

  # Do not cache large media files
	if (req.url ~ "(?i)^[^?]*\.(mp3|mp4|rar|rpm|tar|tgz|gz|wav|zip|bz2|xz|7z|avi|mov|ogm|mpe?g|mk[av]|webm)(\?.*)?$")
		unset req.http.Cookie;
		return (hash);
	}

	# Remove all cookies that Drupal/Redmine/Mediawiki does not need to know about.
	# ANY remaining cookie will cause the request to pass-through to a backend.
	# For the most part we always set the NO_CACHE cookie after any POST request,
	# disabling the Varnish cache temporarily. The session cookie allows all authenticated
	# users to pass through as long as they are logged in.
	#
	# 1. Append a semi-colon to the front of the cookie string.
	# 2. Remove all spaces that appear after semi-colons.
	# 3. Match the cookies we want to keep, adding the space we removed
	#    previously, back. (\1) is first matching group in the regsuball.
	# 4. Remove all other cookies, identifying them by the fact that they have
	#    no space after the preceding semi-colon.
	# 5. Remove all spaces and semi-colons from the beginning and end of the
	#    cookie string.
	if (req.http.Cookie) {
	  # Store the cookie, so that we can restore it later.
	  set req.http.X-OriginalCookie = req.http.Cookie;
	  set req.http.Cookie = ";" + req.http.Cookie;
	  set req.http.Cookie = regsuball(req.http.Cookie, "; +", ";");
	  set req.http.Cookie = regsuball(req.http.Cookie, ";(S{1,2}ESS[a-z0-9]+|NO_CACHE|[a-z0-9\-]+_session|_redmine_default|mw_installer_session)=", "; \1=");
	  set req.http.Cookie = regsuball(req.http.Cookie, ";[^ ][^;]*", "");
	  set req.http.Cookie = regsuball(req.http.Cookie, "^[; ]+|[; ]+$", "");
	  if (req.http.Cookie == "") {
	    # If there are no remaining cookies, remove the cookie header. If there
	    # are not any cookie headers, Varnish default behavior will be to cache
	    # the page.
	    unset req.http.Cookie;
	    unset req.http.X-OriginalCookie;
	  } else {
	        # If there is any cookies left (a session or NO_CACHE cookie), do not
	        # cache the page. Pass it on to Apache directly.
	        set req.http.Cookie = req.http.X-OriginalCookie;
	        unset req.http.X-OriginalCookie;
	        return (pass);
	   }

	    ## Unset Authorization header if it has the correct details...
	    if (req.http.Authorization || req.http.Cookie) {
	        /* Not cacheable by default */
	        return (pass);
	    }


	}

}

sub vcl_deliver {
  set resp.http.grace = req.http.grace;
}

sub vcl_hit {
  if (obj.ttl >= 0s) {
    # normal hit
    return (deliver);
  }
  # We have no fresh fish. Lets look at the stale ones.
  if (std.healthy(req.backend_hint)) {
    # Backend is healthy. Limit age to 1h.
    if (obj.ttl + 1h > 0s) {
      set req.http.grace = "normal(limited)";
      return (deliver);
    } else {
      # No candidate for grace. Fetch a fresh object.
      return(miss);
   }
  } else {
    # backend is sick - use full grace
    if (obj.ttl + obj.grace > 0s) {
      set req.http.grace = "full";
      return (deliver);
    } else {
     # no graced object.
       return (miss);
   }
  }
}

sub vcl_backend_response {
  # Do not allow static files to set cookies.
  if (bereq.url ~ "(?i)\.(png|gif|jpeg|jpg|ico|swf|css|js|html|htm)(\?[a-z0-9]+)?$") {
    unset beresp.http.set-cookie;
  }
	# Stream large media files
  if (bereq.url ~ "(?i)^[^?]*\.(mp[34]|rar|rpm|tar|tgz|gz|wav|zip|bz2|xz|7z|avi|mov|ogm|mpe?g|mk[av]|webm)(\?.*)?$") {
			unset beresp.http.set-cookie;
		set beresp.do_stream = true;
	}
  # Allow items to be stale if needed.
  set beresp.ttl = 1h;
  set beresp.grace = 6h;
}

sub vcl_synth {
  # Redirect and 750 errors
  if(resp.status == 750){
    set resp.http.Location = resp.reason;
    set resp.status = 302;
    return (deliver);
  }
  # Too many connections error
  if(resp.status == 429){
    synthetic({"<html>
<head>
<title>429: Too many requests</title>
<meta http-equiv="refresh" content="10">
</head>
<body>
<div class="text">
<h1>429: Too many requests</h1>
<p>Please limit the rate at which you request resources from our server. All clients are limited to <strong>3</strong> requests every <strong>10 seconds for uncac$
<p>If you continue to experience this problem and do not believe you are bombarding our servers, then please contact the <a href="mailto:scratchpads@nhm.ac.uk?sub$
</div>
</body>
</html>"});
    return (deliver);
  }

}

# In the event of an error, show friendlier messages.
sub vcl_backend_error {
  # Friendly error!
  set beresp.http.Content-Type = "text/html; charset=utf-8";
  set beresp.http.Retry-After = "15";
  synthetic({"<html>
<head>
<title>Technical difficulties</title>
<meta http-equiv="refresh" content="15">
</head>
<body>
<div class="text">
<h1>Technical difficulties</h1>
<p>Site is experiencing technical difficulties.  Please try again later.</p>
</div>
</body>
</html>"});
  return (deliver);
}
