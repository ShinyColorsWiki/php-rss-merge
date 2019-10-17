<?php

declare(strict_types=1);

require __DIR__ . '/vendor/autoload.php';

use Cache\Adapter\Filesystem\FilesystemCachePool;
use League\Flysystem\Adapter\Local;
use League\Flysystem\Filesystem;
use RSSMerger\Initializer;
use RSSMerger\RSS;

Initializer::initialize();

$filesystemAdapter = new Local(__DIR__ . '/');
$filesystem        = new Filesystem($filesystemAdapter);

$cache = new FilesystemCachePool($filesystem);

$siteName = 'ShinyWiki';
$siteUrl  = 'https://wiki.shinycolo.rs/wiki/Main_Page';

$feed[] = new RSS(
    'sc_imasblog',
    'シャイニーカラーズ – アイドルマスター公式ブログ',
    'https://idolmaster.jp/blog',
    'アイドルマスター公式ブログ',
    [
        'https://idolmaster.jp/blog/?cat=103&paged=1&feed=rss2',
        'https://idolmaster.jp/blog/?cat=103&paged=2&feed=rss2',
        'https://idolmaster.jp/blog/?cat=103&paged=3&feed=rss2',
        'https://idolmaster.jp/blog/?cat=103&paged=4&feed=rss2',
        'https://idolmaster.jp/blog/?cat=103&paged=5&feed=rss2',
        'https://idolmaster.jp/blog/?cat=103&paged=6&feed=rss2',
        'https://idolmaster.jp/blog/?cat=103&paged=7&feed=rss2',
        'https://idolmaster.jp/blog/?cat=103&paged=8&feed=rss2',
    ],
    3600,
    'UTF-8',
    'ja'
);
