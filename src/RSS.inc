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
    public $id;

    /**
     * A site title
     *
     * @var string
     */
    public $title;

    /**
     * A site link
     *
     * @var string
     */
    public $link;

    /**
     * A site description
     *
     * @var string
     */
    public $description;

    /**
     * Links of feed
     *
     * @var string[]
     */
    public $feeds;

    /**
     * A time to live value
     *
     * @var int
     */
    public $ttl;

    /**
     * A xml encoding
     *
     * @var string
     */
    public $encoding;

    /**
     * A language code
     *
     * @var string
     */
    public $lang;

    /**
     * Cache.
     *
     * @var InternalCache
     */
    public $cache;



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

            if (isset($xml->channel->item)) {
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
            }else if (isset($xml->entry)) {
                foreach ($xml->entry as $item) {
                    if (substr($item->id, 0, strlen('yt:video:')) === 'yt:video:') {
                        $url = $item->link->attributes();
                        $videourl = $url->href;
                        $video = str_replace("https://www.youtube.com/watch?v=","",$videourl);
                        $arr = (array) $item->link;
                        $new['title']          = $item->title;
                        $new['link']           = $videourl;
                        $new['category']       = $item->category;
                        $new['pubDate']        = $item->published;
                        $new['guid']           = $item->id;
                        $new['dc:creator']     = $item->author->name;
                        $new['description']    = $item->description;
                        $new['contentEncoded'] = '<p><iframe width="560" height="315" src="https://www.youtube.com/embed/'.$video.'" frameborder="0" allowfullscreen></iframe></p>';
                        $new['date']           = strtotime((string) $item->pubDate);
                    }

                    array_push($items, $new);
                }
            }
        }

        usort($items, Statics::sortBy('date'));
        return $items;
    }
}
