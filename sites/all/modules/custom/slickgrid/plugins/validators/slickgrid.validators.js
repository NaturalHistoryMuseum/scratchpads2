function requiredInlineValidator(value, $input){
  if(value == null || value == undefined || !value.length) {
    return {valid: false, msg: "This is a required field"};
  } else {
    return {valid: true, msg: null};
  }
}