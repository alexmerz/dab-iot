<?php
/**
 * Erwartet die Daten als GET-Request, GET-Request darf nicht länger als 250 Zeichen sein.
 * Parameter: 'token', 'date', 'time','temp', 'humi', 'dust', 'light', 'uv', 'sound',
 * 'lat', 'long', 'alt', 'press', 'zacc'
 */
require_once 'inc/exception.php';
require_once '/files/webapp/dbconfig.php';
require_once 'inc/db.php';

set_error_handler("error_handler");

// Parameter checken
if(!isset($_GET)
	|| !isset($_GET['d'])
	|| '' == trim($_GET['d'])) {
	throw new Exception("Kein d-Parameter!");
}

$raw = trim($_GET['d']);
$data = json_decode($raw, true);
if(json_last_error()) {
	throw new Exception('JSON: '.json_last_error_msg());
}

$required_keys = array('token', 'date', 'time', 
	'temp', 'humi', 'dust', 'light', 'uv', 'sound',
	'lat', 'long', 'alt', 'press', 'zacc');

if(0 < count(array_diff($required_keys,array_keys($data)))) {
	throw new Exception("Daten fehlen!");
}

// Daten aus dbconfig.php
$db = get_db($db_host, $db_user, $db_pw, $db_name);

$tpl = array(
	'token' => null,
	'ts_send' => null,
	'sound' => null,
	'light' => null,
	'uv' => null,
	'dust' => null,
	'humidity' => null,
	'temperature' => null,
	'position' => null,
	'altitude' => null,
	'z_acc' => null,
	'pressure' => null
);

// Datums- und Zeitangabe umformatieren
$dt = DateTime::createFromFormat('dmyHis*u', $data['date'].$data['time']);
$tpl['ts_send'] = $dt->format("Y-m-d H:i:s");

// Position in eine MySQL Point-Angabe umwandeln
if('' === $data['lat'] || '' === $data['long']) {
	$tpl['position'] = "null";
} else {
	if(-1 < strpos($data['lat'], 'S')) {
		$data['lat'] = '-'.($data['lat']*100);
	}
	if(-1 < strpos($data['long'], 'W')) {
		$data['long'] = '-'.($data['long']*100);
	}
	$chars = array('W', 'E', 'S', 'N');
	$tpl['position'] = sprintf("ST_GeomFromText('POINT(%s)')",
						str_replace($chars, '', $data['long'].' '.$data['lat']));
}

// Daten für DB aufbereiten
$tpl['token'] = $data['token'];
$tpl['sound'] = $data['sound'];
$tpl['light'] = $data['light'];
$tpl['uv'] = round($data['uv']*100);
$tpl['pressure'] = $data['press'];
$tpl['humidity'] = round($data['humi'] * 10); // In Integer umwandeln
$tpl['temperature'] = round($data['temp'] * 10);
$tpl['altitude'] = round($data['alt'] * 10);
$tpl['z_acc'] = round($data['zacc'] * 100);
$tpl['dust'] = round($data['dust'] * 100000);

saveRaw($db, $raw);
saveData($db, $tpl);

echo "Ok";
exit;

/**
 * Daten strukturiert in DB speichern
 */
function saveData($db, $data) {

	$f = 'INSERT INTO data ';
	$f.= ' (token, ts_send, sound, light, uv, dust, ';
	$f.= ' humidity, temperature, position, altitude, z_acc, pressure) VALUES ';
	$f.= ' (%d,"%s",%d,%d,%d,%d,%d,%d,%s,%d,%d,%d)';
	$sql = sprintf($f,
		$db->real_escape_string($data['token']),
		$db->real_escape_string($data['ts_send']),
		$db->real_escape_string($data['sound']),
		$db->real_escape_string($data['light']),
		$db->real_escape_string($data['uv']),
		$db->real_escape_string($data['dust']),
		$db->real_escape_string($data['humidity']),
		$db->real_escape_string($data['temperature']),
		$data['position'],
		$db->real_escape_string($data['altitude']),
		$db->real_escape_string($data['z_acc']),		
		$db->real_escape_string($data['pressure'])		
		);

	$result = $db->query($sql);
	if(!$result || "" !== $db->error) {
		throw new Exception($db->error.' '.$sql);
	}
}

/**
 * Daten as is in DB speichern
 */
function saveRaw($db, $_data) {
	$f = 'INSERT INTO log (raw) VALUES ("%s")';
	$sql = sprintf($f, $db->real_escape_string($_data));
	$result = $db->query($sql);
	if(!$result || "" !== $db->error) {
		throw new Exception($db->error.' '.$sql);
	}
}

/**
 * Wird aufgerufen wenn ein Fehler auftritt
 * über exception.php werden auch Exceptions 
 * als error übermittelt
 */
function error_handler($code, $msg) {
	header($_SERVER["SERVER_PROTOCOL"]." 501  Internal Server Error", true, 500);
	echo $msg;
	exit;
}
?>
