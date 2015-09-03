<?php

/*****************************************************
               Grid Reference Utilities
Version 2.1 - Written by Mark Wilton-Jones 1-10/4/2010
Updated 6/5/2010 to allow methods to return text
strings and add support for ITM, UTM and UPS
Updated 9/5/2010 to add dd_format
******************************************************

Please see http://www.howtocreate.co.uk/php/ for details
Please see http://www.howtocreate.co.uk/php/gridref.php for demos and instructions
Please see http://www.howtocreate.co.uk/php/gridrefapi.php for API documentation
Please see http://www.howtocreate.co.uk/jslibs/termsOfUse.html for terms and conditions of use

Provides functions to convert between different NGR formats and latitude/longitude formats.

_______________________________________________________________________________________________*/

//use a class to keep all of the method/property clutter out of the global scope
class Grid_Ref_Utils {

	//class instantiation forcing use as a singleton - state can also be stored if needed (or can use separate instances)
	//could be done as an abstract class with static methods, but would still need instantiation to prepare UK_grid_numbers
	//(as methods/properties do not exist during initial class constructor parsing), and would restrict future flexibility
	private static $only_instance;
	public static function toolbox() {
		if( !isset( self::$only_instance ) ) {
			self::$only_instance = new Grid_Ref_Utils();
		}
		return self::$only_instance;
	}
	private function __construct() {
		$this->UK_grid_numbers = $this->grid_to_hashmap($this->UK_grid_squares);
		$this->TEXT_EUROPEAN = html_entity_decode( '&deg;', ENT_QUOTES, 'ISO-8859-1' );
		$this->TEXT_UNICODE = html_entity_decode( '&deg;', ENT_QUOTES, 'UTF-8' );
		$this->TEXT_ASIAN = html_entity_decode( '&deg;', ENT_QUOTES, 'Shift_JIS' );
	}

	//character grids used by map systems
	private function grid_to_hashmap($grid_2d_array) {
		//make a hashmap of arrays giving the x,y values for grid letters
		$hashmap = Array();
		foreach( $grid_2d_array as $grid_row_index => $grid_row_array ) {
			foreach( $grid_row_array as $grid_col_index => $grid_letter ) {
				$hashmap[$grid_letter] = Array($grid_col_index,$grid_row_index);
			}
		}
		return $hashmap;
	}
	private $UK_grid_squares = Array(
		//the order of grid square letters in the UK NGR system - note that in the array they start from the bottom, like grid references
		//there is no I in team
		Array('V','W','X','Y','Z'),
		Array('Q','R','S','T','U'),
		Array('L','M','N','O','P'),
		Array('F','G','H','J','K'),
		Array('A','B','C','D','E')
	);
	private $UK_grid_numbers; //will be set up when the object is constructed

	//define return types
	public $DATA_ARRAY = false;
	public $HTML = true;
	//will be set up when the object is constructed
	public $TEXT_EUROPEAN;
	public $TEXT_UNICODE;
	public $TEXT_ASIAN;

	//conversions between 12345,67890 and SS234789 grid reference formats
	public function get_UK_grid_ref($orig_x,$orig_y = false,$figures = false,$return_type = false,$deny_bad_reference = false,$is_irish = false) {
		if( is_array($orig_x) ) {
			//passed an array, so extract the grid reference from it
			//no need to shuffle the $is_irish parameter over, since it will always be at the end
			$deny_bad_reference = $return_type;
			$return_type = $figures;
			$figures = $orig_y;
			$orig_y = $orig_x[1];
			$orig_x = $orig_x[0];
		}
		if( is_bool($figures) || $figures === null ) {
			$figures = 4;
		} else {
			//enforce integer 1-25
			$figures = max( min( floor( $figures ), 25 ), 1 );
		}
		//prepare factors used for enforcing a number of grid ref figures
		$insig_figures = $figures - 5;
		//round off unwanted detail
		$x = round( $orig_x, $insig_figures );
		$y = round( $orig_y, $insig_figures );
		$errorletters = 'OUT_OF_GRID';
		if( $is_irish ) {
			//the Irish grid system uses the same letter layout as the UK system, but it only has one letter, with origin at V
			$arY = floor( $y / 100000 );
			$arX = floor( $x / 100000 );
			if( $arX < 0 || $arX > 4 || $arY < 0 || $arY > 4 ) {
				//out of grid
				if( $deny_bad_reference ) {
					return false;
				}
				$letters = $errorletters;
			} else {
				$letters = $this->UK_grid_squares[ $arY ][ $arX ];
			}
		} else {
			//origin is at SV, not VV - offset it to VV instead
			$x += 1000000;
			$y += 500000;
			$ar1Y = floor( $y / 500000 );
			$ar1X = floor( $x / 500000 );
			$ar2Y = floor( ( $y % 500000 ) / 100000 );
			$ar2X = floor( ( $x % 500000 ) / 100000 );
			if( $ar1X < 0 || $ar1X > 4 || $ar1Y < 0 || $ar1Y > 4 ) {
				//out of grid - don't need to test $ar2Y and $ar2X since they cannot be more than 4, and ar1_ will also be less than 0 if ar2_ is
				if( $deny_bad_reference ) {
					return false;
				}
				$letters = $errorletters;
			} else {
				//first grid letter is for the 500km x 500km squares
				$letters = $this->UK_grid_squares[ $ar1Y ][ $ar1X ];
				//second grid letter is for the 100km x 100km squares
				$letters .= $this->UK_grid_squares[ $ar2Y ][ $ar2X ];
			}
		}
		//% casts to integer, so need to use floor instead
		//$x %= 100000;
		//$y %= 100000;
		//floating point errors can reappear here if using numbers after the decimal point
		//this approach also makes negative become positive remainder, but that is wanted anyway
		$x -= floor( $x / 100000 ) * 100000;
		$y -= floor( $y / 100000 ) * 100000;
		//avoid stupid -0 by adding 0
		if( $figures <= 5 ) {
			$figure_factor = pow( 10, 5 - $figures );
			$x = str_pad( ( ( $x % 100000 ) / $figure_factor ) + 0, $figures, '0', STR_PAD_LEFT );
			$y = str_pad( ( ( $y % 100000 ) / $figure_factor ) + 0, $figures, '0', STR_PAD_LEFT );
		} else {
			//pad only the left side of the decimal point
			//use sprintf to remove floating point errors introduced by -= and avoid stupid -0
			$x = explode( '.', sprintf( '%.'.$insig_figures.'F', $x ) );
			$x[0] = str_pad( $x[0], 5, '0', STR_PAD_LEFT );
			$x = implode( '.', $x );
			$y = explode( '.', sprintf( '%.'.$insig_figures.'F', $y ) );
			$y[0] = str_pad( $y[0], 5, '0', STR_PAD_LEFT );
			$y = implode( '.', $y );
		}
		if( $return_type && is_string($return_type) ) {
			return $letters . ' ' . $x . ' ' . $y;
		} elseif( $return_type ) {
			return '<var class="grid">' . $letters . '</var><var>' . $x . '</var><var>' . $y . '</var>';
		} else {
			return Array( $letters, $x, $y );
		}
	}
	public function get_UK_grid_nums($letters,$x = false,$y = false,$return_type = false,$deny_bad_reference = false,$is_irish = false) {
		if( is_array($letters) ) {
			//passed an array, so extract only the grid reference from it
			//no need to shuffle the $is_irish parameter over, since it will always be at the end
			$deny_bad_reference = $y;
			$return_type = $x;
			$x = $letters[1];
			$y = $letters[2];
			$letters = str_split(strtoupper($letters[0]));
			array_unshift($letters,'');
		} elseif( !is_string( $x ) || !is_string( $y ) ) {
			//a single string 'X[Y]12345678' or 'X[Y] 1234 5678', split into parts
			//captured whitespace hack makes sure it fails reliably (UK/Irish grid refs must not match each other), while giving consistent matches length
			$deny_bad_reference = $y;
			if( !preg_match( "/^\s*([A-HJ-Z])".($is_irish?"(\s*)":"([A-HJ-Z])\s*")."([\d\.]+)\s*([\d\.]*)\s*$/", strtoupper($letters), $letters ) || ( !$letters[4] && strlen($letters[3]) < 2 ) || ( !$letters[4] && strpos( $letters[3], '.' ) !== false ) ) {
				//invalid format
				if( $deny_bad_reference ) {
					return false;
				}
				//assume 0,0
				$letters = Array( '', $is_irish ? 'V' : 'S', 'V', '0', '0' );
			}
			if( !$letters[4] ) {
				//a single string 'X[Y]12345678', break the numbers in half
				$halflen = ceil( strlen($letters[3]) / 2 );
				$letters[4] = substr( $letters[3], $halflen );
				$letters[3] = substr( $letters[3], 0, $halflen );
			}
			$return_type = $x;
			$x = $letters[3];
			$y = $letters[4];
		} else {
			$letters = str_split(strtoupper($letters));
			array_unshift($letters,'');
		}
		if( !is_numeric($x) ) {
			if( $deny_bad_reference ) {
				return false;
			}
			$x = '0';
		}
		if( !is_numeric($y) ) {
			if( $deny_bad_reference ) {
				return false;
			}
			$y = '0';
		}
		//need 1m x 1m squares
		if( strpos( $x, '.' ) === false && strpos( $y, '.' ) === false ) {
			$x = str_pad( $x, 5, '0' );
			$y = str_pad( $y, 5, '0' );
		}
		if( $is_irish ) {
			if( isset( $this->UK_grid_numbers[$letters[1]] ) ) {
				$x += $this->UK_grid_numbers[$letters[1]][0] * 100000;
				$y += $this->UK_grid_numbers[$letters[1]][1] * 100000;
			} elseif( $deny_bad_reference ) {
				return false;
			} else {
				$x = $y = 0;
			}
		} else {
			if( isset( $this->UK_grid_numbers[$letters[1]] ) && isset( $this->UK_grid_numbers[$letters[2]] ) ) {
				//remove offset from VV to put origin back at SV
				$x += ( $this->UK_grid_numbers[$letters[1]][0] * 500000 ) + ( $this->UK_grid_numbers[$letters[2]][0] * 100000 ) - 1000000;
				$y += ( $this->UK_grid_numbers[$letters[1]][1] * 500000 ) + ( $this->UK_grid_numbers[$letters[2]][1] * 100000 ) - 500000;
			} elseif( $deny_bad_reference ) {
				return false;
			} else {
				$x = $y = 0;
			}
		}
		if( $return_type ) {
			//avoid stupid -0 by adding 0
			return (round($x)+0).','.(round($y)+0);
		} else {
			return Array($x+0,$y+0);
		}
	}
	public function get_Irish_grid_ref($x,$y = false,$figures = false,$return_type = false,$deny_bad_reference = false) {
		return $this->get_UK_grid_ref($x,$y,$figures,$return_type,$deny_bad_reference,true);
	}
	public function get_Irish_grid_nums($letters,$x = false,$y = false,$return_type = false,$deny_bad_reference = false) {
		return $this->get_UK_grid_nums($letters,$x,$y,$return_type,$deny_bad_reference,true);
	}
	public function add_grid_units($x,$y = false,$return_type = false) {
		if( is_array($x) ) {
			//passed an array, so extract the numbers from it
			$return_type = $y;
			$y = $x[1];
			$x = $x[0];
		}
		//avoid stupid -0 by adding 0
		$y = round($y) + 0;
		$x = round($x) + 0;
		if( $return_type ) {
			return ( ( $x < 0 ) ? 'W ' : 'E ' ) . abs($x) . 'm, ' . ( ( $y < 0 ) ? 'S ' : 'N ' ) . abs($y) . 'm';
		} else {
			return Array( abs($x), ( $x < 0 ) ? -1 : 1, abs($y), ( $y < 0 ) ? -1 : 1 );
		}
	}
	public function parse_grid_nums($coords,$return_type = false,$deny_bad_coords = false,$strict_nums = false) {
		if( is_array($coords) ) {
			//passed an array, so extract the numbers from it
			$matches = Array('','',round($coords[0]*$coords[1]),'',round($coords[2]*$coords[3]));
		} else {
			//look for two integers either side of a comma (extra captures ensure array indexes remain the same as $flexi's)
			$rigid = "/^(\s*)(-?\d+)\s*,(\s*)(-?\d+)\s*$/";
			//look for two integers either side of a comma or space, with optional leading E/W or N/S, and trailing m
			//[EW][-]<float>[m][, ][NS][-]<float>[m]
			$flexi = "/^\s*([EW]?)\s*(-?[\d\.]+)(?:\s*M)?(?:\s+|\s*,\s*)([NS]?)\s*(-?[\d\.]+)(?:\s*M)?\s*$/";
			if( !preg_match( $strict_nums ? $rigid : $flexi, strtoupper($coords), $matches ) ) {
				//invalid format
				if( $deny_bad_coords ) {
					return false;
				}
				//assume 0,0
				$matches = Array( '', '', '0', '', '0' );
			}
			$matches[2] *= ( $matches[1] == 'W' ) ? -1 : 1;
			$matches[4] *= ( $matches[3] == 'S' ) ? -1 : 1;
		}
		//avoid stupid -0 by adding 0
		if( $return_type ) {
			return (round($matches[2])+0).','.(round($matches[4])+0);
		} else {
			return Array($matches[2]+0,$matches[4]+0);
		}
	}

	//ellipsoid parameters used during grid->lat/long conversions and Helmert transformations
	private $ellipsoid_Airy_1830 = Array(
		//Airy 1830 (OS)
		'a' => 6377563.396,
		'b' => 6356256.910
	);
	private $ellipsoid_Airy_1830_mod = Array(
		//Airy 1830 modified (OSI)
		'a' => 6377340.189,
		'b' => 6356034.447
	);
	private $ellipsoid_WGS84 = Array(
		//WGS84 (GPS)
		'a' => 6378137,
		'b' => 6356752.314140356
	);
	private $datumset_OSGB36 = Array(
		//Airy 1830 (OS)
		'a' => 6377563.396,
		'b' => 6356256.910,
		'F0' => 0.9996012717,
		'E0' => 400000,
		'N0' => -100000,
		'Phi0' => 49,
		'Lambda0' => -2
	);
	private $datumset_Ireland_1965 = Array(
		//Airy 1830 modified (OSI)
		'a' => 6377340.189,
		'b' => 6356034.447,
		'F0' => 1.000035,
		'E0' => 200000,
		'N0' => 250000,
		'Phi0' => 53.5,
		'Lambda0' => -8
	);
	private $datumset_IRENET95 = Array(
		//ITM (uses WGS84) (OSI) taken from http://en.wikipedia.org/wiki/Irish_Transverse_Mercator
		'a' => 6378137,
		'b' => 6356752.314140356,
		'F0' => 0.999820,
		'E0' => 600000,
		'N0' => 750000,
		'Phi0' => 53.5,
		'Lambda0' => -8
	);
	//UPS (uses WGS84), taken from http://www.epsg.org/guides/ number 7 part 2 "Coordinate Conversions and Transformations including Formulas"
	//officially defined in http://earth-info.nga.mil/GandG/publications/tm8358.2/TM8358_2.pdf
	private $datumset_UPS_North = Array(
		'a' => 6378137,
		'b' => 6356752.314140356,
		'F0' => 0.994,
		'E0' => 2000000,
		'N0' => 2000000,
		'Phi0' => 90,
		'Lambda0' => 0
	);
	private $datumset_UPS_South = Array(
		'a' => 6378137,
		'b' => 6356752.314140356,
		'F0' => 0.994,
		'E0' => 2000000,
		'N0' => 2000000,
		'Phi0' => -90,
		'Lambda0' => 0
	);
	public function get_ellipsoid($name) {
		$name = 'ellipsoid_'.$name;
		if( isset( $this->$name ) ) {
			return $this->$name;
		}
		return null;
	}
	public function create_ellipsoid($a,$b) {
		return Array('a'=>$a,'b'=>$b);
	}
	public function get_datum($name) {
		$name = 'datumset_'.$name;
		if( isset( $this->$name ) ) {
			return $this->$name;
		}
		return null;
	}
	public function create_datum($ellip,$F0,$E0,$N0,$Phi0,$Lambda0) {
		if( !isset($ellip['a']) ) {
			return null;
		}
		return Array('a'=>$ellip['a'],'b'=>$ellip['b'],'F0'=>$F0,'E0'=>$E0,'N0'=>$N0,'Phi0'=>$Phi0,'Lambda0'=>$Lambda0);
	}

	//conversions between 12345,67890 grid references and latitude/longitude formats
	public $COORDS_OS_UK = 1;
	public $COORDS_OSI = 2;
	public $COORDS_GPS_UK = 3;
	public $COORDS_GPS_IRISH = 4;
	public $COORDS_GPS_ITM = 5;
	public function grid_to_lat_long($E,$N,$type = false,$return_type = false) {
		//horribly complex conversion according to "A guide to coordinate systems in Great Britain" Annexe C:
		//http://www.ordnancesurvey.co.uk/oswebsite/gps/information/coordinatesystemsinfo/guidecontents/
		//http://www.movable-type.co.uk/scripts/latlong-gridref.html shows an alternative script for JS, which also says what some OS variables represent
		if( is_array( $E ) ) {
			//passed an array, split it into parts
			$return_type = $type;
			$type = $N;
			$N = $E[1];
			$E = $E[0];
		}
		//get appropriate ellipsoid semi-major axis 'a' (metres) and semi-minor axis 'b' (metres),
		//grid scale factor on central meridian, and true origin (grid and lat-long) from Annexe A
		//extracts variables called $a,$b,$F0,$E0,$N0,$Phi0,$Lambda0
		if( is_array($type) ) {
			extract( $type, EXTR_SKIP );
		} elseif( $type == $this->COORDS_OS_UK || $type == $this->COORDS_GPS_UK ) {
			extract( $this->datumset_OSGB36, EXTR_SKIP );
		} elseif( $type == $this->COORDS_GPS_ITM ) {
			extract( $this->datumset_IRENET95, EXTR_SKIP );
		} elseif( $type == $this->COORDS_OSI || $type == $this->COORDS_GPS_IRISH ) {
			extract( $this->datumset_Ireland_1965, EXTR_SKIP );
		}
		if( !isset($F0) ) {
			//invalid type
			return false;
		}
		//PHP will not allow expressions in the arrays as they are defined inline as class properties, so do the conversion to radians here
		$Phi0 *= M_PI / 180;
		//eccentricity-squared from Annexe B B1
		//$e2 = ( ( $a * $a ) - ( $b * $b ) ) / ( $a * $a );
		$e2 = 1 - ( $b * $b ) / ( $a * $a ); //optimised
		//C1
		$n = ( $a - $b ) / ( $a + $b );
		//pre-compute values that will be re-used many times in the C3 formula
		$n2 = $n * $n;
		$n3 = pow( $n, 3 );
		$n_parts1 = ( 1 + $n + 1.25 * $n2 + 1.25 * $n3 );
		$n_parts2 = ( 3 * $n + 3 * $n2 + 2.625 * $n3 );
		$n_parts3 = ( 1.875 * $n2 + 1.875 * $n3 );
		$n_parts4 = ( 35 / 24 ) * $n3;
		//iterate to find latitude (when $N - $N0 - $M < 0.01mm)
		$Phi = $Phi0;
		$M = 0;
		$loopcount = 0;
		do {
			//C6 and C7
			$Phi += ( ( $N - $N0 - $M ) / ( $a * $F0 ) );
			//C3
			$M = $b * $F0 * (
				$n_parts1 * ( $Phi - $Phi0 ) -
				$n_parts2 * sin( $Phi - $Phi0 ) * cos( $Phi + $Phi0 ) +
				$n_parts3 * sin( 2 * ( $Phi - $Phi0 ) ) * cos( 2 * ( $Phi + $Phi0 ) ) -
				$n_parts4 * sin( 3 * ( $Phi - $Phi0 ) ) * cos( 3 * ( $Phi + $Phi0 ) )
			); //meridonal arc
			//due to number precision, it is possible to get infinite loops here for extreme cases (especially for invalid ellipsoid numbers)
			//in tests, upto 6 loops are needed for grid 25 times Earth circumference - if it reaches 100, assume it must be infinite, and break out
		} while( abs( $N - $N0 - $M ) >= 0.00001 && ++$loopcount < 100 ); //0.00001 == 0.01 mm
		//pre-compute values that will be re-used many times in the C2 and C8 formulae
		$sin_Phi = sin( $Phi );
		$sin2_Phi = $sin_Phi * $sin_Phi;
		$tan_Phi = tan( $Phi );
		$sec_Phi = 1 / cos( $Phi );
		$tan2_Phi = $tan_Phi * $tan_Phi;
		$tan4_Phi = $tan2_Phi * $tan2_Phi;
		//C2
		$Rho = $a * $F0 * ( 1 - $e2 ) * pow( 1 - $e2 * $sin2_Phi, -1.5 ); //meridional radius of curvature
		$Nu = $a * $F0 / sqrt( 1 - $e2 * $sin2_Phi ); //transverse radius of curvature
		$Eta2 = $Nu / $Rho - 1;
		//pre-compute more values that will be re-used many times in the C8 formulae
		$Nu3 = pow( $Nu, 3 );
		$Nu5 = pow( $Nu, 5 );
		//C8 parts
		$VII = $tan_Phi / ( 2 * $Rho * $Nu );
		$VIII = ( $tan_Phi / ( 24 * $Rho * $Nu3 ) ) * ( 5 + 3 * $tan2_Phi + $Eta2 - 9 * $tan2_Phi * $Eta2 );
		$IX = ( $tan_Phi / ( 720 * $Rho * $Nu5 ) ) * ( 61 + 90 * $tan2_Phi + 45 * $tan4_Phi );
		$X = $sec_Phi / $Nu;
		$XI = ( $sec_Phi / ( 6 * $Nu3 ) ) * ( ( $Nu / $Rho ) + 2 * $tan2_Phi );
		$XII = ( $sec_Phi / ( 120 * $Nu5 ) ) * ( 5 + 28 * $tan2_Phi + 24 * $tan4_Phi );
		$XIIA = ( $sec_Phi / ( 5040 * pow( $Nu, 7 ) ) ) * ( 61 + 662 * $tan2_Phi + 1320 * $tan4_Phi + 720 * pow( $tan_Phi, 6 ) );
		//C8, C9
		$Edif = $E - $E0;
		$latitude = ( $Phi - $VII * $Edif * $Edif + $VIII * pow( $Edif, 4 ) - $IX * pow( $Edif, 6 ) ) * ( 180 / M_PI );
		$longitude = $Lambda0 + ( $X * $Edif - $XI * pow( $Edif, 3 ) + $XII * pow( $Edif, 5 ) - $XIIA * pow( $Edif, 7 ) ) * ( 180 / M_PI );
		if( $type == $this->COORDS_GPS_UK ) {
			list( $latitude, $longitude ) = $this->Helmert_transform( $latitude, $longitude, $this->ellipsoid_Airy_1830, $this->Helmert_OSGB36_to_WGS84, $this->ellipsoid_WGS84 );
		} elseif( $type == $this->COORDS_GPS_IRISH ) {
			list( $latitude, $longitude ) = $this->Helmert_transform( $latitude, $longitude, $this->ellipsoid_Airy_1830_mod, $this->Helmert_Ireland65_to_WGS84, $this->ellipsoid_WGS84 );
		}
		//force the longitude between -180 and 180
		if( $longitude > 180 || $longitude < -180 ) {
			$longitude -= floor( ( $longitude + 180 ) / 360 ) * 360;
		}
		if( $return_type ) {
			//sprintf to produce simple numbers instead of scientific notation (also reduces accuracy to 6 decimal places)
			$deg = is_string($return_type) ? $return_type : '&deg;';
			return sprintf( '%F', $latitude ) . $deg . ', ' . sprintf( '%F', $longitude ) . $deg;
		} else {
			//avoid stupid -0 by adding 0
			return Array($latitude+0,$longitude+0);
		}
	}
	public function lat_long_to_grid($latitude,$longitude,$type = false,$return_type = false) {
		//horribly complex conversion according to "A guide to coordinate systems in Great Britain" Annexe C:
		//http://www.ordnancesurvey.co.uk/oswebsite/gps/information/coordinatesystemsinfo/guidecontents/
		//http://www.movable-type.co.uk/scripts/latlong-gridref.html shows an alternative script for JS, which also says what some OS variables represent
		if( is_array( $latitude ) ) {
			//passed an array, split it into parts
			$return_type = $type;
			$type = $longitude;
			$longitude = $latitude[1];
			$latitude = $latitude[0];
		}
		//convert back to local ellipsoid coordinates
		if( $type == $this->COORDS_GPS_UK ) {
			list( $latitude, $longitude ) = $this->Helmert_transform( $latitude, $longitude, $this->ellipsoid_WGS84, $this->Helmert_WGS84_to_OSGB36, $this->ellipsoid_Airy_1830 );
		} elseif( $type == $this->COORDS_GPS_IRISH ) {
			list( $latitude, $longitude ) = $this->Helmert_transform( $latitude, $longitude, $this->ellipsoid_WGS84, $this->Helmert_WGS84_to_Ireland65, $this->ellipsoid_Airy_1830_mod );
		}
		//get appropriate ellipsoid semi-major axis 'a' (metres) and semi-minor axis 'b' (metres),
		//grid scale factor on central meridian, and true origin (grid and lat-long) from Annexe A
		//extracts variables called $a,$b,$F0,$E0,$N0,$Phi0,$Lambda0
		if( is_array($type) ) {
			extract( $type, EXTR_SKIP );
		} elseif( $type == $this->COORDS_OS_UK || $type == $this->COORDS_GPS_UK ) {
			extract( $this->datumset_OSGB36, EXTR_SKIP );
		} elseif( $type == $this->COORDS_GPS_ITM ) {
			extract( $this->datumset_IRENET95, EXTR_SKIP );
		} elseif( $type == $this->COORDS_OSI || $type == $this->COORDS_GPS_IRISH ) {
			extract( $this->datumset_Ireland_1965, EXTR_SKIP );
		}
		if( !isset($F0) ) {
			//invalid type
			return false;
		}
		//PHP will not allow expressions in the arrays as they are defined inline as class properties, so do the conversion to radians here
		$Phi0 *= M_PI / 180;
		$Phi = $latitude * M_PI / 180;
		$Lambda = $longitude - $Lambda0;
		//force Lambda between -180 and 180
		if( $Lambda > 180 || $Lambda < -180 ) {
			$Lambda -= floor( ( $Lambda + 180 ) / 360 ) * 360;
		}
		$Lambda *= M_PI / 180;
		//eccentricity-squared from Annexe B B1
		//$e2 = ( ( $a * $a ) - ( $b * $b ) ) / ( $a * $a );
		$e2 = 1 - ( $b * $b ) / ( $a * $a ); //optimised
		//C1
		$n = ( $a - $b ) / ( $a + $b );
		//pre-compute values that will be re-used many times in the C2, C3 and C4 formulae
		$sin_Phi = sin( $Phi );
		$sin2_Phi = $sin_Phi * $sin_Phi;
		$cos_Phi = cos( $Phi );
		$cos3_Phi = pow( $cos_Phi, 3 );
		$cos5_Phi = pow( $cos_Phi, 5 );
		$tan_Phi = tan( $Phi );
		$tan2_Phi = $tan_Phi * $tan_Phi;
		$tan4_Phi = $tan2_Phi * $tan2_Phi;
		$n2 = $n * $n;
		$n3 = pow( $n, 3 );
		//C2
		$Nu = $a * $F0 / sqrt( 1 - $e2 * $sin2_Phi ); //transverse radius of curvature
		$Rho = $a * $F0 * ( 1 - $e2 ) * pow( 1 - $e2 * $sin2_Phi, -1.5 ); //meridional radius of curvature
		$Eta2 = $Nu / $Rho - 1;
		//C3, meridonal arc
		$M = $b * $F0 * (
			( 1 + $n + 1.25 * $n2 + 1.25 * $n3 ) * ( $Phi - $Phi0 ) -
			( 3 * $n + 3 * $n2 + 2.625 * $n3 ) * sin( $Phi - $Phi0 ) * cos( $Phi + $Phi0 ) +
			( 1.875 * $n2 + 1.875 * $n3 ) * sin( 2 * ( $Phi - $Phi0 ) ) * cos( 2 * ( $Phi + $Phi0 ) ) -
			( 35 / 24 ) * $n3 * sin( 3 * ( $Phi - $Phi0 ) ) * cos( 3 * ( $Phi + $Phi0 ) )
		);
		//C4
		$I = $M + $N0;
		$II = ( $Nu / 2 ) * $sin_Phi * $cos_Phi;
		$III = ( $Nu / 24 ) * $sin_Phi * $cos3_Phi * ( 5 - $tan2_Phi + 9 * $Eta2 );
		$IIIA = ( $Nu / 720 ) * $sin_Phi * $cos5_Phi * ( 61 - 58 * $tan2_Phi + $tan4_Phi );
		$IV = $Nu * $cos_Phi;
		$V = ( $Nu / 6 ) * $cos3_Phi * ( ( $Nu / $Rho ) - $tan2_Phi );
		$VI = ( $Nu / 120 ) * $cos5_Phi * ( 5 - 18 * $tan2_Phi + $tan4_Phi + 14 * $Eta2 - 58 * $tan2_Phi * $Eta2 );
		$N = $I + $II * $Lambda * $Lambda + $III * pow( $Lambda, 4 ) + $IIIA * pow( $Lambda, 6 );
		$E = $E0 + $IV * $Lambda + $V * pow( $Lambda, 3 ) + $VI * pow( $Lambda, 5 );
		if( $return_type ) {
			//avoid stupid -0 by adding 0
			return (round($E)+0).','.(round($N)+0);
		} else {
			return Array($E+0,$N+0);
		}
	}

	//UTM - freaky format consisting of 60 transverse mercators
	public function utm_to_lat_long($zone,$north = false,$x = false,$y = false,$ellip = false,$return_type = false,$deny_bad_reference = false) {
		if( is_array( $zone ) ) {
			//passed an array, split it into parts
			$deny_bad_reference = $y;
			$return_type = $x;
			$ellip = $north;
			$y = $zone[3];
			$x = $zone[2];
			$north = $zone[1];
			$zone = $zone[0];
		} else if( is_string( $zone ) ) {
			$deny_bad_reference = $y;
			$return_type = $x;
			$ellip = $north;
			$zone = strtoupper($zone);
			if( preg_match( "/^\s*(0?[1-9]|[1-5][0-9]|60)\s*([C-HJ-NP-X]|NORTH|SOUTH)\s*(-?[\d\.]+)\s*[,\s]\s*(-?[\d\.]+)\s*$/", $zone, $parsed_zone ) ) {
				//matched the shorthand 30U 1234 5678
				//[01-60](<letter>|north|south)<float>[, ]<float>
				//radix parameter is needed since numbers often start with a leading 0 and must not be treated as octal
				$zone = $parsed_zone[1] * 1;
				if( strlen( $parsed_zone[2] ) > 1 ) {
					$north = ( $parsed_zone[2] == 'NORTH' ) ? 1 : -1;
				} else {
					$north = ( $parsed_zone[2] > 'M' ) ? 1 : -1;
				}
				$x = $parsed_zone[3] * 1;
				$y = $parsed_zone[4] * 1;
			} else if( preg_match( "/^\s*(-?[\d\.]+)\s*[A-Z]*\s*[,\s]\s*(-?[\d\.]+)\s*[A-Z]*\s*[\s,]\s*ZONE\s*(0?[1-9]|[1-5][0-9]|60)\s*,?\s*([NS])/", $zone, $parsed_zone ) ) {
				//matched the longhand 630084 mE 4833438 mN, zone 17, Northern Hemisphere
				//<float>[letters][, ]<float>[letters][, ]zone[01-60][,][NS]...
				$zone = $parsed_zone[3] * 1;
				$north = ( $parsed_zone[4] == 'N' ) ? 1 : -1;
				$x = $parsed_zone[1] * 1;
				$y = $parsed_zone[2] * 1;
			} else {
				//make it reject it
				$zone = 0;
			}
		}
		if( !is_numeric($zone) || $zone < 1 || $zone > 60 || !is_numeric($x) || !is_numeric($y) ) {
			if( $deny_bad_reference ) {
				//invalid format
				return false;
			}
			if( is_array($ellip) && !isset($ellip['F0']) ) {
				//invalid ellipsoid takes priority over return value
				return false;
			}
			//default coordinates put it at 90,0 lat/long - use dms_to_dd to get the right return_type
			return $this->dms_to_dd(Array(90,0,0,1,0,0,0,0),$return_type);
		}
		$ellipsoid = is_array($ellip) ? $ellip : $this->ellipsoid_WGS84;
		$ellipsoid = Array( 'a' => $ellipsoid['a'], 'b' => $ellipsoid['b'], 'F0' => 0.9996, 'E0' => 500000, 'N0' => ( $north < 0 ) ? 10000000 : 0, 'Phi0' => 0, 'Lambda0' => ( 6 * $zone ) - 183 );
		return $this->grid_to_lat_long($x,$y,$ellipsoid,$return_type);
	}
	public function lat_long_to_utm($latitude,$longitude = false,$ellip = false,$format = false,$return_type = false,$deny_bad_coords = false) {
		if( is_array( $latitude ) ) {
			//passed an array, split it into parts
			$deny_bad_coords = $return_type;
			$return_type = $format;
			$format = $ellip;
			$ellip = $longitude;
			$longitude = $latitude[1];
			$latitude = $latitude[0];
		}
		//force the longitude between -180 and 179.99...9
		if( $longitude >= 180 || $longitude < -180 ) {
			$longitude -= floor( ( $longitude + 180 ) / 360 ) * 360;
			if( $longitude == 180 ) {
				$longitude = -180;
			}
		}
		if( !is_numeric($longitude) || !is_numeric($latitude) || $latitude > 84 || $latitude < -80 ) {
			if( $deny_bad_coords ) {
				//invalid format
				return false;
			}
			//default coordinates put it at ~0,0 lat/long
			if( !is_numeric($longitude) ) {
				$longitude = 0;
			}
			if( !is_numeric($latitude) ) {
				$latitude = 0;
			}
			if( $latitude > 84 ) {
				//out of range, return appropriate polar letter, and bail out
				$zoneletter = $format ? 'North' : ( ( $longitude < 0 ) ? 'Y' : 'Z' );
				$zone = $x = $y = 0;
			}
			if( $latitude < -80 ) {
				//out of range, return appropriate polar letter, and bail out
				$zoneletter = $format ? 'South' : ( ( $longitude < 0 ) ? 'A' : 'B' );
				$zone = $x = $y = 0;
			}
		}
		if( !isset($zoneletter) || !$zoneletter ) {
			//add hacks to work out if it lies in the non-standard zones
			if( $latitude >= 72 && $longitude >= 6 && $longitude < 36 ) {
				//band X, these parts are moved around
				if( $longitude < 9 ) {
					$zone = 31;
				} else if( $longitude < 21 ) {
					$zone = 33;
				} else if( $longitude < 33 ) {
					$zone = 35;
				} else {
					$zone = 37;
				}
			} else if( $latitude >= 56 && $latitude < 64 && $longitude >= 3 && $longitude < 6 ) {
				//band Y, this part of zone 31 is moved into zone 32
				$zone = 32;
			} else {
				//yay for standards!
				$zone = floor( $longitude / 6 ) + 31;
			}
			//get the band letter
			if( $format ) {
				$zoneletter = ( $latitude < 0 ) ? 'South' : 'North';
			} else {
				$zoneletter = floor( $latitude / 8 ) + 77; //67 is ASCII C
				if( $zoneletter > 72 ) {
					//skip I
					$zoneletter++;
				}
				if( $zoneletter > 78 ) {
					//skip O
					$zoneletter++;
				}
				if( $zoneletter > 88 ) {
					//X is as high as it goes
					$zoneletter = 88;
				}
				$zoneletter = chr($zoneletter);
			}
			//do actual transformation
			$ellipsoid = is_array($ellip) ? $ellip : $this->ellipsoid_WGS84;
			$ellipsoid = Array( 'a' => $ellipsoid['a'], 'b' => $ellipsoid['b'], 'F0' => 0.9996, 'E0' => 500000, 'N0' => ( $latitude < 0 ) ? 10000000 : 0, 'Phi0' => 0, 'Lambda0' => ( 6 * $zone ) - 183 );
			$tmpcoords = $this->lat_long_to_grid($latitude,$longitude,$ellipsoid);
			if( !$tmpcoords ) { return false; }
			$x = $tmpcoords[0];
			$y = $tmpcoords[1];
		}
		if( $return_type ) {
			//avoid stupid -0 by adding 0
			$x = round($x) + 0;
			$y = round($y) + 0;
			if( $format ) {
				return $x . 'mE, ' . $y . 'mN, Zone ' . $zone . ', ' . $zoneletter . 'ern Hemisphere';
			}
			return $zone . $zoneletter . ' ' . $x . ' ' . $y;
		} else {
			return Array( $zone, ( $latitude < 0 ) ? -1 : 1, $x+0, $y+0, $zoneletter );
		}
	}

	//basic polar stereographic pojections
	//formulae according to http://www.epsg.org/guides/ number 7 part 2 "Coordinate Conversions and Transformations including Formulas"
	public function polar_to_lat_long($easting,$northing,$datum = false,$return_type = false) {
		if( is_array($easting) ) {
			//passed an array, split it into parts
			$return_type = $datum;
			$datum = $northing;
			$northing = $easting[1];
			$easting = $easting[0];
		}
		if( !$datum ) {
			return false;
		}
		if( !isset($datum['F0']) || ( $datum['Phi0'] != 90 && $datum['Phi0'] != -90 ) ) {
			//invalid type
			return false;
		}
		$a = $datum['a'];
		$b = $datum['b'];
		$k0 = $datum['F0'];
		$FE = $datum['E0'];
		$FN = $datum['N0'];
		$Phi0 = $datum['Phi0'];
		$Lambda0 = $datum['Lambda0'];
		//eccentricity-squared
		$e2 = 1 - ( $b * $b ) / ( $a * $a ); //optimised
		$e = sqrt($e2);
		$Rho = sqrt( pow( $easting - $FE, 2 ) + pow( $northing - $FN, 2 ) );
		$t = $Rho * sqrt( pow( 1 + $e, 1 + $e ) * pow( 1 - $e, 1 - $e ) ) / ( 2 * $a * $k0 );
		if( $Phi0 < 0 ) {
			//south
			$x = 2 * atan($t) - M_PI / 2;
		} else {
			//north
			$x = M_PI / 2 - 2 * atan($t);
		}
		//pre-compute values that will be re-used many times in the Phi formula
		$e4 = $e2 * $e2;
		$e6 = $e4 * $e2;
		$e8 = $e4 * $e4;
		$Phi = $x + ( $e2 / 2 + 5 * $e4 / 24 + $e6 / 12 + 13 * $e8 / 360 ) * sin( 2 * $x ) +
			( 7 * $e4 / 48 + 29 * $e6 / 240 + 811 * $e8 / 11520 ) * sin( 4 * $x ) +
			( 7 * $e6 / 120 + 81 * $e8 / 1120 ) * sin( 6 * $x ) +
			( 4279 * $e8 / 161280 ) * sin( 8 * $x );
		//longitude
		//formulas here are wrong in the epsg guide; atan(foo/bar) should have been atan2(foo,bar) or it is wrong for half of the grid
		if( $Phi0 < 0 ) {
			//south
			$Lambda = $Lambda0 + atan2( $easting - $FE, $northing - $FN );
		} else {
			//north
			$Lambda = $Lambda0 + atan2( $easting - $FE, $FN - $northing );
		}
		$latitude = $Phi * 180 / M_PI;
		$longitude = $Lambda * 180 / M_PI + $Lambda0;
		//force the longitude between -180 and 180 (in case Lambda0 pushes it beyond the limits)
		if( $longitude > 180 || $longitude < -180 ) {
			$longitude -= floor( ( $longitude + 180 ) / 360 ) * 360;
		}
		if( $return_type ) {
			//sprintf to produce simple numbers instead of scientific notation (also reduces accuracy to 6 decimal places)
			$deg = is_string($return_type) ? $return_type : '&deg;';
			return sprintf( '%F', $latitude ) . $deg . ', ' . sprintf( '%F', $longitude ) . $deg;
		} else {
			//avoid stupid -0 by adding 0
			return Array($latitude+0,$longitude+0);
		}
	}
	public function lat_long_to_polar($latitude,$longitude,$datum = false,$return_type = false) {
		if( is_array($latitude) ) {
			//passed an array, split it into parts
			$return_type = $datum;
			$datum = $longitude;
			$longitude = $latitude[1];
			$latitude = $latitude[0];
		}
		if( !$datum ) {
			return false;
		}
		if( !isset($datum['F0']) || ( $datum['Phi0'] != 90 && $datum['Phi0'] != -90 ) ) {
			//invalid type
			return false;
		}
		$a = $datum['a'];
		$b = $datum['b'];
		$k0 = $datum['F0'];
		$FE = $datum['E0'];
		$FN = $datum['N0'];
		$Phi0 = $datum['Phi0'];
		$Lambda0 = $datum['Lambda0'];
		$Phi = $latitude * M_PI / 180;
		$Lambda = ( $longitude - $Lambda0 ) * M_PI / 180;
		//eccentricity-squared
		$e2 = 1 - ( $b * $b ) / ( $a * $a ); //optimised
		$e = sqrt($e2);
		//t
		$sinPhi = sin( $Phi );
		if( $latitude < 0 ) {
			//south
			$t = tan( ( M_PI / 4 ) + ( $Phi / 2 ) ) / pow( ( 1 + $e * $sinPhi ) / ( 1 - $e * $sinPhi ), $e / 2 );
		} else {
			//north
			$t = tan( ( M_PI / 4 ) - ( $Phi / 2 ) ) * pow( ( 1 + $e * $sinPhi ) / ( 1 - $e * $sinPhi ), $e / 2 );
		}
		//Rho
		$Rho = 2 * $a * $k0 * $t / sqrt( pow( 1 + $e, 1 + $e ) * pow( 1 - $e, 1 - $e ) );
		if( $latitude < 0 ) {
			//south
			$N = $FN + $Rho * cos( $Lambda );
		} else {
			//north - origin is *down*
			$N = $FN - $Rho * cos( $Lambda );
		}
		$E = $FE + $Rho * sin( $Lambda );
		if( $return_type ) {
			//avoid stupid -0 by adding 0
			return (round($E)+0).','.(round($N)+0);
		} else {
			return Array($E+0,$N+0);
		}
	}
	public function ups_to_lat_long($hemisphere,$x = false,$y = false,$return_type = false,$deny_bad_reference = false,$min_length = false) {
		if( is_array($hemisphere) ) {
			//passed an array, so extract the grid reference from it
			$min_length = $return_type;
			$deny_bad_reference = $y;
			$return_type = $x;
			$x = $hemisphere[1];
			$y = $hemisphere[2];
			$hemisphere = $hemisphere[0];
		} else if( !is_string($x) || !is_string($y) ) {
			//a single string 'X 12345 67890', split into parts
			$min_length = $return_type;
			$deny_bad_reference = $y;
			$return_type = $x;
			//(A|B|Y|Z|N|S|north|south)[,]<float>[, ]<float>
			if( !preg_match( "/^\s*([ABNSYZ]|NORTH|SOUTH)\s*,?\s*(-?[\d\.]+)\s*[\s,]\s*(-?[\d\.]+)\s*$/i", $hemisphere, $hemisphere ) ) {
				if( $deny_bad_reference ) {
					//invalid format
					return false;
				}
				$x = $y = null;
			} else {
				$x = $hemisphere[2];
				$y = $hemisphere[3];
				$hemisphere = $hemisphere[1];
			}
		}
		if( is_string($hemisphere) ) {
			$hemisphere = strtoupper($hemisphere);
		}
		if( !is_numeric($x) || !is_numeric($y) || ( !is_numeric($hemisphere) && ( !is_string($hemisphere) || !preg_match( "/^([ABNSYZ]|NORTH|SOUTH)$/", $hemisphere ) ) ) || ( $min_length && ( $x < 800000 || $y < 800000 ) ) ) {
			if( $deny_bad_reference ) {
				return false;
			}
			//default coordinates put it at 0,0 lat/long - use dms_to_dd to get the right return_type
			return $this->dms_to_dd(Array(0,0,0,0,0,0,0,0),$return_type);
		}
		if( !is_string($hemisphere) ) {
			$hemisphere = ( $hemisphere < 0 ) ? 'S' : 'N';
		}
		if( $hemisphere == 'N' || $hemisphere == 'NORTH' || $hemisphere == 'Y' || $hemisphere == 'Z' ) {
			$hemisphere = $this->datumset_UPS_North;
		} else {
			$hemisphere = $this->datumset_UPS_South;
		}
		return $this->polar_to_lat_long($x*1,$y*1,$hemisphere,$return_type);
	}
	public function lat_long_to_ups($latitude,$longitude = false,$format = false,$return_type = false,$deny_bad_coords = false) {
		if( is_array($latitude) ) {
			//passed an array, split it into parts
			$deny_bad_coords = $return_type;
			$return_type = $format;
			$format = $longitude;
			$longitude = $latitude[1];
			$latitude = $latitude[0];
		}
		//force the longitude between -179.99...9 and 180
		if( $longitude > 180 || $longitude < -180 ) {
			$longitude -= floor( ( $longitude + 180 ) / 360 ) * 360;
			if( $longitude == -180 ) {
				$longitude = 180;
			}
		}
		if( !is_numeric($longitude) || !is_numeric($latitude) || $latitude > 90 || $latitude < -90 || ( $latitude < 83.5 && $latitude > -79.5 ) ) {
			if( $deny_bad_coords ) {
				//invalid format
				return false;
			}
			//default coordinates put it as 90,0 in OUT_OF_GRID zone
			$tmp = Array( 2000000, 2000000 );
			$letter = 'OUT_OF_GRID';
		} else {
			$tmp = $this->lat_long_to_polar( $latitude, $longitude, ( $latitude < 0 ) ? $this->datumset_UPS_South : $this->datumset_UPS_North );
			if( $latitude < 0 ) {
				if( $format ) {
					$letter = 'S';
				} else if( $longitude < 0 ) {
					$letter = 'A';
				} else {
					$letter = 'B';
				}
			} else {
				if( $format ) {
					$letter = 'N';
				} else if( $longitude < 0 ) {
					$letter = 'Y';
				} else {
					$letter = 'Z';
				}
			}
		}
		if( $return_type ) {
			//avoid stupid -0 by adding 0
			$tmp[0] = round($tmp[0]) + 0;
			$tmp[1] = round($tmp[1]) + 0;
			return $letter.' '.$tmp[0].' '.$tmp[1];
		} else {
			return Array( ( $latitude < 0 ) ? -1 : 1, $tmp[0]+0, $tmp[1]+0, $letter );
		}
	}

	//Helmert transform parameters used during Helmert transformations
	//OSGB<->WGS84 parameters taken from "6.6 Approximate WGS84 to OSGB36/ODN transformation"
	//http://www.ordnancesurvey.co.uk/oswebsite/gps/information/coordinatesystemsinfo/guidecontents/guide6.html
	private $Helmert_WGS84_to_OSGB36 = Array(
		'tx' => -446.448,
		'ty' => 125.157,
		'tz' => -542.060,
		's' => 20.4894,
		'rx' => -0.1502,
		'ry' => -0.2470,
		'rz' => -0.8421
	);
	private $Helmert_OSGB36_to_WGS84 = Array(
		'tx' => 446.448,
		'ty' => -125.157,
		'tz' => 542.060,
		's' => -20.4894,
		'rx' => 0.1502,
		'ry' => 0.2470,
		'rz' => 0.8421
	);
	//Ireland65<->WGS84 parameters taken from http://en.wikipedia.org/wiki/Helmert_transformation
	private $Helmert_WGS84_to_Ireland65 = Array(
		'tx' => -482.53,
		'ty' => 130.596,
		'tz' => -564.557,
		's' => -8.15,
		'rx' => 1.042,
		'ry' => 0.214,
		'rz' => 0.631
	);
	private $Helmert_Ireland65_to_WGS84 = Array(
		'tx' => 482.53,
		'ty' => -130.596,
		'tz' => 564.557,
		's' => 8.15,
		'rx' => -1.042,
		'ry' => -0.214,
		'rz' => -0.631
	);
	public function get_transformation($name) {
		$name = 'Helmert_'.$name;
		if( isset( $this->$name ) ) {
			return $this->$name;
		}
		return null;
	}
	public function create_transformation($tx,$ty,$tz,$s,$rx,$ry,$rz) {
		return Array('tx'=>$tx,'ty'=>$ty,'tz'=>$tz,'s'=>$s,'rx'=>$rx,'ry'=>$ry,'rz'=>$rz);
	}
	public function Helmert_transform($N,$E,$H,$from,$via = false,$to = false,$return_type = false) {
		//conversion according to formulae listed on http://www.movable-type.co.uk/scripts/latlong-convert-coords.html
		//parts taken from http://www.ordnancesurvey.co.uk/oswebsite/gps/information/coordinatesystemsinfo/guidecontents/
		$has_height = true;
		if( is_array( $N ) ) {
			//passed an array, split it into parts
			$return_type = $via;
			$to = $from;
			$via = $H;
			$from = $E;
			$E = $N[1];
			$N = $N[0];
			$has_height = isset( $N[2] );
			$H = $has_height ? $N[2] : 0;
		} elseif( is_array( $H ) ) {
			//no height, assume 0
			$has_height = false;
			$return_type = $to;
			$to = $via;
			$via = $from;
			$from = $H;
			$H = 0;
		}
		//work in radians
		$N *= M_PI / 180;
		$E *= M_PI / 180;
		//convert polar coords to cartesian
		//eccentricity-squared of source ellipsoid from Annexe B B1
		$e2 = 1 - ( $from['b'] * $from['b'] ) / ( $from['a'] * $from['a'] );
		$sin_Phi = sin( $N );
		$cos_Phi = cos( $N );
		//transverse radius of curvature
		$Nu = $from['a'] / sqrt( 1 - $e2 * $sin_Phi * $sin_Phi );
		$x = ( $Nu + $H ) * $cos_Phi * cos( $E );
		$y = ( $Nu + $H ) * $cos_Phi * sin( $E );
		$z = ( ( 1 - $e2 ) * $Nu + $H ) * $sin_Phi;
		//extracts variables called $tx,$ty,$tz,$s,$rx,$ry,$rz
		extract( $via, EXTR_SKIP );
		//convert seconds to radians
		$rx *= M_PI / 648000;
		$ry *= M_PI / 648000;
		$rz *= M_PI / 648000;
		//convert ppm to pp_one, and add one to avoid recalculating
		$s = $s / 1000000 + 1;
		//apply Helmert transform (algorithm notes incorrectly show rx instead of rz in $x1 line)
		$x1 = $tx + $s * $x - $rz * $y + $ry * $z;
		$y1 = $ty + $rz * $x + $s * $y - $rx * $z;
		$z1 = $tz - $ry * $x + $rx * $y + $s * $z;
		//convert cartesian coords back to polar
		//eccentricity-squared of destination ellipsoid from Annexe B B1
		$e2 = 1 - ( $to['b'] * $to['b'] ) / ( $to['a'] * $to['a'] );
		$p = sqrt( $x1 * $x1 + $y1 * $y1 );
		$Phi = atan2( $z1, $p * ( 1 - $e2 ) );
		$Phi1 = 2 * M_PI;
		$accuracy = 0.000001 / $to['a']; //0.01 mm accuracy, though the OS transform itself only has 4-5 metres
		$loopcount = 0;
		//due to number precision, it is possible to get infinite loops here for extreme cases (especially for invalid parameters)
		//in tests, upto 4 loops are needed - if it reaches 100, assume it must be infinite, and break out
		while( abs( $Phi - $Phi1 ) > $accuracy && $loopcount++ < 100 ) {
			$sin_Phi = sin( $Phi );
			$Nu = $to['a'] / sqrt( 1 - $e2 * $sin_Phi * $sin_Phi );
			$Phi1 = $Phi;
			$Phi = atan2( $z1 + $e2 * $Nu * $sin_Phi, $p );
		}
		$Lambda = atan2( $y1, $x1 );
		$H = ( $p / cos( $Phi ) ) - $Nu;
		//done converting - return in degrees - avoid stupid -0 by adding 0
		$latitude = ( $Phi * ( 180 / M_PI ) ) + 0;
		$longitude = ( $Lambda * ( 180 / M_PI ) ) + 0;
		if( $return_type ) {
			//sprintf to produce simple numbers instead of scientific notation (also reduces accuracy to 6 decimal places)
			$deg = is_string($return_type) ? $return_type : '&deg;';
			return sprintf( '%F', $latitude ) . $deg . ', ' . sprintf( '%F', $longitude ) . $deg . ( $has_height ? ( ', ' . sprintf( '%F', $H ) ) : '' );
		} else {
			$temparray = Array($latitude,$longitude);
			if( $has_height ) { $temparray[] = $H; }
			return $temparray;
		}
	}

	//conversions between different latitude/longitude formats
	public function dd_to_dms($N,$E = false,$only_dm = false,$return_type = false) {
		//decimal degrees (49.5,-123.5) to degrees-minutes-seconds (49°30'0"N, 123°30'0"W)
		if( is_array( $N ) ) {
			//passed an array, split it into parts
			$return_type = $only_dm;
			$only_dm = $E;
			$E = $N[1];
			$N = $N[0];
		}
		$N_abs = abs($N);
		$E_abs = abs($E);
		$degN = floor($N_abs);
		$degE = floor($E_abs);
		if( $only_dm ) {
			$minN = 60 * ( $N_abs - $degN );
			$secN = 0;
			$minE = 60 * ( $E_abs - $degE );
			$secE = 0;
		} else {
			//the approach used here is careful to respond consistently to floating point errors for all of degrees/minutes/seconds
			//errors should not cause one to be increased while another is decreased (which could cause eg. 5 minutes 60 seconds)
			$minN = 60 * $N_abs;
			$secN = ( $minN - floor( $minN ) ) * 60;
			$minN %= 60;
			$minE = 60 * $E_abs;
			$secE = ( $minE - floor( $minE ) ) * 60;
			$minE %= 60;
		}
		//avoid stupid -0 by adding 0
		$degN += 0;
		$minN += 0;
		$secN += 0;
		$degE += 0;
		$minE += 0;
		$secE += 0;
		if( $return_type ) {
			//sprintf to produce simple numbers instead of scientific notation (also reduces accuracy to 6 decimal places)
			$deg = is_string($return_type) ? $return_type : '&deg;';
			$quot = is_string($return_type) ? '"' : '&quot;';
			if( $only_dm ) {
				//careful not to round up to 60 minutes when displaying
				return
					$degN . $deg . sprintf( '%F', ( $minN >= 59.9999995 ) ? 59.999999 : $minN ) . "'" . ( ( $N < 0 ) ? 'S' : 'N' ) . ', ' .
					$degE . $deg . sprintf( '%F', ( $minE >= 59.9999995 ) ? 59.999999 : $minE ) . "'" . ( ( $E < 0 ) ? 'W' : 'E' );
			} else {
				//careful not to round up to 60 seconds when displaying
				return
					$degN . $deg . $minN . "'" . sprintf( '%F', ( $secN >= 59.9999995 ) ? 59.999999 : $secN ) . $quot . ( ( $N < 0 ) ? 'S' : 'N' ) . ', ' .
					$degE . $deg . $minE . "'" . sprintf( '%F', ( $secE >= 59.9999995 ) ? 59.999999 : $secE ) . $quot . ( ( $E < 0 ) ? 'W' : 'E' );
			}
		} else {
			return Array( $degN, $minN, $secN, ( $N < 0 ) ? -1 : 1, $degE, $minE, $secE, ( $E < 0 ) ? -1 : 1 );
		}
	}
	public function dd_format($N,$E = false,$no_units = false,$return_type = false) {
		//different formats of decimal degrees (49.5,-123.5)
		if( is_array( $N ) ) {
			//passed an array, split it into parts
			$return_type = $no_units;
			$no_units = $E;
			$E = $N[1];
			$N = $N[0];
		}
		if( $no_units ) {
			$lat_mul = $long_mul = $return_type ? '' : 1;
		} else {
			$lat_mul = $return_type ? ( ( $N < 0 ) ? 'S' : 'N' ) : ( ( $N < 0 ) ? -1 : 1 );
			$long_mul = $return_type ? ( ( $E < 0 ) ? 'W' : 'E' ) : ( ( $E < 0 ) ? -1 : 1 );
			$N = abs($N);
			$E = abs($E);
		}
		if( $return_type ) {
			$deg = is_string($return_type) ? $return_type : '&deg;';
			return sprintf( '%F', $N ) . $deg . $lat_mul . ', ' . sprintf( '%F', $E ) . $deg . $long_mul;
		}
		return Array( $N, 0, 0, $lat_mul, $E, 0, 0, $long_mul );
	}
	public function dms_to_dd($dms,$return_type = false,$deny_bad_coords = false) {
		//degrees-minutes-seconds (49°30'0"N, 123°30'0"W) to decimal degrees (49.5,-123.5)
		if( is_array( $dms ) ) {
			//passed an array of values, which can be unshifted once to get the right positions
			$latlong = $dms;
			array_unshift($latlong,'','');
			array_splice($latlong,6,0,'');
		} else {
			//simple regex ;) ... matches upto 3 sets of number[non-number] per northing/easting (allows for DMS, DM or D)
			//uses \D+ to avoid encoding conflicts with ° character which can become multibyte (u flag just fails to match)
			//note that this cannot accept HTML strings from dd_to_dms as it will not match &quot;
			//[-]<float><separator>[<float><separator>[<float><separator>]]([NS][,]|,)[-]<float><separator>[<float><separator>[<float><separator>]][EW]
			//Captures -, <float>, <float>, <float>, [NS], -, <float>, <float>, <float>, [EW]
			if( !preg_match( "/^\s*(-?)([\d\.]+)\D+\s*(?:([\d\.]+)\D\s*(?:([\d\.]+)\D\s*)?)?(?:([NS])\s*,?|,)\s*(-?)([\d\.]+)\D+\s*(?:([\d\.]+)\D\s*(?:([\d\.]+)\D\s*)?)?([EW]?)\s*$/", strtoupper($dms), $latlong ) ) {
				//invalid format
				if( $deny_bad_coords ) {
					return false;
				}
				//assume 0,0
				$latlong = Array('','','0','0','0','N','','0','0','0','E');
			}
		}
		if( !$latlong[3] ) { $latlong[3] = 0; }
		if( !$latlong[4] ) { $latlong[4] = 0; }
		if( !$latlong[8] ) { $latlong[8] = 0; }
		if( !$latlong[9] ) { $latlong[9] = 0; }
		$lat = $latlong[2] + $latlong[3] / 60 + $latlong[4] / 3600;
		if( $latlong[1] ) { $lat *= -1; }
		if( $latlong[5] == 'S' || $latlong[5] == -1 ) { $lat *= -1; }
		$long = $latlong[7] + $latlong[8] / 60 + $latlong[9] / 3600;
		if( $latlong[6] ) { $long *= -1; }
		if( $latlong[10] == 'W' || $latlong[10] == -1 ) { $long *= -1; }
		if( $return_type ) {
			//sprintf to produce simple numbers instead of scientific notation (also reduces accuracy to 6 decimal places)
			$deg = is_string($return_type) ? $return_type : '&deg;';
			return sprintf( '%F', $lat ) . $deg . ', ' . sprintf( '%F', $long ) . $deg;
		} else {
			//avoid stupid -0 by adding 0
			return Array( $lat + 0, $long + 0 );
		}
	}

}

?>