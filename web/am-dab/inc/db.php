<?php

/**
 * Erzeugt DB-Verbindung
 */
function get_db($db_host, $db_user, $db_pw, $db_name) {
	$db = new mysqli($db_host, $db_user, $db_pw, $db_name);
	if($db->connect_error) {
		throw new Exception("Keine DB!");
	}
	return $db;
}
?>