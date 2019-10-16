<?php
    require './config.php';
    use RSSMerger\{Statics, BaseError};

try {
    $renderer = include './src/template/index.php';
    $feeds    = Statics::beforeRenderFeeds($GLOBALS['feed']);
    echo $renderer(
        [
            'count_number' => count($feeds),
            'feeds'        => $feeds,
            'siteName'     => $GLOBALS['siteName'],
            'siteUrl'      => $GLOBALS['siteUrl'],
            'generator'    => Statics::GENERATOR(),
        ]
    );
} catch (Error $e) {
    $error = new BaseError($e);
    $error->renderHTML();
} finally {
    unset($feeds);
    unset($renderer);
    unset($error);
}//end try
