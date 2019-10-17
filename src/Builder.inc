<?php

namespace RSSMerger;

/**
 * Class Builder
 */
class Builder
{

    /**
     * List of RSS
     *
     * @var Merger[]
     */
    public $lists;


    /**
     * Builder constructor.
     *
     * @param array $rssArray RSS Array.
     */
    public function __construct(array $rssArray)
    {
        $lists = [];
        foreach ($rssArray as $rss) {
            $lists[$rss->id] = new Merger($rss);
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
    public function generateOutput(string $id)
    {
        $merger = $this->lists[$id];
        $rss    = $merger->rss;
        $cache  = $merger->cache;

        if ($cache->checkCache() === true && isset($_GET['force']) === false) {
            return $cache->getCache();
        }

        $feeds = $merger->getFeeds();

        $result = [
            'encoding'    => $rss->encoding,
            'title'       => $rss->title,
            'link'        => $rss->link,
            'description' => $rss->description,
            'pubDate'     => date(DATE_RFC822),
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
