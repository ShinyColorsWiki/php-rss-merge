<?php

declare(strict_types=1);

namespace RSSMerger;

use Cache\Adapter\Common\CacheItem;

/**
 * Class InternalCache
 */
class InternalCache
{
    /**
     * ID of cache
     *
     * @var string
     */
    public string $id;

    /**
     * Expires time
     *
     * @var int
     */
    public int $ttl;


    /**
     * @param string $id  ID of cache.
     * @param int    $ttl Expires time.
     */
    public function __construct(string $id, int $ttl = 3600)
    {
        $this->id  = $id;
        $this->ttl = $ttl;
    }


    /**
     * Check cache
     */
    public function checkCache() : bool
    {
        /**
         * @var CacheItem $item
         */
        $item = $GLOBALS['cache']->getItem($this->id);
        if ($GLOBALS['cache']->hasItem($this->id) === true && $item->isHit() === true) {
            return true;
        }

        return false;
    }


    /**
     * Set Cache
     *
     * @param string $string Data.
     */
    public function setCache(string $string) : void
    {
        /**
         * @var CacheItem $item
         */
        $item = $GLOBALS['cache']->getItem($this->id);
        $item->set($string);
        $item->expiresAfter($this->ttl);
        $GLOBALS['cache']->save($item);
    }


    /**
     * Get Cache
     *
     * @return string|mixed|void|null
     */
    public function getCache()
    {
        /**
         * @var CacheItem $item
         */
        $item = $GLOBALS['cache']->getItem($this->id);
        return $item->get();
    }
}
