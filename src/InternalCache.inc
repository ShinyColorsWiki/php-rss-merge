<?php

namespace RSSMerger;

/**
 * Class InternalCache
 */
class InternalCache
{

    /**
     * @var string
     */
    public $id;
    /**
     * @var int
     */
    public $ttl;

    /**
     * InternalCache constructor.
     * @param string $id
     * @param int $ttl
     */
    public function __construct($id, $ttl = 3600)
    {
        $this->id = $id;
        $this->ttl = $ttl;
    }

    /**
     * @return bool
     * @throws \Psr\Cache\InvalidArgumentException
     */
    public function checkCache()
    {
        global $cache;
        $item = $cache->getItem($this->id);
        return $cache->hasItem($this->id) && $item->isHit();
    }

    /**
     * @param string $string
     * @throws \Psr\Cache\InvalidArgumentException
     */
    public function setCache($string)
    {
        global $cache;
        $item = $cache->getItem($this->id);
        $item->set($string);
        $item->expiresAfter($this->ttl);
        $cache->save($item);
    }

    /**
     * @return mixed|void|null
     * @throws \Psr\Cache\InvalidArgumentException
     */
    public function getCache()
    {
        global $cache;
        $item = $cache->getItem($this->id);
        return $item->get();
    }
}
