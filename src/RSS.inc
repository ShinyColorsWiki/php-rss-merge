<?php

declare(strict_types=1);

namespace RSSMerger;

use function array_push;
use function simplexml_load_file;
use function strtotime;
use function usort;

/**
 * Class RSS
 */
class RSS
{
    /**
     * An identifier
     *
     * @var string
     */
    public string $id;

    /**
     * A site title
     *
     * @var string
     */
    public string $title;

    /**
     * A site link
     *
     * @var string
     */
    public string $link;

    /**
     * A site description
     *
     * @var string
     */
    public string $description;

    /**
     * Links of feed
     *
     * @var string[]
     */
    public array $feeds;

    /**
     * A time to live value
     *
     * @var int
     */
    public int $ttl;

    /**
     * A xml encoding
     *
     * @var string
     */
    public string $encoding;

    /**
     * A language code
     *
     * @var string
     */
    public string $lang;

    /**
     * Cache.
     *
     * @var InternalCache
     */
    public InternalCache $cache;



    /**
     * @param string   $id          ID of RSS.
     * @param string   $title       RSS Title.
     * @param string   $link        RSS Link.
     * @param string   $description RSS Description.
     * @param string[] $feeds       RSS Feed List.
     * @param int      $ttl         Time-to-live.
     * @param string   $encoding    Encoding of XML.
     * @param string   $lang        Language of RSS.
     */
    public function __construct(
        string $id,
        string $title,
        string $link,
        string $description,
        array $feeds,
        int $ttl = 3600,
        string $encoding = 'UTF-8',
        string $lang = 'en'
    ) {
        $this->id          = $id;
        $this->title       = $title;
        $this->link        = $link;
        $this->description = $description;
        $this->feeds       = $feeds;
        $this->ttl         = $ttl;
        $this->encoding    = $encoding;
        $this->lang        = $lang;
        $this->cache       = new InternalCache($id);
    }

    /**
     * Get Feeds
     *
     * @return object[]
     */
    public function getFeeds() : array
    {
        $urls  = $this->feeds;
        $items = [];
        foreach ($urls as $url) {
            $xml = simplexml_load_file($url, null, 0);
            if ($xml === null) {
                continue;
            }

            foreach ($xml->channel->item as $item) {
                $new['title']          = $item->title;
                $new['link']           = $item->link;
                $new['category']       = $item->category;
                $new['pubDate']        = $item->pubDate;
                $new['guid']           = $item->guid;
                $new['dc:creator']     = Statics::getNsChild($item, 'dc', 'creator');
                $new['description']    = $item->description;
                $new['contentEncoded'] = Statics::getNsChild($item, 'content', 'encoded');
                $new['date']           = strtotime((string) $item->pubDate);

                array_push($items, $new);
            }
        }

        usort($items, Statics::sortBy('date'));
        return $items;
    }
}
