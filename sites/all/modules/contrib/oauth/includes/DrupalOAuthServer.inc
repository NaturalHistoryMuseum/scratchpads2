<?php

class DrupalOAuthServer extends OAuthServer {
  public function __construct($context) {
    parent::__construct(new DrupalOAuthDataStore($context));

    if (isset($this->context->authorization_options['signature_methods'])) {
      $sig_methods = $this->context->authorization_options['signature_methods'];
    }
    else {
      $sig_methods = array('HMAC-SHA1', 'HMAC-SHA256', 'HMAC-SHA384', 'HMAC-SHA512');
    }

    foreach ($sig_methods as $signature_method) {
      if ($signature_method == 'PLAINTEXT') {
        $this->add_signature_method(new OAuthSignatureMethod_PLAINTEXT());
      }
      else {
        // Check if the system supports the hashing algorithm
        $algo = explode('-', $signature_method, 2);
        if ($algo[0] == 'HMAC' && in_array(strtolower($algo[1]), hash_algos())) {
          $this->add_signature_method(new OAuthSignatureMethod_HMAC($algo[1]));
        }
      }
    }
  }
}
