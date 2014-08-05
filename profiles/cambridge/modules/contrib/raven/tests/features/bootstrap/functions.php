<?php

function is_serialized($value) {
  // Bit of a give away this one
  if (!is_string($value)) {
    return FALSE;
  }

  // Serialized false, return true. unserialize() returns false on an
  // invalid string or it could return false if the string is serialized
  // false, eliminate that possibility.
  if ($value === 'b:0;') {
    $result = FALSE;
    return TRUE;
  }

  $length = strlen($value);
  $end = '';

  if ($length === 0) {
    return FALSE;
  }

  switch ($value[0]) {
    case 's':
      if ($value[$length - 2] !== '"') {
        return FALSE;
      }
    case 'b':
    case 'i':
    case 'd':
      // This looks odd but it is quicker than isset()ing
      $end .= ';';
    case 'a':
    case 'O':
      $end .= '}';

      if ($value[1] !== ':') {
        return FALSE;
      }

      switch ($value[2]) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
          break;

        default:
          return FALSE;
      }
    case 'N':
      $end .= ';';

      if ($value[$length - 1] !== $end[0]) {
        return FALSE;
      }
      break;

    default:
      return FALSE;
  }

  if (($result = @unserialize($value)) === FALSE) {
    $result = NULL;
    return FALSE;
  }
  return TRUE;
}

function maybe_serialize($data) {
  if (FALSE === is_serialized($data)) {
    if ('TRUE' === $data) {
      $data = TRUE;
    }
    elseif ('FALSE' === $data) {
      $data = FALSE;
    }
    elseif ('NULL' === $data) {
      $data = NULL;
    }
    elseif ($data == (string) (int) $data) {
      $data = (int) $data;
    }

    $data = serialize($data);
  }

  return $data;
}

function create_raven_response($url, $status = 200, $principal = 'test0001', $problem = NULL) {
  if (FALSE === in_array($status, array(200, 410, 510, 520, 530, 540, 560, 570, 999))) {
    $status = 200;
  }

  $response = array();
  $response['ver'] = 3;
  $response['status'] = $status;
  $response['msg'] = '';
  $response['issue'] = date('Ymd\THis\Z', $problem === 'expired' ? time() - 36001 : time());
  $response['id'] = '1351247047-25829-18';

  if ('url' === $problem) {
    $response['url'] = 'http://www.example.com/';
  }
  else {
    $response['url'] = $url;
  }

  $response['url'] = str_replace(array('%', '!'), array('%25', '%21'), $response['url']);

  $response['principal'] = $principal;

  $response['ptags'] = '';

  switch ($problem) {
    case 'auth':
      $response['auth'] = 'test';
      $response['sso'] = '';
      break;
    case 'sso':
      $response['auth'] = '';
      $response['sso'] = 'test';
      break;
    default:
      $response['auth'] = 'pwd';
      $response['sso'] = '';
      break;
  }

  $response['life'] = 36000;
  $response['params'] = '';

  if ('kid' === $problem) {
    $response['kid'] = 999;
  }
  else {
    $response['kid'] = 901;
  }

  $data = implode(
    '!',
    array(
      $response['ver'],
      $response['status'],
      $response['msg'],
      $response['issue'],
      $response['id'],
      $response['url'],
      $response['principal'],
      $response['ptags'],
      $response['auth'],
      $response['sso'],
      $response['life'],
      $response['params'],
    )
  );
  $pkeyid = openssl_pkey_get_private(
    '-----BEGIN RSA PRIVATE KEY-----
MIICWwIBAAKBgQC4RYvbSGb42EEEXzsz93Mubo0fdWZ7UJ0HoZXQch5XIR0Zl8AN
aLf3tVpRz4CI2JBUVpUjXEgzOa+wZBbuvczOuiB3BfNDSKKQaftxWKouboJRA5ac
xa3fr2JZc8O5Qc1J6Qq8E8cjuSQWlpxTGa0JEnbKV7/PVUFDuFeEI11e/wIDAQAB
AoGACr2jBUkXF3IjeAnE/aZyxEYVW7wQGSf9vzAf92Jvekyn0ZIS07VC4+FiPlqF
93QIFaJmVwVOAA5guztaStgtU9YX37wRPkFwrtKgjZcqV8ReQeC67bjo5v3Odht9
750F7mKWXctZrm0MD1PoDlkLvVZ2hDolHm5tpfP52jPvQ6ECQQDgtI4K3IuEVOIg
75xUG3Z86DMmwPmme7vsFgf2goWV+p4471Ang9oN7l+l+Jj2VISdz7GE7ZQwW6a1
IQev3+h7AkEA0e9oC+lCcYsMsI9vtXvB8s6Bpl0c1U19HUUWHdJIpluwvxF6SIL3
ug4EJPP+sDT5LvdV5cNy7nmO9uUd+Se2TQJAdxI2UrsbkzwHt7xA8rC60OWadWa8
4+OdaTUjcxUnBJqRTUpDBy1vVwKB3MknBSE0RQvR3canSBjI9iJSmHfmEQJAKJlF
49fOU6ryX0q97bjrPwuUoxmqs81yfrCXoFjEV/evbKPypAc/5SlEv+i3vlfgQKbw
Y6iyl0/GyBRzAXYemQJAVeChw15Lj2/uE7HIDtkqd8POzXjumOxKPfESSHKxRGnP
3EruVQ6+SY9CDA1xGfgDSkoFiGhxeo1lGRkWmz09Yw==
-----END RSA PRIVATE KEY-----'
  );

  openssl_sign($data, $signature, $pkeyid);

  openssl_free_key($pkeyid);

  $signature =
    preg_replace(
      array(
        '#\+#',
        '#/#',
        '#=#',
      ),
      array(
        '-',
        '.',
        '_',
      ),
      base64_encode($signature)
    );

  $response['sig'] = $signature;

  switch ($problem) {
    case 'invalid':
      // need an invalid response, so just need to change a value
      $response['id'] = 12312424;
      break;
    case 'incomplete':
      unset($response['id']);
      break;
  }

  return $url . '?WLS-Response=' . urlencode(implode('!', $response));
}
