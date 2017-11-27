<?php
/**
 * Konvertiert nicht abgefangen exceptions
 * zu PHP-Errors
 */
set_exception_handler("exception_handler");

function exception_handler($exception) {
	trigger_error($exception->getMessage(), E_USER_ERROR);
}
?>