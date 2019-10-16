<?php

namespace RSSMerger;

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
     * @var integer
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
     * RSS constructor.
     *
     * @param string   $id          ID of RSS.
     * @param string   $title       RSS Title.
     * @param string   $link        RSS Link.
     * @param string   $description RSS Description.
     * @param string[] $feeds       RSS Feed List.
     * @param integer  $ttl         Time-to-live.
     * @param string   $encoding    Encoding of XML.
     * @param string   $lang        Language of RSS.
     */
    public function __construct(
        string $id,
        string $title,
        string $link,
        string $description,
        array $feeds,
        int $ttl=3600,
        string $encoding='UTF-8',
        string $lang='en'
    ) {
        $this->id          = $id;
        $this->title       = $title;
        $this->link        = $link;
        $this->description = $description;
        $this->feeds       = $feeds;
        $this->ttl         = $ttl;
        $this->encoding    = $encoding;
        $this->lang        = $lang;

    }//end __construct()


}//end class
