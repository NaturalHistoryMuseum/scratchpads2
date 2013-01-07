  ------------------------------------------------------------------------------------
                         ABOUT SWITCH
  ------------------------------------------------------------------------------------

	The switch module provides an iphone-like switch form element & widget. 

  ------------------------------------------------------------------------------------
                         	USAGE
  ------------------------------------------------------------------------------------

	There are two ways to use the widget:
	
	1. Field UI
	A widget is provided for checkbox (boolean) fields. When creating a boolean field, select 'switch' as the widget.
	
	2. Form API
	The module defines a new form element type 'switch' - use as you would a checkbox.
	
	$form['example'] = array(
	  '#type' => 'switch',
	  '#title' => t('Example'),
	); 