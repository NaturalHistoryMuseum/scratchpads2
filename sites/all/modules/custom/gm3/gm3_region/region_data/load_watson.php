<?php

$f = fopen('watson_vice_counties', 'r');
while($line = fgets($f)){
  switch(strtolower(substr($line, 0, 9))){
    case '  vcname ':
      echo $line;
      break;
    case '  multipo':
    case '  polygon':
      echo $line;
      exit;
  }
}
