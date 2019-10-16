<?php
    require './config.php';
    use RSSMerger\StaticValue;
    global $feed;

function current_dir_url()
{
    $path = str_replace($_SERVER['DOCUMENT_ROOT'], '', realpath(__DIR__));
    if (strpos($path, 0) !== '/') {
        $path = "/" . $path;
    }
    return (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] === 'on' ? "https" : "http") .
        "://$_SERVER[HTTP_HOST]" .
        str_replace("\\", "/", $path);
}

function error($message)
{
    $renderer = include('./src/template/error.php');
    echo $renderer(array(
        "code" => 500,
        "message" => $message
    ));
}

function beforeRenderFeeds($rss)
{
    foreach ($rss as &$f) {
        $feed = (array) $f;
        $feed['feeds'] = join("<br />", $f->feeds);
        $feed['feed_url'] = current_dir_url() .
            'feed.php?feed=' . $f->id;
        $f = $feed;
    }
    return $rss;
}
try {
    global $siteName, $siteUrl;
    $renderer = include('./src/template/index.php');
    $feeds = beforeRenderFeeds($feed);
    echo $renderer(array(
        'count_number' => count($feeds),
        'feeds' => $feeds,
        'siteName' => $siteName,
        'siteUrl' => $siteUrl,
        'generator' => StaticValue::GENERATOR()
    ));
} catch (Exception $e) {
    error($e->getMessage());
} catch (Error $e) {
    error($e->getMessage());
} finally {
    unset($feeds);
    unset($renderer);
}
