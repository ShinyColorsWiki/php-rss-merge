<?php
/** @noinspection PhpUndefinedMethodInspection */
/** @noinspection PhpUndefinedFieldInspection */

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

/**
 * Class Builder
 */
class Builder
{
    /**
     * @var array
     */
    public $lists;

    /**
     * Builder constructor.
     * @param array $rss_array
     */
    public function __construct(array $rss_array)
    {
        $lists = array();
        foreach ($rss_array as $rss) {
            $lists[$rss->id] = new Merger($rss);
        }
        $this->lists = $lists;
    }

    /**
     * @param string $id
     * @return string
     */
    public function generateOutput($id)
    {
        $merger = $this->lists[$id];
        $rss = $merger->rss;
        $cache = $merger->cache;

        if ($cache->checkCache() && $_GET['force'] != true) {
            return $cache->getCache();
        }

        $feeds = $merger->getFeeds();

        $items = array();
        foreach ($feeds as $item) {
            $items[] = join("\n", [
                "\t\t" . '<item>',
                "\t\t\t" . '<title>' . $item['title'] . '</title>',
                "\t\t\t" . '<link>' . $item['link'] . '</link>',
                "\t\t\t" . '<description><![CDATA[' . $item['description'] . ']]></description>',
                "\t\t\t" . '<pubDate>' . $item['pubDate'] . '</pubDate>',
                "\t\t" . '</item>' . "\n\n"
            ]);
        }

        $result = join("\n", array(
            '<?xml version="1.0" encoding="' . $rss->encoding . '"?>',
            '<rss version="2.0">',
            "\t" . '<channel>',
            "\t\t" . '<title>' . $rss->title . '</title>',
            "\t\t" . '<link>' . $rss->link . '</link>',
            "\t\t" . '<description>' . $rss->description . '</description>',
            "\t\t" . '<pubDate>' . date(DATE_RFC822) . '</pubDate>',
            "\t\t" . '<ttl>' . $rss->ttl . '</ttl>',
            "\t\t" . '<generator>PHP RSS Merge</generator>',
            "\t\t" . '<language>' . $rss->lang . '</language>',
            join("\n", $items),
            "\t" . '</channel>',
            '</rss>'
        ));

        $cache->setCache($result);
        return $result;
    }
}
