<?php

declare(strict_types=1);

namespace RSSMerger;

use Closure;
use SimpleXMLElement;
use function array_push;
use function simplexml_load_file;
use function strtotime;
use function usort;

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
     * Cache.
     *
     * @var InternalCache
     */
    public $cache;


    /**
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
     * @return object[]
     */
    public function getFeeds() : array
    {
        $urls  = $this->rss->feeds;
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
                $new['dc:creator']     = self::getNsChild($item, 'dc', 'creator');
                $new['description']    = $item->description;
                $new['contentEncoded'] = self::getNsChild($item, 'content', 'encoded');
                $new['date']           = strtotime((string) $item->pubDate);

                array_push($items, $new);
            }
        }

        usort($items, self::sortBy('date'));
        return $items;
    }


    /**
     * Sort by provided key
     *
     * @param string $key Key name.
     *
     * @return Closure
     */
    private static function sortBy(string $key) : Closure
    {
        return static function ($a, $b) use ($key) {
            if ($a[$key] === $b[$key]) {
                return 0;
            }

            if ($a[$key] > $b[$key]) {
                return -1;
            }

            return 1;
        };
    }

    /**
     * Get Namespace's Child
     *
     * @param  SimpleXMLElement $xml       XML Element.
     * @param  string           $namespace Namspace name.
     * @param  string           $nodeName  Child Node name.
     *
     * @return SimpleXMLElement
     */
    private static function getNsChild(SimpleXMLElement $xml, string $namespace, string $nodeName) : SimpleXMLElement
    {
        $ns = $xml->getNamespaces(true);
        return $xml->children($ns[$namespace])->{$nodeName};
    }
}
