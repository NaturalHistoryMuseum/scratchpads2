<?php

class RpcRouter {
	function test($value){
		return ['You said', $value];
	}

	function getSiteSettings(){
		$colour = variable_get('scratchpads_colour', SCRATCHPADS_DEFAULT_COLOUR);
		if($colour == 'custom'){
			$colour = variable_get('scratchpads_custom_colour');
		}

		return [
			'name' => variable_get('site_name', 'Scratchpad'),
			'theme_color' => '#' . $colour
		];
	}
}
