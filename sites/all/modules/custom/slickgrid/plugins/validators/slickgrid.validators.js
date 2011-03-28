/**
 * Basic required validator for slickgrid editor
 */

function requiredFieldValidator(value, $input) {
  
    if (value == null || value == undefined || !value.length) {
      // If beautytips is avaiable, use it to display the error message
      if(typeof jQuery.fn.bt == 'function'){
        $input.bt('This is a required field!',{
      		positions : 'right',
      		fill : 'rgba(0, 0, 0, .7)',
      		strokeWidth : 0,
      		spikeLength : 10,
      		cssStyles : {
      			color : 'white',
      			'font-size' : '10px'
      		},
      		width: 120,
      		closeWhenOthersOpen : true,
      		trigger : 'none'
      	});
        $input.btOn();
        $input.keydown(function() {
            $input.btOff();
        })
      }
      
        return {
            valid: false,
            msg: "This is a required field"
        };
    }

    else {
        return {
            valid: true,
            msg: null
        };
    }
}