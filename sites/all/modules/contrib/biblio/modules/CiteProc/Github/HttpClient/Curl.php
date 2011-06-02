<?php

/**
 * Performs requests on GitHub API. API documentation should be self-explanatory.
 *
 * @author    Thibault Duplessis <thibault.duplessis at gmail dot com>
 * @license   MIT License
 */
class Github_HttpClient_Curl extends Github_HttpClient
{
    /**
     * Send a request to the server, receive a response
     *
     * @param  string   $path          Request url
     * @param  array    $parameters    Parameters
     * @param  string   $httpMethod    HTTP method to use
     * @param  array    $options       Request options
     *
     * @return string   HTTP response
     */
    public function doRequest($url, array $parameters = array(), $httpMethod = 'GET', array $options)
    {
        $curlOptions = array();

        if ($options['login']) {
            switch ($options['auth_method']) {
                case Github_Client::AUTH_HTTP_PASSWORD:
                    $curlOptions += array(
                        CURLOPT_USERPWD => $options['login'].':'.$options['secret'],
                    );
                    break;
                case Github_Client::AUTH_HTTP_TOKEN:
                    $curlOptions += array(
                        CURLOPT_USERPWD => $options['login'].'/token:'.$options['secret'],
                    );
                    break;
                case Github_Client::AUTH_URL_TOKEN:
                default:
                    $parameters = array_merge(array(
                        'login' => $options['login'],
                        'token' => $options['secret']
                            ), $parameters);
                    break;
            }
        }

        if (!empty($parameters)) {
            $queryString = utf8_encode(http_build_query($parameters, '', '&'));

            if ('GET' === $httpMethod) {
                $url .= '?'.$queryString;
            } else {
                $curlOptions += array(
                    CURLOPT_POST => true,
                    CURLOPT_POSTFIELDS => $queryString
                );
            }
        }

        $curlOptions += array(
            CURLOPT_URL => $url,
            CURLOPT_PORT => $options['http_port'],
            CURLOPT_USERAGENT => $options['user_agent'],
            CURLOPT_FOLLOWLOCATION => true,
            CURLOPT_RETURNTRANSFER => true,
            CURLOPT_TIMEOUT => $options['timeout']
        );

        $response = $this->doCurlCall($curlOptions);

        if (!in_array($response['headers']['http_code'], array(0, 200, 201))) {
            throw new Github_HttpClient_Exception(null, (int) $response['headers']['http_code']);
        }

        if ($response['errorNumber'] != '') {
            throw new Github_HttpClient_Exception('error '.$response['errorNumber']);
        }

        return $response['response'];
    }

    protected function doCurlCall(array $curlOptions){
      //follow on location problems
      $safe_mode = ini_get('safe_mode');
      $open_basedir = ini_get('open_basedir');
        $curl = curl_init();
      if (empty($open_basedir) && empty($safe_mode)) {
        return $this->doNormalCurlCall($curl, $curlOptions);
      }
      else{
        unset($curlOptions[CURLOPT_FOLLOWLOCATION]);
        $curlOptions[CURLOPT_HEADER] = TRUE;
        return $this->doSafeModeCurlCall($curl, $curlOptions);
      }
      curl_close($go);
      return $syn;
    }
    protected function doNormalCurlCall($curl, array $curlOptions)
    {
        curl_setopt_array($curl, $curlOptions);

        $response = curl_exec($curl);
        $headers = curl_getinfo($curl);
        $errorNumber = curl_errno($curl);
        $errorMessage = curl_error($curl);

        curl_close($curl);

        return compact('response', 'headers', 'errorNumber', 'errorMessage');
    }
    //follow on location problems workaround
    protected function doSafeModeCurlCall($curl, array $curlOptions)
    {
      static $curl_loops = 0;

      if ($curl_loops++ > 20)
      {
        $curl_loops = 0;
        return FALSE;
      }
      curl_setopt_array($curl, $curlOptions);
      $response = curl_exec($curl);
      list($header, $response) = explode("\n\r\n", $response, 2);
      $response = trim($response);
      $headers = curl_getinfo($curl);
      $errorNumber = curl_errno($curl);
      $errorMessage = curl_error($curl);


      $http_code = curl_getinfo($curl, CURLINFO_HTTP_CODE);
      if ($http_code == 301 || $http_code == 302)
      {
        $matches = array();
        preg_match('/Location:(.*?)\n/', $header, $matches);
        $url = @parse_url(trim(array_pop($matches)));
        if (!$url)
        {
          //couldn't process the url to redirect to
          $curl_loops = 0;
          curl_close($curl);
          return compact('response', 'headers', 'errorNumber', 'errorMessage');
        }
        $last_url = parse_url(curl_getinfo($curl, CURLINFO_EFFECTIVE_URL));
        if (!$url['scheme']) {
          $url['scheme'] = $last_url['scheme'];
        }
        if (!$url['host']) {
          $url['host'] = $last_url['host'];
        }
        if (!$url['path']) {
          $url['path'] = $last_url['path'];
        }
        $new_url = $url['scheme'] . '://' . $url['host'] . $url['path'] . ($url['query']?'?'.$url['query']:'');
        $curlOptions['CURLOPT_URL'] = $new_url;
        return $this->doSafeModeCurlCall($curl, $curlOptions);
      }
      else {
        $curl_loops=0;
        curl_close($curl);
        return compact('response', 'headers', 'errorNumber', 'errorMessage');
      }
    }
}
