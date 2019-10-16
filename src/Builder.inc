<?php

namespace RSSMerger;

/**
 * Class Builder
 */
class Builder
{
    /**
     * @var array
     */
    public $lists;

    /**
     * Builder constructor.
     * @param array $rss_array
     */
    public function __construct(array $rss_array)
    {
        $lists = array();
        foreach ($rss_array as $rss) {
            $lists[$rss->id] = new Merger($rss);
        }
        $this->lists = $lists;
    }

    /**
     * @param string $id
     * @return string
     */
    public function generateOutput($id)
    {
        $merger = $this->lists[$id];
        $rss = $merger->rss;
        $cache = $merger->cache;

        if ($cache->checkCache() && $_GET['force'] != true) {
            return $cache->getCache();
        }

        $feeds = $merger->getFeeds();


        $result = array(
            "encoding" => $rss->encoding,
            "title" => $rss->title,
            "link" => $rss->link,
            "description" => $rss->description,
            "pubDate" => date(DATE_RFC822),
            "ttl" => $rss->ttl,
            "generator" => StaticValue::GENERATOR(),
            "language" => $rss->lang,
            "items" => $feeds
        );


        $renderer = include('template/feed.php');
        $output = $renderer($result);

        $cache->setCache($output);
        return $output;
    }
}
