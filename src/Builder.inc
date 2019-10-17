<?php

declare(strict_types=1);

namespace RSSMerger;

use const DATE_RFC822;
use function date;

/**
 * Class Builder
 */
class Builder
{
    /**
     * List of RSS
     *
     * @var RSS[]
     */
    public array $lists;


    /**
     * @param RSS[] $rssArray RSS Array.
     */
    public function __construct(array $rssArray)
    {
        $lists = [];
        foreach ($rssArray as $rss) {
            $lists[$rss->id] = $rss;
        }

        $this->lists = $lists;
    }


    /**
     * Generate Output.
     *
     * @param string $id RSS id.
     *
     * @return string rendered XML.
     */
    public function generateOutput(string $id) : string
    {
        $rss   = $this->lists[$id];
        $cache = $rss->cache;

        if ($cache->checkCache() === true && isset($_GET['force']) === false) {
            return $cache->getCache();
        }

        $feeds = $rss->getFeeds();

        $result = [
            'encoding'    => $rss->encoding,
            'title'       => $rss->title,
            'atomlink'    => Statics::getFeedUrl($id),
            'link'        => $rss->link,
            'description' => $rss->description,
            'buildDate'   => date(DATE_RFC822),
            'pubDate'     => $feeds[0]['pubDate'],
            'ttl'         => $rss->ttl,
            'generator'   => Statics::GENERATOR(),
            'language'    => $rss->lang,
            'items'       => $feeds,
        ];

        $renderer = include 'template/feed.php';
        $output   = $renderer($result);

        $cache->setCache($output);
        return $output;
    }
}
