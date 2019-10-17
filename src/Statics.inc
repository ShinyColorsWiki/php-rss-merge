<?php

declare(strict_types=1);

namespace RSSMerger;

use Closure;
use SimpleXMLElement;
use function header;
use function implode;
use function realpath;
use function str_replace;
use function strpos;

/**
 * Class Statics
 */
class Statics
{
    /**
     * Program name
     *
     * @var string
     */
    protected static $generatorName = 'PHP RSS Merger';

    /**
     * Program version
     *
     * @var string
     */
    protected static $generatorVersion = '0.0.1-dev';


    /**
     * Return Program name and version.
     *
     * @return string
     */
    public static function GENERATOR() : string
    {
        return self::$generatorName . ' ' . self::$generatorVersion;
    }


    /**
     * Return current directory url
     *
     * @return string
     */
    public static function currentDirectoryUrl() : string
    {
        $path = str_replace($_SERVER['DOCUMENT_ROOT'], '', realpath(__DIR__ . '/../'));

        return self::requestType() . '://' . $_SERVER['HTTP_HOST'] . str_replace('\\', '/', $path) . '/';
    }


    /**
     * Get request type
     *
     * @return string
     */
    private static function requestType() : string
    {
        if (isset($_SERVER['HTTPS']) === true && ($_SERVER['HTTPS'] === 'on')) {
            return 'https';
        }

        return 'http';
    }


    /**
     * Set Location to redirect
     *
     * @param string $location Location to redirect.
     */
    public function setLocation(?string $location) : void
    {
        if ($location === null) {
            header('Location: ' . self::currentDirectoryUrl());
        } else {
            header('Location: ' . $location);
        }
    }

    public static function getFeedUrl(string $id) : string
    {
        return self::currentDirectoryUrl() . 'feed.php?feed=' . $id;
    }

    /**
     * Process of before render feeds
     *
     * @param RSS[] $rss RSS array.
     *
     * @return object[]
     */
    public static function beforeRenderFeeds(array $rss) : array
    {
        foreach ($rss as &$f) {
            $feed             = (array) $f;
            $feed['feeds']    = implode('<br />', $f->feeds);
            $feed['feed_url'] = self::getFeedUrl($f->id);
            $f                = $feed;
        }

        return $rss;
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
    public static function getNsChild(SimpleXMLElement $xml, string $namespace, string $nodeName) : SimpleXMLElement
    {
        $ns = $xml->getNamespaces(true);
        return $xml->children($ns[$namespace])->{$nodeName};
    }

    /**
     * Sort by provided key
     *
     * @param string $key Key name.
     *
     * @return Closure
     */
    public static function sortBy(string $key) : Closure
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
}
