<?php

/**
 * A generic signature method implementation that leverages hash_hmac() for
 * increased flexibility.
 */
class OAuthSignatureMethod_HMAC extends OAuthSignatureMethod {
  private $algo = NULL;

  /**
   * Create a HMAC oauth signature method object using the (or one of the)
   * specified algorithm implementations.
   *
   * @param mixed $algo
   *  Pass the name of a algorithm supported by hash_hmac() or an array of
   *  names when you have several candidate algorithms that may be used. The
   *  first algorithm int the array that is supported on the system will be used.
   * @throws Exception
   *  A exception is thrown when none of the provided algorithms are supported
   *  by the system.
   */
  public function __construct($algo) {
    $algos = hash_algos();
    // Create a single-element array from strings to simplify the logic of
    // support checking and failure handling.
    if (is_string($algo)) {
      $algo = array($algo);
    }

    // Find a supported algorithm among the candidates
    foreach ($algo as $a) {
      if (in_array(strtolower($a), $algos)) {
        $this->algo = strtolower($a);
        continue;
      }
    }
    // Throw a exception if no matching algorithm can be found
    if (empty($this->algo)) {
      throw new OAuthException(t('None of the suggested hash algorithms (@cand) were '
        . 'supported by the server. Try one of the following: !algos.', array(
          '@cand' => join($algo, ', '),
          '!algos' => join($algos, ', '),
      )));
    }
  }

  public function get_name() {
    return "HMAC-" . strtoupper($this->algo);
  }

  public function build_signature($request, $consumer, $token) {
    $base_string = $request->get_signature_base_string();
    $request->base_string = $base_string;

    $key_parts = array(
      $consumer->secret,
      ($token) ? $token->secret : ""
    );

    $key_parts = OAuthUtil::urlencode_rfc3986($key_parts);
    $key = implode('&', $key_parts);

    return base64_encode(hash_hmac($this->algo, $base_string, $key, TRUE));
  }
}
