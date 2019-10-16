<?php
require './config.php';

use RSSMerger\Builder;

global $feed;

function current_dir_url()
{
    $path = str_replace($_SERVER['DOCUMENT_ROOT'], '', realpath(__DIR__));
    return (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] === 'on' ? "https" : "http") .
        "://$_SERVER[HTTP_HOST]" .
        str_replace("\\", "/", $path);
}

function setLocation()
{
    header('Location: ' . current_dir_url());
}

function error($message, $code = 500)
{
    http_response_code($code);
    header('Content-Type: application/xml; charset=UTF-8');
    $renderer = include('./src/template/xmlerror.php');
    echo $renderer(array(
        "code" => $code,
        "message" => $message
    ));
}

$f = $_GET['feed'];
if (isset($f)) {
    $builder = new Builder($feed);
    if (array_key_exists($f, $builder->lists)) {
        try {
            $output = $builder->generateOutput($f);
            header('Content-Type: application/rss+xml; charset=UTF-8');
            echo $output;
        } catch (Exception $e) {
            error($e->getMessage());
        } catch (Error $e) {
            error($e->getMessage());
        } finally {
            unset($output);
            unset($renderer);
        }
    } else {
        error("Not Found", 404);
    }
    unset($builder);
} else {
    http_response_code(404);
    error("Not Found");
    setLocation();
}
unset($f);
