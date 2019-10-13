<?php
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

class InternalCache
{

    public $id;
    public $ttl;

    public function __construct(string $id, int $ttl = 3600)
    {
        $this->id = $id;
        $this->ttl = $ttl;
    }

    public function checkCache()
    {
        global $cache;
        $item = $cache->getItem($this->id);
        return $cache->hasItem($this->id) && $item->isHit();
    }

    public function setCache(string $string)
    {
        global $cache;
        $item = $cache->getItem($this->id);
        $item->set($string);
        $item->expiresAfter($this->ttl);
        $cache->save($item);
    }

    public function getCache()
    {
        global $cache;
        $item = $cache->getItem($this->id);
        return $item->get();
    }
}


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

    public $cache;

    public function __construct(RSS $rss)
    {
        global $cache;
        $this->rss = $rss;
        $this->cache = new InternalCache($rss->id);
    }

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

class Builder
{
    public $lists;

    public function __construct(array $rss_array)
    {
        $lists = array();
        foreach ($rss_array as $rss) {
            $lists[$rss->id] = new Merger($rss);
        }
        $this->lists = $lists;
    }

    public function generateOutput(string $id)
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

        $result = join("\n", [
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
        ]);

        $cache->setCache($result);
        return $result;
    }
}
