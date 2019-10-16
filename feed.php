<?php
require './config.php';

use RSSMerger\{Builder, BaseError};

$f = $_GET['feed'];
if (isset($f) === true) {
    $builder = new Builder($GLOBALS['feed']);
    if (array_key_exists($f, $builder->lists) === true) {
        try {
            $output = $builder->generateOutput($f);
            header('Content-Type: application/rss+xml; charset=UTF-8');
            echo $output;
        } catch (Error $e) {
            $error = new BaseError($e);
            $error->renderXML();
        } finally {
            unset($output);
        }
    } else {
        $error = new BaseError(null);
        $error->renderXML(404, 'Not Found');
    }

    unset($builder);
} else {
    $error = new BaseError(null);
    $error->renderXML(404, 'Not Found');
}//end if

unset($error);
unset($f);
