<?php

namespace RSSMerger;

/**
 * Class RSS
 */
class RSS
{
    /**
     * An identifier
     * @var string
     */
    public $id;

    /**
     * A site title
     * @var string
     */
    public $title;

    /**
     * A site link
     * @var string
     */
    public $link;

    /**
     * A site description
     * @var string
     */
    public $description;

    /**
     * Links of feed
     * @var array
     */
    public $feeds;

    /**
     * A time to live value
     * @var int
     */
    public $ttl;

    /**
     * A xml encoding
     * @var string
     */
    public $encoding;

    /**
     * A language code
     * @var string
     */
    public $lang;


    /**
     * RSS constructor.
     * @param $id
     * @param $title
     * @param $link
     * @param $description
     * @param $feeds
     * @param int $ttl
     * @param string $encoding
     * @param string $lang
     */
    public function __construct(
        $id,
        $title,
        $link,
        $description,
        $feeds,
        $ttl = 3600,
        $encoding = 'UTF-8',
        $lang = 'en'
    ) {
        $this->id = $id;
        $this->title = $title;
        $this->link = $link;
        $this->description = $description;
        $this->feeds = $feeds;
        $this->ttl = $ttl;
        $this->encoding = $encoding;
        $this->lang = $lang;
    }
}
