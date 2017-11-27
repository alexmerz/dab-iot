<?php
/**
 * Liefert die Daten für einen Token oder alle über den Zeitraum der 
 * letzten Stunde oder des letzten Monats
 * 
 * read.php?token=<token>&period=<[hour|month]>
 * read.php?period=<[hour|month]> 
 */

require_once 'inc/exception.php';
require_once '/files/webapp/dbconfig.php';
require_once 'inc/db.php';

set_error_handler("error_handler");

$allowed_period = array('hour', 'month');

$period = 'hour';
$token = null;
$today = new DateTime();

// Variablencheck

if(isset($_GET['period'])) {
	if(!in_array($_GET['period'], $allowed_period)) {
		throw new Exception("period-Wert nicht erlaubt.");		
	} else {
		$period = $_GET['period'];
	}
}

if(isset($_GET['token'])
	&& is_numeric($_GET['token'])) {
	if(1 > $_GET['token']) {
		throw new Exception("token-Wert nicht erlaubt.");		
	} else {
		$token = $_GET['token'];		
	}
}

// Zeit von bis ermitteln

$endtime = $today->format('Y-m-d H:i:s');
$ps = '';
switch($period) {
	case 'hour':
		$ps = 'PT1H'; break;
	case 'month':
		$ps = 'P1M'; break;
}
$today->sub(new DateInterval($ps));
$starttime = $today->format('Y-m-d H:i:s');

// Datenbankabfrage

$f = "SELECT ";
$f.= " token, ts_send, ts_insert, sound, light, uv, dust, ";
$f.= " humidity, temperature, ST_AsText(position) AS longlat, altitude, z_acc, pressure";
$f.= " FROM data WHERE ";
$f.= " ts_insert > '%s' ";
$f.= " AND ts_insert < '%s' ";

if(null !== $token) {
	$f.= " AND token = ".$token." ";
}

$f.= " ORDER BY token, ts_send";

$sql = sprintf($f, $starttime, $endtime);

// Daten aus dbconfig.php
$db = get_db($db_host, $db_user, $db_pw, $db_name);
$result = $db->query($sql);

if('' !== $db->error 
	|| false === $result) {
	throw new Exception($db->error.' '.$sql);
}

$data = array();
$record = array();
while($record = $result->fetch_assoc()) {
	$data[] = convert_record($record);
}

send_result($data);

/**
 * Konvertiert einige Zahlenwerte wieder in Fließkommazahlen 
 * zurück und liefert die Ortsangabe in einem lesbaren Format
 */
function convert_record($record) {
	$data = array();
	sscanf($record['longlat'], 'POINT(%f %f)', $data['long'], $data['lat']);	
	$data['token'] = intval($record['token']);
	$data['ts_send'] = $record['ts_send']; 
	$data['ts_insert'] = $record['ts_insert']; 	
	$data['sound'] = intval($record['sound']); 
	$data['light'] = intval($record['light']); 	
	$data['uv'] = $record['uv']/100; 		
	$data['pressure'] = intval($record['pressure']); 			
	$data['dust'] = $record['dust']/100000;
	$data['humidity'] = $record['humidity']/10;
	$data['temperature'] = $record['temperature']/10;
	$data['altitude'] = $record['altitude']/10;
	$data['z_acc'] = $record['z_acc']/100;
	return $data;
}

/**
 * Daten als JSON zurückgeben
 */
function send_result($data) {
	header('Content-Type: application/json');
	$ret = array('error' => false, 'data' => $data);
	echo json_encode($ret);
	exit;
}

/**
 * Wird aufgerufen wenn ein Fehler auftritt
 * über exception.php werden auch Exceptions 
 * als error übermittelt
 */
function error_handler($code, $msg) {
	header($_SERVER["SERVER_PROTOCOL"]." 501  Internal Server Error", true, 500);
	header('Content-Type: text/json');
	$ret = array('error' => $msg, 'data' => array());
	echo json_encode($ret);
	exit;
}

?>