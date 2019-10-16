<?php

namespace RSSMerger;

/**
 * Class Merger
 */
class Merger
{
    /**
     * RSS value
     * @var RSS
     */
    public $rss;

    /**
     * Retrieved RSS feeds
     * @var array
     */
    public $feeds;

    /**
     * @var InternalCache
     */
    public $cache;

    /**
     * Merger constructor.
     * @param RSS $rss
     */
    public function __construct($rss)
    {
        global $cache;
        $this->rss = $rss;
        $this->cache = new InternalCache($rss->id);
    }

    /**
     * @return array
     */
    public function getFeeds()
    {
        $urls = $this->rss->feeds;
        $items = array();
        foreach ($urls as $url) {
            $xml = simplexml_load_file($url, null, false);
            if ($xml) {
                foreach ($xml->channel->item as $item) {
                    $new['title'] = $item->title;
                    $new['link'] = $item->link;
                    $new['description'] = $item->description;
                    $new['pubDate'] = $item->pubDate;
                    $new['guid'] = $item->guid;
                    $new['date'] = strtotime($item->pubDate);

                    array_push($items, $new);
                }
            }
        }
        usort($items, array(&$this, 'sortByDate'));
        return $items;
    }

    /**
     * @param $a
     * @param $b
     * @return int
     */
    private static function sortByDate($a, $b)
    {
        if ($a['date'] == $b['date']) {
            return 0;
        } elseif ($a['date'] > $b['date']) {
            return -1;
        } else {
            return 1;
        }
    }
}
