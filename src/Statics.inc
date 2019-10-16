<?php

namespace RSSMerger;

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
        return self::$generatorName.' '.self::$generatorVersion;

    }//end GENERATOR()


    /**
     * Return current directory url
     *
     * @return string
     */
    public static function currentDirectoryUrl() : string
    {
        $path = str_replace($_SERVER['DOCUMENT_ROOT'], '', realpath(__DIR__.'/../'));
        if (strpos($path, 0) !== '/') {
            $path = '/'.$path;
        }

        return self::requestType().'://'.$_SERVER['HTTP_HOST'].str_replace('\\', '/', $path);

    }//end currentDirectoryUrl()


    /**
     * Get request type
     *
     * @return string
     */
    private static function requestType()
    {
        if (isset($_SERVER['HTTPS']) === true && ($_SERVER['HTTPS'] === 'on')) {
            return 'https';
        } else {
            return 'http';
        }

    }//end requestType()


    /**
     * Set Location to redirect
     *
     * @param string $location Location to redirect.
     *
     * @return void
     */
    public function setLocation(?string $location)
    {
        if ($location === null) {
            header('Location: '.self::currentDirectoryUrl());
        } else {
            header('Location: '.$location);
        }

    }//end setLocation()


    /**
     * Process of before render feeds
     *
     * @param RSS[] $rss RSS array.
     *
     * @return array
     */
    public static function beforeRenderFeeds(array $rss)
    {
        foreach ($rss as &$f) {
            $feed             = (array) $f;
            $feed['feeds']    = join('<br />', $f->feeds);
            $feed['feed_url'] = self::currentDirectoryUrl().'feed.php?feed='.$f->id;
            $f                = $feed;
        }

        return $rss;

    }//end beforeRenderFeeds()


}//end class
