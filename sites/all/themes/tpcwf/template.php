<?php


function tpcwf_preprocess_node(&$variables, $hook) {
	 $variables['unpublished'] = (!$variables['status']) ? TRUE : FALSE;
}