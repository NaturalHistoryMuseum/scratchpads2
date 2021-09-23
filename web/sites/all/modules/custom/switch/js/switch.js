/**
 * @file switch javascript file
 */
(function ($) {
  Drupal.behaviors.Switch = {
    attach: function (context, settings) {

      $('.form-type-switch', context).each(function(){
    		  var container;
      		var image;
      		var input = $('input', $(this)).hide();      		
      		var state;
   		
      		if(input.is(':checked')){
      		  state = 'on';
      		}else{
      		  state = 'off';
      		}

      		// make the container
      		container = $('<div class="iphone_switch_container" style="height:'+Drupal.settings.switch.switch_height+'px; width:'+Drupal.settings.switch.switch_width+'px; position: relative; overflow: hidden"></div>');
          img = $('<img class="iphone_switch" style="height:'+Drupal.settings.switch.switch_height+'px; width:'+Drupal.settings.switch.switch_width+'px; background-image:url('+Drupal.settings.switch.switch_path+'); background-repeat:none; background-position:'+(state == 'on' ? 0 : -53)+'px" src="'+(state == 'on' ? Drupal.settings.switch.switch_on_container_path : Drupal.settings.switch.switch_off_container_path)+'" /></div>').appendTo(container);

          img.mouseover(function(){
      			img.css("cursor", Drupal.settings.switch.mouse_over);
      		});

      		img.mouseout(function(){
      			img.css("background", Drupal.settings.switch.mouse_out);
      		});
      		
      		// click handling
      		container.mousedown(function() {
      		  img = $('img', $(this));
      		  // Prevent dragging the image.
            img.bind('dragstart', function(event) { event.preventDefault(); });
      			if(state == 'on') {
      				img.animate({backgroundPosition: -53}, "slow", function() {
      					img.attr('src', Drupal.settings.switch.switch_off_container_path);      					
      				});
      				state = 'off';
      				input.attr('checked', false);
      			}
      			else {
      				img.animate({backgroundPosition: 0}, "slow");      				
      				input.attr('checked', true);      				
      				img.attr('src', Drupal.settings.switch.switch_on_container_path);
      				state = 'on';
      			}
      		});
      		$(this).append(container);
       
      });

    }
  }
})(jQuery);
