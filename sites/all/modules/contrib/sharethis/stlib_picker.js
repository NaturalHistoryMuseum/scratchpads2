//This library requires JQuery
//It also requires stcommon.js in your header for an official list of services
//stlib_picker.defaultServices defines the services from stcommon that get loaded as the default services in the picker
//Styling can be found in stlib_picker.css and should be linked in the page.
//To get selected services as an array of strings:  (ie ["twitter", "sharethis", "facebook"] )
//   Call: var answer = stlib_picker.pickerList[uniqueID]["getServices"]();

var stlib_picker = {};
stlib_picker.pickerList = [];
stlib_picker.defaultServices = ["sharethis", "tumblr", "bebo"];
stlib_picker.getServices = function (id) {
	var func = stlib_picker.pickerList[id]["getServices"];
	return func();
}

//Creates the picker - make sure it has a unique ID
stlib_picker.setupPicker = function(jQElement, newDefaults, callback) {
	console.log("setting up picker");
	console.log(jQElement);
	//Make an array to store any needed options
	var optionsArray = [];
	optionsArray["El"] = jQElement;
	optionsArray["isSelect"] = false;
	optionsArray["getServices"] = function() {
		var answer = [];
		var lis = jQElement.children(".stp_pickerLeft").find(".stp_li");
		lis.each(function() {
			answer.push(jQuery(this).attr("id").substring(6));
		});
		return answer;
	};
	
	//Append the three divs that are needed:
	jQElement.append("<div class='stp_pickerLeft'><span class='stp_header'>Selected Service</span><ul class='stp_ulLeft'></ul></div>");
	jQElement.append("<div class='stp_pickerArrow'><div class='stp_arrow'><img class='stp_up' src='http://www.sharethis.com/images/Direction_Icons_Sprite.png'></img></div>" +
												  "<div class='stp_arrow'><img class='stp_left' src='http://www.sharethis.com/images/Direction_Icons_Sprite.png'></img></div>" +
												  "<div class='stp_arrow'><img class='stp_right' src='http://www.sharethis.com/images/Direction_Icons_Sprite.png'></img></div>" +
												  "<div class='stp_arrow'><img class='stp_down' src='http://www.sharethis.com/images/Direction_Icons_Sprite.png'></img></div>" +
					"</div>");
	jQElement.append("<div class='stp_pickerRight'><span class='stp_header'>Possible Services</span><ul class='stp_ulRight'></ul></div>");
	jQElement.append("<div class='stp_clear'></div>");
	
	//Add default Services
	var pickerDefaults = [];
	if (newDefaults) {
		pickerDefaults = newDefaults;
	} else {
		pickerDefaults = stlib_picker.defaultServices;
	}
	
	//Add all the services to the picker:
	jQuery.each(_all_services, function(key, value) {
		if(jQuery.inArray(key, pickerDefaults) == -1) {
			var ul = jQElement.children(".stp_pickerRight").children(".stp_ulRight");
			ul.append("<li id='st_li_" + key + "' class='stp_li'><img src='http://w.sharethis.com/images/"+key+"_32.png'></img><span class='stp_liText'>" + value.title + "</span></li>");
		}
	});
	for(i=0;i<pickerDefaults.length;i++) {
		var ul = jQElement.children(".stp_pickerLeft").children(".stp_ulLeft");
		ul.append("<li id='st_li_" + pickerDefaults[i] + "' class='stp_li'><img src='http://w.sharethis.com/images/"+pickerDefaults[i]+"_32.png'></img><span class='stp_liText'>" + _all_services[pickerDefaults[i]].title + "</span></li>");
	}
	
	//Add the various Event handlers 
	//Need to make sure that we don't get confused when there are multiple pickers
	jQElement.find(".stp_li").click(function() {
		jQElement.find(".stp_select").removeClass("stp_select");
		jQuery(this).addClass("stp_select");
		stlib_picker.pickerList[jQElement.attr("id")]["isSelect"] = true;
	});
	
	var arrowDiv = jQElement.children(".stp_pickerArrow").children(".stp_arrow");
	arrowDiv.children(".stp_up").click(function() {
		if (stlib_picker.pickerList[jQElement.attr("id")]["isSelect"]) {
			var li = jQElement.find(".stp_select");
			var prev = li.prev();
			if (prev.length != 0) {
				prev.before(li);
			}
			if (callback) {
				callback();
			}
		}
	});
	arrowDiv.children(".stp_left").click(function() {
		if (stlib_picker.pickerList[jQElement.attr("id")]["isSelect"]) {
			var li = jQElement.find(".stp_select");
			var ul = jQElement.children(".stp_pickerLeft").children(".stp_ulLeft");
			ul.prepend(li);
			if (callback) {
				callback();
			}
		}
	});
	arrowDiv.children(".stp_right").click(function() {
		if (stlib_picker.pickerList[jQElement.attr("id")]["isSelect"]) {
			var li = jQElement.find(".stp_select");
			var ul = jQElement.children(".stp_pickerRight").children(".stp_ulRight");
			ul.prepend(li);
			if (callback) {
				callback();
			}
		}
	});
	arrowDiv.children(".stp_down").click(function() {
		if (stlib_picker.pickerList[jQElement.attr("id")]["isSelect"]) {
			var li = jQElement.find(".stp_select");
			var next = li.next();
			if (next.length != 0) {
				next.after(li);
			}
			if (callback) {
				callback();
			}
		}
	});
		
	//Save the options (and the picker) globally
	stlib_picker.pickerList[jQElement.attr("id")] = optionsArray;
}