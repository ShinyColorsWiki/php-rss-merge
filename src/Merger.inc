<?php

namespace RSSMerger;

/**
 * Class Merger
 */
class Merger
{

    /**
     * RSS value
     *
     * @var RSS
     */
    public $rss;

    /**
     * Retrieved RSS feeds
     *
     * @var array
     */
    public $feeds;

    /**
     * Cache.
     *
     * @var InternalCache
     */
    public $cache;


    /**
     * Merger constructor.
     *
     * @param RSS $rss RSS.
     */
    public function __construct(RSS $rss)
    {
        $this->rss   = $rss;
        $this->cache = new InternalCache($rss->id);
    }


    /**
     * Get Feeds
     *
     * @return array
     */
    public function getFeeds(): array
    {
        $urls  = $this->rss->feeds;
        $items = [];
        foreach ($urls as $url) {
            $xml = simplexml_load_file($url, null, false);
            if ($xml !== null) {
                foreach ($xml->channel->item as $item) {
                    $new['title']       = $item->title;
                    $new['link']        = $item->link;
                    $new['description'] = $item->description;
                    $new['pubDate']     = $item->pubDate;
                    $new['guid']        = $item->guid;
                    $new['date']        = strtotime($item->pubDate);

                    array_push($items, $new);
                }
            }
        }

        usort($items, [&$this, 'sortByDate']);
        return $items;
    }


    /**
     * Sort by date
     *
     * @param array $a A.
     * @param array $b B.
     *
     * @return integer Sort result.
     */
    private static function sortByDate(array $a, array $b): int
    {
        if ($a['date'] === $b['date']) {
            return 0;
        } elseif ($a['date'] > $b['date']) {
            return -1;
        } else {
            return 1;
        }
    }
}
