<?php

namespace RSSMerger;

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
    public $id;

    /**
     * Expires time
     *
     * @var integer
     */
    public $ttl;


    /**
     * InternalCache constructor.
     *
     * @param string  $id  ID of cache.
     * @param integer $ttl Expires time.
     */
    public function __construct(string $id, int $ttl=3600)
    {
        $this->id  = $id;
        $this->ttl = $ttl;

    }//end __construct()


    /**
     * Check cache
     *
     * @return boolean
     */
    public function checkCache()
    {
        $item = $GLOBALS['cache']->getItem($this->id);
        if ($GLOBALS['cache']->hasItem($this->id) === true && $item->isHit() === true) {
            return true;
        } else {
            return false;
        }

    }//end checkCache()


    /**
     * Set Cache
     *
     * @param string $string Data.
     *
     * @return void
     */
    public function setCache(string $string)
    {
        $item = $GLOBALS['cache']->getItem($this->id);
        $item->set($string);
        $item->expiresAfter($this->ttl);
        $GLOBALS['cache']->save($item);

    }//end setCache()


    /**
     * Get Cache
     *
     * @return mixed|void|null
     */
    public function getCache()
    {
        $item = $GLOBALS['cache']->getItem($this->id);
        return $item->get();

    }//end getCache()


}//end class